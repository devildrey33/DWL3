// EdicionTextoEx.cpp : Define el punto de entrada de la aplicación.
//

#include "framework.h"
#include "main.h"



int APIENTRY _tWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPTSTR lpCmdLine, _In_ int nCmdShow) {
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

	// Inicio la DWL con la clase aplicación de este proyecto
	INICIAR_DWL3(EjemploEdicionTextoExApp);

	App.ConsolaDebug.Crear(L"DEBUG");

	App.Ventana.Crear();

    HACCEL	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_EDICIONTEXTOEX));
    MSG		msg;

    // Bucle principal de mensajes:
    while (GetMessage(&msg, nullptr, 0, 0)) {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}


