

#ifndef DVENTANA_H
#define DVENTANA_H

#include "DhWnd.h"


namespace DWL {

	class DVentana : public DhWnd {
	public:
									DVentana(void) : DhWnd() { }
		                           ~DVentana(void) { }
									// Función para crear una ventana																																																	  							
		HWND						CrearVentana(DhWnd *nPadre, const TCHAR *nNombre, const TCHAR *nTexto, const int cX, const int cY, const int cAncho, const int cAlto, DWORD nEstilos, DWORD nEstilosExtendidos = NULL, UINT nEstilosClase = NULL, HMENU nMenu = NULL, HBRUSH nColorFondo = NULL, const int nIconoRecursos = 32512);

		void						Titulo(std::wstring &nTitulo);

		DBarraTareas                BarraTareas;

		void						Opacidad(const BYTE nNivel);
		const BYTE                  Opacidad(void);

		inline virtual void			Repintar(void) { RedrawWindow(hWnd(), NULL, NULL, RDW_INVALIDATE | RDW_INTERNALPAINT | RDW_FRAME); };

		virtual LRESULT CALLBACK	GestorMensajes(UINT uMsg, WPARAM wParam, LPARAM lParam);
	protected:
		static LRESULT CALLBACK    _GestorMensajes(HWND HandleVentana, UINT uMsg, WPARAM wParam, LPARAM lParam);
	};
};
#endif