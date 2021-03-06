#ifndef DMARCAEX_H
#define DMARCAEX_H

#include "DControlEx.h"
#include "DAnimacion.h"
#include "DFuente.h"
#include "DEventoMouse.h"
#include "DIcono.h"

namespace DWL {

	class DMarcaEx_Skin {
	  public:
						DMarcaEx_Skin(void);
						// Colores por defecto
		COLORREF		FondoNormal;
		COLORREF		FondoResaltado;
		COLORREF		FondoPresionado;
		COLORREF		FondoMarcaNormal;
		COLORREF		FondoMarcaResaltado;
		COLORREF		FondoMarcaPresionado;
		COLORREF		FondoMarcaDesactivado;
		COLORREF		BordeNormal;
		COLORREF		BordeResaltado;
		COLORREF		BordePresionado;
		COLORREF		TextoNormal;
		COLORREF		TextoResaltado;
		COLORREF		TextoPresionado;
		COLORREF		TextoDesactivado;
		COLORREF		TextoSombra;
						// Fuente
		int				FuenteTam;
		std::wstring	FuenteNombre;
		BOOL			FuenteNegrita;
		BOOL			FuenteCursiva;
		BOOL			FuenteSubrayado;
		BOOL			FuenteSombraTexto;
	};

	enum DMarcaEx_Estado {
		DMarcaEx_Estado_Normal,
		DMarcaEx_Estado_Resaltado,
		DMarcaEx_Estado_Presionado
	};

	enum DMarcaEx_Transicion {
		DMarcaEx_Transicion_Normal,
		DMarcaEx_Transicion_Resaltado,
		DMarcaEx_Transicion_Presionado,
		DMarcaEx_Transicion_Desactivado
	};

	class DMarcaEx : public DControlEx	{
	  public:
											DMarcaEx(void);
								           ~DMarcaEx(void);

		HWND								CrearMarcaEx(DhWnd *nPadre, const TCHAR *nTxt, const int cX, const int cY, const int cAncho, const int cAlto, const INT_PTR cID, const INT_PTR IDIconoMarca, const long Estilos = WS_CHILD | WS_VISIBLE);
		
		virtual void						Evento_MouseSaliendo(void)							{ };
		std::function<void(void)>			EventoMouseSaliendo;
		virtual void						Evento_MouseMovimiento(DEventoMouse& DatosMouse)	{ };
		std::function<void(DEventoMouse&)>	EventoMouseMovimiento;
		virtual void						Evento_MousePresionado(DEventoMouse& DatosMouse)	{ };
		std::function<void(DEventoMouse&)>	EventoMousePresionado;
		virtual void						Evento_MouseSoltado(DEventoMouse& DatosMouse)		{ };
		std::function<void(DEventoMouse&)>	EventoMouseSoltado;
		virtual void						Evento_MouseClick(DEventoMouse& DatosMouse)			{ };
		std::function<void(DEventoMouse&)>	EventoMouseClick;

		virtual const DhWnd_Tipo			TipoWnd(void) { return DhWnd_Tipo_EtiquetaEx; };
		LRESULT CALLBACK					GestorMensajes(UINT uMsg, WPARAM wParam, LPARAM lParam);
		inline const BOOL					Marcado(void) { return _Marcado; }
		void								Marcado(const BOOL nMarcar);
		virtual void						Activado(const BOOL nActivar);
		inline const BOOL					Activado(void) { return DhWnd::Activado(); }
		void								Pintar(HDC DC);
											// Inicia una transición desde el estado actual al estado especificado
		void								Transicion(const DMarcaEx_Transicion nTransicion);
		DFuente								Fuente;
		DMarcaEx_Skin						Skin;
	  protected:
		void						       _Evento_MouseMovimiento(WPARAM wParam, LPARAM lParam);
		void						       _Evento_MousePresionado(const int Boton, WPARAM wParam, LPARAM lParam);
		void							   _Evento_MouseSoltado(const int Boton, WPARAM wParam, LPARAM lParam);
		void							   _Evento_MouseSaliendo(void);
		void                               _Evento_Pintar(void);
		std::wstring                       _Texto;
		BOOL                               _Marcado;
		DMarcaEx_Estado                    _Estado;
		DIcono                             _Icono;
		DAnimacion                         _AniTransicion;
		COLORREF                           _ColorFondoMarca;
		COLORREF                           _ColorFondo;
		COLORREF                           _ColorBorde;
		COLORREF                           _ColorTexto;
	};
	
};

#endif