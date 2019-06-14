
#ifndef DDESPLEGABLEEX_H
#define DDESPLEGABLEEX_H

#include "DEdicionTextoEx.h"
#include "DDesplegableEx_ExplorarDirectoriosEx (DEPRECATED).h"
//#include "DListaEx.h"

/* ComboBox que puede desplegar un DListaEx o un DArbolEx, y que puede tener el texto editable o no */
/* Añadir facilidades para hacer un modo busqueda directorios o directorios y archivos */

namespace DWL {
	/*
	#define DDESPLEGABLEEX_EXPLORARDIRECTORIOS	1
	#define DDESPLEGABLEEX_ARBOLEX				2
	#define DDESPLEGABLEEX_LISTAEX				3

	enum DDesplegableEx_TipoEdicion {
		DDesplegableEx_TipoEdicion_SinEdicion,
		DDesplegableEx_TipoEdicion_Texto,
		DDesplegableEx_TipoEdicion_NumericoEntero,
		DDesplegableEx_TipoEdicion_NumericoDecimal
	};

	enum DDesplegableEx_TipoDesplegable {
		DDesplegableEx_TipoDesplegable_Lista,
		DDesplegableEx_TipoDesplegable_Arbol,
		DDesplegableEx_TipoDesplegable_ExplorarDirectorios
	};

	enum DDesplegableEx_Estado {
		DDesplegableEx_Estado_Indefinido,
		DDesplegableEx_Estado_Normal,
		DDesplegableEx_Estado_Resaltado,
		DDesplegableEx_Estado_Presionado
	};

	enum DDesplegableEx_Presionado {
		DDesplegableEx_Presionado_Nada,
		DDesplegableEx_Presionado_Visor,
		DDesplegableEx_Presionado_Boton
	};

	// Control del estilo ComboBox que puede mostrar una DListaEx o un DArbolEx además del editbox / static text iniciales 
	class DDesplegableEx : public DControlEx, public DEdicionTextoEx_Nucleo {
  	  public:
												DDesplegableEx(void);
									           ~DDesplegableEx(void);
		HWND									CrearDesplegable(DhWnd *nPadre, const int cX, const int cY, const int cAncho, const int cAlto, const int cID, const DDesplegableEx_TipoEdicion nTipoEdicion = DDesplegableEx_TipoEdicion_Texto, const DDesplegableEx_TipoDesplegable nTipoDesplegable = DDesplegableEx_TipoDesplegable_Lista);
		void									MostrarDesplegable(void);
//		void									OcultarDesplegable(void);
		void									Pintar(HDC hDC);

												// Eventos virtuales
		virtual void          					Evento_MouseEntrando(void)									{ };
		virtual void							Evento_MouseSaliendo(void)									{ };
		virtual void							Evento_MouseMovimiento(DEventoMouse &DatosMouse)			{ };
		virtual void							Evento_MousePresionado(DEventoMouse &DatosMouse)			{ };
		virtual void							Evento_MouseSoltado(DEventoMouse &DatosMouse)				{ };
		virtual void					        Evento_MouseRueda(DEventoMouseRueda &DatosMouse)			{ };

		virtual void				            Evento_TeclaPresionada(DEventoTeclado &DatosTeclado)		{ };
		virtual void							Evento_TeclaSoltada(DEventoTeclado &DatosTeclado)			{ };
		virtual void							Evento_Tecla(DEventoTeclado &DatosTeclado)					{ };


		LRESULT CALLBACK						GestorMensajes(UINT uMsg, WPARAM wParam, LPARAM lParam);

		inline void                             AsignarIcono(DListaIconos_Icono *nIcono)					{ _Icono = nIcono; }
	  protected:
//		void                                   _MostrarExplorarDirectorios(void);

		void								   _Evento_Pintar(void);
		void								   _Evento_MouseMovimiento(WPARAM wParam, LPARAM lParam);
		void								   _Evento_MousePresionado(const UINT Boton, WPARAM wParam, LPARAM lParam);
		void								   _Evento_MouseSoltado(const UINT Boton, WPARAM wParam, LPARAM lParam);
		void						           _Evento_MouseRueda(WPARAM wParam, LPARAM lParam);
//		void								   _Evento_MouseDobleClick(const UINT Boton, const int cX, const int cY, const UINT Param);
		void								   _Evento_MouseSaliendo(void);
		void						           _Evento_TeclaPresionada(WPARAM wParam, LPARAM lParam);
		void							       _Evento_TeclaSoltada(WPARAM wParam, LPARAM lParam);
		void								   _Evento_Tecla(WPARAM wParam, LPARAM lParam);

		DDesplegableEx_TipoEdicion			   _TipoEdicion;				
		DDesplegableEx_TipoDesplegable		   _TipoDesplegable;			

		DDesplegableEx_Presionado			   _Presionado;

		DDesplegableEx_Estado				   _EstadoBoton;
		DDesplegableEx_Estado				   _EstadoVisor;
		DDesplegableEx_Estado				   _UEstadoBoton;
		DDesplegableEx_Estado				   _UEstadoVisor;
												// Control desplegable (puede ser DArbolEx, DListaEx o DExplorarDirectoriosEx)
		DArbolEx							  *_Arbol;
		DListaEx							  *_Lista;
		DDesplegableEx_ExplorarDirectoriosEx  *_ExplorarDirectorios;

		DListaIconos_Icono                    *_Icono;
	};*/
}
#endif