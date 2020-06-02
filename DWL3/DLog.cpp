#include "pch.h"
#include "DLog.h"
#include "Strsafe.h"

namespace DWL {

	const BOOL DLog::MostrarUltimoError(void) {
		WaitForSingleObject(_Mutex, INFINITE);
		const wchar_t Texto[] = L"GetLastError() : ";
		LPVOID lpMsgBuf = nullptr;
		LPVOID lpDisplayBuf = nullptr;
		DWORD dw = GetLastError();
		if (dw == 0) {
			ReleaseMutex(_Mutex);
			//EscribirMS(L"GetLastError() : Sin errores.\n");
			return FALSE;
		}

		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, dw, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);

		if (lpMsgBuf != nullptr) {
			// Display the error message and exit the process
			lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT, (long)lstrlen((LPCTSTR)lpMsgBuf + (long)lstrlen((LPCTSTR)Texto) + 40l) * sizeof(TCHAR));
			StringCchPrintf((LPTSTR)lpDisplayBuf, LocalSize(lpDisplayBuf) / sizeof(TCHAR), TEXT("%s failed with error %d: %s\n"), Texto, dw, lpMsgBuf);
			std::wstring TmpTxt = (LPCTSTR)lpDisplayBuf;
			EscribirMS(TmpTxt);
		}

		//		MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK);

		LocalFree(lpMsgBuf);
		LocalFree(lpDisplayBuf);

		ReleaseMutex(_Mutex);
		return TRUE;
	}

};