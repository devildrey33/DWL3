#include "pch.h"
#include "DIcono.h"
#include <Shellapi.h>		// SHGetFileInfo
//#include <Shtypes.h>		
#include <Shlobj.h>			// SHGetSpecialFolderLocation 
#include <algorithm>

namespace DWL {

	std::vector<DIcono::DIcono_Datos *>  DIcono::_Iconos;
	INT_PTR                              DIcono::_IDNegativa = 0;


	DIcono::DIcono(void) : _Icono(NULL) {
		// Agrego un primer icono transparente
		if (_Iconos.size() == 0) {
			_Iconos.push_back(new DIcono_Datos(0, 0, 128, 128));
		}
	}

	DIcono::DIcono(const INT_PTR nIDRecursos, const int nAncho, const int nAlto) : _Icono(NULL) {
		CrearIconoRecursos(nIDRecursos, nAncho, nAlto);
	}

	DIcono::DIcono(const GUID &rfid, const int nAncho, const int nAlto) : _Icono(NULL) {
		CrearIconoKnownFolder(rfid, nAncho, nAlto);
	}

	DIcono::DIcono(const wchar_t *Path, const int nAncho, const int nAlto, const int nPosicionIco) : _Icono(NULL) {
		CrearIconoExterno(Path, nAncho, nAlto);
	}


	DIcono::~DIcono(void) {
	}


	void DIcono::CrearIconoRecursos(const INT_PTR IDRecursos, const int nAncho, const int nAlto) {
		// Sin icono
		if (IDRecursos == 0) {
			_Icono = NULL;
			return;
		}
		// Busco el icono de los recursos
		for (size_t i = 0; i < _Iconos.size(); i++) {
			if (_Iconos[i]->ID == IDRecursos && _Iconos[i]->Alto == nAlto && _Iconos[i]->Ancho == nAncho) {
				_Icono = _Iconos[i];
				return;
			}
		}
		// No se ha encontrado el icono, lo creo
		HICON hIcono = static_cast<HICON>(LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDRecursos), IMAGE_ICON, nAncho, nAlto, LR_DEFAULTCOLOR));

		_Icono = new DIcono_Datos(hIcono, IDRecursos, nAncho, nAlto);
		_Iconos.push_back(_Icono);
	}

	void DIcono::CrearIconoKnownFolder(const GUID &rfid, const int nAncho, const int nAlto) {
		PIDLIST_ABSOLUTE	npi;
		DWORD_PTR			Ret = SHGetKnownFolderIDList(rfid, NULL, NULL, &npi);
		SHFILEINFO			sfi = { 0 };
		Ret = SHGetFileInfo((LPCTSTR)npi, -1, &sfi, sizeof(sfi), SHGFI_PIDL | SHGFI_ICONLOCATION);
		std::wstring		Nombre = sfi.szDisplayName;
		int					PosIco = sfi.iIcon;

		_Icono = _BuscarIDStr(Nombre.c_str(), PosIco);
		// No existe, lo creamos
		if (_Icono == NULL) {
			Ret = SHGetFileInfo((LPCTSTR)npi, -1, &sfi, sizeof(sfi), SHGFI_PIDL | SHGFI_ICON);
			if (SUCCEEDED(Ret)) {
				_Icono = new DIcono_Datos(sfi.hIcon, --_IDNegativa, nAncho, nAlto, Nombre, PosIco);
				_Iconos.push_back(_Icono);
			}
		}
		// Borro el PIDLIST_ABSOLUTE
		ILFree(npi);
	}

	void DIcono::CrearIconoExterno(const wchar_t *Path, const int nAncho, const int nAlto, const int nPosicionIco) {
		DWORD_PTR	 hr;
		LPITEMIDLIST pidl = 0;
		SHFILEINFO   sfi = { 0 };
		hr = SHGetFileInfo(Path, 0, &sfi, sizeof(sfi), SHGFI_ICONLOCATION);
		std::wstring  Nombre = sfi.szDisplayName;
		std::transform(Nombre.begin(), Nombre.end(), Nombre.begin(), toupper); // transformo el path a mayusculas
		int			  PosIco = sfi.iIcon;
		// Busco el icono
		_Icono = _BuscarIDStr(Nombre.c_str(), sfi.iIcon);
		// No existe, lo creamos
		if (_Icono == NULL) {
			DWORD Attr = GetFileAttributes(Path);
			// El path especificado no existe asigno el icono icono transparente 
			if (Attr == INVALID_FILE_ATTRIBUTES) {
				_Icono = _Iconos[0];
			}
			// El path exite, extraigo el icono
			else {
				hr = SHGetFileInfo(Path, nPosicionIco, &sfi, sizeof(sfi), SHGFI_ICON);
				if (SUCCEEDED(hr)) {
					_Icono = new DIcono_Datos(sfi.hIcon, --_IDNegativa, nAncho, nAlto, Nombre, PosIco);
					_Iconos.push_back(_Icono);
				}
			}
		}
	}


	void DIcono::EliminarIconos(void) {
		for (size_t i = 0; i < _Iconos.size(); i++) {
			if (_Iconos[i]->Icono != NULL) DeleteObject(_Iconos[i]->Icono);
			delete _Iconos[i];
		}
	}

	DIcono::DIcono_Datos *DIcono::_BuscarIDStr(const wchar_t *nIDStr, const int nPosicionStr) {		
		for (size_t i = 0; i < _Iconos.size(); i++) {								//  Asegura que el icono consultado no sea un icono de recursos o knownfolder
			if (_Iconos[i]->IDStr == nIDStr && _Iconos[i]->IDStrPos == nPosicionStr && _Iconos[i]->ID == 0) {
				return _Iconos[i];
			}
		}
		return NULL;
	}
};