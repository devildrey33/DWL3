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
   		  public: //////////////////////////// Constructor copia
											DTemporizador_Unico(const DTemporizador_Unico &Temporizador) : ID(Temporizador.ID), Timer(Temporizador.Timer), Padre(Temporizador.Padre), UnaVez(FALSE) { };
											// Constructor que crea un temporizador
											DTemporizador_Unico(const UINT_PTR nID, DTemporizador *nPadre, BOOL nUnaVez) : ID(nID), Timer(0), Padre(nPadre), UnaVez(nUnaVez) { };
											// Constructor que crea un temporizador con un callback lambda
											DTemporizador_Unico(const UINT_PTR nID, DTemporizador *nPadre, BOOL nUnaVez, std::function<void(void)> nCallback) : ID(nID), Timer(0), Padre(nPadre), UnaVez(nUnaVez), Callback(nCallback) { };
											// Destructor
									       ~DTemporizador_Unico(void) { };
										    // Objeto padre
			DTemporizador                  *Padre;
											// ID del temporizador
			UINT_PTR						ID;
											// Identificador de windows para el temporizador
			HANDLE							Timer;
											// Variable que determina si el temporizador es de un solo uso, o se repite
			BOOL							UnaVez;
											// Callback para el temporizador
			std::function<void(void)>		Callback;
		}; ///////////////////////////////////
											// Constructor
											DTemporizador(void);
											// Destructor
								           ~DTemporizador(void);
											// Función que crea un temporizador
		void								CrearTemporizador(const UINT_PTR nID, const DWORD Milisegundos, const BOOL UnaVez = FALSE);
											// Función que crea un temporizador que usa un callback lambda
		void                                CrearTemporizador(const DWORD Milisegundos, const BOOL UnaVez, std::function<void(void)> Callback);
											// Función que termina un temporizador
		void								TerminarTemporizador(const UINT_PTR nID);
											// Función que termina todos los temporizadores creados con este objeto
		void								TerminarTemporizadores(void);
											// Evento virtual para el callback del temporizador (NO SE USA EN TEMPORIZADORES LAMBDA)
		virtual void						Evento_Temporizador(const UINT_PTR cID) { };
	  private: /////////////////////////////// Función que busca un temporizador por su ID
		const BOOL                         _BuscarTemporizadorPos(const UINT_PTR cID, size_t &cPos);
											// Callback para temporizadores normales
		static void CALLBACK               _TimerProc(PVOID lpParameter, BOOLEAN TimerOrWaitFired);
											// Callback para temporizadores con función lambda
		static void CALLBACK               _TimerProcLambda(PVOID lpParameter, BOOLEAN TimerOrWaitFired);
											// Vector de temporizadores
		std::vector<DTemporizador_Unico *> _Temporizadores;
		
	};

}
#endif