#include "pch.h"
#include "DMarcoScrollEx.h"

namespace DWL {

	#define ID_PAGINA 33

	// Función para crear el marco
	HWND DMarcoScrollEx::CrearMarcoScrollEx(DhWnd *nPadre, const int cX, const int cY, const int cAncho, const int cAlto, const int cID, const long Estilos) {
		// Establezco el color del fondo, si es null se utilizará el del skin
//		HBRUSH ColFondo = (nColorFondo == nullptr) ? _Aplicacion->_ColorFondoVentana : nColorFondo;


		// Creo el control principal con el marco y las barras de scroll
		_hWnd = CrearControlEx(nPadre, L"DMarcoScrollEx", L"", cID, cX, cY, cAncho, cAlto, Estilos, NULL, NULL, _Aplicacion->_ColorFondoVentana);
		// Creo un control para que sea el marco sin las barras de scroll
		_Marco.CrearControlEx(this, L"DMarcoScrollEx_Marco", L"", ID_PAGINA, 0, 0, cAncho, cAlto, WS_CHILD | WS_VISIBLE, NULL, NULL, _Aplicacion->_ColorFondoVentana);
		// Creo un control dentro que se usara para mover todo el contenido
		// Todos los controles creados dentro del MarcoScroll realmente se crean dentro del control _Pagina (Se redirige automáticamente desde DControlEx::CrearControlEx)
		_Pagina.CrearControlEx(&_Marco, L"DMarcoScrollEx_Pagina", L"", ID_PAGINA, 0, 0, cAncho, cAlto, WS_CHILD | WS_VISIBLE, NULL, NULL, _Aplicacion->_ColorFondoVentana);
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
	void DMarcoScrollEx::Evento_ControlCreado(DhWnd *nControl) {
		// Si no es la página
		if (nControl->TipoWnd() != DhWnd_Tipo_MarcoScrollEx_Pagina) {
			CalcularPagina();
		}
	}

	// Función que calcula el tamaño que debe tener la pagina según su contenido
	void DMarcoScrollEx::CalcularPagina(void) {
		RECT RC;
		GetClientRect(_hWnd, &RC);
		// Establezco el ancho y alturas mínimos
		_AnchoPagina = RC.right;
		_AltoPagina  = RC.bottom;
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
			// 1.0 es el total
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
			_ScrollV_Pagina = (1.0f / _AltoPagina) * static_cast<float>(RC.bottom);
			// Si la suma de la posición del scroll y la pagina son más grandes que 100% 
			if (_ScrollV_Posicion > 1.0f) {
				_ScrollV_Posicion = 1.0f; // Ajusto la posición del scroll para que sumada con la página sea el 100%
			}
		}
		ScrollV_Visible(SV);

		// Redimensiono la página
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
		if (This->_AnchoPagina < RVC.right) This->_AnchoPagina = RVC.right;
		// Compruebo si el alto es mas grande que el que hay guardado
		if (This->_AltoPagina < RVC.right)  This->_AltoPagina = RVC.bottom;
		
		// Continuo la enumeración
		return TRUE;
	}


	// Gestor de mensages para el marco
	LRESULT CALLBACK DMarcoScrollEx::GestorMensajes(UINT uMsg, WPARAM wParam, LPARAM lParam) {
		switch (uMsg) {

		}
		return DBarraScrollEx::GestorMensajes(uMsg, wParam, lParam);
	}















	// Gestor de mensages para la página
	LRESULT CALLBACK DMarcoScrollEx::DMarcoScrollEx_Pagina::GestorMensajes(UINT uMsg, WPARAM wParam, LPARAM lParam) {
		switch (uMsg) {
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