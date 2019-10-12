#include "pch.h"
#include "DStringUtils.h"

#include <iostream>
#include <sstream>
#include <algorithm>
#include <iterator>

namespace DWL {


		

	Strings::Split::Split(std::wstring& Texto, const wchar_t Delimitador) {
		_SepararWide(Texto.c_str(), Delimitador);
	}

	Strings::Split::Split(const wchar_t* Texto, const wchar_t Delimitador) {
		_SepararWide(Texto, Delimitador);
	}

	Strings::Split::Split(std::wstring& Texto, const wchar_t *Delimitadores) {
		_SepararWide(Texto.c_str(), Delimitadores);
	}

	Strings::Split::Split(const wchar_t* Texto, const wchar_t *Delimitadores) {
		_SepararWide(Texto, Delimitadores);
	}


	Strings::Split::~Split(void) {
	}


	std::wstring& Strings::Split::operator[](const size_t Pos) {
		static std::wstring StringVacio;
		if (Pos >= _TextoSeparado.size())	return StringVacio;
		else								return _TextoSeparado[Pos];
	}

	const bool Strings::Split::operator() (void) {
		return (_TextoSeparado.size() > 0);
	}


	void Strings::Split::_SepararWide(const wchar_t* Texto, const wchar_t Delimitador) {
		std::wstringstream Stream(Texto);
		std::wstring Tmp;
		while (std::getline(Stream, Tmp, Delimitador)) {
			if (!Tmp.empty())
				_TextoSeparado.push_back(Tmp);
		}
	}


	void Strings::Split::_SepararWide(const wchar_t* Texto, const wchar_t *Delimitadores) {
		// https://stackoverflow.com/questions/7621727/split-a-string-into-words-by-multiple-delimiters

		std::wstringstream stringStream(Texto);
		std::wstring line;
		while (std::getline(stringStream, line)) {
			std::size_t prev = 0, pos;
			while ((pos = line.find_first_of(Delimitadores, prev)) != std::string::npos) {
				if (pos > prev)
					_TextoSeparado.push_back(line.substr(prev, pos - prev));
				prev = pos + 1;
			}
			if (prev < line.length())
				_TextoSeparado.push_back(line.substr(prev, std::string::npos));
		}

	}



	Strings::SplitA::SplitA(std::string& Texto, const char Delimitador) {
		_SepararAnsi(Texto.c_str(), Delimitador);
	}

	Strings::SplitA::SplitA(const char* Texto, const char Delimitador) {
		_SepararAnsi(Texto, Delimitador);
	}

	Strings::SplitA::SplitA(std::string& Texto, const char *Delimitadores) {
		_SepararAnsi(Texto.c_str(), Delimitadores);
	}

	Strings::SplitA::SplitA(const char* Texto, const char *Delimitadores) {
		_SepararAnsi(Texto, Delimitadores);
	}


	Strings::SplitA::~SplitA(void) {
	}


	std::string& Strings::SplitA::operator[](const size_t Pos) {
		static std::string StringVacio;
		if (Pos >= _TextoSeparado.size())	return StringVacio;
		else								return _TextoSeparado[Pos];
	}

	const bool Strings::SplitA::operator() (void) {
		return (_TextoSeparado.size() > 0);
	}


	void Strings::SplitA::_SepararAnsi(const char *Texto, const char Delimitador) {
		std::stringstream Stream(Texto);
		std::string Tmp;
		while (std::getline(Stream, Tmp, Delimitador)) {
			if (!Tmp.empty())
				_TextoSeparado.push_back(Tmp);
		}
	}


	void Strings::SplitA::_SepararAnsi(const char* Texto, const char *Delimitadores) {
		std::stringstream stringStream(Texto);
		std::string line;
		while (std::getline(stringStream, line)) {
			std::size_t prev = 0, pos;
			while ((pos = line.find_first_of(" ';", prev)) != std::string::npos) {
				if (pos > prev)
					_TextoSeparado.push_back(line.substr(prev, pos - prev));
				prev = pos + 1;
			}
			if (prev < line.length())
				_TextoSeparado.push_back(line.substr(prev, std::string::npos));
		}
	}
















	const size_t Strings::ContarCaracter(std::wstring &Texto, wchar_t Caracter) {
		size_t Total = 0;
		for (size_t i = 0; i < Texto.size(); i++) {
			if (Texto[i] == Caracter) Total++;
		}
		return Total;
	}

	
	const int Strings::UTF8ToWide(const char* IN_UTF8, std::wstring& OUT_Wide) {
		static wchar_t TmpWTxt[4096] = L"";
		int Ret = MultiByteToWideChar(CP_UTF8, MB_PRECOMPOSED, IN_UTF8, -1, TmpWTxt, 4096);
		OUT_Wide = TmpWTxt;
		return (Ret > 0) ? TRUE : FALSE;
	}

	const int Strings::WideToUTF8(const wchar_t* IN_Wide, std::string& OUT_UTF8) {
		static char TmpTxt[4096] = "";
		int	Ret = WideCharToMultiByte(CP_UTF8, NULL, IN_Wide, -1, TmpTxt, 4096, NULL, NULL);
		OUT_UTF8 = TmpTxt;
		return (Ret > 0) ? TRUE : FALSE;
	}

	const int Strings::AnsiToWide(const char* IN_Ansi, std::wstring& OUT_Wide) {
		static wchar_t TmpWTxt[4096] = L"";
		int Ret = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, IN_Ansi, -1, TmpWTxt, 4096);
		OUT_Wide = TmpWTxt;
		return (Ret > 0) ? TRUE : FALSE;
	}

	const int Strings::WideToAnsi(const wchar_t* IN_Wide, std::string& OUT_Ansi) {
		static char TmpTxt[4096] = "";
		int	Ret = WideCharToMultiByte(CP_ACP, NULL, IN_Wide, -1, TmpTxt, 4096, NULL, NULL);
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
