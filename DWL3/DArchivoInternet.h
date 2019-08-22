#ifndef DARCHIVOINTERNET_H
#define DARCHIVOINTERNET_H

#include "DPeticion.h"

#pragma comment(lib, "wininet.lib")	// Agrego la libreria wininet.lib al proyecto

namespace DWL {

	class DApp;

	// Clase est�tica para descargar archivos de internet multithread
	// Dise�ada para archivos de texto ligeros
	class DArchivoInternet	{
	  public: //////////////////////////////// Constructor vacio
											DArchivoInternet(void) { };
											// Funci�n que inicia una petici�n, y devuelve su ID
		static const UINT					Obtener(const wchar_t	 		           *nUrl,												// Url a descargar
													std::function<void(DPeticion &)>	LambdaTerminado,   /* [](DWL::DPeticion &) { } */	// Funci�n lambda para cuando se ha terminado de descargar
													std::function<void(const UINT)>		LambdaError			= [](const UINT)       { },		// Funci�n lambda por si sucede un error
													std::function<void(const float)>	LambdaPorcentaje	= [](const float)      { }	);	// Funci�n lambda que nos dice el porcentaje de descarga
											// Funci�n que inicia una petici�n, y devuelve su ID
		static const UINT					Obtener(std::wstring	 			       &nUrl,												// Url a descargar
													std::function<void(DPeticion &)>	LambdaTerminado,   /* [](DWL::DPeticion &) { } */	// Funci�n lambda para cuando se ha terminado de descargar
													std::function<void(const UINT)>		LambdaError			= [](const UINT)       { },		// Funci�n lambda por si sucede un error
													std::function<void(const float)>	LambdaPorcentaje	= [](const float)      { }	);	// Funci�n lambda que nos dice el porcentaje de descarga
											// Funci�n para cancelar una petici�n
		static void							Cancelar(const UINT cID);
											// Funci�n para buscar una petici�n por su ID
		static DPeticion		           *Buscar(const UINT bID);

	  private:
											// Vector de peticiones
		static std::vector<DPeticion *>    _Peticiones;
											// Contador para ir repartiendo ID's unicas a cada petici�n
		static UINT					       _ID;
											// Funci�n para eliminar una peticion
		static void						   _EliminarPeticion(const UINT nID);

		friend class DApp;
	};

};



#endif
