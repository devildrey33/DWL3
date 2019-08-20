#include "pch.h"
#include "DApp.h"
#include <versionhelpers.h>
#include <Shellapi.h>

DWL::DApp *_Aplicacion = NULL;

namespace DWL {

	// Constructor
	DApp::DApp(void) {
		_Aplicacion = this;
		_ObtenerLineaComandos();
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


	
};

