#include "pch.h"
#include "DStringUtils.h"

#include <iostream>
#include <sstream>
#include <algorithm>
#include <iterator>

namespace DWL {


		

	Strings::Split::Split(std::wstring &Texto, const TCHAR Separador) {
		_SepararWide(Texto.c_str(), Separador);
	}

	Strings::Split::Split(const TCHAR *Texto, const TCHAR Separador) {
		_SepararWide(Texto, Separador);
	}


	Strings::Split::~Split(void) {
	}


	std::wstring &Strings::Split::operator[](const size_t Pos) {
		static std::wstring StringVacio;
		if (Pos >= _TextoSeparado.size())	return StringVacio;
		else								return _TextoSeparado[Pos];
	}

	const bool Strings::Split::operator() (void) {
		return (_TextoSeparado.size() > 0);
	}


	void Strings::Split::_SepararWide(const TCHAR *Texto, const TCHAR Separador) {
		std::wstringstream Stream(Texto);
		std::wstring Tmp;
		while (std::getline(Stream, Tmp, Separador)) {
			if (!Tmp.empty())
				_TextoSeparado.push_back(Tmp);
		}
	}






	const size_t Strings::ContarCaracter(std::wstring &Texto, wchar_t Caracter) {
		size_t Total = 0;
		for (size_t i = 0; i < Texto.size(); i++) {
			if (Texto[i] == Caracter) Total++;
		}
		return Total;
	}


	const int Strings::AnsiToWide(const char* IN_Ansi, std::wstring& OUT_Wide) {
		wchar_t TmpWTxt[4096];
		int Ret = MultiByteToWideChar(CP_UTF8, MB_PRECOMPOSED, IN_Ansi, -1, TmpWTxt, 4096);
		OUT_Wide = TmpWTxt;
		return (Ret > 0) ? TRUE : FALSE;
	}


	const int Strings::WideToAnsi(const wchar_t* IN_Wide, std::string& OUT_Ansi) {
		char TmpTxt[4096];
		int	Ret = WideCharToMultiByte(CP_UTF8, NULL, IN_Wide, -1, TmpTxt, 4096, NULL, NULL);
		OUT_Ansi = TmpTxt;
		return (Ret > 0) ? TRUE : FALSE;
	}



	/* Devuelve un string con el numero de bytes en formato legible (Bytes, KiloBytes, MegaBytes, GigaBytes, TeraBytes) */
	std::wstring& Strings::FormatoBytes(const unsigned long long nBytes, std::wstring& OUT_StrBytes) {
		const wchar_t* Medidas[5] = { L"B", L"KB", L"MB", L"GB", L"TB" };
		int					PosMedidas = 0;
		long double			Resultado = static_cast<long double>(nBytes);
		unsigned long long  Bytes = nBytes;
		for (PosMedidas = 0; PosMedidas < 5 && Bytes >= 1024; PosMedidas++, Bytes /= 1024) {
			Resultado = static_cast<long double>(Bytes) / 1024.0f;
		}

		OUT_StrBytes = ToStrF(Resultado, 2) + Medidas[PosMedidas];

		return OUT_StrBytes;
	}


	// Devuelve un string con el nombre de archivo sin el resto del path
	std::wstring &Path_BaseName(const wchar_t* nPath, const BOOL Extension) {
		static std::wstring Ret;
		std::wstring		Tmp = nPath;
		size_t				Punto = (Extension == TRUE) ? Tmp.size() : 0;
		size_t				Barra = 0;
		for (size_t i = Tmp.size() - 1; i > 0; i--) {
			// Primer punto empezando por la derecha
			if (Tmp[i] == '.' && Punto == 0) {
				Punto = i;
			}
			// Primera barra empezando por la derecha
			if (Tmp[i] == '\\') {
				Barra = i;
				break;
			}
		}
		Ret = Tmp.substr(Barra, Punto - Barra);
		return Ret;
	}

	// Devuelve un string con el path sin el nombre de archivo
	// Puedes hacer que devuelva uno o varios directorios inferiores si ajustas la variable BajarPaths
	std::wstring &Path_DirName(const wchar_t* nPath, const UINT BajarPaths) {
		static std::wstring Ret;
		std::wstring		Tmp		= nPath;
		UINT				BP		= 0;
		size_t				Barra	= 0;
		for (size_t i = Tmp.size() - 1; i > 0; i--) {
			if (Tmp[i] == '\\') {
				if (BajarPaths == BP) {
					Barra = i;
					break;
				}
				BP++;
			}
		}
		Ret = Tmp.substr(0, Barra);
		return Ret;
	}


};
