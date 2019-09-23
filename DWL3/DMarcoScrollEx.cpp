#include "pch.h"
#include "DMarcoScrollEx.h"

namespace DWL {

	#define ID_PAGINA 33

	// Función para crear el marco
	HWND DMarcoScrollEx::CrearMarcoScrollEx(DhWnd *nPadre, const int cX, const int cY, const int cAncho, const int cAlto, const int cID, const long Estilos) {
		
		SkinScroll.FondoScrollNormal				= COLOR_SCROLL_OSCURO_FONDO;
		SkinScroll.FondoScrollResaltado				= COLOR_SCROLL_OSCURO_FONDO_RESALTADO;
		SkinScroll.FondoScrollPresionado			= COLOR_SCROLL_OSCURO_FONDO_PRESIONADO;
		SkinScroll.BarraScrollNormal				= COLOR_SCROLL_OSCURO_BARRA;
		SkinScroll.BarraScrollResaltado				= COLOR_SCROLL_OSCURO_BARRA_RESALTADO;
		SkinScroll.BarraScrollPresionado			= COLOR_SCROLL_OSCURO_BARRA_PRESIONADO;
		// Colores del orde del control (donde no hay nodos ni items)
//		SkinScroll.BordeNormal						= COLOR_SCROLL_OSCURO_BORDE;
//		SkinScroll.BordeResaltado					= COLOR_SCROLL_OSCURO_BORDE_RESALTADO;
		// Colores del fondo del control (donde no hay nodos ni items)
		SkinScroll.FondoNormal						= COLOR_SCROLL_OSCURO_CONTROL_FONDO;
		SkinScroll.FondoResaltado					= COLOR_SCROLL_OSCURO_CONTROL_FONDO_RESALTADO;
		ActualizarSkin();

		// Creo el control principal con el marco y las barras de scroll
		_hWnd = CrearControlEx(nPadre, L"DMarcoScrollEx", L"", cID, cX, cY, cAncho, cAlto, Estilos, NULL, CS_HREDRAW | CS_VREDRAW, _Aplicacion->_ColorFondoVentana);
		// Creo un control para que sea el marco sin las barras de scroll
		_Marco.CrearControlEx(this, L"DMarcoScrollEx_Marco", L"", ID_PAGINA, 0, 0, cAncho, cAlto, WS_CHILD | WS_VISIBLE, NULL, CS_HREDRAW | CS_VREDRAW, _Aplicacion->_ColorFondoVentana);
		// Creo un control dentro que se usara para mover todo el contenido
		// Todos los controles creados dentro del MarcoScroll realmente se crean dentro del control _Pagina (Se redirige automáticamente desde DControlEx::CrearControlEx)
		_Pagina.CrearControlEx(&_Marco, L"DMarcoScrollEx_Pagina", L"", ID_PAGINA, 0, 0, cAncho, cAlto, WS_CHILD | WS_VISIBLE, NULL, CS_HREDRAW | CS_VREDRAW, _Aplicacion->_ColorFondoVentana);
		// Devuelvo el hWnd de este control
		return _hWnd;
	}

	// Función que destruye el marco, todos los controles, y libera la memória
	const BOOL DMarcoScrollEx::Destruir(void) {
		_Pagina.Destruir();
		_Marco.Destruir();
		return DBarraScrollEx::Destruir();
	}


	// Evento que se recibe al crear un control dentro del marco
	void DMarcoScrollEx::Evento_ControlCreado(DhWnd *nControl, const int cX, const int cY, const int cAncho, const int cAlto) {
		// Si no es la página
		if (nControl->TipoWnd() != DhWnd_Tipo_MarcoScrollEx_Pagina) {
			//CalcularPagina();
		}
	}

	// Función que calcula el tamaño que debe tener la pagina según su contenido
	void DMarcoScrollEx::CalcularPagina(void) {
		RECT RC;
		GetClientRect(_hWnd, &RC);
		// Establezco el ancho y alturas mínimos
		_AnchoPagina = 0;
		_AltoPagina  = 0;
		// Enumero los controles
		EnumChildWindows(_Pagina.hWnd(), _EnumChildProc, reinterpret_cast<LPARAM>(this));

		BOOL SV = FALSE, SH = FALSE;
		// Determino si se necesita scroll horizontal
		if (RC.right >= static_cast<LONG>(_AnchoPagina))	{ SH = FALSE; }
		else												{ SH = TRUE;	RC.bottom -= _ScrollH_Alto; }
		// Determino si se necesita scroll vertical
		if (RC.bottom >= static_cast<LONG>(_AltoPagina))	{ SV = FALSE; }
		else												{ SV = TRUE;	RC.right -= _ScrollV_Ancho; }


		// Calculo el scroll para el ancho
		if (SH == FALSE) {
			_ScrollH_Pagina = 1.0f;
		}
		else {
			// Añado 10 pixeles de margen a la derecha
			_AnchoPagina += 10;
			// Calculo la proporcón horizontal de la página, de 0 a 1
			_ScrollH_Pagina = (1.0f / _AnchoPagina) * static_cast<float>(RC.right);
			// Si la suma de la posición del scroll y la pagina son más grandes que 100% 
			if (_ScrollH_Posicion > 1.0f) {
				_ScrollH_Posicion = 1.0f; // Ajusto la posición del scroll para que sumada con la página sea el 100%
			}
		}
		ScrollH_Visible(SH);

		// Calculo el scroll para la altura
		if (SV == FALSE) {
			_ScrollV_Pagina = 1.0f;
		}
		else {
			// Añado 10 pixeles de margen abajo
			_AltoPagina += 10;
			// Calculo la proporcón vertical de la página, de 0 a 1
			_ScrollV_Pagina = (1.0f / _AltoPagina) * static_cast<float>(RC.bottom);
			// Si la suma de la posición del scroll y la pagina son más grandes que 100% 
			if (_ScrollV_Posicion > 1.0f) {
				_ScrollV_Posicion = 1.0f; // Ajusto la posición del scroll para que sumada con la página sea el 100%
			}
		}
		ScrollV_Visible(SV);


		RECT RCS, RCB;
		ObtenerRectaCliente(&RC, &RCS, &RCB);

		#if DMARCOSCROLLEX_MOSTRARDEBUG == TRUE
			Debug_Escribir_Varg(L"DMarcoScrollEx::CalcularPagina Ancho:%d Alto:%d ScrollV:%d ScrollH:%d\n", RCS.right - RCS.left, RCS.bottom - RCS.top, SV, SH);
		#endif


		// Redimensiono el marco
		MoveWindow(_Marco.hWnd(), RCS.left, RCS.top, RCS.right - RCS.left, RCS.bottom - RCS.top, TRUE);
		// Redimensiono la pagina
		MoveWindow(_Pagina.hWnd(), 0, 0, _AnchoPagina, _AltoPagina, TRUE);
	}


	// Función que enumera las ventanas hijo
	BOOL CALLBACK DMarcoScrollEx::_EnumChildProc(_In_ HWND hwnd, _In_ LPARAM lParam) {
		DMarcoScrollEx *This = reinterpret_cast<DMarcoScrollEx *>(lParam);
		RECT RVM, RVC;
		// Obtengo la posición de la ventana
		GetWindowRect(This->hWnd(), &RVM);	// Recta del marco
		GetWindowRect(hwnd, &RVC);			// Recta del control
		// Resto el inicio del marco a la recta del control
		RVC.left   -= RVM.left;
		RVC.top    -= RVM.top;
		RVC.right  -= RVM.left;
		RVC.bottom -= RVM.top;
		// Compruebo si el ancho es mas grande que el que hay guardado
		if (This->_AnchoPagina < RVC.right)		This->_AnchoPagina = RVC.right;
		// Compruebo si el alto es mas grande que el que hay guardado
		if (This->_AltoPagina < RVC.bottom)		This->_AltoPagina = RVC.bottom;
		
		// Continuo la enumeración
		return TRUE;
	}


	void DMarcoScrollEx::Pintar(HDC hDC) {
		RECT RC, RCS, RCB;
		ObtenerRectaCliente(&RC, &RCS, &RCB);

		HDC		Buffer		= CreateCompatibleDC(hDC);
		HBITMAP Bmp			= CreateCompatibleBitmap(hDC, RCB.right - RCB.left, RCB.bottom - RCB.top);
		HBITMAP BmpViejo	= static_cast<HBITMAP>(SelectObject(Buffer, Bmp));

		// Pinto el fondo
/*		HBRUSH BFondo = CreateSolidBrush(_ColorFondo);
		FillRect(Buffer, &RCB, BFondo);
		DeleteObject(BFondo);*/

		// Pinto el borde del control
		PintarBorde(&RCB, Buffer);

		// Pinto las barras de scroll en el buffer
		Scrolls_Pintar(Buffer, RC);
		int TamH = 1, TamV = 1;

		if (_ScrollH_Estado != DBarraScrollEx_Estado_Invisible) TamH = _ScrollH_Alto + 1;
		if (_ScrollV_Estado != DBarraScrollEx_Estado_Invisible) TamV = _ScrollV_Ancho + 1;

		// Pinto el buffer en el DC
//		BitBlt(hDC, RCB.left, RCB.top, RCB.right, RCB.bottom, Buffer, 0, 0, SRCCOPY);
		// Pinto el borde superior del buffer en el DC
		BitBlt(hDC, RCB.left, RCB.top, RCB.right, 1, Buffer, 0, 0, SRCCOPY);
		// Pinto el borde inferior del buffer en el DC
		BitBlt(hDC, RCB.left, RCB.bottom - TamH, RCB.right, TamH, Buffer, 0, RCB.bottom - TamH, SRCCOPY);
		// Pinto el borde izquierdo del buffer en el DC
		BitBlt(hDC, RCB.left, RCB.top, 1, RCB.bottom, Buffer, 0, 0, SRCCOPY);
		// Pinto el borde derecho del buffer en el DC
		BitBlt(hDC, RCB.right - TamV, RCB.top, TamV, RCB.bottom, Buffer, RCB.right - TamV, 0, SRCCOPY);

		// Elimino objetos gdi de la memoria
		SelectObject(Buffer, BmpViejo);
		DeleteObject(Bmp);
		DeleteDC(Buffer);
	}


	void DMarcoScrollEx::Repintar(void) {
		HDC DC = GetDC(_hWnd);
		Pintar(DC);
		ReleaseDC(_hWnd, DC);
	}



	void DMarcoScrollEx::_Evento_Pintar(void) {
		PAINTSTRUCT PS;
		HDC DC = BeginPaint(hWnd(), &PS);
		Pintar(DC);
		EndPaint(hWnd(), &PS);
	}

	// Función interna para porcesar el movimiento del mouse
	void DMarcoScrollEx::_Evento_MouseMovimiento(WPARAM wParam, LPARAM lParam) {
		DEventoMouse DatosMouse(wParam, lParam, this);
		// Utilizo la función _MouseEntrando() para poder recibir los mensajes WM_MOUSELEAVE
		if (_MouseEntrando() == TRUE) {
			Evento_MouseEntrando();
			Scrolls_MouseEntrando();
		}

		if (Scrolls_MouseMovimiento(DatosMouse) == TRUE) { return; } // las coordenadas pertenecen al scroll (salgo del evento)
		Evento_MouseMovimiento(DatosMouse);
	}

	// Función interna para cuando sale el mouse del control
	void DMarcoScrollEx::_Evento_MouseSaliendo(void) {
		POINT Pt;
		GetCursorPos(&Pt);
		// Si la nueva ventana es una hija de esta, no ejecutamos el mouse leave
//		if (IsChild(WindowFromPoint(Pt), _hWnd) != 0) {
			BOOL nRepintar = Scrolls_MouseSaliendo();
			_MouseDentro = FALSE;
			Evento_MouseSaliendo();
			if (nRepintar == TRUE) Repintar();
//		}
	}

	void DMarcoScrollEx::_Evento_MousePresionado(const int Boton, WPARAM wParam, LPARAM lParam) {
		DEventoMouse DatosMouse(wParam, lParam, this, Boton);
		SetCapture(_hWnd);
		SetFocus(_hWnd);
		if (Scrolls_MousePresionado(DatosMouse) == TRUE) { return; }

		// Evento virtual
		Evento_MousePresionado(DatosMouse);

	}


	void DMarcoScrollEx::_Evento_MouseSoltado(const int Boton, WPARAM wParam, LPARAM lParam) {
		DEventoMouse DatosMouse(wParam, lParam, this, Boton);
		// Libero la captura del mouse
		ReleaseCapture();

		// Si el evento del moouse pertenece al scroll, salgo.
		if (Scrolls_MouseSoltado(DatosMouse) == TRUE) { return; }

		// Evento virtual
		Evento_MouseSoltado(DatosMouse);

	}

	void DMarcoScrollEx::_Evento_MouseRueda(WPARAM wParam, LPARAM lParam) {
		DEventoMouseRueda DatosMouse(wParam, lParam, this);

		RECT RC, RCS, RCB;
		ObtenerRectaCliente(&RC, &RCS, &RCB);

		if (DatosMouse.Delta() > 0) { // hacia arriba
			_ScrollV_Posicion -= _ScrollV_Pagina / 10.0f;
			if (_ScrollV_Posicion < 0.0f) _ScrollV_Posicion = 0.0f;
		}
		else { // hacia abajo
			_ScrollV_Posicion += _ScrollV_Pagina / 10.0f;
			if (_ScrollV_Posicion > 1.0f) _ScrollV_Posicion = 1.0f;
		}

		int PosX = static_cast<int>(static_cast<float>(_AnchoPagina - (RCS.right - RCS.left)) * _ScrollH_Posicion);
		int PosY = static_cast<int>(static_cast<float>(_AltoPagina - (RCS.bottom - RCS.top)) * _ScrollV_Posicion);
		SetWindowPos(_Pagina.hWnd(), NULL, -PosX, -PosY, 0, 0, SWP_NOSIZE);


		Evento_MouseRueda(DatosMouse);
		Repintar();
	}

	void DMarcoScrollEx::Scrolls_EventoCambioPosicion(void) {
		RECT RC, RCS, RCB;
		ObtenerRectaCliente(&RC, &RCS, &RCB);

		// Resto la página al área total, y lo multiplico por el valor de la posición
		int PosX = static_cast<int>(static_cast<float>(_AnchoPagina - (RCS.right - RCS.left)) * _ScrollH_Posicion);
		int PosY = static_cast<int>(static_cast<float>(_AltoPagina - (RCS.bottom - RCS.top)) * _ScrollV_Posicion);

		SetWindowPos(_Pagina.hWnd(), NULL, -PosX, -PosY, 0, 0, SWP_NOSIZE);
	}


	// Gestor de mensages para el marco
	LRESULT CALLBACK DMarcoScrollEx::GestorMensajes(UINT uMsg, WPARAM wParam, LPARAM lParam) {
		switch (uMsg) {
			// Pintado
			case WM_PAINT:			_Evento_Pintar();																															return 0;
			// Mouse movimiento
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

			// Mouse rueda
			case WM_MOUSEWHEEL:		_Evento_MouseRueda(wParam, lParam);																											return 0;

		}
		return DBarraScrollEx::GestorMensajes(uMsg, wParam, lParam);
	}















	// Gestor de mensages para la página
	LRESULT CALLBACK DMarcoScrollEx::DMarcoScrollEx_Pagina::GestorMensajes(UINT uMsg, WPARAM wParam, LPARAM lParam) {
		switch (uMsg) {
			// Lista de mensajes que se pasaran a DMarcoScrollEx
			case DWL_BARRAEX_CAMBIANDO :
			case DWL_BARRAEX_CAMBIADO :
			case DWL_BOTONEX_MOUSEDOWN :
			case DWL_BOTONEX_MOUSEUP :
			case DWL_BOTONEX_CLICK :
			case DWL_ARBOLEX_MOUSEPRESIONADO :
			case DWL_ARBOLEX_MOUSESOLTADO :
			case DWL_ARBOLEX_MOUSEMOVIMIENTO :
			case DWL_ARBOLEX_DOBLECLICK:
			case DWL_ARBOLEX_CLICK:
			case DWL_LISTAEX_MOUSEPRESIONADO :
			case DWL_LISTAEX_MOUSESOLTADO :
			case DWL_LISTAEX_MOUSEMOVIMIENTO :
			case DWL_LISTAEX_DOBLECLICK:
			case DWL_LISTAEX_CLICK:
			case DWL_MARCAEX_CLICK :
			case DWL_EDICIONTEXTOEX_CLICK :
			case DWL_EDICIONTEXTOEX_CAMBIO :
			case DWL_DESPLEGABLEEX_CAMBIO :
			case DWL_MENUEX_CLICK :
			case DWL_MENUEX_BARRA_CAMBIADO :
			case DWL_MENUEX_BARRA_CAMBIANDO :
				if (_Padre != nullptr) {
					if (static_cast<DMarcoScrollEx_Pagina*>(_Padre)->_Padre != nullptr) {
						return static_cast<DMarcoScrollEx*>(static_cast<DMarcoScrollEx_Pagina*>(_Padre)->_Padre)->GestorMensajes(uMsg, wParam, lParam);
					}
				}
				break;
		}
		return DControlEx::GestorMensajes(uMsg, wParam, lParam);
	}

	// Gestor de mensages para el marco sin scrolls
	LRESULT CALLBACK DMarcoScrollEx::DMarcoScrollEx_Marco::GestorMensajes(UINT uMsg, WPARAM wParam, LPARAM lParam) {
/*		switch (uMsg) {
			case DWL_BARRAEX_CAMBIANDO :
			case DWL_BARRAEX_CAMBIADO :
			case DWL_BOTONEX_MOUSEDOWN :
			case DWL_BOTONEX_MOUSEUP :
			case DWL_BOTONEX_CLICK :
			case DWL_ARBOLEX_MOUSEPRESIONADO :
			case DWL_ARBOLEX_MOUSESOLTADO :
			case DWL_ARBOLEX_MOUSEMOVIMIENTO :
			case DWL_ARBOLEX_DOBLECLICK :
			case DWL_LISTAEX_MOUSEPRESIONADO :
			case DWL_LISTAEX_MOUSESOLTADO :
			case DWL_LISTAEX_MOUSEMOVIMIENTO :
			case DWL_LISTAEX_DOBLECLICK :
			case DWL_MARCAEX_CLICK :
			case DWL_EDICIONTEXTOEX_CLICK :
			case DWL_EDICIONTEXTOEX_CAMBIO :
			case DWL_DESPLEGABLEEX_CAMBIO :
			case DWL_MENUEX_CLICK :
			case DWL_MENUEX_BARRA_CAMBIADO :
			case DWL_MENUEX_BARRA_CAMBIANDO :
				return static_cast<DMarcoScrollEx *>(static_cast<DMarcoScrollEx_Pagina *>(_Padre)->_Padre)->GestorMensajes(uMsg, wParam, lParam);
		}*/
		return DControlEx::GestorMensajes(uMsg, wParam, lParam);
	}

}