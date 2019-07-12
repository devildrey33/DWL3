#include "pch.h"
#include "DMensajesWnd.h"
#include "DDlgDirectorios.h"
//#include "resource.h"

namespace DWL {

	#define ID_ARBOLDIRECTORIOS		WM_USER + 1000
	#define ID_EDICIONDIRECTORIO	WM_USER + 1001
	#define ID_BOTONACEPTAR			WM_USER + 1002
	#define ID_BOTONCANCELAR		WM_USER + 1003

	#define MIN_DLGDIRECTORIOS_ANCHO 300
	#define MIN_DLGDIRECTORIOS_ALTO  300

	DDlgDirectorios::DDlgDirectorios(void) : _Padre(NULL), _Terminado(FALSE){
	}


	DDlgDirectorios::~DDlgDirectorios(void) {
	}


	const BOOL DDlgDirectorios::Mostrar(DhWnd *nPadre, std::wstring &rPath, const int cX, const int cY, const int cAncho, const int cAlto, const int ID_Icono) {
		_Padre = nPadre;
		_Terminado = FALSE;
		// Creo la ventana
		CrearVentana(nPadre, L"DDlgDirectorio", L"Selecciona un Directorio", cX, cY, (cAncho > 100) ? cAncho : 100, (cAlto > 100) ? cAlto : 100, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME, WS_EX_APPWINDOW, NULL, NULL, NULL, ID_Icono);
		// Obtengo el tamaño de la recta cliente de la ventana.
		RECT RC;
		GetClientRect(_hWnd, &RC);
		// Creo los controles
		ArbolDirectorios.CrearExplorarDirectoriosEx(this, 10, 10, RC.right - 20, RC.bottom - 100, ID_ARBOLDIRECTORIOS, WS_CHILD | WS_VISIBLE | WS_BORDER);
		ArbolDirectorios.MultiSeleccion = FALSE;
		ArbolDirectorios.SubSeleccion   = FALSE;
		EdicionSeleccion.CrearEdicionTextoEx(this, L"", 10, RC.bottom - 80, RC.right - 20, 24, ID_EDICIONDIRECTORIO, NULL, WS_CHILD | WS_VISIBLE);
		EdicionSeleccion.Entrada = DEdicionTextoEx_Entrada_SinEntrada;
		BotonAceptar.CrearBotonEx(this, L"Aceptar", (RC.right / 2) - 110, RC.bottom - 40, 100, 30, ID_BOTONACEPTAR);
		BotonCancelar.CrearBotonEx(this, L"Cancelar", (RC.right / 2) + 10, RC.bottom - 40, 100, 30, ID_BOTONCANCELAR);
		
		// Muestro los controles
		ArbolDirectorios.Visible(TRUE);
//		BotonAceptar.Visible(TRUE);
		BotonAceptar.Activado(FALSE);
//		BotonCancelar.Visible(TRUE);

		_AjustarControles(RC);

		// Muestro la ventana modal
		Visible(TRUE);
		// Des-activo la ventana padre
		nPadre->Activado(FALSE);

		// Bucle de mensajes que espera la respuesta del usuario hasta devolver el control a la ventana principal
		MSG		msg;
		HACCEL	hAccelTable = LoadAccelerators(GetModuleHandle(NULL), MAKEINTRESOURCE(ID_Icono));
		while (GetMessage(&msg, NULL, 0, 0) && _Terminado == FALSE) {
			if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}

		Visible(FALSE);


		rPath  = EdicionSeleccion.Texto();
//		rIcono = EdicionSeleccion.Icono();

		Destruir();

		// Re-activo la ventana padre
		nPadre->Activado(TRUE);
		nPadre->AsignarFoco();

		// Si se ha pulsado cancelar devuleve FALSE
		return (_Terminado == 2) ? FALSE : TRUE;
	}

	void DDlgDirectorios::Evento_Cerrar(void) {
		_Terminado = 2; // cancelado
	}

	void DDlgDirectorios::Evento_BotonEx_Mouse_Click(DWL::DEventoMouse &DatosMouse) {
		switch (DatosMouse.ID()) {
			case ID_BOTONACEPTAR :
				_Terminado = 1;
				break;
			case ID_BOTONCANCELAR :
				_Terminado = 2;
				break;
		}
		
	}

	void DDlgDirectorios::Evento_BorraFondo(HDC DC) {
		RECT RC;
		GetClientRect(hWnd(), &RC);
		HBRUSH BrochaFondo = CreateSolidBrush(COLOR_FONDO);
		FillRect(DC, &RC, BrochaFondo);
		DeleteObject(BrochaFondo);
	}


	void DDlgDirectorios::_ActualizarAceptar(void) {
		DExplorarDirectoriosEx_Nodo *Nodo = ArbolDirectorios.PathResaltado();
		BOOL nActivar = FALSE;
		// SI hay un nodo
		if (Nodo != NULL) {
			// Si el nodo está activado
			if (Nodo->Activado() == TRUE) {
				// Si no es un nodo de la raíz
				if (Nodo != ArbolDirectorios.PathEscritorio() &&
					Nodo != ArbolDirectorios.PathMiPC() &&
					Nodo != ArbolDirectorios.PathRed() &&
					Nodo != ArbolDirectorios.PathMisDocumentos()) {

					nActivar = TRUE;
					EdicionSeleccion.Texto(Nodo->Path, FALSE);
					EdicionSeleccion.Icono(&Nodo->Icono(), TRUE);
				}
			}
		}

		BotonAceptar.Activado(nActivar);
	}


	void DDlgDirectorios::Evento_CambiandoTam(const UINT Lado, RECT* Rectangulo) {
		// Comprobación de tamaño mínimo
		if ((Rectangulo->right - Rectangulo->left) < MIN_DLGDIRECTORIOS_ANCHO)		Rectangulo->right  = Rectangulo->left + MIN_DLGDIRECTORIOS_ANCHO;
		if ((Rectangulo->bottom - Rectangulo->top) < MIN_DLGDIRECTORIOS_ALTO)		Rectangulo->bottom = Rectangulo->top  + MIN_DLGDIRECTORIOS_ALTO;

		RECT RC;
		RC.left   = 0;
		RC.top    = 0;
		RC.right  = (Rectangulo->right - Rectangulo->left);
		RC.bottom = (Rectangulo->bottom - Rectangulo->top);


		Debug_Escribir_Varg(L"DDlgDirectorios::Evento_CambiandoTam %d, %d\n", RC.right, RC.bottom);
	}


	void DDlgDirectorios::_AjustarControles(RECT& RC) {
		MoveWindow(ArbolDirectorios.hWnd(), 10, 10, RC.right - 20, RC.bottom - 100, TRUE);
		MoveWindow(EdicionSeleccion.hWnd(), 10, RC.bottom - 80, RC.right - 20, 24, TRUE);
		MoveWindow(BotonAceptar.hWnd(), (RC.right / 2) - 110, RC.bottom - 40, 100, 30, TRUE);
		MoveWindow(BotonCancelar.hWnd(), (RC.right / 2) + 10, RC.bottom - 40, 100, 30, TRUE);
	}


	LRESULT CALLBACK DDlgDirectorios::GestorMensajes(UINT uMsg, WPARAM wParam, LPARAM lParam) {
		switch (uMsg) {
			case WM_ERASEBKGND:
				Evento_BorraFondo(reinterpret_cast<HDC>(wParam));
				return TRUE;
			case WM_CLOSE:
				Evento_Cerrar();
				return 0;
			case DWL_BOTONEX_CLICK:
				Evento_BotonEx_Mouse_Click(WPARAM_TO_DEVENTOMOUSE(wParam));
				return 0;
			case DWL_ARBOLEX_CLICK: 
				_ActualizarAceptar();
				return 0;

			case WM_EXITSIZEMOVE:
//				RECT RCWMS2;
//				GetWindowRect(hWnd(), &RCWMS2);
//				App.BD.Opciones_GuardarPosTamDlgDirectorios(RCWMS2);
				Debug_Escribir_Varg(L"DDlgDirectorios::WM_EXITSIZEMOVE %d, %d\n", wParam, lParam);
				SendMessage(_Padre->hWnd(), DWL_DLGDIRECTORIOS_POSTAM_CAMBIADO, HWND_TO_WPARAM(_hWnd), 0);
				return 0;
			case WM_SIZE:
				RECT RCWMS;
				GetClientRect(hWnd(), &RCWMS);
				_AjustarControles(RCWMS);
				Debug_Escribir_Varg(L"DDlgDirectorios::WM_SIZE %d, %d\n", wParam, lParam);
				return 0;
			case WM_SIZING:
				Evento_CambiandoTam(static_cast<UINT>(wParam), reinterpret_cast<RECT*>(lParam));
				return 0;

		}
		return DefWindowProc(hWnd(), uMsg, wParam, lParam);		
	}
};