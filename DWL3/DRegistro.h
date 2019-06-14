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
						// Funci�n que agrega / modifica un valor tipo string del registro 
						// static LONG AsignarValor_String(HKEY Root, const TCHAR *Key, const TCHAR *Nombre, TCHAR *TextoAGuardar);
		 static LONG	AsignarValor_String(HKEY Root, const TCHAR *Key, const TCHAR *Nombre, const TCHAR *TextoAGuardar);
						//
						// Funci�n que agrega / modifica un valor tipo string del registro 
						// static LONG AsignarValor_String(HKEY Root, const TCHAR *Key, const TCHAR *Nombre, TCHAR *TextoAGuardar);
		 static LONG	AsignarValor_DWORD(HKEY Root, const TCHAR *Key, const TCHAR *Nombre, DWORD DWORDAGuardar);
						//
						// Funci�n para obtener el valor de un string del registro
						// static LONG ObtenerValor_String(HKEY Root, const TCHAR *Key, const TCHAR *Nombre, TCHAR *Resultado, DWORD ResTam);
		 static LONG	ObtenerValor_String(HKEY Root, const TCHAR *Key, const TCHAR *Nombre, TCHAR *Resultado, DWORD ResTam);
						//
						// Funci�n para obtener el valor de un string del registro
						// static LONG ObtenerValor_String(HKEY Root, const TCHAR *Key, const TCHAR *Nombre, std::wstring &Resultado);
		 static LONG	ObtenerValor_String(HKEY Root, const TCHAR *Key, const TCHAR *Nombre, std::wstring &Resultado);
						//
						// Funci�n para eliminar un valor del registro de cualquier tipo
						//  static LONG EliminarValor(HKEY Root, const TCHAR *Key, const TCHAR *Nombre);
		 static LONG    EliminarValor(HKEY Root, const TCHAR *Key, const TCHAR *Nombre);
						//
						// Funci�n para eliminar una clave del registro de cualquier tipo
						//  static LONG EliminarClave(HKEY Root, const TCHAR *Key, const TCHAR *Nombre);
		 static LONG    EliminarClave(HKEY Root, const TCHAR *Key, const TCHAR *Nombre);
		};
		////////////////// Fin DWLRegistro
	}
#endif