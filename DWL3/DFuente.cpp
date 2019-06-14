#include "pch.h"
#include "DFuente.h"
#include "DStringUtils.h"
#include <algorithm>

namespace DWL {



/*	 _____  ______                _                  _____        _
	|  __ \|  ____|              | |                |  __ \      | |
	| |  | | |__ _   _  ___ _ __ | |_ ___           | |  | | __ _| |_ ___  ___
	| |  | |  __| | | |/ _ \ '_ \| __/ _ \          | |  | |/ _` | __/ _ \/ __|
	| |__| | |  | |_| |  __/ | | | ||  __/          | |__| | (_| | || (_) \__ \
	|_____/|_|   \__,_|\___|_| |_|\__\___|  ______  |_____/ \__,_|\__\___/|___/
										   |______|	                          	 */

	// Obtiene el tamaño del texto especificado, utilizando la fuente de esta clase
	const SIZE DFuente::DFuente_Datos::ObtenerTamTexto(const TCHAR *nTexto) {
		HDC		Buffer = GetDC(NULL);
		SIZE	nTam = { 0, 0 };
		HFONT	VFuente = static_cast<HFONT>(SelectObject(Buffer, Fuente));
		GetTextExtentPoint(Buffer, nTexto, static_cast<int>(wcslen(nTexto)), &nTam);
		SelectObject(Buffer, VFuente);
		ReleaseDC(NULL, Buffer);
		return nTam;
	}

	// Obtiene el tamaño del string especificado, utilizando la fuente de esta clase
	const SIZE DFuente::DFuente_Datos::ObtenerTamTexto(std::wstring &nTexto) {
		HDC		Buffer = GetDC(NULL);
		SIZE	nTam = { 0, 0 };
		HFONT	VFuente = static_cast<HFONT>(SelectObject(Buffer, Fuente));
		GetTextExtentPoint(Buffer, nTexto.c_str(), static_cast<int>(nTexto.size()), &nTam);
		SelectObject(Buffer, VFuente);
		ReleaseDC(NULL, Buffer);
		return nTam;
	}

	// Obtiene el tamaño del texto especificado. El DC especificado debe tener seleccionada esta fuente.
	const SIZE DFuente::DFuente_Datos::ObtenerTamTexto(HDC DC, const TCHAR *nTexto) {
		SIZE	nTam = { 0, 0 };
		GetTextExtentPoint(DC, nTexto, static_cast<int>(wcslen(nTexto)), &nTam);
		return nTam;
	}

	// Obtiene el tamaño del string especificado. El DC especificado debe tener seleccionada esta fuente.
	const SIZE DFuente::DFuente_Datos::ObtenerTamTexto(HDC DC, std::wstring &nTexto) {
		SIZE	nTam = { 0, 0 };
		GetTextExtentPoint(DC, nTexto.c_str(), static_cast<int>(nTexto.size()), &nTam);
		return nTam;
	}


	
/*	  _____  ______                _
	 |  __ \|  ____|              | |
	 | |  | | |__ _   _  ___ _ __ | |_ ___
	 | |  | |  __| | | |/ _ \ '_ \| __/ _ \
	 | |__| | |  | |_| |  __/ | | | ||  __/
	 |_____/|_|   \__,_|\___|_| |_|\__\___|	  */


	// Vector de fuentes estático
	std::vector<DFuente::DFuente_Datos *> DFuente::_Fuentes;


	DFuente::DFuente(void) : _Fuente(NULL) {
	}


	DFuente::DFuente(const int nTam, const wchar_t *nNombre, const BOOL nNegrita, const BOOL nCursiva, const BOOL nSubrayado, const BOOL nTachado) : _Fuente(NULL) {
		CrearFuente(nTam, nNombre, nNegrita, nCursiva, nSubrayado, nTachado);
	}

	DFuente::~DFuente(void) {
	}

	// Función que crea el tipo de fuente especificado, si se da el caso de que la fuente ya fue creada anteriormente, se retornará la anterior.
	void DFuente::CrearFuente(const int nTam, const wchar_t *nNombre, const BOOL nNegrita, const BOOL nCursiva, const BOOL nSubrayado, const BOOL nTachado) {
		std::wstring _Nombre = nNombre;
		std::transform(_Nombre.begin(), _Nombre.end(), _Nombre.begin(), toupper);		
		DFuente_Datos *Tmp = _BuscarFuente(nTam, _Nombre, nNegrita, nCursiva, nSubrayado, nTachado);
		if (Tmp == NULL) {
			HFONT nFuente = CreateFont(nTam, 0, 0, 0, (nNegrita == TRUE) ? FW_BOLD : FW_NORMAL, nCursiva, nSubrayado, nTachado, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, FF_ROMAN, nNombre);
			_Fuentes.push_back(new DFuente_Datos(nTam, _Nombre, nNegrita, nCursiva, nSubrayado, nTachado, nFuente));
			_Fuente = _Fuentes[_Fuentes.size() - 1];
		}
		else {
			_Fuente = Tmp;
		}
	}


	void DFuente::Tam(const int nTam) {
		CrearFuente(nTam, _Fuente->Nombre.c_str(), _Fuente->Negrita, _Fuente->Cursiva, _Fuente->Subrayado, _Fuente->Tachado);
	}

	void DFuente::Nombre(const wchar_t *nNombre) {
		CrearFuente(_Fuente->Tam, nNombre, _Fuente->Negrita, _Fuente->Cursiva, _Fuente->Subrayado, _Fuente->Tachado);
	}

	void DFuente::Negrita(const BOOL nNegrita) {
		CrearFuente(_Fuente->Tam, _Fuente->Nombre.c_str(), nNegrita, _Fuente->Cursiva, _Fuente->Subrayado, _Fuente->Tachado);
	}

	void DFuente::Cursiva(const BOOL nCursiva) {
		CrearFuente(_Fuente->Tam, _Fuente->Nombre.c_str(), _Fuente->Negrita, nCursiva, _Fuente->Subrayado, _Fuente->Tachado);
	}

	void DFuente::Subrayado(const BOOL nSubrayado) {
		CrearFuente(_Fuente->Tam, _Fuente->Nombre.c_str(), _Fuente->Negrita, _Fuente->Cursiva, nSubrayado, _Fuente->Tachado);
	}

	void DFuente::Tachado(const BOOL nTachado) {
		CrearFuente(_Fuente->Tam, _Fuente->Nombre.c_str(), _Fuente->Negrita, _Fuente->Cursiva, _Fuente->Subrayado, nTachado);
	}

	/*
	const SIZE DFuente::ObtenerTamTexto(const TCHAR *nTexto) {
//		return _Fuentes[_FuentePos].ObtenerTamTexto(nTexto);
		return _Fuente->ObtenerTamTexto(nTexto);
	}

	const SIZE DFuente::ObtenerTamTexto(std::wstring &nTexto) {
//		return _Fuentes[_FuentePos].ObtenerTamTexto(nTexto);
		return _Fuente->ObtenerTamTexto(nTexto);
	}*/


	// Función que elimina todas las fuentes (OJO solo utilizar al terminar la aplicación)
	void DFuente::EliminarFuentes(void) {
		for (size_t i = 0; i < _Fuentes.size(); i++) {
			DeleteObject(_Fuentes[i]->Fuente);
			delete _Fuentes[i];
		}
		_Fuentes.resize(0);
	}

	DFuente::DFuente_Datos *DFuente::_BuscarFuente(const int nTam, std::wstring &nNombre, const BOOL nNegrita, const BOOL nCursiva, const BOOL nSubrayado, const BOOL nTachado) {
		for (size_t i = 0; i < _Fuentes.size(); i++) {
			if (nTam == _Fuentes[i]->Tam && nNombre == _Fuentes[i]->Nombre && nNegrita == _Fuentes[i]->Negrita && nCursiva == _Fuentes[i]->Cursiva && nSubrayado == _Fuentes[i]->Subrayado && nTachado == _Fuentes[i]->Tachado) {
				return _Fuentes[i];
			}
		}
		return NULL;
	}


}