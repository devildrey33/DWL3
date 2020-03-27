#ifndef DCONTROLEX_H
#define DCONTROLEX_H

#include "DhWnd.h"

namespace DWL {
	/* 
		- Eventos virtuales y Eventos lambda
			- Los controles Ex deben tener eventos virtuales que empiezen por Evento_, y funciones lambda enlazadas a dichos eventos virtuales que empezaran por Evento 
			- Internamente los controles solo deben utilizar las funciones Lambda, de est� forma las funciones lambda tendr�n preferencia sobre los eventos virtuales.
			- La �nica excepci�n son las funciones de pintado, ya que si queremos re-emplazar una, va a necesitar mucho c�digo, por no hablar de la sobrecarga durante el pintado.
	*/


	/* Controles de usuario fabricados desde 0 */
	class DControlEx : public DhWnd {
	  public: //////////////////////// Constructor
									DControlEx(void) : DhWnd() { }
									// Destructor
		                           ~DControlEx(void) { }
									// Funci�n virtual para activar / desactivar el control extendido
		inline virtual void			Activado(const BOOL nActivar)	{ EnableWindow(_hWnd, nActivar); };
									// Funci�n virtual que devuelve si el control extendido est� activado
		inline virtual const BOOL	Activado(void)					{ return IsWindowEnabled(_hWnd); };
									// Tipo de control extendido
		virtual const DhWnd_Tipo	TipoWnd(void)					{ return DhWnd_Tipo_ControlEx; };
									// Gestor de mensajes para el control extendido
		virtual LRESULT CALLBACK	GestorMensajes(UINT uMsg, WPARAM wParam, LPARAM lParam);
	  protected: ///////////////////// Funci�n protegida para crear el control extendido
		HWND						CrearControlEx(DhWnd *nPadre, const TCHAR *nNombre, const TCHAR *nTexto, const INT_PTR cID, const int cX, const int cY, const int cAncho, const int cAlto, DWORD nEstilos, DWORD nEstilosExtendidos, UINT nEstilosClase = NULL, HBRUSH nColorFondo = NULL);
									// Funci�n para conectar este objeto con un control creado en un dialogo
		void					   _ConectarControl(const UINT nID, DhWnd *nPadre);
									// Gestor de mensajes estatico
		static LRESULT CALLBACK    _GestorMensajes(HWND HandleVentana, UINT uMsg, WPARAM wParam, LPARAM lParam);
	};



}

#endif