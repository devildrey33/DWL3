#include "pch.h"
#include "Strsafe.h"
#include "DLogConsola.h"

namespace DWL {

	DLogConsola::DLogConsola(void) : _Consola(NULL), DLog() {
	}

	DLogConsola::~DLogConsola(void) {
		FreeConsole();
	}

	const BOOL DLogConsola::Crear(const wchar_t *Titulo) {
		if (!GetConsoleWindow()) {
			AllocConsole();
			SetConsoleTitle(Titulo);
			_Consola	= GetStdHandle(STD_OUTPUT_HANDLE);
			_UltimoTick = GetTickCount64();
			return TRUE;
		}
		return FALSE;
	}


/*	const BOOL DConsola::Escribir(const wchar_t *Texto) {
		DWORD CaracteresEscribidos = 0;
		return WriteConsole(_Consola, Texto, static_cast<DWORD>(wcslen(Texto)), &CaracteresEscribidos, NULL);
	}*/

	const BOOL DLogConsola::Escribir(const wchar_t *Texto, ...) {
		WaitForSingleObject(_Mutex, INFINITE);
		static wchar_t  T[4096];
		va_list			Marker;
		va_start(Marker, Texto);
		int TotalCaracteres = vswprintf_s(T, 4096, Texto, Marker);
		DWORD CaracteresEscribidos = 0;
		BOOL Ret = WriteConsole(_Consola, T, TotalCaracteres, &CaracteresEscribidos, NULL);
		va_end(Marker);
		ReleaseMutex(_Mutex);
		return Ret;
	}

	const BOOL DLogConsola::Escribir(std::wstring &Texto) {
		WaitForSingleObject(_Mutex, INFINITE);
		DWORD CaracteresEscribidos = 0;
		BOOL Ret = WriteConsole(_Consola, Texto.c_str(), static_cast<DWORD>(Texto.size()), &CaracteresEscribidos, NULL);
		ReleaseMutex(_Mutex);
		return Ret;
	}

	/*const BOOL DLogConsola::Leer(std::wstring &Texto) {
		WaitForSingleObject(_Mutex, INFINITE);
		DWORD CaracteresLeidos = 0;
		static wchar_t Caracteres[4096] = L"\0";
		SecureZeroMemory(Caracteres, 4096 * sizeof(wchar_t));
		BOOL R = ReadConsole(_Consola, Caracteres, 4096, &CaracteresLeidos, NULL);
		Texto = Caracteres;
		ReleaseMutex(_Mutex);
		return R;
	}*/






/*	const BOOL DConsola::EscribirMS(const wchar_t *Texto) {

		DWORD CaracteresEscribidos = 0;
		static wchar_t Txt[512];
		DWORD TmpTick = GetTickCount();
		int TotalCaracteres = swprintf_s(Txt, 512, L"[%.8d] %s", TmpTick - _UltimoTick, Texto);
		_UltimoTick = TmpTick;
		return WriteConsole(_Consola, Txt, TotalCaracteres, &CaracteresEscribidos, NULL);
	}*/

	const BOOL DLogConsola::EscribirMS(const wchar_t *Texto, ...) {
		WaitForSingleObject(_Mutex, INFINITE);
		static wchar_t  T[4096];
		va_list			Marker;
		va_start(Marker, Texto);
		int TotalCaracteres = vswprintf_s(T, 4096, Texto, Marker);
		DWORD CaracteresEscribidos = 0;
		static wchar_t Txt[4096];
		ULONGLONG TmpTick = GetTickCount64();
		TotalCaracteres = swprintf_s(Txt, 4096, L"[%.8d] %s", static_cast<DWORD>(TmpTick - _UltimoTick), T);
		_UltimoTick = TmpTick;
		BOOL Ret = WriteConsole(_Consola, Txt, TotalCaracteres, &CaracteresEscribidos, NULL);
		va_end(Marker);
		ReleaseMutex(_Mutex);
		return Ret;
	}

	const BOOL DLogConsola::EscribirMS(std::wstring &Texto) {
		WaitForSingleObject(_Mutex, INFINITE);
		DWORD CaracteresEscribidos = 0;
		static wchar_t Txt[4096];
		ULONGLONG TmpTick = GetTickCount64();
		int TotalCaracteres = swprintf_s(Txt, 4096, L"[%.8d] %s", static_cast<DWORD>(TmpTick - _UltimoTick), Texto.c_str());
		_UltimoTick = TmpTick;
		BOOL Ret = WriteConsole(_Consola, Txt, TotalCaracteres, &CaracteresEscribidos, NULL);
		ReleaseMutex(_Mutex);
		return Ret;
	}


};