#include "pch.h"
#include "DListaDesplegableEx.h"

//#define ID_LISTA  1000

namespace DWL {
	DListaDesplegableEx::DListaDesplegableEx(void) : _AltoLista(100)  {
	}


	DListaDesplegableEx::~DListaDesplegableEx(void)	{
	}

	void DListaDesplegableEx::CrearListaDesplegable(DhWnd *nPadre, const wchar_t *nTexto, const INT_PTR nID, const INT_PTR nIDIcono, const int cX, const int cY, const int cAncho, const int cAlto, const DEdicionTextoEx_Entrada Entrada, const UINT AltoLista) {
		CrearControlDesplegable(nPadre, nTexto, nID, nIDIcono, cX, cY, cAncho, cAlto, Entrada);
		RECT RV;
		GetWindowRect(_hWnd, &RV);
		_AltoLista = AltoLista;
		_Lista.Padre = this;
		_Lista.CrearListaEx(this, RV.left, RV.bottom, cAncho, AltoLista, NULL, WS_POPUP, WS_EX_NOACTIVATE | WS_EX_TOPMOST | WS_EX_TOOLWINDOW);
		_Lista.AgregarColumna(-1, DListaEx_Columna_Alineacion_Izquierda);
	}

	void DListaDesplegableEx::AgregarItem(const wchar_t *nTexto, const INT_PTR nIcono) {
		_Lista.AgregarItem(nIcono, -1, { nTexto });
	}

	void DListaDesplegableEx::Evento_Desplegar(void) {
		if (_Lista.Visible() == FALSE) {
			// Ajusto el tamaño de la lista si el (total de items * altura del item + 2 pixeles) es mas pequeño, la altura del item se mide con la altura de la fuente + el (padding * 2)
			if (static_cast<LONG_PTR>(_AltoLista) > (_Lista.TotalItems() * (_Lista.Fuente.Alto() + (DLISTAEX_PADDING * 2))) + 2)	// El +2 es el borde
				_AltoLista = static_cast<UINT>((_Lista.TotalItems() * (_Lista.Fuente.Alto() + (DLISTAEX_PADDING * 2))) + 2);

			RECT RV;
			GetWindowRect(_hWnd, &RV);
			SetWindowPos(_Lista.hWnd(), HWND_TOPMOST, RV.left, RV.bottom, RV.right - RV.left, _AltoLista, SWP_NOACTIVATE | SWP_FRAMECHANGED | SWP_SHOWWINDOW);
//			SetFocus(_Lista.hWnd()); // REVISADO
		}
		else {
			_Lista.Visible(FALSE);
		}		
	}

	LRESULT CALLBACK DListaDesplegableEx::GestorMensajes(UINT uMsg, WPARAM wParam, LPARAM lParam) {
		switch (uMsg) {
			case DWL_LISTAEX_MOUSESOLTADO :
				if (_Lista.ItemPresionado() != NULL) {
					_Edicion.Texto(_Lista.ItemPresionado()->Texto(0), TRUE);
				}
				SendMessage(GetParent(_hWnd), DWL_DESPLEGABLEEX_CAMBIO, static_cast<WPARAM>(ID()), 0);
				EventoCambiado();
				return 0;
		}
		return DControlDesplegableEx::GestorMensajes(uMsg, wParam, lParam);
	}
	
}