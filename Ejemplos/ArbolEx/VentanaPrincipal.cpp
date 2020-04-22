#include "VentanaPrincipal.h"
#include "resource.h"

#define ID_ARBOL	1000


void VentanaPrincipal::Crear(void) {
	CrearVentana(NULL, L"Ejemplo_ArbolEx", L"Ejemplo ArbolEx", 100, 100, 400, 400, WS_OVERLAPPEDWINDOW | WS_VISIBLE);
	
	Arbol.CrearArbolEx(this, 10, 10, 380, 370, ID_ARBOL, WS_CHILD | WS_VISIBLE);

	// Creo unos cuantos nodos
	DWL::DArbolEx_Nodo *Nodo = Arbol.AgregarNodo(L"Nodo1", NULL, IDI_ICON6);
	Nodo->Expandido(TRUE);
	Arbol.AgregarNodo(L"SubNodo1.1", Nodo, IDI_ICON1);
	Arbol.AgregarNodo(L"SubNodo1.2", Nodo, IDI_ICON2);
	Arbol.AgregarNodo(L"SubNodo1.3", Nodo, IDI_ICON3);
	Arbol.AgregarNodo(L"SubNodo1.4", Nodo, IDI_ICON4);
	Arbol.AgregarNodo(L"SubNodo1.5", Nodo, IDI_ICON5);
	Arbol.AgregarNodo(L"SubNodo1.6", Nodo, IDI_ICON6);
	DWL::DArbolEx_Nodo *Nodo2 = Arbol.AgregarNodo(L"Nodo2", NULL, IDI_ICON1);
	Arbol.AgregarNodo(L"SubNodo2.1", Nodo2, IDI_ICON2);
	Arbol.AgregarNodo(L"SubNodo2.2", Nodo2, IDI_ICON3);
	Arbol.AgregarNodo(L"SubNodo2.3", Nodo2, IDI_ICON4);
	// TEST AgregarNodo en posiciones especificas que no sean el final ni ordenado alfabericamente
	Arbol.AgregarNodo(L"SubNodo2.0", Nodo2, IDI_ICON2, NULL, 0);
	Arbol.AgregarNodo(L"SubNodo2.5", Nodo2, IDI_ICON2, NULL, 4); // Lo añado a la posición 4 (hay 4 creados se quedará al final)
	Arbol.AgregarNodo(L"SubNodo2.4", Nodo2, IDI_ICON2, NULL, 4); // Lo añado a la posición 4 (ahora no está al final y el SubNodo2.5 pasa al final)
	DWL::DArbolEx_Nodo* SubNodo7 = Arbol.AgregarNodo(L"SubNodo1.7", Nodo, IDI_ICON5);
	Arbol.AgregarNodo(L"SubNodo1.7.1", SubNodo7, IDI_ICON6);
	Arbol.AgregarNodo(L"SubNodo1.7.2", SubNodo7, IDI_ICON1);
	Arbol.AgregarNodo(L"SubNodo1.7.3", SubNodo7, IDI_ICON2);
}


LRESULT CALLBACK VentanaPrincipal::GestorMensajes(UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
		case WM_CLOSE :
			PostQuitMessage(0);
			return 0;
	}
	return DVentana::GestorMensajes(uMsg, wParam, lParam);
}