#include "VentanaPrincipal.h"
#include "resource.h"

#define ID_ARBOL	1000


void VentanaPrincipal::Crear(void) {
	CrearVentana(NULL, L"Ejemplo_ListalEx", L"Ejemplo ListaEx", 100, 100, 400, 400, WS_OVERLAPPEDWINDOW | WS_VISIBLE);
	
	Lista.CrearListaEx(this, 10, 10, 360, 200, ID_ARBOL, WS_CHILD | WS_VISIBLE);

	Lista.AgregarColumna(120, DWL::DListaEx_Columna_Alineacion_Izquierda);
	Lista.AgregarColumna(120, DWL::DListaEx_Columna_Alineacion_Centrado);
	Lista.AgregarColumna(-1, DWL::DListaEx_Columna_Alineacion_Derecha);

	Lista.AgregarItem(IDI_ICON1, -1, { L"Item1", L"SubItem1.1", L"SubItem1.2" });
	Lista.AgregarItem(IDI_ICON2, -1, { L"Item2", L"SubItem2.1", L"SubItem1.2" });
	Lista.AgregarItem(IDI_ICON3, -1, { L"Item3", L"SubItem3.1", L"SubItem3.2" });
	Lista.AgregarItem(IDI_ICON4, -1, { L"Item4", L"SubItem4.1", L"SubItem4.2" });
	Lista.AgregarItem(IDI_ICON5, -1, { L"Item5", L"SubItem5.1", L"SubItem5.2" });
	Lista.AgregarItem(IDI_ICON6, -1, { L"Item6", L"SubItem6.1", L"SubItem6.2" });
	Lista.AgregarItem(IDI_ICON1, -1, { L"Item7", L"SubItem7.1", L"SubItem7.2" });
	Lista.AgregarItem(IDI_ICON2, -1, { L"Item8", L"SubItem8.1", L"SubItem8.2" });
}


LRESULT CALLBACK VentanaPrincipal::GestorMensajes(UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
		case WM_CLOSE :
			PostQuitMessage(0);
			return 0;
	}
	return DVentana::GestorMensajes(uMsg, wParam, lParam);
}