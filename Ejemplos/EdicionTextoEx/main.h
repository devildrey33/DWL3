#pragma once

#include "resource.h"
#include <DApp.h>
#include "VentanaPrincipal.h"

// Archivo lib de la DWL según la configuración
#ifdef _DEBUG
	#if _WIN64
		#pragma comment(lib, "..\\..\\Build\\DWL\\x64\\Debug\\DWL3.lib")
	#else
		#pragma comment(lib, "..\\..\\Build\\DWL\\x86\\Debug\\DWL3.lib")
	#endif
#else
	#if _WIN64
		#pragma comment(lib, "..\\..\\Build\\DWL\\x64\\Release\\DWL3.lib")
	#else
		#pragma comment(lib, "..\\..\\Build\\DWL\\x86\\Release\\DWL3.lib")
	#endif
#endif



// Paso 1 : crear un objeto que herede de DApp para que sea nuestra clase aplicación
class EjemploEdicionTextoExApp : public DWL::DApp {
  public :
						EjemploEdicionTextoExApp(void) { };
	
	VentanaPrincipal	Ventana;
};


/*
// Paso 2 : crear un singleton de nuestra clase aplicación
class MiApp : public DWL::DSingleton<EjemploEdicionTextoExApp> { };



// Paso 3 : Definir la macro App para que apunte a la instancia de nuestra aplicación
#define INICIAR_DWL3(DAPP) class _DWLApp : public DWL::DSingleton<DAPP> { };
#define App _DWLApp::Instancia()
*/