#include "pch.h"
#include "DMenuEx.h"
#include "DMouse.h"
#include <dwmapi.h>


namespace DWL {
	DMenuEx_Skin::DMenuEx_Skin(void) :
		// Colores por defecto
		FondoNormal			(COLOR_MENU_FONDO),
		FondoResaltado		(COLOR_MENU_FONDO_RESALTADO),
		FondoPresionado		(COLOR_MENU_FONDO_PRESIONADO),
		TextoNormal			(COLOR_MENU_TEXTO),
		TextoResaltado		(COLOR_MENU_TEXTO_RESALTADO),
		TextoPresionado		(COLOR_MENU_TEXTO_PRESIONADO),
		TextoDesactivado	(COLOR_MENU_TEXTO_DESACTIVADO),
		TextoSombra			(COLOR_MENU_TEXTO_SOMBRA),
		Borde				(COLOR_MENU_BORDE),
		Separador			(COLOR_MENU_SEPARADOR),
		// Fuente
		FuenteTam			(FUENTE_GRANDE),
		FuenteNombre		(FUENTE_NOMBRE),
		FuenteNegrita		(FALSE),
		FuenteCursiva		(FALSE),
		FuenteSubrayado		(FALSE),
		FuenteSombraTexto	(TRUE)  {
	}




	DMenuEx *DMenuEx::_ResultadoModal	= NULL;
	DhWnd	*DMenuEx::_hWndDest			= NULL;


	DMenuEx::DMenuEx(void) : 
		DWL::DVentana()													, 
		_Padre(NULL)													, 
		_Tipo(DMenuEx_Tipo::DMenuEx_Tipo_Raiz)							,
		_ID(0)															, 
		_Activado(TRUE)													, 
		_MenuResaltado(NULL)											, 
		_MenuPresionado(NULL)											,
		_MenuDesplegado(NULL)											,
		_ColorFondo(Skin.FondoNormal)									,
		_ColorTexto(Skin.TextoNormal)									,
		_BarraPosX(0)													, 
		_MargenI(0)														,
		MaxOpacidad(230.0f)												, 
		Parametro(0)													,
		EventoMostrarMenu([=](DMenuEx &e) { Evento_MostrarMenu(e);	})	,
		EventoMouseClick([=](DMenuEx &e)  { Evento_MouseClick(e);	})	,
		_OcultarEnClick(TRUE)											,
		_CalcularTam(FALSE)												,
		_Recta({ 0, 0, 0, 0 })											{
			Fuente.CrearFuente(Skin.FuenteTam, Skin.FuenteNombre.c_str(), Skin.FuenteNegrita, Skin.FuenteCursiva, Skin.FuenteSubrayado);
	}

	// Constructor menú tipo separador (interno AgregarSeparador)
	DMenuEx::DMenuEx(DMenuEx* nPadre, DMenuEx_Tipo nTipo, DhWnd* nhWndPadre, const INT_PTR nID) : 
		DWL::DVentana()													,
		_Padre(nPadre)													, 
		_Tipo(DMenuEx_Tipo::DMenuEx_Tipo_Separador)						,
		_ID(nID)														,
		_MenuResaltado(NULL)											,
		_MenuPresionado(NULL)											,
		_Activado(TRUE)													,
		_MenuDesplegado(NULL)											,
		_ColorFondo(Skin.FondoNormal)									, 
		_ColorTexto(Skin.TextoNormal)									, 
		_BarraPosX(0)													, 
		_MargenI(0)														, 
		Parametro(0)													,
		MaxOpacidad(230.0f)												,
		EventoMostrarMenu([=](DMenuEx &e) { Evento_MostrarMenu(e);	})	,
		EventoMouseClick([=](DMenuEx &e)  { Evento_MouseClick(e);	})	,
		_OcultarEnClick(TRUE)											,
		_CalcularTam(FALSE)												,
		_Recta({ 0, 0, 0, 0 })											{
			Fuente.CrearFuente(Skin.FuenteTam, Skin.FuenteNombre.c_str(), Skin.FuenteNegrita, Skin.FuenteCursiva, Skin.FuenteSubrayado);
	}

	// Constructor menú tipo texto (interno AgregarMenu)
	DMenuEx::DMenuEx(DMenuEx *nPadre, DMenuEx_Tipo nTipo, DhWnd *nhWndPadre, const INT_PTR nID, const wchar_t *nTexto, const INT_PTR nIconoRecursos, const BOOL nActivado) : 
		DWL::DVentana()													,
		_Padre(nPadre)													,
		_Tipo(DMenuEx_Tipo::DMenuEx_Tipo_Texto)							,
		_ID(nID)														, 
		_Texto(nTexto)													, 
		_Activado(nActivado)											,
		_MenuResaltado(NULL)											,
		_MenuPresionado(NULL)											,
		_MenuDesplegado(NULL)											,
		_ColorFondo(Skin.FondoNormal)									,
		_ColorTexto(Skin.TextoNormal)									,
		_BarraPosX(0)													, 
		_MargenI(0)														,
		Parametro(0)													,
		MaxOpacidad(230.0f)												,
		EventoMostrarMenu([=](DMenuEx &e) { Evento_MostrarMenu(e);	})	,
		EventoMouseClick([=](DMenuEx &e)  { Evento_MouseClick(e);	})	,
		_OcultarEnClick(TRUE)											,
		_CalcularTam(FALSE)												,
		_Recta({ 0, 0, 0, 0 })											{
			_Icono.CrearIconoRecursos(nIconoRecursos, DMENUEX_TAMICONO, DMENUEX_TAMICONO);
			Fuente.CrearFuente(Skin.FuenteTam, Skin.FuenteNombre.c_str(), Skin.FuenteNegrita, Skin.FuenteCursiva, Skin.FuenteSubrayado);
	}

	// Constructor menú tipo texto (interno AgregarBarra)
	DMenuEx::DMenuEx(DMenuEx *nPadre, DMenuEx_Tipo nTipo, DhWnd *nhWndPadre, const INT_PTR nID, const wchar_t *nTexto, const INT_PTR nIconoRecursos, const BOOL nActivado, const float nMinimo, const float nMaximo, const float nValor) :
		DWL::DVentana()													,
		_Padre(nPadre)													,
		_Tipo(DMenuEx_Tipo::DMenuEx_Tipo_Barra)							,
		_ID(nID)														, 
		_Texto(nTexto)													,
		_Activado(nActivado)											,
		_MenuResaltado(NULL)											,
		_MenuPresionado(NULL)											,
		_MenuDesplegado(NULL)											,
		_ColorFondo(Skin.FondoNormal)									,
		_ColorTexto(Skin.TextoNormal)									, 
		_BarraPosX(0)													, 
		_MargenI(0)														,
		Parametro(0)													,
		MaxOpacidad(230.0f)												,
		EventoMostrarMenu([=](DMenuEx &e) { Evento_MostrarMenu(e);	})	,
		EventoMouseClick([=](DMenuEx &e)  { Evento_MouseClick(e);	})	,
		_OcultarEnClick(TRUE)											,
		_CalcularTam(FALSE)												,
		_Recta({ 0, 0, 0, 0 })											{
			_Icono.CrearIconoRecursos(nIconoRecursos, DMENUEX_TAMICONO, DMENUEX_TAMICONO);
			_Barra._Minimo = nMinimo;
			_Barra._Maximo = nMaximo;
			_Barra._Valor  = nValor;
			Fuente.CrearFuente(Skin.FuenteTam, Skin.FuenteNombre.c_str(), Skin.FuenteNegrita, Skin.FuenteCursiva, Skin.FuenteSubrayado);
	}

	// Destructor
	DMenuEx::~DMenuEx(void) {
		Terminar();
	}

	// Función que muestra el menú, y espera a una respuesta por parte del usuario
	// Devuelve NULL si no se ha seleccionado ningún menú, o devuelve el menú seleccionado.
	DMenuEx *DMenuEx::MostrarModal(DhWnd *nPadre, const int PosX, const int PosY) {
		_ResultadoModal = NULL;
		Mostrar(nPadre, PosX, PosY);
		while (_hWnd != NULL) {
			MSG msg;
			if (TRUE == GetMessage(&msg, 0, 0, 0)) {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}

		if (_ResultadoModal != NULL) {
			if (_ResultadoModal->Activado() == TRUE) {
				return _ResultadoModal;
			}
		}

		return NULL;
	}

	DMenuEx *DMenuEx::MostrarModal(DhWnd *nPadre) {
		POINT Punto;
		GetCursorPos(&Punto);
		return MostrarModal(nPadre, Punto.x, Punto.y);
	}

	void DMenuEx::Mostrar(DhWnd *nPadre) {
		POINT Punto;
		GetCursorPos(&Punto);
		Mostrar(nPadre, Punto.x, Punto.y);
	}


	void DMenuEx::Mostrar(DhWnd *nPadre, int PosX, int PosY) {
		_MouseDentro    = FALSE;
		_MenuResaltado  = NULL;
		_MenuPresionado = NULL;
		_MenuDesplegado = NULL;

		// Ejecuto el evento lambda de antes de mostrar el menú
		// De esta forma se puede modificar los submenus justo antes de mostrar-los
		EventoMostrarMenu(*this);

		// Calculo el tamaño que necesitará el menú
		POINT Tam = CalcularEspacio();
		// Guardo el hWnd de la ventana que ha llamado al menú
		_hWndDest = nPadre;
		if (_hWnd == NULL) {

			// Miro si hay suficiente espacio para mostrar el menú en el monitor actual
			MONITORINFO InfoMonitor;
			InfoMonitor.cbSize = sizeof(MONITORINFO);
			// Obtengo la recta del monitor
			HMONITOR Monitor = MonitorFromWindow(_hWndDest->hWnd(), MONITOR_DEFAULTTOPRIMARY);
			GetMonitorInfoA(Monitor, &InfoMonitor);
			// No hay suficiente ancho
			if (PosX + Tam.x < InfoMonitor.rcMonitor.left || PosX + Tam.x > InfoMonitor.rcMonitor.right) {
				PosX = InfoMonitor.rcMonitor.right - Tam.x;
			}
			if (PosY + Tam.y < InfoMonitor.rcMonitor.top || PosY + Tam.y > InfoMonitor.rcMonitor.bottom) {
				PosY = InfoMonitor.rcMonitor.bottom - Tam.y;
			}
			// Creo la ventana
			CrearVentana(_hWndDest, L"DMenuEx", L"", PosX, PosY, Tam.x, Tam.y, WS_POPUP | WS_CAPTION, WS_EX_TOPMOST, CS_DBLCLKS);	
			DMouse::CambiarCursor();
			Opacidad(0);
			MARGINS Margen = { 0, 0, 0, 1 };
			DwmExtendFrameIntoClientArea(_hWnd, &Margen);

			// Asigno la posición del menú y lo hago siempre visible
			SetWindowPos(_hWnd, HWND_TOPMOST, PosX, PosY, Tam.x, Tam.y, SWP_NOACTIVATE | SWP_FRAMECHANGED | SWP_SHOWWINDOW);

			// Creo las barras (si ahy alguna), y asigno los colores por defecto
			for (size_t i = 0; i < _Menus.size(); i++) {
				_Menus[i]->_ColorFondo = Skin.FondoNormal;
				_Menus[i]->_ColorTexto = (_Menus[i]->_Activado == TRUE) ? Skin.TextoNormal : Skin.TextoDesactivado;

				if (_Menus[i]->_Tipo == DMenuEx_Tipo::DMenuEx_Tipo_Barra) {
					int nAnchoBarra = _Menus[i]->_Recta.right - (_BarraPosX + DMENUEX_MARGEN_X);
					_Menus[i]->_Barra.CrearBarraDesplazamientoEx(this, _BarraPosX, _Menus[i]->_Recta.top + DMENUEX_MARGEN_Y, nAnchoBarra, (_Menus[i]->_Recta.bottom - _Menus[i]->_Recta.top) - (DMENUEX_MARGEN_Y * 2), _Menus[i]->_ID, _Menus[i]->_Barra._Minimo, _Menus[i]->_Barra._Maximo, _Menus[i]->_Barra._Valor);
					_Menus[i]->_Barra.Activado(_Menus[i]->_Activado);
				}
			}
			_AniMostrar.Terminar();
			_AniMostrar.Iniciar({ 0.0f }, { &MaxOpacidad }, DhWnd::TiempoAnimaciones, [=](DAnimacion::Valores& Datos, const BOOL Terminado) {
				Opacidad(static_cast<BYTE>(Datos[0].Decimal()));
			});

		}
		// Asigno el foco al menú
		SetFocus(_hWnd); // REVISADO
		// Envio el mensaje WM_NCACTIVATE a la ventana principal para que no se vea como pierde el foco, y parezca que el desplegable es un hijo de la ventana principal
		SendMessage(_hWndDest->hWnd(), WM_NCACTIVATE, TRUE, 0);
	}

	// Función que oculta este menú 
	//	Si OcultarTodos es TRUE, se ocultaran todos los menus hijos de este menú, y todos los ancestros de este menú (padre, abuelo, etc..)
	//  Si OcultarTodos es FALSE, se ocultaran todos los menus hijos de este menú, y este menú.
	void DMenuEx::Ocultar(const BOOL OcultarTodos) {
		// Oculto todos los menús hijos de este
		_OcultarRecursivo(this);
		// Elimino las barras
		for (size_t i = 0; i < _Menus.size(); i++) {
			_Menus[i]->_Barra.Destruir();
		}

		if (OcultarTodos == TRUE) {
			// Oculto los menus ancestros
			DMenuEx *pPadre = _Padre;
			while (pPadre != NULL) {
				pPadre->Ocultar(FALSE);
				pPadre = pPadre->_Padre;
			}
			SetFocus(_hWndDest->hWnd()); // REVISADO
		}
		// Destruyo la ventana del menú
		DestroyWindow(_hWnd);
		_hWnd			= NULL;
	}

	// Oculta todos los menus hijos recursivamente
	void DMenuEx::_OcultarRecursivo(DMenuEx *oMenu) {
		for (size_t i = 0; i < oMenu->_Menus.size(); i++) {
			if (oMenu->_Menus[i]->_Menus.size() > 0) {
				oMenu->_Menus[i]->Ocultar(FALSE);
			}
		}
	}

	// Busca un sub-menu de este menú por su ID (recursiva)
	DMenuEx *DMenuEx::BuscarMenu(const INT_PTR bID) {
		DMenuEx* TmpMenu = NULL;
		for (size_t i = 0; i < _Menus.size(); i++) {
			if (bID == _Menus[i]->ID()) return _Menus[i];
			TmpMenu = _Menus[i]->BuscarMenu(bID);
			if (TmpMenu != NULL) return TmpMenu;
		}
		return NULL;
	}

	// Función que agrega un sub-menú a este menú
	DMenuEx *DMenuEx::AgregarMenu(const INT_PTR nID, const wchar_t *nTexto, const INT_PTR nIconoRecursos, const int nPosicion, const BOOL nActivado) {
		int Pos = nPosicion;
		if (nPosicion == -1) {
			Pos = static_cast<int>(_Menus.size());
		}

		DMenuEx *TmpMenu = new DMenuEx(this, DMenuEx_Tipo::DMenuEx_Tipo_Texto, _hWndDest, nID, nTexto, nIconoRecursos, nActivado);
		_Menus.insert(_Menus.begin() + Pos, TmpMenu);
		return TmpMenu;
	}

	// Función que agrega un sub-menú con una barra a este menú
	DMenuEx *DMenuEx::AgregarBarra(const INT_PTR nID, const wchar_t *nTexto, const INT_PTR nIconoRecursos, const float nMinimo, const float nMaximo, const float nValor, const DBarraEx_MostrarValor nMostrarValor, const int nPosicion, const BOOL nActivado) {
		int Pos = nPosicion;
		if (nPosicion == -1) {
			Pos = static_cast<int>(_Menus.size());
		}
		
		DMenuEx *TmpMenu = new DMenuEx(this, DMenuEx_Tipo::DMenuEx_Tipo_Barra, _hWndDest, nID, nTexto, nIconoRecursos, nActivado, nMinimo, nMaximo, nValor);
		TmpMenu->_Barra.MostrarValor(nMostrarValor);
		_Menus.insert(_Menus.begin() + Pos, TmpMenu);
		return TmpMenu;		
	}

	// Función que agrega un separador a este menú
	DMenuEx *DMenuEx::AgregarSeparador(const INT_PTR uID, const int nPosicion) {
		int Pos = nPosicion;
		if (nPosicion == -1) {
			Pos = static_cast<int>(_Menus.size());
		}

		DMenuEx *TmpMenu = new DMenuEx(this, DMenuEx_Tipo::DMenuEx_Tipo_Separador, _hWndDest, uID);
		_Menus.insert(_Menus.begin() + Pos, TmpMenu);
		return TmpMenu;
	}


	// Función para activar / desactivar el menú
	void DMenuEx::Activado(const BOOL nActivar) {
		_Activado = nActivar;
		Transicion((nActivar == TRUE) ? DMenuEx_Transicion::DMenuEx_Transicion_Normal : DMenuEx_Transicion::DMenuEx_Transicion_Desactivado);
		
		if (_Barra.hWnd() != NULL) 
			_Barra.Activado(nActivar);

		if (_Padre != NULL) {
			if (_Padre->_hWnd != NULL) {
				_Padre->Repintar();
			}
		}
	}

	// Función que termina el menú y elimina la memória
	void DMenuEx::Terminar(void) {
		#if DMENUEX_MOSTRARDEBUG == TRUE
			Debug_Escribir(L"DMenuEx::Terminar Ocultar menu\n");
		#endif
		Ocultar();
		EliminarTodosLosMenus();
//		_hWndDest	= NULL;
//		_Padre		= NULL;
//		_Tipo		= DMenuEx_Tipo::DMenuEx_Tipo_Raiz;
//		_ID			= 0;
	}

	// Función para eliminar todos los menus
	void DMenuEx::EliminarTodosLosMenus(void) {		
		for (size_t i = 0; i < _Menus.size(); i++) {
			delete _Menus[i];
		}
		_Menus.resize(0);
	}

	// Función para eliminar el menu especificado por la ID
	void DMenuEx::EliminarMenu(const INT_PTR eID) {
		for (size_t i = 0; i < _Menus.size(); i++) {
			if (_Menus[i]->ID() == eID) {
				delete _Menus[i];
				_Menus.erase(_Menus.begin() + i);
			}
		}
	}

	// Función para eliminar el menu especificado por el objeto
	void DMenuEx::EliminarMenu(DMenuEx *eMenu) {
		for (size_t i = 0; i < _Menus.size(); i++) {
			if (_Menus[i] == eMenu) {
				delete _Menus[i];
				_Menus.erase(_Menus.begin() + i);
			}
		}
	}

	void DMenuEx::Icono(const int nIconoRecursos) {
		_Icono.CrearIconoRecursos(nIconoRecursos, DMENUEX_TAMICONO, DMENUEX_TAMICONO);
		if (_Padre != NULL) {
			if (_Padre->_hWnd != NULL) {
				_Padre->Repintar();
			}
		}
	}

	// Función para asignar el texto del menú (wchar_t)
	// Si nRepìntar es TRUE, se re-calcula el tamaño del padre, y se repinta el padre
	void DMenuEx::Texto(const wchar_t *nTxt, const BOOL nRepintar) {
		_Texto = nTxt; 
		if (_Padre != NULL) {
			_Padre->_CalcularTam = TRUE;
			if (nRepintar == TRUE) {
				if (_Padre->_hWnd != NULL) {
					// Hay que recalcular el tamaño del padre, y redimensionar el menú si es necesario
					_Padre->Repintar();
				}
			}
		}
	}

	// Función para asignar el texto del menú (std::wstring)
	// Si nRepìntar es TRUE, se re-calcula el tamaño del padre, y se repinta el padre
	void DMenuEx::Texto(std::wstring &nTxt, const BOOL nRepintar) {
		_Texto = nTxt; 
		if (_Padre != NULL) {
			_Padre->_CalcularTam = TRUE;
			if (nRepintar == TRUE) {
				if (_Padre->_hWnd != NULL) {
					_Padre->Repintar();
				}
			}
		}
	}

	// Al repintar tambien miro si hay que redimensionar la ventana del menú
	void DMenuEx::Repintar(void) {
		// Si la ventana está visible, y se ha modificado el texto de algún menú
		if (_hWnd != NULL && _CalcularTam == TRUE) {
			POINT Tam;
			Tam = CalcularEspacio();
			SetWindowPos(_hWnd, NULL, 0, 0, Tam.x, Tam.y, SWP_NOMOVE | SWP_NOOWNERZORDER);
		}
		DhWnd::Repintar();
	}


	// Función que pinta todo este menú
	void DMenuEx::Pintar(HDC hDC) {
		// Obtengo el tamaño de la ventana
		RECT RC;
		GetClientRect(_hWnd, &RC);

		// Creo un DC compatible para utilizar-lo de back buffer
		HDC		DC		 = CreateCompatibleDC(NULL);
		HBITMAP Bmp		 = CreateCompatibleBitmap(hDC, RC.right, RC.bottom);
		HBITMAP BmpViejo = static_cast<HBITMAP>(SelectObject(DC, Bmp));
//		HFONT	VFont	 = static_cast<HFONT>(SelectObject(DC, Fuente21Normal.Fuente()));

		// Pinto el borde
		HBRUSH BrochaBorde = CreateSolidBrush(Skin.Borde);
		FillRect(DC, &RC, BrochaBorde);
		DeleteObject(BrochaBorde);

		// El texto se pintará sin fondo
		SetBkMode(DC, TRANSPARENT);

		for (size_t i = 0; i < _Menus.size(); i++) {
			switch (_Menus[i]->_Tipo) {
				case DMenuEx_Tipo::DMenuEx_Tipo_Texto :
				case DMenuEx_Tipo::DMenuEx_Tipo_Barra :
					_PintarMenu(DC, _Menus[i]);
					break;
				case DMenuEx_Tipo::DMenuEx_Tipo_Separador :
					_PintarSeparador(DC, _Menus[i]);
					break;
			}
		}

		BitBlt(hDC, 0, 0, RC.right, RC.bottom, DC, 0, 0, SRCCOPY);

		// Elimino objetos gdi de la memoria
//		SelectObject(DC, VFont);
		SelectObject(DC, BmpViejo);
		DeleteObject(Bmp);
		DeleteDC(DC);
	}

	// Función que pinta un separador
	void DMenuEx::_PintarSeparador(HDC DC, DMenuEx *pMenu) {
		RECT RectaSeparador = { pMenu->_Recta.left + 5 , pMenu->_Recta.top + DMENUEX_MARGEN_Y,  pMenu->_Recta.right - 5 , pMenu->_Recta.bottom - DMENUEX_MARGEN_Y };

		// Pinto el fondo
		HBRUSH BrochaFondo = CreateSolidBrush(Skin.FondoNormal);
		FillRect(DC, &pMenu->_Recta, BrochaFondo);
		DeleteObject(BrochaFondo);

		// Pinto el separador
		HBRUSH BrochaSeparador = CreateSolidBrush(Skin.Separador);
		FillRect(DC, &RectaSeparador, BrochaSeparador);
		DeleteObject(BrochaSeparador);
	}

	// Función que pinta un sub-menú
	void DMenuEx::_PintarMenu(HDC DC, DMenuEx *pMenu) {
		int bPresionado = 0;
		//int MargenI = (_TieneIconos == TRUE) ? DMENUEX_TAMICONO + (DMENUEX_MARGEN_X * 2) : DMENUEX_MARGEN_X;
		// Pinto el fondo
		HBRUSH BrochaFondo = CreateSolidBrush(pMenu->_ColorFondo);
		FillRect(DC, &pMenu->_Recta, BrochaFondo);
		DeleteObject(BrochaFondo);

		// Pinto el icono
		int YIcono = static_cast<int>((static_cast<float>(pMenu->_Recta.bottom - pMenu->_Recta.top) - static_cast<float>(DMENUEX_TAMICONO)) / 2.0f);
		if (pMenu->_Icono() != NULL) {
			DrawIconEx(DC, bPresionado + DMENUEX_MARGEN_X, bPresionado + pMenu->_Recta.top + YIcono, pMenu->_Icono(), DMENUEX_TAMICONO, DMENUEX_TAMICONO, 0, 0, DI_NORMAL);
		}

		HFONT	VFont = static_cast<HFONT>(SelectObject(DC, pMenu->Fuente()));
		if (Skin.FuenteSombraTexto == TRUE) {
			// Pinto la sombra del texto
			SetTextColor(DC, Skin.TextoSombra);
			TextOut(DC, 1 + bPresionado + _MargenI, 1 + pMenu->_Recta.top + DMENUEX_MARGEN_Y, pMenu->_Texto.c_str(), static_cast<int>(pMenu->_Texto.size()));
		}

		// Pinto el texto
		SetTextColor(DC, pMenu->_ColorTexto);
		TextOut(DC, bPresionado + _MargenI, pMenu->_Recta.top + DMENUEX_MARGEN_Y, pMenu->_Texto.c_str(), static_cast<int>(pMenu->_Texto.size()));

		SelectObject(DC, VFont);
		
		// Si el menú tiene submenus y está activado
		if (pMenu->_Menus.size() > 0 && pMenu->_Activado == TRUE) {
			// Pinto el expansor
			_PintarExpansor(DC, pMenu->_Recta.right - (DMENUEX_TAMICONO + DMENUEX_MARGEN_X), pMenu->_Recta.top + YIcono);
		}

		// Si el menu tiene barra la pinto
		if (pMenu->_Tipo == DMenuEx_Tipo::DMenuEx_Tipo_Barra) {
			pMenu->_Barra.PintarBarraEx(DC, _BarraPosX, pMenu->_Recta.top + DMENUEX_MARGEN_Y);
		}
	}

	// Función que pinta el expansor (para mostrar que este sub-menu tiene uno o mas sub-menus hijos)
	void DMenuEx::_PintarExpansor(HDC DC, const int eX, const int eY) {
		HPEN Pluma  = CreatePen(PS_SOLID, 1, Skin.TextoNormal);
		HPEN VPluma = static_cast<HPEN>(SelectObject(DC, Pluma));
		int MedioIcono = (DMENUEX_TAMICONO / 2);
		MoveToEx(DC, eX + MedioIcono, eY, NULL);
		LineTo(DC, eX + DMENUEX_TAMICONO, eY + MedioIcono);
		LineTo(DC, eX + MedioIcono, eY + DMENUEX_TAMICONO);
		SelectObject(DC, VPluma);
		DeleteObject(Pluma);
	}


	// Fuinción que calcula el espacio necesario para mostrar este menú
	const POINT DMenuEx::CalcularEspacio(void) {
		LONG  TmpAncho  = 0;
		POINT Ret		= { 0, 0 };
		SIZE  Tam		= { 0, 0 };
		HDC   hDC		= GetDC(NULL);

		// Compruebo si los sub-menus tienen iconos para dejar un margen a la izquierda
		_MargenI = DMENUEX_MARGEN_X;
		for (size_t i = 0; i < _Menus.size(); i++) {
			if (_Menus[i]->_Icono.ID() != 0) {
				_MargenI = DMENUEX_TAMICONO + (DMENUEX_MARGEN_X * 2);
				break;
			}
		}
		
		_BarraPosX = 0;

		for (size_t i = 0; i < _Menus.size(); i++) {
			HFONT VFont = static_cast<HFONT>(SelectObject(hDC, _Menus[i]->Fuente()));
			switch (_Menus[i]->_Tipo) {
				case DMenuEx_Tipo::DMenuEx_Tipo_Texto :
					// Obtengo el tamaño del texto
					GetTextExtentPoint32(hDC, _Menus[i]->_Texto.c_str(), static_cast<int>(_Menus[i]->_Texto.size()), &Tam);

					// Calculo el ancho necesario
					// Margen + Ancho icono + Margen + Ancho texto + Margen + Acho icono + Margen
					TmpAncho = Tam.cx + _MargenI + (DMENUEX_MARGEN_X);
					// Si el ancho temporal es mas grande que el ancho final, asigno el ancho temporal al ancho final
					if (TmpAncho > Ret.x) Ret.x = TmpAncho;

					// Calculo la altura necesaria
					// Si la altura del texto es menor que la altura del icono, asigno la altura del icono
					if (Tam.cy < DMENUEX_TAMICONO) Tam.cy = DMENUEX_TAMICONO;
					// Asigno la posición inicial del menú
					_Menus[i]->_Recta.top = Ret.y + DMENUEX_BORDE;
					// Sumo el margen superior, margen inferior, y altura del texto
					Ret.y += (DMENUEX_MARGEN_Y * 2) + Tam.cy;
					_Menus[i]->_Recta.bottom = Ret.y + DMENUEX_BORDE;
					break;
				case DMenuEx_Tipo::DMenuEx_Tipo_Barra:
					// Obtengo el tamaño del texto
					GetTextExtentPoint32(hDC, _Menus[i]->_Texto.c_str(), static_cast<int>(_Menus[i]->_Texto.size()), &Tam);

					// Miro la x inicial para las barras (para que queden alineadas)
					if (_BarraPosX < Tam.cx + _MargenI + (DMENUEX_MARGEN_X))
						_BarraPosX = Tam.cx + _MargenI + (DMENUEX_MARGEN_X);

					// Calculo el ancho necesario
					// Margen + Ancho icono + Margen + Ancho texto + Margen + Acho icono + Margen
					TmpAncho = Tam.cx + _MargenI + (DMENUEX_TAMICONO) + (DMENUEX_MARGEN_X * 3) + DMENUEX_ANCHOBARRA;
					// Si el ancho temporal es mas grande que el ancho final, asigno el ancho temporal al ancho final
					if (TmpAncho > Ret.x) Ret.x = TmpAncho;

					// Calculo la altura necesaria
					// Si la altura del texto es menor que la altura del icono, asigno la altura del icono
					if (Tam.cy < DMENUEX_TAMICONO) Tam.cy = DMENUEX_TAMICONO;
					// Asigno la posición inicial del menú
					_Menus[i]->_Recta.top = Ret.y + DMENUEX_BORDE;
					// Sumo el margen superior, margen inferior, y altura del texto
					Ret.y += (DMENUEX_MARGEN_Y * 2) + Tam.cy;
					_Menus[i]->_Recta.bottom = Ret.y + DMENUEX_BORDE;
					break;

				case DMenuEx_Tipo::DMenuEx_Tipo_Separador :
					_Menus[i]->_Recta.top = Ret.y + DMENUEX_BORDE;
					Ret.y += (DMENUEX_MARGEN_Y * 2) + 1; // Espacio adicional para el separador
					_Menus[i]->_Recta.bottom = Ret.y + DMENUEX_BORDE;
					break;
			}
			SelectObject(hDC, VFont);
		}

/*		int nBarraPos = (DMENUEX_ANCHOBARRA + DMENUEX_TAMICONO + (DMENUEX_MARGEN_X * 3));
		// Reposiciono las barras a la derecha del menú, si hay espacio disponible
		if (_BarraPosX + nBarraPos > Ret.x - nBarraPos) {
			_BarraPosX = Ret.x - nBarraPos;
		}*/

		// Añado el borde
		Ret.x += (DMENUEX_BORDE * 2);
		Ret.y += (DMENUEX_BORDE * 2);

		// Miro si hay algun submenu con hijos para dejar espacio a la marca
		BOOL Marca = FALSE;
		for (size_t i = 0; i < _Menus.size(); i++) {
			if (_Menus[i]->TotalMenus() > 0) {
				Marca = TRUE;
				break;
			}
		}
		if (Marca == TRUE) {
			Ret.x += DMENUEX_TAMICONO + DMENUEX_MARGEN_X;
		}

		// Des-selecciono la fuente y libero el DC
		ReleaseDC(NULL, hDC);

		// Asigno el ancho a todos los submenus, una vez se ha calculado
		for (size_t i = 0; i < _Menus.size(); i++) {
			_Menus[i]->_Recta.left = DMENUEX_BORDE;
			_Menus[i]->_Recta.right = Ret.x - DMENUEX_BORDE;
		}

		_CalcularTam = FALSE;
		// Devuelve el tamaño total con bordes incluidos
		return Ret;
	}


	// Función que muestra un Sub-menu con mas sub-menus 
	void DMenuEx::_MostrarSubMenu(DMenuEx *nPadre, int cX, int cY, const BOOL AsignarFoco) {
		if (_Activado == FALSE) return;

		if (_hWnd == NULL) {

			EventoMostrarMenu(*this);

			#if DMENUEX_MOSTRARDEBUG == TRUE
				Debug_Escribir_Varg(L"DMenuEx::_MostrarSubMenu AsignarFoco = %d\n", AsignarFoco);
			#endif
			//POINT Punto = { cX, cY };
			RECT WR;
			GetWindowRect(nPadre->hWnd(), &WR);

			_MenuResaltado = NULL;
			_MenuPresionado = NULL;
			_MenuDesplegado = NULL;

			// Calculo el tamaño que necesitará el menú
			POINT Tam = CalcularEspacio();
			
			cX = -4 + WR.left + cX;
			cY = -1 + WR.top + cY;

			// Miro si hay suficiente espacio para mostrar el menú en el monitor actual
			MONITORINFO InfoMonitor;
			InfoMonitor.cbSize = sizeof(MONITORINFO);
			// Obtengo la recta del monitor
			HMONITOR Monitor = MonitorFromWindow(_hWndDest->hWnd(), MONITOR_DEFAULTTOPRIMARY);
			GetMonitorInfoA(Monitor, &InfoMonitor);
			// No hay suficiente ancho
			if (cX + Tam.x > InfoMonitor.rcMonitor.right) {
				cX = WR.left - Tam.x + 4;
			}
			if (cY + Tam.y > InfoMonitor.rcMonitor.bottom) {
				cY = WR.top - Tam.y + 1;
			}


			CrearVentana(_hWndDest, L"DMenuEx", L"", cX, cY, Tam.x, Tam.y, WS_POPUP | WS_CAPTION, NULL, CS_DBLCLKS);

			DMouse::CambiarCursor();
//			BYTE Op = static_cast<BYTE>(MaxOpacidad);
			Opacidad(0);

			MARGINS Margen = { 0, 0, 0, 1 };
			DwmExtendFrameIntoClientArea(_hWnd, &Margen);

			// Asigno la posición del menú y lo hago siempre visible
			SetWindowPos(_hWnd, HWND_TOPMOST, cX, cY, Tam.x, Tam.y, SWP_NOACTIVATE | SWP_FRAMECHANGED | SWP_SHOWWINDOW);

			// Creo las barras (si ahy alguna), y asigno los colores por defecto en cada submenu
			for (size_t i = 0; i < _Menus.size(); i++) {
				_Menus[i]->_ColorFondo = Skin.FondoNormal;
				_Menus[i]->_ColorTexto = Skin.TextoNormal;
				if (_Menus[i]->_Tipo == DMenuEx_Tipo::DMenuEx_Tipo_Barra) {
					int nAnchoBarra = _Menus[i]->_Recta.right - (_BarraPosX + DMENUEX_MARGEN_X);
					_Menus[i]->_Barra.CrearBarraDesplazamientoEx(this, _BarraPosX, _Menus[i]->_Recta.top + DMENUEX_MARGEN_Y, nAnchoBarra, (_Menus[i]->_Recta.bottom - _Menus[i]->_Recta.top) - (DMENUEX_MARGEN_Y * 2), _Menus[i]->_ID, _Menus[i]->_Barra._Minimo, _Menus[i]->_Barra._Maximo, _Menus[i]->_Barra._Valor);
					_Menus[i]->_Barra.Activado(_Menus[i]->_Activado);
				}
			}

			_AniMostrar.Terminar();
			_AniMostrar.Iniciar({ 0.0f }, { &MaxOpacidad }, DhWnd::TiempoAnimaciones, [=](DAnimacion::Valores& Datos, const BOOL Terminado) {
				Opacidad(static_cast<BYTE>(Datos[0].Entero()));
			});

		}

		SetFocus(_hWnd); // REVISADO
		// Envio el mensaje WM_NCACTIVATE a la ventana principal para que no se vea como pierde el foco, y parezca que el desplegable es un hijo de la ventana principal
		SendMessage(_hWndDest->hWnd(), WM_NCACTIVATE, TRUE, 0);
	}

	// Función que recibe el mensaje WM_PAINT
	void DMenuEx::_Evento_Pintar(void) {
		PAINTSTRUCT PS;
		HDC DC = BeginPaint(hWnd(), &PS);
		Pintar(DC);
		EndPaint(hWnd(), &PS);
	}

	// Función que recibe el mensaje WM_MOUSEMOVE
	void DMenuEx::_Evento_MouseMovimiento(WPARAM wParam, LPARAM lParam) {
		if (_MouseEntrando() == TRUE) {
			// Mouse enter
			#if DMENUEX_MOSTRARDEBUG == TRUE
				Debug_Escribir(L"DMenuEx::_Evento_MouseMovimiento WM_MOUSEENTER\n");
			#endif
		}

		#if DMENUEX_MOSTRARDEBUG == TRUE
			Debug_Escribir_Varg(L"DMenuEx::Evento_MouseMovimiento hWnd : %d\n", _hWnd);
		#endif
		// Si el mouse no está presionado
		if (_MenuPresionado == NULL) {
			DEventoMouse	DatosMouse(wParam, lParam, this);
			POINT			Pt = { DatosMouse.X(), DatosMouse.Y() };
			RECT            RC;
			GetClientRect(_hWnd, &RC);
			// Busco cual és el menú resaltado
			for (size_t i = 0; i < _Menus.size(); i++) {
				// Si el mouse está dentro de este menú
				if (PtInRect(&_Menus[i]->_Recta, Pt) != FALSE) {
					// Si el submenu resaltado no es el mismo (para no repintar siempre)
					if (_MenuResaltado != _Menus[i]) {
						// Oculto el ultimo menú desplegado
						if (_MenuDesplegado != _Menus[i] && _MenuDesplegado != NULL) {
							#if DMENUEX_MOSTRARDEBUG == TRUE
								Debug_Escribir(L"DMenuEx::Evento_MouseMovimiento Ocultar menu desplegado\n");
							#endif
							_MenuDesplegado->Ocultar();
							_MenuDesplegado = NULL;
							SetFocus(_hWnd); // REVISADO
							SendMessage(_hWndDest->hWnd(), WM_NCACTIVATE, TRUE, 0);
						}
						// Vuelvo a dejar el menu resaltado anterior al estado normal
						if (_MenuResaltado != NULL && _MenuResaltado->_Activado == TRUE) _MenuResaltado->Transicion(DMenuEx_Transicion::DMenuEx_Transicion_Normal);
						// Asigno el nuevo menú resaltado (aunque este desactivado)
						_MenuResaltado = _Menus[i];
						if (_Menus[i]->_Activado == TRUE) {
							// Estado para el menu resaltado a resaltado
							_MenuResaltado->Transicion(DMenuEx_Transicion::DMenuEx_Transicion_Resaltado);
						}

						// Si tiene Sub-menus
						if (_MenuResaltado->TotalMenus() > 0) {
							_MenuResaltado->_MostrarSubMenu(this, RC.right, _MenuResaltado->_Recta.top);
							_MenuDesplegado = _MenuResaltado;
						}
						Repintar();
					}
					break;
				}
			}
		}
	}

	void DMenuEx::_Evento_MousePresionado(const int Boton, WPARAM wParam, LPARAM lParam) {
		#if DMENUEX_MOSTRARDEBUG == TRUE
			Debug_Escribir(L"DMenuEx::_Evento_MousePresionado\n");
		#endif
		SetCapture(_hWnd);

		// Asigno el menú presionado
		DEventoMouse	DatosMouse(wParam, lParam, this);
		POINT Pt = { DatosMouse.X(), DatosMouse.Y() };
		for (size_t i = 0; i < _Menus.size(); i++) {
			if (PtInRect(&_Menus[i]->_Recta, Pt) != FALSE) {
				_MenuPresionado = _Menus[i];
				if (_MenuPresionado->_Activado == TRUE)	_MenuPresionado->Transicion(DMenuEx_Transicion::DMenuEx_Transicion_Presionado);
				Repintar();
				return;
			}
		}
	}

	// Función que devuelve si el menú está visible o no
	const BOOL DMenuEx::Visible(void) { 
		if (_hWnd != NULL) return IsWindowVisible(_hWnd);
		return FALSE;
	}


	void DMenuEx::Transicion(const DMenuEx_Transicion nTransicion) {
		DWORD Duracion = DhWnd::TiempoAnimaciones;
		if (_AniTransicion.Animando() == TRUE) {
			Duracion = _AniTransicion.TiempoActual();
			_AniTransicion.Terminar();
		}

		COLORREF *FondoHasta = 0, *TextoHasta = 0;
		switch (nTransicion) {
			case DMenuEx_Transicion::DMenuEx_Transicion_Normal:
				FondoHasta = &Skin.FondoNormal;
				TextoHasta = &Skin.TextoNormal;
				break;
			case DMenuEx_Transicion::DMenuEx_Transicion_Resaltado:
				FondoHasta = &Skin.FondoResaltado;
				TextoHasta = &Skin.TextoResaltado;
				break;
			case DMenuEx_Transicion::DMenuEx_Transicion_Presionado:
				FondoHasta = &Skin.FondoPresionado;
				TextoHasta = &Skin.TextoPresionado;
				break;
			case DMenuEx_Transicion::DMenuEx_Transicion_Desactivado:
				FondoHasta = &Skin.FondoNormal;
				TextoHasta = &Skin.TextoDesactivado;
				break;
		}
		_AniTransicion.Iniciar({ _ColorFondo, _ColorTexto }, { FondoHasta, TextoHasta }, Duracion, [=](DAnimacion::Valores& Datos, const BOOL Terminado) {
			_ColorFondo = Datos[0].Color();
			_ColorTexto = Datos[1].Color();
			_Padre->Repintar();
		});
		#if DMENUEX_MOSTRARDEBUG == TRUE
			Debug_Escribir_Varg(L"DMenuEx::Transición %d\n", nTransicion);
		#endif

	}
	
	void DMenuEx::_Evento_MouseSoltado(const int Boton, WPARAM wParam, LPARAM lParam) {
		// Repintar;
		BOOL Rp = FALSE;
		ReleaseCapture();
		// Si existe un menu presionado		
		if (_MenuPresionado) {
			// Si está habilitado el OcultarEnClick, oculto todos los menus
			if (_MenuPresionado->_OcultarEnClick == TRUE) {
				Ocultar(TRUE);
			}
			else {
				Rp = TRUE;
			}
		}
		// Si no existe un menu presionado, por defecto cerramos el menú
		else {
			Ocultar(TRUE);
		}

		if (_MenuPresionado != NULL) {

			#if DMENUEX_MOSTRARDEBUG == TRUE
				Debug_Escribir(L"DMenuEx::_Evento_MouseSoltado\n");
			#endif

			DEventoMouse DatosMouse(wParam, lParam, this, Boton);
			POINT Pt = { DatosMouse.X(), DatosMouse.Y() };
			if (PtInRect(&_MenuPresionado->_Recta, Pt) == TRUE) {
				_ResultadoModal = (_Activado == TRUE) ? _MenuPresionado : NULL;
				if (_ResultadoModal != NULL) {
					if (_ResultadoModal->Activado() == TRUE) {
						if (_ResultadoModal->_Tipo == DMenuEx_Tipo::DMenuEx_Tipo_Texto) {
							// Envio el mensaje conforme se ha hecho click en el menú al HWND destino
							SendMessage(_hWndDest->hWnd(), DWL_MENUEX_CLICK, _ResultadoModal->ID(), 0);
							// Llamo al evento lambda
							EventoMouseClick(*_ResultadoModal);
						}
					}
				}
			}

			if (Rp == FALSE) _MenuPresionado = NULL;
		}

		if (Rp == TRUE) { // No hace falta comprobar si _MenuPresionado no es NULL, se hace arriba
			_MenuPresionado->Transicion(DMenuEx_Transicion::DMenuEx_Transicion_Resaltado);
			_MenuPresionado = NULL;
		}
	}



	
	void DMenuEx::_Evento_TeclaPresionada(WPARAM wParam, LPARAM lParam) {
		DEventoTeclado DatosTeclado(wParam, lParam, this);
		RECT RC;
		GetClientRect(_hWnd, &RC);

//		DhWnd::_Teclado[DatosTeclado.TeclaVirtual()] = true;
		
		// Obtengo la posición del menu resaltado dentro del vector
		int MenuPos = -1;
		if (_MenuResaltado != NULL) {
			for (MenuPos = 0; MenuPos < static_cast<int>(_Menus.size()); MenuPos++) {
				if (_Menus[MenuPos] == _MenuResaltado) {
					break;
				}
			}
		}

		// Elijo el menu resaltado según la tecla presionada
		switch (DatosTeclado.TeclaVirtual()) {
			case VK_HOME : 
				#if DMENUEX_MOSTRARDEBUG == TRUE
					Debug_Escribir(L"DMenuEx::_Evento_TeclaPresionada VK_HOME\n");
				#endif
				_MenuResaltado = _Menus[0];
				break;
			case VK_END:
				#if DMENUEX_MOSTRARDEBUG == TRUE
					Debug_Escribir(L"DMenuEx::_Evento_TeclaPresionada VK_END\n");
				#endif
				_MenuResaltado = _Menus[_Menus.size() - 1];
				break;
			case VK_DOWN :
				if (MenuPos < static_cast<int>(_Menus.size() - 1))	MenuPos ++;
				// Si el menu resaltado tiene un siguiente menú
				if (MenuPos < static_cast<int>(_Menus.size() - 1)) {
					// Si es un separador lo salto
					if (_Menus[MenuPos]->_Tipo == DMenuEx_Tipo::DMenuEx_Tipo_Separador) MenuPos++;
				}
				// Si el menu resaltado no tiene un siguiente menu
				else {
					// Si es un separador, vuelvo atras
					if (_Menus[MenuPos]->_Tipo == DMenuEx_Tipo::DMenuEx_Tipo_Separador) MenuPos--;
				}
				#if DMENUEX_MOSTRARDEBUG == TRUE
					Debug_Escribir(L"DMenuEx::_Evento_TeclaPresionada VK_DOWN\n");
				#endif
				if (MenuPos == -1)	_MenuResaltado = NULL;
				else				_MenuResaltado = _Menus[MenuPos];
//				_AnularMouseMove = TRUE;
				break;
			case VK_UP:
				if (MenuPos > 0)	MenuPos--;
				// Si el menu resaltado tiene un menú anterior
				if (MenuPos > 0) {
					// Si es un separador lo salto
					if (_Menus[MenuPos]->_Tipo == DMenuEx_Tipo::DMenuEx_Tipo_Separador) MenuPos--;
				}
				// Si el menu resaltado no tiene un menu anterior
				else {
					// Si es un separador, vuelvo atras
					if (MenuPos != -1) {
						if (_Menus[MenuPos]->_Tipo == DMenuEx_Tipo::DMenuEx_Tipo_Separador) MenuPos++;
					}
				}
				#if DMENUEX_MOSTRARDEBUG == TRUE
					Debug_Escribir(L"DMenuEx::_Evento_TeclaPresionada VK_UP\n");
				#endif
				if (MenuPos == -1)	_MenuResaltado = NULL;
				else				_MenuResaltado = _Menus[MenuPos];
//				_AnularMouseMove = TRUE;
				break;
			case VK_LEFT :
				if (_Padre != NULL) {
					_MenuResaltado = NULL;
					Ocultar(FALSE);
					SetFocus(_Padre->_hWnd); // REVISADO
					SendMessage(_hWndDest->hWnd(), WM_NCACTIVATE, TRUE, 0);
					Repintar();
				}
				#if DMENUEX_MOSTRARDEBUG == TRUE
					Debug_Escribir(L"DMenuEx::_Evento_TeclaPresionada VK_LEFT\n");
				#endif
				return;
			case VK_RIGHT :
				if (_MenuResaltado != NULL) {
					if (_MenuResaltado->_Menus.size() > 0) {
						//SetFocus(_MenuResaltado->_hWnd);
						//SendMessage(_hWndDest->hWnd(), WM_NCACTIVATE, TRUE, 0);
						//_MenuResaltado->Repintar();
						_MenuResaltado->_MostrarSubMenu(this, RC.right, _MenuResaltado->_Recta.top, FALSE);
						_MenuDesplegado = _MenuResaltado;
						_MenuResaltado->_MenuResaltado = _MenuResaltado->_Menus[0];

					}
				}
				#if DMENUEX_MOSTRARDEBUG == TRUE
					Debug_Escribir(L"DMenuEx::_Evento_TeclaPresionada VK_RIGHT\n");
				#endif
				return;
			case VK_ESCAPE :
				Ocultar(TRUE);
				#if DMENUEX_MOSTRARDEBUG == TRUE
					Debug_Escribir(L"DMenuEx::_Evento_TeclaPresionada VK_ESCAPE\n");
				#endif
				return;
		}

		// Si hay un sub.menu desplegado
		if (_MenuDesplegado != NULL) {
			// Si el menú desplegado no es el mismo que el menú resaltado
			if (_MenuDesplegado != _MenuResaltado) {
				#if DMENUEX_MOSTRARDEBUG == TRUE
					Debug_Escribir(L"DMenuEx::_Evento_TeclaPresionada Ocultar menu desplegado\n");
				#endif
				_MenuDesplegado->Ocultar();
				_MenuDesplegado = NULL;
			}
		}

//		SetFocus(_hWnd);
//		SendMessage(_hWndPadre, WM_NCACTIVATE, TRUE, 0);

		// Miro si hay que desplegar un sub-menú
/*		if (_MenuResaltado != NULL) {
			// Si tiene Sub-menus
			if (_MenuResaltado->TotalMenus() > 0) {
				#if DMENUEX_MOSTRARDEBUG == TRUE
					Debug_Escribir(L"DMenuEx::_Evento_TeclaPresionada MostrarSubmenu sin foco\n");
				#endif
				_MenuResaltado->_MostrarSubMenu(this, RC.right, _MenuResaltado->_Recta.top, FALSE);
				_MenuDesplegado = _MenuResaltado;				
			}
		}*/
		Repintar();

	}

	void DMenuEx::_Evento_TeclaSoltada(WPARAM wParam, LPARAM lParam) {
		DEventoTeclado DatosTeclado(wParam, lParam, this);
		//DhWnd::_Teclado[DatosTeclado.TeclaVirtual()] = false;
	}

	void DMenuEx::_Evento_Tecla(WPARAM wParam, LPARAM lParam) {

	}

	void DMenuEx::_Evento_FocoObtenido(HWND UltimoFoco) {
		wchar_t CN[512];
		GetClassName(UltimoFoco, CN, 512);
		#if DMENUEX_MOSTRARDEBUG == TRUE
			Debug_Escribir_Varg(L"DMenuEx::_Evento_FocoObtenido : %d '%s'\n", UltimoFoco, CN);
		#endif		
	}

	void DMenuEx::_Evento_FocoPerdido(HWND UltimoFoco) {
		wchar_t CN[512];
		GetClassName(UltimoFoco, CN, 512);
		#if DMENUEX_MOSTRARDEBUG == TRUE
			Debug_Escribir_Varg(L"DMenuEx::_Evento_FocoPerdido : %d '%s'\n", UltimoFoco, CN);
		#endif
		LRESULT R = SendMessage(UltimoFoco, WM_ESMENUEX, 0, 0);
		// El nuevo foco no es parte de un MenuEx
		if (R != WM_ESMENUEX) {
			POINT Pt;
			DWL::DMouse::ObtenerPosicion(&Pt);
			HWND VentanaDebajoMouse = WindowFromPoint(Pt);
			// La ventana que hay debajo del ratón no es un MenuEx
			if (SendMessage(VentanaDebajoMouse, WM_ESMENUEX, 0, 0) != WM_ESMENUEX) {
				#if DMENUEX_MOSTRARDEBUG == TRUE
					Debug_Escribir_Varg(L"DMenuEx::GestorMensajes WM_KILLFOCUS Ocultar menu hWnd %d %d\n", UltimoFoco, VentanaDebajoMouse);
				#endif
				Ocultar(TRUE);
			}
		}
	}


	void DMenuEx::_Evento_BarraEx_Cambiando(DEventoMouse &DatosMouse) {
		DMenuEx *MenuBarra = BuscarMenu(DatosMouse.ID());
		if (MenuBarra != NULL) {
			if (MenuBarra->Activado() == TRUE) {
				//_ResultadoModal = MenuBarra;
				SendMessage(_hWndDest->hWnd(), DWL_MENUEX_BARRA_CAMBIANDO, MenuBarra->ID(), static_cast<LPARAM>(static_cast<double>(MenuBarra->_Barra.Valor()) * 100.0f));
			}
		}
	}

	void DMenuEx::_Evento_BarraEx_Cambiado(DEventoMouse &DatosMouse) {
		//Ocultar(TRUE);
		// Aunque parezca un poco raro, al pulsar sobre una barra del menú no desaparecerá el menú, por lo que este debe recuperar el foco
		SetFocus(_hWnd); // REVISADO
		SendMessage(_hWndDest->hWnd(), WM_NCACTIVATE, TRUE, 0);
		DMenuEx *MenuBarra = BuscarMenu(DatosMouse.ID());		
		if (MenuBarra != NULL) {
			if (MenuBarra->Activado() == TRUE) {
				_ResultadoModal = MenuBarra;
				SendMessage(_hWndDest->hWnd(), DWL_MENUEX_BARRA_CAMBIADO, MenuBarra->ID(), static_cast<LPARAM>(static_cast<double>(MenuBarra->_Barra.Valor()) * 100.0f));
			}
		}
	}


	void DMenuEx::_Evento_MouseSaliendo(void) {
		#if DMENUEX_MOSTRARDEBUG == TRUE
			Debug_Escribir(L"DMenuEx::_Evento_MouseSaliendo\n");
		#endif

		_MouseDentro = FALSE;
		if (_MenuResaltado != NULL) {
			POINT Pt;
			DWL::DMouse::ObtenerPosicion(&Pt);
			ScreenToClient(_hWnd, &Pt);
			// Si no está dentro del menú
			if (PtInRect(&_MenuResaltado->_Recta, Pt) == FALSE) {
				if (_MenuResaltado->_Activado == TRUE && _MenuResaltado->_Menus.size() == 0) {
					_MenuResaltado->Transicion(DMenuEx_Transicion::DMenuEx_Transicion_Normal);
				}
				_MenuResaltado = NULL;
			}
		}
/*		if (_MenuDesplegado != NULL) {
			// Obtengo la ventana debajo del mouse
			POINT Pt;
			DWL::DMouse::ObtenerPosicion(&Pt);
			HWND WFP = WindowFromPoint(Pt);
			if (SendMessage(WFP, WM_ESMENUEX, 0, 0) != WM_ESMENUEX) {
				SetFocus(_hWnd);
				SendMessage(_hWndDest->hWnd(), WM_NCACTIVATE, TRUE, 0);
				_MenuDesplegado->Ocultar();
				_MenuDesplegado = NULL;
			}
		}*/

/*		if (_MenuPresionado == NULL) {
			// Obtengo la ventana debajo del mouse
			POINT Pt;
			DWL::DMouse::ObtenerPosicion(&Pt);
			HWND WFP = WindowFromPoint(Pt);
			if (_MenuDesplegado != NULL) {
				// Si la ventana debajo del mouse no es un MenuEx
				if (SendMessage(WFP, WM_ESMENUEX, 0, 0) != WM_ESMENUEX) {
					if (_MenuResaltado != NULL)	_MenuResaltado->Transicion((_Activado == TRUE) ? DMenuEx_Transicion_Normal : DMenuEx_Transicion_Desactivado);
					_MenuResaltado = NULL;
					// TODO : Tanca TOT el menu al sortir...
//					if (_MenuDesplegado->_Padre != NULL) {
					_MenuDesplegado->Ocultar();
					_MenuDesplegado = NULL;
//					}
//					Repintar();
				}
			}
			else {
				// Si la ventana debajo del mouse no es una BarraEx que tiene como padre un MenuEx
				if (SendMessage(GetParent(WFP), WM_ESMENUEX, 0, 0) != WM_ESMENUEX) {
					if (_MenuResaltado != NULL)	_MenuResaltado->Transicion((_Activado == TRUE) ? DMenuEx_Transicion_Normal : DMenuEx_Transicion_Desactivado);
					_MenuResaltado = NULL;
					//Repintar();
				}
			}			
		}*/
	}

	LRESULT CALLBACK DMenuEx::GestorMensajes(UINT uMsg, WPARAM wParam, LPARAM lParam) {
		switch (uMsg) {
		
			// Barra de desplazamiento
			case DWL_BARRAEX_CAMBIANDO:	// Se está modificando (mouse down)
				_Evento_BarraEx_Cambiando(WPARAM_TO_DEVENTOMOUSE(wParam));
				return 0;
			case DWL_BARRAEX_CAMBIADO:  // Se ha modificado	(mouse up)
				_Evento_BarraEx_Cambiado(WPARAM_TO_DEVENTOMOUSE(wParam));
				return 0;


			case WM_PAINT:			_Evento_Pintar();																															return 0;
//			case WM_NCPAINT:		_Evento_PintarNC((HRGN)wParam);																												return 0;
			// Mouse movimiento
			case WM_MOUSEMOVE:		_Evento_MouseMovimiento(wParam, lParam);																									return 0;
			case WM_MOUSELEAVE:     _Evento_MouseSaliendo();																													return 0;
			// Mouse presionado
			case WM_LBUTTONDOWN:	_Evento_MousePresionado(0, wParam, lParam);																									return 0;
			case WM_RBUTTONDOWN:	_Evento_MousePresionado(1, wParam, lParam);																									return 0;
			case WM_MBUTTONDOWN:	_Evento_MousePresionado(2, wParam, lParam);																									return 0;
			// Mouse soltado
			case WM_LBUTTONUP:		_Evento_MouseSoltado(0, wParam, lParam);																									return 0;
			case WM_RBUTTONUP:		_Evento_MouseSoltado(1, wParam, lParam);																									return 0;
			case WM_MBUTTONUP:		_Evento_MouseSoltado(2, wParam, lParam);																									return 0;
			// Teclado (Sin SetFocus no hay teclado)
			case WM_KEYDOWN:		_Evento_TeclaPresionada(wParam, lParam);																									return 0;
			case WM_KEYUP:			_Evento_TeclaSoltada(wParam, lParam);																										return 0;
			case WM_CHAR:           _Evento_Tecla(wParam, lParam);																												return 0;
			// Mouse doble click
/*			case WM_LBUTTONDBLCLK:	_Evento_MouseDobleClick(0, wParam, lParam);																									return 0;
			case WM_RBUTTONDBLCLK:	_Evento_MouseDobleClick(1, wParam, lParam);																									return 0;
			case WM_MBUTTONDBLCLK:	_Evento_MouseDobleClick(2, wParam, lParam);																									return 0;*/

			// Print y Print Client (para AnimateWindow)
			case WM_PRINT:			Pintar(reinterpret_cast<HDC>(wParam));																										return 0;
			case WM_PRINTCLIENT:    Pintar(reinterpret_cast<HDC>(wParam));																										return 0;

			// Si pierde el foco, compruebo que el nuevo foco vaya a otro MenuEx, o en caso contrario oculto todo.
			case WM_SETFOCUS:		_Evento_FocoObtenido(reinterpret_cast<HWND>(wParam));																						return 0;
			// Si pierde el foco, compruebo que el nuevo foco vaya a otro MenuEx, o en caso contrario oculto todo.
			case WM_KILLFOCUS:		_Evento_FocoPerdido(reinterpret_cast<HWND>(wParam));																						return 0;
//			case WM_ACTIVATE :      _Evento_Activar(wParam, lParam);																											return 0;
			// Consulta para determinar si la ventana es parte de un MenuEx
			case WM_ESMENUEX:																																					return WM_ESMENUEX;
			
			// Sombra de la ventana
			// https://stackoverflow.com/questions/43818022/borderless-window-with-drop-shadow
			case WM_NCCALCSIZE:
				if (wParam == TRUE)	{
					// DWL_MSGRESULT (no esta definit)
					SetWindowLongPtr(_hWnd, 0, 0);
					return TRUE;
				}				
				return 0;
			// Evita que al obtener el foco cambie la parte del caption
			case WM_NCACTIVATE:																																					return TRUE;
		}
		return DVentana::GestorMensajes(uMsg, wParam, lParam);
	}

};