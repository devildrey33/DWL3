#ifndef DARCHIVOBINARIO_H
#define DARCHIVOBINARIO_H

#include <string>
#include <fstream>

namespace DWL {

	class DArchivoBinario {
	  public :	//////////////////////////////
											// Constructor por defecto
											DArchivoBinario(void);
											// Constructor que abre el archivo especificado para lectura y escritura
											DArchivoBinario(const wchar_t *nPath, const BOOL Abrir_si_no_existe = TRUE);
											// Constructor que abre el archivo especificado para lectura y escritura
											DArchivoBinario(std::wstring &Path, const BOOL Abrir_si_no_existe = TRUE);
											// Destructor (cierra el archivo y elimina los datos de la mem�ria)
			                               ~DArchivoBinario(void);
											// Funci�n que abre el archivo para lectura y escritura (si ya habia un archivo abierto lo cierra)
		const BOOL							AbrirArchivo(const wchar_t* nPath, const BOOL Abrir_si_no_existe = FALSE);
											// Funci�n que abre el archivo para lectura (si ya habia un archivo abierto lo cierra)
		const BOOL							AbrirArchivoLectura(const wchar_t* nPath);
											// Funci�n que abre el archivo para escritura (si ya habia un archivo abierto lo cierra)
		const BOOL							AbrirArchivoEscritura(const wchar_t* nPath);
											// Funci�n que cierra el archivo y libera la mem�ria
		void								Cerrar(void);
											// Funci�n que devuelve TRUE si hay algun archivo abierto por este objeto, FALSE en caso contrario
		const BOOL							EstaAbierto(void);
											// Funci�n que devuelve el tama�o del archivo en bytes en formato 64 bits
//		const LARGE_INTEGER					Longitud64(void);
											// Funci�n que devuelve el tama�o del archivo en bytes en formato 32 bits
		const std::streamsize				Longitud(void);
											// Funci�n que asigna la posici�n de lectura dentro del archivo
		const std::streampos				PosicionLectura(const long Pos, const BOOL Desde_El_Final = FALSE);
											// Devuelve la posici�n de lectura desde el principio dentro del archivo
		const std::streampos				PosicionLectura(void);
											// Funci�n que asigna la posici�n de escritura dentro del archivo
		const std::streampos				PosicionEscritura(const long Pos, const BOOL Desde_El_Final = FALSE);
											// Devuelve la posici�n de escritura desde el principio dentro del archivo
		const std::streampos				PosicionEscritura(void);
											// Funci�n que devuelve si se ha llegado al final del archivo en la ultima operaci�n de lectura
		inline const BOOL					FinalDelArchivo(void)	{ return _Archivo.eof();	}
											// Funci�n que devuelve el total de bytes leidos en la ultima operaci�n de lectura
		inline const size_t					BytesLeidos(void)		{ return (size_t)_Archivo.gcount();	}
											// Funci�n que devuelve el path del archivo
		inline const wchar_t               *Path(void)				{ return _Path.c_str();		}	
											// Funci�n para leer datos del archivo
		const size_t						Leer(char *Buffer, const size_t Longitud_en_caracteres);
											// Funci�n para guardar datos en el archivo
		void								Guardar(const char *Buffer, const size_t Longitud_en_caracteres);
											// Funci�n plantilla para leer datos del tipo especificado 
		template <typename T> const size_t	Leer(T *Buffer, const size_t Longitud_en_caracteres = sizeof(T)) { 
												_Archivo.read((char *)Buffer, Longitud_en_caracteres);
												return (size_t)_Archivo.gcount();
											};
											// Funci�n plantilla para guardar datos del tipo especificado
		template <typename T> void			Guardar(const T *Buffer, const size_t Longitud_en_caracteres = sizeof(T)) {
												char *B = (char *)Buffer;
												_Archivo.write((char *)Buffer, Longitud_en_caracteres);
											};
											// Funci�n para leer un std::wstring
		const size_t						LeerString(std::wstring &Texto);
											// Funci�n para guardar un std::wstring (Si GuardarTam es TRUE, se guardar� el tama�o del string antes del string)
		const BOOL							GuardarString(std::wstring &Texto, const BOOL GuardarTam = TRUE);
											// Funci�n para leer un std::wstring
		const size_t						LeerString(std::string &Texto);
											// Funci�n para guardar un std::wstring (Si GuardarTam es TRUE, se guardar� el tama�o del string antes del string)
		const BOOL							GuardarString(std::string &Texto, const BOOL GuardarTam = TRUE);
											// Funci�n que genera y devuelve el hash MD5 del archivo en un string de 16 bytes
		std::wstring                       &MD5(void);
											// Funci�n que genera y devuelve el hash MD5 del archivo en un string de 8 bytes
		std::string                        &MD5_char(void);
	  protected:
											// Path del archivo
		std::wstring                       _Path;
											// FileStream que se usa internamente para las operaciones lectura/escritura de esta clase
		std::fstream	                   _Archivo;

	};

	/*
	class DArchivoBinario {
	  public :	//////////////////////////////
											// Constructor por defecto
											DArchivoBinario(void);
											// Constructor que abre el archivo especificado para lectura y escritura
											DArchivoBinario(const wchar_t *nPath, const BOOL Abrir_si_no_existe = TRUE);
											// Constructor que abre el archivo especificado para lectura y escritura
											DArchivoBinario(std::wstring &Path, const BOOL Abrir_si_no_existe = TRUE);
											// Destructor (cierra el archivo y elimina los datos de la mem�ria)
			                               ~DArchivoBinario(void);
											// Funci�n que abre el archivo para lectura y escritura (si ya habia un archivo abierto lo cierra)
		const BOOL							AbrirArchivo(const wchar_t* nPath, const BOOL Abrir_si_no_existe = FALSE);
											// Funci�n que abre el archivo para lectura (si ya habia un archivo abierto lo cierra)
		const BOOL							AbrirArchivoLectura(const wchar_t* nPath);
											// Funci�n que abre el archivo para escritura (si ya habia un archivo abierto lo cierra)
		const BOOL							AbrirArchivoEscritura(const wchar_t* nPath);
											// Funci�n que cierra el archivo y libera la mem�ria
		void								Cerrar(void);
											// Funci�n que devuelve TRUE si hay algun archivo abierto por este objeto, FALSE en caso contrario
		const BOOL							EstaAbierto(void);
											// Funci�n que devuelve el tama�o del archivo en bytes en formato 64 bits
		const LARGE_INTEGER					Longitud64(void);
											// Funci�n que devuelve el tama�o del archivo en bytes en formato 32 bits
		const DWORD							Longitud(void);
											// Funci�n que asigna la posici�n dentro del archivo
		const DWORD							Posicion(const long Pos, const BOOL Desde_El_Final = FALSE);
											// Devuelve la posici�n desde el principio dentro del archivo
		const DWORD							Posicion(void);
											// Funci�n que devuelve si se ha llegado al final del archivo en la ultima operaci�n de lectura
		inline const BOOL					FinalDelArchivo(void)	{ return _FinalDelArchivo;  }
											// Funci�n que devuelve el total de bytes leidos en la ultima operaci�n de lectura
		inline const DWORD					BytesLeidos(void)		{ return _BytesLeidos;		}
											// Funci�n que devuelve el path del archivo
		inline const wchar_t               *Path(void)				{ return _Path.c_str();		}	
											// Funci�n para leer datos del archivo
		const BOOL							Leer(char *Buffer, const DWORD Longitud_en_caracteres);
											// Funci�n para guardar datos en el archivo
		const BOOL							Guardar(const char *Buffer, const DWORD Longitud_en_caracteres);
											// Funci�n plantilla para leer datos del tipo especificado (NO USAR CON PUNTEROS)
		template <typename T> const BOOL	Leer(T &Buffer, const DWORD Longitud_en_bytes = sizeof(T)) { 
												_BytesLeidos = 0;
												BOOL Ret = ReadFile(_Archivo, reinterpret_cast<char *>(Buffer), Longitud_en_bytes, &_BytesLeidos, NULL);
												_FinalDelArchivo = (_BytesLeidos < Longitud_en_bytes) ? TRUE : FALSE;
												return Ret;
											};
											// Funci�n plantilla para guardar datos del tipo especificado
		template <typename T> const BOOL	Guardar(const T Buffer, const DWORD Longitud_en_bytes = sizeof(T)) {
												DWORD Guardado = 0;												
												WriteFile(_Archivo, reinterpret_cast<char*>(Buffer), Longitud_en_bytes, &Guardado, NULL);
												return static_cast<BOOL>(Guardado == Longitud_en_bytes);
											};
											// Funci�n para leer un std::wstring
		const BOOL							LeerString(std::wstring &Texto);
											// Funci�n para guardar un std::wstring (Si GuardarTam es TRUE, se guardar� el tama�o del string antes del string)
		const BOOL							GuardarString(std::wstring &Texto, const BOOL GuardarTam = TRUE);
											// Funci�n para leer un std::wstring
		const BOOL							LeerString(std::string &Texto);
											// Funci�n para guardar un std::wstring (Si GuardarTam es TRUE, se guardar� el tama�o del string antes del string)
		const BOOL							GuardarString(std::string &Texto, const BOOL GuardarTam = TRUE);
											// Funci�n que genera y devuelve el hash MD5 del archivo en un string de 16 bytes
		std::wstring                       &MD5(void);
											// Funci�n que genera y devuelve el hash MD5 del archivo en un string de 8 bytes
		std::string                        &MD5_char(void);
	  protected :  ///////////////////////////
											// Handle para el archivo
		HANDLE                             _Archivo;
						   					// Miembro que contiene los bytes leidos en la ultima operaci�n de lectura
		DWORD                              _BytesLeidos;
								 			// Variable que indica si se ha llegado al final del archivo en una operaci�n de lectura
		BOOL                               _FinalDelArchivo;
											// Path del archivo
		std::wstring                       _Path;
	};
	*/
};

#endif
