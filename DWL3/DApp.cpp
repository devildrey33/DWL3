#include "pch.h"
#include "DApp.h"
#include <versionhelpers.h>
#include <Shellapi.h>
#include "DArchivoInternet.h"
#include <eh.h>
#include <Psapi.h>
#include <sstream>
#include "DStringUtils.h"

DWL::DApp *_Aplicacion = nullptr;

namespace DWL {

	// Constructor
	DApp::DApp(void) : _ColorFondoVentana(nullptr) {
		// Asigno esta clase al puntero global de la aplicación
		_Aplicacion = this;
		// Activo el _set_se_translator para poder hacer un catch a todas las excepciones
		_Catch_Exceptions();
		// Obtengo la línea de comandos
		_ObtenerLineaComandos();
		// Creo la ventana para los mensajes multithread
		_CrearVentanaMensajes();
		// Inicializo la librería COM (para el TaskBarList)
		HRESULT CIE = CoInitializeEx(0, COINIT_MULTITHREADED | COINIT_SPEED_OVER_MEMORY);
		// Creo el color que se usara para el fondo de las ventanas (NO BORRAR lo hace el windows)
		_ColorFondoVentana = CreateSolidBrush(COLOR_FONDO);
	}


	/* Función que inicia el set_se_translator para poder hacer catch en todas las excepciones (0xC0000005 : Access Violation) en especial
	    Usar :
			try								{   ...	  }
			catch(const std::exception &ex)	{   ...   };
	*/
	void DApp::_Catch_Exceptions(void) {
		// Be sure to enable "Yes with SEH Exceptions (/EHa)" in C++ / Code Generation;
		_set_se_translator([](unsigned int Codigo, EXCEPTION_POINTERS* pExp) {
			std::string ErrStr = DApp::_ErrorStr(pExp, true, Codigo);
			// En modo DEBUG imprimo el error por la consola
			#ifdef _DEBUG
				std::wstring ErrWStr;
				DWL::Strings::AnsiToWide(ErrStr.c_str(), ErrWStr);
				ErrWStr += L"\n";
				Debug_Escribir(ErrWStr);
			#endif
			throw std::exception(ErrStr.c_str());
		});

		/*_set_se_translator([](unsigned int u, EXCEPTION_POINTERS* pExp) {
			std::string error = "SE Exception: ";
			switch (u) {
				case 0xC0000005:
					error += "Access Violation";
					break;
				default:
					char result[11];
					sprintf_s(result, 11, "0x%08X", u);
					error += result;
					break;
			};
			throw std::exception(error.c_str());
		})*/;
	}

	// Función que convierte una excepción a string
	// https://stackoverflow.com/questions/457577/catching-access-violation-exceptions
	std::string DApp::_ErrorStr(struct _EXCEPTION_POINTERS *ep, bool has_exception_code, exception_code_t code) {		
		HMODULE hm;
		::GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, static_cast<LPCTSTR>(ep->ExceptionRecord->ExceptionAddress), &hm);
		MODULEINFO mi;
		::GetModuleInformation(::GetCurrentProcess(), hm, &mi, sizeof(mi));
		char fn[MAX_PATH];
		::GetModuleFileNameExA(::GetCurrentProcess(), hm, fn, MAX_PATH);

		std::ostringstream oss;
		oss << "SE " << (has_exception_code ? _seDescripcion(code) : "") << " at address 0x" << std::hex << ep->ExceptionRecord->ExceptionAddress << std::dec
			<< " inside " << fn << " loaded at base address 0x" << std::hex << mi.lpBaseOfDll << "\n";

		if (has_exception_code && (
			code == EXCEPTION_ACCESS_VIOLATION ||
			code == EXCEPTION_IN_PAGE_ERROR)) {
			oss << "Invalid operation: " << _opDescripcion(static_cast<ULONG>(ep->ExceptionRecord->ExceptionInformation[0])) << L" at address 0x" << std::hex << ep->ExceptionRecord->ExceptionInformation[1] << std::dec << "\n";
		}

		if (has_exception_code && code == EXCEPTION_IN_PAGE_ERROR) {
			oss << "Underlying NTSTATUS code that resulted in the exception " << ep->ExceptionRecord->ExceptionInformation[2] << "\n";
		}

		return oss.str();
	}

	
	// https://stackoverflow.com/questions/3523716/is-there-a-function-to-convert-exception-pointers-struct-to-a-string
	// Funcion que devuelve el texto de la despreición de una excepción
	const char *DApp::_seDescripcion(const exception_code_t &code)	{
		switch (code) {
			case EXCEPTION_ACCESS_VIOLATION:			return "EXCEPTION_ACCESS_VIOLATION";
			case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:		return "EXCEPTION_ARRAY_BOUNDS_EXCEEDED";
			case EXCEPTION_BREAKPOINT:					return "EXCEPTION_BREAKPOINT";
			case EXCEPTION_DATATYPE_MISALIGNMENT:		return "EXCEPTION_DATATYPE_MISALIGNMENT";
			case EXCEPTION_FLT_DENORMAL_OPERAND:		return "EXCEPTION_FLT_DENORMAL_OPERAND";
			case EXCEPTION_FLT_DIVIDE_BY_ZERO:			return "EXCEPTION_FLT_DIVIDE_BY_ZERO";
			case EXCEPTION_FLT_INEXACT_RESULT:			return "EXCEPTION_FLT_INEXACT_RESULT";
			case EXCEPTION_FLT_INVALID_OPERATION:		return "EXCEPTION_FLT_INVALID_OPERATION";
			case EXCEPTION_FLT_OVERFLOW:				return "EXCEPTION_FLT_OVERFLOW";
			case EXCEPTION_FLT_STACK_CHECK:				return "EXCEPTION_FLT_STACK_CHECK";
			case EXCEPTION_FLT_UNDERFLOW:				return "EXCEPTION_FLT_UNDERFLOW";
			case EXCEPTION_ILLEGAL_INSTRUCTION:			return "EXCEPTION_ILLEGAL_INSTRUCTION";
			case EXCEPTION_IN_PAGE_ERROR:				return "EXCEPTION_IN_PAGE_ERROR";
			case EXCEPTION_INT_DIVIDE_BY_ZERO:			return "EXCEPTION_INT_DIVIDE_BY_ZERO";
			case EXCEPTION_INT_OVERFLOW:				return "EXCEPTION_INT_OVERFLOW";
			case EXCEPTION_INVALID_DISPOSITION:			return "EXCEPTION_INVALID_DISPOSITION";
			case EXCEPTION_NONCONTINUABLE_EXCEPTION:	return "EXCEPTION_NONCONTINUABLE_EXCEPTION";
			case EXCEPTION_PRIV_INSTRUCTION:			return "EXCEPTION_PRIV_INSTRUCTION";
			case EXCEPTION_SINGLE_STEP:					return "EXCEPTION_SINGLE_STEP";
			case EXCEPTION_STACK_OVERFLOW:				return "EXCEPTION_STACK_OVERFLOW";
			default:									return "UNKNOWN EXCEPTION";
		}
	}

	// Función que devuelve el texto de la la operación ejecutada durante una excepción
	const char *DApp::_opDescripcion(const ULONG opcode) {
		switch (opcode) {
			case 0:		return "read";
			case 1:		return "write";
			case 8:		return "user-mode data execution prevention (DEP) violation";
			default:	return "unknown";
		}
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
				if (PreVentana == nullptr) return FALSE;
				PreVentana->_hWnd = nhWnd;
				SetWindowLongPtr(nhWnd, GWLP_USERDATA, (LONG_PTR)PreVentana);
				PreVentana->_GestorMensajes(uMsg, wParam, lParam);
				return TRUE;			
			default: 
				PreVentana = reinterpret_cast<DApp*>(::GetWindowLongPtr(nhWnd, GWLP_USERDATA));
				if (PreVentana != nullptr) {
					return PreVentana->_GestorMensajes(uMsg, wParam, lParam);
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
		DPeticion *P = DArchivoInternet::Buscar(nID);
		if (P != nullptr)	P->_LambdaError(nError);
		DArchivoInternet::_EliminarPeticion(nID);
	}

	// Función que recibe el porcentaje de la descarga
	void DApp::_Internet_Porcentaje(const float nValor, const UINT nID) {
		DPeticion *P = DArchivoInternet::Buscar(nID);
		if (P != nullptr)	P->_LambdaPorcentaje(nValor);
	}

	// Función que indica que la descarga ha finalizado correctamente
	void DApp::_Internet_Terminado(const UINT nID) {
		DPeticion *P = DArchivoInternet::Buscar(nID);
		if (P != nullptr)	P->_LambdaTerminado(*P);
	}


	LRESULT CALLBACK DApp::_GestorMensajes(UINT uMsg, WPARAM wParam, LPARAM lParam) {
		switch (uMsg) {
			case DWL_INTERNET_ELIMINAR		:	_Internet_Eliminar((UINT)lParam);											return 0;
			case DWL_INTERNET_ERROR			:	_Internet_Error((UINT)wParam, (UINT)lParam);								return 0;
			case DWL_INTERNET_PORCENTAJE	:	_Internet_Porcentaje(reinterpret_cast<float &>(wParam), (UINT)lParam);		return 0;
			case DWL_INTERNET_TERMINADO		:	_Internet_Terminado((UINT)lParam);											return 0;
		}
		return GestorMensajes(uMsg, wParam, lParam);
	}
	
};

