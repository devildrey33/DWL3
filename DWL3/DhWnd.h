#ifndef DHWND_H
	#define DHWND_H


	//! Macro para obtener la coordenada X de un LPARAM
	#define GET_X_LPARAM(lp)			((int)(short)LOWORD(lp))
	//! Macro para obtener la coordenada Y de un LPARAM 
	#define GET_Y_LPARAM(lp)			((int)(short)HIWORD(lp))

#include <string>
#include "DWL_Configuracion.h"

#include <objidl.h>
#include <gdiplus.h>
//using namespace Gdiplus;

#if DWL_SKIN == 0
	#include "DWL_Skin.h"
#elif DWL_SKIN == 1
	#include "DWL_SkinVisualStudio.h"
#endif

//#include "DhWnd_Fuente.h"
#include "DBarraTareas.h"
/*#include "DEventoMouse.h"
#include "DEventoMouseRueda.h"
#include "DEventoTeclado.h"*/
#include "DMensajesWnd.h"

#define WPARAM_TO_HWND(WP) (HWND)WP
#define HWND_TO_WPARAM(DE) (WPARAM)DE


namespace DWL {


	// Clase base para el HWND con algunas funciones
	// - Est� costruida de forma que solo se puede asignar el miembro _hWmd internamente.
	class DhWnd {
	  public:
									DhWnd(void)		 : _hWnd(0)		, _MouseDentro(FALSE) { };
//									DhWnd(HWND hWnd) : _hWnd(hWnd)	, _MouseDentro(FALSE) { };
                                   ~DhWnd(void)						{ Destruir(); };
		inline HWND					hWnd(void)						{ return _hWnd; };
		inline virtual void			Activado(const BOOL nActivar)   { EnableWindow(_hWnd, nActivar); };
		inline virtual const BOOL	Activado(void)					{ return IsWindowEnabled(_hWnd); };
		inline const BOOL			Minimizar(void)					{ return ShowWindow(_hWnd, SW_MINIMIZE); };
		inline const BOOL			Minimizado(void)				{ return IsIconic(_hWnd); };
		const BOOL					Maximizada(void);
		inline const HWND			AsignarFoco(void)               { return SetFocus(_hWnd); }; 
		inline virtual const BOOL	Destruir(void)					{ if (_hWnd == NULL) { return TRUE; } BOOL R = DestroyWindow(_hWnd); _hWnd = NULL; return R; };
		inline INT_PTR				ID(void)						{ return static_cast<INT_PTR>(GetWindowLongPtr(_hWnd, GWL_ID)); };
		inline HWND					hWndPadre(void)					{ return GetParent(_hWnd); };
		inline const BOOL			Visible(const BOOL nMostrar)    { 
			return ShowWindow(_hWnd, (nMostrar != TRUE) ? SW_HIDE : SW_SHOW); 
		};
		inline const BOOL			Visible(void)                   { return IsWindowVisible(_hWnd); };

//		inline void					BorrarBufferTeclado(void)		{ for (size_t i = 0; i < 256; i++) Teclado[i] = false; }
//		void						IniciarMiembrosEstaticos(void);
									// Repinta la ventana
		inline virtual void			Repintar(void)					{ RedrawWindow(hWnd(), NULL, NULL, RDW_INVALIDATE | RDW_INTERNALPAINT); };
									// Funci�n que repinta la ventana si no se ha utilizado esta funci�n hace 15 milisegundos (de esta forma aseguramos que no se repinte 1000 veces un control, pero que se repinte a 60fps)
									// Ideal para utilizar con DAnimacion con multiples animaciones
		inline virtual void			RepintarAni(void)				{ static DWORD Tick = 0; DWORD T = GetTickCount(); if (T > Tick + 15) { RedrawWindow(hWnd(), NULL, NULL, RDW_INVALIDATE | RDW_INTERNALPAINT);  Tick = T; } };

		ATOM						RegistrarClase(const TCHAR *nNombre, WNDPROC WindowProcedureInicial, UINT Estilos = 0, const int nIconoRecursos = 0, HBRUSH nColorFondo = NULL, HINSTANCE hInstance = NULL);
									// Debe utilizarse una sola vez al terminar la apliaci�n
//		static void                 EliminarFuentesEstaticas();

/*		static DhWnd_Fuente         Fuente16Normal;
		static DhWnd_Fuente         Fuente18Normal;
		static DhWnd_Fuente         Fuente18Negrita;
		static DhWnd_Fuente         Fuente21Normal;
		static DhWnd_Fuente         Fuente21Negrita;
		static DhWnd_Fuente         FuenteTest;*/

//		static bool                 Teclado[256];
									// Tiempo para animaciones / transiciones
		static DWORD                TiempoAnimaciones;

	  protected:
		const BOOL                 _MouseEntrando(void);
		HWND                       _hWnd;
		BOOL                       _MouseDentro;

		//		static HFONT       _FuenteB;

		/*friend class DVentana;
		friend class DControl;
		friend class DControlEx;
		friend class DStaticControl;
		friend class DButton;
		friend class DTrackBar;
		friend class DListView;
		friend class DTreeView;*/
//		friend class DBarraEx;
	};






}
#endif