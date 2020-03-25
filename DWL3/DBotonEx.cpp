#include "pch.h"
#include "DBotonEx.h"
//#include "Rave_Skin.h"
#include "DMensajesWnd.h"
#include "DMouse.h"

namespace DWL {

	DBotonEx_Skin::DBotonEx_Skin(void) :
		// Valores visuales por defecto del BotonEx
		// Colores del fondo
		FondoNormal			(COLOR_BOTON),
		FondoResaltado		(COLOR_BOTON_RESALTADO),
		FondoPresionado		(COLOR_BOTON_PRESIONADO),
		FondoMarcado		(COLOR_BOTON_MARCADO),
		// Colores del borde
		BordeNormal			(COLOR_BOTON_BORDE),
		BordeResaltado		(COLOR_BOTON_BORDE_RESALTADO),
		BordePresionado		(COLOR_BOTON_BORDE_PRESIONADO),
		// Colores del texto
		TextoNormal			(COLOR_BOTON_TEXTO),
		TextoResaltado		(COLOR_BOTON_TEXTO_RESALTADO),
		TextoPresionado		(COLOR_BOTON_TEXTO_PRESIONADO),
		TextoDesactivado	(COLOR_BOTON_TEXTO_DESACTIVADO),
		TextoSombra			(COLOR_BOTON_TEXTO_SOMBRA),
		// Tipo de fuente
		FuenteTam			(FUENTE_GRANDE),
		FuenteNombre		(FUENTE_NOMBRE),
		FuenteNegrita		(TRUE),
		FuenteCursiva		(FALSE),
		FuenteSubrayado		(FALSE),
		// Sombra para el texto
		FuenteSombraTexto   (TRUE)	{
	}






	DBotonEx::DBotonEx(void) : 
		DControlEx()																	, 
		_Marcado(FALSE)																	, 
		_PosIconoX(-1)																	,
		_PosIconoY(-1)																	,
		_ColorFondo(Skin.FondoNormal)													,
		_ColorBorde(Skin.BordeNormal)													,
		_ColorTexto(Skin.TextoNormal)													,
		_Estado(DBotonEx_Estado::DBotonEx_Estado_Normal)								,
		// Eventos Lambda enlazados a los eventos virtuales por defecto
		EventoMouseMovimiento([=](DEventoMouse &e)		{ Evento_MouseMovimiento(e);	}),
		EventoMousePresionado([=](DEventoMouse& e)		{ Evento_MousePresionado(e);	}),
		EventoMouseSoltado([=](DEventoMouse& e)			{ Evento_MouseSoltado(e);		}),
		EventoMouseClick([=](DEventoMouse& e)			{ Evento_MouseClick(e);			})	{
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
		_ColorFondo = Skin.FondoNormal;
		_ColorBorde = Skin.BordeNormal;
		_ColorTexto = Skin.TextoNormal;
		_Marcado = FALSE;
//		if (hWnd()) { Debug_Escribir(L"DBotonEx::CrearBotonEx() Error : ya se ha creado el botón\n"); return hWnd(); }
		_hWnd = CrearControlEx(nPadre, L"DBotonEx", L"", cID, cX, cY, cAncho, cAlto, Estilos, NULL);
		Fuente.CrearFuente(Skin.FuenteTam, Skin.FuenteNombre.c_str(), Skin.FuenteNegrita, Skin.FuenteCursiva, Skin.FuenteSubrayado);
		_Estado = DBotonEx_Estado::DBotonEx_Estado_Normal;
		_MouseDentro = FALSE;
		return hWnd();
	}


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
		if (_Estado == DBotonEx_Estado::DBotonEx_Estado_Presionado) {
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
			if (Skin.FuenteSombraTexto == TRUE) {
				// Pinto la sombra del texto
				SetTextColor(Buffer, Skin.TextoSombra);
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

	void DBotonEx::Activado(const BOOL nActivar) {
		BOOL Ret = FALSE;
		Ret = EnableWindow(_hWnd, nActivar);
		Transicion((nActivar == TRUE) ? DBotonEx_Transicion::DBotonEx_Transicion_Normal : DBotonEx_Transicion::DBotonEx_Transicion_Desactivado);
		//Repintar();
	}

	

	void DBotonEx::_Evento_MouseMovimiento(const WPARAM wParam, const LPARAM lParam) {
		DEventoMouse DatosMouse(wParam, lParam, this);
		if (_MouseEntrando() == TRUE) {
			// Mouse enter
			if (_Estado != DBotonEx_Estado::DBotonEx_Estado_Presionado) {
				_Estado = DBotonEx_Estado::DBotonEx_Estado_Resaltado;
				Transicion(DBotonEx_Transicion::DBotonEx_Transicion_Resaltado);
			}
		}

		EventoMouseMovimiento(DatosMouse);
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

		COLORREF *FondoHasta = 0, *BordeHasta = 0, *TextoHasta = 0;
		switch (nTransicion) {
			case DBotonEx_Transicion::DBotonEx_Transicion_Normal:
				FondoHasta = &Skin.FondoNormal;
				BordeHasta = &Skin.BordeNormal;
				TextoHasta = &Skin.TextoNormal;
				break;
			case DBotonEx_Transicion::DBotonEx_Transicion_Resaltado:
				FondoHasta = &Skin.FondoResaltado;
				BordeHasta = &Skin.BordeResaltado;
				TextoHasta = &Skin.TextoResaltado;
				break;
			case DBotonEx_Transicion::DBotonEx_Transicion_Presionado:
				FondoHasta = &Skin.FondoPresionado;
				BordeHasta = &Skin.BordePresionado;
				TextoHasta = &Skin.TextoPresionado;
				break;
			case DBotonEx_Transicion::DBotonEx_Transicion_Marcado:
				FondoHasta = &Skin.FondoMarcado;
				BordeHasta = &Skin.BordeNormal;
				TextoHasta = &Skin.TextoNormal;
				break;
			case DBotonEx_Transicion::DBotonEx_Transicion_Desactivado:
				FondoHasta = &Skin.FondoNormal;
				BordeHasta = &Skin.BordeNormal;
				TextoHasta = &Skin.TextoDesactivado;
				break;
		}

		_AniTransicion.Iniciar(
			{ _ColorFondo, _ColorBorde, _ColorTexto }, 
			{ FondoHasta, BordeHasta, TextoHasta }, 
			Duracion, 
			[=](DAnimacion::Valores& Datos, const BOOL Terminado) {
				_ColorFondo = Datos[0].Color();
				_ColorBorde = Datos[1].Color();
				_ColorTexto = Datos[2].Color();
				Repintar();
			}
		);

/*		_AniTransicion.Iniciar(_ColorFondo, FondoHasta, _ColorBorde, BordeHasta, _ColorTexto, TextoHasta, Duracion, [=](DAnimacion2::Valores &Datos, const BOOL Terminado) {
			_ColorFondo = Datos[0].Color();
			_ColorBorde = Datos[1].Color();
			_ColorTexto = Datos[2].Color();
			Repintar();
		});*/
	}

	void DBotonEx::_Evento_MousePresionado(const WPARAM wParam, const LPARAM lParam, const int Boton) {
		#if DBOTONEX_MOSTRARDEBUG == TRUE
			Debug_Escribir(L"DBotonEx::_Evento_MousePresionado\n");
		#endif	
		DEventoMouse DatosMouse(wParam, lParam, this, Boton);
		SetCapture(hWnd());
		Transicion(DBotonEx_Transicion::DBotonEx_Transicion_Presionado);
		_Estado = DBotonEx_Estado::DBotonEx_Estado_Presionado;
		SendMessage(GetParent(hWnd()), DWL_BOTONEX_MOUSEDOWN, DEVENTOMOUSE_TO_WPARAM(DatosMouse), 0);
		EventoMousePresionado(DatosMouse);
	}

	void DBotonEx::_Evento_MouseSoltado(const WPARAM wParam, const LPARAM lParam, const int Boton) {		
		#if DBOTONEX_MOSTRARDEBUG == TRUE
			Debug_Escribir(L"DBotonEx::_Evento_MouseSoltado\n");
		#endif	
		ReleaseCapture();

		if (_Estado == DBotonEx_Estado::DBotonEx_Estado_Presionado) {
			DEventoMouse DatosMouse(wParam, lParam, this, Boton);
			

			RECT RC;
			GetClientRect(hWnd(), &RC);

			EventoMouseSoltado(DatosMouse);
			SendMessage(GetParent(hWnd()), DWL_BOTONEX_MOUSEUP, DEVENTOMOUSE_TO_WPARAM(DatosMouse), 0);

			POINT Pt = { DatosMouse.X(), DatosMouse.Y() };
			// Si el mouse está dentro del control
			if (PtInRect(&RC, Pt) != 0) {
				_Estado = DBotonEx_Estado::DBotonEx_Estado_Resaltado;
				Transicion(DBotonEx_Transicion::DBotonEx_Transicion_Resaltado);
				EventoMouseClick(DatosMouse);
				SendMessage(GetParent(hWnd()), DWL_BOTONEX_CLICK, DEVENTOMOUSE_TO_WPARAM(DatosMouse), 0);
			}
			else {
				_Estado = DBotonEx_Estado::DBotonEx_Estado_Normal;
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
			_ColorFondo = Skin.FondoResaltado;
//			Transicion(DBotonEx_Transicion_Resaltado); // no se necesita una transición porque el mouse está encima y sigue siendo resaltado
			_Estado = DBotonEx_Estado::DBotonEx_Estado_Resaltado;
		}
		else { // Si no está encima del control asigno el color según si está marcado o no
			_Estado = DBotonEx_Estado::DBotonEx_Estado_Normal;
			Transicion((nMarcar == TRUE) ? DBotonEx_Transicion::DBotonEx_Transicion_Marcado : DBotonEx_Transicion::DBotonEx_Transicion_Normal);
			
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
		if (_Estado != DBotonEx_Estado::DBotonEx_Estado_Presionado && Activado() == TRUE) {
			Transicion((_Marcado == FALSE) ? DBotonEx_Transicion::DBotonEx_Transicion_Normal : DBotonEx_Transicion::DBotonEx_Transicion_Marcado);
			_Estado = DBotonEx_Estado::DBotonEx_Estado_Normal;
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