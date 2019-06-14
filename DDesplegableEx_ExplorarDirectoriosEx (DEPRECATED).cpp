#include "pch.h"
#include "DDesplegableEx (DEPRECATED).h"
#include "DDesplegableEx_ExplorarDirectoriosEx (DEPRECATED).h"
//#include "RAVE.h"

namespace DWL {
	/*
	DDesplegableEx_ExplorarDirectoriosEx::DDesplegableEx_ExplorarDirectoriosEx(DDesplegableEx *nPadre) : DExplorarDirectoriosEx(), _Mostrando(FALSE) {
		_Padre = nPadre;
		
		RECT RC;
		GetWindowRect(nPadre->hWnd(), &RC);
		Debug_UltimoError(0);		// Reseteo el ultimo error del WINAPI
		_hWnd = CrearControlEx(NULL, L"DArbolEx", L"", NULL, RC.left, RC.bottom, RC.right - RC.left, 200, WS_POPUP | WS_BORDER, WS_EX_NOACTIVATE | WS_EX_TOPMOST | WS_EX_TOOLWINDOW, NULL);  // CS_DBLCLKS (el control recibe notificaciones de doble click)
		Debug_MostrarUltimoError(); // Muestra el error de CreateWindowEx por la consola (si ha sucedido uno)
		Fuente = Fuente18Normal;
//		SetLayeredWindowAttributes(_hWnd, 0, 240, LWA_ALPHA);
		ActualizarDirectorios();
		//SetForegroundWindow(App.VentanaRave.hWnd());
	}


	DDesplegableEx_ExplorarDirectoriosEx::~DDesplegableEx_ExplorarDirectoriosEx(void) {
	}


	void DDesplegableEx_ExplorarDirectoriosEx::Mostrar(void) {
		RECT RW;
		GetWindowRect(_Padre->hWnd(), &RW);
		MoveWindow(_hWnd, RW.left, RW.bottom, RW.right - RW.left, 200, FALSE);
		SetWindowPos(hWnd(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOACTIVATE | SWP_SHOWWINDOW | SWP_NOMOVE | SWP_NOSIZE);
		_Mostrando = TRUE;
		SetFocus(hWnd());
		// Envio el mensaje WM_NCACTIVATE a la ventana principal para que no se vea como pierde el foco, i parezca que el desplegable es un hijo de la ventana principal
		SendMessage(GetAncestor(_Padre->hWnd(), GA_ROOT), WM_NCACTIVATE, TRUE, 0);
	}

	void DDesplegableEx_ExplorarDirectoriosEx::Evento_FocoPerdido(HWND hWndNuevoFoco) {
		if (_Mostrando == TRUE) {
			_Mostrando = FALSE;
		}
		else {
			Visible(FALSE);
		}
		Debug_Escribir_Varg(L"Foco perdido : %d\n", hWndNuevoFoco);
	}

	void DDesplegableEx_ExplorarDirectoriosEx::Evento_MousePresionado(DEventoMouse &DatosMouse) {
//		SetFocus(_Padre->hWnd()); 
	};

	void DDesplegableEx_ExplorarDirectoriosEx::Evento_MouseSoltado(DEventoMouse &DatosMouse) {
		if (_NodoResaltado != NULL && _NodoResaltado == _NodoPresionado) {													// Si el nodo presionado es el mismo que el nodo resaltado (indica que el nodo presionado es el mismo en el mousedown que en el mouseup)
			if (_NodoMarcado->Activado == TRUE) {																			// Si el nodo está activado (los nodos desactivados son paths a los que no se puede acceder con los privilegios actuales)
				if (_NodoPresionadoParte == _NodoResaltadoParte && _NodoPresionadoParte != DArbolEx_ParteNodo_Expansor) {	// Si la parte presionada del nodo NO es el expansor 
					DExplorarDirectoriosEx_Nodo *Tmp = static_cast<DExplorarDirectoriosEx_Nodo *>(_NodoPresionado);			// Convierto el nodo al formato de ExplorarDirectorios
					if (Tmp->Path.size() != 0) {					// Si el nodo contiene un path válido
						ShowWindow(_hWnd, SW_HIDE);
						_Padre->Texto(Tmp->Path);
						_Padre->AsignarIcono(Tmp->Icono());
						_Padre->Repintar();
						Debug_Escribir_Varg(L"Path seleccionado : %s\n", static_cast<DExplorarDirectoriosEx_Nodo *>(_NodoPresionado)->Path.c_str());
					}
				}
			}
		}
			
	};
	*/
}