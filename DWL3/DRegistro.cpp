#include "pch.h"
#include "DRegistro.h"
#include <winreg.h>
#include <string>

#define KEY_ALL			KEY_QUERY_VALUE | KEY_SET_VALUE | KEY_CREATE_SUB_KEY | KEY_ENUMERATE_SUB_KEYS | KEY_NOTIFY | KEY_CREATE_LINK | READ_CONTROL 

namespace DWL {
	DRegistro::DRegistro(void) {
	}


	DRegistro::~DRegistro(void) {
	}


	//
	// Función que agrega / modifica un valor tipo string del registro 
	// static LONG AsignarValor_String(HKEY Root, const TCHAR *Key, const TCHAR *Nombre, TCHAR *TextoAGuardar);
	LONG DRegistro::AsignarValor_String(HKEY Root, const TCHAR *Key, const TCHAR *Nombre, const TCHAR *TextoAGuardar) { //
		HKEY                hParentKey;																					// Clave a guardar
		SECURITY_ATTRIBUTES SA;																							// Atributos de Seguridad
		SA.nLength = sizeof(SA);																						// Longitud de la estructura
		SA.lpSecurityDescriptor = NULL;																					// Seguridad Nula
		SA.bInheritHandle = true;																						//

		DWORD Len = static_cast<DWORD>(sizeof(TCHAR) * wcslen(TextoAGuardar));											// Obtengo el tamaño del texto en caracteres
		LONG L = RegCreateKeyEx(Root, Key, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL, &SA, &hParentKey, NULL);			// Creo / Abro la clave
		LONG Ret = RegSetValueEx(hParentKey, Nombre, 0, REG_SZ, reinterpret_cast<const BYTE*>(TextoAGuardar), Len);	// Guardo los datos
#if DREGISTRO_MOSTRARDEBUG == TRUE
		if (Ret != ERROR_SUCCESS) Debug_Escribir_Varg(L"DRegistro::AsignarValor_String %d", Ret);
#endif
		RegCloseKey(hParentKey);																						// Cierro la clave
		return Ret;
	};


	//
	// Función que agrega / modifica un valor tipo DWORD del registro 
	// static LONG DRegistro::AsignarValor_DWORD(HKEY Root, const TCHAR *Key, const TCHAR *Nombre, DWORD DWORDAGuardar);
	LONG DRegistro::AsignarValor_DWORD(HKEY Root, const TCHAR *Key, const TCHAR *Nombre, DWORD DWORDAGuardar) { // 
		HKEY                hParentKey;                                                                         // Clave a guardar
		SECURITY_ATTRIBUTES SA;                                                                                 // Atributos de Seguridad
		SA.nLength = sizeof(SA);																		        // Longitud de la estructura
		SA.lpSecurityDescriptor = NULL;																	        // Seguridad Nula
		SA.bInheritHandle = true;																				//

		LONG L = RegCreateKeyEx(Root, Key, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL, &SA, &hParentKey, NULL);	// Creo / Abro la clave
		LONG Ret = RegSetValueEx(hParentKey, Nombre, 0, REG_DWORD, (BYTE*)& DWORDAGuardar, sizeof(DWORD));		// Guardo los datos
#if DREGISTRO_MOSTRARDEBUG == TRUE
		if (Ret != ERROR_SUCCESS) Debug_Escribir_Varg(L"DRegistro::AsignarValor_DWORD %d", Ret);
#endif
		RegCloseKey(hParentKey);																				// Cierro la clave
		return Ret;
	};


	//
	// Función para obtener el valor de un string del registro
	// static LONG ObtenerValor_String(HKEY Root, const TCHAR *Key, const TCHAR *Nombre, TCHAR *Resultado, DWORD ResTam);
	LONG DRegistro::ObtenerValor_String(HKEY Root, const TCHAR* Key, const TCHAR* Nombre, TCHAR* Resultado, DWORD ResTam) {
		HKEY hParentKey;																		// Clave a abrir
		RegOpenKeyEx(Root, Key, 0, KEY_READ, &hParentKey);										// Abro la clave
		LONG Ret = RegQueryValueEx(hParentKey, Nombre, 0, NULL, (BYTE*)Resultado, &ResTam);		// Obtengo los datos
		RegCloseKey(hParentKey);																// Cierro la Clave
		return Ret;
	};


	//
	// Función para obtener el valor de un string del registro
	// static LONG ObtenerValor_String(HKEY Root, const TCHAR *Key, const TCHAR *Nombre, std::wstring &Resultado);
	LONG DRegistro::ObtenerValor_String(HKEY Root, const TCHAR* Key, const TCHAR* Nombre, std::wstring& Resultado) {
		HKEY			hParentKey;													// Clave a abrir
		static TCHAR	Tmp[2048] = L"";										    // String temporal
		DWORD			Tam = 2048;
		RegOpenKeyEx(Root, Key, 0, KEY_READ, &hParentKey);							// Abro la clave
		LONG Ret = RegQueryValueEx(hParentKey, Nombre, 0, NULL, (BYTE*)Tmp, &Tam);	// Obtengo los datos
		RegCloseKey(hParentKey);													// Cierro la Clave
		Resultado = Tmp;															// Copio el string al resultado
		return Ret;
	};


	//
	// Función para eliminar un valor del registro de cualquier tipo
	//  static LONG EliminarValor(HKEY Root, const TCHAR *Key, const TCHAR *Nombre);
	LONG DRegistro::EliminarValor(HKEY Root, const TCHAR* Key, const TCHAR* Nombre) {
		HKEY hParentKey;										// Clave a borrar
		RegOpenKeyEx(Root, Key, 0, KEY_ALL, &hParentKey);		// Abro la clave
		LONG Ret = RegDeleteValue(hParentKey, Nombre);			// Borro los datos
#if DREGISTRO_MOSTRARDEBUG == TRUE
		if (Ret != ERROR_SUCCESS) Debug_Escribir_Varg(L"DRegistro::EliminarValor %d", Ret);
#endif
		RegCloseKey(hParentKey);								// Cierro la clave
		return Ret;
	};


	//
	// Función para eliminar una clave del registro de cualquier tipo
	//  static LONG EliminarClave(HKEY Root, const TCHAR *Key, const TCHAR *Nombre);
	LONG DRegistro::EliminarClave(HKEY Root, const TCHAR* Key, const TCHAR* Nombre) {
		HKEY hParentKey = NULL;										// Clave a borrar
		RegOpenKeyEx(Root, Key, 0, KEY_ALL, &hParentKey);		// Abro la clave padre
		LONG Ret = RegDeleteKey(hParentKey, Nombre);			// Borro la clave
#if DREGISTRO_MOSTRARDEBUG == TRUE
		if (Ret != ERROR_SUCCESS) Debug_Escribir_Varg(L"DRegistro::EliminarClave %d", Ret);
#endif
		RegCloseKey(hParentKey);								// Cierro la clave
		return Ret;
	};

}