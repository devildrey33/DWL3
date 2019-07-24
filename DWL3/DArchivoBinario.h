#ifndef DARCHIVOBINARIO_H
#define DARCHIVOBINARIO_H

#include <string>

namespace DWL {

	class DArchivoBinario {
	  public :	//////////////////////////////
											// Constructor por defecto
											DArchivoBinario(void);
											// Constructor que abre el archivo especificado para lectura y escritura
											DArchivoBinario(const wchar_t *nPath, const BOOL Abrir_si_no_existe = TRUE);
											// Destructor (cierra el archivo y elimina los datos de la mem�ria)
			                               ~DArchivoBinario(void);
											// Funci�n que abre el archivo para lectura y escritura (si ya habia un archivo abierto lo cierra)
		const BOOL							AbrirArchivo(const wchar_t* nPath, const BOOL Abrir_si_no_existe = FALSE);
											// Funci�n que abre el archivo para lectura (si ya habia un archivo abierto lo cierra)
		const BOOL							AbrirArchivoLectura(const wchar_t* nPath);
											// Funci�n que abre el archivo para escritura (si ya habia un archivo abierto lo cierra)
		const BOOL							AbrirArchivoEscritura(const wchar_t* nPath);
											// Funci�n que cierra el archivo y libera la mem�ria
		void								CerrarArchivo(void);
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
		const BOOL							Leer(char *Buffer, const size_t Longitud_en_caracteres);
											// Funci�n para guardar datos en el archivo
		const BOOL							Guardar(const char *Buffer, const size_t Longitud_en_caracteres);
											// Funci�n plantilla para leer datos del tipo especificado (NO USAR CON PUNTEROS)
		template <typename T> const BOOL	Leer(T &Buffer, const size_t Longitud_en_bytes = sizeof(T)) { 
												_BytesLeidos = 0;
												BOOL Ret = ReadFile(_Archivo, reinterpret_cast<LPVOID>(Buffer), LongitudEnBytes, &_CaracteresLeidos, NULL);
												_FinalDelArchivo = (_BytesLeidos < LongitudEnCaracteres) ? TRUE : FALSE;
												return Ret;
											};
											// Funci�n plantilla para guardar datos del tipo especificado
		template <typename T> const BOOL	Guardar(const T &Buffer, const size_t Longitud_en_bytes = sizeof(T)) {
												DWORD Guardado = 0;
												WriteFile(_Archivo, reinterpret_cast<LPCVOID>(Buffer), LongitudEnBytes, &Guardado, NULL);
												return static_cast<BOOL>(Guardado == LongitudEnBytes);
											};
											// Funci�n para leer un std::wstring
		const BOOL							Leer(std::wstring &Texto);
											// Funci�n para guardar un std::wstring
		const BOOL							Guardar(std::wstring &Texto);
											// Funci�n que genera y devuelve el hash MD5 del archivo
		std::wstring                       &MD5(void);
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

};

#endif
