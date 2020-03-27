#ifndef DEVENTOMOUSERUEDA_H
#define DEVENTOMOUSERUEDA_H

#include "DhWnd.h"
#include "DMouse.h"

//! Espacio de nombres DWL
namespace DWL {

	#define WPARAM_TO_DEVENTOMOUSERUEDA(WP) *reinterpret_cast<DWL::DEventoMouseRueda *>(WP)
	#define DEVENTOMOUSERUEDA_TO_WPARAM(DE) reinterpret_cast<WPARAM>(&DE)

	//! Clase que contiene los parametros de un evento de la rueda del mouse, para pasarlos a la función de su ventana padre.

	class DEventoMouseRueda	{
	  public: //////////////////////////// Constructor por defecto
										DEventoMouseRueda(void) : wParam(0), lParam(0), Wnd(NULL), MousePosCliente({0, 0}) { };
										//! Constructor que define todos los valores
										/*!	Constructor asignador de datos.
											\fn			DEventoMouseRueda(const int cX, const int cY, const UINT cID);
											\param[in]	wParam	: WPARAM del evento.
											\param[in]	lParam  : LPARAM del evento.
											\param[in]	Wnd		: DhWnd base del Control.
											\return		No devuelve nada.
										*/
										DEventoMouseRueda(WPARAM nwParam, LPARAM nlParam, DhWnd *nhWnd) : wParam(nwParam), lParam(nlParam), Wnd(nhWnd) {
											DWL::DMouse::ObtenerPosicion(&MousePosCliente);
											ScreenToClient(Wnd->hWnd(), &MousePosCliente);
										};

										// Destructor
		                               ~DEventoMouseRueda() { };

										//! Función que devuelve la coordenada X de este evento relativa al área del cliente.
										/*! Función que devuelve la coordenada X de este evento relativa al área del cliente.
											\fn			inline const int X(void);
											\return		devuelve la coordenada X.
										*/
		inline const int                X(void) {
											return MousePosCliente.x;
										};

										//! Función que devuelve la coordenada X de este evento relativa a la pantalla.
										/*! Función que devuelve la coordenada X de este evento relativa a la pantalla.
											\fn			inline const int XPantalla(void);
											\return		devuelve la coordenada X.
										*/
		inline const int                XPantalla(void) {
											return GET_X_LPARAM(lParam); // Devuelve las coordenadas del mouse relativas a la pantalla....!!!!
										};

										//! Función que devuelve la coordenada Y de este evento relativa al área del cliente.
										/*! Función que devuelve la coordenada Y de este evento relativa al área del cliente.
											\fn			inline const int Y(void);
											\return		devuelve la coordenada Y.
										*/
		inline const int                Y(void) {
											return MousePosCliente.y;
										};

										//! Función que devuelve la coordenada Y de este evento relativa al área del cliente.
										/*! Función que devuelve la coordenada Y de este evento relativa al área del cliente.
											\fn			inline const int YPantalla(void);
											\return		devuelve la coordenada Y.
										*/
		inline const int                YPantalla(void) {
											return GET_Y_LPARAM(lParam);
										};

										/* Utilizar varios botones ademas de la rueda me parece chungo como poco, ya se me hace dificil pensar el controlar si algun boton del mouse está presionado ademas del desplazamiento de la rueda.) */
		inline const int                Boton(void) {
											WORD Estado = GET_KEYSTATE_WPARAM(wParam);
											if (Estado & MK_LBUTTON)  return 0;
											if (Estado & MK_RBUTTON)  return 1;
											if (Estado & MK_MBUTTON)  return 2;
											if (Estado & MK_XBUTTON1) return 3;
											if (Estado & MK_XBUTTON2) return 4;
											return -1;
										};

		inline const BOOL               ControlPresionado(void) { 
											return GET_KEYSTATE_WPARAM(wParam) & MK_CONTROL;
										};

		inline const BOOL               ShiftPresionado(void) { 
											return GET_KEYSTATE_WPARAM(wParam) & MK_SHIFT;
										};

		inline const short				Delta(void) {
											return GET_WHEEL_DELTA_WPARAM(wParam);
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

		WPARAM							wParam;
		LPARAM							lParam;
										// DhWnd base del Control, si necesitas un tipo de control concreto hay que hacer un reinterpret_cast.
		DhWnd                          *Wnd;
//		UINT_PTR                        ID;
		POINT                           MousePosCliente;
	};
};
#endif