#include "pch.h"
#include "DStringUtils.h"

#include <iostream>
#include <sstream>
#include <algorithm>
#include <iterator>

namespace DWL {

	namespace Strings {
		

		Split::Split(std::wstring &Texto, const TCHAR Separador) {
			_SepararWide(Texto.c_str(), Separador);
		}

		Split::Split(const TCHAR *Texto, const TCHAR Separador) {
			_SepararWide(Texto, Separador);
		}


		Split::~Split(void) {
		}


		std::wstring &Split::operator[](const size_t Pos) {
			static std::wstring StringVacio;
			if (Pos >= _TextoSeparado.size())	return StringVacio;
			else								return _TextoSeparado[Pos];
		}

		const bool Split::operator() (void) {
			return (_TextoSeparado.size() > 0);
		}


		void Split::_SepararWide(const TCHAR *Texto, const TCHAR Separador) {
			std::wstringstream Stream(Texto);
			std::wstring Tmp;
			while (std::getline(Stream, Tmp, Separador)) {
				if (!Tmp.empty())
					_TextoSeparado.push_back(Tmp);
			}
		}






		const size_t ContarCaracter(std::wstring &Texto, wchar_t Caracter) {
			size_t Total = 0;
			for (size_t i = 0; i < Texto.size(); i++) {
				if (Texto[i] == Caracter) Total++;
			}
			return Total;
		}

		/*
		const BOOL DString_AnsiToWide(char *IN_Ansi, std::wstring &OUT_Wide) {
			wchar_t TmpWTxt[4096];
			int Ret = MultiByteToWideChar(CP_UTF8, MB_PRECOMPOSED, IN_Ansi, -1, TmpWTxt, 4096);

			OUT_Wide = TmpWTxt;
			return (Ret > 0) ? TRUE : FALSE;
		}*/
	};
};