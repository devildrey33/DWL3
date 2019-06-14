#ifndef DTEMPORIZADOR_H
#define DTEMPORIZADOR_H

#include <vector>
#include <functional>

namespace DWL {

	// Clase para controlar multiples temporizadores (se puede heredar y re-emplazar la función virtual Evento_Temporizador, o se puede utilizar una función lambda como callback)
	class DTemporizador {
   	  public:
		// Objeto que engloba un unico temporizador
		class DTemporizador_Unico {
   		  public:
											DTemporizador_Unico(const DTemporizador_Unico &Temporizador) : ID(Temporizador.ID), Timer(Temporizador.Timer), Padre(Temporizador.Padre), UnaVez(FALSE) { };
											DTemporizador_Unico(const UINT_PTR nID, DTemporizador *nPadre, BOOL nUnaVez) : ID(nID), Timer(0), Padre(nPadre), UnaVez(nUnaVez) { };
											DTemporizador_Unico(const UINT_PTR nID, DTemporizador *nPadre, BOOL nUnaVez, std::function<void(void)> nCallback) : ID(nID), Timer(0), Padre(nPadre), UnaVez(nUnaVez), Callback(nCallback) { };
									       ~DTemporizador_Unico(void) { };
			DTemporizador                  *Padre;
			UINT_PTR						ID;
			HANDLE							Timer;
			BOOL							UnaVez;
			std::function<void(void)>		Callback;
		};

											DTemporizador(void);
								           ~DTemporizador(void);
		void								CrearTemporizador(const UINT_PTR nID, const DWORD Milisegundos, const BOOL UnaVez = FALSE);
		void                                CrearTemporizador(const DWORD Milisegundos, const BOOL UnaVez, std::function<void(void)> Callback);
		void								TerminarTemporizador(const UINT_PTR nID);
		void								TerminarTemporizadores(void);
		virtual void						Evento_Temporizador(const UINT_PTR cID) { };
	  private:
		const BOOL                         _BuscarTemporizadorPos(const UINT_PTR cID, size_t &cPos);
		static void CALLBACK               _TimerProc(PVOID lpParameter, BOOLEAN TimerOrWaitFired);
		static void CALLBACK               _TimerProcLambda(PVOID lpParameter, BOOLEAN TimerOrWaitFired);
		std::vector<DTemporizador_Unico *> _Temporizadores;
		
	};

}
#endif