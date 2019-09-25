#ifndef DBARRADESPLAZAMIENTOEX_H
#define DBARRADESPLAZAMIENTOEX_H

#include "DBarraProgresoEx.h"
#include "DToolTipEx.h"
#include "DEventoMouse.h"

namespace DWL {


	enum DBarraEx_ToolTip {
		DBarraEx_ToolTip_SinToolTip,
		DBarraEx_ToolTip_Arriba,
		DBarraEx_ToolTip_Abajo,
		DBarraEx_ToolTip_Izquierda,
		DBarraEx_ToolTip_Derecha
	};



	class DBarraDesplazamientoEx : public DBarraProgresoEx	{
	  public:
													DBarraDesplazamientoEx(void);
											       ~DBarraDesplazamientoEx(void);
	   HWND											CrearBarraDesplazamientoEx(DhWnd *nPadre, const int cX, const int cY, const int cAncho, const int cAlto, const INT_PTR cID, const float nMinimo = 0.0f, const float nMaximo = 1.0f, const float nValor = 0.0, const DBarraEx_Alineacion nAlineacion = IzquierdaDerecha);
		
	   inline DBarraEx_Estado						Estado(void) { return _Estado; }
	   void											ToolTip(DBarraEx_ToolTip nValor);
	   void											OcultarToolTip(void);

	   // virtuales
	   virtual void									Evento_PintarBorde(HDC DC, RECT &RBorde);
	   virtual void									Evento_PintarBarra(HDC DC, RECT &RBarra);
	   virtual void									Evento_PintarFondo(HDC DC, RECT &RFondo);
		
	   virtual void									Evento_MouseMovimiento(DEventoMouse &DatosMouse) { };
	   std::function<void(DEventoMouse&)>			EventoMouseMovimiento;
	   virtual void									Evento_MousePresionado(DEventoMouse &DatosMouse) { };
	   std::function<void(DEventoMouse&)>			EventoMousePresionado;
	   virtual void									Evento_MouseSoltado(DEventoMouse &DatosMouse) { };
	   std::function<void(DEventoMouse&)>			EventoMouseSoltado;
	   virtual void									Evento_MostrarToolTip(float nValor, std::wstring &Texto);
	   std::function<void(float, std::wstring &)>	EventoMostrarToolTip;

//	   virtual void                             Resaltar(const BOOL Resaltado);

	   virtual const DhWnd_Tipo					TipoWnd(void) { return DhWnd_Tipo_BarraDesplazamientoEx; };

	   LRESULT CALLBACK							GestorMensajes(UINT uMsg, WPARAM wParam, LPARAM lParam);
	  protected:
		void								   _Evento_MouseMovimiento(WPARAM wParam, LPARAM lParam);
		void								   _Evento_MousePresionado(const int Boton, WPARAM wParam, LPARAM lParam);
		void								   _Evento_MouseSoltado(const int Boton, WPARAM wParam, LPARAM lParam);
		void                                   _Evento_MouseSaliendo(void);

	    DBarraEx_ToolTip		               _MostrarToolTip;
	    DToolTipEx							   _ToolTip;
	};
}

#endif