#include "pch.h"
#include "DControlEx.h"
#include "DEventoTeclado.h"
#include "DMarcoScrollEx.h"

namespace DWL {

	HWND DControlEx::CrearControlEx(DhWnd *nPadre, const TCHAR *nNombre, const TCHAR *nTexto, const INT_PTR cID, const int cX, const int cY, const int cAncho, const int cAlto, DWORD nEstilos, DWORD nEstilosExtendidos, UINT nEstilosClase, HBRUSH nColorFondo) {
		// Si existe el hWnd pero la ventana / control ya no existe
		if (_hWnd != NULL && IsWindow(_hWnd) == 0) { Destruir(); }
		// Si el control ya se habia creado anteriormente
		if (hWnd()) { 
			Debug_Escribir(L"DControlEx::Crear() Error : ya se ha creado el control extendido\n"); 
			// Devuelvo el hWnd del control que ya estaba creado
			return hWnd();
		}
		// Registro la clase para este control
		ATOM CA = RegistrarClase(nNombre, _GestorMensajes, nEstilosClase);
		// Guardo el padre
		_Padre = nPadre;
		// HWND padre, por defecto es HWND_DESKTOP si nPadre es NULL
		HWND hWndPadre = HWND_DESKTOP;
		// Compruebo si existe el padre
		if (nPadre != nullptr) {
			// Si el padre es un control del tipo MarcoScrollEx y este control no es del tipo DhWnd_Tipo_MarcoScrollEx_Pagina, tengo que crear el control dentro de su hijo _Pagina			
			// En caso contrario es una ventana / control normal, le paso su HWND 
//			if (nPadre->TipoWnd() == DhWnd_Tipo_MarcoScrollEx && TipoWnd() != DhWnd_Tipo_MarcoScrollEx_Pagina) 	hWndPadre = static_cast<DMarcoScrollEx *>(nPadre)->_Pagina.hWnd();
			if (nPadre->TipoWnd() == DhWnd_Tipo_MarcoScrollEx && TipoWnd() != DhWnd_Tipo_MarcoScrollEx_Marco) 	hWndPadre = static_cast<DMarcoScrollEx *>(nPadre)->_Marco.hWnd();
			else																								hWndPadre = nPadre->hWnd();
		}
		// Aseguro que el mouse no este marcado como que está dentro del control
		_MouseDentro = FALSE;
		// Creo el control
		_hWnd = CreateWindowEx(nEstilosExtendidos, nNombre, nTexto, nEstilos, cX, cY, cAncho, cAlto, hWndPadre, reinterpret_cast<HMENU>(cID), GetModuleHandle(NULL), this);
		#ifdef _DEBUG
			// Si hay un error lo muestro en la consola
			if (_hWnd == nullptr) Debug_MostrarUltimoError();
		#endif

		// Evento control creado del padre
		if (nPadre != nullptr) nPadre->Evento_ControlCreado(this);

		return _hWnd;
	};


	void DControlEx::_ConectarControl(const UINT nID, DhWnd *nPadre) {
		if (hWnd() != NULL) {
			SetWindowLongPtr(hWnd(), GWLP_USERDATA, (LONG_PTR)this);
			SetWindowLongPtr(hWnd(), GWLP_WNDPROC, (LONG_PTR)_GestorMensajes);
//			SendMessage(hWnd(), WM_SETFONT, (WPARAM)Fuente18Normal(), 0);
		}
	};


	LRESULT CALLBACK DControlEx::_GestorMensajes(HWND nhWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
		switch (uMsg) {
			case WM_CREATE: {
				DControlEx *PreControlEx = reinterpret_cast<DControlEx *>(((CREATESTRUCT *)lParam)->lpCreateParams);
				if (PreControlEx == NULL) return FALSE;
				PreControlEx->_hWnd = nhWnd;
				SetWindowLongPtr(nhWnd, GWLP_USERDATA, (LONG_PTR)PreControlEx);
				PreControlEx->GestorMensajes(uMsg, wParam, lParam);
				return TRUE;
			}
//			case WM_NCDESTROY :
//				return DefWindowProc(nhWnd, uMsg, wParam, lParam);
			default: {
					//				HWND h = GetParent(nhWnd);
				DControlEx* ControlEx = reinterpret_cast<DControlEx*>(GetWindowLongPtr(nhWnd, GWLP_USERDATA));
				if (ControlEx != NULL) {
					return ControlEx->GestorMensajes(uMsg, wParam, lParam);
					//					if (Ret != DWL_USAR_GESTOR_POR_DEFECTO) return 0;
				}
			}
		}
		return DefWindowProc(nhWnd, uMsg, wParam, lParam);
	};

	/* Devuleve TRUE si el mouse acaba de entrar en el área del control, FALSE si está fuera o ya estaba dentro del área del control */
	/* De esta forma se simula el mensaje WM_MOUSEENTER */
/*	const BOOL DControlEx::_MouseEntrando(void) {
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
	};*/

	LRESULT CALLBACK DControlEx::GestorMensajes(UINT uMsg, WPARAM wParam, LPARAM lParam) {
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
