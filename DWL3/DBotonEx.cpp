#include "pch.h"
#include "DBotonEx.h"
//#include "Rave_Skin.h"
#include "DMensajesWnd.h"
#include "DMouse.h"

namespace DWL {

	// Valores visuales por defecto del BotonEx
	// Colores del fondo
	COLORREF		DBotonEx_Skin::FondoNormal			= COLOR_BOTON;
	COLORREF		DBotonEx_Skin::FondoResaltado		= COLOR_BOTON_RESALTADO;
	COLORREF		DBotonEx_Skin::FondoPresionado		= COLOR_BOTON_PRESIONADO;
	COLORREF		DBotonEx_Skin::FondoMarcado			= COLOR_BOTON_MARCADO;
	// Colores del borde
	COLORREF		DBotonEx_Skin::BordeNormal			= COLOR_BOTON_BORDE;
	COLORREF		DBotonEx_Skin::BordeResaltado		= COLOR_BOTON_BORDE_RESALTADO;
	COLORREF		DBotonEx_Skin::BordePresionado		= COLOR_BOTON_BORDE_PRESIONADO;
	// Colores del texto
	COLORREF		DBotonEx_Skin::TextoNormal			= COLOR_BOTON_TEXTO;
	COLORREF		DBotonEx_Skin::TextoResaltado		= COLOR_BOTON_TEXTO_RESALTADO;
	COLORREF		DBotonEx_Skin::TextoPresionado		= COLOR_BOTON_TEXTO_PRESIONADO;
	COLORREF		DBotonEx_Skin::TextoDesactivado		= COLOR_BOTON_TEXTO_DESACTIVADO;
	COLORREF		DBotonEx_Skin::TextoSombra			= COLOR_BOTON_TEXTO_SOMBRA;
	// Tipo de fuente
	int				DBotonEx_Skin::FuenteTam			= FUENTE_GRANDE;
	std::wstring	DBotonEx_Skin::FuenteNombre			= FUENTE_NOMBRE;
	BOOL            DBotonEx_Skin::FuenteNegrita		= TRUE;
	BOOL            DBotonEx_Skin::FuenteCursiva		= FALSE;
	BOOL            DBotonEx_Skin::FuenteSubrayado		= FALSE;

	// Sombra para el texto
	BOOL			DBotonEx_Skin::FuenteSombraTexto	= TRUE;


	DBotonEx::DBotonEx(void) : DControlEx(), _Marcado(FALSE), _PosIconoX(-1), _PosIconoY(-1), _ColorFondo(DBotonEx_Skin::FondoNormal), _ColorBorde(DBotonEx_Skin::BordeNormal), _ColorTexto(DBotonEx_Skin::TextoNormal), _Estado(DBotonEx_Estado_Normal) {
	}


	DBotonEx::~DBotonEx(void) {
	} 

	// Función que crea un BotonEx con texto
	HWND DBotonEx::CrearBotonEx(DhWnd *nPadre, const TCHAR *nTxt, const int cX, const int cY, const int cAncho, const int cAlto, const int cID, const long Estilos) {
		_Texto = nTxt;
		return _CrearBotonEx(nPadre, cX, cY, cAncho, cAlto, cID, Estilos);
	}

	// Función que crea un BotonEx con icono
	HWND DBotonEx::CrearBotonEx(DhWnd *nPadre, const int IDIcono, const int TamIcono, const int PosIconoX, const int PosIconoY, const int cX, const int cY, const int cAncho, const int cAlto, const int cID, const long Estilos) {
		HWND h = _CrearBotonEx(nPadre, cX, cY, cAncho, cAlto, cID, Estilos);
		Icono(IDIcono, TamIcono, PosIconoX, PosIconoY);
		return h;
	}

	// Función que crea un BotonEx con icono y texto
	HWND DBotonEx::CrearBotonEx(DhWnd *nPadre, const int IDIcono, const int TamIcono, const int PosIconoX, const int PosIconoY, const TCHAR *nTxt, const int cX, const int cY, const int cAncho, const int cAlto, const int cID, const long Estilos) {		
		_Texto = nTxt;
		HWND h =_CrearBotonEx(nPadre, cX, cY, cAncho, cAlto, cID, Estilos);
		Icono(IDIcono, TamIcono, PosIconoX, PosIconoY);
		return h;
	}

	// Función que asigna un icono al BotonEx
	void DBotonEx::Icono(const int IDIcono, const int TamIcono, const int PosX, const int PosY) {
		RECT RC;
		GetClientRect(_hWnd, &RC);
		_Icono.CrearIconoRecursos(IDIcono, TamIcono, TamIcono);
		_PosIconoX = (PosX == -1) ? (RC.right - TamIcono) / 2 : PosX;
		_PosIconoY = (PosY == -1) ? (RC.bottom - TamIcono) / 2 : PosX;
		Repintar();
	}

	HWND DBotonEx::_CrearBotonEx(DhWnd *nPadre, const int cX, const int cY, const int cAncho, const int cAlto, const int cID, const long Estilos) {
		_ColorFondo = DBotonEx_Skin::FondoNormal;
		_ColorBorde = DBotonEx_Skin::BordeNormal;
		_ColorTexto = DBotonEx_Skin::TextoNormal;
		_Marcado = FALSE;
//		if (hWnd()) { Debug_Escribir(L"DBotonEx::CrearBotonEx() Error : ya se ha creado el botón\n"); return hWnd(); }
		_hWnd = CrearControlEx(nPadre, L"DBotonEx", L"", cID, cX, cY, cAncho, cAlto, Estilos, NULL);
		Fuente.CrearFuente(DBotonEx_Skin::FuenteTam, DBotonEx_Skin::FuenteNombre.c_str(), DBotonEx_Skin::FuenteNegrita, DBotonEx_Skin::FuenteCursiva, DBotonEx_Skin::FuenteSubrayado);
		_Estado = DBotonEx_Estado_Normal;
		_MouseDentro = FALSE;
		return hWnd();
	}

#if DBOTONEX_GDIPLUS == FALSE

	void DBotonEx::Pintar(HDC DC, const int nX, const int nY) {
		RECT    RC, RCF, RCT, RCS;
		GetClientRect(hWnd(), &RC);
		RCF = RC; RCF.left++; RCF.top++; RCF.right--; RCF.bottom--;
		RCS = RC; RCS.left++; RCS.top++; RCS.right++; RCS.bottom++;
		RCT = RC; 

		// Creo un buffer en memória para pintar el control
		HDC		Buffer   = CreateCompatibleDC(NULL);
		// Creo un DC compatible para el buffer
		HBITMAP Bmp      = CreateCompatibleBitmap(DC, RC.right, RC.bottom);
		HBITMAP BmpViejo = static_cast<HBITMAP>(SelectObject(Buffer, Bmp));
		HFONT   vFuente  = static_cast<HFONT>(SelectObject(Buffer, Fuente()));

		int bPresionado = 0;
		// Si el control está presionado retoco las posiciones del texto i los iconos
		if (_Estado == DBotonEx_Estado_Presionado) {
			RCT.top += 2;		// Recta texto
			RCS.top += 2;		// Recta sombra
			bPresionado = 1;	// x e y iniciales para el icono
		}

		// Pinto el borde
		HBRUSH BrochaBorde = CreateSolidBrush(_ColorBorde);
		FrameRect(Buffer, &RC, BrochaBorde);
		DeleteObject(BrochaBorde);

		// Pinto el fondo
		HBRUSH BrochaFondo = CreateSolidBrush(_ColorFondo);
		FillRect(Buffer, &RCF, BrochaFondo);
		DeleteObject(BrochaFondo);

		// Si tiene texto
		if (_Texto.size() > 0) {
			SetBkMode(Buffer, TRANSPARENT);
			if (DBotonEx_Skin::FuenteSombraTexto == TRUE) {
				// Pinto la sombra del texto
				SetTextColor(Buffer, DBotonEx_Skin::TextoSombra);
				DrawText(Buffer, _Texto.c_str(), static_cast<int>(_Texto.size()), &RCS, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			}

			// Pinto el texto
			SetTextColor(Buffer, _ColorTexto);
			DrawText(Buffer, _Texto.c_str(), static_cast<int>(_Texto.size()), &RCT, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}

		// Si tiene icono
		if (_Icono() != NULL) {
			DrawIconEx(Buffer, bPresionado + _PosIconoX, bPresionado + _PosIconoY, _Icono(), _Icono.Ancho(), _Icono.Alto(), 0, 0, DI_NORMAL);
		}

		// Llamo al evento pintar virtual
		Evento_Pintar(Buffer);

		// Copio el buffer al DC
		BitBlt(DC, nX + RC.left, nY + RC.top, RC.right, RC.bottom, Buffer, 0, 0, SRCCOPY);

		// Elimino el buffer de la memória
		SelectObject(Buffer, vFuente);
		SelectObject(Buffer, BmpViejo);
		DeleteObject(Bmp);
		DeleteDC(Buffer);
	}

#else

	void DBotonEx::PintarP(HDC DC, const int nX, const int nY) {
		Gdiplus::ClientRectF	RC(_hWnd);
		Gdiplus::RectF			RF(RC);
		RF.X++; RF.Y++; RF.Width--; RF.Height--;
		Gdiplus::RectF			RCTS(RC);
		RCTS.X++; RCTS.Y++; RCTS.Width++; RCTS.Height++;


		Gdiplus::Bitmap			BufferBmp(RC.Width, RC.Height);
		Gdiplus::Graphics		Graficos(DC);
		Gdiplus::Graphics	   *Buffer(Gdiplus::Graphics::FromImage(&BufferBmp));
		Gdiplus::Color			ColorBorde(GetRValue(_ColorBorde), GetGValue(_ColorBorde), GetBValue(_ColorBorde));
		Gdiplus::Color			ColorFondo(GetRValue(_ColorFondo), GetGValue(_ColorFondo), GetBValue(_ColorFondo));
		Gdiplus::Color			ColorTexto(GetRValue(_ColorTexto), GetGValue(_ColorTexto), GetBValue(_ColorTexto));
		Gdiplus::Color			ColorSombra(GetRValue(DBotonEx_Skin::TextoSombra), GetGValue(DBotonEx_Skin::TextoSombra), GetBValue(DBotonEx_Skin::TextoSombra));
		Gdiplus::Pen			PlumaBorde(ColorBorde, 1.0f);
		Gdiplus::SolidBrush		BrochaFondo(ColorFondo);
		Gdiplus::SolidBrush		BrochaTexto(ColorTexto);
		Gdiplus::SolidBrush		BrochaTextoSombra(ColorSombra);

		// Pinto el fondo
		Buffer->FillRectangle(&BrochaFondo, RF);

		// Pinto el borde 
		Buffer->DrawRectangle(&PlumaBorde, 0.0f, 0.0f, RC.Width - 1.0f, RC.Height - 1.0f);

		// Si tiene texto
		if (_Texto.size() > 0) {
			Gdiplus::StringFormat SF;
			SF.SetAlignment(Gdiplus::StringAlignmentCenter);
			SF.SetLineAlignment(Gdiplus::StringAlignmentCenter);
			// Pinto la sombra
			Buffer->DrawString(_Texto.c_str(), _Texto.size(), Fuente(), RCTS, &SF , &BrochaTextoSombra);
			// Pinto el texto
			Buffer->DrawString(_Texto.c_str(), _Texto.size(), Fuente(), RC, &SF, &BrochaTexto);
		}

		// Si tiene icono
		if (_Icono() != NULL) {
			Buffer->DrawImage(_Icono(), _PosIconoX, _PosIconoY);
		}

		Graficos.DrawImage(&BufferBmp, PointF(0,0));
	}
#endif

	void DBotonEx::Activado(const BOOL nActivar) {
		BOOL Ret = FALSE;
		Ret = EnableWindow(_hWnd, nActivar);
		Transicion((nActivar == TRUE) ? DBotonEx_Transicion_Normal : DBotonEx_Transicion_Desactivado);
		//Repintar();
	}

	

	void DBotonEx::_Evento_MouseMovimiento(const WPARAM wParam, const LPARAM lParam) {
		DEventoMouse DatosMouse(wParam, lParam, this);
		if (_MouseEntrando() == TRUE) {
			// Mouse enter
			if (_Estado != DBotonEx_Estado_Presionado) {
				_Estado = DBotonEx_Estado_Resaltado;
				Transicion(DBotonEx_Transicion_Resaltado);
			}
		}

		Evento_MouseMovimiento(DatosMouse);
	}


	void DBotonEx::Transicion(const DBotonEx_Transicion nTransicion) {
		DWORD Duracion = DhWnd::TiempoAnimaciones;
		if (_AniTransicion.Animando() == TRUE) {
			Duracion = _AniTransicion.TiempoActual();
			_AniTransicion.Terminar();
		}

		#if DBOTONEX_MOSTRARDEBUG == TRUE
			Debug_Escribir_Varg(L"DBotonEx::Transicion %d\n", static_cast<int>(nTransicion));
		#endif	

		COLORREF FondoHasta = 0, BordeHasta = 0, TextoHasta = 0;
		switch (nTransicion) {
			case DBotonEx_Transicion_Normal:
				FondoHasta = DBotonEx_Skin::FondoNormal;
				BordeHasta = DBotonEx_Skin::BordeNormal;
				TextoHasta = DBotonEx_Skin::TextoNormal;
				break;
			case DBotonEx_Transicion_Resaltado:
				FondoHasta = DBotonEx_Skin::FondoResaltado;
				BordeHasta = DBotonEx_Skin::BordeResaltado;
				TextoHasta = DBotonEx_Skin::TextoResaltado;
				break;
			case DBotonEx_Transicion_Presionado:
				FondoHasta = DBotonEx_Skin::FondoPresionado;
				BordeHasta = DBotonEx_Skin::BordePresionado;
				TextoHasta = DBotonEx_Skin::TextoPresionado;
				break;
			case DBotonEx_Transicion_Marcado:
				FondoHasta = DBotonEx_Skin::FondoMarcado;
				BordeHasta = DBotonEx_Skin::BordeNormal;
				TextoHasta = DBotonEx_Skin::TextoNormal;
				break;
			case DBotonEx_Transicion_Desactivado:
				FondoHasta = DBotonEx_Skin::FondoNormal;
				BordeHasta = DBotonEx_Skin::BordeNormal;
				TextoHasta = DBotonEx_Skin::TextoDesactivado;
				break;
		}
		_AniTransicion.Iniciar(_ColorFondo, FondoHasta, _ColorBorde, BordeHasta, _ColorTexto, TextoHasta, Duracion, [=](DAnimacion::Valores &Datos, const BOOL Terminado) {
			_ColorFondo = Datos[0].Color();
			_ColorBorde = Datos[1].Color();
			_ColorTexto = Datos[2].Color();
			Repintar();
		});
	}

	void DBotonEx::_Evento_MousePresionado(const WPARAM wParam, const LPARAM lParam, const int Boton) {
		#if DBOTONEX_MOSTRARDEBUG == TRUE
			Debug_Escribir(L"DBotonEx::_Evento_MousePresionado\n");
		#endif	
		DEventoMouse DatosMouse(wParam, lParam, this, Boton);
		SetCapture(hWnd());
		Transicion(DBotonEx_Transicion_Presionado);
		_Estado = DBotonEx_Estado_Presionado;
		SendMessage(GetParent(hWnd()), DWL_BOTONEX_MOUSEDOWN, DEVENTOMOUSE_TO_WPARAM(DatosMouse), 0);
		Evento_MousePresionado(DatosMouse);
	}

	void DBotonEx::_Evento_MouseSoltado(const WPARAM wParam, const LPARAM lParam, const int Boton) {		
		#if DBOTONEX_MOSTRARDEBUG == TRUE
			Debug_Escribir(L"DBotonEx::_Evento_MouseSoltado\n");
		#endif	
		ReleaseCapture();

		if (_Estado == DBotonEx_Estado_Presionado) {
			DEventoMouse DatosMouse(wParam, lParam, this, Boton);
			

			RECT RC;
			GetClientRect(hWnd(), &RC);

			Evento_MouseSoltado(DatosMouse);
			SendMessage(GetParent(hWnd()), DWL_BOTONEX_MOUSEUP, DEVENTOMOUSE_TO_WPARAM(DatosMouse), 0);

			POINT Pt = { DatosMouse.X(), DatosMouse.Y() };
			if (PtInRect(&RC, Pt) != 0) {
				_Estado = DBotonEx_Estado_Resaltado;
				Transicion(DBotonEx_Transicion_Resaltado);
				Evento_MouseClick(DatosMouse);
				SendMessage(GetParent(hWnd()), DWL_BOTONEX_CLICK, DEVENTOMOUSE_TO_WPARAM(DatosMouse), 0);
			}
			else {
				_Estado = DBotonEx_Estado_Normal;
//				Transicion(DBotonEx_Transicion_Normal); (no hace falta salta el mouse leave)
			}
//			Repintar();
		}
	}

	void DBotonEx::Marcado(const BOOL nMarcar) {
		_Marcado = nMarcar;
		POINT P;
		DWL::DMouse::ObtenerPosicion(&P);
		ScreenToClient(_hWnd, &P);
		RECT RC;
		GetClientRect(_hWnd, &RC);
		// Miro si el mouse está encima del control, y asigno el color rojo resaltado
		if (PtInRect(&RC, P) == TRUE) {
			_ColorFondo = DBotonEx_Skin::FondoResaltado;
//			Transicion(DBotonEx_Transicion_Resaltado); // no se necesita una transición porque el mouse está encima y sigue siendo resaltado
			_Estado = DBotonEx_Estado_Resaltado;
		}
		else { // Si no está encima del control asigno el color según si está marcado o no
			_Estado = DBotonEx_Estado_Normal;
			Transicion((nMarcar == TRUE) ? DBotonEx_Transicion_Marcado : DBotonEx_Transicion_Normal);
			
		}	
	}

	void DBotonEx::_Evento_Pintar(void) {
		HDC         DC;
		PAINTSTRUCT PS;
		DC = BeginPaint(hWnd(), &PS);
		#if DBOTONEX_GDIPLUS == FALSE
			Pintar(DC);
		#else
			PintarP(DC);
		#endif
		EndPaint(hWnd(), &PS);
	}

	void DBotonEx::_Evento_MouseSaliendo(void) {
		#if DBOTONEX_MOSTRARDEBUG == TRUE
			Debug_Escribir(L"DBotonEx::_Evento_MouseSaliendo\n");
		#endif	
		_MouseDentro = FALSE;
		if (_Estado != DBotonEx_Estado_Presionado && Activado() == TRUE) {
			Transicion((_Marcado == FALSE) ? DBotonEx_Transicion_Normal : DBotonEx_Transicion_Marcado);
			_Estado = DBotonEx_Estado_Normal;
		}
	}

	LRESULT CALLBACK DBotonEx::GestorMensajes(UINT uMsg, WPARAM wParam, LPARAM lParam) {
		switch (uMsg) {
			case WM_PAINT		:		_Evento_Pintar();									return 0;
			case WM_MOUSEMOVE	:		_Evento_MouseMovimiento(wParam, lParam);			return 0;
			case WM_MOUSELEAVE	:		_Evento_MouseSaliendo();							return 0;
			case WM_LBUTTONDOWN	:		_Evento_MousePresionado(wParam, lParam, 0);			return 0;
			case WM_RBUTTONDOWN	:		_Evento_MousePresionado(wParam, lParam, 1);			return 0;
			case WM_MBUTTONDOWN	:		_Evento_MousePresionado(wParam, lParam, 2);			return 0;
			case WM_LBUTTONUP	:		_Evento_MouseSoltado(wParam, lParam, 0);			return 0;
			case WM_RBUTTONUP	:		_Evento_MouseSoltado(wParam, lParam, 1);			return 0;
			case WM_MBUTTONUP	:		_Evento_MouseSoltado(wParam, lParam, 2);			return 0;
		}
		return DControlEx::GestorMensajes(uMsg, wParam, lParam);
	}
}