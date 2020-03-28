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
	// Objeto con la configuraci�n de colores y fuente por defecto para la lista
	class DListaEx_Skin {
	  public:
						DListaEx_Skin(void);
						// Colores para el fondo (OJO los colores del fondo y del borde del control est�n en DBarraSroll_Skin)
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
						// Fuente (estos miembros solo se usan al crear la lista, una vez creada si queremos modificar la fuente hay que usar el miembro DListaEx::Fuente)
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

	// Estados del mouse que puede tener un item 
	enum DListaEx_Item_EstadoMouse {
		DListaEx_Item_EstadoMouse_Normal		= 0,
		DListaEx_Item_EstadoMouse_Resaltado		= 1,
		DListaEx_Item_EstadoMouse_Presionado	= 2
	};

	class DDesplegableEx;

	class DListaEx : public virtual DBarraScrollEx {
	  public: //////////////////////////////////////////////////////////// Constructor
																		DListaEx(void);
																		// Destructor
		                                                               ~DListaEx(void);
																		// Funci�n que crea el control ListaEx
		HWND															CrearListaEx(DhWnd *nPadre, const int cX, const int cY, const int cAncho, const int cAlto, const int cID, const DWORD Estilos = WS_CHILD | WS_VISIBLE, const DWORD EstilosExtendidos = NULL);
																		// Funci�n que destruye la lista y libera la mem�ria
		const BOOL														Destruir(void);
																		// Funci�n que agrega una columna a la lista
		DListaEx_Columna							                   *AgregarColumna(const int nAncho = DLISTAEX_COLUMNA_ANCHO_AUTO, DListaEx_Columna_Alineacion nAlineacion = DListaEx_Columna_Alineacion_Izquierda);
																		// Funci�n que elimina todas las columnas de la lista
		void															EliminarTodasLasColumnas(void);
																		// Agrega un item personalizado (por defecto es del tipo DListaEx_Item) para iconos de los recursos
		template <class TItem = DListaEx_Item> TItem                   *AgregarItem(const INT_PTR nIconoRecursos, const INT_PTR PosicionFila, std::initializer_list<std::wstring> Textos) {
																			TItem  *nItem = new TItem();
																			DWL::DIcono Ico(nIconoRecursos, DLISTAEX_TAMICONO, DLISTAEX_TAMICONO);
																			_AgregarItem(nItem, Ico, PosicionFila, Textos);
																			return nItem;
																		};
																		// Agrega un item personalizado (por defecto es del tipo DListaEx_Item) para iconos del sistema
		template <class TItem = DListaEx_Item> TItem                   *AgregarItem(const GUID nIconoKnownFolder, const INT_PTR PosicionFila, std::initializer_list<std::wstring> Textos) {
																			TItem *nItem = new TItem();
																			DWL::DIcono Ico(nIconoKnownFolder, DLISTAEX_TAMICONO, DLISTAEX_TAMICONO);
																			_AgregarItem(nItem, Ico, PosicionFila, Textos);
																			return nItem;
																		};

																		// Agrega un item personalizado (por defecto es del tipo DListaEx_Item) para iconos especificos de un path
		template <class TItem = DListaEx_Item> TItem                   *AgregarItem(const wchar_t *nPathIcono, const int nPosIcono, const INT_PTR PosicionFila, std::initializer_list<std::wstring> Textos) {
																			TItem  *nItem = new TItem();
																			DWL::DIcono Ico(nPathIcono, DLISTAEX_TAMICONO, DLISTAEX_TAMICONO, nPosIcono);
																			_AgregarItem(nItem, Ico, PosicionFila, Textos);
																			return nItem;
																		};
																		// Funci�n que elimina el item especificado por la posici�n
		void															EliminarItem(const LONG_PTR ePosItem);
																		// Funci�n que elimina el item especificado con un DListaEx_Item
		void															EliminarItem(DListaEx_Item *eItem);
																		// Funci�n que elimina los items seleccionados
		const LONG_PTR													EliminarItemsSeleccionados(void);
																		// Funci�n que elimina todos los items
		void															EliminarTodosLosItems(void);
																		// Funci�n que devuelve el item especificado por la posici�n
		inline DListaEx_Item              				               *Item(const LONG_PTR iPos) { if (iPos == -1)            { return NULL; }	return _Items[static_cast<size_t>(iPos)]; }
																		// Funci�n que devuelve el item resaltado con el mouse
		inline DListaEx_Item                 				           *ItemResaltado(void)	  	  { if (_ItemResaltado == -1)  { return NULL; }	return _Items[static_cast<size_t>(_ItemResaltado)]; };
																		// Funci�n que devuelve el item presionado con el mouse
		inline DListaEx_Item            				               *ItemPresionado(void)	  { if (_ItemPresionado == -1) { return NULL; }	return _Items[static_cast<size_t>(_ItemPresionado)]; };
																		// Funci�n que devuelve el item marcado (el que tiene el foco del teclado)
		inline DListaEx_Item            				               *ItemMarcado(void)		  { if (_ItemMarcado == -1)    { return NULL; }	return _Items[static_cast<size_t>(_ItemMarcado)]; };
																		// Funci�n que asigna el item marcado
		void															ItemMarcado(DListaEx_Item *NuevoItemMarcado, const BOOL nRepintar = FALSE);
																		// Devuelve la posici�n del DListaEx_Item que est� dentro de esta lista
		const LONG_PTR													ItemPos(DListaEx_Item *pItem);
																		// Funci�n que repinta la lista
		virtual void													Repintar(const BOOL nForzar = FALSE);
																		// Funci�n que selecciona / des-selecciona el item especificado por la posici�n
		void															SeleccionarItem(const LONG_PTR sPos, const BOOL nSeleccion);
																		// Funci�n que selecciona / des-selecciona el item especificado
		void												            SeleccionarItem(DListaEx_Item *sItem, const BOOL nSeleccion);
																		// Funci�n que des-selecciona todos los items
		void															DesSeleccionarTodo(void);
																		// Funci�n que muestra el item especificado por la posici�n
		void															MostrarItem(const LONG_PTR iPos, const BOOL nRepintar = TRUE);
																		// Funci�n que muestra el item especificado
		void															MostrarItem(DListaEx_Item *eItem);
																		// Funci�n que obtiene el rectangulo que ocupa el item especificado por la posici�n
		const BOOL								                        ObtenerRectaItem(const LONG_PTR iPos, RECT &rRecta);
																		// Funci�n que obtiene el rectangulo que ocupa el item especificado
		inline const BOOL										        ObtenerRectaItem(DListaEx_Item *eItem, RECT &rRecta) { return ObtenerRectaItem(ItemPos(eItem), rRecta); }
																		// Funci�n que devuelve el total de items en la lista
		inline const LONG_PTR											TotalItems(void) { return static_cast<LONG_PTR>(_Items.size()); }
																		// Funci�n que devuelve el total de items seleccionados
		const LONG_PTR													TotalItemsSeleccionados(void);
																		// Funci�n que devuelve el total de columnas
		inline const size_t												TotalColumnas(void) { return _Columnas.size();  }
																		// Funci�n que devuelve la columna especificada por su posici�n
		inline DListaEx_Columna								           *Columna(const LONG_PTR cPos) { return _Columnas[static_cast<unsigned int>(cPos)];  }
																		// Funci�n que pinta la lista en el DC especificado
		void															Pintar(HDC hDC);
																		// Funci�n que pinta el item en el DC y espacio especificados
		void															PintarItem(HDC hDC, const LONG_PTR pPosItem, RECT &Espacio);
																		// Funci�n que devuelve la posici�n de un item utilizando las coordenadas especificadas
		const LONG_PTR													HitTest(const int cX, const int cY, LONG_PTR *PosSubItem = NULL);
																		// Evento que se ejecuta al cambiar la posici�n de las barras de scroll
		void															Scrolls_EventoCambioPosicion(void);
																		// Evento virtual que responde al mensaje WM_MOUSEENTER
		virtual void          											Evento_MouseEntrando(void)																		{ };
																		// Funci�n lambda que responde al mensaje WM_MOUSEENTER
		std::function<void(void)>										EventoMouseEntrando;
																		// Evento virtual que responde al mensaje WM_MOUSELEAVE
		virtual void													Evento_MouseSaliendo(void)																		{ };
																		// Funci�n lambda que responde al mensaje WM_MOUSELEAVE
		std::function<void(void)>										EventoMouseSaliendo;
																		// Evento virtual que responde al mensaje WM_MOUSEMOVE
		virtual void													Evento_MouseMovimiento(DEventoMouse &DatosMouse)												{ };
																		// Funci�n lambda que responde al mensaje WM_MOUSEMOVE
		std::function<void(DEventoMouse&)>								EventoMouseMovimiento;
																		// Evento virtual que responde a los mensajes WM_?MOUSEDOWN
		virtual void													Evento_MousePresionado(DEventoMouse &DatosMouse) 												{ };
																		// Funci�n lambda que responde al mensaje WM_?MOUSEDOWN
		std::function<void(DEventoMouse&)>								EventoMousePresionado;
																		// Evento virtual que responde a los mensajes WM_?MOUSEUP
		virtual void													Evento_MouseSoltado(DEventoMouse &DatosMouse)													{ };
																		// Funci�n lambda que responde al mensaje WM_?MOUSEUP
		std::function<void(DEventoMouse&)>								EventoMouseSoltado;
																		// Evento virtual que responde al mensaje WM_MOUSECLICK
		virtual void													Evento_MouseClick(DEventoMouse &DatosMouse)														{ };
																		// Funci�n lambda que responde al mensaje WM_MOUSECLICK
		std::function<void(DEventoMouse&)>								EventoMouseClick;
																		// Evento virtual que responde al mensaje WM_MOUSEWHEEL
		virtual void                             				        Evento_MouseRueda(DEventoMouseRueda &DatosMouse)												{ };
																		// Funci�n lambda que responde al mensaje WM_MOUSEWHEEL
		std::function<void(DEventoMouseRueda&)>							EventoMouseRueda;
																		// Evento virtual que responde al mensaje WM_?BUTTONDBLCLK
		virtual void													Evento_MouseDobleClick(DEventoMouse &DatosMouse)												{ };
																		// Funci�n lambda que responde al mensaje WM_?BUTTONDBLCLK
		std::function<void(DEventoMouse&)>								EventoMouseDobleClick;
																		// Evento virtual que se ejecuta al terminar una operaci�n de drag & drop
		virtual void													Evento_DragTerminado(void)                                                                      { };
																		// Funci�n lambda que se ejecuta al terminar una operaci�n de drag & drop
		std::function<void(void)>										EventoDragTerminado;
																		// Evento virtual que responde al mensaje WM_KEYDOWN
		virtual void                             				        Evento_TeclaPresionada(DEventoTeclado &DatosTeclado)											{ };
																		// Funci�n lambda que responde al mensaje WM_KEYDOWN
		std::function<void(DEventoTeclado&)>							EventoTeclaPresionada;
																		// Evento virtual que responde al mensaje WM_KEYUP
		virtual void                            				        Evento_TeclaSoltada(DEventoTeclado &DatosTeclado)												{ };
																		// Funci�n lambda que responde al mensaje WM_KEYUP
		std::function<void(DEventoTeclado&)>							EventoTeclaSoltada;
		virtual void													Evento_Tecla(DEventoTeclado &DatosTeclado)														{ };
																		// Funci�n lambda que responde al mensaje WM_CHAR
		std::function<void(DEventoTeclado&)>							EventoTecla;
																		// Evento virtual que responde al mensaje WM_SETFOCUS
		virtual void                         				            Evento_FocoObtenido(HWND hWndUltimoFoco)														{ };
																		// Funci�n lambda que responde al mensaje WM_SETFOCUS
		std::function<void(HWND)>										EventoFocoObtenido;
																		// Evento virtual que responde al mensaje WM_KILLFOCUS
		virtual void                        				            Evento_FocoPerdido(HWND hWndNuevoFoco)															{ };
																		// Funci�n lambda que responde al mensaje WM_KILLFOCUS
		std::function<void(HWND)>										EventoFocoPerdido;
																		// No hay lambda en este evento para no enredar mas la cosa en el pintado... si quieres personalizar el pintado, mejor hereda esta clase y sobre-escribe esta funci�n
		virtual void													Evento_PintarSubItem(HDC hDC, const LONG_PTR NumItem, const LONG_PTR NumSubItem, RECT *Espacio) { };
																		// Variable que determina si hay que mostrar la selecci�n
		BOOL                         				                    MostrarSeleccion;
																		// Variable que determina si se puede seleccionar mas de un item
		BOOL                       				                        MultiSeleccion;
																		// Variable que determina si se pueden arrastrar los items por la lista para modificar su posici�n
		BOOL                      				                        MoverItemsDrag;
																		// Funci�n que devuelve el tipo de DhWnd
		virtual const DhWnd_Tipo										TipoWnd(void) { return DhWnd_Tipo_ListaEx; };
																		// Gestor de mensajes
		LRESULT CALLBACK												GestorMensajes(UINT uMsg, WPARAM wParam, LPARAM lParam);
																		// Objeto que contiene la fuente por defecto de la lista
		DFuente															Fuente;
																		// Objeto que contiene la configuraci�n gr�fica de la lista
		DListaEx_Skin													Skin;
																		// Funci�n que actualiza todos los items de la lista seg�n el skin
		void															ActualizarSkin(void);
	  protected: ///////////////////////////////////////////////////////// Funci�n que responde al mensaje WM_PAINT
		inline void                                                    _Evento_Pintar(void);
																		// Funci�n que responde al mensaje WM_MOUSEMOVE
		void										                   _Evento_MouseMovimiento(WPARAM wParam, LPARAM lParam);
																		// Funci�n que responde a los mensajes WM_?MOUSEDOWN
		void										                   _Evento_MousePresionado(const int Boton, WPARAM wParam, LPARAM lParam);
																		// Funci�n que responde a los mensajes WM_?MOUSEUP
		void										                   _Evento_MouseSoltado(const int Boton, WPARAM wParam, LPARAM lParam);
																		// Funci�n que responde al mensaje WM_MOUSEWHEEL
		void                                                           _Evento_MouseRueda(WPARAM wParam, LPARAM lParam);
																		// Funci�n que responde al mensaje WM_MOUSELEAVE
		void										                   _Evento_MouseSaliendo(void);
																		// Funci�n que responde a los mensajes WM_?BUTTONDBLCLK
		void										                   _Evento_MouseDobleClick(const int Boton, WPARAM wParam, LPARAM lParam);
																		// Funci�n que responde al mensaje WM_KEYDOWN
		void                                                           _Evento_TeclaPresionada(WPARAM wParam, LPARAM lParam);
																		// Funci�n que responde al mensaje WM_KEYUP
		void                                                           _Evento_TeclaSoltada(WPARAM wParam, LPARAM lParam);
																		// Funci�n que responde al mensaje WM_CHAR
		void										                   _Evento_Tecla(WPARAM wParam, LPARAM lParam);
																		// Funci�n que responde al mensaje WM_SETFOCUS
		void										                   _Evento_FocoObtenido(HWND hWndUltimoFoco);
																		// Funci�n que responde al mensaje WM_KILLFOCUS
		void										                   _Evento_FocoPerdido(HWND hWndNuevoFoco);
																		// Funci�n que responde al mensaje WM_TIMER
		void                                                           _Evento_Temporizador(WPARAM wParam);
																		// Funci�n que realiza las operaciones para arrastrar items
		void                                                           _Drag(DEventoMouse& DatosMouse);
																		// Temporizador para arrastrar items hacia arriba (una vez se sobrepasa el �rea cliente)
		UINT_PTR                                                       _TimerDragArriba;
																		// Temporizador para arrastrar items hacia abajo (una vez se sobrepasa el �rea cliente)
		UINT_PTR                                                       _TimerDragAbajo;
																		// Funci�n que sube los items seleccionados
		void                                                           _SubirItemsSeleccionados(void);
																		// Funci�n que baja los items seleccionados
		void                                                           _BajarItemsSeleccionados(void);
																		// Funci�n que se ejecuta al presionar la tecla cursor arriba
		void										                   _Tecla_CursorArriba(DEventoTeclado &DatosTeclado);
																		// Funci�n que se ejecuta al presionar la tecla cursor abajo
		void										                   _Tecla_CursorAbajo(DEventoTeclado &DatosTeclado);
																		// Funci�n que se ejecuta al presionar la tecla inicio
		void										                   _Tecla_Inicio(DEventoTeclado &DatosTeclado);
																		// Funci�n que se ejecuta al presionar la tecla fin
		void										                   _Tecla_Fin(DEventoTeclado &DatosTeclado);
																		// Funci�n que se ejecuta al presionar la tecla avpag
		void										                   _Tecla_AvPag(DEventoTeclado &DatosTeclado);
																		// Funci�n que se ejecuta al presionar la tecla repag
		void										                   _Tecla_RePag(DEventoTeclado &DatosTeclado);
																		// Valor que determina si hay que recalcular los tama�os antes de pintar
		BOOL                                                           _Repintar;
																		// Funci�n que calcula el espacio visible en la lista
		void									                	   _CalcularTotalEspacioVisible(RECT &RC);
																		// Funci�n que calcula los scrolls
		void										                   _CalcularScrolls(void);
																		// Funci�n que calcula las columnas
		void                                                           _CalcularColumnas(void);
																		// Funci�n que calcula los items que caben en la p�gina
		void                                                           _CalcularItemsPagina(const size_t TamPagina);
																		// Funci�n que calcula si hay que pintar iconos
		void                                                           _CalcularPintarIconos(void);
																		// Funci�n protegida para agregar items
		DListaEx_Item                                                 *_AgregarItem(DListaEx_Item *nItem, DIcono &nIcono, const INT_PTR PosicionFila, std::initializer_list<std::wstring> Textos);
																		// Vector de items de la lista
		std::vector<DListaEx_Item *>                                   _Items;
																		// Vector de columnas de la lista
		std::vector<DListaEx_Columna *>                                _Columnas;
																		// Tiempo que lleva presionado el item (para hacer drag)
		ULONGLONG                                                      _TiempoItemPresionado; 
																		// Posici�n del item resaltado
		LONG_PTR                                                       _ItemResaltado;
																		// Posici�n del ultimo item resaltado
		LONG_PTR	                                                   _ItemUResaltado;
																		// Es en n�mero de columa partiendo de la _FilaRsaltada
		LONG_PTR                                                       _SubItemResaltado; 
																		// Posici�n del ultimo sub-item resaltado
		LONG_PTR								                	   _SubItemUResaltado;
																		// Posici�n del item presionado
		LONG_PTR			                                           _ItemPresionado;
																		// Item presionado
		DListaEx_Item                                                 *_PItemPresionado;
																		// Posici�n del sub item presionado
		LONG_PTR			                                           _SubItemPresionado;
																		// Posici�n del item marcado
		LONG_PTR			                                           _ItemMarcado;
																		// Posici�n del item donde se ha presionado el shift
		LONG_PTR					                                   _ItemShift;
																		// Diferencia de pixeles inicial entre la _FilaInicioPagina y el inicio de la p�gina visible
		LONG                                                           _ItemPaginaVDif;
											                			// Diferencia de pixeles entre .... 
		LONG                                                           _ItemPaginaHDif;
																		// Posici�n del item donde empieza la p�gina
		LONG_PTR                                                       _ItemPaginaInicio;
																		// Posici�n del item donde termina la p�gina
		LONG_PTR                                                       _ItemPaginaFin;
																		// Total de ancho visible en pixeles
		size_t                                                         _TotalAnchoVisible;
																		// Total de altura visible en pixeles
		size_t								                		   _TotalAltoVisible;
																		// Funci�n que crea un backbuffer donde se pintaran los items
		void									              		   _CrearBufferItem(const int nAncho, const int nAlto);
																		// Buffer DC permanente para pintar UNA fila
		HDC                                                            _BufferItem;
																		// Bitmap del buffer del DC permante
		HBITMAP                                                        _BufferItemBmp;
																		// Bitmap viejo del buffer del DC
		HBITMAP                                                        _BufferItemBmpViejo;
																		// Fuente vieja del buffer del DC
		HFONT                                                          _BufferItemFuenteViejo;
																		// Variable que determina si hay que pintar los iconos
		BOOL                                                           _PintarIconos;
																		// Variable que determina si se est� ejecutando una operaci�n de arrastrar y soltar
		BOOL								                		   _EnDrag;

		friend class DListaEx_Columna;
		friend class DListaEx_Item;
		friend class DListaEx_SubItem;

		friend class DDesplegableEx;

	};

}
#endif