#ifndef DREGISTRO_H
#define DREGISTRO_H

#define DREGISTRO_MOSTRARDEBUG	FALSE

#include <string>

namespace DWL {
	class DRegistro {
		public : ///////// Miembros publicos
						//
						// Constructor
						// DRegistro(void);
						DRegistro(void);
						//
						// Destructor
						// ~DRegistro(void);
					   ~DRegistro(void);						
						//
						// Función que agrega / modifica un valor tipo string del registro 
						// static LONG AsignarValor_String(HKEY Root, const TCHAR *Key, const TCHAR *Nombre, TCHAR *TextoAGuardar);
		 static LONG	AsignarValor_String(HKEY Root, const TCHAR *Key, const TCHAR *Nombre, const TCHAR *TextoAGuardar);
						//
						// Función que agrega / modifica un valor tipo string del registro 
						// static LONG AsignarValor_String(HKEY Root, const TCHAR *Key, const TCHAR *Nombre, TCHAR *TextoAGuardar);
		 static LONG	AsignarValor_DWORD(HKEY Root, const TCHAR *Key, const TCHAR *Nombre, DWORD DWORDAGuardar);
						//
						// Función para obtener el valor de un string del registro
						// static LONG ObtenerValor_String(HKEY Root, const TCHAR *Key, const TCHAR *Nombre, TCHAR *Resultado, DWORD ResTam);
		 static LONG	ObtenerValor_String(HKEY Root, const TCHAR *Key, const TCHAR *Nombre, TCHAR *Resultado, DWORD ResTam);
						//
						// Función para obtener el valor de un string del registro
						// static LONG ObtenerValor_String(HKEY Root, const TCHAR *Key, const TCHAR *Nombre, std::wstring &Resultado);
		 static LONG	ObtenerValor_String(HKEY Root, const TCHAR *Key, const TCHAR *Nombre, std::wstring &Resultado);
						//
						// Función para eliminar un valor del registro de cualquier tipo
						//  static LONG EliminarValor(HKEY Root, const TCHAR *Key, const TCHAR *Nombre);
		 static LONG    EliminarValor(HKEY Root, const TCHAR *Key, const TCHAR *Nombre);
						//
						// Función para eliminar una clave del registro de cualquier tipo
						//  static LONG EliminarClave(HKEY Root, const TCHAR *Key, const TCHAR *Nombre);
		 static LONG    EliminarClave(HKEY Root, const TCHAR *Key, const TCHAR *Nombre);
		};
		////////////////// Fin DWLRegistro
	}
#endif