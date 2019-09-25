#include "pch.h"
#include "DBarraDesplazamientoEx.h"
//#include "Rave_Skin.h"
#include "DMensajesWnd.h"

namespace DWL {

	DBarraDesplazamientoEx::DBarraDesplazamientoEx(void) :	
		DBarraProgresoEx(),
		_MostrarToolTip(DBarraEx_ToolTip_SinToolTip),
		// Eventos Lambda enlazados a los eventos virtuales por defecto
		EventoMouseMovimiento([=](DEventoMouse& d)			{ Evento_MouseMovimiento(d);	}),
		EventoMousePresionado([=](DEventoMouse& d)			{ Evento_MousePresionado(d);	}), 
		EventoMouseSoltado([=](DEventoMouse& d)				{ Evento_MouseSoltado(d);		}),
		EventoMostrarToolTip([=](float v, std::wstring& s)	{ Evento_MostrarToolTip(v, s);	})  {
	}

	DBarraDesplazamientoEx::~DBarraDesplazamientoEx(void) {
	}


	HWND DBarraDesplazamientoEx::CrearBarraDesplazamientoEx(DhWnd *nPadre, const int cX, const int cY, const int cAncho, const int cAlto, const INT_PTR cID, const float nMinimo, const float nMaximo, const float nValor, const DBarraEx_Alineacion nAlineacion) {
//		if (hWnd()) { Debug_Escribir(L"DBarraDesplazamientoEx::CrearBarraEx() Error : ya se ha creado la barra\n"); return hWnd(); }
		_hWnd = CrearControlEx(nPadre, L"DBarraDesplazamientoEx", L"", cID, cX, cY, cAncho, cAlto, WS_CHILD | WS_VISIBLE, NULL, CS_HREDRAW | CS_VREDRAW);
		_Fuente.CrearFuente(Skin.FuenteTam, Skin.FuenteNombre.c_str(), Skin.FuenteNegrita, Skin.FuenteCursiva);
		_Alineacion = nAlineacion;
		_Minimo		= nMinimo;
		_Maximo		= nMaximo;
		_Valor		= nValor;
		if (_Activado == TRUE) {
			_ColorBarra = Skin.BarraNormal;
			_ColorFondo = Skin.FondoNormal;
			_ColorBorde = Skin.BordeNormal;
		}
		else {
			_ColorBarra = Skin.BarraDesactivado;
			_ColorFondo = Skin.FondoDesactivado;
			_ColorBorde = Skin.BordeNormal;
			EnableWindow(_hWnd, FALSE);
		}

		_ToolTip.CrearToolTipEx(nPadre);
		return hWnd();
	}

	void DBarraDesplazamientoEx::ToolTip(DBarraEx_ToolTip nValor) {
		_MostrarToolTip = nValor;
	}

	void DBarraDesplazamientoEx::OcultarToolTip(void) {
		_ToolTip.Visible(FALSE);
	}

	void DBarraDesplazamientoEx::Evento_PintarBarra(HDC DC, RECT &RBarra) {
		COLORREF Color = _ColorBarra;
		HBRUSH BrochaBarra = CreateSolidBrush(Color);
		FillRect(DC, &RBarra, BrochaBarra);
		DeleteObject(BrochaBarra);
	}

	void DBarraDesplazamientoEx::Evento_PintarFondo(HDC DC, RECT &RFondo) {
		COLORREF Color = _ColorFondo;
		HBRUSH BrochaFondo = CreateSolidBrush(Color);
		FillRect(DC, &RFondo, BrochaFondo);
		DeleteObject(BrochaFondo);
	}

	void DBarraDesplazamientoEx::Evento_PintarBorde(HDC DC, RECT &RBorde) {
		COLORREF Color = _ColorBorde;
		HBRUSH BrochaBorde = CreateSolidBrush(Color);
		FrameRect(DC, &RBorde, BrochaBorde);
		DeleteObject(BrochaBorde);
	}


	void DBarraDesplazamientoEx::_Evento_MouseMovimiento(WPARAM wParam, LPARAM lParam) {

		if (_MouseEntrando() == TRUE) {
			// Mouse enter
			if (_Estado != DBarraEx_Estado_Presionado) {
				_Estado = DBarraEx_Estado_Resaltado;
				//Repintar();
				//Resaltar(TRUE);
				Transicion(DBarraEx_Transicion_Resaltado);
			}
		}

		DEventoMouse DatosMouse(wParam, lParam, this);
		RECT  RC = { 0, 0, 0, 0 }, RW = { 0, 0, 0, 0 };
		GetClientRect(_hWnd, &RC);
		GetWindowRect(_hWnd, &RW);
		int cX = DatosMouse.X(), cY = DatosMouse.Y();
		float valor = _ValorMouse(RC, cX, cY);		

		std::wstring TextoToolTip;
		EventoMostrarToolTip(valor, TextoToolTip);

		switch (_MostrarToolTip) {
			case DBarraEx_ToolTip_Arriba	:	_ToolTip.Mostrar(RW.left + cX, RW.top - 35, TextoToolTip);		break;
			case DBarraEx_ToolTip_Abajo		:	_ToolTip.Mostrar(RW.left + cX, RW.bottom + 10, TextoToolTip);	break;
			case DBarraEx_ToolTip_Izquierda	:	_ToolTip.Mostrar(RW.left -35, RW.top + cY, TextoToolTip);		break;
			case DBarraEx_ToolTip_Derecha	:	_ToolTip.Mostrar(RW.right + 35, RW.top + cY, TextoToolTip);		break;
		}

		if (_Estado == DBarraEx_Estado_Presionado) {
			#if DBARRAEX_DEBUG == TRUE
				Debug_Escribir_Varg(L"DBarraDesplazamientoEx::_Evento_MouseMovimiento Min : %02f, Max : %02f, Valor : %02f\n", _Minimo, _Maximo, _Valor);
			#endif
			_Valor = valor;
			Repintar();
			SendMessage(GetParent(hWnd()), DWL_BARRAEX_CAMBIANDO, DEVENTOMOUSE_TO_WPARAM(DatosMouse), 0);
			EventoMouseMovimiento(DatosMouse);
		}
	}

	void DBarraDesplazamientoEx::Evento_MostrarToolTip(float nValor, std::wstring &Texto) {
		Texto = std::to_wstring(nValor);
	}

	void DBarraDesplazamientoEx::_Evento_MousePresionado(const int Boton, WPARAM wParam, LPARAM lParam) {
		DEventoMouse DatosMouse(wParam, lParam, this, Boton);
		POINT Pt = { DatosMouse.X(), DatosMouse.Y() };
		RECT  RC = { 0, 0, 0, 0 };
		GetClientRect(hWnd(), &RC);
		if (PtInRect(&RC, Pt) == TRUE) {
			SetCapture(hWnd());
			_Estado = DBarraEx_Estado_Presionado;
			_Valor = _ValorMouse(RC, Pt.x, Pt.y);
			//Repintar();
			SendMessage(GetParent(hWnd()), DWL_BARRAEX_CAMBIANDO, DEVENTOMOUSE_TO_WPARAM(DatosMouse), 0);
			EventoMousePresionado(DatosMouse);
			Transicion(DBarraEx_Transicion_Presionado);
		}
	}

	void DBarraDesplazamientoEx::_Evento_MouseSoltado(const int Boton, WPARAM wParam, LPARAM lParam) {
		DEventoMouse DatosMouse(wParam, lParam, this, Boton);
		POINT Pt = { DatosMouse.X(), DatosMouse.Y() };
		RECT  RC = { 0, 0, 0, 0 };
		GetClientRect(hWnd(), &RC);
		ReleaseCapture();
		if (PtInRect(&RC, Pt) == TRUE) { 
			_Estado = DBarraEx_Estado_Resaltado; 
			Transicion(DBarraEx_Transicion_Resaltado);
		}
		else {
			// No hace falta transición (saltara la del mousesaliendo)
			_Estado = DBarraEx_Estado_Normal; 
		}
		_Valor = _ValorMouse(RC, Pt.x, Pt.y);
		SendMessage(GetParent(hWnd()), DWL_BARRAEX_CAMBIADO, DEVENTOMOUSE_TO_WPARAM(DatosMouse), 0);
	}

	void DBarraDesplazamientoEx::_Evento_MouseSaliendo(void) {
		_ToolTip.Ocultar();
		_MouseDentro = FALSE;
		if (_Estado != DBarraEx_Estado_Presionado) {
			_Estado = DBarraEx_Estado_Normal;
			//Repintar();
			Transicion(DBarraEx_Transicion_Normal);
		}
	}


	LRESULT CALLBACK DBarraDesplazamientoEx::GestorMensajes(UINT uMsg, WPARAM wParam, LPARAM lParam) {
		switch (uMsg) {
			case WM_ERASEBKGND:
				PintarBarraEx(reinterpret_cast<HDC>(wParam));
				return TRUE;
			case WM_PAINT:
				HDC         DC;
				PAINTSTRUCT PS;
				DC = BeginPaint(hWnd(), &PS);
				PintarBarraEx(DC);
				EndPaint(hWnd(), &PS);
				return 0;
			case WM_MOUSEMOVE:
				_Evento_MouseMovimiento(wParam, lParam);
				return 0;
			case WM_MOUSELEAVE:
				_Evento_MouseSaliendo();
				return 0;
			case WM_LBUTTONDOWN:
				_Evento_MousePresionado(0, wParam, lParam);
				return 0;
			case WM_RBUTTONDOWN:
				_Evento_MousePresionado(1, wParam, lParam);
				return 0;
			case WM_MBUTTONDOWN:
				_Evento_MousePresionado(2, wParam, lParam);
				return 0;
			case WM_LBUTTONUP:
				_Evento_MouseSoltado(0, wParam, lParam);
				return 0;
			case WM_RBUTTONUP:
				_Evento_MouseSoltado(1, wParam, lParam);
				return 0;
			case WM_MBUTTONUP:
				_Evento_MouseSoltado(2, wParam, lParam);
				return 0;
		}
		return DControlEx::GestorMensajes(uMsg, wParam, lParam);
	}

}