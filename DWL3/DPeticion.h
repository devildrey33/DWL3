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

	// Objeto que controla una única petición con su propio thread
	class DPeticion {
   	  public:
		// Posibles errores durante el proceso de descarga
		enum Errores {
			Error_Servidor		= 0,	// No va el servidor, o no hay internet
			Error_ObteniendoTam = 1,	// Error al intentar obtener el tamaño del archivo
			Error_Memoria		= 2,	// Error intentado reservar la memória necesaria para el archivo (puede que sea muy grande)
			Error_Descarga		= 3		// Error durante la descarga...
		};

											// Constructores
											DPeticion(const wchar_t *nUrl, HWND hWndMensajes, const UINT nID);
											DPeticion(std::wstring &nURL, HWND hWndMensajes, const UINT nID);
											// Destructor
										   ~DPeticion(void);
											// Función que permite asignar el estado cancelado tanto para la busqueda, como para la descarga
		void								Cancelar(const BOOL nCancelar);
											// Función que obtiene el estado cancelado para la busqueda y la descarga
		const BOOL							Cancelar(void);
											// Función que devuelve la ID de la petición
		const UINT							ID(void);
											// Función que devuelve los datos
		const char		     	           *Datos(void);
											// Función que devuelve la URL de la petición
		std::wstring			           &Url(void);
	  private:
											// Función que inicia el thread para obtener el archivo
		void			                   _IniciarThread(void);
											// Punto de inició del thread
		static unsigned long               _Thread(DPeticion *pThis);
											// URL a descargar
		std::wstring                       _Url;
											// Identificador del thread
		HANDLE                             _hThread;
											// HWND de la ventana de mensajes del thread principal
		HWND                               _VentanaMensajes;
											// Archivo descargado en memória
		char    				          *_Datos;
											// Variable que determina si hay que cancelar la petición
		BOOL                               _Cancelar;
											// Mutex para la variable cancelar
		std::mutex                         _Mutex;
											// ID del thread
		UINT                               _ID;
											// Función lambda para quando se termina la petición
		std::function<void(DPeticion &)>  _LambdaTerminado;
											// Función lambda para quando se hay un error en la petición
		std::function<void(const UINT)>    _LambdaError;
											// Función lambda para informar sobre el progreso (de 0.0 a 1.0)
		std::function<void(const float)>   _LambdaPorcentaje;

		friend class DArchivoInternet;
		friend class DApp;
	};
};

#endif