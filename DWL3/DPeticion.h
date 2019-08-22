#ifndef DPETICION_H
#define DPETICION_H

#include <functional>
#include <vector>
#include <string>
#include <mutex>
#include "DhWnd.h"

namespace DWL {

	class DApp;
	class DArchivoInternet;

	// Objeto que controla una �nica petici�n con su propio thread
	class DPeticion {
   	  public:
		// Posibles errores durante el proceso de descarga
		enum Errores {
			Error_Servidor		= 0,	// No va el servidor, o no hay internet
			Error_ObteniendoTam = 1,	// Error al intentar obtener el tama�o del archivo
			Error_Memoria		= 2,	// Error intentado reservar la mem�ria necesaria para el archivo (puede que sea muy grande)
			Error_Descarga		= 3		// Error durante la descarga...
		};

											// Constructores
											DPeticion(const wchar_t *nUrl, HWND hWndMensajes, const UINT nID);
											DPeticion(std::wstring &nURL, HWND hWndMensajes, const UINT nID);
											// Destructor
										   ~DPeticion(void);
											// Funci�n que permite asignar el estado cancelado tanto para la busqueda, como para la descarga
		void								Cancelar(const BOOL nCancelar);
											// Funci�n que obtiene el estado cancelado para la busqueda y la descarga
		const BOOL							Cancelar(void);
											// Funci�n que devuelve la ID de la petici�n
		const UINT							ID(void);
											// Funci�n que devuelve los datos
		const char		     	           *Datos(void);
											// Funci�n que devuelve la URL de la petici�n
		std::wstring			           &Url(void);
	  private:
											// Funci�n que inicia el thread para obtener el archivo
		void			                   _IniciarThread(void);
											// Punto de inici� del thread
		static unsigned long               _Thread(DPeticion *pThis);
											// URL a descargar
		std::wstring                       _Url;
											// Identificador del thread
		HANDLE                             _hThread;
											// HWND de la ventana de mensajes del thread principal
		HWND                               _VentanaMensajes;
											// Archivo descargado en mem�ria
		char    				          *_Datos;
											// Variable que determina si hay que cancelar la petici�n
		BOOL                               _Cancelar;
											// Mutex para la variable cancelar
		std::mutex                         _Mutex;
											// ID del thread
		UINT                               _ID;
											// Funci�n lambda para quando se termina la petici�n
		std::function<void(DPeticion &)>  _LambdaTerminado;
											// Funci�n lambda para quando se hay un error en la petici�n
		std::function<void(const UINT)>    _LambdaError;
											// Funci�n lambda para informar sobre el progreso (de 0.0 a 1.0)
		std::function<void(const float)>   _LambdaPorcentaje;

		friend class DArchivoInternet;
		friend class DApp;
	};
};

#endif