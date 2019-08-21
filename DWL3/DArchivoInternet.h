#ifndef DARCHIVOINTERNET_H
#define DARCHIVOINTERNET_H

#include <functional>
#include <vector>
#include <string>
#include <mutex>
#include "DhWnd.h"

#pragma comment(lib, "wininet.lib")	// Agrego la libreria wininet.lib al proyecto

namespace DWL {

	class DApp;

	// Clase est�tica para descargar archivos de internet multithread
	// Dise�ada para archivos de texto ligeros
	class DArchivoInternet	{
	  private:
		// Objeto que controla una �nica petici�n con su propio thread
		class Peticion {
		   public:
												// Constructores
												Peticion(const wchar_t *nUrl, HWND hWndMensajes, const UINT nID);
												Peticion(std::wstring &nURL, HWND hWndMensajes, const UINT nID);
												// Destructor
											   ~Peticion(void);
												// Funci�n que permite asignar el estado cancelado tanto para la busqueda, como para la descarga
			void								Cancelar(const BOOL nCancelar);
												// Funci�n que obtiene el estado cancelado para la busqueda y la descarga
			const BOOL							Cancelar(void);
												// Funci�n que devuelve la ID de la petici�n
			const UINT							ID(void);
												// Funci�n que devuelve los datos
			inline const char    	           *Datos(void) { return _Datos; };
		   private:
												// Funci�n que inicia el thread para obtener el archivo
			 void			                   _IniciarThread(void);
												// Punto de inici� del thread
			 static unsigned long              _Thread(Peticion *pThis);
												// URL a descargar
			 std::wstring                      _Url;
												// Identificador del thread
			 HANDLE                            _hThread;
												// HWND de la ventana de mensajes del thread principal
			 HWND                              _VentanaMensajes;
												// Archivo descargado en mem�ria
			 char    				          *_Datos;
												// Variable que determina si hay que cancelar la petici�n
			 BOOL                              _Cancelar;
												// Mutex para la variable cancelar
			 std::mutex                        _Mutex;
												// ID del thread
			 UINT                              _ID;
												// Funci�n lambda para quando se termina la petici�n
			 std::function<void(const char *)> _LambdaTerminado;
												// Funci�n lambda para quando se hay un error en la petici�n
			 std::function<void(const UINT)>   _LambdaError;
												// Funci�n lambda para informar sobre el progreso (de 0.0 a 1.0)
			 std::function<void(const float)>  _LambdaPorcentaje;

			 friend class DArchivoInternet;
			 friend class DApp;
		};

	public:
		// Posibles errores durante el proceso de descarga
		enum Errores {
			Error_Servidor		= 0,	// No va el servidor, o no hay internet
			Error_ObteniendoTam = 1,	// Error al intentar obtener el tama�o del archivo
			Error_Memoria		= 2,	// Error intentado reservar la mem�ria necesaria para el archivo (puede que sea muy grande)
			Error_Descarga		= 3		// Error durante la descarga...
		};

										DArchivoInternet(void) { };
										// Funci�n que inicia una petici�n, y devuelve su ID
		static const UINT				Obtener(const wchar_t	 		           *nUrl,												// Url a descargar
												std::function<void(const char *)>	LambdaTerminado,   /* [](const char *) { } */		// Funci�n lambda para cuando se ha terminado de descargar
												std::function<void(const UINT)>		LambdaError			= [](const UINT)   { },			// Funci�n lambda por si sucede un error
												std::function<void(const float)>	LambdaPorcentaje	= [](const float)  { }		);	// Funci�n lambda que nos dice el porcentaje de descarga
										// Funci�n que inicia una petici�n, y devuelve su ID
		static const UINT				Obtener(std::wstring	 			       &nUrl,												// Url a descargar
												std::function<void(const char *)>	LambdaTerminado,   /* [](const char *) { } */		// Funci�n lambda para cuando se ha terminado de descargar
												std::function<void(const UINT)>		LambdaError			= [](const UINT)   { },			// Funci�n lambda por si sucede un error
												std::function<void(const float)>	LambdaPorcentaje	= [](const float)  { }		);	// Funci�n lambda que nos dice el porcentaje de descarga
										// Funci�n para cancelar una petici�n
		static void						Cancelar(const UINT cID);
	  private:
										// Funci�n para buscar una petici�n por su ID
		static Peticion               *_Buscar(const UINT bID);
										// Vector de peticiones
		static std::vector<Peticion *> _Peticiones;
										// Contador para ir repartiendo ID's unicas a cada petici�n
		static UINT					   _ID;
										// Funci�n para eliminar una peticion
		static void					   _EliminarPeticion(const UINT nID);

		friend class DApp;
	};

};


#endif
