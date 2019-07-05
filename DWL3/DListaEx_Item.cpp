#include "pch.h"
#include "DListaEx.h"


namespace DWL {
	
	DListaEx_Item::DListaEx_Item() : _Lista(NULL), _ColorTexto(0), _ColorTextoSombra(0), _ColorFondo(0), Seleccionado(FALSE) {
	}
	
	/*
	DListaEx_Item::DListaEx_Item(const TCHAR *nTexto, DListaIconos_Icono *nIcono) : _Icono(nIcono), Seleccionado(FALSE) {
		Texto = nTexto;
	}

	DListaEx_Item::DListaEx_Item(std::wstring &nTexto, DListaIconos_Icono *nIcono) : Texto(nTexto), _Icono(nIcono), Seleccionado(FALSE) {
	}

	*/

	DListaEx_Item::~DListaEx_Item(void)	{
		for (size_t i = 0; i < _SubItems.size(); i++) {
			delete _SubItems[i];
		}
		_SubItems.resize(0);
	}

	void DListaEx_Item::Icono(const int nIconoRecursos) {
		_Icono.CrearIconoRecursos(nIconoRecursos, DLISTAEX_TAMICONO, DLISTAEX_TAMICONO);
	}

	void DListaEx_Item::_TransicionNormal(void) {
		_Transicion((Seleccionado == TRUE) ? DListaEx_TransicionItem_Seleccionado : DListaEx_TransicionItem_Normal);
	}

	void DListaEx_Item::_TransicionResaltado(void) {
		_Transicion((Seleccionado == TRUE) ? DListaEx_TransicionItem_SeleccionadoResaltado : DListaEx_TransicionItem_Resaltado);
	}

//	void DListaEx_Item::_

	void DListaEx_Item::_Transicion(const DListaEx_TransicionItem nTransicion) {
		DWORD Duracion = DhWnd::TiempoAnimaciones;
		if (_AniTransicion.Animando() == TRUE) {
			Duracion = _AniTransicion.TiempoActual();
			_AniTransicion.Terminar();
		}

		COLORREF TextoHasta = 0, SombraHasta = 0, FondoHasta = 0;
		switch (nTransicion) {
			case DListaEx_TransicionItem_Normal:
				TextoHasta  = _Lista->Skin.TextoItemNormal;
				SombraHasta = _Lista->Skin.TextoItemSombra;
				if (_Lista->_MouseDentro == TRUE) FondoHasta  = _Lista->SkinScroll.FondoResaltado;
				else                              FondoHasta  = _Lista->SkinScroll.FondoNormal;
				#if DLISTAEX_MOSTRARDEBUG == TRUE
					Debug_Escribir_Varg(L"DListaEx_Item::_Transicion('%s' -> Normal)\n", _SubItems[0]->Texto.c_str());
				#endif			
				break;
			case DListaEx_TransicionItem_Resaltado:
				TextoHasta  = _Lista->Skin.TextoItemResaltado;
				SombraHasta = _Lista->Skin.TextoItemSombra;
				FondoHasta  = _Lista->Skin.FondoItemResaltado;
				#if DLISTAEX_MOSTRARDEBUG == TRUE
					Debug_Escribir_Varg(L"DListaEx_Item::_Transicion('%s' -> Resaltado)\n", _SubItems[0]->Texto.c_str());
				#endif
				break;
			case DListaEx_TransicionItem_Seleccionado:
				TextoHasta  = _Lista->Skin.TextoItemSeleccionado;
				SombraHasta = _Lista->Skin.TextoItemSeleccionadoSombra;
				FondoHasta  = _Lista->Skin.FondoItemSeleccionado;
				#if DLISTAEX_MOSTRARDEBUG == TRUE
					Debug_Escribir_Varg(L"DListaEx_Item::_Transicion('%s' -> Seleccionado)\n", _SubItems[0]->Texto.c_str());
				#endif
				break;
			case DListaEx_TransicionItem_SeleccionadoResaltado :
				TextoHasta  = _Lista->Skin.TextoItemSeleccionadoResaltado;
				SombraHasta = _Lista->Skin.TextoItemSeleccionadoSombra;
				FondoHasta  = _Lista->Skin.FondoItemSeleccionadoResaltado;
				#if DLISTAEX_MOSTRARDEBUG == TRUE
					Debug_Escribir_Varg(L"DListaEx_Item::_Transicion('%s' -> Seleccionado resaltado)\n", _SubItems[0]->Texto.c_str());
				#endif
				break;
			case DListaEx_TransicionItem_Presionado :
				TextoHasta  = _Lista->Skin.TextoItemPresionado;
				SombraHasta = _Lista->Skin.TextoItemSeleccionadoSombra;
				FondoHasta  = _Lista->Skin.FondoItemPresionado;
				#if DLISTAEX_MOSTRARDEBUG == TRUE
					Debug_Escribir_Varg(L"DListaEx_Item::_Transicion('%s' -> Seleccionado presionado)\n", _SubItems[0]->Texto.c_str());
				#endif
				break;
		}

		_AniTransicion.Iniciar(_ColorTexto, TextoHasta, _ColorTextoSombra, SombraHasta, _ColorFondo, FondoHasta, Duracion, [=](DAnimacion::Valores &Datos, const BOOL Terminado) {
			_ColorTexto			= Datos[0].Color();
			_ColorTextoSombra	= Datos[1].Color();
			_ColorFondo			= Datos[2].Color();
			_Lista->RepintarAni();
		});
	}
}