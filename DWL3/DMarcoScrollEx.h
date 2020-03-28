#pragma once

#include "DBarraScrollEx.h"
#include "DEventoMouseRueda.h"

namespace DWL {

#define DMARCOSCROLLEX_MOSTRARDEBUG TRUE
//#define DMARCOSCROLLEX_MOSTRARDEBUG FALSE

	// Marco con barras de scroll con contenido desplazable
	// Si tienes que crear controles dentro. debes crear-los usando hWndPagina() como padre.
	class DMarcoScrollEx : public DBarraScrollEx {
	  protected:
		// Control que contiene los hijos y hace de p�gina para poder mover todos los hijos de una tacada
		class DMarcoScrollEx_Pagina : public DControlEx {
		  public: //////////////////////// Constructor
										DMarcoScrollEx_Pagina(void) { }
										// Tipo de HWND
			virtual const DhWnd_Tipo	TipoWnd(void)				{ return DhWnd_Tipo_MarcoScrollEx_Pagina; };
										// Gestor de mensajes para la p�gina
			LRESULT CALLBACK			GestorMensajes(UINT uMsg, WPARAM wParam, LPARAM lParam);
			friend class DMarcoScrollEx;
		};

		// Control que contiene los hijos y hace de p�gina para poder mover todos los hijos de una tacada
		class DMarcoScrollEx_Marco : public DControlEx {
		  public: //////////////////////// Constructor
										DMarcoScrollEx_Marco(void) { }
										// Tipo de HWND
			virtual const DhWnd_Tipo	TipoWnd(void)				{ return DhWnd_Tipo_MarcoScrollEx_Marco; };
										// Gestor de mensajes para el marco
			LRESULT CALLBACK			GestorMensajes(UINT uMsg, WPARAM wParam, LPARAM lParam);
			friend class DMarcoScrollEx;
		};


	  public: ////////////////////////////////////////////
														// Constructor
														DMarcoScrollEx(void);
														// Funci�n que crea un BotonEx con texto
		HWND											CrearMarcoScrollEx(DhWnd *nPadre, const int cX, const int cY, const int cAncho, const int cAlto, const int cID, const long Estilos = WS_CHILD | WS_VISIBLE);
														// Funci�n que destruye el marco, todos los controles, y libera la mem�ria
		const BOOL										Destruir(void);
														// Devuelve el HWND del control que hace de P�gina para mover todos el contenido
		inline HWND										hWndPagina(void)									{ return _Pagina.hWnd(); }
														// Funci�n que calcula el tama�o que debe tener la pagina seg�n su contenido
		void											CalcularPagina(void);
														// Funci�n que pinta el fondo del control y las barras de scroll
		void											Pintar(HDC hDC);
														// Evento que salta cuando se crea un control dentro de este control
		void											Evento_ControlCreado(DhWnd *nControl, const int cX, const int cY, const int cAncho, const int cAlto);
														// Funci�n que devuelve el tipo de DhWnd
		virtual const DhWnd_Tipo						TipoWnd(void)										{ return DhWnd_Tipo_MarcoScrollEx; };
														// Funci�n que repinta el control
		void											Repintar(void);

														// Evento virtual para el evento mouse enter
		virtual void          							Evento_MouseEntrando(void)							{ };
														// Funci�n lambda para el evento mouse enter
		std::function<void(void)>						EventoMouseEntrando;
														// Evento virtual para el evento mouse leave
		virtual void									Evento_MouseSaliendo(void)							{ };
														// Funci�n lambda para el evento mouse leave
		std::function<void(void)>						EventoMouseSaliendo;
														// Evento virtual para el evento mouse move
		virtual void									Evento_MouseMovimiento(DEventoMouse& DatosMouse)	{ };
														// Funci�n lambda para el evento mouse move
		std::function<void(DEventoMouse&)>				EventoMouseMovimiento;
														// Evento virtual para el evento mouse down
		virtual void									Evento_MousePresionado(DEventoMouse& DatosMouse)	{ };
														// Funci�n lambda para el evento mouse down
		std::function<void(DEventoMouse&)>				EventoMousePresionado;
														// Evento virtual para el evento mouse up
		virtual void									Evento_MouseSoltado(DEventoMouse& DatosMouse)		{ };
														// Funci�n lambda para el evento mouse up
		std::function<void(DEventoMouse&)>				EventoMouseSoltado;
														// Evento virtual para la rueda del mouse
		virtual void                                    Evento_MouseRueda(DEventoMouseRueda& DatosMouse)	{ };
														// Funci�n lambda para el evento de la rueda del mouse
		std::function<void(DEventoMouseRueda&)>			EventoMouseRueda;

														// Evento que salta cuando los crolls cambian de posici�n
		void											Scrolls_EventoCambioPosicion(void);
														// Gestor de mensages virtual para el marco
		virtual LRESULT CALLBACK						GestorMensajes(UINT uMsg, WPARAM wParam, LPARAM lParam);
	  protected: /////////////////////////////////////////
														// Funci�n interna para porcesar el movimiento del mouse
		void					                       _Evento_MouseMovimiento(WPARAM wParam, LPARAM lParam);
														// Funci�n interna para porcesar cuando se presiona un boton del mouse
		void				                   	       _Evento_MousePresionado(const int Boton, WPARAM wParam, LPARAM lParam);
														// Funci�n interna para porcesar cuando se suelta un boton del mouse
		void				                   	       _Evento_MouseSoltado(const int Boton, WPARAM wParam, LPARAM lParam);
							                   			// Funci�n interna para porcesar el movimiento de la rueda del mouse
		void                                           _Evento_MouseRueda(WPARAM wParam, LPARAM lParam);
														// Funci�n interna para cuando sale el mouse del control
		void				                           _Evento_MouseSaliendo(void);

														// Funci�n que pinta el marco
   	    void					                       _Evento_Pintar(void);
														// Funci�n que enumera las ventanas hijo
		static BOOL CALLBACK	                       _EnumChildProc(_In_ HWND hwnd, _In_ LPARAM lParam);
														// Control que contiene la pagina sin las barras de scroll
		DMarcoScrollEx_Marco                           _Marco;
														// Control que contiene los hijos
		DMarcoScrollEx_Pagina                          _Pagina;
														// Ancho necesario para la p�gina
		LONG					                       _AnchoPagina;
														// Altura necerasia para la p�gina
		LONG					                       _AltoPagina;

		friend class DControlEx;
	};

};
