#include "pch.h"
#include "DVentana.h"
#include "DEventoTeclado.h"

namespace DWL {

	
	HWND DVentana::CrearVentana(DhWnd *nPadre, const TCHAR *nNombre, const TCHAR *nTexto, const int cX, const int cY, const int cAncho, const int cAlto, DWORD nEstilos, DWORD nEstilosExtendidos, UINT nEstilosClase, HMENU nMenu, HBRUSH nColorFondo, const int nIconoRecursos) {
		// La ventana ya no existe pero tenemos el hWnd, por lo que elimino toda la memória
		if (_hWnd != NULL && IsWindow(_hWnd) == 0) { Destruir(); }

		if (_hWnd) {
//			Debug_Escribir(L"DVentana::Crear() Error : ya se ha creado la ventana\n"); 
			return hWnd(); 
		}
		// hWnd del padre
		HWND hWndPadre = (nPadre != NULL) ? nPadre->hWnd() : HWND_DESKTOP;

		// Afegit expresament per borrar el fondo amb els colors del RAVE
		HBRUSH ColFondo = CreateSolidBrush(COLOR_FONDO); // NO ELIMINAR LA BROCHA DE MEMORIA, HO FA EL WINDOWS
		
		ATOM RetRgistrarClase = RegistrarClase(nNombre, reinterpret_cast<WNDPROC>(_GestorMensajes), nEstilosClase, nIconoRecursos, ColFondo);
		
		// Recalculo el espacio para que el área cliente de la ventana mida exactamente el ancho y alto especificados
		RECT RC = { cX, cY, cX + cAncho, cY + cAlto };
		AdjustWindowRectEx(&RC, nEstilos, FALSE, nEstilosExtendidos);

		_hWnd = CreateWindowEx(nEstilosExtendidos, nNombre, nTexto, nEstilos, RC.left, RC.top, RC.right - RC.left, RC.bottom - RC.top, hWndPadre, nMenu, GetModuleHandle(NULL), this);
		Debug_MostrarUltimoError();
//		SendMessage(hWnd(), WM_SETFONT, (WPARAM)Fuente18Normal(), 0);
		BarraTareas._Iniciar(_hWnd);
		return hWnd();
	}

	void DVentana::Titulo(std::wstring & nTitulo) {
		SetWindowText(hWnd(), nTitulo.c_str());
	}

	LRESULT CALLBACK DVentana::_GestorMensajes(HWND nhWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
		switch (uMsg) {
			case WM_CREATE: {
				DVentana *PreVentana = reinterpret_cast<DVentana *>(((CREATESTRUCT *)lParam)->lpCreateParams);
				if (PreVentana == NULL) return FALSE;
				PreVentana->_hWnd = nhWnd;
				SetWindowLongPtr(nhWnd, GWLP_USERDATA, (LONG_PTR)PreVentana);
				PreVentana->GestorMensajes(uMsg, wParam, lParam);
				return TRUE;
			}
			default: {
				DVentana *Ventana = reinterpret_cast<DVentana *>(::GetWindowLongPtr(nhWnd, GWLP_USERDATA));
				if (Ventana != NULL) {
					return Ventana->GestorMensajes(uMsg, wParam, lParam);
					//					if (Ret != DWL_USAR_GESTOR_POR_DEFECTO) return 0;
				}
			}
		}
		return DefWindowProc(nhWnd, uMsg, wParam, lParam);
	}

	// Establece la opacidad de la ventana (0 transparente, 255 solido)
	void DVentana::Opacidad(const BYTE nNivel) {
		SetWindowLongPtr(_hWnd, GWL_EXSTYLE, WS_EX_LAYERED | WS_EX_TOPMOST | WS_EX_TOOLWINDOW);
		SetLayeredWindowAttributes(_hWnd, 0, nNivel, LWA_ALPHA);
	};

	const BYTE DVentana::Opacidad(void) {
		BYTE Ret;
		DWORD Params = LWA_ALPHA;
		GetLayeredWindowAttributes(_hWnd, NULL, &Ret, &Params);
		return Ret;
	}

	LRESULT CALLBACK DVentana::GestorMensajes(UINT uMsg, WPARAM wParam, LPARAM lParam) {
		DWL::DEventoTeclado ET(wParam, lParam, this);
		switch (uMsg) {
			case WM_KEYDOWN: 
				_Aplicacion->Evento_TeclaPresionada(ET);
				return 0;
			case WM_KEYUP:
				_Aplicacion->Evento_TeclaSoltada(ET);
				return 0;
		}
		return DefWindowProc(hWnd(), uMsg, wParam, lParam);
	}






}