#include "pch.h"
#include "DArchivoBinario.h"
#include <wincrypt.h>

namespace DWL {

	// Constructor por defecto
	DArchivoBinario::DArchivoBinario(void) : _Archivo(INVALID_HANDLE_VALUE), _FinalDelArchivo(FALSE), _BytesLeidos(0) {
	}
	
	// Constructor que abre el archivo especificado para lectura y escritura
	DArchivoBinario::DArchivoBinario(const wchar_t* nPath, const BOOL Abrir_si_no_existe) : _Archivo(INVALID_HANDLE_VALUE), _FinalDelArchivo(FALSE), _BytesLeidos(0) {
		AbrirArchivo(nPath, Abrir_si_no_existe);
	}

	// Constructor que abre el archivo especificado para lectura y escritura
	DArchivoBinario::DArchivoBinario(std::wstring &nPath, const BOOL Abrir_si_no_existe) : _Archivo(INVALID_HANDLE_VALUE), _FinalDelArchivo(FALSE), _BytesLeidos(0) {
		AbrirArchivo(nPath.c_str(), Abrir_si_no_existe);
	}

	// Destructor (cierra el archivo y elimina los datos de la memória)
	DArchivoBinario::~DArchivoBinario(void) {
		Cerrar();
	}

	// Función que abre el archivo para lectura y escritura (si ya habia un archivo abierto lo cierra)
	const BOOL DArchivoBinario::AbrirArchivo(const wchar_t* nPath, const BOOL Abrir_si_no_existe) {
		Cerrar();
		_Path			 = nPath;
		_FinalDelArchivo = FALSE;
		_Archivo = CreateFile(nPath, FILE_READ_DATA | FILE_WRITE_DATA, 0, 0, (Abrir_si_no_existe == TRUE) ? OPEN_ALWAYS : OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
		return (_Archivo != INVALID_HANDLE_VALUE) ? TRUE : FALSE;
	}

	// Función que abre el archivo para lectura (si ya habia un archivo abierto lo cierra)
	const BOOL DArchivoBinario::AbrirArchivoLectura(const wchar_t* nPath) {
		Cerrar();
		_Path			 = nPath;
		_FinalDelArchivo = FALSE;
		_Archivo = CreateFile(nPath, FILE_READ_DATA, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
		return (_Archivo != INVALID_HANDLE_VALUE) ? TRUE : FALSE;
	}

	// Función que abre el archivo para escritura (si ya habia un archivo abierto lo cierra)
	const BOOL DArchivoBinario::AbrirArchivoEscritura(const wchar_t* nPath) {
		Cerrar();
		_Path			 = nPath;
		_FinalDelArchivo = FALSE;
		_Archivo = CreateFile(nPath, FILE_WRITE_DATA, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
		return (_Archivo != INVALID_HANDLE_VALUE) ? TRUE : FALSE;
	}

	// Función que cierra el archivo y libera la memória
	void DArchivoBinario::Cerrar(void) {
		if (_Archivo != INVALID_HANDLE_VALUE) CloseHandle(_Archivo);
		_Archivo	  = INVALID_HANDLE_VALUE;
		_BytesLeidos  = 0;
		_Path		  = L"";
	}

	// Función que devuelve TRUE si hay algun archivo abierto por este objeto, FALSE en caso contrario
	const BOOL DArchivoBinario::EstaAbierto(void) {
		return (_Archivo != INVALID_HANDLE_VALUE) ? TRUE : FALSE;
	}

	// Función que devuelve el tamaño del archivo en bytes en formato 64 bits
	const LARGE_INTEGER DArchivoBinario::Longitud64(void) {
		static LARGE_INTEGER Tam;
		GetFileSizeEx(_Archivo, &Tam);
		return Tam;
	}

	// Función que devuelve el tamaño del archivo en bytes en formato 32 bits
	const DWORD DArchivoBinario::Longitud(void) {
		return GetFileSize(_Archivo, NULL);
	}

	// Función que asigna la posición dentro del archivo
	const DWORD DArchivoBinario::Posicion(const long Pos, const BOOL Desde_El_Final) {
		return SetFilePointer(_Archivo, Pos, NULL, (Desde_El_Final == FALSE) ? FILE_BEGIN : FILE_END);
	}

	// Devuelve la posición desde el principio dentro del archivo
	const DWORD DArchivoBinario::Posicion(void) {
		return SetFilePointer(_Archivo, 0, NULL, FILE_CURRENT);
	}

	// Función para leer datos del archivo
	const BOOL DArchivoBinario::Leer(char *Buffer, const DWORD LongitudEnCaracteres) {
		_BytesLeidos = 0;
		BOOL	Ret = ReadFile(_Archivo, reinterpret_cast<LPVOID>(Buffer), LongitudEnCaracteres * sizeof(char), &_BytesLeidos, NULL);
		_FinalDelArchivo = (_BytesLeidos < LongitudEnCaracteres) ? TRUE : FALSE;
		return Ret;
	}

	// Función para guardar datos en el archivo
	const BOOL DArchivoBinario::Guardar(const char *Buffer, const DWORD LongitudEnCaracteres) {
		DWORD Guardado = 0;
		return WriteFile(_Archivo, reinterpret_cast<LPCVOID>(Buffer), LongitudEnCaracteres * sizeof(char), &Guardado, NULL);
	}

	// Función para leer un std::wstring
	const BOOL DArchivoBinario::LeerString(std::wstring& Texto) {
		size_t   TamStr = 0;
		wchar_t* TmpChar = NULL;
		if (Leer<size_t>(TamStr) == FALSE) return FALSE; // Leo el tamaño
		TmpChar = new wchar_t[TamStr + 1];
		BOOL Ret = Leer(TmpChar, static_cast<DWORD>(TamStr * sizeof(wchar_t)));
		if (Ret != static_cast<int>(TamStr * sizeof(wchar_t))) {
			delete[] TmpChar;
			return FALSE;
		}
		TmpChar[TamStr] = 0;
		Texto = TmpChar;
		delete[] TmpChar;
		return Ret; // + sizeof(size_t);
	}

	// Función para guardar un std::wstring
	const BOOL DArchivoBinario::GuardarString(std::wstring& Texto, const BOOL GuardarTam) {
		BOOL	Ret = 0;
		if (GuardarTam == TRUE) {
			Ret = Guardar<size_t>(Texto.size(), sizeof(size_t));
			if (Ret == 0)	return FALSE;
		}
		return Guardar(&Texto[0], static_cast<DWORD>(Texto.size() * sizeof(wchar_t)));
	}


	// Función para leer un std::wstring
	const BOOL DArchivoBinario::LeerString(std::string& Texto) {
		size_t  TamStr = 0;
		char   *TmpChar = NULL;
		if (Leer<size_t>(TamStr) == FALSE) return FALSE; // Leo el tamaño
		TmpChar = new char[TamStr + 1];
		BOOL Ret = Leer(TmpChar, static_cast<DWORD>(TamStr * sizeof(char)));
		if (Ret != static_cast<int>(TamStr * sizeof(char))) {
			delete[] TmpChar;
			return FALSE;
		}
		TmpChar[TamStr] = 0;
		Texto = TmpChar;
		delete[] TmpChar;
		return Ret; // + sizeof(size_t);
	}

	// Función para guardar un std::wstring
	const BOOL DArchivoBinario::GuardarString(std::string& Texto, const BOOL GuardarTam) {
		BOOL	Ret = 0;
		if (GuardarTam == TRUE) {
			Ret = Guardar<size_t>(Texto.size(), sizeof(size_t));
			if (Ret == 0)	return FALSE;
		}
		return Guardar(&Texto[0], static_cast<DWORD>(Texto.size() * sizeof(char)));
	}



#define BUFSIZE 1024
#define MD5LEN  16

	// Función que genera y devuelve el hash MD5 del archivo
	// https://docs.microsoft.com/es-es/windows/win32/seccrypto/example-c-program--creating-an-md-5-hash-from-file-content
	// https://stackoverflow.com/questions/13256446/compute-md5-hash-value-by-c-winapi
	std::wstring& DArchivoBinario::MD5(void) {
		static std::wstring	Ret;

		// El archivo no es válido
		if (_Archivo == INVALID_HANDLE_VALUE) return Ret;

		BOOL				bResult = FALSE;
		HCRYPTPROV			hProv = 0;
		HCRYPTHASH			hHash = 0;
		DWORD				cbRead = 0;
		DWORD				cbHash = 0;
		CHAR				rgbDigits[] = "0123456789abcdef";
		BYTE				rgbFile[BUFSIZE];
		BYTE				rgbHash[MD5LEN];

		Ret.resize(0);

		// Situo el puntero del archivo al principio
		Posicion(0);

		// Get handle to the crypto provider
		if (!CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT)) {
			return Ret;
		}

		if (!CryptCreateHash(hProv, CALG_MD5, 0, 0, &hHash)) {
			CryptReleaseContext(hProv, 0);
			return Ret;
		}

		while (bResult = ReadFile(_Archivo, rgbFile, BUFSIZE, &cbRead, NULL)) {
			// Si no se han leido caracteres, salgo del bucle
			if (0 == cbRead) {
				break;
			}

			if (!CryptHashData(hHash, rgbFile, cbRead, 0)) {
				CryptReleaseContext(hProv, 0);
				CryptDestroyHash(hHash);
				return Ret;
			}
		}

		if (!bResult) {
			CryptReleaseContext(hProv, 0);
			CryptDestroyHash(hHash);
			return Ret;
		}

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

		return Ret;
	}


	std::string &DArchivoBinario::MD5_char(void) {
		static std::string	Ret;

		// El archivo no es válido
		if (_Archivo == INVALID_HANDLE_VALUE) return Ret;

		BOOL				bResult = FALSE;
		HCRYPTPROV			hProv = 0;
		HCRYPTHASH			hHash = 0;
		DWORD				cbRead = 0;
		DWORD				cbHash = 0;
		CHAR				rgbDigits[] = "0123456789abcdef";
		BYTE				rgbFile[BUFSIZE];
		BYTE				rgbHash[MD5LEN];

		Ret.resize(0);

		// Situo el puntero del archivo al principio
		Posicion(0);

		// Get handle to the crypto provider
		if (!CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT)) {
			return Ret;
		}

		if (!CryptCreateHash(hProv, CALG_MD5, 0, 0, &hHash)) {
			CryptReleaseContext(hProv, 0);
			return Ret;
		}

		while (bResult = ReadFile(_Archivo, rgbFile, BUFSIZE, &cbRead, NULL)) {
			// Si no se han leido caracteres, salgo del bucle
			if (0 == cbRead) {
				break;
			}

			if (!CryptHashData(hHash, rgbFile, cbRead, 0)) {
				CryptReleaseContext(hProv, 0);
				CryptDestroyHash(hHash);
				return Ret;
			}
		}

		if (!bResult) {
			CryptReleaseContext(hProv, 0);
			CryptDestroyHash(hHash);
			return Ret;
		}

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

		return Ret;
	}

};