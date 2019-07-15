#include "pch.h"
#include "DPortapapeles.h"

namespace DWL {
	void DPortapapeles::ObtenerTexto(HWND hWndPaste, std::wstring& OUT_Texto) {
		HGLOBAL				Paste = NULL;
		LPTSTR				Str = NULL;
//		static DWLString    Txt;
		if (OpenClipboard(hWndPaste) == TRUE) {
			#if defined UNICODE
				Paste = GetClipboardData(CF_UNICODETEXT);
			#else
				Paste = GetClipboardData(CF_TEXT);
			#endif
			//DWLDebug::MostrarUltimoError();
			if (Paste != NULL) {
				Str = static_cast<LPTSTR>(GlobalLock(Paste));
				if (Str != nullptr) OUT_Texto = Str;
				GlobalUnlock(Paste);
			}
			CloseClipboard();
		}
	}

	void DPortapapeles::AsignarTexto(const wchar_t* nTexto) {
		if (nTexto == NULL) return;
		size_t Len = wcslen(nTexto) + 1;
		if (Len == 0) return;
		if (OpenClipboard(NULL) == TRUE) {
			HGLOBAL Copy = GlobalAlloc(GMEM_MOVEABLE, Len * sizeof(TCHAR)); 
			LPTSTR  Str  = NULL;
			if (Copy) { 
				Str = static_cast<LPTSTR>(GlobalLock(Copy)); 
				if (Str) {
					wcscpy_s(Str, Len, nTexto);
					Str[Len - 1] = static_cast<TCHAR>(0);
					GlobalUnlock(Copy);  
					EmptyClipboard();
					SetClipboardData(CF_UNICODETEXT, Copy); 
					CloseClipboard();
				}
			}
		}

	}

	void DPortapapeles::AsignarTexto(std::wstring& Texto) {
		AsignarTexto(Texto.c_str());
	}

}