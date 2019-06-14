#pragma once

#include "DControlEx.h"
#include "DEventoMouse.h"
#include "DAnimacion.h"

namespace DWL {
	// Mostrar mensajes de depuración en la consola
	#define DBARRASCROLLEX_MOSTRARDEBUG		FALSE

	// Colores por defecto de un control EX con barra de scroll
	class DBarraScrollEx_Skin {
	   public :
							DBarraScrollEx_Skin(void) { }
						   ~DBarraScrollEx_Skin(void) { }
							// Colores para la barra de scroll (para la transición del scroll V/H)
		static COLORREF		FondoScrollNormal;
		static COLORREF		FondoScrollResaltado;
		static COLORREF		FondoScrollPresionado;
		static COLORREF		BarraScrollNormal;
		static COLORREF		BarraScrollResaltado;
		static COLORREF		BarraScrollPresionado;
							// Colores del borde del control (para la transición hover)
		static COLORREF     BordeNormal;
		static COLORREF     BordeResaltado;
							// colores del fondo del control (para la transición hover)
		static COLORREF     FondoNormal;
		static COLORREF     FondoResaltado;
	};

	enum DBarraScrollEx_Estado {
		DBarraScrollEx_Estado_Invisible,
		DBarraScrollEx_Estado_Normal,
		DBarraScrollEx_Estado_FondoResaltado,
		DBarraScrollEx_Estado_BarraResaltada,
//		DBarraScrollEx_Estado_Resaltado,
		DBarraScrollEx_Estado_Presionado
	};

	enum DBarraScrollEx_Transicion {
		DBarraScrollEx_Transicion_Normal,
		DBarraScrollEx_Transicion_BarraResaltada,
		DBarraScrollEx_Transicion_FondoResaltado,
		DBarraScrollEx_Transicion_Presionado
	};

	enum DBarraScrollEx_Transicion_Borde {
		DBarraScrollEx_Transicion_Borde_Normal,
		DBarraScrollEx_Transicion_Borde_Resaltado
	};

	// Control que puede mostrar una barra de scroll vertical y una barra de scroll horizontal.
	class DBarraScrollEx : public DControlEx {
	  public:
									DBarraScrollEx(void);
		                           ~DBarraScrollEx(void);
//		HWND						CrearBarraScrollEx(DhWnd &Padre, const int cX, const int cY, const int cAncho, const int cAlto, const UINT nID);
		void						Scrolls_Pintar(HDC hDC, RECT &RC); // El HDC especificado se supone que es un back buffer
		const BOOL					Scrolls_MouseMovimiento(DEventoMouse &DatosMouse);
		const BOOL					Scrolls_MousePresionado(DEventoMouse &DatosMouse);
		const BOOL					Scrolls_MouseSoltado(DEventoMouse &DatosMouse);

		const BOOL					Scrolls_MouseEntrando(void);
		const BOOL                  Scrolls_MouseSaliendo(void);
									// Obtiene el área que pertenece al control (RectaCliente es el resultado de GetClientRect sin el borde del control, y RectaClienteSinScroll es el área del control excluyendo las barras de scroll) 
		void						ObtenerRectaCliente(RECT *RectaCliente, RECT *RectaClienteSinScroll, RECT *RectaBorde);
									// Obtiene el área de los scrolls
		void						ObtenerRectasScroll(RECT &IN_RC, RECT &OUT_RectaH, RECT &OUT_RectaV);
		void						ObtenerRectasScroll(RECT &RectaH, RECT &RectaV);
									// Obtiene el área de la barra dentro del scroll vertical
		void						ObtenerRectaBarraScrollV(RECT &RectaScroll, RECT &RectaBarra);
									// Obtiene el área de la barra dentro del scroll horizontal
		void						ObtenerRectaBarraScrollH(RECT &RectaScroll, RECT &RectaBarra);

		virtual void				Scrolls_EventoCambioPosicion(void) { };
		////////////////////////////// ScrollH
		void						ScrollH_Pagina(const float nValor);
		const float					ScrollH_Pagina(void);
		void						ScrollH_Posicion(const float nValor);
		const float					ScrollH_Posicion(void);
		void						ScrollH_Visible(const BOOL nVisible);
		////////////////////////////// ScrollV
		void						ScrollV_Pagina(const float nValor);
		const float					ScrollV_Pagina(void);
		void						ScrollV_Posicion(const float nValor);
		const float					ScrollV_Posicion(void);
		void						ScrollV_Visible(const BOOL nVisible);

		virtual void                AvPag(void);
		virtual void                RePag(void);

		void						ScrollV_Transicion(const DBarraScrollEx_Transicion nTransicion);
		void						ScrollH_Transicion(const DBarraScrollEx_Transicion nTransicion);
									
		void                        Scrolls_TransicionBorde(const DBarraScrollEx_Transicion_Borde nTransicion);
		void                        PintarBorde(RECT *Recta, HDC hDC);
	  protected:
		void					   _PintarBarraScrollEx(HDC hDC, RECT &RectaScroll, RECT &RectaBarra, const COLORREF pColorBarra, const COLORREF pColorFondo);
		const float				   _CalcularPosScrollH(const UINT nTam, const int nPos);
		const float				   _CalcularPosScrollV(const UINT nTam, const int nPos);
									
		POINT					   _Scroll_PosPresionado;		// Coordenadas del mouse al iniciar el drag
		float					   _Scroll_PosInicio;			// Posición del scroll al iniciar el drag

		int                        _ScrollV_Ancho;
		DBarraScrollEx_Estado      _ScrollV_Estado;
		float					   _ScrollV_Pagina;
		float					   _ScrollV_Posicion;
		int                        _ScrollH_Alto;
		DBarraScrollEx_Estado      _ScrollH_Estado;
		float					   _ScrollH_Pagina;
		float					   _ScrollH_Posicion;

		DAnimacion                 _ScrollV_AniTransicion;
		DAnimacion                 _ScrollH_AniTransicion;
		COLORREF                   _ColorFondoV;
		COLORREF                   _ColorBarraV;
		COLORREF                   _ColorFondoH;
		COLORREF                   _ColorBarraH;

		DAnimacion                 _Scrolls_AniTransicionBorde;
		COLORREF                   _ColorBorde;
		COLORREF                   _ColorFondo;
	};

};