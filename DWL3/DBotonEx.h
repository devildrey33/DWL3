#ifndef DBOTONEX_H
#define DBOTONEX_H

#include "DControlEx.h"
#include "DAnimacion.h"
#include "DFuente.h"
#include "DEventoMouse.h"
#include "DIcono.h"


namespace DWL {
	// Estados para el bot�n
	enum class DBotonEx_Estado {
		DBotonEx_Estado_Normal,
		DBotonEx_Estado_Resaltado,
		DBotonEx_Estado_Presionado
	};

	// Tipo de transici�n para el bot�n
	enum class DBotonEx_Transicion {
		DBotonEx_Transicion_Normal,
		DBotonEx_Transicion_Resaltado,
		DBotonEx_Transicion_Presionado,
		DBotonEx_Transicion_Marcado,
		DBotonEx_Transicion_Desactivado
	};

	#define DBOTONEX_MOSTRARDEBUG	FALSE
	#define DBOTONEX_CENTRADO		-1


	// Skin por defecto del boton
	class DBotonEx_Skin {
	  public : /////////// Constructor
						DBotonEx_Skin(void);
						// Destructor
		               ~DBotonEx_Skin(void) { }
						// Colores
		 COLORREF		FondoNormal;
		 COLORREF		FondoResaltado;
		 COLORREF		FondoPresionado;
		 COLORREF		FondoMarcado;
		 COLORREF		BordeNormal;
		 COLORREF		BordeResaltado;
		 COLORREF		BordePresionado;
		 COLORREF		TextoNormal;
		 COLORREF		TextoResaltado;
		 COLORREF		TextoPresionado;
		 COLORREF		TextoDesactivado;
		 COLORREF       TextoSombra;
						// Fuente (estos miembros solo se usan al crear el bot�n, una vez creado si queremos modificar la fuente hay que usar el miembro DBotonEx::Fuente)
		 int			FuenteTam;
		 std::wstring	FuenteNombre;
		 BOOL           FuenteNegrita;
		 BOOL           FuenteCursiva;
		 BOOL           FuenteSubrayado;
		 BOOL			FuenteSombraTexto;
	};

	// Objeto para la creaci�n y control de un boton extendido
	class DBotonEx : public DControlEx {
	  public: //////////////////////////////// Constructor								
											DBotonEx(void);
											// Destructor
									       ~DBotonEx(void);
											// Funci�n que crea un BotonEx con texto
		HWND								CrearBotonEx(DhWnd *nPadre, const TCHAR *nTxt, const int cX, const int cY, const int cAncho, const int cAlto, const int cID, const long Estilos = WS_CHILD | WS_VISIBLE);
											// Funci�n que crea un BotonEx con icono
		HWND								CrearBotonEx(DhWnd *nPadre, const int IDIcono, const int TamIcono, const int PosIconoX, const int PosIconoY, const int cX, const int cY, const int cAncho, const int cAlto, const int cID, const long Estilos = WS_CHILD | WS_VISIBLE);
											// Funci�n que crea un BotonEx con icono y texto
		HWND								CrearBotonEx(DhWnd *nPadre, const int IDIcono, const int TamIcono, const int PosIconoX, const int PosIconoY, const TCHAR *nTxt, const int cX, const int cY, const int cAncho, const int cAlto, const int cID, const long Estilos = WS_CHILD | WS_VISIBLE);
											// Funci�n que activa / desactiva el bot�n
		virtual void						Activado(const BOOL nActivar);
											// Funci�n que devuelve si el bot�n est� activado o no 
		inline const BOOL					Activado(void)										{ return DhWnd::Activado(); }
											// Funci�n que pinta el bot�n en el DC y coordenadas especificadas
		void								Pintar(HDC DC, const int nX = 0, const int nY = 0);
											// Funci�n virtual que recibe cuando se mueve el mouse por el bot�n
		virtual void						Evento_MouseMovimiento(DEventoMouse& DatosMouse)	{ };
											// Funci�n lambda que recibe cuando se mueve el mouse por el bot�n
		std::function<void(DEventoMouse&)>	EventoMouseMovimiento;
											// Funci�n virtual que recibe cuando se presiona el mouse en el control
		virtual void						Evento_MousePresionado(DEventoMouse& DatosMouse)	{ };
											// Funci�n lambda que recibe cuando se presiona el mouse en el control
		std::function<void(DEventoMouse&)>	EventoMousePresionado;
											// Funci�n virtual que recibe cuando se suelta el boton del mouse en el control
		virtual void						Evento_MouseSoltado(DEventoMouse& DatosMouse)		{ };
											// Funci�n lambda que recibe cuando se suelta el boton del mouse en el control
		std::function<void(DEventoMouse&)>	EventoMouseSoltado;
											// Funci�n virtual que recibe cuando se hace click en el bot�n
		virtual void						Evento_MouseClick(DEventoMouse& DatosMouse)			{ };
											// Funci�n lambda que recibe cuando se hace click en el bot�n
		std::function<void(DEventoMouse&)>	EventoMouseClick;
											// Funci�n virtual de post pintado
		virtual void						Evento_Pintar(HDC DC)								{ };
											// Funci�n virtual que devuelve el tipo de control DhWnd
		virtual const DhWnd_Tipo			TipoWnd(void)										{ return DhWnd_Tipo_BotonEx; };
											// Gestor de mensajes para este control
		LRESULT CALLBACK					GestorMensajes(UINT uMsg, WPARAM wParam, LPARAM lParam);
											// Fuente para este control
		DWL::DFuente						Fuente;
											// Funci�n que devuelve si el bot�n est� marcado
		inline const BOOL					Marcado(void) { return _Marcado; }
											// Funci�n para asignar el estado marcado del bot�n
		void								Marcado(const BOOL nMarcar);
											// Funci�n para asignar un icono para el bot�n
		void								Icono(const int IDIcono, const int TamIcono, const int PosX = DBOTONEX_CENTRADO, const int PosY = DBOTONEX_CENTRADO);
											// Funci�n que devuelve la ID del icono del bot�n
		inline const INT_PTR				IDIcono(void)										{ return (_Icono() != NULL) ? _Icono.ID() : 0; };
											// Funci�n para asignar texto a un boton a partir de un std::wstring
		inline void							Texto(std::wstring &nTexto)							{ _Texto = nTexto; Repintar(); };
											// Funci�n para asignar texto a un boton a partir de un const wchar_t *
		inline void							Texto(const wchar_t *nTexto)						{ _Texto = nTexto; Repintar(); };
											// Funci�n que realiza una transici�n del estado actual al estado especificado
		void								Transicion(const DBotonEx_Transicion nTransicion);
											// Configuraci�n visual para el bot�n
		DBotonEx_Skin						Skin;
											// Texto para el tooltip
		std::wstring						TextoToolTip;
	  protected: /////////////////////////////
											// Funci�n que responde al mensaje WM_MOUSEMOVE
		void						       _Evento_MouseMovimiento(const WPARAM wParam, const LPARAM lParam);
											// Funci�n que responde a los mensajes WM_?BUTTONDOWN
		void							   _Evento_MousePresionado(const WPARAM wParam, const LPARAM lParam, const int Boton);
											// Funci�n que responde a los mensajes WM_?BUTTONUP
		void						       _Evento_MouseSoltado(const WPARAM wParam, const LPARAM lParam, const int Boton);
											// Funci�n que responde al mensaje WM_MOUSELEAVE
		void					           _Evento_MouseSaliendo(void);
											// Funci�n que responde al mensaje WM_PAINT
		void					           _Evento_Pintar(void);
											// Funci�n que responde al mensaje WM_TIMER
		void							   _Evento_Temporizador(WPARAM wParam);
											// Funci�n protegida para la creaci�n del boton
		HWND						       _CrearBotonEx(DhWnd *nPadre, const int cX, const int cY, const int cAncho, const int cAlto, const int cID, const long Estilos);
											// Texto del bot�n
		std::wstring					   _Texto;
											// Estado del bot�n
		DBotonEx_Estado	  			       _Estado;
											// Estado marcado del bot�n
		BOOL						       _Marcado;
											// Icono para el bot�n
		DIcono						       _Icono;
											// Posici�n X para el icono del bot�n
		int							       _PosIconoX;
											// Posici�n Y para el icono del bot�n
		int							       _PosIconoY;
											// Objeto DAnimacion para las transiciones del bot�n
		DAnimacion					       _AniTransicion;
											// Color del borde actual
		COLORREF					       _ColorBorde;
											// Color del fondo actual
		COLORREF					       _ColorFondo;
											// Color del texto actual
		COLORREF					       _ColorTexto;
											// Identificador para el temporizador del tooltip
		UINT_PTR						   _TimerToolTip;
	};		

}

#endif