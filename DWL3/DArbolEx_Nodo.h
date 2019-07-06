#pragma once

#include "DIcono.h"
#include <vector>
#include "DAnimacion.h"
#include "DFuente.h"

namespace DWL {



	#define DARBOLEX_PADDING				2  // Espacio entre el marco de la selección y el texto (tambien se utiliza para espaciar el expansor, el icono y el texto horizontalmente)
	#define DARBOLEX_MARGEN_Y_SELECCION		1  // Espacio entre la Y inicial / final del texto y el marco de selección
	#define DARBOLEX_TAMEXPANSOR			11 // OJO!! tiene que ser impar para quedar bien
	#define DARBOLEX_TAMICONO				16 // Tamaño del icono


	enum DArbolEx_Nodo_Estado {
		DArbolEx_Nodo_Estado_Normal,
		DArbolEx_Nodo_Estado_Resaltado,
		DArbolEx_Nodo_Estado_Presdionado,
	};

	enum DArbolEx_MostrarExpansor {
		DArbolEx_MostrarExpansor_Auto,
		DArbolEx_MostrarExpansor_MostrarTriangulo,
		DArbolEx_MostrarExpansor_MostrarTrianguloLinea,
		DArbolEx_MostrarExpansor_MostrarRectangulo,
		DArbolEx_MostrarExpansor_Ocultar
	};

	enum DArbolEx_TransicionExpansor {
		DArbolEx_TransicionExpansor_Normal,
		DArbolEx_TransicionExpansor_Resaltado,
		DArbolEx_TransicionExpansor_Presionado
	};

	enum DArbolEx_TransicionNodo {
		DArbolEx_TransicionNodo_Normal,
		DArbolEx_TransicionNodo_Resaltado,
		DArbolEx_TransicionNodo_Seleccionado,
		DArbolEx_TransicionNodo_SeleccionadoResaltado,
		DArbolEx_TransicionNodo_SeleccionadoPresionado,
		DArbolEx_TransicionNodo_SubSeleccionado,
		DArbolEx_TransicionNodo_SubSeleccionadoResaltado,
		DArbolEx_TransicionNodo_SubSeleccionadoPresionado,
		DArbolEx_TransicionNodo_Desactivado,
		DArbolEx_TransicionNodo_DesactivadoResaltado
	};

	class DArbolEx;
	class DExplorarDirectoriosEx;


	// Cuaquier cambio efectuado en esta clase, debe ser actualizado con la función Repintar del ArbolEx.
	class DArbolEx_Nodo {
	  public:
										DArbolEx_Nodo(void);
									   ~DArbolEx_Nodo(void);

		inline const size_t             Ancestros(void)														{ return _Ancestros;					}
		inline void						MostrarExpansor(const DArbolEx_MostrarExpansor nMostrarExpansor)	{ _MostrarExpansor = nMostrarExpansor;	}
		const DArbolEx_MostrarExpansor	MostrarExpansor(void);

		inline DFuente			       &Fuente(void)														{ return _Fuente; }
		inline void						Fuente(DFuente &nFuente)                                            { _Fuente = nFuente;	_AnchoTexto = _Fuente.ObtenerTamTexto(Texto).cx;	}

		inline DArbolEx_Nodo           *Padre(void)															{ return _Padre;						}
		inline DArbolEx_Nodo           *Siguiente(void)														{ return _Siguiente;					}
		inline DArbolEx_Nodo	       *Hijo(const size_t Pos)												{ return _Hijos[Pos];					}
		inline const size_t				TotalHijos(void)													{ return _Hijos.size();					}
		inline DArbolEx_Nodo           *UltimoHijo(void)													{ return (_Hijos.size() == 0) ? NULL : _Hijos[_Hijos.size() - 1]; }

		inline const INT_PTR            IDIcono(void)														{ return _Icono.ID(); }
		void                            Icono(const int nIconoRecursos);
		void                            Icono(const GUID nIconoKnownFolder);
		void                            Icono(const wchar_t *nIconoPath, const int nIconoPos);
		inline DIcono                  &Icono(void) { return _Icono; };
		void                            BorrarHijos(void);

		//const size_t					Posicion(void);

		std::wstring				    Texto;

		inline const BOOL               Activado(void) { return _Activado; }
		void                            Activado(const BOOL nActivar);
//		BOOL                            Activado;			// bits necesaris 1	
		BOOL						    Expandido;			// bits necesaris 1	
		BOOL						    Seleccionado;		// bits necesaris 1
	  protected:
										// Función que termina cualquier animación en curso y asigna los colores del nodo
		void                           _AsignarColores(COLORREF nColTexto, COLORREF nColTextoSombra, COLORREF nColFondo);

		void                           _TransicionResaltado(void);
		void                           _TransicionNormal(void);

		BOOL                           _Activado;
		DArbolEx_MostrarExpansor       _MostrarExpansor;	// bits necesaris 4
		BOOL						   _SubSeleccionado;	// bits necesaris 1

		int                            _AnchoTexto;
		DIcono			               _Icono;


		DArbolEx_Nodo			      *_Padre;
		size_t						   _Ancestros; // Total de padres por encima de este nodo
		DArbolEx_Nodo			      *_Siguiente;
		DArbolEx_Nodo			      *_Anterior;
		std::vector<DArbolEx_Nodo *>   _Hijos;
		DFuente				           _Fuente;
		DArbolEx                      *_Arbol;

		void                           _TransicionExpansor(const DArbolEx_TransicionExpansor nTransicion);
		COLORREF                       _ColorExpansor;
		DAnimacion                     _AniTransicionExpansor;

		void                           _Transicion(const DArbolEx_TransicionNodo nTransicion);
		COLORREF                       _ColorTexto;
		COLORREF                       _ColorTextoSombra;
		COLORREF                       _ColorFondo;
		DAnimacion                     _AniTransicion;
		DArbolEx_TransicionNodo		   _UltimaTransicion;

		friend class DArbolEx;
		friend class DExplorarDirectoriosEx;
	};

}