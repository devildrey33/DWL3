#ifndef DBARRADESPLAZAMIENTOEX_H
#define DBARRADESPLAZAMIENTOEX_H

#include "DBarraProgresoEx.h"
#include "DToolTipEx.h"
#include "DEventoMouse.h"
#include "DEventoMouseRueda.h"

namespace DWL {

	// Alineación para el tooltip
	enum DBarraEx_ToolTip {
		DBarraEx_ToolTip_SinToolTip,
		DBarraEx_ToolTip_Arriba,
		DBarraEx_ToolTip_Abajo,
		DBarraEx_ToolTip_Izquierda,
		DBarraEx_ToolTip_Derecha
	};


	// Barra de desplazamiento basada en la barra de progreso
	class DBarraDesplazamientoEx : public DBarraProgresoEx	{
	  public: //////////////////////////////////////// Constructor
													DBarraDesplazamientoEx(void);
													// Destructor
											       ~DBarraDesplazamientoEx(void);
												    // Función para crear la barra de desplazamiento
	   HWND											CrearBarraDesplazamientoEx(DhWnd *nPadre, const int cX, const int cY, const int cAncho, const int cAlto, const INT_PTR cID, const float nMinimo = 0.0f, const float nMaximo = 1.0f, const float nValor = 0.0, const DBarraEx_Alineacion nAlineacion = IzquierdaDerecha);
													// Función que devuelve el estado de la barra de desplazamiento
	   inline DBarraEx_Estado						Estado(void) { return _Estado; }
													// Función que asigna si se debe mostrar un tooltip, y con que alineación
	   void											ToolTip(DBarraEx_ToolTip nValor);
													// Función que oculta el tooltip
	   void											OcultarToolTip(void);
													// Evento virtual para pintar el borde 
	   virtual void									Evento_PintarBorde(HDC DC, RECT &RBorde);
													// Evento virtual para pintar la parte de la barra
	   virtual void									Evento_PintarBarra(HDC DC, RECT &RBarra);
													// Evento virtual para pintar el fondo de la barra
	   virtual void									Evento_PintarFondo(HDC DC, RECT &RFondo);
													// Evento virtual para recibir el mensaje WM_MOUSEMOVE				
	   virtual void									Evento_MouseMovimiento(DEventoMouse &DatosMouse) { };
													// Función lambda que enlaza con el mensaje WM_MOUSEMOVE
	   std::function<void(DEventoMouse&)>			EventoMouseMovimiento;
													// Evento virtual para recibir el mensaje WM_?MOUSEDOWN
	   virtual void									Evento_MousePresionado(DEventoMouse &DatosMouse) { };
													// Función lambda que enlaza con el mensaje WM_?MOUSEDOWN
	   std::function<void(DEventoMouse&)>			EventoMousePresionado;
													// Evento virtual para recibir el mensaje WM_?MOUSEUP
	   virtual void									Evento_MouseSoltado(DEventoMouse &DatosMouse) { };
													// Función lambda que enlaza con el mensaje WM_?MOUSEUP
	   std::function<void(DEventoMouse&)>			EventoMouseSoltado;
													// Evento virtual para recibir el mensaje WM_MOUSEWHEEL
	   virtual void                                 Evento_MouseRueda(DEventoMouseRueda &DatosMouse) { };
													// Función lambda que enlaza con el mensaje WM_MOUSEWHEEL
	   std::function<void(DEventoMouseRueda &)>		EventoMouseRueda;
													// Evento virtual que salta al mostrar el tooltip
	   virtual void									Evento_MostrarToolTip(float nValor, std::wstring &Texto);
													// Función lambda que salta al mostrar el tooltip
	   std::function<void(float, std::wstring &)>	EventoMostrarToolTip;
													// Función que devuelve el tipo de control DhWnd
	   virtual const DhWnd_Tipo						TipoWnd(void) { return DhWnd_Tipo_BarraDesplazamientoEx; };
													// Gestor de mensajes para la barra
	   LRESULT CALLBACK								GestorMensajes(UINT uMsg, WPARAM wParam, LPARAM lParam);
	  protected: ///////////////////////////////////// Función que enlaza con el mensaje WM_MOUSEMOVE
		void									   _Evento_MouseMovimiento(WPARAM wParam, LPARAM lParam);
													// Función que enlaza con los mensajes WM_?MOUSEDOWN
		void									   _Evento_MousePresionado(const int Boton, WPARAM wParam, LPARAM lParam);
													// Función que enlaza con los mensajes WM_?MOUSEUP
		void									   _Evento_MouseSoltado(const int Boton, WPARAM wParam, LPARAM lParam);
													// Función que enlaza con el mensaje WM_MOUSEWHEEL
		void									   _Evento_MouseRueda(WPARAM wParam, LPARAM lParam);
													// Función que enlaza con el mensaje WM_MOUSELEAVE
		void						               _Evento_MouseSaliendo(void);
													// Variable que determina si hay que mostrar el tooltip y como alinearlo
	    DBarraEx_ToolTip				           _MostrarToolTip;
													// Objeto para el tooltip
//	    DToolTipEx								   _ToolTip;
	};
}

#endif