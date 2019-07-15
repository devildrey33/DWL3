// EdicionTextoEx.cpp : Define el punto de entrada de la aplicación.
//

#include "framework.h"
#include "main.h"



int APIENTRY _tWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPTSTR lpCmdLine, _In_ int nCmdShow) {
	// Inicio la DWL con la clase aplicación de este proyecto
	INICIAR_DWL3(EjemploListaExApp);

	#ifdef _DEBUG
		App.ConsolaDebug.Crear(L"DEBUG");
	#endif

	App.Ventana.Crear();

	return App.BucleMensajes();
}


