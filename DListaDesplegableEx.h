
#ifndef DLISTADESPLEGABLEEX_H
#define DLISTADESPLEGABLEEX_H

#include "DControlDesplegableEx.h"
#include "DListaEx.h"

namespace DWL {
	class DListaDesplegableEx_Lista : public DListaEx {
	  public:
								DListaDesplegableEx_Lista(void) : DListaEx(), Padre(NULL)	{ };
		                       ~DListaDesplegableEx_Lista(void)								{ };
		void					Evento_FocoPerdido(HWND hWndNuevoFoco)						{ Visible(FALSE); };
		void					Evento_MouseSoltado(DEventoMouse &DatosMouse)				{ 
									Visible(FALSE); 
									SendMessage(Padre->hWnd(), DWL_LISTAEX_MOUSESOLTADO, reinterpret_cast<WPARAM>(&DatosMouse), 0);
								};
		void                    Evento_TeclaSoltada(DEventoTeclado &DatosTeclado)			{
									Visible(FALSE); 
									SendMessage(Padre->hWnd(), DWL_LISTAEX_MOUSESOLTADO, 0, 0);
								};
		DControlDesplegableEx  *Padre;
	};

	class DListaDesplegableEx : public DControlDesplegableEx {
	  public:
										DListaDesplegableEx(void);
		                               ~DListaDesplegableEx(void);
		void							CrearListaDesplegable(DhWnd *nPadre, const wchar_t *nTexto, const INT_PTR nID, const INT_PTR nIDIcono, const int cX, const int cY, const int cAncho, const int cAlto, const DEdicionTextoEx_Entrada Entrada, const UINT AltoLista);
		void							Evento_Desplegar(void);
		void                            AgregarItem(const wchar_t *nTexto, const INT_PTR nIcono = 0);
		LRESULT CALLBACK				GestorMensajes(UINT uMsg, WPARAM wParam, LPARAM lParam);
		inline void                     EliminarTodosLosItems(void) { _Lista.EliminarTodosLosItems(); };
	  protected:
		DListaDesplegableEx_Lista      _Lista;
		UINT                           _AltoLista;
	};

};
#endif