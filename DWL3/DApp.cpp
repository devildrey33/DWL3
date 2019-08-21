#include "pch.h"
#include "DApp.h"
#include <versionhelpers.h>
#include <Shellapi.h>
#include "DArchivoInternet.h"

DWL::DApp *_Aplicacion = nullptr;

namespace DWL {

	// Constructor
	DApp::DApp(void) {
		// Asigno esta clase al puntero global de la aplicación
		_Aplicacion = this;
		// Obtengo la línea de comandos
		_ObtenerLineaComandos();
		// Creo la ventana para los mensajes multithread
		_CrearVentanaMensajes();
		// Inicializo la librería COM (para el TaskBarList)
		HRESULT CIE = CoInitializeEx(0, COINIT_MULTITHREADED | COINIT_SPEED_OVER_MEMORY);
	}

	// Devuelve el path de la aplicación sin el ejecutable
	std::wstring &DApp::Path(const BOOL SinEjecutable) {
		static std::wstring AppPath;

		// Obtengo el directorio actual de la aplicación, para ello debo obtener el path del ejecutable, y recortarlo
		TCHAR PathApp[1024];
		DWORD Size = GetModuleFileName(NULL, PathApp, 1024);
		if (SinEjecutable == TRUE) {
			for (Size; Size > 0; Size--) {
				if (PathApp[Size] == TCHAR('\\')) {
					PathApp[Size + 1] = 0;
					AppPath = PathApp;
					break;
				}
			}
		}
		else {
			AppPath = PathApp;
		}

		return AppPath;
	}

	// Función que devuelve el path del sistema donde se está ejecutando la aplicación 
	std::wstring& DApp::PathActual(void) {
		static wchar_t		Path[1024];
		static std::wstring Ret;
		GetCurrentDirectory(1024, Path);
		Ret = Path;
		return Ret;
	}

	// Función que asigna el path del sistema donde se está ejecutando la aplicación 
	const BOOL DApp::PathActual(const wchar_t* Path) {
		return SetCurrentDirectory(Path);
	}

	// Función que asigna el path del sistema donde se está ejecutando la aplicación 
	const BOOL DApp::PathActual(std::wstring &Path) {
		return SetCurrentDirectory(Path.c_str());
	}

	// Obtiene la linea de comandos y la almacena en el vector _LineaComandos
	void DApp::_ObtenerLineaComandos(void) {
		int		TotalArgs	= 0;
		TCHAR **Args		= CommandLineToArgvW(GetCommandLine(), &TotalArgs);
		// Agrego los argumentos al vector _LineaComandos
		for (int i = 0; i < TotalArgs; i++) {
			_LineaComandos.push_back(Args[i]);
		}

		// Libero la memória 
		if (TotalArgs > 0)
			LocalFree(Args);
	}


	/* Vacia la cola de mensajes para este hilo */
	void DApp::Eventos_Mirar(void) {
		static MSG msg;
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}


	// Devuelve el sistema operativo que se está utilizando en formato texto
	const wchar_t *DApp::ObtenerSO(void) {
		UINT Version = 0;

		if (IsWindowsXPOrGreater())			Version++;
		if (IsWindowsXPSP1OrGreater())		Version++;
		if (IsWindowsXPSP2OrGreater())		Version++;
		if (IsWindowsXPSP3OrGreater())		Version++;
		if (IsWindowsVistaOrGreater())		Version++;
		if (IsWindowsVistaSP1OrGreater())	Version++;
		if (IsWindowsVistaSP2OrGreater())	Version++;
		if (IsWindows7OrGreater())			Version++;
		if (IsWindows7SP1OrGreater())		Version++;
		if (IsWindows8OrGreater())			Version++;
		if (IsWindows8Point1OrGreater())	Version++;
		if (IsWindows10OrGreater())			Version++;

		switch (Version) {
			case  0: return L"";
			case  1: return L"Windows XP";
			case  2: return L"Windows XP SP1";
			case  3: return L"Windows XP SP2";
			case  4: return L"Windows XP SP3";
			case  5: return L"Windows Vista";
			case  6: return L"Windows Vista SP1";
			case  7: return L"Windows Vista SP2";
			case  8: return L"Windows 7";
			case  9: return L"Windows 7 SP1";
			case 10: return L"Windows 8";
			case 11: return L"Windows 8.1";
			case 12: return L"Windows 10";
			default: return L"Windows 10 o superior";
		}

		return L"";
	}

	// Bucle de mensajes para las ventanas
	int DApp::BucleMensajes(void) {
		MSG Msg;
		while (GetMessage(&Msg, NULL, 0, 0) > 0) {
			TranslateMessage(&Msg);
			DispatchMessage(&Msg);
		}
		return static_cast<int>(Msg.wParam);
	}

	// Obtiene la descripción del ultimo error de windows
	std::wstring& DApp::UltimoError(void) {
		static std::wstring Texto;

		LPTSTR Error = 0;
		if (!FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), 0, (LPTSTR)& Error, 0, NULL) == 0) {
			Texto = Error;
		}
		else {
			Texto = L"";
		}

		if (Error) LocalFree(Error);

		return Texto;
	}



	// Función que crea una ventana invisible para manejar mensajes de multiples threads
	void DApp::_CrearVentanaMensajes(void) {
		// La ventana ya no existe pero tenemos el hWnd, por lo que elimino toda la memória
		if (_hWnd != NULL && IsWindow(_hWnd) == 0) { Destruir(); }

		// Registro la clase para la ventana invisible de los mensajes
		ATOM RetRgistrarClase = RegistrarClase(L"DWL_Mensajes", reinterpret_cast<WNDPROC>(_GestorMensajesEstatico));
		// Creo la ventana para los mensajes
		_hWnd = CreateWindowEx(NULL, L"DWL_Mensajes", L"", WS_POPUP, 0, 0, 10, 10, HWND_MESSAGE , NULL, GetModuleHandle(NULL), this);
		// Muestro un error por la consola si no se ha creado
//		Debug_MostrarUltimoError();
	}


	// Gestor de mensajes estático que busca el gestor de mensajes asociado a la clase aplicación
	LRESULT CALLBACK DApp::_GestorMensajesEstatico(HWND nhWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
		static DApp *PreVentana = nullptr;
		switch (uMsg) {
			case WM_CREATE: 
				PreVentana = reinterpret_cast<DApp*>(((CREATESTRUCT*)lParam)->lpCreateParams);
				if (PreVentana == NULL) return FALSE;
				PreVentana->_hWnd = nhWnd;
				SetWindowLongPtr(nhWnd, GWLP_USERDATA, (LONG_PTR)PreVentana);
				PreVentana->GestorMensajes(uMsg, wParam, lParam);
				return TRUE;			
			default: 
				PreVentana = reinterpret_cast<DApp*>(::GetWindowLongPtr(nhWnd, GWLP_USERDATA));
				if (PreVentana != NULL) {
					return PreVentana->GestorMensajes(uMsg, wParam, lParam);
				}
		}
		return DefWindowProc(nhWnd, uMsg, wParam, lParam);
	}


	// Función para eliminar peticiones de descarga
	void DApp::_Internet_Eliminar(const UINT nID) {
		DArchivoInternet::_EliminarPeticion(nID);
	}

	// Función para los errores en una petición de descarga
	void DApp::_Internet_Error(const UINT nError, const UINT nID) {
		DArchivoInternet::_Buscar(nID)->_LambdaError(nError);
		DArchivoInternet::_EliminarPeticion(nID);
	}

	// Función que recibe el porcentaje de la descarga
	void DApp::_Internet_Porcentaje(const float nValor, const UINT nID) {
		DArchivoInternet::_Buscar(nID)->_LambdaPorcentaje(nValor);
	}

	// Función que indica que la descarga ha finalizado correctamente
	void DApp::_Internet_Terminado(const char *Datos, const UINT nID) {
		DArchivoInternet::_Buscar(nID)->_LambdaTerminado(Datos);
	}


	LRESULT CALLBACK DApp::_GestorMensajes(UINT uMsg, WPARAM wParam, LPARAM lParam) {
		switch (uMsg) {
			case DWL_INTERNET_ELIMINAR		:	_Internet_Eliminar((UINT)lParam);										return 0;
			case DWL_INTERNET_ERROR			:	_Internet_Error((UINT)wParam, (UINT)lParam);							return 0;
			case DWL_INTERNET_PORCENTAJE	:	_Internet_Porcentaje((float)wParam, (UINT)lParam);						return 0;
			case DWL_INTERNET_TERMINADO		:	_Internet_Terminado((const char *)LongToPtr(wParam), (UINT)lParam);		return 0;
		}
		return GestorMensajes(uMsg, wParam, lParam);
	}
	
};

