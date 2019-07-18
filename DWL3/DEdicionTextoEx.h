#ifndef DEDICIONTEXTO_H
#define DEDICIONTEXTO_H

#include "DControlEx.h"
#include "DAnimacion.h"
#include "DFuente.h"
#include "DIcono.h"

namespace DWL {

	#define DEDICIONTEXTOEX_MOSTRARDEBUG	FALSE

	// Configuración de colores y la fuente
	class DEdicionTextoEx_Skin {
	  public:
					 DEdicionTextoEx_Skin(void);		// Constructor
		COLORREF     FondoNormal;						// Color del fondo normal
		COLORREF     FondoResaltado;					// Color del fondo resaltado
		COLORREF     FondoPresionado;					// Color del fondo presionado
		COLORREF     FondoDesactivado;					// Color del fondo desactivado
		COLORREF     FondoSeleccion;					// Color del fondo de la selección

		COLORREF     Texto;								// Color del texto
		COLORREF     TextoSombra;						// Color de la sombra del texto
		COLORREF     TextoResaltado;					// Color del texto resaltado
		COLORREF     TextoResaltadoSombra;				// Color de la sombra del texto resaltado
		COLORREF     TextoPresionado;					// Color del texto presionado
		COLORREF     TextoPresionadoSombra;				// Color de la sombra del texto presionado
		COLORREF     TextoDesactivado;					// Color del texto desactivado
		COLORREF     TextoDesactivadoSombra;			// Color de la sombra del texto desactivado
		COLORREF     TextoPlaceholder;					// Color del placeholder
		COLORREF     TextoPlaceholderSombra;			// Color de la sombra del placeholder

		COLORREF     BordeNormal;						// Color del borde
		COLORREF     BordeResaltado;					// Color del borde resaltado
		COLORREF     BordePresionado;					// Color del borde presionado

		COLORREF     Cursor;							// Color del cursor
					 // Fuente
		int			 FuenteTam;							// Tamaño de la fuente  (SOLO SE USA PARA CREAR EL CONTROL)
		std::wstring FuenteNombre;						// Nombre de la fuente  (SOLO SE USA PARA CREAR EL CONTROL)
		BOOL         FuenteNegrita;						// Fuente en negrita	(SOLO SE USA PARA CREAR EL CONTROL)
		BOOL         FuenteCursiva;						// Fuente en cursiva	(SOLO SE USA PARA CREAR EL CONTROL)
		BOOL         FuenteSubrayado;					// Fuente subrayada		(SOLO SE USA PARA CREAR EL CONTROL)

		BOOL	 	 FuenteSombraTexto;					// Pintar sombras a los textos
	};

	// Transiciones posibles
	enum DEdicionTextoEx_Transicion {	
		DEdicionTextoEx_Transicion_Normal,				// Estado normal
		DEdicionTextoEx_Transicion_Resaltado,			// Estado resatado
		DEdicionTextoEx_Transicion_Presionado,			// Estado presionado
		DEdicionTextoEx_Transicion_Desactivado			// Estado desactivado
	};

	// Alineación
	enum DEdicionTextoEx_Alineacion {
		DEdicionTextoEx_Alineacion_Izquierda = 0,		// Texto a la izquierda
		DEdicionTextoEx_Alineacion_Centrado	 = 1,		// Texto centrado
		DEdicionTextoEx_Alineacion_Derecha	 = 2		// Texto a la derecha
	};

	// Tipo de entrada que acepta por el usuario
	enum DEdicionTextoEx_Entrada {
		DEdicionTextoEx_Entrada_SinEntrada		 = 0,	// Nada
		DEdicionTextoEx_Entrada_Texto			 = 1,	// Todo
		DEdicionTextoEx_Entrada_ValoresEnteros	 = 2,	// 0123456789
		DEdicionTextoEx_Entrada_ValoresDecimales = 3,	// 0123456789.
		DEdicionTextoEx_Entrada_ValoresTiempo    = 4	// 0123456789:
	};

	class DEdicionTextoEx : public DControlEx {
	  public: //////////////////////// 
									// Constructor
									DEdicionTextoEx(void);
									// Destructor
		                           ~DEdicionTextoEx(void);
									// Función que crea el control DEdicionTextoEx sin icono
		HWND						CrearEdicionTextoEx(DhWnd *nPadre, const TCHAR *nTxt, const int cX, const int cY, const int cAncho, const int cAlto, const int cID, const long Estilos = WS_CHILD | WS_VISIBLE);
									// Función que crea el control DEdicionTextoEx utilizando un icono de los recursos
		HWND						CrearEdicionTextoEx(DhWnd *nPadre, const UINT IDI_Icono, const TCHAR *nTxt, const int cX, const int cY, const int cAncho, const int cAlto, const int cID, const long Estilos = WS_CHILD | WS_VISIBLE);
									// Función que crea el control DEdicionTextoEx utilizando un icono del sistema
		HWND						CrearEdicionTextoEx(DhWnd* nPadre, const GUID nIconoKnownFolder, const TCHAR* nTxt, const int cX, const int cY, const int cAncho, const int cAlto, const int cID, const long Estilos = WS_CHILD | WS_VISIBLE);
									// Función que crea el control DEdicionTextoEx utilizando un icono de los recursos
		HWND						CrearEdicionTextoEx(DhWnd *nPadre, const wchar_t* nPathIcono, const int nPosIcono, const TCHAR *nTxt, const int cX, const int cY, const int cAncho, const int cAlto, const int cID, const long Estilos = WS_CHILD | WS_VISIBLE);
									// Función que crea el control DEdicionTextoEx partiendo de una clase DIcono que puede ser nullptr
		HWND						CrearEdicionTextoEx(DhWnd *nPadre, DIcono* nIcono, const TCHAR *nTxt, const int cX, const int cY, const int cAncho, const int cAlto, const int cID, const long Estilos = WS_CHILD | WS_VISIBLE);
									// Función que activa / desactiva el control
		virtual void				Activado(const BOOL nActivar);
									// Función que devuelve si el control está activado
		inline const BOOL			Activado(void) { return DhWnd::Activado(); }
									// Función que pinta este control en las coordenadas y el DC especificados
		void						Pintar(HDC hDC, const int cX = 0, const int cY = 0);
									// Función para obtener el DIcono de este control
		inline DIcono              &Icono(void) { return _Icono; }
									// Función para asignar un nuevo DIcono a este control
		void                        Icono(DIcono *nIcono, const BOOL nRepintar = TRUE);
									// Función que devuelve el texto de este control
		std::wstring			   &Texto(void) { return _Texto; }
									// Función que asigna un valor entero al texto
		virtual void                Texto(const LONG_PTR ValorEntero, const BOOL nRepintar = TRUE);
									// Función que asigna un valor decimal al texto
		virtual void                Texto(const double ValorDecimal, const int Decimales = 2, const BOOL nRepintar = TRUE);
									// Función que asigna el texto partiendo de un std::wstring
		virtual void				Texto(std::wstring &nTexto, const BOOL nRepintar = TRUE);
									// Función que asigna el texto partiendo de un wchar_t
		virtual void				Texto(const wchar_t *nTexto, const BOOL nRepintar = TRUE);
									// Función que inicia la animación del estado especificado
		void						Transicion(const DEdicionTextoEx_Transicion nTransicion);
									// Fuente que se mostrará en el control
		DFuente						Fuente;
									// Tipo de entrada permitida para el usuario
		DEdicionTextoEx_Entrada		Entrada;		
									// Alineación del texto dentro del control
		DEdicionTextoEx_Alineacion	Alineacion;
									// Skin con la configuración de colores
		DEdicionTextoEx_Skin        Skin;
									// Función que obtiene la posición del cursor de las coordenadas especificadas
		const size_t                HitTest(const int cX, const int cY);
									// Texto que se mostrará en el control antes de tener el foco o un texto al estilo HTML
		std::wstring                Placeholder;
									// Gestor de mensajes para este control
		LRESULT CALLBACK			GestorMensajes(UINT uMsg, WPARAM wParam, LPARAM lParam);
	  protected: /////////////////////
									// Determina si el caracter introducido es válido para el tipo de entrada actual
		const BOOL                 _EntradaPermitida(const wchar_t Caracter); 
									// Evento WM_PAINT
		void					   _Evento_Pintar(void);
									// Evento WM_MOUSEMOVE
		void					   _Evento_MouseMovimiento(const WPARAM wParam, const LPARAM lParam);
									// Evento WM_MOUSEDOWN*
		void					   _Evento_MousePresionado(const WPARAM wParam, const LPARAM lParam, const int Boton);
									// Evento WM_MOUSEUP*
		void					   _Evento_MouseSoltado(const WPARAM wParam, const LPARAM lParam, const int Boton);
									// Evento WM_MOUSELEAVE
		void					   _Evento_MouseSaliendo(void);
									// Evento WM_DOBLECLICK*
		void					   _Evento_MouseDobleClick(const WPARAM wParam, const LPARAM lParam, const int Boton);
									// Evento WM_KEYDOWN
		void                       _Evento_TeclaPresionada(WPARAM wParam, LPARAM lParam);
									// Evento WM_KEYUP
		void					   _Evento_TeclaSoltada(WPARAM wParam, LPARAM lParam);
									// Evento WM_CHAR
		void					   _Evento_Tecla(WPARAM wParam, LPARAM lParam);
									// Evento WM_SETFOCUS
		void                       _Evento_FocoAsignado(void);
									// Evento WM_KILLFOCUS
		void                       _Evento_FocoPerdido(void);
									// Evento WM_TIMER
		void					   _Evento_Temporizador(const INT_PTR tID);
									// Copiar
		void                       _ControlC(void);
									// Cortar
		void                       _ControlX(void);
									// Pegar
		void                       _ControlV(void);
									// Des-hacer
		void                       _ControlZ(void);
									// Re-hacer
		void                       _ControlY(void);
									// Función que borra el caracter antes del cursor, o la selección
		void                       _Borrar(void);
									// Función que borra el caracter por despues del cursor, o la selección
		void                       _Suprimir(void);
									// Función que borra el texto seleccionado
		void                       _BorrarTextoSeleccionado(void);
									// Función que agrega un nuevo texto a la lista de des-hacer / re-hacer
		void                       _AgregarTextoUndo(void);
									// Icono para mostrar a la izquierda
		DIcono                     _Icono;
									// Texto actual del control
		std::wstring               _Texto;
									// Textos guardados para el control Z y el control Y
		std::vector<std::wstring>  _Textos;
									// Posición de los cursores guardados para el control Z y el control Y
		std::vector<size_t>        _PosCursores;
									// Posición del des-hacer / re-hacer dentro del vector _Textos
		size_t                     _PosRedoUndo;
									// Función que devuelve un RECT con el tamaño y posición del texto dentro del control
		const RECT                 _PosicionTexto(std::wstring& nTexto, RECT &RC);
									// Color del texto para transiciones
		COLORREF                   _ColorTexto;
									// Color de la sombra texto para transiciones
		COLORREF                   _ColorTextoSombra;
									// Color del placeholder para transiciones
		COLORREF                   _ColorTextoPH;
									// Color de la sombra del placeholder para transiciones
		COLORREF                   _ColorTextoSombraPH;
									// Color del fondo para transiciones
		COLORREF                   _ColorFondo;
									// Color del borde para transiciones
		COLORREF                   _ColorBorde;
									// Color del cursor para transiciones
		COLORREF                   _ColorCursor;
									// Posición del cursor
		size_t                     _PosCursor;
									// Posición de la selección (si es la misma que _PosCursor no hay selección)
		size_t                     _PosSeleccion;
									// Objeto para la animación del cursor
		DAnimacion                 _AniCursor;
									// Objeto para las transiciones de estado
		DAnimacion                 _AniTransicion;		
									// Variable que determina si el control está presionado
		BOOL                       _Presionado;
	};



}
#endif