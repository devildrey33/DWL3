#pragma once

#include "DBarraScrollEx.h"

namespace DWL {

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
									// Evento que salta cuando se crea un control dentro de este control
		void						Evento_ControlCreado(DhWnd *nControl);
									// Tipo de HWND
		virtual const DhWnd_Tipo	TipoWnd(void)		{ return DhWnd_Tipo_MarcoScrollEx; };
									// Gestor de mensages virtual para el marco
		virtual LRESULT CALLBACK	GestorMensajes(UINT uMsg, WPARAM wParam, LPARAM lParam);

	  protected: /////////////////////
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
