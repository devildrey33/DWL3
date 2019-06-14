#ifndef DCONTROL_H
#define DCONTROL_H

#include "DhWnd.h"

namespace DWL {

	/* Controles estandar del windows */
	class DControl : public DhWnd {
	  public:
									DControl(void) : DhWnd() { }
		                           ~DControl(void) { }

		HWND						CrearControl(DhWnd *nPadre, const TCHAR *nNombre, const TCHAR *nTexto, const INT_PTR cID, const int cX, const int cY, const int cAncho, const int cAlto, DWORD nEstilos, DWORD nEstilosExtendidos, HBRUSH nColorFondo = NULL);

		virtual LRESULT CALLBACK	GestorMensajes(UINT uMsg, WPARAM wParam, LPARAM lParam);
	  protected:
		void					   _ConectarControl(const UINT nID, DhWnd *nPadre);
		static LRESULT CALLBACK    _GestorMensajes(HWND HandleVentana, UINT uMsg, WPARAM wParam, LPARAM lParam);
									//! WindowProcedure Orignal del control estándar
		WNDPROC                    _GestorMensajesOriginal;

	};


};

#endif