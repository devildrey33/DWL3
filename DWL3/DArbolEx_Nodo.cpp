#include "pch.h"
#include "DArbolEx.h"
//#include "DListaIconos.h"


namespace DWL {

	DArbolEx_Nodo::DArbolEx_Nodo(void) :	_Siguiente(NULL), _Anterior(NULL), _Ancestros(0), _AnchoTexto(0), _Arbol(NULL), _Padre(NULL),
											_ColorExpansor(DArbolEx_Skin::ExpansorNormal), _ColorTexto(DArbolEx_Skin::TextoNodoNormal), _ColorTextoSombra(DArbolEx_Skin::TextoNodoSombra), _ColorFondo(DBarraScrollEx_Skin::FondoNormal),
											Expandido(FALSE), Seleccionado(FALSE), _SubSeleccionado(FALSE), _MostrarExpansor(DArbolEx_MostrarExpansor_Auto), _Activado(TRUE) {
	};
	
	DArbolEx_Nodo::~DArbolEx_Nodo(void) {
		// Elimino de la memória los hijos que pueda tener
		for (size_t i = 0; i < _Hijos.size(); i++) delete _Hijos[i]; 
	};

	/* Devuelve TRUE o FALSE dependiendo de si hay que mostrar el expansor o no.. (si MostrarExpansor está en Auto, devolverá TRUE si el nodo tiene hijos, FALSE enc aso contrario) */
	const DArbolEx_MostrarExpansor DArbolEx_Nodo::MostrarExpansor(void) {
		if (_MostrarExpansor == DArbolEx_MostrarExpansor_Auto) {
			DArbolEx_Expansor EPD = _Arbol->ExpansorPorDefecto();
			if (_Hijos.size() == 0) 
				return DArbolEx_MostrarExpansor_Ocultar;
			switch (EPD) {
				case DArbolEx_Expansor_Triangulo		:  return DArbolEx_MostrarExpansor_MostrarTriangulo;
				case DArbolEx_Expansor_TrianguloLinea	:  return DArbolEx_MostrarExpansor_MostrarTrianguloLinea;
				case DArbolEx_Expansor_Rectangulo		:  return DArbolEx_MostrarExpansor_MostrarRectangulo;
			}
		}
		return _MostrarExpansor;
	}


	void DArbolEx_Nodo::Icono(const int nIconoRecursos) {
		_Icono.CrearIconoRecursos(nIconoRecursos, DARBOLEX_TAMICONO, DARBOLEX_TAMICONO);
	}

	void DArbolEx_Nodo::Icono(const GUID nIconoKnownFolder) {
		_Icono.CrearIconoKnownFolder(nIconoKnownFolder, DARBOLEX_TAMICONO, DARBOLEX_TAMICONO);
	}

	void DArbolEx_Nodo::Icono(const wchar_t *nIconoPath, const int nIconoPos) {
		_Icono.CrearIconoExterno(nIconoPath, DARBOLEX_TAMICONO, DARBOLEX_TAMICONO, nIconoPos);
	}

	void DArbolEx_Nodo::BorrarHijos(void) {
		_Arbol->_Repintar = TRUE;
		for (size_t i = 0; i < _Hijos.size(); i++) {
			delete _Hijos[i];
		}
		_Hijos.resize(0);
	}

	/*
	const size_t DArbolEx_Nodo::Posicion(void) {
		if (_Padre != NULL) { 
			return std::find(_Padre->_Hijos.begin(), _Padre->_Hijos.end(), this) - _Padre->_Hijos.begin(); 
		}
		else { 
			return std::find(_Arbol->_Raiz._Hijos.begin(), _Arbol->_Raiz._Hijos.end(), this) - _Arbol->_Raiz._Hijos.begin(); 
		}
	}*/

	void DArbolEx_Nodo::_TransicionExpansor(const DArbolEx_TransicionExpansor nTransicion) {
		DWORD Duracion = DhWnd::TiempoAnimaciones;
		if (_AniTransicionExpansor.Animando() == TRUE) {
			Duracion = _AniTransicionExpansor.TiempoActual();
			_AniTransicionExpansor.Terminar();
		}

		COLORREF ExpansorHasta = 0;
		switch (nTransicion) {
			case DArbolEx_TransicionExpansor_Normal:
				ExpansorHasta = DArbolEx_Skin::ExpansorNormal;
				break;
			case DArbolEx_TransicionExpansor_Resaltado:
				ExpansorHasta = DArbolEx_Skin::ExpansorResaltado;
				break;
			case DArbolEx_TransicionExpansor_Presionado:
				ExpansorHasta = DArbolEx_Skin::ExpansorPresionado;
				break;
		}

		_AniTransicionExpansor.Iniciar(_ColorExpansor, ExpansorHasta, Duracion, [=](DAnimacion::Valores &Datos, const BOOL Terminado) {
			_ColorExpansor = Datos[0].Color();
			_Arbol->RepintarAni();
		});
	}
	
	void DArbolEx_Nodo::_Transicion(const DArbolEx_TransicionNodo nTransicion) {
		DWORD Duracion = DhWnd::TiempoAnimaciones;
		if (_AniTransicion.Animando() == TRUE) {
			Duracion = _AniTransicion.TiempoActual();
			_AniTransicion.Terminar();
		}

		COLORREF TextoHasta = 0, SombraHasta = 0, FondoHasta = 0;
		switch (nTransicion) {
			case DArbolEx_TransicionNodo_Normal:
				TextoHasta  = DArbolEx_Skin::TextoNodoNormal;
				SombraHasta = DArbolEx_Skin::TextoNodoSombra;
				if (_Arbol->_MouseDentro == TRUE) FondoHasta  = DBarraScrollEx_Skin::FondoResaltado;
				else                              FondoHasta  = DBarraScrollEx_Skin::FondoNormal;
				#if DARBOLEX_MOSTRARDEBUG == TRUE
					Debug_Escribir_Varg(L"DArbolEx_Nodo::_Transicion('%s' -> Normal)\n", Texto.c_str());
				#endif			
				break;
			case DArbolEx_TransicionNodo_Resaltado:
				TextoHasta  = DArbolEx_Skin::TextoNodoResaltado;
				SombraHasta = DArbolEx_Skin::TextoNodoSombra;
				FondoHasta  = DArbolEx_Skin::FondoNodoResaltado;
				#if DARBOLEX_MOSTRARDEBUG == TRUE
					Debug_Escribir_Varg(L"DArbolEx_Nodo::_Transicion('%s' -> Resaltado)\n", Texto.c_str());
				#endif
				break;
			case DArbolEx_TransicionNodo_Seleccionado:
				TextoHasta  = DArbolEx_Skin::TextoNodoSeleccionado;
				SombraHasta = DArbolEx_Skin::TextoNodoSeleccionadoSombra;
				FondoHasta  = DArbolEx_Skin::FondoNodoSeleccionado;
				#if DARBOLEX_MOSTRARDEBUG == TRUE
					Debug_Escribir_Varg(L"DArbolEx_Nodo::_Transicion('%s' -> Seleccionado)\n", Texto.c_str());
				#endif
				break;
			case DArbolEx_TransicionNodo_SeleccionadoResaltado :
				TextoHasta  = DArbolEx_Skin::TextoNodoSeleccionadoResaltado;
				SombraHasta = DArbolEx_Skin::TextoNodoSeleccionadoSombra;
				FondoHasta  = DArbolEx_Skin::FondoNodoSeleccionadoResaltado;
				#if DARBOLEX_MOSTRARDEBUG == TRUE
					Debug_Escribir_Varg(L"DArbolEx_Nodo::_Transicion('%s' -> Seleccionado resaltado)\n", Texto.c_str());
				#endif
				break;
			case DArbolEx_TransicionNodo_SeleccionadoPresionado :
				TextoHasta  = DArbolEx_Skin::TextoNodoPresionado;
				SombraHasta = DArbolEx_Skin::TextoNodoSeleccionadoSombra;
				FondoHasta  = DArbolEx_Skin::FondoNodoPresionado;
				#if DARBOLEX_MOSTRARDEBUG == TRUE
					Debug_Escribir_Varg(L"DArbolEx_Nodo::_Transicion('%s' -> Seleccionado presionado)\n", Texto.c_str());
				#endif
				break;
			case DArbolEx_TransicionNodo_SubSeleccionado:
				TextoHasta  = DArbolEx_Skin::TextoNodoSubSeleccionado;
				SombraHasta = DArbolEx_Skin::TextoNodoSubSeleccionadoSombra;
				FondoHasta  = DArbolEx_Skin::FondoNodoSubSeleccionado;
				#if DARBOLEX_MOSTRARDEBUG == TRUE
				Debug_Escribir_Varg(L"DArbolEx_Nodo::_Transicion('%s' -> SubSeleccionado resaltado)\n", Texto.c_str());
				#endif
				break;
			case DArbolEx_TransicionNodo_SubSeleccionadoResaltado:
				TextoHasta  = DArbolEx_Skin::TextoNodoSubSeleccionadoResaltado;
				SombraHasta = DArbolEx_Skin::TextoNodoSubSeleccionadoSombra;
				FondoHasta  = DArbolEx_Skin::FondoNodoSubSeleccionadoResaltado;
				#if DARBOLEX_MOSTRARDEBUG == TRUE
					Debug_Escribir_Varg(L"DArbolEx_Nodo::_Transicion('%s' -> SubSeleccionado resaltado)\n", Texto.c_str());
				#endif
				break;
			case DArbolEx_TransicionNodo_SubSeleccionadoPresionado:
				TextoHasta  = DArbolEx_Skin::TextoNodoPresionado;
				SombraHasta = DArbolEx_Skin::TextoNodoSeleccionadoSombra;
				FondoHasta  = DArbolEx_Skin::FondoNodoPresionado;
				#if DARBOLEX_MOSTRARDEBUG == TRUE
					Debug_Escribir_Varg(L"DArbolEx_Nodo::_Transicion('%s' -> SubSeleccionado presionado)\n", Texto.c_str());
				#endif
				break;
			case DArbolEx_TransicionNodo_Desactivado:
				TextoHasta  = DArbolEx_Skin::TextoNodoDesactivado;
				SombraHasta = DArbolEx_Skin::TextoNodoSombra;
				if (_Arbol->_MouseDentro == TRUE) FondoHasta  = DBarraScrollEx_Skin::FondoResaltado;
				else                              FondoHasta  = DBarraScrollEx_Skin::FondoNormal;
				#if DARBOLEX_MOSTRARDEBUG == TRUE
					Debug_Escribir_Varg(L"DArbolEx_Nodo::_Transicion('%s' -> Desactivado)\n", Texto.c_str());
				#endif
				break;
			case DArbolEx_TransicionNodo_DesactivadoResaltado	:
				TextoHasta  = DArbolEx_Skin::TextoNodoDesactivado;
				SombraHasta = DArbolEx_Skin::TextoNodoSombra;
				FondoHasta  = DArbolEx_Skin::FondoNodoResaltado;
				#if DARBOLEX_MOSTRARDEBUG == TRUE
					Debug_Escribir_Varg(L"DArbolEx_Nodo::_Transicion('%s' -> Desactivado resaltado)\n", Texto.c_str());
				#endif
				break;
		}

		_AniTransicion.Iniciar(_ColorTexto, TextoHasta, _ColorTextoSombra, SombraHasta, _ColorFondo, FondoHasta, Duracion, [=](DAnimacion::Valores &Datos, const BOOL Terminado) {
			_ColorTexto			= Datos[0].Color();
			_ColorTextoSombra	= Datos[1].Color();
			_ColorFondo			= Datos[2].Color();
			_Arbol->RepintarAni();
		});
	}

	void DArbolEx_Nodo::Activado(const BOOL nActivado) {
		_AniTransicion.Terminar();
		_Activado = nActivado;
		
		if (_Activado == TRUE) {
			if (Seleccionado == TRUE) {
				if (_SubSeleccionado == TRUE) {					
					_ColorTexto			= DArbolEx_Skin::TextoNodoSubSeleccionado;
					_ColorTextoSombra	= DArbolEx_Skin::TextoNodoSubSeleccionadoSombra;
					_ColorFondo			= DArbolEx_Skin::FondoNodoSubSeleccionado;;
				}
				else {
					_ColorTexto			= DArbolEx_Skin::TextoNodoSeleccionado;
					_ColorTextoSombra	= DArbolEx_Skin::TextoNodoSeleccionadoSombra;
					_ColorFondo			= DArbolEx_Skin::FondoNodoSeleccionado;
				}
			}
			else {
				_ColorTexto			= DArbolEx_Skin::TextoNodoNormal;
				_ColorTextoSombra	= DArbolEx_Skin::TextoNodoSombra;
				_ColorFondo			= _Arbol->_ColorFondo;
			}
		}
		else {
			_ColorTexto			= DArbolEx_Skin::TextoNodoDesactivado;
			_ColorTextoSombra	= DArbolEx_Skin::TextoNodoSombra;
			_ColorFondo			= _Arbol->_ColorFondo;
		}
		
	}


	void DArbolEx_Nodo::_AsignarColores(COLORREF nColTexto, COLORREF nColTextoSombra, COLORREF nColFondo) {
		_AniTransicion.Terminar();
		_ColorTexto			= nColTexto;
		_ColorTextoSombra	= nColTextoSombra;
		_ColorFondo			= nColFondo;
	}

	// Hace la transición a resaltado
	void DArbolEx_Nodo::_TransicionResaltado(void) {
		_Transicion(
			(_Activado == TRUE) ? 
				(_SubSeleccionado == TRUE) ? 
					DArbolEx_TransicionNodo_SubSeleccionadoResaltado	 :
					(Seleccionado == TRUE) ?
						DArbolEx_TransicionNodo_SeleccionadoResaltado : 
						DArbolEx_TransicionNodo_Resaltado : 
				DArbolEx_TransicionNodo_DesactivadoResaltado
		);
	}

	// Hace la transición a normal
	void DArbolEx_Nodo::_TransicionNormal(void) {
		_Transicion(
			(_Activado == TRUE) ? 
				(_SubSeleccionado == TRUE) ? 
					DArbolEx_TransicionNodo_SubSeleccionado	 :
					(Seleccionado == TRUE) ?
						DArbolEx_TransicionNodo_Seleccionado : 
						DArbolEx_TransicionNodo_Normal : 
				DArbolEx_TransicionNodo_Desactivado
		);
	}

}