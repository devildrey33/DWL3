#ifndef DCONTROL_H
#define DCONTROL_H

#include "DhWnd.h"

namespace DWL {

	/* Controles estandar del windows */
	class DControl : public DhWnd {
	  public: //////////////////////// Constructor
									DControl(void) : DhWnd(), _GestorMensajesOriginal(NULL) { }
									// Destructor
		                           ~DControl(void) { }
								    // Función que crea el control
		HWND						CrearControl(DhWnd *nPadre, const TCHAR *nNombre, const TCHAR *nTexto, const INT_PTR cID, const int cX, const int cY, const int cAncho, const int cAlto, DWORD nEstilos, DWORD nEstilosExtendidos, HBRUSH nColorFondo = NULL);
									// Función que devuelve el tipo de control
		virtual const DhWnd_Tipo	TipoWnd(void) { return DhWnd_Tipo_Control; };
									// Gestor de mensajes
		virtual LRESULT CALLBACK	GestorMensajes(UINT uMsg, WPARAM wParam, LPARAM lParam);
	  protected: ///////////////////// Función que enlaza este objeto con un control creado dentro de un dialogo
		void					   _ConectarControl(const UINT nID, DhWnd *nPadre);
									// Gestor de mensajes estatico
		static LRESULT CALLBACK    _GestorMensajes(HWND HandleVentana, UINT uMsg, WPARAM wParam, LPARAM lParam);
									//! WindowProcedure Orignal del control estándar
		WNDPROC                    _GestorMensajesOriginal;

	};


};

#endif