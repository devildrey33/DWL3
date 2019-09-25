#ifndef DCONTROLEX_H
#define DCONTROLEX_H

#include "DhWnd.h"

namespace DWL {
	/* 
		- Eventos virtuales y Eventos lambda
			- Los controles Ex deben tener eventos virtuales que empiezen por Evento_, y funciones lambda enlazadas a dichos eventos virtuales que empezaran por Evento 
			- Internamente los controles solo deben utilizar las funciones Lambda, de está forma las funciones lambda tendrán preferencia sobre los eventos virtuales.
			- La única excepción son las funciones de pintado, ya que si queremos re-emplazar una, va a necesitar mucho código, por no hablar de la sobrecarga durante el pintado.
	*/


	/* Controles de usuario fabricados desde 0 */
	class DControlEx : public DhWnd {
	  public:
									DControlEx(void) : DhWnd() { }
		                           ~DControlEx(void) { }

		inline virtual void			Activado(const BOOL nActivar)	{ EnableWindow(_hWnd, nActivar); };
		inline virtual const BOOL	Activado(void)					{ return IsWindowEnabled(_hWnd); };

		virtual const DhWnd_Tipo	TipoWnd(void)					{ return DhWnd_Tipo_ControlEx; };

		virtual LRESULT CALLBACK	GestorMensajes(UINT uMsg, WPARAM wParam, LPARAM lParam);
//		inline void					Repintar(void) { RedrawWindow(hWnd(), NULL, NULL, RDW_INVALIDATE | RDW_INTERNALPAINT); };
	  protected:
		HWND						CrearControlEx(DhWnd *nPadre, const TCHAR *nNombre, const TCHAR *nTexto, const INT_PTR cID, const int cX, const int cY, const int cAncho, const int cAlto, DWORD nEstilos, DWORD nEstilosExtendidos, UINT nEstilosClase = NULL, HBRUSH nColorFondo = NULL);
		void					   _ConectarControl(const UINT nID, DhWnd *nPadre);
		static LRESULT CALLBACK    _GestorMensajes(HWND HandleVentana, UINT uMsg, WPARAM wParam, LPARAM lParam);
	};



}

#endif