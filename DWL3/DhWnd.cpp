#include "pch.h"
#include "DhWnd.h"

//#include "resource.h"
#include "DMensajesWnd.h"
#include "DWL_Configuracion.h"

namespace DWL {

/*	DhWnd_Fuente DhWnd::Fuente16Normal;
	DhWnd_Fuente DhWnd::Fuente18Normal;
	DhWnd_Fuente DhWnd::Fuente18Negrita;
	DhWnd_Fuente DhWnd::Fuente21Normal;
	DhWnd_Fuente DhWnd::Fuente21Negrita;
	DhWnd_Fuente DhWnd::FuenteTest;*/

//	bool         DhWnd::Teclado[256];// = { false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,	false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,	false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,	false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,	false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,	false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,	false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,	false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,	false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,	false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,	false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,	false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,	false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false };
	DWORD        DhWnd::TiempoAnimaciones = 400;

	//void DhWnd::IniciarMiembrosEstaticos(void) {
		// Creamos las fuentes si no existen, y limpiamos el buffer para el teclado
/*		if (Fuente18Normal.Fuente() == NULL) {
//			BorrarBufferTeclado();
			Fuente16Normal	= CreateFont(FUENTE_PEQUE, 0, 0, 0, FW_NORMAL, false, false, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, FF_ROMAN, FUENTE_NOMBRE);
			Fuente18Normal	= CreateFont(FUENTE_NORMAL, 0, 0, 0, FW_NORMAL, false, false, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, FF_ROMAN, FUENTE_NOMBRE);
			Fuente18Negrita	= CreateFont(FUENTE_NORMAL, 0, 0, 0, FW_BOLD, false, false, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, FF_ROMAN, FUENTE_NOMBRE);
			Fuente21Normal	= CreateFont(FUENTE_GRANDE, 0, 0, 0, FW_NORMAL, false, false, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, FF_ROMAN, FUENTE_NOMBRE);
			Fuente21Negrita	= CreateFont(FUENTE_GRANDE, 0, 0, 0, FW_BOLD, false, false, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, FF_ROMAN, FUENTE_NOMBRE);
			FuenteTest		= CreateFont(26, 0, 0, 0, FW_BOLD, false, false, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, FF_ROMAN, FUENTE_NOMBRE);
		}*/
	//}

	/*void DhWnd::EliminarFuentesEstaticas(void) {
		Fuente18Normal.Destruir();
		Fuente18Negrita.Destruir();
		Fuente21Normal.Destruir();
		Fuente21Negrita.Destruir();
		FuenteTest.Destruir();
	}**/

	ATOM DhWnd::RegistrarClase(const TCHAR *nNombre, WNDPROC WindowProcedureInicial, UINT nEstilos, const int nIconoRecursos, HBRUSH nColorFondo, HINSTANCE hInstance) {
		
		WNDCLASSEX WndClass;
		WndClass.cbSize = sizeof(WNDCLASSEX);
		WndClass.style = nEstilos; // CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
		WndClass.lpfnWndProc = reinterpret_cast<WNDPROC>(WindowProcedureInicial);
		WndClass.cbClsExtra = 0;
		WndClass.cbWndExtra = 0;
		if (hInstance == NULL) hInstance = GetModuleHandle(NULL);
		WndClass.hInstance = hInstance;
		if (nIconoRecursos != 0) {
			WndClass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(nIconoRecursos));
			WndClass.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(nIconoRecursos));
		}
		else {
			WndClass.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
			WndClass.hIconSm = LoadIcon(hInstance, IDI_APPLICATION);
		}
		WndClass.hCursor = LoadCursor(0, IDC_ARROW);
		// Probar a utilitzar un CreateSolidBrush(COLOR_FONDO) y comentar els WM_ERASEBKGRND aviam si ho fa igual
		if (nColorFondo != NULL) {
			WndClass.hbrBackground = nColorFondo;
		}
		else {			
			WndClass.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
			//			WndClass.hbrBackground = (HBRUSH)(COLOR_APPWORKSPACE + 1);
		}
		WndClass.lpszMenuName = 0;
		WndClass.lpszClassName = nNombre;
		return RegisterClassEx(&WndClass);
	}


	const BOOL DhWnd::Maximizada(void) {
		WINDOWPLACEMENT WndPl = { 0, 0, 0, { 0, 0}, {0,0}, {0, 0, 0,0} };
		GetWindowPlacement(_hWnd, &WndPl);
		if (WndPl.showCmd == SW_MAXIMIZE) return TRUE;
		return FALSE;
	};

	/* Devuleve TRUE si el mouse acaba de entrar en el área del control, FALSE si está fuera o ya estaba dentro del área del control */
	/* De esta forma se simula el mensaje WM_MOUSEENTER */
	const BOOL DhWnd::_MouseEntrando(void) {
		if (_MouseDentro == FALSE) {
			TRACKMOUSEEVENT Trk;
			Trk.cbSize = sizeof(TRACKMOUSEEVENT);
			Trk.dwFlags = TME_LEAVE;
			Trk.hwndTrack = hWnd();
			TrackMouseEvent(&Trk);
			_MouseDentro = TRUE;
			return TRUE;
		}
		return FALSE;
	};



/*	void DhWnd::WM_ParentNotify(WPARAM wParam, LPARAM lParam) {
		// Se ha destruido un control hijo
		if (LOWORD(wParam) == WM_DESTROY) {
			#if DHWND_MOSTRARDEBUG == TRUE
				Debug_Escribir_Varg(L"DhWnd::WM_ParentNotify Destroy -> %d %d", HIWORD(wParam), lParam);
			#endif

			for (size_t i = 0; i < _Hijos.size(); i++) {
				if (_Hijos[i]->ID() == HIWORD(wParam) && (HWND)lParam == _Hijos[i]->_hWnd) {
					_Hijos[i]->_hWnd = nullptr;
					return;
				}
			}
		}
	}

	// Libero los window procedures
	void DhWnd::WM_Close(DhWnd *C) {
		if (C == nullptr) C = this;
		for (size_t i = 0; i < C->_Hijos.size(); i++) {
			if (C->_Hijos[i]->_Hijos.size() != 0) WM_Close(C->_Hijos[i]);
			SetWindowLongPtr(C->_Hijos[i]->_hWnd, GWLP_USERDATA, NULL);
			C->_Hijos[i]->_hWnd = nullptr;
		}
		C->_Hijos.resize(0);
		SetWindowLongPtr(C->_hWnd, GWLP_USERDATA, NULL);
	}*/


	const BOOL DhWnd::Destruir(void) {
		#if DHWND_MOSTRARDEBUG == TRUE
			Debug_Escribir_Varg(L"DhWnd::Destruir %d %d\n", ID(), _hWnd);
		#endif
		if (_hWnd == NULL) {
			return TRUE;
		}

		SetWindowLongPtr(_hWnd, GWLP_USERDATA, NULL);

/*		if (_Padre != NULL) {
			for (size_t i = 0; i < _Padre->_Hijos.size(); i++) {
				if (_Padre->_Hijos[i]->ID() == ID()) {
					// Borro este control de la lista de hijos del padre.
					_Padre->_Hijos.erase(_Padre->_Hijos.begin() + static_cast<unsigned int>(i));
					break;
				}
			}
		}*/
		_Padre = NULL;
		BOOL R = DestroyWindow(_hWnd);
		_hWnd = NULL;
		return R;
	};







};