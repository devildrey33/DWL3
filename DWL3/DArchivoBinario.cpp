#include "pch.h"
#include "DArchivoBinario.h"
#include <wincrypt.h>

namespace DWL {

	// Constructor por defecto
	DArchivoBinario::DArchivoBinario(void) {
	}
	
	// Constructor que abre el archivo especificado para lectura y escritura
	DArchivoBinario::DArchivoBinario(const wchar_t* nPath, const BOOL Abrir_si_no_existe) {
		AbrirArchivo(nPath, Abrir_si_no_existe);
	}

	// Constructor que abre el archivo especificado para lectura y escritura
	DArchivoBinario::DArchivoBinario(std::wstring &nPath, const BOOL Abrir_si_no_existe) {
		AbrirArchivo(nPath.c_str(), Abrir_si_no_existe);
	}

	// Destructor (cierra el archivo y elimina los datos de la memória)
	DArchivoBinario::~DArchivoBinario(void) {
		Cerrar();
	}

	// Función que abre el archivo para lectura y escritura (si ya habia un archivo abierto lo cierra)
	const BOOL DArchivoBinario::AbrirArchivo(const wchar_t* nPath, const BOOL Abrir_si_no_existe) {
		Cerrar();
		_Path = nPath;
		_Archivo.open(nPath, (Abrir_si_no_existe == TRUE) ?	std::ios::in | std::ios::out | std::ios::trunc | std::ios::binary	:	 std::ios::in | std::ios::out | std::ios::binary);
		return _Archivo.is_open();
	}

	// Función que abre el archivo para lectura (si ya habia un archivo abierto lo cierra)
	const BOOL DArchivoBinario::AbrirArchivoLectura(const wchar_t* nPath) {
		Cerrar();
		_Path = nPath;
		_Archivo.open(nPath, std::ios::in | std::ios::binary);
		return _Archivo.is_open();
	}

	// Función que abre el archivo para escritura (si ya habia un archivo abierto lo cierra)
	const BOOL DArchivoBinario::AbrirArchivoEscritura(const wchar_t* nPath) {
		Cerrar();
		_Path = nPath;
		_Archivo.open(nPath, std::ios::out | std::ios::binary);
		return _Archivo.is_open();
	}

	// Función que cierra el archivo y libera la memória
	void DArchivoBinario::Cerrar(void) {
		if (_Archivo.is_open() == true)
			_Archivo.close();
	}

	// Función que devuelve TRUE si hay algun archivo abierto por este objeto, FALSE en caso contrario
	const BOOL DArchivoBinario::EstaAbierto(void) {
		return _Archivo.is_open();
	}

	// Función que devuelve el tamaño del archivo en bytes en formato 64 bits
/*	const LARGE_INTEGER DArchivoBinario::Longitud64(void) {
		static LARGE_INTEGER Tam;
		GetFileSizeEx(_Archivo, &Tam);
		return Tam;
	}*/

	// Función que devuelve el tamaño del archivo en bytes en formato 32 bits
	const std::streamsize DArchivoBinario::Longitud(void) {
//		size_t Pos = (size_t)_Archivo.tellg();
		_Archivo.clear();
		_Archivo.seekg(0, _Archivo.end);
		std::streamsize Tam = _Archivo.tellg();
		_Archivo.seekg(0, _Archivo.beg);
		return Tam;
	}

	// Función que asigna la posición dentro del archivo
	const std::streampos DArchivoBinario::PosicionLectura(const long Pos, const BOOL Desde_El_Final) {
		_Archivo.seekg(Pos, (Desde_El_Final == FALSE) ? std::ios_base::beg : std::ios_base::end);
		return _Archivo.tellg();
	}

	// Devuelve la posición desde el principio dentro del archivo
	const std::streampos DArchivoBinario::PosicionLectura(void) {
		return _Archivo.tellg();
	}

	// Función que asigna la posición dentro del archivo
	const std::streampos DArchivoBinario::PosicionEscritura(const long Pos, const BOOL Desde_El_Final) {
		_Archivo.seekp(Pos, (Desde_El_Final == FALSE) ? std::ios_base::beg : std::ios_base::end);
		return _Archivo.tellp();
	}

	// Devuelve la posición desde el principio dentro del archivo
	const std::streampos DArchivoBinario::PosicionEscritura(void) {
		return _Archivo.tellp();
	}

	// Función para leer datos del archivo, devuelve los caracteres leidos
	const size_t DArchivoBinario::Leer(char *Buffer, const size_t LongitudEnCaracteres) {
		if (!_Archivo.read(Buffer, LongitudEnCaracteres * sizeof(char))) {
			char c[256];
			strerror_s(c, errno);
			return 0;
		}
		return (size_t)_Archivo.gcount();
	}

	// Función para guardar datos en el archivo
	void DArchivoBinario::Guardar(const char *Buffer, const size_t LongitudEnCaracteres) {
		_Archivo.write(Buffer, LongitudEnCaracteres);
	}


	// Función para leer un std::wstring
	const size_t DArchivoBinario::LeerString(std::wstring &Texto) {
		size_t   TamStr = 0;
		wchar_t *TmpChar = nullptr;
		Leer<size_t>(&TamStr);// Leo el tamaño
		if (TamStr == 0)
			return FALSE; 
		TmpChar = new wchar_t[TamStr + 1];
		size_t Ret = Leer<wchar_t>(TmpChar, TamStr * sizeof(wchar_t));
		if (Ret != TamStr * sizeof(wchar_t)) {
			delete[] TmpChar;
			return FALSE;
		}
		TmpChar[TamStr] = L'\0';
		Texto = TmpChar;
		delete[] TmpChar;
		return Ret; // + sizeof(size_t);
	}


	// Función para guardar un std::wstring
	const BOOL DArchivoBinario::GuardarString(std::wstring &Texto, const BOOL GuardarTam) {
		BOOL	Ret = 0;
		if (GuardarTam == TRUE) {
			size_t T = Texto.size();
			Guardar<size_t>(&T);
		}
		Guardar<wchar_t>(Texto.c_str(), Texto.size() * sizeof(wchar_t));
		return TRUE;
	}


	// Función para leer un std::wstring
	const size_t DArchivoBinario::LeerString(std::string &Texto) {
		size_t  TamStr = 0;
		char   *TmpChar = nullptr;
		Leer<size_t>(&TamStr); // Leo el tamaño
		if (TamStr == 0)
			return FALSE; 
		TmpChar = new char[TamStr + 1];
		size_t Ret = Leer<char>(TmpChar, TamStr * sizeof(char));
		if (Ret != TamStr) {
			delete[] TmpChar;
			return FALSE;
		}
		TmpChar[TamStr] = '\0';
		Texto = TmpChar;
		delete[] TmpChar;
		return Ret; // + sizeof(size_t);
	}

	// Función para guardar un std::wstring
	const BOOL DArchivoBinario::GuardarString(std::string &Texto, const BOOL GuardarTam) {
		BOOL	Ret = 0;
		if (GuardarTam == TRUE) {
			size_t T = Texto.size();
			Guardar<size_t>(&T);
		}
		Guardar<char>(Texto.c_str(), Texto.size() * sizeof(char));
		return TRUE;
	}



#define BUFSIZE 1024
#define MD5LEN  16

	// Función que genera y devuelve el hash MD5 del archivo
	// https://docs.microsoft.com/es-es/windows/win32/seccrypto/example-c-program--creating-an-md-5-hash-from-file-content
	// https://stackoverflow.com/questions/13256446/compute-md5-hash-value-by-c-winapi
	std::wstring &DArchivoBinario::MD5(void) {
		static std::wstring	Ret;

		// El archivo no es válido
		if (_Archivo.is_open() == FALSE) return Ret;

//		BOOL				bResult = FALSE;
		HCRYPTPROV			hProv = 0;
		HCRYPTHASH			hHash = 0;
		DWORD				cbRead = 0;
		DWORD				cbHash = 0;
		CHAR				rgbDigits[] = "0123456789abcdef";
		BYTE				rgbFile[BUFSIZE];
		BYTE				rgbHash[MD5LEN];

		Ret.resize(0);

		// Situo el puntero del archivo al principio
		PosicionLectura(0);

		// Get handle to the crypto provider
		if (!CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT)) {
			return Ret;
		}

		if (!CryptCreateHash(hProv, CALG_MD5, 0, 0, &hHash)) {
			CryptReleaseContext(hProv, 0);
			return Ret;
		}

		

//		while (bResult = ReadFile(_Archivo, rgbFile, BUFSIZE, &cbRead, NULL)) {
		while (FinalDelArchivo() == FALSE) {
			Leer<BYTE>(rgbFile, BUFSIZE * sizeof(BYTE));
			cbRead = (DWORD)_Archivo.gcount();
			// Si no se han leido caracteres, salgo del bucle
			if (0 == cbRead) {
				break;
			}

			if (!CryptHashData(hHash, rgbFile, cbRead, 0)) {
				CryptReleaseContext(hProv, 0);
				CryptDestroyHash(hHash);
				_Archivo.clear();
				return Ret;
			}
		}

/*		if (!bResult) {
			CryptReleaseContext(hProv, 0);
			CryptDestroyHash(hHash);
			return Ret;
		}*/

		cbHash = MD5LEN;
		if (CryptGetHashParam(hHash, HP_HASHVAL, rgbHash, &cbHash, 0)) {
			for (DWORD i = 0; i < cbHash; i++) {
				Ret += rgbDigits[rgbHash[i] >> 4];
				Ret += rgbDigits[rgbHash[i] & 0xf];
			}
		}
		else {
			Ret.resize(0);
		}

		CryptDestroyHash(hHash);
		CryptReleaseContext(hProv, 0);

		_Archivo.clear();

		return Ret;
	}




	std::string &DArchivoBinario::MD5_char(void) {
		static std::string	Ret;

		// El archivo no es válido
		if (_Archivo.is_open() == FALSE) return Ret;

//		BOOL				bResult = FALSE;
		HCRYPTPROV			hProv = 0;
		HCRYPTHASH			hHash = 0;
		DWORD				cbRead = 0;
		DWORD				cbHash = 0;
		CHAR				rgbDigits[] = "0123456789abcdef";
		BYTE				rgbFile[BUFSIZE];
		BYTE				rgbHash[MD5LEN];

		Ret.resize(0);

		// Situo el puntero del archivo al principio
		PosicionLectura(0);

//		size_t p = PosicionLectura();
//		p = PosicionEscritura();
//		PosicionEscritura(0);

		// Get handle to the crypto provider
		if (!CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT)) {
			return Ret;
		}

		if (!CryptCreateHash(hProv, CALG_MD5, 0, 0, &hHash)) {
			CryptReleaseContext(hProv, 0);
			return Ret;
		}

		//		while (bResult = ReadFile(_Archivo, rgbFile, BUFSIZE, &cbRead, NULL)) {
		while (FinalDelArchivo() == FALSE) {
			Leer((char *)&rgbFile, BUFSIZE);
			cbRead = (DWORD)_Archivo.gcount();
			// Si no se han leido caracteres, salgo del bucle
			if (0 == cbRead) {
				break;
			}

			if (!CryptHashData(hHash, rgbFile, cbRead, 0)) {
				CryptReleaseContext(hProv, 0);
				CryptDestroyHash(hHash);
				_Archivo.clear();
				return Ret;
			}
		}

/*		if (!bResult) {
			CryptReleaseContext(hProv, 0);
			CryptDestroyHash(hHash);
			return Ret;
		}*/

		cbHash = MD5LEN;
		if (CryptGetHashParam(hHash, HP_HASHVAL, rgbHash, &cbHash, 0)) {
			for (DWORD i = 0; i < cbHash; i++) {
				Ret += rgbDigits[rgbHash[i] >> 4];
				Ret += rgbDigits[rgbHash[i] & 0xf];
			}
		}
		else {
			Ret.resize(0);
		}

		CryptDestroyHash(hHash);
		CryptReleaseContext(hProv, 0);
		_Archivo.clear();

		return Ret;
	}

};