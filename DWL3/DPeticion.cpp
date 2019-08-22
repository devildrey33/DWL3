#include "pch.h"
#include "DPeticion.h"
#include "DApp.h"
#include "DMensajesWnd.h"
#include <wininet.h>

namespace DWL {

	DPeticion::DPeticion(const wchar_t* nURL, HWND hWndMensajes, const UINT nID) : _hThread(NULL), _VentanaMensajes(hWndMensajes), _Url(nURL), _Cancelar(FALSE), _Datos(nullptr), _ID(nID) {
		_IniciarThread();
	}

	DPeticion::DPeticion(std::wstring& nURL, HWND hWndMensajes, const UINT nID) : _hThread(NULL), _VentanaMensajes(hWndMensajes), _Url(nURL), _Cancelar(FALSE), _Datos(nullptr), _ID(nID) {
		_IniciarThread();
	}

	DPeticion::~DPeticion(void) {
		if (_Datos != nullptr) delete[] _Datos;
		_Datos = nullptr;
	}

	// Funci�n que permite asignar el estado cancelado tanto para la busqueda, como para la descarga
	void DPeticion::Cancelar(const BOOL nCancelar) {
		_Mutex.lock();
		_Cancelar = nCancelar;
		_Mutex.unlock();
	}

	// Funci�n que obtiene el estado cancelado para la busqueda y la descarga
	const BOOL DPeticion::Cancelar(void) {
		_Mutex.lock();
		BOOL Ret = _Cancelar;
		_Mutex.unlock();
		return Ret;
	}

	// Funci�n que devuelve la ID unica de esta petici�n
	const UINT DPeticion::ID(void) {
		_Mutex.lock();
		UINT Ret = _ID;
		_Mutex.unlock();
		return Ret;
	}

	// Funci�n que devuelve la URL de la petici�n
	std::wstring &DPeticion::Url(void) {
		static std::wstring Ret;
		_Mutex.lock();
		Ret = _Url;
		_Mutex.unlock();
		return Ret;
	}

	// Funci�n que devuelve los datos descargados
	const char *DPeticion::Datos(void) {
		return _Datos;
	}


	// Funci�n que inicia el thread para obtener el archivo
	void DPeticion::_IniciarThread(void) {
		// Inicio el thread
		_hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)this->_Thread, (void*)this, 0, NULL);

		// Si hay un error, lo muestro por la consola
		if (_hThread == NULL) {
			Debug_Escribir(L"Peticion::_IniciarThread ERROR creando el thread.\n");
		}
	}

	// Thread estatico
	unsigned long DPeticion::_Thread(DPeticion* This) {
		TCHAR			szHead[] = L"Accept: */*\r\n\r\n";
		HINTERNET		Sesion = InternetOpen(L"RAVE_Actualizacion", INTERNET_OPEN_TYPE_PRECONFIG, NULL, INTERNET_INVALID_PORT_NUMBER, 0);
		HINTERNET		Peticion = InternetOpenUrl(Sesion, This->_Url.c_str(), szHead, 0, INTERNET_FLAG_RELOAD, 0);
		wchar_t			TamStr[64] = L"";
		DWORD           Descargado = 64; // HttpQueryInfo pide el tama�o del buffer, y luego devuelve los bytes descargados en la misma variable.
		DWORD			Tam = 0;

		// Error en la petici�n (probablemente no va el servidor, o no hay internet)
		if (Peticion == NULL) {
			InternetCloseHandle(Sesion);
			// Informo al thread principal de que se ha salido con error
			PostMessage(This->_VentanaMensajes, DWL_INTERNET_ERROR, Errores::Error_Servidor, This->ID());
			return -1;
		}

		// Obtengo el tama�o del archivo
		BOOL Ret = HttpQueryInfo(Peticion, HTTP_QUERY_CONTENT_LENGTH, (LPVOID)TamStr, &Descargado, (LPDWORD)0);

		// Si no hay error convierto el string del n�mero a DWORD
		if (Ret == TRUE) Tam = _wtol(TamStr);

		// Error obteniendo el tama�o del archivo
		if (Tam == 0) {
			InternetCloseHandle(Peticion);
			InternetCloseHandle(Sesion);
			// Informo al thread principal de que se ha salido con error
			PostMessage(This->_VentanaMensajes, DWL_INTERNET_ERROR, Errores::Error_ObteniendoTam, This->ID());
			return -1;
		}

		char	    Descarga[4097] = "";
		DWORD		TotalDescargado = 0;
		BOOL        P = FALSE;
		float		Valor = 0.0f;
		// Intento crear la mem�ria para almacenar todo el archivo
		try {
			This->_Datos = new char[Tam];
		}
		// no hay suficiente mem�ria
		catch (...) {
			PostMessage(This->_VentanaMensajes, DWL_INTERNET_ERROR, Errores::Error_Memoria, This->ID());
			return -1;
		}

		while (TRUE) {
			// Calculo el porcentaje de la descarg
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
		SendMessage(This->_VentanaMensajes, DWL_INTERNET_TERMINADO, 0, This->ID());

		// Informo al thread principal que ya se puede borrar esta petici�n
		PostMessage(This->_VentanaMensajes, DWL_INTERNET_ELIMINAR, 0, This->ID());

		return 0;
	}

};