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















};