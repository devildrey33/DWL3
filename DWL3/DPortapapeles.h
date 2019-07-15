#pragma once
#include <string>

namespace DWL {
	class DPortapapeles {
	  public:
		static void ObtenerTexto(HWND hWndPaste, std::wstring& OUT_Texto);
		static void AsignarTexto(const wchar_t* Texto);
		static void AsignarTexto(std::wstring& Texto);
	};

};