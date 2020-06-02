#include "pch.h"
#include "DLogArchivo.h"
#include "DStringUtils.h"

namespace DWL {

	DLogArchivo::DLogArchivo(void) : _Archivo(NULL), DLog() {

	}
	// Destructor
	DLogArchivo::~DLogArchivo(void) {
		Cerrar();
	}

	// Función que crea la consola en un archivo de texto
	const BOOL DLogArchivo::Crear(const wchar_t *Path) {
		if (_Archivo == NULL) {
			_Archivo = CreateFile(Path, (GENERIC_READ | GENERIC_WRITE), FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			return TRUE;
		}

		return FALSE;
	}

	// Funnción que cierra el archivo
	void DLogArchivo::Cerrar(void) {
		CloseHandle(_Archivo);
		_Archivo = NULL;
	}

	// Función que lee el stdin de la consola y lo devuelve en el parámetro Texto (NO SE USA EN ARCHIVOS DE LOG)
/*	const BOOL DLogArchivo::Leer(std::wstring &Texto) {
		return FALSE;
	}*/

	// Función para escribir en la consola al estilo printf
	const BOOL DLogArchivo::Escribir(const wchar_t* Texto, ...) {
		WaitForSingleObject(_Mutex, INFINITE);
		static wchar_t  T[4096];
		static char     TMB[4096];
		va_list			Marker;
		va_start(Marker, Texto);
		int TotalCaracteres = vswprintf_s(T, 4096, Texto, Marker);
		DWORD CaracteresEscribidos = 0;
		TotalCaracteres = DWL::Strings::WideToAnsi(T, TMB, 4096);
		BOOL Ret = WriteFile(_Archivo, TMB, TotalCaracteres -1, &CaracteresEscribidos, NULL);
		va_end(Marker);
		ReleaseMutex(_Mutex);
		return Ret;
	}

	// Función para escribir en la consola usando un std::wstring
	const BOOL DLogArchivo::Escribir(std::wstring& Texto) {
		WaitForSingleObject(_Mutex, INFINITE);
		static char     TMB[4096];
		DWORD			CaracteresEscribidos = 0;
		DWL::Strings::WideToAnsi(Texto.c_str(), TMB, 4096);
		BOOL Ret = WriteFile(_Archivo, TMB, static_cast<DWORD>(Texto.size() -1), &CaracteresEscribidos, NULL);
		ReleaseMutex(_Mutex);
		return Ret;
	}

	// Función para escribir en la consola al estilo printf, que muestra los milisegundos que han pasado desde la ultima escritura en la consola
	const BOOL DLogArchivo::EscribirMS(const wchar_t* Texto, ...) {
		WaitForSingleObject(_Mutex, INFINITE);
		static wchar_t  T[4096];
		static char     TMB[4096];
		va_list			Marker;
		va_start(Marker, Texto);
		int TotalCaracteres = vswprintf_s(T, 4096, Texto, Marker);
		DWORD CaracteresEscribidos = 0;
		static wchar_t Txt[4096];
		ULONGLONG TmpTick = GetTickCount64();
		TotalCaracteres = swprintf_s(Txt, 4096, L"[%.8d] %s", static_cast<DWORD>(TmpTick - _UltimoTick), T);
		_UltimoTick = TmpTick;
		TotalCaracteres = DWL::Strings::WideToAnsi(Txt, TMB, 4096);		
		BOOL Ret = WriteFile(_Archivo, TMB, TotalCaracteres -1, &CaracteresEscribidos, NULL);
		va_end(Marker);
		ReleaseMutex(_Mutex);
		return Ret;
	}
	
	// Función para escribir en la consola usando un std::wstring, que muestra los milisegundos que han pasado desde la ultima escritura en la consola
	const BOOL DLogArchivo::EscribirMS(std::wstring& Texto) {
		WaitForSingleObject(_Mutex, INFINITE);
		DWORD CaracteresEscribidos = 0;
		static wchar_t Txt[4096];
		static char    TMB[4096];
		ULONGLONG TmpTick = GetTickCount64();
		int TotalCaracteres = swprintf_s(Txt, 4096, L"[%.8d] %s", static_cast<DWORD>(TmpTick - _UltimoTick), Texto.c_str());
		_UltimoTick = TmpTick;
		Strings::WideToAnsi(Txt, TMB, 4096);
		BOOL Ret = WriteFile(_Archivo, TMB, TotalCaracteres -1, &CaracteresEscribidos, NULL);

		ReleaseMutex(_Mutex);
		return Ret;
	}
	
};