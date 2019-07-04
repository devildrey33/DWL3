#ifndef DLISTAEX_H
#define DLISTAEX_H

#include "DControlEx.h"
#include "DListaEx_Columna.h"
#include "DListaEx_Item.h"
#include "DFuente.h"
#include "DIcono.h"
#include "DBarraScrollEx.h"
#include "DEventoTeclado.h"
#include "DEventoMouseRueda.h"


namespace DWL {
	class DListaEx_Skin {
	  public:
						DListaEx_Skin(void);
						// Colores para el fondo (OJO los colores del fondo y del borde del control están en DBarraSroll_Skin)
		COLORREF		FondoItemNormal;
		COLORREF		FondoItemResaltado;
		COLORREF		FondoItemSeleccionado;
		COLORREF		FondoItemSeleccionadoResaltado;
		COLORREF		FondoItemPresionado;
						// Color para el borde del item marcado
		COLORREF		BordeItemMarcado;
						// Colores para el texto
		COLORREF		TextoItemNormal;
		COLORREF		TextoItemResaltado;
		COLORREF		TextoItemSombra;
		COLORREF		TextoItemSeleccionado;
		COLORREF		TextoItemSeleccionadoSombra;
		COLORREF		TextoItemSeleccionadoResaltado;
		COLORREF		TextoItemPresionado;
						// Fuente
		int				FuenteTam;
		std::wstring	FuenteNombre;
		BOOL			FuenteNegrita;
		BOOL			FuenteCursiva;
		BOOL			FuenteSubrayado;
		BOOL			FuenteSombraTexto;
	};


	#define DLISTAEX_MOSTRARDEBUG	FALSE

	#ifdef _WIN64
		#define DLISTAEX_POSICION_INICIO	 0
		#define DLISTAEX_POSICION_FIN		-1
		#define DLISTAEX_POSICION_ORDENADO	-2
	#else
		#define DLISTAEX_POSICION_INICIO	0
		#define DLISTAEX_POSICION_FIN		_UI32_MAX - 1
		#define DLISTAEX_POSICION_ORDENADO	_UI32_MAX - 2
	#endif

	/* Estados del mouse que puede tener un item */
	enum DListaEx_Item_EstadoMouse {
		DListaEx_Item_EstadoMouse_Normal		= 0,
		DListaEx_Item_EstadoMouse_Resaltado		= 1,
		DListaEx_Item_EstadoMouse_Presionado	= 2
	};

	class DDesplegableEx;

	class DListaEx : public virtual DBarraScrollEx {
	  public:
														DListaEx(void);
		                                               ~DListaEx(void);
														// Crea el control ListaEx
		HWND											CrearListaEx(DhWnd *nPadre, const int cX, const int cY, const int cAncho, const int cAlto, const int cID, const DWORD Estilos = WS_CHILD | WS_VISIBLE, const DWORD EstilosExtendidos = NULL);
		const BOOL										Destruir(void);

		DListaEx_Columna							   *AgregarColumna(const int nAncho = DLISTAEX_COLUMNA_ANCHO_AUTO, DListaEx_Columna_Alineacion nAlineacion = DListaEx_Columna_Alineacion_Izquierda);

//		void                                            EliminarColumna(const size_t nPosColumna);
		void											EliminarTodasLasColumnas(void);

														// Agrega un item personalizado (por defecto es del tipo DListaEx_Item) para iconos de los recursos
		template <class TItem = DListaEx_Item> TItem   *AgregarItem(const INT_PTR nIconoRecursos, const INT_PTR PosicionFila, std::initializer_list<std::wstring> Textos) {
															TItem  *nItem = new TItem();
															DWL::DIcono Ico(nIconoRecursos, DLISTAEX_TAMICONO, DLISTAEX_TAMICONO);
															//DListaIconos_Icono *TmpIco = DListaIconos::AgregarIconoRecursos(nIconoRecursos, DLISTAEX_TAMICONO, DLISTAEX_TAMICONO);
															_AgregarItem(nItem, Ico, PosicionFila, Textos);
															return nItem;
														};

														// Agrega un item personalizado (por defecto es del tipo DListaEx_Item) para iconos del sistema
		template <class TItem = DListaEx_Item> TItem   *AgregarItem(const GUID nIconoKnownFolder, const INT_PTR PosicionFila, std::initializer_list<std::wstring> Textos) {
															TItem *nItem = new TItem();
															DWL::DIcono Ico(nIconoKnownFolder, DLISTAEX_TAMICONO, DLISTAEX_TAMICONO);
															//DListaIconos_Icono *TmpIco = DListaIconos::AgregarIconoKnownFolder(nIconoKnownFolder, DLISTAEX_TAMICONO, DLISTAEX_TAMICONO);
															_AgregarItem(nItem, Ico, PosicionFila, Textos);
															return nItem;
														};

														// Agrega un item personalizado (por defecto es del tipo DListaEx_Item) para iconos especificos de un path
		template <class TItem = DListaEx_Item> TItem   *AgregarItem(const wchar_t *nPathIcono, const int nPosIcono, const INT_PTR PosicionFila, std::initializer_list<std::wstring> Textos) {
															TItem  *nItem = new TItem();
															DWL::DIcono Ico(nPathIcono, DLISTAEX_TAMICONO, DLISTAEX_TAMICONO, nPosIcono);
															//DListaIconos_Icono *TmpIco = DListaIconos::AgregarIconoExterno(nPathIcono, DLISTAEX_TAMICONO, DLISTAEX_TAMICONO, nPosIcono);
															_AgregarItem(nItem, Ico, PosicionFila, Textos);
															return nItem;
														};


		void											EliminarItem(const LONG_PTR ePosItem);
		void											EliminarItem(DListaEx_Item *eItem);
		const LONG_PTR									EliminarItemsSeleccionados(void);
		void											EliminarTodosLosItems(void);
														// Acceso a los DListaEx_Item
		inline DListaEx_Item                           *Item(const LONG_PTR iPos) { if (iPos == -1)            { return NULL; }	return _Items[static_cast<size_t>(iPos)]; }
		inline DListaEx_Item                           *ItemResaltado(void)	  	  { if (_ItemResaltado == -1)  { return NULL; }	return _Items[static_cast<size_t>(_ItemResaltado)]; };
		inline DListaEx_Item                           *ItemPresionado(void)	  { if (_ItemPresionado == -1) { return NULL; }	return _Items[static_cast<size_t>(_ItemPresionado)]; };
		inline DListaEx_Item                           *ItemMarcado(void)		  { if (_ItemMarcado == -1)    { return NULL; }	return _Items[static_cast<size_t>(_ItemMarcado)]; };
		void											ItemMarcado(DListaEx_Item *NuevoItemMarcado, const BOOL nRepintar = FALSE);
														// Devuelve la posición del DListaEx_Item que está dentro de esta lista
		const LONG_PTR                                  ItemPos(DListaEx_Item *pItem);
		virtual void									Repintar(const BOOL nForzar = FALSE);

		void                                            SeleccionarItem(const LONG_PTR sPos, const BOOL nSeleccion);
		void                                            SeleccionarItem(DListaEx_Item *sItem, const BOOL nSeleccion);

		void											DesSeleccionarTodo(void);
		void											MostrarItem(const LONG_PTR iPos, const BOOL nRepintar = TRUE);
		void											MostrarItem(DListaEx_Item *eItem);

		const BOOL                                      ObtenerRectaItem(const LONG_PTR iPos, RECT &rRecta);
		inline const BOOL                               ObtenerRectaItem(DListaEx_Item *eItem, RECT &rRecta) { return ObtenerRectaItem(ItemPos(eItem), rRecta); }

		inline const LONG_PTR							TotalItems(void) { return static_cast<LONG_PTR>(_Items.size()); }
		const LONG_PTR									TotalItemsSeleccionados(void);

		inline const size_t                             TotalColumnas(void) { return _Columnas.size();  }
		inline DListaEx_Columna                        *Columna(const LONG_PTR cPos) { return _Columnas[static_cast<unsigned int>(cPos)];  }

		void											Pintar(HDC hDC);
		void											PintarItem(HDC hDC, const LONG_PTR pPosItem, RECT &Espacio);

		const LONG_PTR									HitTest(const int cX, const int cY, LONG_PTR*PosSubItem = NULL);

		void											Scrolls_EventoCambioPosicion(void);

														// Eventos virtuales
		virtual void          							Evento_MouseEntrando(void)																		{ };
		virtual void									Evento_MouseSaliendo(void)																		{ };
		virtual void									Evento_MouseMovimiento(DEventoMouse &DatosMouse)												{ };
		virtual void									Evento_MousePresionado(DEventoMouse &DatosMouse)												{ };
		virtual void									Evento_MouseSoltado(DEventoMouse &DatosMouse)													{ };
		virtual void                                    Evento_MouseRueda(DEventoMouseRueda &DatosMouse)												{ };
		virtual void									Evento_MouseDobleClick(DEventoMouse &DatosMouse)												{ };

		virtual void                                    Evento_TeclaPresionada(DEventoTeclado &DatosTeclado)											{ };
		virtual void                                    Evento_TeclaSoltada(DEventoTeclado &DatosTeclado)												{ };
		virtual void									Evento_Tecla(DEventoTeclado &DatosTeclado)														{ };

		virtual void								    Evento_FocoObtenido(HWND hWndUltimoFoco)														{ };
		virtual void								    Evento_FocoPerdido(HWND hWndNuevoFoco)															{ };

		virtual void                                    Evento_PintarSubItem(HDC hDC, const LONG_PTR NumItem, const LONG_PTR NumSubItem, RECT *Espacio) { };

		BOOL                                            MostrarSeleccion;
		BOOL                                            MultiSeleccion;
		BOOL                                            MoverItemsDrag;

		LRESULT CALLBACK								GestorMensajes(UINT uMsg, WPARAM wParam, LPARAM lParam);

		DFuente			                                Fuente;
		DListaEx_Skin                                   Skin;
		void                                            ActualizarSkin(void);
	  protected:
		
														// Eventos internos
		inline void                                    _Evento_Pintar(void);

		void										   _Evento_MouseMovimiento(WPARAM wParam, LPARAM lParam);
		void										   _Evento_MousePresionado(const int Boton, WPARAM wParam, LPARAM lParam);
		void										   _Evento_MouseSoltado(const int Boton, WPARAM wParam, LPARAM lParam);
		void                                           _Evento_MouseRueda(WPARAM wParam, LPARAM lParam);
		void										   _Evento_MouseSaliendo(void);
		void										   _Evento_MouseDobleClick(const int Boton, WPARAM wParam, LPARAM lParam);

		void                                           _Evento_TeclaPresionada(WPARAM wParam, LPARAM lParam);
		void                                           _Evento_TeclaSoltada(WPARAM wParam, LPARAM lParam);
		void										   _Evento_Tecla(WPARAM wParam, LPARAM lParam);

		void										   _Evento_FocoObtenido(HWND hWndUltimoFoco);
		void										   _Evento_FocoPerdido(HWND hWndNuevoFoco);

		void                                           _Evento_Temporizador(WPARAM wParam);

		void                                           _Drag(DEventoMouse& DatosMouse);
		UINT_PTR                                       _TimerDragArriba;
		UINT_PTR                                       _TimerDragAbajo;

		void                                           _SubirItemsSeleccionados(void);
		void                                           _BajarItemsSeleccionados(void);

		// Teclas especiales
		void										   _Tecla_CursorArriba(DEventoTeclado &DatosTeclado);
		void										   _Tecla_CursorAbajo(DEventoTeclado &DatosTeclado);
		void										   _Tecla_Inicio(DEventoTeclado &DatosTeclado);
		void										   _Tecla_Fin(DEventoTeclado &DatosTeclado);
		void										   _Tecla_AvPag(DEventoTeclado &DatosTeclado);
		void										   _Tecla_RePag(DEventoTeclado &DatosTeclado);

														// Valor que determina si hay que recalcular los tamaños antes de pintar
		BOOL                                           _Repintar; 		

														// Funciones que calculan el espacio para el listview
		void										   _CalcularTotalEspacioVisible(RECT &RC);
		void										   _CalcularScrolls(void);
		void                                           _CalcularColumnas(void);
		void                                           _CalcularItemsPagina(const size_t TamPagina);
		void                                           _CalcularPintarIconos(void);

		DListaEx_Item                                 *_AgregarItem(DListaEx_Item *nItem, DIcono &nIcono, const INT_PTR PosicionFila, std::initializer_list<std::wstring> Textos);
		std::vector<DListaEx_Item *>                   _Items;
		std::vector<DListaEx_Columna *>                _Columnas;

		DWORD                                          _TiempoItemPresionado; // Tiempo que lleva presionado el item (para hacer drag)
		LONG_PTR                                       _ItemResaltado;
		LONG_PTR	                                   _ItemUResaltado;
		LONG_PTR                                       _SubItemResaltado; // es en número de columa partiendo de la _FilaRsaltada
		LONG_PTR									   _SubItemUResaltado;
		LONG_PTR			                           _ItemPresionado;
		DListaEx_Item                                 *_PItemPresionado;
		LONG_PTR			                           _SubItemPresionado;
		LONG_PTR			                           _ItemMarcado;
		LONG_PTR					                   _ItemShift;
														// Diferencia de pixeles inicial entre la _FilaInicioPagina y el inicio de la página visible
		LONG                                           _ItemPaginaVDif;
														// Diferencia de pixeles entre .... 
		LONG                                           _ItemPaginaHDif;

		LONG_PTR                                       _ItemPaginaInicio;
		LONG_PTR                                       _ItemPaginaFin;

		size_t                                         _TotalAnchoVisible;
		size_t										   _TotalAltoVisible;

		void										   _CrearBufferItem(const int nAncho, const int nAlto);
														// Buffer DC permanente para pintar UNA fila
		HDC                                            _BufferItem;
		HBITMAP                                        _BufferItemBmp;
		HBITMAP                                        _BufferItemBmpViejo;
		HFONT                                          _BufferItemFuenteViejo;

		BOOL                                           _PintarIconos;
/*		int                                            _MouseX;
		int                                            _MouseY;*/

		friend class DListaEx_Columna;
		friend class DListaEx_Item;
		friend class DListaEx_SubItem;

		friend class DDesplegableEx;

	};

}
#endif