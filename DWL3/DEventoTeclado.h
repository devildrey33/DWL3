#ifndef DEVENTOTECLADO_H
#define DEVENTOTECLADO_H

#define WPARAM_TO_DEVENTOTECLADO(WP) *reinterpret_cast<DWL::DEventoTeclado *>(WP)
#define DEVENTOTECLADO_TO_WPARAM(DE) reinterpret_cast<WPARAM>(&DE)

#include "DhWnd.h"

namespace DWL {
	class DEventoTeclado {
	  public:
										DEventoTeclado(void) : wParam(0), lParam(0), Wnd(NULL) { };
										DEventoTeclado(WPARAM nwParam, LPARAM nlParam, DhWnd *Control) : wParam(nwParam), lParam(nlParam), Wnd(Control) { };
		                               ~DEventoTeclado(void)	{ };
									    // Contador de repeticiones para este mensaje. 
										// MSDN : "The repeat count for the current message. The value is the number of times the keystroke is autorepeated as a result of the user holding down the key. If the keystroke is held long enough, multiple messages are sent. However, the repeat count is not cumulative."
		inline const int                Repeticiones(void)		{ return lParam & 0xffff; };
									    // MSDN : "The scan code. The value depends on the OEM."
		inline const int                Oem(void)				{ return (lParam >> 16) & 0xff; };
									    // Tecla extendida 
										// MSDN : "Indicates whether the key is an extended key, such as the right-hand ALT and CTRL keys that appear on an enhanced 101- or 102-key keyboard. The value is 1 if it is an extended key; otherwise, it is 0." 
		inline const BOOL               Extendida(void)			{ return (lParam >> 24) & 1; };
									    // Contexto 
										// MSDN : "The context code. The value is 1 if the ALT key is held down while the key is pressed; otherwise, the value is 0."
		inline const BOOL               Contexto(void)			{ return (lParam >> 29) & 1; };
									    // Estado previo
										// MSDN : "The previous key state. The value is 1 if the key is down before the message is sent, or it is 0 if the key is up."
		inline const BOOL               EstadoPrevio(void)		{ return (lParam >> 30) & 1; };
									    // Estado de transición 
										// MSDN : "The transition state. The value is 1 if the key is being released, or it is 0 if the key is being pressed."
		inline const BOOL               EstadoTransicion(void)	{ return (lParam >> 31) & 1; };
										// Llama a GetAsyncKeyState para determinar si la tecla Control está presionada
		inline const BOOL               Control(void)			{ return ((GetAsyncKeyState(VK_CONTROL) & 0x8000) != 0); }
										// Llama a GetAsyncKeyState para determinar si la tecla Alt está presionada
		inline const BOOL               Alt(void)				{ return ((GetAsyncKeyState(VK_MENU) & 0x8000) != 0); }
										// Llama a GetAsyncKeyState para determinar si la tecla Shift está presionada
		inline const BOOL               Shift(void)				{ return ((GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0); }
		
										// Caracter (solo para Evento_Tecla)
		inline const wchar_t            Caracter(void)			{ return static_cast<wchar_t>(wParam); };
										// Caracter (solo para Evento_TeclaPresionada, y Evento_TeclaSoltada)
		inline const int 	            TeclaVirtual(void)		{ return static_cast<int>(wParam); };
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
		WPARAM                          wParam;
										// wParam que devuelven los mensajes del mouse
		LPARAM                          lParam;
										// DhWnd base del Control, si necesitas un tipo de control concreto hay que hacer un reinterpret_cast.
		DhWnd                          *Wnd;
	  private: /////////////////////////// Miembros privados

										//! Constructor copia des-habilitado
										DEventoTeclado(const DEventoTeclado &) : wParam(0), lParam(0), Wnd(NULL) { };
										//! Operador = des-habilitado
		inline DEventoTeclado		   &operator=(const DEventoTeclado &) { return *this; };

	};
};

#endif