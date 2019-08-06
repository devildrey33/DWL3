#include "pch.h"
#include "DMarcaEx.h"
#include "DMensajesWnd.h"
#include "DIcono.h"

namespace DWL {

	DMarcaEx_Skin::DMarcaEx_Skin(void) :
		// Colores por defecto
		FondoNormal				(COLOR_MARCA_FONDO),
		FondoResaltado			(COLOR_MARCA_FONDO_RESALTADO),
		FondoPresionado			(COLOR_MARCA_FONDO_PRESIONADO),
		FondoMarcaNormal		(COLOR_MARCA_FONDO_MARCA),
		FondoMarcaResaltado		(COLOR_MARCA_FONDO_MARCA_RESALTADO),
		FondoMarcaPresionado	(COLOR_MARCA_FONDO_MARCA_PRESIONADO),
		FondoMarcaDesactivado	(COLOR_MARCA_FONDO_MARCA_DESACTIVADO),
		BordeNormal				(COLOR_MARCA_BORDE),
		BordeResaltado			(COLOR_MARCA_BORDE_RESALTADO),
		BordePresionado			(COLOR_MARCA_BORDE_PRESIONADO),
		TextoNormal				(COLOR_MARCA_TEXTO),
		TextoResaltado			(COLOR_MARCA_TEXTO_RESALTADO),
		TextoPresionado			(COLOR_MARCA_TEXTO_PRESIONADO),
		TextoDesactivado		(COLOR_MARCA_TEXTO_DESACTIVADO),
		TextoSombra				(COLOR_MARCA_TEXTO_SOMBRA),

		// Fuente
		FuenteTam				(FUENTE_NORMAL),
		FuenteNombre			(FUENTE_NOMBRE),
		FuenteNegrita			(FALSE),
		FuenteCursiva			(FALSE),
		FuenteSubrayado			(FALSE),
		FuenteSombraTexto		(TRUE) {
	}


	#define DMARCAEX_TAMICONO 16

	DMarcaEx::DMarcaEx(void): _Estado(DMarcaEx_Estado_Normal), _Marcado(FALSE), _ColorFondoMarca(Skin.FondoMarcaNormal), _ColorFondo(Skin.FondoNormal), _ColorTexto(Skin.TextoNormal), _ColorBorde(Skin.BordeNormal) {	}


	DMarcaEx::~DMarcaEx(void) {
	}

	// Alto mínimo 18
	HWND DMarcaEx::CrearMarcaEx(DhWnd *nPadre, const TCHAR *nTxt, const int cX, const int cY, const int cAncho, const int cAlto, const INT_PTR cID, const INT_PTR IDIconoMarca, const long Estilos) {
//		if (hWnd())		 { Debug_Escribir(L"DMarcaEx::CrearBotonEx() Error : ya se ha creado la marca\n");				return hWnd(); }
		if (cAlto <= 18) { Debug_Escribir(L"DMarcaEx::CrearBotonEx() Error : la altura es inferior a 18 pixeles\n");	return NULL; }
	
		_hWnd = CrearControlEx(nPadre, L"DMarcaEx", L"", cID, cX, cY, cAncho, cAlto, Estilos, NULL);
		_Texto = nTxt;
		_Icono.CrearIconoRecursos(IDIconoMarca, DMARCAEX_TAMICONO, DMARCAEX_TAMICONO);
		_MouseDentro	 = FALSE;
		_ColorFondo		 = Skin.FondoNormal;
		_ColorTexto		 = Skin.TextoNormal;
		_ColorFondoMarca = Skin.FondoMarcaNormal;
		_ColorBorde		 = Skin.BordeNormal;

		Fuente.CrearFuente(Skin.FuenteTam, Skin.FuenteNombre.c_str(), Skin.FuenteNegrita, Skin.FuenteCursiva, Skin.FuenteSubrayado);
		return hWnd();
	}


	void DMarcaEx::Pintar(HDC DC) {
		RECT RC;
		GetClientRect(_hWnd, &RC);
		// Creo un buffer en memória para pintar el control
		HDC		Buffer		= CreateCompatibleDC(NULL);
		// Creo un DC compatible para el buffer
		HBITMAP Bmp			= CreateCompatibleBitmap(DC, RC.right, RC.bottom);
		HBITMAP BmpViejo	= static_cast<HBITMAP>(SelectObject(Buffer, Bmp));
		HFONT   vFuente		= static_cast<HFONT>(SelectObject(Buffer, Fuente()));

		// Pinto el fondo
		HBRUSH ColorFondo = CreateSolidBrush(_ColorFondo);
		FillRect(Buffer, &RC, ColorFondo);
		DeleteObject(ColorFondo);

		// Si está presionado
		int bPresionado = 0;
		if (_Estado == DMarcaEx_Estado_Presionado) {
			bPresionado   = 1;
		}

		// Calculo la posición del marco
		int  YMarco = (RC.bottom - (DMARCAEX_TAMICONO + 2)) / 2;
		RECT Marco  = { 0, YMarco, DMARCAEX_TAMICONO + 2, (DMARCAEX_TAMICONO + 2) + YMarco };
		// Creo los objetos GDSI necesarios para pintar el marco
		HBRUSH	ColorFondoMarco = CreateSolidBrush(_ColorFondoMarca);
		HPEN	ColorBordeMarco = CreatePen(PS_SOLID, 1, _ColorBorde);
		HBRUSH  BrochaViejo		= static_cast<HBRUSH>(SelectObject(Buffer, ColorFondoMarco));
		HPEN    PlumaViejo		= static_cast<HPEN>(SelectObject(Buffer, ColorBordeMarco));
		// Pinto el marco para el check
		Rectangle(Buffer, Marco.left, Marco.top, Marco.right, Marco.bottom);
		// Des-selecciono ogjetos GDI y libero su memória
		SelectObject(Buffer, BrochaViejo);
		SelectObject(Buffer, PlumaViejo);
		DeleteObject(ColorFondoMarco);
		DeleteObject(ColorBordeMarco);

		// Pinto la marca si es necesario
		if (_Marcado == TRUE) {
			DrawIconEx(Buffer, 1, YMarco + 1, _Icono(), DMARCAEX_TAMICONO, DMARCAEX_TAMICONO, 0, 0, DI_NORMAL);			
		}

		// Fondo transparente para pintar texto
		SetBkMode(Buffer, TRANSPARENT);
		// Calculo la altura donde debe empezar el texto
		SIZE AltoTexto = Fuente.ObtenerTamTexto(Buffer, L"W");
		int YTexto = (RC.bottom - AltoTexto.cy) / 2;

		// Pinto la sombra del texto
		if (Skin.FuenteSombraTexto == TRUE) {
			SetTextColor(Buffer, Skin.TextoSombra);
			TextOut(Buffer, DMARCAEX_TAMICONO + 7, bPresionado + YTexto + 1, _Texto.c_str(), static_cast<int>(_Texto.size()));
		}
		// Pinto el texto
		SetTextColor(Buffer, _ColorTexto);
		TextOut(Buffer, DMARCAEX_TAMICONO + 6, bPresionado + YTexto, _Texto.c_str(), static_cast<int>(_Texto.size()));

		// Copio el buffer al DC
		BitBlt(DC, RC.left, RC.top, RC.right, RC.bottom, Buffer, 0, 0, SRCCOPY);

		// Elimino el buffer de la memória
		SelectObject(Buffer, vFuente);
		SelectObject(Buffer, BmpViejo);
		DeleteObject(Bmp);
		DeleteDC(Buffer);
	}

	void DMarcaEx::Transicion(const DMarcaEx_Transicion nTransicion) {
		DWORD Duracion = DhWnd::TiempoAnimaciones;
		if (_AniTransicion.Animando() == TRUE) {
			Duracion = _AniTransicion.TiempoActual();
			_AniTransicion.Terminar();
		}

		COLORREF *FondoHasta = 0, *BordeHasta = 0, *TextoHasta = 0, *FondoMarcaHasta = 0;
		switch (nTransicion) {
			case DMarcaEx_Transicion_Normal:
				FondoHasta		= &Skin.FondoNormal;
				FondoMarcaHasta = &Skin.FondoMarcaNormal;
				BordeHasta		= &Skin.BordeNormal;
				TextoHasta		= &Skin.TextoNormal;
				break;
			case DMarcaEx_Transicion_Resaltado:
				FondoHasta		= &Skin.FondoResaltado;
				FondoMarcaHasta = &Skin.FondoMarcaResaltado;
				BordeHasta		= &Skin.BordeResaltado;
				TextoHasta		= &Skin.TextoResaltado;
				break;
			case DMarcaEx_Transicion_Presionado:
				FondoHasta		= &Skin.FondoPresionado;
				FondoMarcaHasta = &Skin.FondoMarcaPresionado;
				BordeHasta		= &Skin.BordePresionado;
				TextoHasta		= &Skin.TextoPresionado;
				break;
			case DMarcaEx_Transicion_Desactivado:
				FondoHasta		= &Skin.FondoNormal;
				FondoMarcaHasta = &Skin.FondoMarcaDesactivado;
				BordeHasta		= &Skin.BordeNormal;
				TextoHasta		= &Skin.TextoDesactivado;
				break;
		}

		_AniTransicion.Iniciar({ _ColorFondo, _ColorFondoMarca, _ColorBorde, _ColorTexto }, { FondoHasta, FondoMarcaHasta, BordeHasta, TextoHasta }, Duracion, [=](DAnimacion::Valores& Datos, const BOOL Terminado) {
			_ColorFondo		 = Datos[0].Color();
			_ColorFondoMarca = Datos[1].Color();
			_ColorBorde		 = Datos[2].Color();
			_ColorTexto		 = Datos[3].Color();
			Repintar();
		});
	}


	void DMarcaEx::_Evento_Pintar(void) {
		HDC         DC;
		PAINTSTRUCT PS;
		DC = BeginPaint(hWnd(), &PS);
		Pintar(DC);
		EndPaint(hWnd(), &PS);
	}


	void DMarcaEx::Marcado(const BOOL nMarcado) {
		_Marcado = nMarcado;
		Repintar();		
	}

	void DMarcaEx::_Evento_MouseMovimiento(WPARAM wParam, LPARAM lParam) {
//		BOOL nRepintar = FALSE;
		DEventoMouse DatosMouse(wParam, lParam, this, -1);
		if (_MouseEntrando() == TRUE) {
			// Mouse enter
			if (_Estado != DMarcaEx_Estado_Presionado) {
				_Estado = DMarcaEx_Estado_Resaltado;
				Transicion(DMarcaEx_Transicion_Resaltado);
//				Resaltar(TRUE);
			}
		}
		Evento_MouseMovimiento(DatosMouse);

//		if (nRepintar == TRUE)	Repintar();
	}

	void DMarcaEx::_Evento_MousePresionado(const int Boton, WPARAM wParam, LPARAM lParam) {
		DEventoMouse DatosMouse(wParam, lParam, this, Boton);
		SetCapture(hWnd());
		_Estado = DMarcaEx_Estado_Presionado;
		Evento_MousePresionado(DatosMouse);
		//Repintar();
		Transicion(DMarcaEx_Transicion_Presionado);
	}

	void DMarcaEx::_Evento_MouseSoltado(const int Boton, WPARAM wParam, LPARAM lParam) {
		DEventoMouse DatosMouse(wParam, lParam, this, Boton);
//		BOOL nRepintar = FALSE;
		ReleaseCapture();
		if (_Estado == DMarcaEx_Estado_Presionado) {

			RECT RC;
			GetClientRect(hWnd(), &RC);

			POINT Pt = { DatosMouse.X(), DatosMouse.Y() };
			if (PtInRect(&RC, Pt) != 0) {
				_Estado = DMarcaEx_Estado_Resaltado;
				Transicion(DMarcaEx_Transicion_Resaltado);
				_Marcado = !_Marcado;
				SendMessage(GetParent(hWnd()), DWL_MARCAEX_CLICK, reinterpret_cast<WPARAM>(&DatosMouse), 0);
			}
			else {
//				Transicion(DMarcaEx_Transicion_Normal);		No hace falta (salta el mouseleave)
				_Estado = DMarcaEx_Estado_Normal;
			}
//			nRepintar = TRUE;
		}
		Evento_MouseSoltado(DatosMouse);
//		if (nRepintar == TRUE)	Repintar();
	}

	void DMarcaEx::_Evento_MouseSaliendo(void) {
		_MouseDentro = FALSE;
//		BOOL nRepintar = FALSE;
		if (_Estado != DMarcaEx_Estado_Presionado) {
			_Estado = DMarcaEx_Estado_Normal;
//			nRepintar = TRUE;
//			Resaltar(FALSE);
			Transicion(DMarcaEx_Transicion_Normal);
		}
		Evento_MouseSaliendo();
//		if (nRepintar == TRUE)	Repintar();
	}

	void DMarcaEx::Activado(const BOOL nActivar) {
		BOOL Ret = FALSE;
		Ret = EnableWindow(_hWnd, nActivar);
		Transicion((nActivar == TRUE) ? DMarcaEx_Transicion_Normal : DMarcaEx_Transicion_Desactivado);
		//Repintar();
	}

	LRESULT CALLBACK DMarcaEx::GestorMensajes(UINT uMsg, WPARAM wParam, LPARAM lParam) {
		switch (uMsg) {
			case WM_PAINT:			_Evento_Pintar();																															return 0;
			case WM_MOUSEMOVE:		_Evento_MouseMovimiento(wParam, lParam);																									return 0;
			case WM_MOUSELEAVE:		_Evento_MouseSaliendo();																													return 0;
				// Mouse presionado
			case WM_LBUTTONDOWN:	_Evento_MousePresionado(0, wParam, lParam);																									return 0;
			case WM_RBUTTONDOWN:	_Evento_MousePresionado(1, wParam, lParam);																									return 0;
			case WM_MBUTTONDOWN:	_Evento_MousePresionado(2, wParam, lParam);																									return 0;
				// Mouse soltado
			case WM_LBUTTONUP:		_Evento_MouseSoltado(0, wParam, lParam);																									return 0;
			case WM_RBUTTONUP:		_Evento_MouseSoltado(1, wParam, lParam);																									return 0;
			case WM_MBUTTONUP:		_Evento_MouseSoltado(2, wParam, lParam);																									return 0;

		}
		return DControlEx::GestorMensajes(uMsg, wParam, lParam);
	}

};