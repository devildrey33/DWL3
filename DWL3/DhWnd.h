#ifndef DHWND_H
	#define DHWND_H


	//! Macro para obtener la coordenada X de un LPARAM
	#define GET_X_LPARAM(lp)			((int)(short)LOWORD(lp))
	//! Macro para obtener la coordenada Y de un LPARAM 
	#define GET_Y_LPARAM(lp)			((int)(short)HIWORD(lp))

#include <string>
#include "DWL_Configuracion.h"

#include <objidl.h>
//#include <gdiplus.h>
//using namespace Gdiplus;

#if DWL_SKIN == 0
	#include "DWL_Skin.h"
#elif DWL_SKIN == 1
	#include "DWL_SkinVisualStudio.h"
#endif

#include "DBarraTareas.h"
#include "DMensajesWnd.h"

#define WPARAM_TO_HWND(WP) (HWND)WP
#define HWND_TO_WPARAM(DE) (WPARAM)DE


#define DHWND_MOSTRARDEBUG	FALSE


namespace DWL {

	enum DhWnd_Tipo {
		DhWnd_Tipo_INDEFINIDO = 0,						// Tipo por defecto 
		DhWnd_Tipo_Control,								// Tipo Control estandar (no se usa)
		DhWnd_Tipo_ControlEx,							// Tipo Control extendido de la DWL
		DhWnd_Tipo_Ventana,								// Tipo Ventana estandar de la DWL
		DhWnd_Tipo_ArbolEx,								// Tipo Arbol
		DhWnd_Tipo_BarraDesplazamientoEx,				// Tipo Barra de desplazamiento (sin eventos de mouse)
		DhWnd_Tipo_BarraProgresoEx,						// Tipo Barra de progreso (con eventos de mouse para poder modificar la barra)
		DhWnd_Tipo_BarraScrollEx,						// Tipo Barra scroll (para controles que necesitan scroll como el Arbol o la Lista)
		DhWnd_Tipo_BotonEx,								// Tipo Boton
		DhWnd_Tipo_DesplegableEx,						// Tipo Control Desplegable b�sico
		DhWnd_Tipo_Desplegable_ExplorarDirectoriosEx,	// Tipo Desplegable que muestra el control ExplorarDirectorios
		DhWnd_Tipo_Desplegable_ListaEx,					// Tipo Desplegable que muestra el control Lista
		DhWnd_Tipo_EdicionTextoEx,						// Tipo Edicion de texto
		DhWnd_Tipo_EtiquetaEx,							// Tipo Etiqueta (solo para mostrar texto, puede ser multilinea)
		DhWnd_Tipo_ExplorarDirectoriosEx,				// Tipo Explorar directorios (arbol para seleccionar uno o varios directorios)
		DhWnd_Tipo_ListaEx,								// Tipo Lista
		DhWnd_Tipo_MarcaEx,								// Tipo Marca
		DhWnd_Tipo_MenuEx,								// Tipo Menu desplegable
		DhWnd_Tipo_MarcoScrollEx,						// Tipo Marco con scroll (para meter controles dentro)
		DhWnd_Tipo_MarcoScrollEx_Pagina,				// Tipo pagina donde se ubican los controles del Marco con scroll
		DhWnd_Tipo_MarcoScrollEx_Marco,					// Tipo Marco donde se mostrara la pagina (sin los scrolls)
		DhWnd_Tipo_ToolTipEx,							// Tipo Tooltip
		DhWnd_Tipo_DlgAbrir,							// Tipo Dialogo para seleccionar uno o mas archivos
		DhWnd_Tipo_DlgDirectorios						// Tipo Dialogo para seleccionar uno o mas directorios
	};
	

//	class DControlEx;
//	class DVentana;

	// Clase base para el HWND con algunas funciones
	// - Est� costruida de forma que solo se puede asignar el miembro _hWmd internamente.

	// TODO : moure Minimizar, Minimizado i Maximizada a DVentana
	class DhWnd {
	  public: //////////////////////// Constructor
									DhWnd(void)	: _hWnd(nullptr), _MouseDentro(FALSE), _Padre(nullptr)	{ };
									// Destructor
                                   ~DhWnd(void)															{ Destruir(); };
								    // Funci�n que devuelve el HWND
		inline HWND					hWnd(void)															{ return _hWnd; };
									// Funci�n virtual para activar / desactivar esta ventana / control
		inline virtual void			Activado(const BOOL nActivar)										{ EnableWindow(_hWnd, nActivar); };
									// Funci�n que devuelve si esta ventana / control est� activado
		inline virtual const BOOL	Activado(void)														{ return IsWindowEnabled(_hWnd); };
									// Funci�n que minimiza esta ventana (NO VALE PARA CONTROLES)
		inline const BOOL			Minimizar(void)														{ return ShowWindow(_hWnd, SW_MINIMIZE); };
									// Funci�n que devuelve si esta ventana es minimizable (NO VALE PARA CONTROLES)
		inline const BOOL			Minimizado(void)													{ return IsIconic(_hWnd); };
									// Funci�n que devuelve si esta ventana est� maximizada (NO VALE PARA CONTROLES)
		const BOOL					Maximizada(void);	
									// Funci�n que asigna el foco a esta ventana / control
		inline const HWND			AsignarFoco(void)													{ return SetFocus(_hWnd); }; 
									// Funci�n que destruye esta ventana / control
		virtual const BOOL			Destruir(void);
									// Funci�n que devuelve la ID de esta ventana / control
		inline INT_PTR				ID(void)															{ return static_cast<INT_PTR>(GetWindowLongPtr(_hWnd, GWL_ID)); };
									// Funci�n que devuelve el padre de esta ventana / control (PUEDE SER NULL)
		inline HWND					hWndPadre(void)														{ return GetParent(_hWnd); };
									// Funci�n que asigna si esta ventana / control  est� visible o no
		inline const BOOL			Visible(const BOOL nMostrar)										{ return ShowWindow(_hWnd, (nMostrar != TRUE) ? SW_HIDE : SW_SHOW); 		};
									// Funci�n que devuelve si esta ventana / control est� visible o no
		inline const BOOL			Visible(void)														{ return IsWindowVisible(_hWnd); };
									// Repinta la ventana
		inline virtual void			Repintar(void)														{ RedrawWindow(hWnd(), NULL, NULL, RDW_INVALIDATE | RDW_INTERNALPAINT); };
									// Funci�n que repinta la ventana si no se ha utilizado esta funci�n hace 15 milisegundos (de esta forma aseguramos que no se repinte 1000 veces un control, pero que se repinte a 60fps)
									// Ideal para utilizar con DAnimacion con multiples animaciones
		inline virtual void			RepintarAni(void)													{ static ULONGLONG Tick = 0; ULONGLONG T = GetTickCount64(); if (T > Tick + 15) { Repintar();  Tick = T; } };
									// Funci�n que registra una clase ventana
		ATOM						RegistrarClase(const TCHAR *nNombre, WNDPROC WindowProcedureInicial, UINT Estilos = 0, const int nIconoRecursos = 0, HBRUSH nColorFondo = NULL, HINSTANCE hInstance = NULL);									
									// Evento que salta cuando se crea un control dentro de este control
		virtual void				Evento_ControlCreado(DhWnd *nControl, const int cX, const int cY, const int cAncho, const int cAlto)	{ };
									// Devuelve el tipo de ventana / control
		virtual const DhWnd_Tipo	TipoWnd(void)														{ return DhWnd_Tipo_INDEFINIDO; };
									// Tiempo para animaciones / transiciones
		static DWORD                TiempoAnimaciones;
	  protected: ///////////////////// Funci�n que comprueba si el mouse acaba de entrar en la ventana / control (PRELUDIO PARA WM_MOUSEENTER / WM_MOUSELEAVE)
		const BOOL                 _MouseEntrando(void);
									// HWND de esta ventana / control
		HWND                       _hWnd;
									// Variable que nos dice si el mouse est� dentro de esta ventana / control
		BOOL                       _MouseDentro;
									// Objeto DhWnd padre (PUEDE SER NULL)
		DhWnd                     *_Padre;
	};






}
#endif