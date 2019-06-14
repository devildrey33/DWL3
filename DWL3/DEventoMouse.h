#ifndef DEVENTOMOUSE_H
#define DEVENTOMOUSE_H

#include "DhWnd.h"

//! Espacio de nombres DWL
namespace DWL {

	#define WPARAM_TO_DEVENTOMOUSE(WP) *reinterpret_cast<DWL::DEventoMouse *>(WP)
	#define DEVENTOMOUSE_TO_WPARAM(DE) reinterpret_cast<WPARAM>(&DE)

	//! Clase que contiene los parametros de un evento del mouse (mouseup, down, move, click, dobleckick, etc...) para pasarlos a la función de su ventana padre.
	class DEventoMouse {
	  public: //////////////////////////// Miembros publicos

										//! Constructor.
										/*!	Constructor por defecto.
											\fn			DEventoMouse(void);
											\return		No devuelve nada.
										*/
										DEventoMouse(void) : wParam(0), lParam(0), Wnd(NULL), Boton(-1) {
										};

										//! Constructor que define todos los valores
										/*!	Constructor asignador de datos.
											\fn			DEventoMouse(const int cX, const int cY, const UINT cID, const UINT nBoton);
											\param[in]	wParam	: WPARAM del evento.
											\param[in]	lParam  : LPARAM del evento.
											\param[in]	cID		: ID del Control.
											\param[in]	hWnd    : HWND del Control.
											\param[in]	cBoton	: Botón del mouse pulsado (si es -1 es que no se ha pulsado ningún botón).
											\return		No devuelve nada.
										*/
										DEventoMouse(WPARAM nwParam, LPARAM nlParam, DhWnd *Control, const int cBoton = -1) :
											wParam(nwParam), lParam(nlParam), Wnd(Control), Boton(cBoton) {
										};

										//! Destructor.
										/*!	Destructor.
											\fn		   ~DEventoMouse(void);
											\return		No devuelve nada.
										*/
			                           ~DEventoMouse(void) {
										};

										//! Función que devuelve la coordenada X de este evento.
										/*! Función que devuelve la coordenada X de este evento.
											\fn			inline const int X(void);
											\return		devuelve la coordenada X.
										*/
		inline const int                X(void) {
											return GET_X_LPARAM(lParam);
										};

										//! Función que devuelve la coordenada Y de este evento.
										/*! Función que devuelve la coordenada Y de este evento. 
											\fn			inline const int Y(void);
											\return		devuelve la coordenada Y.
										*/
		inline const int                Y(void) {
											return GET_Y_LPARAM(lParam);
										};

										//! Función que devuelve el botón del mouse que se ha presionado.
										/*! Función que devuelve el botón del mouse que se ha presionado.
											\fn			const int Boton(void);
											\return		devuelve el boton del mouse presionado.
										*/
										// ANULADA (El mouseUp no devuelve el botón.....)
/*		const int                       Boton(void) {
											if (wParam & MK_LBUTTON)		return 0; // Botón izquierdo
											if (wParam & MK_MBUTTON)		return 1; // Botón derecho
											if (wParam & MK_RBUTTON)		return 2; // Botón medio
											if (wParam & MK_XBUTTON1)		return 3; // Botón extra 1
											if (wParam & MK_XBUTTON2)		return 4; // Botón extra 2						
											return -1;								  // No se ha presionado ningún botón del mouse.
										};*/

										//! Función que devuelve si la tecla Control está presionada.
										/*! Función que devuelve si la tecla Control está presionada.
											\fn			inline const int Control(void);
											\return		devuelve TRUE si la tecla control está presionada.
										*/
		inline const BOOL               Control(void) {
											return ((GetAsyncKeyState(VK_CONTROL) & 0x8000) != 0); //return (wParam & MK_CONTROL);
										};

										//! Función que devuelve si se ha presionado la tecla Shift.
										/*! Función que devuelve si se ha presionado la tecla Shift.
											\fn			inline const int Shift(void);
											\return		devuelve TRUE si la tecla shift está presionada.
										*/
		inline const BOOL               Shift(void) {
											return ((GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0);//return (wParam & MK_SHIFT);	// No acaba de funcionar, tirare de getasynckey
										};
										
										//! Función que devuelve la ID del control de donde proviene este evento.
										/*! Función que devuelve la ID del control de donde proviene este evento.
											\fn			inline const INT_PTR ID(void);
											\return		devuelve la id del control.
										*/
		inline const INT_PTR            ID(void) { if (Wnd != NULL) { return Wnd->ID(); } return NULL; }

										//! Función que devuelve el hWnd de la ventana / control de donde proviene este evento.
										/*! Función que devuelve el hWnd de la ventana / control de donde proviene este evento.
											\fn			inline const INT_PTR ID(void);
											\return		devuelve la id del control.
										*/
		inline const HWND               hWnd(void) { if (Wnd != NULL) { return Wnd->hWnd(); } return NULL; }
										// wParam que devuelven los mensajes del mouse
		WPARAM                          wParam;
										// wParam que devuelven los mensajes del mouse
		LPARAM                          lParam;
										// DhWnd base del Control, si necesitas un tipo de control concreto hay que hacer un reinterpret_cast.
		DhWnd                          *Wnd;
										// Botón presionado (puede ser -1)
		int                             Boton;

	  private: /////////////////////////// Miembros privados

										//! Constructor copia des-habilitado
										DEventoMouse(const DEventoMouse &) : wParam(0), lParam(0), Wnd(NULL), Boton(-1) { };
										//! Operador = des-habilitado
		inline DEventoMouse			    &operator=(const DEventoMouse &) { return *this; };


	};									//
	////////////////////////////////////// DEventoMouse
};

#endif
