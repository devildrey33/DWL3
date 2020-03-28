#pragma once

#include "DIcono.h"
#include <vector>
#include "DAnimacion.h"
#include "DFuente.h"

namespace DWL {



	#define DARBOLEX_PADDING				2  // Espacio entre el marco de la selecci�n y el texto (tambien se utiliza para espaciar el expansor, el icono y el texto horizontalmente)
	#define DARBOLEX_MARGEN_Y_SELECCION		1  // Espacio entre la Y inicial / final del texto y el marco de selecci�n
	#define DARBOLEX_TAMEXPANSOR			11 // OJO!! tiene que ser impar para quedar bien
	#define DARBOLEX_TAMICONO				16 // Tama�o del icono

	// Estado para el nodo
	enum DArbolEx_Nodo_Estado {
		DArbolEx_Nodo_Estado_Normal,
		DArbolEx_Nodo_Estado_Resaltado,
		DArbolEx_Nodo_Estado_Presdionado,
	};

	// Visibilidad del expansor del nodo
	enum DArbolEx_MostrarExpansor {
		DArbolEx_MostrarExpansor_Auto,					// Solo se muestra si hay nodos hijo
		DArbolEx_MostrarExpansor_MostrarTriangulo,		// Se muestra un triangulo s�lido
		DArbolEx_MostrarExpansor_MostrarTrianguloLinea, // Se muestra el borde de un triangulo
		DArbolEx_MostrarExpansor_MostrarRectangulo,	    // Se muestra un rectangulo con una cruz
		DArbolEx_MostrarExpansor_Ocultar				// No se muestra
	};

	// Estado del expansor del nodo
	enum DArbolEx_TransicionExpansor {
		DArbolEx_TransicionExpansor_Normal,
		DArbolEx_TransicionExpansor_Resaltado,
		DArbolEx_TransicionExpansor_Presionado
	};

	// Transiciones del nodo
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

	// Objeto para controlar un nodo del ArbolEx
	// NOTA : Cuaquier cambio efectuado desde este objeto, debe ser actualizado con la funci�n Repintar del ArbolEx.
	class DArbolEx_Nodo {
	  public: //////////////////////////// Constructor
										DArbolEx_Nodo(void);
										// Destructor
									   ~DArbolEx_Nodo(void);
										// Funci�n que devuelve el numero de ancestros de este nodo (los que est�n por encima : padre, abuelo, etc...)
		inline const size_t             Ancestros(void)														{ return _Ancestros;					}
										// Funci�n para asignar la visibilidad del expansor de este nodo
		inline void						MostrarExpansor(const DArbolEx_MostrarExpansor nMostrarExpansor)	{ _MostrarExpansor = nMostrarExpansor;	}
										// Funci�n que devuelve el estado de visibilidad para este nodo
		const DArbolEx_MostrarExpansor	MostrarExpansor(void);
										// Funci�n que devuelve la fuente para este nodo
		inline DFuente			       &Fuente(void)														{ return _Fuente; }
										// Funci�n que asigna la fuente para este nodo
		inline void						Fuente(DFuente &nFuente)                                            { _Fuente = nFuente;	_AnchoTexto = _Fuente.ObtenerTamTexto(Texto).cx;	}
										// Funci�n que devuelve el nodo padre de este nodo (puede ser NULL)
		inline DArbolEx_Nodo           *Padre(void)															{ return _Padre;						}
										// Funci�n que devuelve el siguiente nodo con el mismo padre (puede ser NULL)
		inline DArbolEx_Nodo           *Siguiente(void)														{ return _Siguiente;					}
										// Funci�n que devuelve el hijo de este nodo especificado por la posici�n (puede ser NULL)
		inline DArbolEx_Nodo	       *Hijo(const size_t Pos)												{ return _Hijos[Pos];					}
										// Funci�n que devuelve el total de hijos que tiene este nodo
		inline const size_t				TotalHijos(void)													{ return _Hijos.size();					}
										// Funci�n que devuelve el ultimo hijo de este nodo (puede ser NULL)
		inline DArbolEx_Nodo           *UltimoHijo(void)													{ return (_Hijos.size() == 0) ? NULL : _Hijos[_Hijos.size() - 1]; }
										// Funci�n que devuelve la ID del icono de este nodo (si es 0, es que no hay icono)
		inline const INT_PTR            IDIcono(void)														{ return _Icono.ID(); }
										// Funci�n que asigna un icono de los recurosos a este nodo
		void                            Icono(const int nIconoRecursos);
										// Funci�n que asigna un icono de windows a este nodo
		void                            Icono(const GUID nIconoKnownFolder);
										// Funci�n que asigna un icono externo a este nodo
		void                            Icono(const wchar_t *nIconoPath, const int nIconoPos);
										// Funci�n que devuelve el objeto DIcono asociado a este nodo
		inline DIcono                  &Icono(void) { return _Icono; };
										// Funci�n que borra todos los hijos de este nodo
		void                            BorrarHijos(void);
										// Texto del nodo
		std::wstring				    Texto;
										// Funci�n que devuelve si este nodo est� activado
		inline const BOOL               Activado(void) { return _Activado; }
										// Funci�n para activar / desactivar este nodo (un nodo desactivado no puede ser expandido ni seleccionado)
		void                            Activado(const BOOL nActivar);
										// Variable que determina si el nodo est� expandido
		BOOL						    Expandido;			// bits necesaris 1	
										// Variable que determina si el nodo est� seleccionado
		BOOL						    Seleccionado;		// bits necesaris 1
	  protected: ///////////////////////// Funci�n que termina cualquier animaci�n en curso y asigna los colores del nodo
		void                           _AsignarColores(COLORREF nColTexto, COLORREF nColTextoSombra, COLORREF nColFondo);
										// Funci�n que realiza la transici�n al estado resaltado
		void                           _TransicionResaltado(void);
										// Funci�n que realiza la transici�n al estado normal
		void                           _TransicionNormal(void);
										// Variable que determina si el nodo est� activado
		BOOL                           _Activado;
										// Variable que determina como hay que mostrar el expansor del nodo
		DArbolEx_MostrarExpansor       _MostrarExpansor;	// bits necesaris 4
										// Variable que determina si el nodo est� sub-seleccionado
		BOOL						   _SubSeleccionado;	// bits necesaris 1
										// Variable con el ancho del texto en pixeles
		int                            _AnchoTexto;
										// Objeto que contiene el icono para este nodo
		DIcono			               _Icono;
										// Nodo padre de este nodo
		DArbolEx_Nodo			      *_Padre;
										// Total de padres por encima de este nodo
		size_t						   _Ancestros; 
										// Nodo inmediatamente siguiente a este nodo (con el mismo padre)
		DArbolEx_Nodo			      *_Siguiente;
										// Nodo inmediatamente anterior a este nodo (con el mismo padre)
		DArbolEx_Nodo			      *_Anterior;
										// Vector de nodos hijo
		std::vector<DArbolEx_Nodo *>   _Hijos;
										// Fuente para este nodo
		DFuente				           _Fuente;
										// ArbolEx al que pertenece este nodo
		DArbolEx                      *_Arbol;
										// Funci�n que realiza las transiciones del expansor
		void                           _TransicionExpansor(const DArbolEx_TransicionExpansor nTransicion);
										// Color actual del expansor
		COLORREF                       _ColorExpansor;
										// Objeto para las animaciones del expansor
		DAnimacion                     _AniTransicionExpansor;
										// Funci�n que realiza las transiciones de este nodo
		void                           _Transicion(const DArbolEx_TransicionNodo nTransicion);
										// Color actual del texto de este nodo
		COLORREF                       _ColorTexto;
										// Color actual de la sombra del texto de este nodo
		COLORREF                       _ColorTextoSombra;
										// Color actual del fondo de este nodo
		COLORREF                       _ColorFondo;
										// Objeto que realiza las animaciones del nodo
		DAnimacion                     _AniTransicion;
										// Variable que conserva la ultima transici�n realizada en este nodo
		DArbolEx_TransicionNodo		   _UltimaTransicion;

		friend class DArbolEx;
		friend class DExplorarDirectoriosEx;
	};

}