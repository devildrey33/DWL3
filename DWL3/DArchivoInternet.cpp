#include "pch.h"
#include "DArchivoInternet.h"
#include "DApp.h"
#include "DMensajesWnd.h"
#include <wininet.h>

namespace DWL {

	DArchivoInternet::Peticion::Peticion(const wchar_t *nURL, HWND hWndMensajes, const UINT nID) : _hThread(NULL), _VentanaMensajes(hWndMensajes), _Url(nURL), _Cancelar(FALSE), _Datos(nullptr), _ID(nID) {
		_IniciarThread();
	}

	DArchivoInternet::Peticion::Peticion(std::wstring &nURL, HWND hWndMensajes, const UINT nID) : _hThread(NULL), _VentanaMensajes(hWndMensajes), _Url(nURL), _Cancelar(FALSE), _Datos(nullptr), _ID(nID) {
		_IniciarThread();
	}

	DArchivoInternet::Peticion::~Peticion(void) {
		if (_Datos != nullptr) delete[] _Datos;
		_Datos = nullptr;
	}

	// Función que permite asignar el estado cancelado tanto para la busqueda, como para la descarga
	void DArchivoInternet::Peticion::Cancelar(const BOOL nCancelar) {
		_Mutex.lock();
		_Cancelar = nCancelar;
		_Mutex.unlock();
	}

	// Función que obtiene el estado cancelado para la busqueda y la descarga
	const BOOL DArchivoInternet::Peticion::Cancelar(void) {
		_Mutex.lock();
		BOOL Ret = _Cancelar;
		_Mutex.unlock();
		return Ret;
	}

	// Función que obtiene el estado cancelado para la busqueda y la descarga
	const UINT DArchivoInternet::Peticion::ID(void) {
		_Mutex.lock();
		UINT Ret = _ID;
		_Mutex.unlock();
		return Ret;
	}


	// Función que inicia el thread para obtener el archivo
	void DArchivoInternet::Peticion::_IniciarThread(void) {
		// Inicio el thread
		_hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)this->_Thread, (void*)this, 0, NULL);

		// Si hay un error, lo muestro por la consola
		if (_hThread == NULL) {
			Debug_Escribir(L"DArchivoInternet::Peticion::_IniciarThread ERROR creando el thread.\n");
		}
	}

	// Thread estatico
	unsigned long DArchivoInternet::Peticion::_Thread(Peticion *This) {
		TCHAR			szHead[]	= L"Accept: */*\r\n\r\n";
		HINTERNET		Sesion		= InternetOpen(L"RAVE_Actualizacion", INTERNET_OPEN_TYPE_PRECONFIG, NULL, INTERNET_INVALID_PORT_NUMBER, 0);
		HINTERNET		Peticion	= InternetOpenUrl(Sesion, This->_Url.c_str(), szHead, 0, INTERNET_FLAG_RELOAD, 0);
		wchar_t			TamStr[64]	= L"";
		DWORD           Descargado	= 64; // HttpQueryInfo pide el tamaño del buffer, y luego devuelve los bytes descargados en la misma variable.
		DWORD			Tam         = 0;

		// Error en la petición (probablemente no va el servidor, o no hay internet)
		if (Peticion == NULL) {
			InternetCloseHandle(Sesion);
			// Informo al thread principal de que se ha salido con error
			PostMessage(This->_VentanaMensajes, DWL_INTERNET_ERROR, Errores::Error_Servidor, This->ID());
			return -1;
		}

		// Obtengo el tamaño del archivo
		BOOL Ret = HttpQueryInfo(Peticion, HTTP_QUERY_CONTENT_LENGTH, (LPVOID)TamStr, &Descargado, (LPDWORD)0);

		// Si no hay error convierto el string del número a DWORD
		if (Ret == TRUE) Tam = _wtol(TamStr);

		// Error obteniendo el tamaño del archivo
		if (Tam == 0) {
			InternetCloseHandle(Peticion);
			InternetCloseHandle(Sesion);
			// Informo al thread principal de que se ha salido con error
			PostMessage(This->_VentanaMensajes, DWL_INTERNET_ERROR, Errores::Error_ObteniendoTam, This->ID());
			return -1;
		}
		
		char	    Descarga[4097]	= "";
		DWORD		TotalDescargado = 0;
		BOOL        P               = FALSE;
		float		Valor			= 0.0f;
		// Intento crear la memória para almacenar todo el archivo
		try {
			This->_Datos = new char[Tam];
		}
		// no hay suficiente memória
		catch (...) {
			PostMessage(This->_VentanaMensajes, DWL_INTERNET_ERROR, Errores::Error_Memoria, This->ID());
			return -1;
		}

		while (TRUE) {
			// Calculo el porcentaje de la descarga
			Valor = static_cast<float>(TotalDescargado) / static_cast<float>(Tam);
			// Informo al thread principal que se ha terminado
			SendMessage(This->_VentanaMensajes, DWL_INTERNET_PORCENTAJE, reinterpret_cast<WPARAM&>(Valor), This->ID());


			P = InternetReadFile(Peticion, (LPVOID)Descarga, 4096, &Descargado);
			// Error al descargar...
			if (P == FALSE) {
				Debug_MostrarUltimoError();
				InternetCloseHandle(Peticion);
				InternetCloseHandle(Sesion);
				// Informo al thread principal de que se ha salido con error
				PostMessage(This->_VentanaMensajes, DWL_INTERNET_ERROR, Errores::Error_Descarga, This->ID());
				return -1;
			}

			// Compruebo si se ha cancelado
			if (This->Cancelar() == TRUE) 	break;			
			
			// No se han leido datos, o no quedan datos por leer, salgo.
			if (Descargado == 0)			break;
			
			// Guardo los datos leidos
			CopyMemory(&This->_Datos[TotalDescargado], Descarga, Descargado);

			// Sumo los datos descargados
			TotalDescargado += Descargado;
		}

		InternetCloseHandle(Peticion);
		InternetCloseHandle(Sesion);

		// Informo al thread principal del resultado
		SendMessage(This->_VentanaMensajes, DWL_INTERNET_TERMINADO, PtrToLong(This->_Datos), This->ID());

		// Informo al thread principal que ya se puede borrar esta petición
		PostMessage(This->_VentanaMensajes, DWL_INTERNET_ELIMINAR, 0, This->ID());

		return 0;
	}













	// Vector de peticiones pendiente estatico
	std::vector<DArchivoInternet::Peticion *>	DArchivoInternet::_Peticiones;
	// ID unica para cada petición que se va incrementando
	UINT										DArchivoInternet::_ID			= 0;

	const UINT DArchivoInternet::Obtener(	const wchar_t	 		           *nUrl,												// Url a descargar
											std::function<void(const char *)>	LambdaTerminado, /*   [](const char *) { } */		// Función lambda para cuando se ha terminado de descargar
											std::function<void(const UINT)>		LambdaError		 /* = [](const UINT)   { } */,		// Función lambda por si sucede un error
											std::function<void(const float)>	LambdaPorcentaje /* = [](const float)  { } */) {	// Función lambda que nos dice el porcentaje de descarga

		// Creo la petición
		Peticion* nPeticion = new Peticion(nUrl, _Aplicacion->hWndMensajes(), _ID++);

		// Asigno las funciones lambda (no se utilizan en ningún momento dentro del thread)
		nPeticion->_LambdaTerminado		= LambdaTerminado;
		nPeticion->_LambdaError			= LambdaError;
		nPeticion->_LambdaPorcentaje	= LambdaPorcentaje;

		// Guardo la petición en el vector de peticiones pendientes
		_Peticiones.push_back(nPeticion);

		// Devuelvo la ID
		return _ID -1;
	}


	const UINT DArchivoInternet::Obtener(	std::wstring 	 		           &nUrl,												// Url a descargar
											std::function<void(const char *)>	LambdaTerminado, /*   [](const char *) { } */		// Función lambda para cuando se ha terminado de descargar
											std::function<void(const UINT)>		LambdaError		 /* = [](const UINT)   { } */,		// Función lambda por si sucede un error
											std::function<void(const float)>	LambdaPorcentaje /* = [](const float)  { } */) {	// Función lambda que nos dice el porcentaje de descarga

		// Creo la petición
		Peticion* nPeticion = new Peticion(nUrl, _Aplicacion->hWndMensajes(), _ID++);

		// Asigno las funciones lambda (no se utilizan en ningún momento dentro del thread)
		nPeticion->_LambdaTerminado		= LambdaTerminado;
		nPeticion->_LambdaError			= LambdaError;
		nPeticion->_LambdaPorcentaje	= LambdaPorcentaje;

		// Guardo la petición en el vector de peticiones pendientes
		_Peticiones.push_back(nPeticion);

		// Devuelvo la ID
		return _ID -1;
	}


	void DArchivoInternet::Cancelar(const UINT cID) {
		Peticion *P = _Buscar(cID);
		if (P != NULL) P->Cancelar(TRUE);
	}


	DArchivoInternet::Peticion *DArchivoInternet::_Buscar(const UINT bID) {
		for (size_t i = 0; i < _Peticiones.size(); i++) {
			if (_Peticiones[i]->ID() == bID) {
				return _Peticiones[i];
			}
		}
		return nullptr;
	}

	void DArchivoInternet::_EliminarPeticion(const UINT nID) {
		for (size_t i = 0; i < _Peticiones.size(); i++) {
			if (_Peticiones[i]->ID() == nID) {
				// Elimino la petición de la memória
				delete _Peticiones[i];
				// Elimino la petición del vector de peticiones pendientes
				_Peticiones.erase(_Peticiones.begin() + i);
				return;
			}
		}
	}

}