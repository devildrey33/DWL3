#include "VentanaPrincipal.h"
#include "resource.h"

#define ID_ARBOL	1000


void VentanaPrincipal::Crear(void) {
	CrearVentana(NULL, L"Ejemplo_ArbolEx", L"Ejemplo ArbolEx", 100, 100, 400, 400, WS_OVERLAPPEDWINDOW | WS_VISIBLE);
	
	Arbol.CrearArbolEx(this, 10, 10, 200, 300, ID_ARBOL, WS_CHILD | WS_VISIBLE);

	// Creo unos cuantos nodos
	DWL::DArbolEx_Nodo* Nodo = Arbol.AgregarNodo(L"Nodo1", NULL, IDI_ICON6);
	Arbol.AgregarNodo(L"SubNodo1", Nodo, IDI_ICON1);
	Arbol.AgregarNodo(L"SubNodo2", Nodo, IDI_ICON2);
	Arbol.AgregarNodo(L"SubNodo3", Nodo, IDI_ICON3);
	Arbol.AgregarNodo(L"SubNodo4", Nodo, IDI_ICON4);
	Arbol.AgregarNodo(L"SubNodo5", Nodo, IDI_ICON5);
	Arbol.AgregarNodo(L"SubNodo6", Nodo, IDI_ICON6);
	DWL::DArbolEx_Nodo* Nodo2 = Arbol.AgregarNodo(L"Nodo2", NULL, IDI_ICON1);
	Arbol.AgregarNodo(L"SubNodo1", Nodo2, IDI_ICON2);
	Arbol.AgregarNodo(L"SubNodo2", Nodo2, IDI_ICON3);
	Arbol.AgregarNodo(L"SubNodo3", Nodo2, IDI_ICON4);
	DWL::DArbolEx_Nodo* SubNodo4 = Arbol.AgregarNodo(L"SubNodo4", Nodo, IDI_ICON5);
	Arbol.AgregarNodo(L"SubSubNodo1", SubNodo4, IDI_ICON6);
	Arbol.AgregarNodo(L"SubSubNodo2", SubNodo4, IDI_ICON1);
	Arbol.AgregarNodo(L"SubSubNodo3", SubNodo4, IDI_ICON2);
}


LRESULT CALLBACK VentanaPrincipal::GestorMensajes(UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
		case WM_CLOSE :
			PostQuitMessage(0);
			return 0;
	}
	return DVentana::GestorMensajes(uMsg, wParam, lParam);
}