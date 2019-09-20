#include "pch.h"
#include "DListaEx.h"
//#include "DListaIconos.h"
#include "DMensajesWnd.h"
#include <assert.h>

namespace DWL {

	#define TIMER_LISTA_DRAG_ARRIBA 1000
	#define TIMER_LISTA_DRAG_ABAJO  1001

	#define MILISEGUNDOS_TIMER_DRAG 200

	DListaEx_Skin::DListaEx_Skin(void) :
		// Colores para el fondo (OJO los colores del fondo y del borde del control están en DBarraSroll_Skin)
		FondoItemNormal						(COLOR_LISTA_FONDO),
		FondoItemResaltado					(COLOR_LISTA_FONDO_RESALTADO),
		FondoItemSeleccionado				(COLOR_LISTA_SELECCION),
		FondoItemSeleccionadoResaltado		(COLOR_LISTA_SELECCION_RESALTADO),
		FondoItemPresionado					(COLOR_LISTA_SELECCION_PRESIONADO),
		// Color para el borde del item marcado
		BordeItemMarcado					(COLOR_LISTA_MARCA_ITEM),
		// Colores para el texto
		TextoItemNormal						(COLOR_LISTA_TEXTO),
		TextoItemResaltado					(COLOR_LISTA_TEXTO_RESALTADO),
		TextoItemSombra						(COLOR_LISTA_TEXTO_SOMBRA),
		TextoItemSeleccionado				(COLOR_LISTA_SELECCION_TEXTO),
		TextoItemSeleccionadoSombra			(COLOR_LISTA_SELECCION_TEXTO_SOMBRA),
		TextoItemSeleccionadoResaltado		(COLOR_LISTA_SELECCION_TEXTO_RESALTADO),
		TextoItemPresionado					(COLOR_LISTA_SELECCION_TEXTO_PRESIONADO),

		// Fuente
		FuenteTam							(FUENTE_NORMAL),
		FuenteNombre						(FUENTE_NOMBRE),
		FuenteNegrita						(FALSE),
		FuenteCursiva						(FALSE),
		FuenteSubrayado						(FALSE),
		FuenteSombraTexto					(FALSE) { 
	}



	DListaEx::DListaEx(void) :  DBarraScrollEx()		, _ItemPaginaInicio(0)		, _ItemPaginaFin(0)			, _ItemPaginaVDif(0)		, _ItemPaginaHDif(0),
								_SubItemResaltado(-1)	, _SubItemUResaltado(-1)	, _SubItemPresionado(-1)	, MostrarSeleccion(TRUE)	, MultiSeleccion(FALSE) , MoverItemsDrag(FALSE),
								_ItemResaltado(-1)		, _ItemUResaltado(-1)		, _ItemMarcado(-1)			, _PintarIconos(TRUE),	    
								_ItemPresionado(-1)		, _ItemShift(-1)			, _Repintar(FALSE)			, _TimerDragArriba(0)       , _TimerDragAbajo(0),
								_TotalAnchoVisible(0)	, _TotalAltoVisible(0)		, _TiempoItemPresionado(0)  , _PItemPresionado(NULL)    , _EnDrag(FALSE),
								_BufferItem(NULL)		, _BufferItemBmp(NULL)		, _BufferItemBmpViejo(NULL)	, _BufferItemFuenteViejo(NULL) {

	}


	DListaEx::~DListaEx(void) {
		EliminarTodasLasColumnas();
		EliminarTodosLosItems();
	}

	
	HWND DListaEx::CrearListaEx(DhWnd *nPadre, const int cX, const int cY, const int cAncho, const int cAlto, const int cID, const DWORD Estilos, const DWORD EstilosExtendidos) {
//		if (hWnd()) { Debug_Escribir(L"DListaEx::CrearListaEx() Error : ya se ha creado la lista\n"); return hWnd(); }
		Fuente.CrearFuente(Skin.FuenteTam, Skin.FuenteNombre.c_str(), Skin.FuenteNegrita, Skin.FuenteCursiva, Skin.FuenteSubrayado);
		_hWnd = CrearControlEx(nPadre, L"DListaEx", L"", cID, cX, cY, cAncho, cAlto, Estilos, EstilosExtendidos, CS_DBLCLKS); // CS_DBLCLKS (el control recibe notificaciones de doble click)
		_Repintar = TRUE;
		return hWnd();
	}
	
	DListaEx_Columna *DListaEx::AgregarColumna(const int nAncho, DListaEx_Columna_Alineacion nAlineacion) {
		DListaEx_Columna *nColumna = new DListaEx_Columna(nAlineacion, nAncho);
		_Columnas.push_back(nColumna);
		_Repintar = TRUE;
		return nColumna;
	}


	DListaEx_Item* DListaEx::_AgregarItem(DListaEx_Item *nItem, DIcono &nIcono, const INT_PTR PosicionItem, std::initializer_list<std::wstring> Textos) {
		nItem->_Lista = this;
		nItem->_ColorTexto		 = Skin.TextoItemNormal;
		nItem->_ColorTextoSombra = Skin.TextoItemSombra;
		nItem->_ColorFondo		 = Skin.FondoItemNormal;
		nItem->_Icono = nIcono;

		// Agrego todos los textos de cada columna
		DListaEx_SubItem *nSubItem = NULL;
		for (auto Elemento : Textos) {
			nSubItem = new DListaEx_SubItem(Elemento.c_str());
			nItem->_SubItems.push_back(nSubItem);
		}

		if		(PosicionItem == -1)	_Items.push_back(nItem);
		else if (PosicionItem == -2)	_Items.push_back(nItem);	// TODO (-2 es ordenado)
		else							_Items.insert(_Items.begin() + PosicionItem, nItem);

		_Repintar = TRUE;

		return nItem;
	}

	/*
	DListaEx_Item *DListaEx::_AgregarItem(DListaEx_Item *nItem, DIcono &nIcono, const INT_PTR PosicionItem, const TCHAR *nTxt, va_list Marker) {
//		DListaEx_Item		*nItem		= new DListaEx_Item();
		DListaEx_SubItem	*nSubItem 	= new DListaEx_SubItem(nTxt);
		nItem->_SubItems.push_back(nSubItem);
		nItem->_Lista = this;
		nItem->_Icono = nIcono;

		// Obtengo los textos de cada columna
		for (size_t i = 1; i < _Columnas.size(); i++) {
			nSubItem = new DListaEx_SubItem(static_cast<const TCHAR *>(va_arg(Marker, const TCHAR *)));
			nItem->_SubItems.push_back(nSubItem);
		}
		
		if		(PosicionItem == -1)	_Items.push_back(nItem);
		else if (PosicionItem == -2)	_Items.push_back(nItem);	// TODO (-2 es ordenado)
		else							_Items.insert(_Items.begin() + PosicionItem, nItem);
		
		_Repintar = TRUE;
//		_CalcularScrolls();

		return nItem;
	}*/

	void DListaEx::EliminarTodosLosItems(void) {
		for (size_t i = 0; i < _Items.size(); i++) {
			delete _Items[i];
		}
		_Items.resize(0);
		_ItemResaltado	= -1;
		_ItemMarcado	= -1;
		_ItemPresionado = -1;
		_Repintar = TRUE;
	}

	const LONG_PTR DListaEx::EliminarItemsSeleccionados(void) {
		LONG_PTR Ret = 0;
		for (LONG_PTR i = _Items.size() - 1; i > 0; i--) {
			if (_Items[i]->Seleccionado == TRUE) {
				EliminarItem(i);
				Ret++;
			}
		}
		return Ret;
	}

	void DListaEx::ActualizarSkin(void) {
		DBarraScrollEx::ActualizarSkin();
		for (size_t i = 0; i < _Items.size(); i++) {
			if (_Items[i]->Seleccionado == TRUE) {
				_Items[i]->_ColorTexto			= Skin.TextoItemSeleccionado;
				_Items[i]->_ColorTextoSombra	= Skin.TextoItemSeleccionadoSombra;
				_Items[i]->_ColorFondo			= Skin.FondoItemSeleccionado;
			}
			else {
				_Items[i]->_ColorTexto			= Skin.TextoItemNormal;
				_Items[i]->_ColorTextoSombra	= Skin.TextoItemSombra;
				_Items[i]->_ColorFondo			= Skin.FondoItemNormal;
			}
		}
	}


	void DListaEx::Repintar(const BOOL nForzar) {
		if (_Repintar == FALSE)	_Repintar = nForzar;
		if (IsWindowVisible(_hWnd) == FALSE) return;
		RedrawWindow(hWnd(), NULL, NULL, RDW_INVALIDATE | RDW_INTERNALPAINT); 
		#if DLISTAEX_MOSTRARDEBUG == TRUE
			Debug_Escribir_Varg(L"DListaEx::Repintar\n", _Repintar);
		#endif
	};

	void DListaEx::EliminarTodasLasColumnas(void) {
		for (size_t i = 0; i < _Columnas.size(); i++) {
			delete _Columnas[i];
		}
		_Columnas.resize(0);
		_Repintar = TRUE;
	}

	void DListaEx::Pintar(HDC hDC) {
/*		#if DLISTAEX_MOSTRARDEBUG == TRUE
			Debug_Escribir_Varg(L"DListaEx::Pintar -> _Repintar = %d\n", _Repintar);
		#endif*/
		if (_Repintar == TRUE) {
			_CalcularScrolls();
		}

		RECT	RC, RCS, RCB;
		ObtenerRectaCliente(&RC, &RCS, &RCB);
		//		GetClientRect(_hWnd, &RCB);

		HDC		Buffer = CreateCompatibleDC(hDC);
		HBITMAP Bmp = CreateCompatibleBitmap(hDC, RCB.right - RCB.left, RCB.bottom - RCB.top);
		HBITMAP BmpViejo = static_cast<HBITMAP>(SelectObject(Buffer, Bmp));

		// Pinto el fondo
		HBRUSH BFondo = CreateSolidBrush(_ColorFondo);
		FillRect(Buffer, &RCB, BFondo);
		DeleteObject(BFondo);


		// Diferencia de pixeles Vertical (puede ser negativo si el primer item es parcialmente visible, o 0 si el primer item está justo al principio del área visible)
		LONG DifInicioV = _ItemPaginaVDif;

		LONG_PTR nItemFin = _ItemPaginaFin;
		if (nItemFin == -1) nItemFin = _Items.size() - 1;

		RECT RectaItem;
		LONG nAlto = Fuente.Alto() + (DLISTAEX_PADDING * 2);
		// _ItemPaginaInicio es unsigned -1 se refiere al valor máximo en una variable del tipo size_t
		if (_ItemPaginaInicio != -1) {
			for (LONG_PTR i = _ItemPaginaInicio; i < nItemFin + 1; i++) {
				// Si RCS.right y RC.right son iguales, es que no hay scroll vertical, y le sumo 2 al ancho para ajustarme al borde 
				RectaItem = { 0, DifInicioV, (RCS.right != RC.right) ? RCS.right : RCS.right + 2, DifInicioV + nAlto };
				PintarItem(Buffer, i, RectaItem);
				DifInicioV += nAlto;							// Sumo la altura del nodo a la diferencia inicial
			}
		}

		PintarBorde(&RCB, Buffer);

		// Pinto las barras de scroll en el buffer
		Scrolls_Pintar(Buffer, RC);

		// Pinto el buffer en el DC
		BitBlt(hDC, RCB.left, RCB.top, RCB.right, RCB.bottom, Buffer, 0, 0, SRCCOPY);

		// Elimino objetos gdi de la memoria
		SelectObject(Buffer, BmpViejo);
		DeleteObject(Bmp);
		DeleteDC(Buffer);
	}

	void DListaEx::PintarItem(HDC hDC, const LONG_PTR nPosItem, RECT &Espacio) {
		if (_Items.size() == 0) return;

		BOOL bResaltado = (nPosItem == _ItemResaltado) ? TRUE : FALSE;
		BOOL bPresionado = (nPosItem == _ItemPresionado) ? TRUE : FALSE;

		// Creo la brocha para el fondo (si el color del fondo del nodo es igual al color del fondo normal o resaltado de la barra de scroll, creo la brocha con el color del fondo del control. En caso contrario creo la brocha con el color que especifica el nodo)
		HBRUSH BrochaFondo = NULL;
		if (_Items[nPosItem]->_ColorFondo != SkinScroll.FondoNormal && _Items[nPosItem]->_ColorFondo != SkinScroll.FondoResaltado) 	BrochaFondo = CreateSolidBrush(_Items[nPosItem]->_ColorFondo);
		else																														BrochaFondo = CreateSolidBrush(_ColorFondo); 
		
		// Pinto el fondo del buffer
		RECT EspacioLocal = { 0 , 0, (static_cast<LONG>(_TotalAnchoVisible) > Espacio.right) ? static_cast<LONG>(_TotalAnchoVisible) : Espacio.right, (2 * DLISTAEX_PADDING) + Fuente.Alto() };
		FillRect(_BufferItem, &EspacioLocal, BrochaFondo);
		DeleteObject(BrochaFondo);

		// Pinto el icono
		if (_PintarIconos == TRUE) {
			if (_Items[static_cast<unsigned int>(nPosItem)]->_Icono() != NULL) {
				int PosYIco = ((Espacio.bottom - Espacio.top) - DLISTAEX_TAMICONO) / 2;
				DrawIconEx(_BufferItem, bPresionado + DLISTAEX_PADDING, bPresionado + PosYIco, _Items[static_cast<unsigned int>(nPosItem)]->_Icono(), DLISTAEX_TAMICONO, DLISTAEX_TAMICONO, 0, 0, DI_NORMAL);
			}
		}
		RECT RCelda;
//		LONG AnchoPintado = (DLISTAEX_PADDING * 2) + DLISTAEX_TAMICONO;
		LONG AnchoPintado = 0;
		//	Recorro todos los sub-items
		for (size_t i = 0; i < _Columnas.size(); i++) {
			RCelda = {
				1 + bPresionado + AnchoPintado + DLISTAEX_PADDING, 
				1 + bPresionado + DLISTAEX_PADDING,
				1 + bPresionado + AnchoPintado + _Columnas[i]->_AnchoCalculado - (DLISTAEX_PADDING * 2),
				1 + bPresionado + Fuente.Alto() + DLISTAEX_PADDING
			};
			if (i == 0 && _PintarIconos == TRUE) { // La primera columna contiene el icono (que ya se ha pintado)
				RCelda.left += (DLISTAEX_PADDING * 2) + DLISTAEX_TAMICONO;
			}
			// Si hay texto lo pinto
			if (_Items[static_cast<unsigned int>(nPosItem)]->Texto(i).size() > 0) {
				// Pinto la sombra
				if (Skin.FuenteSombraTexto == TRUE) {
					SetTextColor(_BufferItem, _Items[nPosItem]->_ColorTextoSombra);
					DrawText(_BufferItem, _Items[static_cast<unsigned int>(nPosItem)]->Texto(i).c_str(), static_cast<int>(_Items[static_cast<unsigned int>(nPosItem)]->Texto(i).size()), &RCelda, _Columnas[i]->Alineacion | DT_NOPREFIX);
				}
				// Pinto el texto
				SetTextColor(_BufferItem, _Items[nPosItem]->_ColorTexto);
				OffsetRect(&RCelda, -1, -1);
				DrawText(_BufferItem, _Items[static_cast<unsigned int>(nPosItem)]->Texto(i).c_str(), static_cast<int>(_Items[static_cast<unsigned int>(nPosItem)]->Texto(i).size()), &RCelda, _Columnas[i]->Alineacion | DT_NOPREFIX);
			}
			// Llamo al evento virtual para pintar el subitem (para extender el control y pintar iconos por ejemplo)
			Evento_PintarSubItem(_BufferItem, nPosItem, i, &RCelda);

			AnchoPintado += _Columnas[i]->_AnchoCalculado;
		}

		if (nPosItem == _ItemMarcado) {
			// Pinto la marca del foco del teclado
			HBRUSH BrochaMarcaItem = CreateSolidBrush(Skin.BordeItemMarcado);
			RECT RFondo = { 0, 0, Espacio.right - Espacio.left, Espacio.bottom - Espacio.top };
			FrameRect(_BufferItem, &RFondo, BrochaMarcaItem);
			DeleteObject(BrochaMarcaItem);
		}

		// Pinto el resultado del buffer al DC de la función pintar
		BitBlt(hDC, Espacio.left, Espacio.top, Espacio.right, Espacio.bottom - Espacio.top, _BufferItem, -_ItemPaginaHDif, 0, SRCCOPY);

	}

	/* Evento que se recibe al cambiar el scroll de posición */
	void DListaEx::Scrolls_EventoCambioPosicion(void) {
		RECT RC, RCS, RCB;
		//		GetClientRect(hWnd(), &RC);
		ObtenerRectaCliente(&RC, &RCS, &RCB);
		_CalcularItemsPagina(RCS.bottom);
	}


	/* Función que determina el item que hay debajo de las coordenadas especificadas 
		[in]  cX			: Coordenada X
		[in]  cY			: Coordenada Y
		[out] nPosSubItem	: Si no es NULL devolverá la posición del SubItem
	*/
	const LONG_PTR DListaEx::HitTest(const int cX, const int cY, LONG_PTR*nPosSubItem) {
		if (_ItemPaginaInicio == -1) {
/*			#if DLISTAEX_MOSTRARDEBUG == TRUE
				Debug_Escribir_Varg(L"DListaEx::HitTest X:%d Y:%d I:-1 SI:-1\n", cX, cY);
			#endif*/
			return -1;
		}

		int PixelesContados		= _ItemPaginaVDif; // Pixeles de altura contados hasta el nodo
		int AltoItem			= Fuente.Alto() + (DLISTAEX_PADDING * 2);
		int PixelesContadosH	= _ItemPaginaHDif;
		LONG_PTR PosInicio		= _ItemPaginaInicio;
		LONG_PTR PosFin			= _ItemPaginaFin + 1;
		
		if (PosFin == 0) PosFin = _Items.size();
		for (LONG_PTR i = PosInicio; i < PosFin; i++) {
			// El item está en las coordenadas del mouse
			if (PixelesContados <= cY && PixelesContados + AltoItem >= cY) {				
				// Si PosSubItem no es NULL necesitamos determinar la posición del SubItem
				if (nPosSubItem != NULL) {
					for (size_t si = 0; si < _Columnas.size(); si++) {

//						if (si == 0) { PixelesContadosH += (DLISTAEX_PADDING * 2) + DLISTAEX_TAMICONO; } // Si es el primer subitem, incluyo el tamaño del icono al ancho contado.

						if (PixelesContadosH <= cX && PixelesContadosH + _Columnas[si]->_AnchoCalculado >= cX) {
							*nPosSubItem = si;
/*							#if DLISTAEX_MOSTRARDEBUG == TRUE
								Debug_Escribir_Varg(L"DListaEx::HitTest X:%d Y:%d I:%d SI:%d\n", cX, cY, i, si);
							#endif*/
							return i;
						}
						PixelesContadosH += _Columnas[si]->_AnchoCalculado;
					}
				}
				else { // Solo se busca el item
/*					#if DLISTAEX_MOSTRARDEBUG == TRUE
						Debug_Escribir_Varg(L"DListaEx::HitTest X:%d Y:%d I:%d\n", cX, cY, i);
					#endif*/
					return i;
				}
			}
			PixelesContados += AltoItem;
		}

/*		#if DLISTAEX_MOSTRARDEBUG == TRUE
			Debug_Escribir_Varg(L"DListaEx::HitTest X:%d Y:%d I:-1\n", cX, cY);
		#endif*/
		return -1;
	}


	void DListaEx::EliminarItem(DListaEx_Item *eItem) {
		for (size_t i = 0; i < _Items.size(); i++) {
			if (_Items[i] == eItem) {
				EliminarItem(i);
				return;
			}
		}
	}

	void DListaEx::EliminarItem(const LONG_PTR ePos) {
		if (ePos < static_cast<LONG_PTR>(_Items.size())) {
			delete _Items[static_cast<unsigned int>(ePos)];
			_Items.erase(_Items.begin() + static_cast<unsigned int>(ePos));
			if (_ItemResaltado >= static_cast<LONG_PTR>(_Items.size())) {
				_ItemResaltado  = -1;
				_ItemUResaltado = -1;
			}
			_ItemMarcado        = -1;
			_ItemPresionado		= -1;
			_SubItemPresionado	= -1;
			_ItemResaltado		= -1;
			_SubItemResaltado	= -1;
			_ItemShift			= -1;
			// Recalculo todos los valores de la lista antes de repintar
			_Repintar = TRUE;
		}
	}

	void DListaEx::DesSeleccionarTodo(void) {
		#if DLISTAEX_MOSTRARDEBUG == TRUE
			Debug_Escribir(L"DListaEx::DesSeleccionarTodo\n");
		#endif

		for (size_t i = 0; i < _Items.size(); i++) {
			SeleccionarItem(_Items[i], FALSE);
/*			_Items[i]->_AniTransicion.Terminar();
			_Items[i]->Seleccionado = FALSE;
			_Items[i]->_ColorFondo			= _ColorFondo;
			_Items[i]->_ColorTexto			= Skin.TextoItemNormal;
			_Items[i]->_ColorTextoSombra	= Skin.TextoItemSombra;*/
		}
	}

	void DListaEx::MostrarItem(DListaEx_Item *mItem) {
		for (size_t i = 0; i < _Items.size(); i++) {
			if (_Items[i] == mItem) {
				MostrarItem(i);
				return;
			}
		}
	}

	void DListaEx::MostrarItem(const LONG_PTR iPos, const BOOL nRepintar) {
		if (iPos >= static_cast<LONG_PTR>(_Items.size()) || _Items.size() == 0) return;

		RECT RItem;
		if (ObtenerRectaItem(iPos, RItem) == FALSE) return;

//		int nAltoItem = _Fuente.Alto() + (LISTAEX_PADDING * 2);
		
		// Obtengo la recta absoluta visible
		RECT RC, RAV, RCB;
		ObtenerRectaCliente(&RC, &RAV, &RCB);
		
		// Sumo a la recta RAV las posiciones de los Scrolls V y H
		float ancho = static_cast<float>(_TotalAnchoVisible) - (static_cast<float>(_TotalAnchoVisible) * _ScrollH_Pagina);
		float alto = static_cast<float>(_TotalAltoVisible) - (static_cast<float>(_TotalAltoVisible) * _ScrollV_Pagina);

		LONG YInicio = static_cast<LONG>(alto * _ScrollV_Posicion);
		LONG XInicio = static_cast<LONG>(ancho * _ScrollH_Posicion);
		OffsetRect(&RAV, XInicio, YInicio);

		if (RItem.left < RAV.left) {			// Hay una parte a la izquierda del item que no es visible (lateral)
//			assert(ancho != 0.0f);
			_ScrollH_Posicion = (1.0f / ancho) * static_cast<float>(RItem.left);
		}

		if (RItem.top < RAV.top) {				// Hay una parte del item que no es visible (por arriba)
//			assert(alto != 0.0f);
			_ScrollV_Posicion = (1.0f / alto) * static_cast<float>(RItem.top);
		}
		else if (RItem.bottom > RAV.bottom) {	// Hay una parte del item que no es visible (por abajo)
												// Sumo la diferencia de RItem.bottom + RAV.bottom a la posición del ScrollV
//			assert(alto != 0.0f);
			_ScrollV_Posicion += (1.0f / alto) * static_cast<float>(RItem.bottom - RAV.bottom);
		}

		// Calculo los nodos InicioPagina y FinPagina
		_CalcularScrolls();
//		_CalcularItemsPagina(RAV.bottom - RAV.top);

		// Repinto el control
		if (nRepintar == TRUE) Repintar();
	}

	const LONG_PTR DListaEx::ItemPos(DListaEx_Item *pItem) {
		if (pItem == NULL) return -1;
		return std::find(_Items.begin(), _Items.end(), pItem) - _Items.begin();
	}

	void DListaEx::ItemMarcado(DListaEx_Item *NuevoItemMarcado, const BOOL nRepintar) {
		for (size_t i = 0; i < _Items.size(); i++) {
			if (NuevoItemMarcado == _Items[i]) {
				_ItemMarcado = i;
				break;
			}
		}
		if (nRepintar == TRUE) Repintar();
	}


	const BOOL DListaEx::Destruir(void) {
		EliminarTodasLasColumnas();
		EliminarTodosLosItems();
		return DControlEx::Destruir();
	}

	
	// Obtiene la recta absoluta del item
	const BOOL DListaEx::ObtenerRectaItem(const LONG_PTR iPos, RECT &rRecta) {
		if (_Items.size() == 0 || iPos >= static_cast<LONG_PTR>(_Items.size())) return FALSE;
		LONG nAncho = 0;
		for (size_t i = 0; i < _Columnas.size(); i++) nAncho += _Columnas[i]->_AnchoCalculado;		

		const int nAltoItem = Fuente.Alto() + (DLISTAEX_PADDING * 2);
		rRecta.left   = 2;
		rRecta.top    = 2 + nAltoItem * static_cast<LONG>(iPos);
		rRecta.right  = nAncho;
		rRecta.bottom = 2 + nAltoItem * (static_cast<LONG>(iPos) + 1);
		return TRUE;
	}

	// Calcula los items que se ven actualmente en la pantalla
	void DListaEx::_CalcularItemsPagina(const size_t TamPagina) {
		_ItemPaginaInicio = -1;
		_ItemPaginaFin = -1;
		if (_Items.size() == 0 || _Columnas.size() == 0) {
			#if DLISTAEX_MOSTRARDEBUG == TRUE
				Debug_Escribir_Varg(L"DListaEx::_CalcularItemsPagina I:%d F:%d\n", static_cast<__int64>(_ItemPaginaInicio), static_cast<__int64>(_ItemPaginaFin));
			#endif
			return;
		}

		LONG   PixelInicio			= 0; 
		LONG   PixelFin				= static_cast<LONG>(TamPagina);

		float ancho = static_cast<float>(_TotalAnchoVisible) - (static_cast<float>(_TotalAnchoVisible) * _ScrollH_Pagina);
		float alto = static_cast<float>(_TotalAltoVisible) - (static_cast<float>(_TotalAltoVisible) * _ScrollV_Pagina);

		
		LONG   TotalPixelesContados = -static_cast<LONG>(static_cast<float>(alto) * _ScrollV_Posicion);

		_ItemPaginaHDif = -static_cast<LONG>(static_cast<float>(ancho) * _ScrollH_Posicion);

		LONG   nAltoItem = Fuente.Alto() + (DLISTAEX_PADDING * 2);

		for (size_t i = 0; i < _Items.size(); i++) {
			if (_ItemPaginaInicio == -1) { // Item inicial
				if (TotalPixelesContados <= PixelInicio && TotalPixelesContados + nAltoItem >= PixelInicio) {
					_ItemPaginaVDif = static_cast<LONG>(TotalPixelesContados - PixelInicio);
					_ItemPaginaInicio = i;
				}
			}
			else {							// Item final
				if (TotalPixelesContados >= PixelFin - nAltoItem) {
					_ItemPaginaFin = i;

					#if DLISTAEX_MOSTRARDEBUG == TRUE
						Debug_Escribir_Varg(L"DListaEx::_CalcularItemsPagina I:%d F:%d\n", static_cast<__int64>(_ItemPaginaInicio), static_cast<__int64>(_ItemPaginaFin));
					#endif

					return;
				}
			}
			TotalPixelesContados += nAltoItem;
		}

		#if DLISTAEX_MOSTRARDEBUG == TRUE
			Debug_Escribir_Varg(L"DListaEx::_CalcularItemsPagina I:%d F:%d\n", static_cast<__int64>(_ItemPaginaInicio), static_cast<__int64>(_ItemPaginaFin));
		#endif
	}

	// Calcula el total de espacio requerido para mostrar toda la lista en pixeles (además también calcula el tamaño de las columnas con ancho automático)
	void DListaEx::_CalcularTotalEspacioVisible(RECT &RC) {
		_TotalAltoVisible	= 0;
		_TotalAnchoVisible	= 0;
		if (_Items.size() == 0 || _Columnas.size() == 0) return;
		// Calculo el ancho de las columnas fijas, y cuento las columnas automáticas
		UINT ColumnasAuto = 0;
		INT  nAnchoVisible	= (DLISTAEX_PADDING *2) + DLISTAEX_TAMICONO;
		for (size_t i = 0; i < _Columnas.size(); i++) {
			if (_Columnas[i]->Ancho != DLISTAEX_COLUMNA_ANCHO_AUTO) {	nAnchoVisible += _Columnas[i]->Ancho;		}
			else													{	ColumnasAuto++;								}
		}
		// Hay una o mas columnas con el ancho automático
		if (ColumnasAuto > 0) {
			_TotalAnchoVisible = RC.right;
			// Calculo el ancho para las columnas que tiene ancho automático
			//  En principio está pensado para una sola columna con ancho automático, pero si hay mas de una se dividirá ese espacio equitativamente entre las columnas automáticas
/*			for (UINT c = 0; c < _Columnas.size(); c++) {
				if (_Columnas[c]->Ancho != DLISTAEX_COLUMNA_ANCHO_AUTO) {	_Columnas[c]->_AnchoCalculado = _Columnas[c]->Ancho;														} // Ancho sobrante dividido por el número de columnas automáticas
				else													{	_Columnas[c]->_AnchoCalculado = static_cast<LONG>((_TotalAnchoVisible - nAnchoVisible) / ColumnasAuto);		} // Ancho en pixeles
			}*/
		}
		else {
			if (nAnchoVisible >= RC.right)	_TotalAnchoVisible = nAnchoVisible;
			else                            _TotalAnchoVisible = RC.right;
		}

		// Altura (total de items * altura de la fuente)
		_TotalAltoVisible = (Fuente.Alto() + (DLISTAEX_PADDING * 2)) * _Items.size();
	}


	void DListaEx::_CalcularColumnas(void) {
		INT		nAnchoFijo = DLISTAEX_PADDING;
//		INT		nAnchoFijo = (_PintarIconos == TRUE) ? (DLISTAEX_PADDING * 2) + DLISTAEX_TAMICONO : DLISTAEX_PADDING;
		UINT	ColumnasAuto	= 0;
		size_t  i               = 0;
		// Cuento el ancho fijo y las columnas con ancho automático
		for (i = 0; i < _Columnas.size(); i++) {
			if (_Columnas[i]->Ancho != DLISTAEX_COLUMNA_ANCHO_AUTO) {	nAnchoFijo += _Columnas[i]->Ancho;		}
			else													{	ColumnasAuto++;							}
		}
		RECT RC, RCSS, RCB;
		ObtenerRectaCliente(&RC, &RCSS, &RCB);
		// Asigno el AnchoCalculado a todas las columnas
		#if DLISTAEX_MOSTRARDEBUG == TRUE
			Debug_Escribir_Varg(L"DListaEx::_CalcularColumnas Total : %d, Cols { ", RCSS.right);
		#endif
		for (i = 0; i < _Columnas.size(); i++) {
			if (_Columnas[i]->Ancho != DLISTAEX_COLUMNA_ANCHO_AUTO) {	
				_Columnas[i]->_AnchoCalculado = _Columnas[i]->Ancho;	// Ancho en pixeles
			} 
			else {					
				_Columnas[i]->_AnchoCalculado = static_cast<LONG>((RCSS.right - nAnchoFijo) / ColumnasAuto);	// Ancho sobrante dividido por el número de columnas automáticas
				_Columnas[i]->_AnchoCalculado = 2 + static_cast<LONG>((RCSS.right - nAnchoFijo) / ColumnasAuto);	// Ancho sobrante dividido por el número de columnas automáticas
				// Si es la primera columna, añado el ancho del icono al tamaño auto
				if (i == 0 && _PintarIconos == TRUE) {
					_Columnas[i]->_AnchoCalculado += DLISTAEX_PADDING + DLISTAEX_TAMICONO;
					nAnchoFijo += DLISTAEX_PADDING + DLISTAEX_TAMICONO;
				}
			}
			#if DLISTAEX_MOSTRARDEBUG == TRUE
				Debug_EscribirSinMS_Varg(L"%d -> %d, ", i, _Columnas[i]->_AnchoCalculado);
			#endif
		}

		// No hay scroll, reajusto el espacio de la ultima columna
/*		if (RC.right == RCSS.right && _Columnas.size() != 0) {
			_Columnas[_Columnas.size() - 1]->_AnchoCalculado += 4;
		}*/

		#if DLISTAEX_MOSTRARDEBUG == TRUE
			Debug_EscribirSinMS(L"}\n");
		#endif
	}


	// Calcula si hay que mostrar los scrolls V y H, sus tamaños de página y posiciones relativas, y además crea el buffer para pintar un item una vez determinados los tamaños de scroll
	void DListaEx::_CalcularScrolls(void) {
		_Repintar = FALSE;

		RECT RC;
		GetClientRect(hWnd(), &RC);
		_CalcularTotalEspacioVisible(RC);

		BOOL SV = FALSE, SH = FALSE;
		// Determino si se necesita scroll horizontal
		if (RC.right >= static_cast<LONG>(_TotalAnchoVisible))	{ SH = FALSE; }
		else													{ SH = TRUE;	RC.bottom -= _ScrollH_Alto; }
		// Determino si se necesita scroll vertical
		if (RC.bottom >= static_cast<LONG>(_TotalAltoVisible))	{ SV = FALSE; }
		else													{ SV = TRUE;	RC.right -= _ScrollV_Ancho; }

		// Borro y vuelvo a crear el buffer DC para pintar los nodos
		_CrearBufferItem((static_cast<LONG>(_TotalAnchoVisible) > RC.right) ? static_cast<int>(_TotalAnchoVisible) : static_cast<int>(RC.right), Fuente.Alto() + (DLISTAEX_PADDING *2) );

		// Calculo el ancho máximo
		if (SH == FALSE) {
			_ScrollH_Pagina = 1.0f;
		}
		else {
			// 1.0 es el total
			_ScrollH_Pagina = (1.0f / _TotalAnchoVisible) * static_cast<float>(RC.right);

//			_ScrollH_Pagina = ((static_cast<float>(RC.right) / static_cast<float>(_TotalAnchoVisible)) * 100.0f);
			// Si la suma de la posición del scroll y la pagina son más grandes que 100% 
			if (_ScrollH_Posicion > 1.0f) {
				_ScrollH_Posicion = 1.0f; // Ajusto la posición del scroll para que sumada con la página sea el 100%
			}
		}
		ScrollH_Visible(SH);

		// Calculo la altura máxima
		if (SV == FALSE) {
			_ScrollV_Pagina = 1.0f;
		}
		else {
			_ScrollV_Pagina = (1.0f / _TotalAltoVisible) * static_cast<float>(RC.bottom);
//			_ScrollV_Pagina = ((static_cast<float>(RC.bottom) / static_cast<float>(_TotalAltoVisible)) * 100.0f);
			// Si la suma de la posición del scroll y la pagina son más grandes que 100% 
			if (_ScrollV_Posicion > 1.0f) {
				_ScrollV_Posicion = 1.0f; // Ajusto la posición del scroll para que sumada con la página sea el 100%
			}
		}
		ScrollV_Visible(SV);

		_CalcularPintarIconos();
		_CalcularColumnas();
		_CalcularItemsPagina(RC.bottom);
	}




	void DListaEx::_CrearBufferItem(const int nAncho, const int nAlto) {
		if (_BufferItem != NULL) {
			SelectObject(_BufferItem, _BufferItemFuenteViejo);
			SelectObject(_BufferItem, _BufferItemBmpViejo);
			DeleteObject(_BufferItemBmp);
			DeleteDC(_BufferItem);
		}
		HDC hDC = GetDC(NULL);
		_BufferItem				= CreateCompatibleDC(NULL);
		_BufferItemBmp			= CreateCompatibleBitmap(hDC, nAncho, nAlto);
		_BufferItemBmpViejo		= static_cast<HBITMAP>(SelectObject(_BufferItem, _BufferItemBmp));
		_BufferItemFuenteViejo	= static_cast<HFONT>(SelectObject(_BufferItem, Fuente()));
		ReleaseDC(NULL, hDC);
		SetBkMode(_BufferItem, TRANSPARENT);
	}

	void DListaEx::_Evento_Pintar(void) {		
		PAINTSTRUCT PS;
		HDC DC = BeginPaint(hWnd(), &PS);
		Pintar(DC);
		EndPaint(hWnd(), &PS);
	}

	void DListaEx::_Evento_MouseMovimiento(WPARAM wParam, LPARAM lParam) {
		DEventoMouse DatosMouse(wParam, lParam, this);
//		_mX = cX; _mY = cY;
		// Utilizo la función _MouseEntrando() para poder recibir los mensajes WM_MOUSELEAVE
		if (_MouseEntrando() == TRUE) {
			Evento_MouseEntrando();
			Scrolls_MouseEntrando();
		}
		
		if (Scrolls_MouseMovimiento(DatosMouse) == TRUE) { return; } // las coordenadas pertenecen al scroll (salgo del evento)
		
		_ItemResaltado = HitTest(DatosMouse.X(), DatosMouse.Y(), &_SubItemResaltado);
		
		Evento_MouseMovimiento(DatosMouse);

		// Envio el evento mouseup a la ventana padre
		SendMessage(GetParent(hWnd()), DWL_LISTAEX_MOUSEMOVIMIENTO, reinterpret_cast<WPARAM>(&DatosMouse), 0);

		if (_ItemPresionado == -1) {
			// Comprueba si el item resaltado es el mismo que la ultima vez, y si no lo és repinta el control
			if (_ItemResaltado != _ItemUResaltado || _SubItemResaltado != _SubItemUResaltado) {

				if (_ItemUResaltado > -1) {
					if (_Items.size() > 0) _Items[_ItemUResaltado]->_TransicionNormal();
				}
				if (_ItemResaltado > -1) {
					if (_Items.size() > 0) _Items[_ItemResaltado]->_TransicionResaltado();
				}
				_ItemUResaltado = _ItemResaltado;
				_SubItemUResaltado = _SubItemResaltado;
				Repintar();
			}
		}
		// Hay un item presionado (Drag & Drop)
		else {			
			if (MoverItemsDrag == TRUE) {
				// Se ha sobrepasado el tiempo para un click simple, es una operación drag
				if (GetTickCount64() > _TiempoItemPresionado + 300) {
					_EnDrag = TRUE;
					_Drag(DatosMouse);
				}
			}
		}
	}

	void DListaEx::_Evento_Temporizador(WPARAM wParam) {
		LONG_PTR i = 0;
		switch(wParam) {
			case TIMER_LISTA_DRAG_ARRIBA :
				#if DLISTAEX_MOSTRARDEBUG == TRUE
					Debug_Escribir_Varg(L"DListaEx::_Evento_Temporizador  Drag hacia arriba.\n");
				#endif
				if (_ItemPaginaInicio > 0) {
					_SubirItemsSeleccionados();
//					MostrarItem(_ItemPaginaInicio - 1);
					// Busco el item presionado
					for (i = 0; i < static_cast<LONG_PTR>(_Items.size()); i++) {
						if (_PItemPresionado == _Items[i]) break;
					}
					_ItemPresionado = i;
					_ItemMarcado = i;
					MostrarItem(i);

				}
				else {
					KillTimer(_hWnd, TIMER_LISTA_DRAG_ARRIBA);
					_TimerDragArriba = 0;
				}
				break;
			case TIMER_LISTA_DRAG_ABAJO :
				#if DLISTAEX_MOSTRARDEBUG == TRUE
					Debug_Escribir_Varg(L"DListaEx::_Evento_Temporizador  Drag hacia abajo.\n");
				#endif					
				if (_ItemPaginaFin < static_cast<LONG_PTR>(_Items.size())) {
					_BajarItemsSeleccionados();
//					if (_ItemPaginaVDif == 0) MostrarItem(_ItemPaginaFin + 1);
//					else                      MostrarItem(_ItemPaginaFin);
					// Busco el item presionado
					for (i = static_cast<LONG_PTR>(_Items.size()) - 1; i > -1 ; i--) {
						if (_PItemPresionado == _Items[i]) break;
					}
					_ItemPresionado = i;
					_ItemMarcado = i;
					MostrarItem(i);
				}
				else {
					KillTimer(_hWnd, TIMER_LISTA_DRAG_ABAJO);
					_TimerDragAbajo = 0;
				}
				break;
		}
	}

	void DListaEx::_Drag(DEventoMouse &DatosMouse) {
		LONG_PTR Diferencia = 0;
		if (_ItemPresionado == -1)				return;
		if (_ItemPresionado == _ItemResaltado)	return;
		if (_ItemResaltado == -1)				return;

		
		// Cuento las posiciones de diferencia
		Diferencia = _ItemPresionado - _ItemResaltado;
		// Si la diferencia es mas grande que 1 salgo para evitar glitches
		if (Diferencia != 1 && Diferencia != -1) return;

		#if DLISTAEX_MOSTRARDEBUG == TRUE
			Debug_Escribir_Varg(L"DListaEx::_Drag  Presionado %d, Resaltado %d, Dif %d\n", _ItemPresionado, _ItemResaltado, Diferencia);
		#endif

		// Muevo los items seleccionados
		LONG_PTR i = 0;
		// Hacia abajo		
		if (Diferencia == -1)	{	
			_BajarItemsSeleccionados();		
			for (i = static_cast<LONG_PTR>(_Items.size()) - 1; i > -1; i--) {
				if (_PItemPresionado == _Items[i]) break;
			}
			_ItemPresionado = i;
			_ItemMarcado    = i;
		}
		// Hacia arriba
		else if (Diferencia == 1)	{
			_SubirItemsSeleccionados();		
			for (i = 0; i < static_cast<LONG_PTR>(_Items.size()); i++) {
				if (_PItemPresionado == _Items[i]) break;
			}
			_ItemPresionado = i;
			_ItemMarcado	= i;

		}	

		_ItemPresionado = _ItemResaltado;
		_ItemMarcado	= _ItemResaltado;

		// Temporizador por si hay que subir el scroll
		if (_ItemPresionado == _ItemPaginaInicio && _ItemPaginaInicio > 0) {
			if (_TimerDragArriba == 0) 	_TimerDragArriba = SetTimer(_hWnd, TIMER_LISTA_DRAG_ARRIBA, MILISEGUNDOS_TIMER_DRAG, NULL);
		}
		else {
			if (_TimerDragArriba != 0) {
				KillTimer(_hWnd, _TimerDragArriba);
				_TimerDragArriba = 0;
			}
		}

		// Temporizador por si hay que bajar el scroll
		if (_ItemPresionado == _ItemPaginaFin && _ItemPaginaFin < static_cast<LONG_PTR>(_Items.size())) {
			if (_TimerDragAbajo == 0) _TimerDragAbajo = SetTimer(_hWnd, TIMER_LISTA_DRAG_ABAJO, MILISEGUNDOS_TIMER_DRAG, NULL);
		}
		else {
			if (_TimerDragAbajo != 0) {
				KillTimer(_hWnd, _TimerDragAbajo);
				_TimerDragAbajo = 0;
			}
		}

		Repintar();
	}

	void DListaEx::_SubirItemsSeleccionados(void) {
		DListaEx_Item* TmpItem = NULL;
		// De 0 al total de items
		for (LONG_PTR i = 0; i < static_cast<LONG_PTR>(_Items.size()); i++) {
			// Si el item está seleccionado
			if (_Items[i]->Seleccionado == TRUE) {
				// Si la posición anterior existe
				if (static_cast<LONG_PTR>(_Items.size()) > i - 1 && i - 1 > -1) {
					// Si el item de la posición anterior no está seleccionado
					if (_Items[i - 1]->Seleccionado == FALSE) {
						TmpItem = _Items[i];
						_Items[i] = _Items[i - 1];
						_Items[i - 1] = TmpItem;
					}
				}
			}
		}
	}

	void DListaEx::_BajarItemsSeleccionados(void) {
		DListaEx_Item* TmpItem = NULL;
		// Del total de items hasta 0
		for (LONG_PTR i = _Items.size() - 1; i > -1; i--) {
			// Si el item está seleccionado
			if (_Items[i]->Seleccionado == TRUE) {
				// Si la siguiente posición existe
				if (static_cast<LONG_PTR>(_Items.size()) > i + 1 && i + 1 > -1) {
					// Si el item de la siguiente posición no está seleccionado
					if (_Items[i + 1]->Seleccionado == FALSE) {
						TmpItem = _Items[i];
						_Items[i] = _Items[i + 1];
						_Items[i + 1] = TmpItem;
					}
				}
			}
		}
	}


	const LONG_PTR DListaEx::TotalItemsSeleccionados(void) {
		LONG_PTR Ret = 0;
		for (size_t i = 0; i < _Items.size(); i++) {
			if (_Items[i]->Seleccionado == TRUE) Ret++;
		}
		return Ret;
	}

	void DListaEx::SeleccionarItem(const LONG_PTR sPos, const BOOL nSeleccion) {
		SeleccionarItem(_Items[sPos], nSeleccion);
	}

	void DListaEx::SeleccionarItem(DListaEx_Item *sItem, const BOOL nSeleccion) {
		sItem->Seleccionado = nSeleccion;
		sItem->_AniTransicion.Terminar();
		if (nSeleccion == TRUE) {
			sItem->_ColorFondo		 = Skin.FondoItemSeleccionado;
			sItem->_ColorTexto		 = Skin.TextoItemSeleccionado;
			sItem->_ColorTextoSombra = Skin.TextoItemSeleccionadoSombra;
		}
		else {
			sItem->_ColorFondo		 = Skin.FondoItemNormal;
			sItem->_ColorTexto		 = Skin.TextoItemNormal;
			sItem->_ColorTextoSombra = Skin.TextoItemSombra;
		}
	}


	void DListaEx::_Evento_MousePresionado(const int Boton, WPARAM wParam, LPARAM lParam) {
		DEventoMouse DatosMouse(wParam, lParam, this, Boton);
		_TiempoItemPresionado = GetTickCount();
		SetCapture(_hWnd);
		SetFocus(_hWnd);
		if (Scrolls_MousePresionado(DatosMouse) == TRUE) { return; }

		_ItemPresionado = HitTest(DatosMouse.X(), DatosMouse.Y(), &_SubItemPresionado);
		_ItemMarcado	= _ItemPresionado;
		if (_ItemPresionado != -1) _PItemPresionado = _Items[_ItemPresionado];

		#if DLISTAEX_MOSTRARDEBUG == TRUE
			Debug_Escribir_Varg(L"DListaEx::_Evento_MousePresionado IP:%d X:%d Y:%d\n", _ItemMarcado, DatosMouse.X(), DatosMouse.Y());
		#endif
		// Pre-Selecciono el item presionado
		if (_ItemPresionado != -1 && Boton == 0) {
			// Si la tecla control no está presionada Pre-Selecciono el item
			if (DatosMouse.Control() != TRUE) SeleccionarItem(_Items[_ItemPresionado], TRUE);
		}
/*		if (_ItemPresionado != -1) {			
			BOOL tShift   = DatosMouse.Shift();
			BOOL tControl = DatosMouse.Control();
			if (tShift == TRUE && _ItemShift != -1) {
				DesSeleccionarTodo();
				if (_ItemShift < _ItemMarcado) { for (LONG_PTR i = _ItemShift; i <= _ItemMarcado; i++) SeleccionarItem(i, TRUE);	}
				else                           { for (LONG_PTR i = _ItemMarcado; i <= _ItemShift; i++) SeleccionarItem(i, TRUE);	}
			}
			else if (tControl == TRUE) {
				SeleccionarItem(_ItemPresionado, !_Items[static_cast<unsigned int>(_ItemPresionado)]->Seleccionado);
//				_Items[static_cast<unsigned int>(_ItemPresionado)]->Seleccionado = !_Items[static_cast<unsigned int>(_ItemPresionado)]->Seleccionado;
			}
			else {
				// Si no es el boton derecho del mouse (el boton derecho suele desplegar un menú)
				if (DatosMouse.Boton != 1) {
					// Si la multiseleccion está des-habilitada o la tecla control no está pulsada
					if (MultiSeleccion == FALSE || tControl == FALSE || tShift == FALSE) DesSeleccionarTodo();
				}
				else {
					// Si solo hay un item seleccionado, lo des-selecciono
					if (TotalItemsSeleccionados() == 1) DesSeleccionarTodo();
				}
				// Selecciono el item que hay debajo del mouse
				SeleccionarItem(_ItemPresionado, TRUE);
				//_Items[static_cast<unsigned int>(_ItemPresionado)]->Seleccionado = TRUE;
			}

			_Items[_ItemPresionado]->_Transicion(DListaEx_TransicionItem::DListaEx_TransicionItem_Presionado);
		}
		else {
			DesSeleccionarTodo();
		}*/

		Evento_MousePresionado(DatosMouse);
		Repintar(TRUE);
		// Envio el evento mouseup a la ventana padre
		SendMessage(GetParent(hWnd()), DWL_LISTAEX_MOUSEPRESIONADO, reinterpret_cast<WPARAM>(&DatosMouse), 0);
	}

	void DListaEx::_Evento_MouseSoltado(const int Boton, WPARAM wParam, LPARAM lParam) {
		DEventoMouse DatosMouse(wParam, lParam, this, Boton);
		_TiempoItemPresionado = 0;

		// Elimino temporizadores del drag
		if (_TimerDragArriba != 0) {
			KillTimer(_hWnd, _TimerDragArriba);
			_TimerDragArriba = 0;
		}
		if (_TimerDragAbajo != 0) {
			KillTimer(_hWnd, _TimerDragAbajo);
			_TimerDragAbajo = 0;
		}

		
		if (_ItemPresionado != -1) {			
			BOOL tShift   = DatosMouse.Shift();
			BOOL tControl = DatosMouse.Control();
			if (tShift == TRUE && _ItemShift != -1) {
				DesSeleccionarTodo();
				if (_ItemShift < _ItemMarcado) { for (LONG_PTR i = _ItemShift; i <= _ItemMarcado; i++) SeleccionarItem(i, TRUE);	}
				else                           { for (LONG_PTR i = _ItemMarcado; i <= _ItemShift; i++) SeleccionarItem(i, TRUE);	}
			}
			else if (tControl == TRUE) {
				SeleccionarItem(_ItemPresionado, !_Items[static_cast<unsigned int>(_ItemPresionado)]->Seleccionado);
//				_Items[static_cast<unsigned int>(_ItemPresionado)]->Seleccionado = !_Items[static_cast<unsigned int>(_ItemPresionado)]->Seleccionado;
			}
			else {
				// Si no es el boton derecho del mouse (el boton derecho suele desplegar un menú)
				if (DatosMouse.Boton != 1) {
					// Si la multiseleccion está des-habilitada o la tecla control no está pulsada
					if (MultiSeleccion == FALSE || tControl == FALSE || tShift == FALSE) DesSeleccionarTodo();
				}
				else {
					// Si solo hay un item seleccionado, lo des-selecciono
					if (TotalItemsSeleccionados() == 1) DesSeleccionarTodo();
				}
				// Selecciono el item que hay debajo del mouse
				SeleccionarItem(_ItemPresionado, TRUE);
				//_Items[static_cast<unsigned int>(_ItemPresionado)]->Seleccionado = TRUE;
			}

			_Items[_ItemPresionado]->_Transicion(DListaEx_TransicionItem::DListaEx_TransicionItem_Presionado);
		}
		else {
			DesSeleccionarTodo();
		}

		// Libero la captura del mouse
		ReleaseCapture();

		// Si el evento del moouse pertenece al scroll, salgo.
		if (Scrolls_MouseSoltado(DatosMouse) == TRUE) { return; }

		// Transiciones para el item presionado
		LONG_PTR T = HitTest(DatosMouse.X(), DatosMouse.Y(), &_SubItemPresionado);
		if (_ItemPresionado != -1) {
			if (T == _ItemPresionado) _Items[_ItemPresionado]->_TransicionResaltado();
			else                      _Items[_ItemPresionado]->_TransicionNormal();
		}

		// Evento virtual
		Evento_MouseSoltado(DatosMouse);
		// Envio el evento mouseup a la ventana padre
		SendMessage(GetParent(hWnd()), DWL_LISTAEX_MOUSESOLTADO, reinterpret_cast<WPARAM>(&DatosMouse), 0);
		// Envio el evento DragTerminado si ha habido una operación de Drag & Drop
		if (_EnDrag == TRUE) {
			SendMessage(GetParent(hWnd()), DWL_LISTAEX_DRAGTERMINADO, 0, 0);
			Evento_DragTerminado();
		}
		_EnDrag = FALSE;
		
		// Compruebo si el mouse está dentro del control
		RECT RC;
		POINT Pt = { DatosMouse.X(), DatosMouse.Y() };
		GetClientRect(_hWnd, &RC);
		if (PtInRect(&RC, Pt) == TRUE) {
			// Envio el evento del click a la ventana padre (solo si el mouse está dentro de la lista)
			SendMessage(GetParent(hWnd()), DWL_LISTAEX_CLICK, reinterpret_cast<WPARAM>(&DatosMouse), 0);
		}

		// Establezco que no hay ningún item presionado, y repinto
		// TODO : El -1 está malament lo millor seria fer servir el objecte DListaEx_Item * directament com a l'arbre
		_ItemPresionado = -1;
		_SubItemPresionado = -1;
		_PItemPresionado = NULL;
		Repintar();

	}

	void DListaEx::_Evento_MouseRueda(WPARAM wParam, LPARAM lParam) {
		DEventoMouseRueda DatosMouse(wParam, lParam, this);

//		RECT RW;
//		GetWindowRect(hWnd(), &RW);

		#if DLISTAEX_MOSTRARDEBUG == TRUE
			Debug_Escribir_Varg(L"DListaEx::_Evento_MouseRueda X:%d Y:%d Delta:%d\n", DatosMouse.X(), DatosMouse.Y(), DatosMouse.Delta());
		#endif


		if (DatosMouse.Delta() > 0) { // hacia arriba
			_ScrollV_Posicion -= _ScrollV_Pagina / 10.0f;
			if (_ScrollV_Posicion < 0.0f) _ScrollV_Posicion = 0.0f;
		}
		else { // hacia abajo
			_ScrollV_Posicion += _ScrollV_Pagina / 10.0f;
			if (_ScrollV_Posicion > 1.0f) _ScrollV_Posicion = 1.0f;
		}

		_CalcularScrolls();
		// Las coordenadas X e Y son relativas a la pantalla...
//		LONG ncX = RW.left - DatosMouse.X();
//		LONG ncY = RW.top - DatosMouse.Y();
		_ItemResaltado = HitTest(DatosMouse.X(), DatosMouse.Y());

		if (_ItemUResaltado != -1) _Items[_ItemUResaltado]->_TransicionNormal();
		if (_ItemResaltado != -1)  _Items[_ItemResaltado]->_TransicionResaltado();

		_ItemUResaltado = _ItemResaltado;

		Evento_MouseRueda(DatosMouse);
		Repintar();
	}


	void DListaEx::_Evento_TeclaPresionada(WPARAM wParam, LPARAM lParam) {
		DEventoTeclado DatosTeclado(wParam, lParam, this);
		// Marco la tecla como presionada
//		DhWnd::Teclado[DatosTeclado.TeclaVirtual()] = true;

		// Si hay items ...
		if (_Items.size() > 0) {
			// Me guardo el item marcado para indicar desde donde empieza el shift
			if (DatosTeclado.TeclaVirtual() == VK_SHIFT) {
				if (_ItemShift == -1) {
					_ItemShift = (_ItemMarcado == -1) ? _ItemPaginaInicio : _ItemMarcado;
				}
			}

			switch (DatosTeclado.TeclaVirtual()) {
				case VK_HOME	: _Tecla_Inicio(DatosTeclado);				break;
				case VK_END		: _Tecla_Fin(DatosTeclado);					break;
				case VK_UP		: _Tecla_CursorArriba(DatosTeclado);		break;
				case VK_DOWN	: _Tecla_CursorAbajo(DatosTeclado);			break;
				case VK_PRIOR	: _Tecla_RePag(DatosTeclado);				break; // RePag
				case VK_NEXT	: _Tecla_AvPag(DatosTeclado);				break; // AvPag
				default			: Evento_Tecla(DatosTeclado);				break; // if (Caracter >= 0x30 && Caracter <= 0x5A) // Cualquier tecla valida
			}
		}
		Evento_TeclaPresionada(DatosTeclado);

	}

	void DListaEx::_Evento_TeclaSoltada(WPARAM wParam, LPARAM lParam) {
		DEventoTeclado DatosTeclado(wParam, lParam, this); 
		if (DatosTeclado.TeclaVirtual() == VK_SHIFT) {
			_ItemShift = -1;
		}		
		Evento_TeclaSoltada(DatosTeclado);
	}

	void DListaEx::_Evento_Tecla(WPARAM wParam, LPARAM lParam) {
		DEventoTeclado DatosTeclado(wParam, lParam, this);
		Evento_Tecla(DatosTeclado);
	}


	// Teclas especiales
	void DListaEx::_Tecla_CursorArriba(DEventoTeclado &DatosTeclado) {
		// Si no hay nodos, salgo de la función
		if (_Items.size() == 0) return;

		// Si no hay item marcado, marco el primero visible y salgo
		if (_ItemMarcado == -1) {	
			_ItemMarcado = _ItemPaginaInicio;
			SeleccionarItem(_ItemMarcado, TRUE);
//			_Items[static_cast<unsigned int>(_ItemMarcado)]->Seleccionado = TRUE;
			MostrarItem(_ItemMarcado);
			return;
		}

		BOOL tControl = DatosTeclado.Control();
		BOOL tShift   = DatosTeclado.Shift();

		// Si hay un item anterior al marcado
		if (_ItemMarcado - 1 != -1) {
			// Asigno el nuevo item marcado
			_ItemMarcado--;

			if (tShift == TRUE && MultiSeleccion == TRUE) {
				DesSeleccionarTodo();
				if (_ItemShift != -1) {
					if (_ItemShift < _ItemMarcado) { for (LONG_PTR i = _ItemShift; i <= _ItemMarcado; i++) 	SeleccionarItem(i, TRUE); }
					else                           { for (LONG_PTR i = _ItemMarcado; i <= _ItemShift; i++) 	SeleccionarItem(i, TRUE); }
				}
				#if DLISTAEX_MOSTRARDEBUG == TRUE
					Debug_Escribir_Varg(L"DListaEx::_Tecla_CursorArriba & Shift -> IS : %d, IM : %d\n", _ItemShift, _ItemMarcado);
				#endif
			}
			// Si el control o el shift estan presionados revierto el ultimo estado seleccionado del item marcado, antes de situarme en el nuevo item marcado
/*			else if (tControl == TRUE) {
				_Items[_ItemMarcado]->Seleccionado = !_Items[_ItemMarcado]->Seleccionado;
				#if DLISTAEX_MOSTRARDEBUG == TRUE
					Debug_Escribir(L"DListaEx::_Tecla_CursorArriba & Control\n");
				#endif
			}*/
			else {
				if (MultiSeleccion == FALSE || tControl == FALSE || tShift == FALSE) DesSeleccionarTodo();
				//_Items[static_cast<unsigned int>(_ItemMarcado)]->Seleccionado = TRUE;
				SeleccionarItem(_ItemMarcado, TRUE);
				#if DLISTAEX_MOSTRARDEBUG == TRUE
					Debug_Escribir(L"DListaEx::_Tecla_CursorArriba\n");
				#endif

			}
			MostrarItem(_ItemMarcado);
		}
	}

	void DListaEx::_Tecla_CursorAbajo(DEventoTeclado &DatosTeclado) {
		// Si no hay nodos, salgo de la función
		if (_Items.size() == 0) return;

		// Si no hay item marcado, marco el primero visible y salgo
		if (_ItemMarcado == -1) {
			_ItemMarcado = _ItemPaginaInicio;
			SeleccionarItem(_ItemMarcado, TRUE);
			//_Items[static_cast<unsigned int>(_ItemMarcado)]->Seleccionado = TRUE;
			MostrarItem(_ItemMarcado);
			return;
		}

		BOOL tControl = DatosTeclado.Control();
		BOOL tShift   = DatosTeclado.Shift();

		// Si hay un item siguiente al maracado
		if (static_cast<size_t>(_ItemMarcado) + 1 < _Items.size()) {
			// Asigno el nuevo item marcado
			_ItemMarcado++;

			if (tShift == TRUE && MultiSeleccion == TRUE) {
				if (_ItemShift != -1) {
					DesSeleccionarTodo();
					if (_ItemShift < _ItemMarcado) { for (LONG_PTR i = _ItemShift; i <= _ItemMarcado; i++) 	SeleccionarItem(i, TRUE); }
					else                           { for (LONG_PTR i = _ItemMarcado; i <= _ItemShift; i++)  SeleccionarItem(i, TRUE); }
				}
				#if DLISTAEX_MOSTRARDEBUG == TRUE
					Debug_Escribir_Varg(L"DListaEx::_Tecla_CursorAbajo & Shift -> IS : %d, IM : %d\n", _ItemShift, _ItemMarcado);
				#endif
			}

/*			else if (tControl == TRUE) {
				_Items[_ItemMarcado]->Seleccionado = !_Items[_ItemMarcado]->Seleccionado;
				#if DLISTAEX_MOSTRARDEBUG == TRUE
					Debug_Escribir(L"DListaEx::_Tecla_CursorAbajo & Control\n");
				#endif
			}*/
			else {
				if (MultiSeleccion == FALSE || tControl == FALSE || tShift == FALSE) DesSeleccionarTodo();
				SeleccionarItem(_ItemMarcado, TRUE);
				//_Items[static_cast<unsigned int>(_ItemMarcado)]->Seleccionado = TRUE;
				#if DLISTAEX_MOSTRARDEBUG == TRUE
					Debug_Escribir(L"DListaEx::_Tecla_CursorAbajo\n");
				#endif
			}
			MostrarItem(_ItemMarcado);
		}
	}

	void DListaEx::_Tecla_Inicio(DEventoTeclado &DatosTeclado) {
		// Si no hay nodos, salgo de la función
		if (_Items.size() == 0) return;

		DesSeleccionarTodo();

		if (DatosTeclado.Shift() == TRUE && MultiSeleccion == TRUE) {
			for (LONG_PTR i = 0; i <= _ItemShift; i++) SeleccionarItem(i, TRUE);
		}

		_ItemMarcado = 0;
		SeleccionarItem(_ItemMarcado, TRUE);
		//_Items[static_cast<unsigned int>(_ItemMarcado)]->Seleccionado = TRUE;
		MostrarItem(_ItemMarcado);
	}

	void DListaEx::_Tecla_Fin(DEventoTeclado &DatosTeclado) {
		// Si no hay nodos, salgo de la función
		if (_Items.size() == 0) return;

		DesSeleccionarTodo();
		_ItemMarcado = _Items.size() - 1;

		if (DatosTeclado.Shift() == TRUE && MultiSeleccion == TRUE) {
			for (LONG_PTR i = _ItemShift; i < _ItemMarcado; i++) SeleccionarItem(i, TRUE);
		}

		//_Items[static_cast<unsigned int>(_ItemMarcado)]->Seleccionado = TRUE;
		SeleccionarItem(_ItemMarcado, TRUE);
		MostrarItem(_ItemMarcado);
	}

	void DListaEx::_Tecla_AvPag(DEventoTeclado &DatosTeclado) {
		if (_ItemMarcado == _ItemPaginaFin) {
			AvPag();
		}		

		DesSeleccionarTodo();
		if (DatosTeclado.Shift() == TRUE && MultiSeleccion == TRUE) {
			LONG_PTR ItemFin = (_ItemPaginaFin != -1) ? _ItemPaginaFin : static_cast<LONG_PTR>(_Items.size());
			if (_ItemShift < ItemFin) { for (LONG_PTR i = _ItemShift; i <= ItemFin; i++) 	SeleccionarItem(i, TRUE); }
			else                      { for (LONG_PTR i = ItemFin; i <= _ItemShift; i++) 	SeleccionarItem(i, TRUE); }
		}

		MostrarItem(_ItemPaginaFin, FALSE);
		_ItemMarcado = _ItemPaginaFin;		// Asigno el itemMarcado DESPRES de MostrarItem (que per alguna rao em cambia el _ItemPagina
		//_Items[static_cast<unsigned int>(_ItemMarcado)]->Seleccionado = TRUE;
		SeleccionarItem(_ItemMarcado, TRUE);
		Repintar();
	}

	void DListaEx::_Tecla_RePag(DEventoTeclado &DatosTeclado) {
		// Al utilitzar MostrarItem el _ItemPaginaInicio cambia i li resta 1 al seu valor, aixo es un parche cutre per que funcioni el RePag...
		if (_ItemMarcado == _ItemPaginaInicio || _ItemMarcado - 1 == _ItemPaginaInicio) {
			RePag();
		}

		DesSeleccionarTodo();
		if (DatosTeclado.Shift() == TRUE && MultiSeleccion == TRUE) {
			LONG_PTR ItemInicio = (_ItemPaginaInicio != -1) ? _ItemPaginaInicio : 0;
			if (_ItemShift < ItemInicio) { for (LONG_PTR i = _ItemShift; i <= ItemInicio; i++) 	SeleccionarItem(i, TRUE); }
			else						 { for (LONG_PTR i = ItemInicio; i <= _ItemShift; i++) 	SeleccionarItem(i, TRUE);  }
		}

		_ItemMarcado = _ItemPaginaInicio;
		MostrarItem(_ItemMarcado, FALSE);
		SeleccionarItem(_ItemMarcado, TRUE);
		//_Items[static_cast<unsigned int>(_ItemMarcado)]->Seleccionado = TRUE;
		Repintar();
	}


	void DListaEx::_Evento_MouseSaliendo(void) {
		BOOL nRepintar = Scrolls_MouseSaliendo();

		if (_ItemUResaltado != -1) {
			if (static_cast<LONG_PTR>(_Items.size()) > _ItemUResaltado)	_Items[_ItemUResaltado]->_TransicionNormal();
		}
		if (_ItemResaltado != -1) {
			if (static_cast<LONG_PTR>(_Items.size()) > _ItemResaltado)	_Items[_ItemResaltado]->_TransicionNormal();
		}

/*		for (size_t i = 0; i < _Items.size(); i++) {
			_Items[i]->_AniTransicion.Terminar();
		}*/

		_MouseDentro = FALSE;
		_ItemResaltado  = -1;
		_ItemUResaltado = -1;
		Evento_MouseSaliendo();
		if (nRepintar == TRUE) Repintar();
	}

	void DListaEx::_Evento_MouseDobleClick(const int Boton, WPARAM wParam, LPARAM lParam) {
		DEventoMouse DatosMouse(wParam, lParam, this, Boton);
		Evento_MouseDobleClick(DatosMouse);

//		Evento_MouseDobleClick(Boton, cX, cY, Param);
	}

	void DListaEx::_Evento_FocoObtenido(HWND hWndUltimoFoco) {
//		BorrarBufferTeclado();
		Evento_FocoObtenido(hWndUltimoFoco);
	}

	void DListaEx::_Evento_FocoPerdido(HWND hWndNuevoFoco) {
//		BorrarBufferTeclado();
		Evento_FocoPerdido(hWndNuevoFoco);
	}

	// Función que mira si algun item de la lista tiene que pintar un icono.
	// Si no hay iconos en la lista, no se reservará espacio para mostrar-los
	void DListaEx::_CalcularPintarIconos(void) {
		_PintarIconos = FALSE;
		for (size_t i = 0; i < _Items.size(); i++) {
			// Hay un item con icono
			if (_Items[i]->_Icono() != NULL) {
				_PintarIconos = TRUE;
				return;	// Ya no es necesario buscar mas, salgo.
			}
		}
	}

	LRESULT CALLBACK DListaEx::GestorMensajes(UINT uMsg, WPARAM wParam, LPARAM lParam) {
		switch (uMsg) {
			// Foco
			case WM_SETFOCUS:		_Evento_FocoObtenido((HWND)wParam);																											return 0;
			case WM_KILLFOCUS:		_Evento_FocoPerdido((HWND)wParam);																											return 0;
			// Pintado
			case WM_PAINT:			_Evento_Pintar();																															return 0;
			// Cambio de tamaño
			case WM_SIZE:			_CalcularScrolls();		Repintar();																											return 0;
			// Mouse
			case WM_MOUSEMOVE:		_Evento_MouseMovimiento(wParam, lParam);																									return 0;
//			case WM_MOUSEMOVE:		_Evento_MouseMovimiento(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), static_cast<UINT>(wParam));												return 0;
			case WM_MOUSELEAVE:		_Evento_MouseSaliendo();																													return 0;
			// Mouse presionado
			case WM_LBUTTONDOWN:	_Evento_MousePresionado(0, wParam, lParam);																									return 0;
			case WM_RBUTTONDOWN:	_Evento_MousePresionado(1, wParam, lParam);																									return 0;
			case WM_MBUTTONDOWN:	_Evento_MousePresionado(2, wParam, lParam);																									return 0;
			// Mouse soltado
			case WM_LBUTTONUP:		_Evento_MouseSoltado(0, wParam, lParam);																									return 0;
			case WM_RBUTTONUP:		_Evento_MouseSoltado(1, wParam, lParam);																									return 0;
			case WM_MBUTTONUP:		_Evento_MouseSoltado(2, wParam, lParam);																									return 0;
			// Mouse doble click
			case WM_LBUTTONDBLCLK:	_Evento_MouseDobleClick(0, wParam, lParam);																									return 0;
			case WM_RBUTTONDBLCLK:	_Evento_MouseDobleClick(1, wParam, lParam);																									return 0;
			case WM_MBUTTONDBLCLK:	_Evento_MouseDobleClick(2, wParam, lParam);																									return 0;

			case WM_TIMER:          _Evento_Temporizador(wParam);                                                                                                               return 0;

			// Mouse rueda
			case WM_MOUSEWHEEL:		_Evento_MouseRueda(wParam, lParam);																											return 0;
			
			// Teclado (Ojo con los breaks que son necesarios)
			case WM_KEYDOWN:		_Evento_TeclaPresionada(wParam, lParam);																									break;
			case WM_KEYUP:			_Evento_TeclaSoltada(wParam, lParam);																										break;
			case WM_CHAR:           _Evento_Tecla(wParam, lParam);																												break;
		}	
		return DControlEx::GestorMensajes(uMsg, wParam, lParam);
	}

}