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
											// Destructor (cierra el archivo y elimina los datos de la memória)
			                               ~DArchivoBinario(void);
											// Función que abre el archivo para lectura y escritura (si ya habia un archivo abierto lo cierra)
		const BOOL							AbrirArchivo(const wchar_t* nPath, const BOOL Abrir_si_no_existe = FALSE);
											// Función que abre el archivo para lectura (si ya habia un archivo abierto lo cierra)
		const BOOL							AbrirArchivoLectura(const wchar_t* nPath);
											// Función que abre el archivo para escritura (si ya habia un archivo abierto lo cierra)
		const BOOL							AbrirArchivoEscritura(const wchar_t* nPath);
											// Función que cierra el archivo y libera la memória
		void								CerrarArchivo(void);
											// Función que devuelve TRUE si hay algun archivo abierto por este objeto, FALSE en caso contrario
		const BOOL							EstaAbierto(void);
											// Función que devuelve el tamaño del archivo en bytes en formato 64 bits
		const LARGE_INTEGER					Longitud64(void);
											// Función que devuelve el tamaño del archivo en bytes en formato 32 bits
		const DWORD							Longitud(void);
											// Función que asigna la posición dentro del archivo
		const DWORD							Posicion(const long Pos, const BOOL Desde_El_Final = FALSE);
											// Devuelve la posición desde el principio dentro del archivo
		const DWORD							Posicion(void);
											// Función que devuelve si se ha llegado al final del archivo en la ultima operación de lectura
		inline const BOOL					FinalDelArchivo(void)	{ return _FinalDelArchivo;  }
											// Función que devuelve el total de bytes leidos en la ultima operación de lectura
		inline const DWORD					BytesLeidos(void)		{ return _BytesLeidos;		}
											// Función que devuelve el path del archivo
		inline const wchar_t               *Path(void)				{ return _Path.c_str();		}	
											// Función para leer datos del archivo
		const BOOL							Leer(char *Buffer, const size_t Longitud_en_caracteres);
											// Función para guardar datos en el archivo
		const BOOL							Guardar(const char *Buffer, const size_t Longitud_en_caracteres);
											// Función plantilla para leer datos del tipo especificado (NO USAR CON PUNTEROS)
		template <typename T> const BOOL	Leer(T &Buffer, const size_t Longitud_en_bytes = sizeof(T)) { 
												_BytesLeidos = 0;
												BOOL Ret = ReadFile(_Archivo, reinterpret_cast<LPVOID>(Buffer), LongitudEnBytes, &_CaracteresLeidos, NULL);
												_FinalDelArchivo = (_BytesLeidos < LongitudEnCaracteres) ? TRUE : FALSE;
												return Ret;
											};
											// Función plantilla para guardar datos del tipo especificado
		template <typename T> const BOOL	Guardar(const T &Buffer, const size_t Longitud_en_bytes = sizeof(T)) {
												DWORD Guardado = 0;
												WriteFile(_Archivo, reinterpret_cast<LPCVOID>(Buffer), LongitudEnBytes, &Guardado, NULL);
												return static_cast<BOOL>(Guardado == LongitudEnBytes);
											};
											// Función para leer un std::wstring
		const BOOL							Leer(std::wstring &Texto);
											// Función para guardar un std::wstring
		const BOOL							Guardar(std::wstring &Texto);
											// Función que genera y devuelve el hash MD5 del archivo
		std::wstring                       &MD5(void);
	  protected :  ///////////////////////////
											// Handle para el archivo
		HANDLE                             _Archivo;
						   					// Miembro que contiene los bytes leidos en la ultima operación de lectura
		DWORD                              _BytesLeidos;
								 			// Variable que indica si se ha llegado al final del archivo en una operación de lectura
		BOOL                               _FinalDelArchivo;
											// Path del archivo
		std::wstring                       _Path;
	};

};

#endif
