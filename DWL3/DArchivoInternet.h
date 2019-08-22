#ifndef DARCHIVOINTERNET_H
#define DARCHIVOINTERNET_H

#include "DPeticion.h"

#pragma comment(lib, "wininet.lib")	// Agrego la libreria wininet.lib al proyecto

namespace DWL {

	class DApp;

	// Clase estática para descargar archivos de internet multithread
	// Diseñada para archivos de texto ligeros
	class DArchivoInternet	{
	  public: //////////////////////////////// Constructor vacio
											DArchivoInternet(void) { };
											// Función que inicia una petición, y devuelve su ID
		static const UINT					Obtener(const wchar_t	 		           *nUrl,												// Url a descargar
													std::function<void(DPeticion &)>	LambdaTerminado,   /* [](DWL::DPeticion &) { } */	// Función lambda para cuando se ha terminado de descargar
													std::function<void(const UINT)>		LambdaError			= [](const UINT)       { },		// Función lambda por si sucede un error
													std::function<void(const float)>	LambdaPorcentaje	= [](const float)      { }	);	// Función lambda que nos dice el porcentaje de descarga
											// Función que inicia una petición, y devuelve su ID
		static const UINT					Obtener(std::wstring	 			       &nUrl,												// Url a descargar
													std::function<void(DPeticion &)>	LambdaTerminado,   /* [](DWL::DPeticion &) { } */	// Función lambda para cuando se ha terminado de descargar
													std::function<void(const UINT)>		LambdaError			= [](const UINT)       { },		// Función lambda por si sucede un error
													std::function<void(const float)>	LambdaPorcentaje	= [](const float)      { }	);	// Función lambda que nos dice el porcentaje de descarga
											// Función para cancelar una petición
		static void							Cancelar(const UINT cID);
											// Función para buscar una petición por su ID
		static DPeticion		           *Buscar(const UINT bID);

	  private:
											// Vector de peticiones
		static std::vector<DPeticion *>    _Peticiones;
											// Contador para ir repartiendo ID's unicas a cada petición
		static UINT					       _ID;
											// Función para eliminar una peticion
		static void						   _EliminarPeticion(const UINT nID);

		friend class DApp;
	};

};



#endif
