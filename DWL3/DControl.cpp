#include "pch.h"
#include "DControl.h"
#include "DEventoTeclado.h"
//#include "DConsola.h"

namespace DWL {




	HWND DControl::CrearControl(DhWnd *nPadre, const TCHAR *nNombre, const TCHAR *nTexto, const INT_PTR cID, const int cX, const int cY, const int cAncho, const int cAlto, DWORD nEstilos, DWORD nEstilosExtendidos, HBRUSH nColorFondo) {
		// La ventana ya no existe pero tenemos el hWnd, por lo que elimino toda la memória
		if (_hWnd != NULL && IsWindow(_hWnd) == 0) { Destruir(); }
		if (hWnd()) { Debug_Escribir(L"DControl::Crear() Error : ya se ha creado el control\n"); return hWnd(); }

		HWND hWndPadre = (nPadre != NULL) ? nPadre->hWnd() : NULL;

		_hWnd = CreateWindowEx(nEstilosExtendidos, nNombre, nTexto, nEstilos, cX, cY, cAncho, cAlto, hWndPadre, reinterpret_cast<HMENU>(cID), GetModuleHandle(NULL), this);

		// Evento control creado del padre
		if (nPadre != nullptr) nPadre->Evento_ControlCreado(this);

		return hWnd();
		//	return CreateWindowEx(nEstiloExtendido, nNombre, nTexto, Estilo, cX, cY, cAncho, cAlto, nPadre(), reinterpret_cast<HMENU>(IntToPtr(nID)), GetModuleHandle(NULL), this);
	};


	LRESULT CALLBACK DControl::_GestorMensajes(HWND nhWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
		DControl *Control = reinterpret_cast<DControl *>(GetWindowLongPtr(nhWnd, GWLP_USERDATA));
		if (Control == NULL) return FALSE;
		return Control->GestorMensajes(uMsg, wParam, lParam);
	};

	void DControl::_ConectarControl(const UINT nID, DhWnd *nPadre) {
		if (hWnd() != NULL) {
			SetWindowLongPtr(hWnd(), GWLP_USERDATA, (LONG_PTR)this);
			_GestorMensajesOriginal = reinterpret_cast<WNDPROC>(SetWindowLongPtr(hWnd(), GWLP_WNDPROC, (LONG_PTR)_GestorMensajes));

//			SendMessage(hWnd(), WM_SETFONT, (WPARAM)Fuente18Normal(), 0);
		}
	};

	LRESULT CALLBACK DControl::GestorMensajes(UINT uMsg, WPARAM wParam, LPARAM lParam) {
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


};