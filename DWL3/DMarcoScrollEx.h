#pragma once

#include "DBarraScrollEx.h"
#include "DEventoMouseRueda.h"

namespace DWL {

#define DMARCOSCROLLEX_MOSTRARDEBUG TRUE
//#defien DMARCOSCROLLEX_MOSTRARDEBUG FALSE

	// Marco con barras de scroll con contenido desplazable
	class DMarcoScrollEx : public DBarraScrollEx {
	  protected:
		// Control que contiene los hijos y hace de página para poder mover todos los hijos de una tacada
		class DMarcoScrollEx_Pagina : public DControlEx {
		  public:
										DMarcoScrollEx_Pagina(void) { }
										// Tipo de HWND
			virtual const DhWnd_Tipo	TipoWnd(void)				{ return DhWnd_Tipo_MarcoScrollEx_Pagina; };
										// Gestor de mensajes para la página
			LRESULT CALLBACK			GestorMensajes(UINT uMsg, WPARAM wParam, LPARAM lParam);
			friend class DMarcoScrollEx;
		};

		// Control que contiene los hijos y hace de página para poder mover todos los hijos de una tacada
		class DMarcoScrollEx_Marco : public DControlEx {
		  public:
										DMarcoScrollEx_Marco(void) { }
										// Tipo de HWND
			virtual const DhWnd_Tipo	TipoWnd(void)				{ return DhWnd_Tipo_MarcoScrollEx_Marco; };
										// Gestor de mensajes para la página
			LRESULT CALLBACK			GestorMensajes(UINT uMsg, WPARAM wParam, LPARAM lParam);
			friend class DMarcoScrollEx;
		};


	  public: ////////////////////////
									DMarcoScrollEx(void) : DBarraScrollEx(), _AnchoPagina(0), _AltoPagina(0) { }
									// Función que crea un BotonEx con texto
		HWND						CrearMarcoScrollEx(DhWnd *nPadre, const int cX, const int cY, const int cAncho, const int cAlto, const int cID, const long Estilos = WS_CHILD | WS_VISIBLE);
									// Función que destruye el marco, todos los controles, y libera la memória
		const BOOL					Destruir(void);
									// Devuelve el HWND del control que hace de Página para mover todos el contenido
		inline HWND					hWndPagina(void)	{ return _Pagina.hWnd(); }
									// Función que calcula el tamaño que debe tener la pagina según su contenido
		void						CalcularPagina(void);
									// Función que pinta el fondo del control y las barras de scroll
		void						Pintar(HDC hDC);
									// Evento que salta cuando se crea un control dentro de este control
		void						Evento_ControlCreado(DhWnd *nControl, const int cX, const int cY, const int cAncho, const int cAlto);
									// Tipo de HWND
		virtual const DhWnd_Tipo	TipoWnd(void)		{ return DhWnd_Tipo_MarcoScrollEx; };

		void						Repintar(void);

									// Eventos virtuales
		virtual void          		Evento_MouseEntrando(void)																		{ };
		virtual void				Evento_MouseSaliendo(void)																		{ };
		virtual void				Evento_MouseMovimiento(DEventoMouse &DatosMouse)												{ };
		virtual void				Evento_MousePresionado(DEventoMouse &DatosMouse)												{ };
		virtual void				Evento_MouseSoltado(DEventoMouse &DatosMouse)													{ };
		virtual void                Evento_MouseRueda(DEventoMouseRueda &DatosMouse)												{ };

									// Gestor de mensages virtual para el marco
		virtual LRESULT CALLBACK	GestorMensajes(UINT uMsg, WPARAM wParam, LPARAM lParam);
	  protected: /////////////////////
									// Función interna para porcesar el movimiento del mouse
		void					   _Evento_MouseMovimiento(WPARAM wParam, LPARAM lParam);
									// Función interna para porcesar cuando se presiona un boton del mouse
		void					   _Evento_MousePresionado(const int Boton, WPARAM wParam, LPARAM lParam);
									// Función interna para porcesar cuando se suelta un boton del mouse
		void					   _Evento_MouseSoltado(const int Boton, WPARAM wParam, LPARAM lParam);
									// Función interna para porcesar el movimiento de la rueda del mouse
		void                       _Evento_MouseRueda(WPARAM wParam, LPARAM lParam);
									// Función interna para cuando sale el mouse del control
		void				       _Evento_MouseSaliendo(void);

									// Función que pinta el marco
   	    void					   _Evento_Pintar(void);
									// Función que enumera las ventanas hijo
		static BOOL CALLBACK	   _EnumChildProc(_In_ HWND hwnd, _In_ LPARAM lParam);
									// Control que contiene la pagina sin las barras de scroll
		DMarcoScrollEx_Marco       _Marco;
									// Control que contiene los hijos
		DMarcoScrollEx_Pagina      _Pagina;
									// Ancho necesario para la página
		LONG					   _AnchoPagina;
									// Altura necerasia para la página
		LONG					   _AltoPagina;

		friend class DControlEx;
	};

};
