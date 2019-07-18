#ifndef DEDICIONTEXTO_H
#define DEDICIONTEXTO_H

#include "DControlEx.h"
#include "DAnimacion.h"
#include "DFuente.h"
#include "DIcono.h"

namespace DWL {

	#define DEDICIONTEXTOEX_MOSTRARDEBUG	FALSE

	class DEdicionTextoEx_Skin {
	  public:
					 DEdicionTextoEx_Skin(void);
		COLORREF     FondoNormal;
		COLORREF     FondoResaltado;
		COLORREF     FondoPresionado;
		COLORREF     FondoDesactivado;
		COLORREF     FondoSeleccion;

		COLORREF     Texto;
		COLORREF     TextoSombra;
		COLORREF     TextoResaltado;
		COLORREF     TextoResaltadoSombra;
		COLORREF     TextoPresionado;
		COLORREF     TextoPresionadoSombra;
		COLORREF     TextoDesactivado;
		COLORREF     TextoDesactivadoSombra;
		COLORREF     BordeNormal;
		COLORREF     BordeResaltado;
		COLORREF     BordePresionado;							

		COLORREF     Cursor;
					 // Fuente
		int			 FuenteTam;
		std::wstring FuenteNombre;
		BOOL         FuenteNegrita;
		BOOL         FuenteCursiva;
		BOOL         FuenteSubrayado;
		BOOL	 	 FuenteSombraTexto;
	};


	enum DEdicionTextoEx_Transicion {
		DEdicionTextoEx_Transicion_Normal,
		DEdicionTextoEx_Transicion_Resaltado,
		DEdicionTextoEx_Transicion_Presionado,
		DEdicionTextoEx_Transicion_Desactivado
	};

	enum DEdicionTextoEx_Alineacion {
		DEdicionTextoEx_Alineacion_Izquierda = DT_LEFT,
		DEdicionTextoEx_Alineacion_Centrado	 = DT_CENTER,
		DEdicionTextoEx_Alineacion_Derecha	 = DT_RIGHT
	};

	// Tipo de entrada que acepta por el usuario
	enum DEdicionTextoEx_Entrada {
		DEdicionTextoEx_Entrada_SinEntrada		 = 0, // Nada
		DEdicionTextoEx_Entrada_Texto			 = 1, // Todo
		DEdicionTextoEx_Entrada_ValoresEnteros	 = 2, // 0123456789
		DEdicionTextoEx_Entrada_ValoresDecimales = 3, // 0123456789.
		DEdicionTextoEx_Entrada_ValoresTiempo    = 4  // 0123456789:
	};

	class DEdicionTextoEx : public DControlEx {
	  public:
									DEdicionTextoEx(void);
		                           ~DEdicionTextoEx(void);
		HWND						CrearEdicionTextoEx(DhWnd *nPadre, const TCHAR *nTxt, const int cX, const int cY, const int cAncho, const int cAlto, const int cID, DIcono *nIcono = NULL, const long Estilos = WS_CHILD | WS_VISIBLE);
		virtual void				Activado(const BOOL nActivar);
		inline const BOOL			Activado(void) { return DhWnd::Activado(); }
		void						Pintar(HDC hDC, const int cX = 0, const int cY = 0);
		LRESULT CALLBACK			GestorMensajes(UINT uMsg, WPARAM wParam, LPARAM lParam);
		inline DIcono              &Icono(void) { return _Icono; }
		void                        Icono(DIcono *nIcono, const BOOL nRepintar = TRUE);
		std::wstring			   &Texto(void) { return _Texto; }
		virtual void                Texto(const LONG_PTR ValorEntero, const BOOL nRepintar = TRUE);
		virtual void                Texto(const double ValorDecimal, const int Decimales = 2, const BOOL nRepintar = TRUE);
		virtual void				Texto(std::wstring &nTexto, const BOOL nRepintar = TRUE);
		virtual void				Texto(const wchar_t *nTexto, const BOOL nRepintar = TRUE);

		void						Transicion(const DEdicionTextoEx_Transicion nTransicion);
		DFuente						Fuente;
//		BOOL                        TextoEditable;
		DEdicionTextoEx_Entrada		Entrada;		// Tipo de entrada permitida para el usuario
		DEdicionTextoEx_Alineacion	Alineacion;
		DEdicionTextoEx_Skin        Skin;
		const size_t                HitTest(const int cX, const int cY);
									// Texto que se mostrará en el control antes de tener el foco o un texto al estilo HTML
		std::wstring                Placeholder;
	  protected:
		const BOOL                 _EntradaPermitida(const wchar_t Caracter); // Determina si el caracter introducido es válido para el tipo de entrada actual
		void					   _Evento_Pintar(void);
		void					   _Evento_MouseMovimiento(const WPARAM wParam, const LPARAM lParam);
		void					   _Evento_MousePresionado(const WPARAM wParam, const LPARAM lParam, const int Boton);
		void					   _Evento_MouseSoltado(const WPARAM wParam, const LPARAM lParam, const int Boton);
		void					   _Evento_MouseSaliendo(void);
		void					   _Evento_MouseDobleClick(const WPARAM wParam, const LPARAM lParam, const int Boton);
		void                       _Evento_TeclaPresionada(WPARAM wParam, LPARAM lParam);
		void					   _Evento_TeclaSoltada(WPARAM wParam, LPARAM lParam);
		void					   _Evento_Tecla(WPARAM wParam, LPARAM lParam);
		void                       _Evento_FocoAsignado(void);
		void                       _Evento_FocoPerdido(void);
		void					   _Evento_Temporizador(const INT_PTR tID);

		void                       _ControlC(void);
		void                       _ControlX(void);
		void                       _ControlV(void);
		void                       _ControlZ(void);
		void                       _ControlY(void);

		void                       _Borrar(void);
		void                       _Suprimir(void);

		void                       _BorrarTextoSeleccionado(void);

		void                       _AgregarTextoUndo(void);

		DIcono                     _Icono;

		std::wstring               _Texto;
									// Textos guardados para el control Z y el control Y
		std::vector<std::wstring>  _Textos;
									// Posición de los cursores guardados para el control Z y el control Y
		std::vector<size_t>        _PosCursores;
		size_t                     _PosRedoUndo;

		const RECT                 _PosicionTexto(std::wstring& nTexto, RECT &RC);

		COLORREF                   _ColorTexto;
		COLORREF                   _ColorTextoSombra;
		COLORREF                   _ColorFondo;
		COLORREF                   _ColorBorde;
		COLORREF                   _ColorCursor;

		size_t                     _PosCursor;
		size_t                     _PosSeleccion;
//		size_t                     _FinSeleccion;
		DAnimacion                 _AniCursor;
		DAnimacion                 _AniTransicion;		
		BOOL                       _Presionado;
	};



}
#endif