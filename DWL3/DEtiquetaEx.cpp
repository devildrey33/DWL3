#include "pch.h"
#include "DEtiquetaEx.h"
//#include "Rave_Skin.h"

namespace DWL {

	DEtiquetaEx_Skin::DEtiquetaEx_Skin(void) :
		// Colores por defecto
		Fondo				(COLOR_ETIQUETA_FONDO),
		Texto				(COLOR_ETIQUETA_TEXTO),
		TextoSombra			(COLOR_ETIQUETA_TEXTO_SOMBRA),
		// Fuente
		FuenteTam			(FUENTE_NORMAL),
		FuenteNombre		(FUENTE_NOMBRE),
		FuenteNegrita		(FALSE),
		FuenteCursiva		(FALSE),
		FuenteSubrayado		(FALSE),
		FuenteSombraTexto	(TRUE) {

	}


	DEtiquetaEx::DEtiquetaEx(void) : Alineacion(DEtiquetaEx_Alineacion_Izquierda) /*, ColorTexto(Skin.Texto), ColorTextoSombra(Skin.TextoSombra), ColorFondo(Skin.Fondo) */ {
	}


	DEtiquetaEx::~DEtiquetaEx(void) {
	}

	HWND DEtiquetaEx::CrearEtiquetaEx(DhWnd *nPadre, const TCHAR *nTxt, const int cX, const int cY, const int cAncho, const int cAlto, const int cID, const DEtiquetaEx_Alineacion nAlineacion, const long Estilos) {
		_hWnd = CrearControlEx(nPadre, L"DEtiquetaEx", L"", cID, cX, cY, cAncho, cAlto, Estilos, NULL);
		Alineacion = nAlineacion;
/*		switch (nAlineacion) {
			case DEtiquetaEx_Alineacion_Izquierda	: _Formato = DT_LEFT;		break;
			case DEtiquetaEx_Alineacion_Centrado	: _Formato = DT_CENTER;		break;
			case DEtiquetaEx_Alineacion_Derecha		: _Formato = DT_RIGHT;		break;
		}*/
//		_Formato = (nCentrado == TRUE) ? DT_CENTER : DT_LEFT;
		Fuente.CrearFuente(Skin.FuenteTam, Skin.FuenteNombre.c_str(), Skin.FuenteNegrita, Skin.FuenteCursiva, Skin.FuenteSubrayado);
		_Texto = nTxt;
		return hWnd();
	}


	void DEtiquetaEx::Pintar(HDC DC) {
		RECT RC;
		GetClientRect(hWnd(), &RC);

		// BackBuffer 
		HDC     TmphDC		= CreateCompatibleDC(NULL);
		HBITMAP TmphDCBmp	= CreateCompatibleBitmap(DC, RC.right, RC.bottom);
		HBITMAP VTmphDCBmp	= static_cast<HBITMAP>(SelectObject(TmphDC, TmphDCBmp));
		HFONT	VFont		= static_cast<HFONT>(SelectObject(TmphDC, Fuente()));

		HBRUSH BrochaFondo = CreateSolidBrush(Skin.Fondo);
		FillRect(TmphDC, &RC, BrochaFondo);
		DeleteObject(BrochaFondo);

		RC.right--; // (resto uno para dejar espacio a la sombra) 

		RECT RC2 = RC;
		RC2.left++; RC2.top++; RC2.right++; RC2.bottom++;

		SetBkMode(TmphDC, TRANSPARENT);
		if (Skin.FuenteSombraTexto == TRUE) {
			// Pinto la sombra del texto
			SetTextColor(TmphDC, Skin.TextoSombra);
			DrawText(TmphDC, _Texto.c_str(), static_cast<int>(_Texto.size()), &RC2, static_cast<UINT>(Alineacion) | DT_WORDBREAK);
		}
		// Pinto el texto
		SetTextColor(TmphDC, Skin.Texto);
		DrawText(TmphDC, _Texto.c_str(), static_cast<int>(_Texto.size()), &RC, static_cast<UINT>(Alineacion) | DT_WORDBREAK);

		RC.right++;	// (vuelvo a poner el right como estaba para el bitblt)

		BitBlt(DC, RC.left, RC.top, RC.right, RC.bottom, TmphDC, 0, 0, SRCCOPY);
//		EndPaint(hWnd(), &PS);

		SelectObject(TmphDC, VFont);
		SelectObject(TmphDC, VTmphDCBmp);
		DeleteObject(TmphDCBmp);
		DeleteDC(TmphDC);
	}

	void DEtiquetaEx::Texto(std::wstring &nTexto) {
		_Texto = nTexto;
		Repintar();
	}

	LRESULT CALLBACK DEtiquetaEx::GestorMensajes(UINT uMsg, WPARAM wParam, LPARAM lParam) {
		switch (uMsg) {
			case WM_PAINT :
				HDC         DC;
				PAINTSTRUCT PS;
				DC = BeginPaint(hWnd(), &PS);
				Pintar(DC);
				EndPaint(hWnd(), &PS);
				return 0;
				break;
		}

		return DControlEx::GestorMensajes(uMsg, wParam, lParam);
	}
}