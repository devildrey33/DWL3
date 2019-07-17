#include "pch.h"
#include "DUnidadesDisco.h"

namespace DWL {

	/****************/
	/* DUnidadDisco */
	/****************/
	DUnidadDisco::DUnidadDisco(void) : _Numero_Serie(0), _Longitud_Componente(0), _Flags(0), _Letra(0), _Tipo_Disco(DEnum_UnidadDisco_Tipo_Desconocido) {
	};

	DUnidadDisco::DUnidadDisco(TCHAR *nPath) {
		Obtener(nPath);
	};

	DUnidadDisco::~DUnidadDisco(void) {
	};

	BOOL DUnidadDisco::Obtener(const TCHAR *nPath) {
		_Letra = nPath[0];
		TCHAR Nombre[MAX_PATH + 1];
		TCHAR Tipo[512];
		BOOL R = GetVolumeInformation(nPath, Nombre, MAX_PATH + 1, &_Numero_Serie, &_Longitud_Componente, &_Flags, Tipo, 512);
		_Nombre_Volumen = Nombre;
		_Tipo_Particion = Tipo;
		_Tipo_Disco = static_cast<DEnum_UnidadDisco_Tipo>(GetDriveType(nPath));
		if (R != FALSE) R = TRUE;
		return R;
	};

	const BOOL DUnidadDisco::Solo_Lectura(void) {
		if (_Flags & FILE_READ_ONLY_VOLUME) return TRUE;
		return FALSE;
	};

	const DWORD DUnidadDisco::Numero_Serie(void) {
		return _Numero_Serie;
	};

	const DWORD DUnidadDisco::Longitud_Componente(void) {
		return _Longitud_Componente;
	};

	const TCHAR *DUnidadDisco::Nombre_Volumen(void) {
		return _Nombre_Volumen.c_str();
	}

	const TCHAR *DUnidadDisco::Tipo_Particion(void) {
		return _Tipo_Particion.c_str();
	}
	// Retorna la letra de la unidad en mayuscula
	const TCHAR DUnidadDisco::Letra(void) {
		return _Letra;
	};
	const DEnum_UnidadDisco_Tipo DUnidadDisco::Tipo_Disco(void) {
		return _Tipo_Disco;
	};





	/******************/
	/* DUnidadesDisco */
	/******************/
	DUnidadesDisco::DUnidadesDisco(void) {
		Escanear_Unidades_Locales();
	};

	DUnidadesDisco::~DUnidadesDisco(void) {
		BorrarMemoria();
	};
	// Mira las unidades locales de la 'C' a la 'Z' evitando las disqueteras.
	const unsigned int DUnidadesDisco::Escanear_Unidades_Locales(void) {
		BorrarMemoria();
		TCHAR Path[4];
		BOOL  Exito = FALSE;
		wcscpy_s(Path, 4, TEXT("?:\\"));
		DUnidadDisco *Unidad = new DUnidadDisco;
		for (TCHAR n = TEXT('C'); n < TEXT('Z'); n++) {
			Path[0] = n;
			Exito = Unidad->Obtener(Path);
			if (Exito == TRUE) {
				_Unidades.push_back(Unidad);
				Unidad = new DUnidadDisco;
			}
		}
		delete Unidad;
		return static_cast<unsigned int>(_Unidades.size());
	};

	DUnidadDisco *DUnidadesDisco::Unidad(const size_t Pos) {
		return _Unidades[Pos];
	};

	DUnidadDisco *DUnidadesDisco::Buscar_Numero_Serie(const DWORD Numero) {
		for (size_t n = 0; n < _Unidades.size(); n++) {
			if (_Unidades[n]->Numero_Serie() == Numero) return _Unidades[n];
		}
		return NULL;
	};

	DUnidadDisco *DUnidadesDisco::Buscar_Letra(const wchar_t Letra) {
		TCHAR L2 = toupper(Letra);
		for (size_t n = 0; n < _Unidades.size(); n++) {
			if (_Unidades[n]->Letra() == L2) return _Unidades[n];
		}
		return NULL;
	};

	const size_t DUnidadesDisco::TotalUnidades(void) {
		return _Unidades.size();
	};

	void DUnidadesDisco::BorrarMemoria(void) {
		for (size_t n = 0; n < _Unidades.size(); n++) delete _Unidades[n];
		_Unidades.resize(0);
	};
};