#include "pch.h"
#include "DLog.h"
#include "Strsafe.h"

namespace DWL {

	const BOOL DLog::MostrarUltimoError(void) {
		WaitForSingleObject(_Mutex, INFINITE);
		DWORD dw = GetLastError();
		if (dw == 0) {
			ReleaseMutex(_Mutex);
			//EscribirMS(L"GetLastError() : Sin errores.\n");
			return FALSE;
		}


		static wchar_t Mensaje[4096];
		Mensaje[0] = 0;


		size_t size = FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), Mensaje, (sizeof(Mensaje) / sizeof(wchar_t)), NULL);


		if (Mensaje[0] != 0) {
			std::wstring TmpTxt = L"GetLastError() : ";
			TmpTxt += (LPCTSTR)Mensaje;
			EscribirMS(TmpTxt);
		}

		ReleaseMutex(_Mutex);
		return TRUE;
	}

};