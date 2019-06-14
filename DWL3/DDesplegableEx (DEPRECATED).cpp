#include "pch.h"
#include "DDesplegableEx (DEPRECATED).h"
#include "DGDI.h"

namespace DWL {
	/*
	DDesplegableEx::DDesplegableEx() :	_TipoEdicion(DDesplegableEx_TipoEdicion_SinEdicion), _TipoDesplegable(DDesplegableEx_TipoDesplegable_Lista),
										_EstadoBoton(DDesplegableEx_Estado_Normal), _EstadoVisor(DDesplegableEx_Estado_Normal),
										_UEstadoBoton(DDesplegableEx_Estado_Indefinido), _UEstadoVisor(DDesplegableEx_Estado_Indefinido),
										_ExplorarDirectorios(NULL), _Arbol (NULL), _Lista(NULL), _Icono(NULL) {
	}


	DDesplegableEx::~DDesplegableEx() {
	}


	HWND DDesplegableEx::CrearDesplegable(DhWnd *nPadre, const int cX, const int cY, const int cAncho, const int cAlto, const int cID, const DDesplegableEx_TipoEdicion nTipoEdicion, const DDesplegableEx_TipoDesplegable nTipoDesplegable) {
//		if (hWnd()) { Debug_Escribir(L"DDesplegableEx::CrearDesplegable() Error : ya se ha creado el control desplegable\n"); return hWnd(); }
		_hWnd = CrearControlEx(nPadre, L"DDesplegableEx", L"", cID, cX, cY, cAncho, cAlto, WS_CHILD | WS_VISIBLE, NULL, NULL);  // CS_DBLCLKS (el control recibe notificaciones de doble click)
		_TipoEdicion		= nTipoEdicion;
		_TipoDesplegable	= nTipoDesplegable;
		Fuente				= Fuente18Normal;

		// Creo el tipo de desplegable especificado
		switch (_TipoDesplegable) {
			case DDesplegableEx_TipoDesplegable_Lista :
				break;
			case DDesplegableEx_TipoDesplegable_Arbol :
				break;
			case DDesplegableEx_TipoDesplegable_ExplorarDirectorios :
				_ExplorarDirectorios = new DDesplegableEx_ExplorarDirectoriosEx(this);
				break;
		}

		// Creo un editbox o un LabelEx según el tipo de edición
		RECT REdit = { 20, 1, cAncho - cAlto, cAlto - 1 };
		IniciarEdicionTextoEx(REdit);

		switch (_TipoEdicion) {
			case DDesplegableEx_TipoEdicion_SinEdicion :
				break;
			case DDesplegableEx_TipoEdicion_Texto :
				break;
			case DDesplegableEx_TipoEdicion_NumericoEntero :
				break;
			case DDesplegableEx_TipoEdicion_NumericoDecimal :
				break;
		}

		return hWnd();

	}


	void DDesplegableEx::MostrarDesplegable(void) {
		RECT RC;
		GetClientRect(hWnd(), &RC);


		switch (_TipoDesplegable) {
			case DDesplegableEx_TipoDesplegable_Lista:
				break;
			case DDesplegableEx_TipoDesplegable_Arbol:
				break;
			case DDesplegableEx_TipoDesplegable_ExplorarDirectorios:		
	//			if (_ExplorarDirectorios->Visible() == FALSE)	
					_ExplorarDirectorios->Mostrar();
				break;
		}

		// Envio el mensaje WM_NCACTIVATE a la ventana principal para que no se vea como pierde el foco, y parezca que el desplegable es un hijo de la ventana principal
		SendMessage(GetAncestor(_hWnd, GA_ROOT), WM_NCACTIVATE, TRUE, 0);

	}


	void DDesplegableEx::Pintar(HDC DC) {
		RECT RC;
		GetClientRect(hWnd(), &RC);

		// Creo un buffer en memória para pintar el control
		HDC		Buffer = CreateCompatibleDC(NULL);
		// Creo un DC compatible para el buffer
		HBITMAP Bmp = CreateCompatibleBitmap(DC, RC.right, RC.bottom);
		HBITMAP BmpViejo = static_cast<HBITMAP>(SelectObject(Buffer, Bmp));

		RECT RBoton = { RC.right - RC.bottom, 0, RC.right, RC.bottom };
		RECT RVisor = { 0, 0, (RC.right - RC.bottom), RC.bottom }; // Recta que contiene la parte del icono y del texto (sin el boton del desplegable)

		COLORREF nColorBotonBorde = 0, nColorBoton = 0,	nColorVisor = 0, nColorVisorBorde = 0;

		switch (_EstadoBoton) {
			case DDesplegableEx_Estado_Normal:
				nColorBotonBorde = COLOR_BORDE;
				nColorBoton = COLOR_BOTON;
				break;
			case DDesplegableEx_Estado_Resaltado:
				nColorBotonBorde = COLOR_BORDE_RESALTADO;
				nColorBoton = COLOR_BOTON_RESALTADO;
				break;
			case DDesplegableEx_Estado_Presionado:
				nColorBotonBorde = COLOR_BORDE_PRESIONADO;
				nColorBoton = COLOR_BOTON_PRESIONADO;
				break;
		}

		switch (_EstadoVisor) {
			case DDesplegableEx_Estado_Normal:
				nColorVisorBorde = COLOR_BORDE;
				nColorVisor = COLOR_FONDO_CLARO;
				break;
			case DDesplegableEx_Estado_Resaltado:
				nColorVisorBorde = COLOR_BORDE_RESALTADO;
				nColorVisor = COLOR_FONDO_CLARO_RESALTADO;
				break;
			case DDesplegableEx_Estado_Presionado:
				nColorVisorBorde = COLOR_BORDE_PRESIONADO;
				nColorVisor = COLOR_FONDO_CLARO_PRESIONADO;
				break;
		}


		// Pinto el borde del visor (el visor es el editbox o texto el estatico)
		HBRUSH BrochaVisorBorde = CreateSolidBrush(nColorVisorBorde);
		FrameRect(Buffer, &RVisor, BrochaVisorBorde);
		DeleteObject(BrochaVisorBorde);

		RVisor.left++; RVisor.top++; RVisor.right--; RVisor.bottom--;
		// Pinto el fondo del visor
		HBRUSH BrochaVisor = CreateSolidBrush(nColorVisor);
		FillRect(Buffer, &RVisor, BrochaVisor);
		DeleteObject(BrochaVisor);


		// Pinto el borde del botón
		HBRUSH BrochaBotonBorde = CreateSolidBrush(nColorBotonBorde);
		FrameRect(Buffer, &RBoton, BrochaBotonBorde);
		DeleteObject(BrochaBotonBorde);

		RBoton.left++; RBoton.top++; RBoton.right--; RBoton.bottom--;
		// Pinto el fondo del botón
		HBRUSH BrochaBoton = CreateSolidBrush(nColorBoton);
		FillRect(Buffer, &RBoton, BrochaBoton);
		DeleteObject(BrochaBoton);

		DGDI::PintarFlecha(Buffer, (RC.right - RC.bottom) + 6, 4, 90.0f, 8, 1, RGB(255, 255, 255));

		// Pinto el texto
		EdicionTexto_Pintar(Buffer);

		// Pinto el icono (si hay icono)
		if (_Icono != NULL) {
			DrawIconEx(Buffer, 4, 7, _Icono->Icono(), DARBOLEX_TAMICONO, DARBOLEX_TAMICONO, 0, 0, DI_NORMAL);
		}


		// Copio el buffer al DC
		BitBlt(DC, RC.left, RC.top, RC.right, RC.bottom, Buffer, 0, 0, SRCCOPY);

		// Elimino el buffer de la memória
		SelectObject(Buffer, BmpViejo);
		DeleteObject(Bmp);
		DeleteDC(Buffer);
	}


	void DDesplegableEx::_Evento_Pintar(void) {
		HDC         DC;
		PAINTSTRUCT PS;
		DC = BeginPaint(hWnd(), &PS);
		Pintar(DC);
		EndPaint(hWnd(), &PS);
	}

	void DDesplegableEx::_Evento_TeclaPresionada(WPARAM wParam, LPARAM lParam) {
		BOOL nRepintar = FALSE;
		DEventoTeclado DatosTeclado(wParam, lParam, this);
		nRepintar = EdicionTexto_Evento_TeclaPresionada(DatosTeclado);
		Evento_TeclaPresionada(DatosTeclado);
		if (nRepintar == TRUE) Repintar();
	}

	void DDesplegableEx::_Evento_TeclaSoltada(WPARAM wParam, LPARAM lParam) {
		BOOL nRepintar = FALSE;
		DEventoTeclado DatosTeclado(wParam, lParam, this);
		nRepintar = EdicionTexto_Evento_TeclaSoltada(DatosTeclado);
		Evento_TeclaSoltada(DatosTeclado);
		if (nRepintar == TRUE) Repintar();
	}

	void DDesplegableEx::_Evento_Tecla(WPARAM wParam, LPARAM lParam) {
		BOOL nRepintar = FALSE;
		DEventoTeclado DatosTeclado(wParam, lParam, this);
		nRepintar = EdicionTexto_Evento_Tecla(DatosTeclado);
		Evento_Tecla(DatosTeclado);
		if (nRepintar == TRUE) Repintar();
	}

	void DDesplegableEx::_Evento_MouseMovimiento(WPARAM wParam, LPARAM lParam) {
		DEventoMouse DatosMouse(wParam, lParam, this);
		BOOL bME = _MouseEntrando();

		RECT RC; 
		GetClientRect(_hWnd, &RC);
		RECT RBoton = { RC.right - RC.bottom, 0, RC.right, RC.bottom };
		RECT RVisor = { 0, 0, RC.right - RC.bottom, RC.bottom };

		POINT Pt = { DatosMouse.X(), DatosMouse.Y() };
		// Está dentro del botón
		if (PtInRect(&RBoton, Pt) == TRUE) {
			if		(_Presionado == DDesplegableEx_Presionado_Nada)		_EstadoBoton = DDesplegableEx_Estado_Resaltado;
			else if (_Presionado == DDesplegableEx_Presionado_Boton)	_EstadoBoton = DDesplegableEx_Estado_Presionado;
			else if (_Presionado == DDesplegableEx_Presionado_Visor)	_EstadoBoton = DDesplegableEx_Estado_Resaltado;
		}
		else {
			_EstadoBoton = DDesplegableEx_Estado_Normal;
		}

		// Está dentro del visor
		if (PtInRect(&RVisor, Pt) == TRUE) {
			if		(_Presionado == DDesplegableEx_Presionado_Nada)		_EstadoVisor = DDesplegableEx_Estado_Resaltado;
			else if (_Presionado == DDesplegableEx_Presionado_Boton)	_EstadoVisor = DDesplegableEx_Estado_Resaltado;
			else if (_Presionado == DDesplegableEx_Presionado_Visor)	_EstadoVisor = DDesplegableEx_Estado_Presionado;
		}
		else {
			_EstadoVisor = DDesplegableEx_Estado_Normal;
		}

		if (_UEstadoVisor != _EstadoVisor || _UEstadoBoton != _EstadoBoton) {
			_UEstadoVisor = _EstadoVisor;
			_UEstadoBoton = _EstadoBoton;
			Repintar();
		}

		Evento_MouseMovimiento(DatosMouse);

		if (bME == TRUE)	Evento_MouseEntrando();
	}

	void DDesplegableEx::_Evento_MousePresionado(const UINT Boton, WPARAM wParam, LPARAM lParam) {
		DEventoMouse DatosMouse(wParam, lParam, this, Boton);
		SetFocus(_hWnd);
		RECT RC;
		GetClientRect(_hWnd, &RC);
		RECT RBoton = { RC.right - RC.bottom, 0, RC.right, RC.bottom };
		RECT RVisor = { 0, 0, RC.right - RC.bottom, RC.bottom };

		SetCapture(_hWnd);

		POINT Pt = { DatosMouse.X(), DatosMouse.Y() };
		// Está dentro del botón
		if (PtInRect(&RBoton, Pt) == TRUE) {
			_EstadoBoton = DDesplegableEx_Estado_Presionado;
			_Presionado  = DDesplegableEx_Presionado_Boton;
		}
		// Está dentro del visor
		if (PtInRect(&RVisor, Pt) == TRUE) {
			_EstadoVisor = DDesplegableEx_Estado_Presionado;
			_Presionado  = DDesplegableEx_Presionado_Visor;
		}

		Evento_MousePresionado(DatosMouse);
		Repintar();
	}

	void DDesplegableEx::_Evento_MouseSoltado(const UINT Boton, WPARAM wParam, LPARAM lParam) {
		DEventoMouse DatosMouse(wParam, lParam, this, Boton);
		ReleaseCapture();

		RECT RC;
		GetClientRect(_hWnd, &RC);
		RECT RBoton = { RC.right - RC.bottom, 0, RC.right, RC.bottom };
		RECT RVisor = { 0, 0, RC.right - RC.bottom, RC.bottom };

		_EstadoBoton = DDesplegableEx_Estado_Normal;
		_EstadoVisor = DDesplegableEx_Estado_Normal;

		POINT Pt = { DatosMouse.X(), DatosMouse.Y() };
		// Está dentro del botón
		if (PtInRect(&RBoton, Pt) == TRUE) {
			_EstadoBoton = DDesplegableEx_Estado_Resaltado;
			MostrarDesplegable();
		}
		// Está dentro del visor
		if (PtInRect(&RVisor, Pt) == TRUE) {
			_EstadoVisor = DDesplegableEx_Estado_Resaltado;
		}

		_Presionado = DDesplegableEx_Presionado_Nada;
		Evento_MouseSoltado(DatosMouse);
		Repintar();
	}

	void DDesplegableEx::_Evento_MouseRueda(WPARAM wParam, LPARAM lParam) {
		DEventoMouseRueda DatosMouse(wParam, lParam, this);
		Evento_MouseRueda(DatosMouse);
	}

	void DDesplegableEx::_Evento_MouseSaliendo(void) {
		_MouseDentro = FALSE;
		if (_EstadoBoton != DDesplegableEx_Estado_Normal || _EstadoVisor != DDesplegableEx_Estado_Normal) {
			_EstadoBoton = DDesplegableEx_Estado_Normal;
			_EstadoVisor = DDesplegableEx_Estado_Normal;
			Repintar();
		}
	}


	LRESULT CALLBACK DDesplegableEx::GestorMensajes(UINT uMsg, WPARAM wParam, LPARAM lParam) {
		switch (uMsg) {
			case WM_SETFOCUS:		BorrarBufferTeclado();																														return 0;
			case WM_KILLFOCUS:		BorrarBufferTeclado();																														return 0;

			case WM_KEYDOWN:		_Evento_TeclaPresionada(wParam, lParam);																									break;
			case WM_KEYUP:			_Evento_TeclaSoltada(wParam, lParam);																										break;
			case WM_CHAR: 			_Evento_Tecla(wParam, lParam);																												break;
							
			case WM_SIZE:			Repintar();																																	return 0;

			case WM_PAINT:			_Evento_Pintar();																															return 0;

			case WM_MOUSEMOVE:		_Evento_MouseMovimiento(wParam, lParam);																									return 0;

			case WM_MOUSELEAVE:		_Evento_MouseSaliendo();																													return 0;

			case WM_MOUSEWHEEL:		_Evento_MouseRueda(wParam, lParam);																											return 0;

			case WM_LBUTTONDOWN:	_Evento_MousePresionado(0, wParam, lParam);																									return 0;
			case WM_RBUTTONDOWN:	_Evento_MousePresionado(1, wParam, lParam);																									return 0;
			case WM_MBUTTONDOWN:	_Evento_MousePresionado(2, wParam, lParam);																									return 0;

			case WM_LBUTTONUP:		_Evento_MouseSoltado(0, wParam, lParam);																									return 0;
			case WM_RBUTTONUP:		_Evento_MouseSoltado(1, wParam, lParam);																									return 0;
			case WM_MBUTTONUP:		_Evento_MouseSoltado(2, wParam, lParam);																									return 0;

		}
		return DControlEx::GestorMensajes(uMsg, wParam, lParam);
	}
	*/
}