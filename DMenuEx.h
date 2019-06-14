#ifndef DMENUEX_H
#define DMENUEX_H

#include "DVentana.h"
#include "DBarraDesplazamientoEx.h"
#include "DFuente.h"
#include "DEventoTeclado.h"
#include "DIcono.h"

namespace DWL {

	class DMenuEx_Skin {
	  public:
							DMenuEx_Skin(void) { };
							// Colores por defecto
		static COLORREF     FondoNormal;
		static COLORREF     FondoResaltado;
		static COLORREF     FondoPresionado;
		static COLORREF     TextoNormal;
		static COLORREF     TextoResaltado;
		static COLORREF     TextoPresionado;
		static COLORREF     TextoDesactivado;
		static COLORREF     TextoSombra;
		static COLORREF     Borde;
		static COLORREF     Separador;
							// Fuente
		static int			FuenteTam;
		static std::wstring FuenteNombre;
		static BOOL         FuenteNegrita;
		static BOOL         FuenteCursiva;
		static BOOL         FuenteSubrayado;
		static BOOL			FuenteSombraTexto;
	};

	#define DMENUEX_MOSTRARDEBUG	FALSE

	#define DMENUEX_TAMICONO  16	// Tamaño de los iconos
	#define DMENUEX_MARGEN_X   6	// Margen horizontal para el texto 
	#define DMENUEX_MARGEN_Y   4	// Margen vertical para el texto
    #define DMENUEX_BORDE      1    // Tamaño del borde

	enum DMenuEx_Tipo {
		DMenuEx_Tipo_Raiz      = 0,
		DMenuEx_Tipo_Texto     = 1,
		DMenuEx_Tipo_Barra     = 2,
		DMenuEx_Tipo_Separador = 3
	};

	/*enum DMenuEx_Estado {
		DMenuEx_Estado_Normal		= 0,
		DMenuEx_Estado_Resaltado	= 1,
		DMenuEx_Estado_Presionado	= 2,
		DMenuEx_Estado_Desactivado  = 3
	};*/

	enum DMenuEx_Transicion {
		DMenuEx_Transicion_Normal,
		DMenuEx_Transicion_Resaltado,
		DMenuEx_Transicion_Presionado,
		DMenuEx_Transicion_Desactivado
	};


	#define DMENUEX_TAMICONO				16 // Tamaño del icono
	#define DMENUEX_ANCHOBARRA             100 // Ancho mínimo de las barras
	#define WM_ESMENUEX			 WM_USER + 500

	class DMenuEx : protected DWL::DVentana {
  	  public:
								DMenuEx(void);
							   ~DMenuEx(void);
							    // Agrega un menú con texto y icono en la posicion especificada (-1 es al final)
		DMenuEx                *AgregarMenu(const INT_PTR nID, const wchar_t *nTexto, const INT_PTR nIconoRecursos = NULL, const int Posicion = -1, const BOOL nActivado = TRUE);
		inline DMenuEx         *AgregarMenu(const INT_PTR nID, std::wstring &nTexto, const INT_PTR nIconoRecursos = NULL, const int Posicion = -1, const BOOL nActivado = TRUE) { return AgregarMenu(nID, nTexto.c_str(), nIconoRecursos, Posicion, nActivado); };
		DMenuEx				   *AgregarSeparador(const INT_PTR uID = 0, const int Posicion = -1);
		DMenuEx                *AgregarBarra(const INT_PTR nID, const wchar_t *nTexto, const INT_PTR nIconoRecursos = NULL, const float nMinimo = 0.0f, const float nMaximo = 1.0f, const float nValor = 0.0f, const DBarraEx_MostrarValor nMostrarValor = DBarraEx_MostrarValor_Nada, const int Posicion = -1, const BOOL nActivado = TRUE);

								// Función que muestra el Menú en el bucle principal de la aplicación y espera su respuesta en un DWL_MENUEX_CLICK (NO DEVUELVE EL MENÚ PRESIONADO)
		void					Mostrar(DhWnd *nPadre, int PosX, int PosY);
		void					Mostrar(DhWnd *nPadre);
								// Función que muestra el Menú y espera a que se elija una opción para devolver-la
								//	NOTA : No desactiva la ventana padre
		DMenuEx			       *MostrarModal(DhWnd *nPadre, const int PosX, const int PosY);
		DMenuEx			       *MostrarModal(DhWnd *nPadre);

		DMenuEx                *BuscarMenu(const INT_PTR bID);

								// Asigna un icono a este menú 
		void                    Icono(const int nIconoRecursos);
		inline void             Icono(DIcono &nIcono)	{ _Icono = nIcono; }

								// Funciones para navegar entre menus
		inline DMenuEx         *Menu(const size_t Pos)				{ return _Menus[Pos]; }
		inline const size_t		TotalMenus(void)					{ return _Menus.size(); }
		inline DMenuEx         *MenuPadre(void)						{ return _Padre; }

		inline const INT_PTR    ID(void)							{ return _ID; }

								// Función para obtener el texto del menú
		inline std::wstring    &Texto(void)							{ return _Texto; }
								// Funciones para asignar el texto al menú
		void					Texto(const wchar_t *nTxt);
		void					Texto(std::wstring &nTxt);

								// Funciónes que devuelven/asignan si el menú está activado
		inline const BOOL		Activado(void)						{ return _Activado; };
		void					Activado(const BOOL nActivar);

								// Función para ocutar los menus
		void                    Ocultar(const BOOL OcultarTodos = FALSE);		

								// Función para eliminar el menu especificado por la ID
		void                    EliminarMenu(const INT_PTR eID);
								// Función para eliminar el menu especificado por el objeto
		void					EliminarMenu(DMenuEx *eMenu);
								// Función para eliminar todos los menus
		void                    EliminarTodosLosMenus(void);

		void                    Pintar(HDC hDC);		

		void					Terminar(void);
								// Función para calcular el tamaño de los menus.
		const POINT			    CalcularEspacio(void);

		const BOOL				Visible(void);

		inline void				BarraValor(const float nValor) { _Barra.Valor(nValor); };
		inline const float		BarraValor(void) { return _Barra.Valor(); };

		void                    Transicion(const DMenuEx_Transicion nTransicion);

		DFuente                 Fuente;

								// Variable para almacenar un parametro
		INT64					Parametro;
	  protected:
								// Constructor menú tipo texto (interno AgregarBarra)
								DMenuEx(DMenuEx *nPadre, DMenuEx_Tipo nTipo, DhWnd *nhWndPadre, const INT_PTR nID, const wchar_t *nTexto, const INT_PTR nIconoRecursos, const BOOL nActivado, const float nMinimo, const float nMaximo, const float nValor);
								// Constructor menú tipo texto (interno AgregarMenu)
								DMenuEx(DMenuEx *nPadre, DMenuEx_Tipo nTipo, DhWnd *nhWndPadre, const INT_PTR nID, const wchar_t *nTexto, const INT_PTR nIconoRecursos = 0, const BOOL nActivado = TRUE);
								// Constructor menú tipo separador (interno AgregarSeparador)
								DMenuEx(DMenuEx *nPadre, DMenuEx_Tipo nTipo, DhWnd *nhWndPadre, const INT_PTR nID);

		void				   _OcultarRecursivo(DMenuEx *oMenu);
								// Función que muestra este menú como un submenú
		void                   _MostrarSubMenu(DMenuEx *nPadre, int cX, int cY, const BOOL AsignarFoco = TRUE);
								// Función que pinta una fila del menú (devuelve la altura de lo pintado)
		void                   _PintarMenu(HDC DC, DMenuEx *pMenu);
								// Función que pinta el separador
		void                   _PintarSeparador(HDC DC, DMenuEx *pMenu);
								// Función que pinta el expansor (flecha que indica que contiene uno o mas sub-menus)
		void                   _PintarExpansor(HDC DC, const int eX, const int eY);
								// Eventos internos
		void                   _Evento_Pintar(void);
//		void                   _Evento_PintarNC(HRGN Region);
		void				   _Evento_MouseMovimiento(WPARAM wParam, LPARAM lParam);
		void				   _Evento_MousePresionado(const int Boton, WPARAM wParam, LPARAM lParam);
		void				   _Evento_MouseSoltado(const int Boton, WPARAM wParam, LPARAM lParam);
		void				   _Evento_MouseSaliendo(void);
		void                   _Evento_TeclaPresionada(WPARAM wParam, LPARAM lParam);
		void                   _Evento_TeclaSoltada(WPARAM wParam, LPARAM lParam);
		void				   _Evento_Tecla(WPARAM wParam, LPARAM lParam);
		void                   _Evento_FocoPerdido(HWND UltimoFoco);
		void                   _Evento_FocoObtenido(HWND UltimoFoco);
		void				   _Evento_BarraEx_Cambiando(DEventoMouse &DatosMouse);
		void				   _Evento_BarraEx_Cambiado(DEventoMouse &DatosMouse);

		RECT                   _Recta;

		LRESULT CALLBACK        GestorMensajes(UINT uMsg, WPARAM wParam, LPARAM lParam);
		std::vector<DMenuEx *> _Menus;
		INT_PTR                _ID;
		std::wstring           _Texto;
		DIcono	               _Icono;
		DMenuEx               *_Padre;

		DMenuEx               *_MenuResaltado;
		DMenuEx               *_MenuDesplegado;
		DMenuEx               *_MenuPresionado;

		int                    _MargenI; // Margen izquierdo para el texto (que incluye el ancho del icono si alguno de los submenus tiene icono)

		BOOL                   _Activado;
		DMenuEx_Tipo           _Tipo;
		static DhWnd          *_hWndDest;  // Destino para los mensajes
//		HWND                   _hWndPadre; // Ventana padre que contiene el hWndDest;
		DAnimacion             _AniTransicion;
		DAnimacion             _AniMostrar;

		DBarraDesplazamientoEx _Barra;
		int                    _BarraPosX;

		COLORREF               _ColorFondo;
		COLORREF               _ColorTexto;

//		BOOL				   _AnularMouseMove;
								// Menu resultado para la función MostrarModal
		static DMenuEx        *_ResultadoModal;
	};
};

#endif