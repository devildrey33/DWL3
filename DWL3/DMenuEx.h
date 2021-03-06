#ifndef DMENUEX_H
#define DMENUEX_H

#include "DVentana.h"
#include "DBarraDesplazamientoEx.h"
#include "DFuente.h"
#include "DEventoTeclado.h"
#include "DIcono.h"

namespace DWL {
	// Mostrar mensajes de depuraci�n del men�
	#define DMENUEX_MOSTRARDEBUG	FALSE

	class DMenuEx_Skin {
	  public:
						DMenuEx_Skin(void);
						// Colores por defecto
		COLORREF		FondoNormal;
		COLORREF		FondoResaltado;
		COLORREF		FondoPresionado;
		COLORREF		TextoNormal;
		COLORREF		TextoResaltado;
		COLORREF		TextoPresionado;
		COLORREF		TextoDesactivado;
		COLORREF		TextoSombra;
		COLORREF		Borde;
		COLORREF		Separador;
						// Fuente (Una vez creado el control, utilizar el miembro Fuente)
		int				FuenteTam;
		std::wstring	FuenteNombre;
		BOOL			FuenteNegrita;
		BOOL			FuenteCursiva;
		BOOL			FuenteSubrayado;
		BOOL			FuenteSombraTexto;
	};


	#define DMENUEX_TAMICONO  16	// Tama�o de los iconos
	#define DMENUEX_MARGEN_X   6	// Margen horizontal para el texto 
	#define DMENUEX_MARGEN_Y   4	// Margen vertical para el texto
    #define DMENUEX_BORDE      1    // Tama�o del borde

	enum class DMenuEx_Tipo {
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

	enum class DMenuEx_Transicion {
		DMenuEx_Transicion_Normal,
		DMenuEx_Transicion_Resaltado,
		DMenuEx_Transicion_Presionado,
		DMenuEx_Transicion_Desactivado
	};


	#define DMENUEX_TAMICONO				16 // Tama�o del icono
	#define DMENUEX_ANCHOBARRA             100 // Ancho m�nimo de las barras
	#define WM_ESMENUEX			 WM_USER + 500

	class DMenuEx : protected DWL::DVentana {
  	  public:
										DMenuEx(void);
							           ~DMenuEx(void);
									    // Agrega un men� con texto y icono en la posicion especificada (-1 es al final)
		DMenuEx                        *AgregarMenu(const INT_PTR nID, const wchar_t *nTexto, const INT_PTR nIconoRecursos = NULL, const int Posicion = -1, const BOOL nActivado = TRUE);
		inline DMenuEx                 *AgregarMenu(const INT_PTR nID, std::wstring  &nTexto, const INT_PTR nIconoRecursos = NULL, const int Posicion = -1, const BOOL nActivado = TRUE) { return AgregarMenu(nID, nTexto.c_str(), nIconoRecursos, Posicion, nActivado); };
		DMenuEx				           *AgregarSeparador(const INT_PTR uID = 0, const int Posicion = -1);
		DMenuEx                        *AgregarBarra(const INT_PTR nID, const wchar_t *nTexto, const INT_PTR nIconoRecursos = NULL, const float nMinimo = 0.0f, const float nMaximo = 1.0f, const float nValor = 0.0f, const DBarraEx_MostrarValor nMostrarValor = DBarraEx_MostrarValor_Nada, const int Posicion = -1, const BOOL nActivado = TRUE);

										// Funci�n que muestra el Men� en el bucle principal de la aplicaci�n y espera su respuesta en un DWL_MENUEX_CLICK (NO DEVUELVE EL MEN� PRESIONADO)
		void							Mostrar(DhWnd *nPadre, int PosX, int PosY);
		void							Mostrar(DhWnd *nPadre);
										// Funci�n que muestra el Men� y espera a que se elija una opci�n para devolver-la
										//	NOTA : No desactiva la ventana padre
		DMenuEx			               *MostrarModal(DhWnd *nPadre, const int PosX, const int PosY);
		DMenuEx			               *MostrarModal(DhWnd *nPadre);

		DMenuEx                        *BuscarMenu(const INT_PTR bID);

										// Asigna un icono a este men� 
		void							Icono(const int nIconoRecursos);
		inline void						Icono(DIcono &nIcono)	{ _Icono = nIcono; }

										// Operador que devuelve la refer�ncia al submenu especificado (OJO no te pases de posici�n...)
		inline DMenuEx				   &operator [](const size_t Pos) { return *_Menus[Pos]; }
										// Funciones para navegar entre menus
		inline DMenuEx                 *Menu(const size_t Pos)				{ return _Menus[Pos]; }
		inline const size_t				TotalMenus(void)					{ return _Menus.size(); }
		inline DMenuEx                 *MenuPadre(void)						{ return _Padre; }

		inline const INT_PTR			ID(void)							{ return _ID; }

										// Funci�n para obtener el texto del men�
		inline std::wstring		       &Texto(void)							{ return _Texto; }
										// Funciones para asignar el texto al men�
		void							Texto(const wchar_t *nTxt, const BOOL nRepintar = FALSE);
		void							Texto(std::wstring &nTxt, const BOOL nRepintar = FALSE);

		virtual void					Repintar(void);

										// Funci�nes que devuelven/asignan si el men� est� activado
		inline const BOOL				Activado(void)						{ return _Activado; };
		void							Activado(const BOOL nActivar);

										// Funci�n para ocutar los menus
		void							Ocultar(const BOOL OcultarTodos = FALSE);		

										// Funci�n para eliminar el menu especificado por la ID
		void							EliminarMenu(const INT_PTR eID);
										// Funci�n para eliminar el menu especificado por el objeto
		void							EliminarMenu(DMenuEx *eMenu);
										// Funci�n para eliminar todos los menus
		void							EliminarTodosLosMenus(void);

		void							Pintar(HDC hDC);		

		void							Terminar(void);
										// Funci�n para calcular el tama�o de los menus.
		const POINT						CalcularEspacio(void);

		const BOOL						Visible(void);
										// Funciones para asignar / obtener el valor de la barra de este men�
		inline void						BarraValor(const float nValor)		{ _Barra.Valor(nValor); };
		inline const float				BarraValor(void)					{ return _Barra.Valor(); };
										// Funci�n para realizar una transici�n de estado animada
		void							Transicion(const DMenuEx_Transicion nTransicion);
										// Fuente del menu
		DFuente							Fuente;
										// Variable para almacenar un par�metro personalizado para este men�
		INT64							Parametro;
										// Skin con los colores y fuentes 
		DMenuEx_Skin					Skin;
										// Maximo de opacidad
		double							MaxOpacidad;
										// Tipo de ventana
		virtual const DhWnd_Tipo		TipoWnd(void) { return DhWnd_Tipo_MenuEx; };

										// Devuelve si se ocultaran todos los menus al hacer click en este men�
		inline const BOOL               OcultarEnClick(void)				{ return _OcultarEnClick;		}
										// Asigna si se ocultaran todos los menus al hacer click en este men� (Por defecto est� activada para todos los men�s)
		DMenuEx						   *OcultarEnClick(const BOOL nOcultar) { _OcultarEnClick = nOcultar;	return this;	}

										// Evento virtual que se llama justo antes de mostrar este men�
		virtual void					Evento_MostrarMenu(DMenuEx &mMenu)	{ 	};
										// Evento lambda que se lanza justo antes de mostrar este men�
		std::function<void(DMenuEx &)>	EventoMostrarMenu;

										// Evento virtual que se llama al hacer click en este men�
		virtual void					Evento_MouseClick(DMenuEx &mMenu)	{ 	};
										// Evento lambda que se lanza al hacer click en este men�
		std::function<void(DMenuEx &)>	EventoMouseClick;

										// Gestor de mensajes
		LRESULT CALLBACK				GestorMensajes(UINT uMsg, WPARAM wParam, LPARAM lParam);
	  protected:
										// Constructor men� tipo texto (interno AgregarBarra)
										DMenuEx(DMenuEx *nPadre, DMenuEx_Tipo nTipo, DhWnd *nhWndPadre, const INT_PTR nID, const wchar_t *nTexto, const INT_PTR nIconoRecursos, const BOOL nActivado, const float nMinimo, const float nMaximo, const float nValor);
										// Constructor men� tipo texto (interno AgregarMenu)
										DMenuEx(DMenuEx *nPadre, DMenuEx_Tipo nTipo, DhWnd *nhWndPadre, const INT_PTR nID, const wchar_t *nTexto, const INT_PTR nIconoRecursos = 0, const BOOL nActivado = TRUE);
										// Constructor men� tipo separador (interno AgregarSeparador)
										DMenuEx(DMenuEx *nPadre, DMenuEx_Tipo nTipo, DhWnd *nhWndPadre, const INT_PTR nID);

		void			    	       _OcultarRecursivo(DMenuEx *oMenu);
									    // Funci�n que muestra este men� como un submen�
		void                           _MostrarSubMenu(DMenuEx *nPadre, int cX, int cY, const BOOL AsignarFoco = TRUE);
										// Funci�n que pinta una fila del men� (devuelve la altura de lo pintado)
		void						   _PintarMenu(HDC DC, DMenuEx *pMenu);
										// Funci�n que pinta el separador
		void                           _PintarSeparador(HDC DC, DMenuEx *pMenu);
										// Funci�n que pinta el expansor (flecha que indica que contiene uno o mas sub-menus)
		void                           _PintarExpansor(HDC DC, const int eX, const int eY);
										// Eventos internos
		void                           _Evento_Pintar(void);
//		void                           _Evento_PintarNC(HRGN Region);
		void				           _Evento_MouseMovimiento(WPARAM wParam, LPARAM lParam);
		void				           _Evento_MousePresionado(const int Boton, WPARAM wParam, LPARAM lParam);
		void				           _Evento_MouseSoltado(const int Boton, WPARAM wParam, LPARAM lParam);
		void				           _Evento_MouseSaliendo(void);
		void                           _Evento_TeclaPresionada(WPARAM wParam, LPARAM lParam);
		void                           _Evento_TeclaSoltada(WPARAM wParam, LPARAM lParam);
		void				           _Evento_Tecla(WPARAM wParam, LPARAM lParam);
		void                           _Evento_FocoPerdido(HWND UltimoFoco);
		void                           _Evento_FocoObtenido(HWND UltimoFoco);
		void				           _Evento_BarraEx_Cambiando(DEventoMouse &DatosMouse);
		void				           _Evento_BarraEx_Cambiado(DEventoMouse &DatosMouse);

		RECT                           _Recta;

		BOOL                           _OcultarEnClick;
		
		std::vector<DMenuEx *>         _Menus;
		INT_PTR                        _ID;
		std::wstring                   _Texto;
		DIcono	                       _Icono;
		DMenuEx                       *_Padre;

		DMenuEx                       *_MenuResaltado;
		DMenuEx                       *_MenuDesplegado;
		DMenuEx                       *_MenuPresionado;

		int                            _MargenI; // Margen izquierdo para el texto (que incluye el ancho del icono si alguno de los submenus tiene icono)

		BOOL                           _Activado;
		DMenuEx_Tipo                   _Tipo;
		static DhWnd                  *_hWndDest;  // Destino para los mensajes
//		HWND                           _hWndPadre; // Ventana padre que contiene el hWndDest;

		DAnimacion                     _AniTransicion;
		DAnimacion                     _AniMostrar;

		DBarraDesplazamientoEx         _Barra;
		int                            _BarraPosX;

		COLORREF                       _ColorFondo;
		COLORREF                       _ColorTexto;
										// Variable que determina si hay que recalcular el tama�o del men� (por si se ha modificado el texto)
		BOOL                           _CalcularTam;

//		BOOL				           _AnularMouseMove;
										// Menu resultado para la funci�n MostrarModal
		static DMenuEx                *_ResultadoModal;
	};
};

#endif