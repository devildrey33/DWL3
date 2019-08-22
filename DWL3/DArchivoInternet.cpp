#include "pch.h"
#include "DArchivoInternet.h"
#include "DApp.h"
#include "DMensajesWnd.h"
#include <wininet.h>

namespace DWL {

	// Vector de peticiones pendiente estatico
	std::vector<DPeticion *>		DArchivoInternet::_Peticiones;
	// ID unica para cada petici�n que se va incrementando
	UINT						DArchivoInternet::_ID			= 0;

	const UINT DArchivoInternet::Obtener(	const wchar_t	 		           *nUrl,												// Url a descargar
											std::function<void(DPeticion &)>	LambdaTerminado, /*   [](DWL::DPeticion &) { } */	// Funci�n lambda para cuando se ha terminado de descargar
											std::function<void(const UINT)>		LambdaError		 /* = [](const UINT)	   { } */,		// Funci�n lambda por si sucede un error
											std::function<void(const float)>	LambdaPorcentaje /* = [](const float)      { } */) {	// Funci�n lambda que nos dice el porcentaje de descarga

		// Creo la petici�n
		DPeticion* nPeticion = new DPeticion(nUrl, _Aplicacion->hWndMensajes(), _ID++);

		// Asigno las funciones lambda (no se utilizan en ning�n momento dentro del thread)
		nPeticion->_LambdaTerminado		= LambdaTerminado;
		nPeticion->_LambdaError			= LambdaError;
		nPeticion->_LambdaPorcentaje	= LambdaPorcentaje;

		// Guardo la petici�n en el vector de peticiones pendientes
		_Peticiones.push_back(nPeticion);

		// Devuelvo la ID
		return _ID -1;
	}


	const UINT DArchivoInternet::Obtener(	std::wstring 	 		           &nUrl,													// Url a descargar
											std::function<void(DPeticion &)>	LambdaTerminado, /*   [](DWL::DPeticion &) { } */		// Funci�n lambda para cuando se ha terminado de descargar
											std::function<void(const UINT)>		LambdaError		 /* = [](const UINT)       { } */,		// Funci�n lambda por si sucede un error
											std::function<void(const float)>	LambdaPorcentaje /* = [](const float)      { } */) {	// Funci�n lambda que nos dice el porcentaje de descarga

		// Creo la petici�n
		DPeticion* nPeticion = new DPeticion(nUrl, _Aplicacion->hWndMensajes(), _ID++);

		// Asigno las funciones lambda (no se utilizan en ning�n momento dentro del thread)
		nPeticion->_LambdaTerminado		= LambdaTerminado;
		nPeticion->_LambdaError			= LambdaError;
		nPeticion->_LambdaPorcentaje	= LambdaPorcentaje;

		// Guardo la petici�n en el vector de peticiones pendientes
		_Peticiones.push_back(nPeticion);

		// Devuelvo la ID
		return _ID -1;
	}


	void DArchivoInternet::Cancelar(const UINT cID) {
		DPeticion *P = Buscar(cID);
		if (P != NULL) P->Cancelar(TRUE);
	}


	DPeticion *DArchivoInternet::Buscar(const UINT bID) {
		for (size_t i = 0; i < _Peticiones.size(); i++) {
			if (_Peticiones[i]->ID() == bID) {
				return _Peticiones[i];
			}
		}
		return nullptr;
	}

	void DArchivoInternet::_EliminarPeticion(const UINT nID) {
		for (size_t i = 0; i < _Peticiones.size(); i++) {
			if (_Peticiones[i]->ID() == nID) {
				// Elimino la petici�n de la mem�ria
				delete _Peticiones[i];
				// Elimino la petici�n del vector de peticiones pendientes
				_Peticiones.erase(_Peticiones.begin() + i);
				return;
			}
		}
	}

}