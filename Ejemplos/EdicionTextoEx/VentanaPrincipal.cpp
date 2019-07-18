#include "VentanaPrincipal.h"

#define ID_EDIT1	1000
#define ID_EDIT2	1001
#define ID_EDIT3	1002
#define ID_EDIT4	1003
#define ID_EDIT5	1004
#define ID_EDIT6	1005


void VentanaPrincipal::Crear(void) {
	CrearVentana(NULL, L"Ejemplo_EdicionTextoEx", L"Ejemplo EdicionTextoEx", 100, 100, 400, 400, WS_OVERLAPPEDWINDOW | WS_VISIBLE);

	RECT RC;
	GetClientRect(_hWnd, &RC);

	EditIzquierda.CrearEdicionTextoEx(this, L"Texto alineado a la izquierda", 10, 10, RC.right - 20, 20, ID_EDIT1);

	EditCentrado.Alineacion = DWL::DEdicionTextoEx_Alineacion_Centrado;
	EditCentrado.CrearEdicionTextoEx(this, L"Texto centrado", 10, 40, RC.right - 20, 20, ID_EDIT2);

	EditDerecha.Alineacion = DWL::DEdicionTextoEx_Alineacion_Derecha;
	EditDerecha.CrearEdicionTextoEx(this, L"Texto alineado a la derecha", 10, 70, RC.right - 20, 20, ID_EDIT3);

	EditEnteros.Entrada = DWL::DEdicionTextoEx_Entrada_ValoresEnteros;
	EditEnteros.Placeholder = L"Solo acepta valores enteros. (1234567890)";
	EditEnteros.CrearEdicionTextoEx(this, L"", 10, 100, RC.right - 20, 20, ID_EDIT4);

	EditDecimales.Entrada = DWL::DEdicionTextoEx_Entrada_ValoresDecimales;
	EditDecimales.Placeholder = L"Solo acepta valores decimales (0.123456789)";
	EditDecimales.CrearEdicionTextoEx(this, L"", 10, 130, RC.right - 20, 20, ID_EDIT5);

	EditTiempo.Entrada = DWL::DEdicionTextoEx_Entrada_ValoresTiempo;
	EditTiempo.Placeholder = L"Solo acepta valores de tiempo (12:34:56).";
	EditTiempo.CrearEdicionTextoEx(this, L"", 10, 160, RC.right - 20, 20, ID_EDIT6);

}


LRESULT CALLBACK VentanaPrincipal::GestorMensajes(UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
		case WM_CLOSE :
			PostQuitMessage(0);
			return 0;
	}
	return DVentana::GestorMensajes(uMsg, wParam, lParam);
}