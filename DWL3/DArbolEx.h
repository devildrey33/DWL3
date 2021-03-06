#pragma once

//#include "RAVE.h"
#include "DBarraScrollEx.h"
#include "DArbolEx_Nodo.h"
#include "DIcono.h"
#include "DEventoMouseRueda.h"
#include "DEventoTeclado.h"
//#include "DFuente.h"

namespace DWL {

	#define DARBOLEX_MOSTRARDEBUG	FALSE

#ifdef _WIN64
	#define DARBOLEX_POSICIONNODO_INICIO	0
	#define DARBOLEX_POSICIONNODO_FIN		_UI64_MAX - 1
	#define DARBOLEX_POSICIONNODO_ORDENADO	_UI64_MAX - 2
#else
	#define DARBOLEX_POSICIONNODO_INICIO	0
	#define DARBOLEX_POSICIONNODO_FIN		_UI32_MAX - 1
	#define DARBOLEX_POSICIONNODO_ORDENADO	_UI32_MAX - 2
#endif
	// Objeto con la configuraci�n de colores y fuente por defecto para el arbol
	class DArbolEx_Skin {
	  public :
						DArbolEx_Skin(void);
						// Colores del fondo para los nodos (OJO el color del fondo del arbol est� en (DBarraScrollEx_Skin::FondoNormal / DBarraScrollEx_Skin::FondoResaltado)
		COLORREF		FondoNodoResaltado;
		COLORREF		FondoNodoSeleccionado;
		COLORREF		FondoNodoSeleccionadoResaltado;
		COLORREF		FondoNodoSubSeleccionado;
		COLORREF		FondoNodoSubSeleccionadoResaltado;
		COLORREF		FondoNodoPresionado;
						// Color del borde del nodo marcado
		COLORREF		BordeNodoMarcado;
						// Colores para el expansor
		COLORREF		ExpansorNormal;
		COLORREF		ExpansorResaltado;
		COLORREF		ExpansorPresionado;
						// Colores para el texto
		COLORREF		TextoNodoNormal;
		COLORREF		TextoNodoResaltado;
		COLORREF		TextoNodoDesactivado;
		COLORREF		TextoNodoSombra;
		COLORREF		TextoNodoSeleccionado;
		COLORREF		TextoNodoSeleccionadoSombra;
		COLORREF		TextoNodoSeleccionadoResaltado;
		COLORREF		TextoNodoSubSeleccionado;
		COLORREF		TextoNodoSubSeleccionadoSombra;
		COLORREF		TextoNodoSubSeleccionadoResaltado;
		COLORREF		TextoNodoPresionado;
						// Fuente (estos miembros solo se usan al crear el arbol, una vez creado si queremos modificar la fuente hay que usar el miembro DArbolEx::Fuente)
		int				FuenteTam;
		std::wstring	FuenteNombre;
		BOOL			FuenteNegrita;
		BOOL			FuenteCursiva;
		BOOL			FuenteSubrayado;
		BOOL			FuenteSombraTexto;
	};

	// Posicionamiento 
	enum DArbolEx_PosicionNodo {
		DArbolEx_PosicionNodo_Inicio,
		DArbolEx_PosicionNodo_Fin,
		DArbolEx_PosicionNodo_Ordenado,
	};

	// Parte del nodo al hacer el hittest
	enum DArbolEx_HitTest {
		DArbolEx_HitTest_Icono,
		DArbolEx_HitTest_Texto,
		DArbolEx_HitTest_Expansor,
		DArbolEx_HitTest_Todo		// (expansor, icono y texto)
	};

	// Parte del nodo
	enum DArbolEx_ParteNodo {
		DArbolEx_ParteNodo_Nada		= 0,
		DArbolEx_ParteNodo_Expansor = 1,
		DArbolEx_ParteNodo_Nodo		= 2,
		DArbolEx_ParteNodo_Icono	= 3,
		DArbolEx_ParteNodo_Texto	= 4
	};

	// Tipo de expansor
	enum DArbolEx_Expansor {
		DArbolEx_Expansor_Rectangulo     = 0,
		DArbolEx_Expansor_Triangulo		 = 1,
		DArbolEx_Expansor_TrianguloLinea = 2
	};

	class DDesplegableEx;


	/* Plantilla ArbolEx que se crea partiendo del tipo de nodo */
	class DArbolEx : public virtual DBarraScrollEx {
	  public: //////////////////////////////////////////// Constructor
														DArbolEx(void);
														// Destructor
		                                               ~DArbolEx(void);
														// Crea el control ArbolEx
		HWND											CrearArbolEx(DhWnd *nPadre, const int cX, const int cY, const int cAncho, const int cAlto, const int cID, DWORD nEstilos = WS_CHILD);
														// Funcion que destruye el arbol y libera su mem�ria
		const BOOL										Destruir(void);
														// Funci�n para expandir / contraer un nodo
		void											Expandir(DArbolEx_Nodo *nNodo, const BOOL nExpandir);
														// Obtiene el nodo de la posici�n especificada (El m�ximo de hijos se obtiene con la funci�n 'const size_t TotalHijos')
		inline DArbolEx_Nodo                           *Nodo(const size_t nPos) { return _Raiz.Hijo(nPos); };
														// Nodo raiz
		inline DArbolEx_Nodo                           *NodoRaiz(void)			{ return &_Raiz; }
														// Funci�n que devuelve el total de nodos
		inline const size_t								TotalNodos(void)		{ return _Raiz._Hijos.size(); }
														// Funci�n que devuelve el total de nodos seleccionados
		const size_t                                    TotalNodosSeleccionados(void);
														// Agrega un nodo personalizado (por defecto es del tipo DArbolEx_Nodo) para iconos de los recursos
		template <class TNodo = DArbolEx_Nodo> TNodo   *AgregarNodo(const TCHAR *nTexto, DArbolEx_Nodo *nPadre = NULL, const int nIcono = NULL, DFuente *nFuente = NULL, const size_t PosicionNodo = DARBOLEX_POSICIONNODO_FIN) {
															DIcono Ico; Ico.CrearIconoRecursos(nIcono, DARBOLEX_TAMICONO, DARBOLEX_TAMICONO);
															TNodo *nNodo = new TNodo();
															_AgregarNodo(nNodo, nTexto, nPadre, &Ico, nFuente, PosicionNodo);
															return nNodo;
														};
														// Agrega un nodo personalizado (por defecto es del tipo DArbolEx_Nodo) para iconos del sistema
		template <class TNodo = DArbolEx_Nodo> TNodo   *AgregarNodo(const TCHAR *nTexto, DArbolEx_Nodo *nPadre, const GUID &KnowFolderId, DFuente *nFuente = NULL, const size_t PosicionNodo = DARBOLEX_POSICIONNODO_FIN) {
															DIcono Ico; Ico.CrearIconoKnownFolder(KnowFolderId, DARBOLEX_TAMICONO, DARBOLEX_TAMICONO);
															TNodo *nNodo = new TNodo();
															_AgregarNodo(nNodo, nTexto, nPadre, &Ico, nFuente, PosicionNodo);
															return nNodo;
														};
														// Agrega un nodo personalizado (por defecto es del tipo DArbolEx_Nodo) para iconos especificos de un path
		template <class TNodo = DArbolEx_Nodo> TNodo   *AgregarNodo(const TCHAR *nTexto, DArbolEx_Nodo *nPadre, const wchar_t *PathIcono, const int nPosIcono = 0, DFuente *nFuente = NULL, const size_t PosicionNodo = DARBOLEX_POSICIONNODO_FIN) {
															DIcono Ico; Ico.CrearIconoExterno(PathIcono, DARBOLEX_TAMICONO, DARBOLEX_TAMICONO, nPosIcono);
															TNodo *nNodo = new TNodo();
															_AgregarNodo(nNodo, nTexto, nPadre, &Ico, nFuente, PosicionNodo);
															return nNodo;
														};
														// Primer nodo visible del arbol (devuelve _Raiz[0] o NULL)
		DArbolEx_Nodo								   *PrimerNodoVisible(void);
														// Ultimo nodo visible del arbol (devuelve el �ltimo nodo del arbol que su padre tenga el miembro _Expandido a TRUE)
		DArbolEx_Nodo								   *UltimoNodoVisible(void);
														// Eliminar Nodo
		void											EliminarNodo(DArbolEx_Nodo *nEliminar);
														// Elimina todos los nodos
		void											BorrarTodo(void);
														// Funci�n que pinta el arbol en el DC especifcado
		void											Pintar(HDC hDC);
														// Funci�n que repinta el arbol 
		virtual void									Repintar(const BOOL nForzar = FALSE);
														// Funci�n que pinta el nodo
		void											PintarNodo(HDC hDC, RECT *Espacio, DArbolEx_Nodo *nNodo, const int PosH);
														// Funci�n que pinta el expansor cuadrado
		void                                            PintarExpansorRectangulo(HDC hDC, RECT *Espacio, DArbolEx_Nodo *nNodo, const int AnchoOcupado);
														// Funci�n que pinta el expansor triangular
		void                                            PintarExpansorTriangulo(HDC hDC, RECT *Espacio, DArbolEx_Nodo *nNodo, const int AnchoOcupado);
														// Funci�n que pinta el borde del expansor triangular
		void                                            PintarExpansorTrianguloLinea(HDC hDC, RECT *Espacio, DArbolEx_Nodo *nNodo, const int AnchoOcupado);
														// Evento del scroll cambio de posici�n
		void											Scrolls_EventoCambioPosicion(void);
														// Evento virtual que responde al mensaje WM_MOUSEENTER
		virtual void          							Evento_MouseEntrando(void)																	{ };
														// Funci�n lambda que responde al mensaje WM_MOUSEENTER
		std::function<void(void)>						EventoMouseEntrando;
														// Evento virtual que responde al mensaje WM_MOUSELEAVE
		virtual void									Evento_MouseSaliendo(void)																	{ };
														// Funci�n lambda que responde al mensaje WM_MOUSELEAVE
		std::function<void(void)>						EventoMouseSaliendo;
														// Evento virtual que responde al mensaje WM_MOUSEMOVE
		virtual void									Evento_MouseMovimiento(DEventoMouse &DatosMouse)											{ };
														// Funci�n lambda que responde al mensaje WM_MOUSEMOVE
		std::function<void(DEventoMouse &)>				EventoMouseMovimiento;
														// Evento virtual que responde a los mensajes WM_?MOUSEDOWN
		virtual void									Evento_MousePresionado(DEventoMouse &DatosMouse)											{ };
														// Funci�n lambda que responde a los mensajes WM_?MOUSEDOWN
		std::function<void(DEventoMouse &)>				EventoMousePresionado;
														// Evento virtual que responde a los mensajes WM_?MOUSEUP
		virtual void									Evento_MouseSoltado(DEventoMouse &DatosMouse)												{ };
														// Funci�n lambda que responde a los mensajes WM_?MOUSEUP
		std::function<void(DEventoMouse&)>				EventoMouseSoltado;
														// Evento virtual que responde al mensaje WM_MOUSECLICK
		virtual void									Evento_MouseClick(DEventoMouse &DatosMouse)													{ };
														// Funci�n lambda que responde al mensaje WM_MOUSECLICK
		std::function<void(DEventoMouse&)>				EventoMouseClick;
														// Evento virtual que responde al mensaje WM_MOUSEWHEEL
		virtual void                                    Evento_MouseRueda(DEventoMouseRueda &DatosMouse)											{ };
														// Funci�n lambda que responde al mensaje WM_MOUSEWHEEL
		std::function<void(DEventoMouseRueda&)>			EventoMouseRueda;
														// Evento virtual que responde al mensaje WM_?BUTTONDBLCLK
		virtual void									Evento_MouseDobleClick(DEventoMouse &DatosMouse)											{ };
														// Funci�n lambda que responde al mensaje WM_?BUTTONDBLCLK
		std::function<void(DEventoMouse&)>				EventoMouseDobleClick;
														// Evento virtual que responde al mensaje WM_KEYDOWN
		virtual void                                    Evento_TeclaPresionada(DEventoTeclado &DatosTeclado)										{ };
														// Funci�n lambda que responde al mensaje WM_KEYDWON
		std::function<void(DEventoTeclado&)>			EventoTeclaPresionada;
														// Evento virtual que responde al mensaje WM_KEYUP
		virtual void                                    Evento_TeclaSoltada(DEventoTeclado &DatosTeclado)											{ };
														// Funci�n lambda que responde al mensaje WM_KEYUP
		std::function<void(DEventoTeclado&)>			EventoTeclaSoltada;
														// Evento virtual que responde al mensaje WM_CHAR
		virtual void									Evento_Tecla(DEventoTeclado &DatosTeclado)													{ };
														// Funci�n lambda que responde al mensaje WM_CHAR
		std::function<void(DEventoTeclado&)>			EventoTecla;
														// Evento virtual que responde al mensaje WM_SETFOCUS
		virtual void                                    Evento_FocoObtenido(HWND hWndUltimoFoco)													{ };
														// Funci�n lambda que responde al mensaje WM_SETFOCUS
		std::function<void(HWND)>						EventoFocoObtenido;
														// Evento virtual que responde al mensaje WM_KILLFOCUS
		virtual void                                    Evento_FocoPerdido(HWND hWndNuevoFoco)														{ };
														// Funci�n lambda que responde al mensaje WM_KILLFOCUS
		std::function<void(HWND)>						EventoFocoPerdido;
														// Al expandir / contraer un nodo
		virtual void									Evento_Nodo_Expandido(DWL::DArbolEx_Nodo *nNodo, const BOOL nExpandido)						{ };
														// Funci�n que devuelve el nodo en la posici�n especificada
		DArbolEx_Nodo                                  *HitTest(const int cX, const int cY, DArbolEx_ParteNodo &Parte);
														// Funci�n que muestra el nodo especificado
		void											MostrarNodo(DArbolEx_Nodo *vNodo);
														// Funci�n que devuelve el �rea del nodo
		const BOOL										ObtenerRectaNodo(DArbolEx_Nodo *rNodo, RECT &rRecta);
														// Funci�n que selecciona / desselecciona el nodo especificado
		void											SeleccionarNodo(DArbolEx_Nodo *sNodo, const BOOL nSeleccionado);
														// Funci�n que selecciona los nodos entre Desde y Hasta (incluyendo ambos), y des-selecciona todos los dem�s nodos
		void											SeleccionarNodosShift(DArbolEx_Nodo *Desde, DArbolEx_Nodo *Hasta);
														// Variable que permite habilitar / des-habilitar la multiselecci�n
		BOOL											MultiSeleccion;
														// Variable que permite habilitar / des-habilitar la subsleecci�n
		BOOL											SubSeleccion;
														// Funci�n que des-selecciona todos los nodos
		void											DesSeleccionarTodo(void);
														// Busca el nodo anterior
		DArbolEx_Nodo								   *BuscarNodoAnterior(DArbolEx_Nodo *nActual, const BOOL nVisible = FALSE);
														// Busca el nodo siguiente
		DArbolEx_Nodo								   *BuscarNodoSiguiente(DArbolEx_Nodo *nActual, const BOOL nVisible = FALSE, DArbolEx_Nodo *DentroDe = NULL);
														// Fuente que se usara en el arbol 
		DFuente					                        Fuente;
														// Funci�n que devuelve el nodo resaltado
		inline DArbolEx_Nodo                           *NodoResaltado(void)										{ return _NodoResaltado; };
														// Funci�n que devuelve el nodo marcado
		inline DArbolEx_Nodo                           *NodoMarcado(void)										{ return _NodoMarcado; };
														// Funci�n que asigna el nodo marcado
		inline void                                     NodoMarcado(DArbolEx_Nodo *nNodoMarcado)				{ _NodoMarcado = nNodoMarcado; };
														// Funci�n que devuelve el tipo de expansor para el arbol
		inline const DArbolEx_Expansor					ExpansorPorDefecto(void)								{ return _ExpansorPorDefecto; };
														// Funci�n que asigna el tipo de expansor para el arbol
		void                                            ExpansorPorDefecto(const DArbolEx_Expansor nExpansor)	{ _ExpansorPorDefecto = nExpansor; }
														// Funci�n que devuelve el tipo de DhWnd
		virtual const DhWnd_Tipo						TipoWnd(void) { return DhWnd_Tipo_ArbolEx; };
														// Gestor de mensajes
		LRESULT CALLBACK								GestorMensajes(UINT uMsg, WPARAM wParam, LPARAM lParam);
														// Objeto con los colores para el arbol
		DArbolEx_Skin									Skin;
														// Funci�n que actualiza todos los nodos del arbol en base al objeto Skin
		void                                            ActualizarSkin(void);
	  protected: ///////////////////////////////////////// Expansor para el arbol		
		DArbolEx_Expansor							   _ExpansorPorDefecto;
														// Valor que determina si hay que recalcular los tama�os antes de pintar
		BOOL                                           _Repintar;
														// Funci�n que responde al mensaje WM_PAINT
		void										   _Evento_Pintar(void);
														// Funci�n que responde al mensaje WM_MOUSEMOVE
		void										   _Evento_MouseMovimiento(WPARAM wParam, LPARAM lParam);
														// Funci�n que responde a los mensajes WM_?MOUSEDOWN
		void										   _Evento_MousePresionado(const int Boton, WPARAM wParam, LPARAM lParam);
														// Funci�n que responde a los mensajes WM_?MOUSEUP
		void										   _Evento_MouseSoltado(const int Boton, WPARAM wParam, LPARAM lParam);
														// Funci�n que responde al mensaje WM_MOUSEWHEEL
		void                                           _Evento_MouseRueda(WPARAM wParam, LPARAM lParam);
														// Funci�n que responde al mensaje WM_MOUSEDBLCLICK
		void										   _Evento_MouseDobleClick(const int Boton, WPARAM wParam, LPARAM lParam);
														// Funci�n que responde al mensaje WM_MOUSELEAVE
		void										   _Evento_MouseSaliendo(void);
														// Funci�n que responde al mensaje WM_KEYDOWN
		void                                           _Evento_TeclaPresionada(WPARAM wParam, LPARAM lParam);
														// Funci�n que responde al mensaje WM_KEYUP
		void                                           _Evento_TeclaSoltada(WPARAM wParam, LPARAM lParam);
														// Funci�n que responde al mensaje WM_CHAR
		void										   _Evento_Tecla(WPARAM wParam, LPARAM lParam);
														// Funci�n que responde al mensaje WM_SETFOCUS
		void										   _Evento_FocoObtenido(HWND hWndUltimoFoco);
														// Funci�n que responde al mensaje WM_KILLFOCUS
		void										   _Evento_FocoPerdido(HWND hWndNuevoFoco);
														// Funci�n que aplica el shift desde el nodo marcado al nodo resaltado
		void                                           _AplicarShift(const LONG nPosShift);
														// Funci�n que crea el buffer de pintado para un nodo
		void										   _CrearBufferNodo(const int nAncho, const int nAlto);
														// Buffer DC permanente para pintar UN nodo
		HDC                                            _BufferNodo;
														// Bitmap asociado al DC permanente
		HBITMAP                                        _BufferNodoBmp;
														// Bitmap viejo del DC permanente
		HBITMAP                                        _BufferNodoBmpViejo;
														// Funci�n que procesa la tecla cursor arriba
		void										   _Tecla_CursorArriba(const BOOL TeclaShift);
														// Funci�n que procesa la tecla cursor abajo
		void										   _Tecla_CursorAbajo(const BOOL TeclaShift);
														// Funci�n que procesa la tecla inicio
		void										   _Tecla_Inicio(const BOOL TeclaShift);
														// Funci�n que procesa la tecla fin
		void										   _Tecla_Fin(const BOOL TeclaShift);
														// Funci�n que procesa la tecla avpag
		void										   _Tecla_AvPag(const BOOL TeclaShift);
														// Funci�n que procesa la tecla repag
		void										   _Tecla_RePag(const BOOL TeclaShift);
														// Funci�n que calcula los nodos que caben en la p�gina
	    void					  					   _CalcularNodosPagina(const size_t TamPagina);
														// Funci�n que calcula las barras de scroll
		void										   _CalcularScrolls(void);
														// Agrega un nodo (se tiene que reservar mem�ria en la variable nNodo antes de agregar. ej nNodo = new DArbolEx_Nodo)
		DArbolEx_Nodo                                 *_AgregarNodo(DArbolEx_Nodo *nNodo, const TCHAR *nTexto, DArbolEx_Nodo *nPadre = NULL, DIcono *nIcono = NULL, DFuente *nFuente = NULL, const size_t PosicionNodo = DARBOLEX_POSICIONNODO_FIN);
														// Obtiene el espacio en pixeles que necesita todo el arbol tal y como est�n los nodos expandidos
		void									       _CalcularTotalEspacioVisible(void);
														// Total de ancho necesario para los nodos visibles
		size_t										   _TotalAnchoVisible;
														// Total de alto necesario para los nodos visibles
		size_t										   _TotalAltoVisible;
														// M�ximo de altura para un nodo VISIBLE (para crear el BufferDC que se usa para pintar los nodos individualmente)
		size_t                                         _MaxAltoNodo;
														// Raiz que contiene todos los nodos
		DArbolEx_Nodo	                               _Raiz;
														// Nodo que tiene el foco del teclado
		DArbolEx_Nodo	                              *_NodoMarcado;
														// Nodo desde el que se ha presionado la tecla shift
		DArbolEx_Nodo							      *_NodoShift;
														// Posiciones que se ha desplazado el shift
		LONG                                           _PosShift;
														// Nodo presionado
		DArbolEx_Nodo                                 *_NodoPresionado;
														// Parte del nodo presionado
		DArbolEx_ParteNodo                             _NodoPresionadoParte;
														// Nodo resaltado
		DArbolEx_Nodo                                 *_NodoResaltado;
														// Parte del nodo resaltado
		DArbolEx_ParteNodo                             _NodoResaltadoParte;
														// Ultimo nodo resaltado
		DArbolEx_Nodo                                 *_NodoUResaltado;
														// Parte del ultimo nodo resaltado
		DArbolEx_ParteNodo                             _NodoUResaltadoParte;
														// Primer nodo visible en la p�gina
		DArbolEx_Nodo                                 *_NodoPaginaInicio;
														// Ultimo nodo visible en la p�gina
		DArbolEx_Nodo                                 *_NodoPaginaFin;
														// Diferencia de pixeles vertical del nodo inicial y el principio de la p�gina (o es 0 o es un valor negativo)
		LONG										   _NodoPaginaVDif;
														// Diferencia de pixeles horizontal del nodo inicial y el principio de la p�gina (o es 0 o es un valor negativo)
		LONG										   _NodoPaginaHDif;
														// String que almacena los caracteres introducidos en el teclado por un tiempo limitado
		std::wstring                                   _TecladoTmp;
														// Tick de cuando se ha presionado la ultima tecla
		ULONGLONG                                      _TiempoTecladoTmp;
														// Funci�n que comprara 2 strings para ver si son iguales (solo se compara hasta el tama�o del string mas peque�o)
		const BOOL                                     _StringEmpiezaPor(std::wstring &String1, std::wstring &String2);
														// Nodo de la ultima busqueda
		DArbolEx_Nodo                                 *_NodoUltimaBusqueda;


		friend class DArbolEx_Nodo;
		friend class DDesplegableEx;
	};


}

//#include "DArbolEx.tpp"
