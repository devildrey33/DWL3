#include "VentanaPrincipal.h"

#define ID_EDIT1	1000
#define ID_EDIT2	1001
#define ID_EDIT3	1002


void VentanaPrincipal::Crear(void) {
	CrearVentana(NULL, L"Ejemplo_EdicionTextoEx", L"Ejemplo EdicionTextoEx", 100, 100, 400, 400, WS_OVERLAPPEDWINDOW | WS_VISIBLE);

	Edit1.CrearEdicionTextoEx(this, L"12345", 10, 10, 300, 20, ID_EDIT1);
	Edit2.Alineacion = DWL::DEdicionTextoEx_Alineacion_Centrado;
	Edit2.CrearEdicionTextoEx(this, L"Test2", 10, 40, 300, 20, ID_EDIT2);
	Edit3.Alineacion = DWL::DEdicionTextoEx_Alineacion_Derecha;
	Edit3.CrearEdicionTextoEx(this, L"Test3", 10, 70, 300, 20, ID_EDIT3);
}


LRESULT CALLBACK VentanaPrincipal::GestorMensajes(UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
		case WM_CLOSE :
			PostQuitMessage(0);
			return 0;
	}
	return DVentana::GestorMensajes(uMsg, wParam, lParam);
}