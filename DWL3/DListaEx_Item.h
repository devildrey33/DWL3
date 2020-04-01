#ifndef DLISTAEX_ITEM
#define DLISTAEX_ITEM

#include "DListaEx_SubItem.h"
#include "DIcono.h"
#include "DAnimacion.h"

namespace DWL {

	#define DLISTAEX_PADDING				 2 // Espacio entre el marco de la selecci�n y el texto (tambien se utiliza para espaciar el expansor, el icono y el texto horizontalmente)
	#define DLISTAEX_MARGEN_Y_SELECCION		 1 // Espacio entre la Y inicial / final del texto y el marco de seleccion
	#define DLISTAEX_TAMICONO				16 // Tama�o del icono

	class DListaEx;

	// Transiciones posibles para un item de la lista extendida
	enum DListaEx_TransicionItem {
		DListaEx_TransicionItem_Normal,
		DListaEx_TransicionItem_Resaltado,
		DListaEx_TransicionItem_Seleccionado,
		DListaEx_TransicionItem_SeleccionadoResaltado,
		DListaEx_TransicionItem_Presionado
	};

	/* Clase que engloba un item de la lista (puede tener varias columnas) 
		NOTA : Cualquier cambio que se realize en un DListaEx_Item necesitara una llamada a la funci�n Repintar() o a la funci�n PintarItem() para actualizar los cambios */
	class DListaEx_Item {
	  public: //////////////////////////////// Constructor
											DListaEx_Item(void);
											// Destructor
										   ~DListaEx_Item(void);
										    // Funci�n que devuelve una referencia al std::wstring que contiene el texto del subitem especificado 
		inline std::wstring                &Texto(const size_t nSubItem = 0) {
												return _SubItems[nSubItem]->Texto;
											};		
										    // Funci�n que devuelve una referencia al std::wstring que contiene el texto del subitem especificado 
		inline std::wstring                &Texto(const size_t nSubItem, std::wstring &nTexto) {
												_SubItems[nSubItem]->Texto = nTexto;
												return _SubItems[nSubItem]->Texto;
											};		
											// Funci�n para asignar el icono del item (tiene que ser un icono de los recursos)
		void                                Icono(const int nIconoRecursos);
											// Miembro que determina si el item est� seleccionado
		BOOL							    Seleccionado;
	  protected : //////////////////////////// Funci�n protegida que ejecuta la transici�n al estado normal											
		void                               _TransicionNormal(void);
											// Funci�n protegida que ejecuta la transici�n al estado resaltado
		void                               _TransicionResaltado(void);
											// Vector de sub items 
		std::vector<DListaEx_SubItem *>    _SubItems;
											// Icono de la lista
		DIcono			                   _Icono;
											// Funci�n protegida para ejecutar las transiciones
		void                               _Transicion(const DListaEx_TransicionItem nTransicion);
											// Color de texto actual para el item
		COLORREF                           _ColorTexto;
											// Color de la sombra del texto actual para el item
		COLORREF                           _ColorTextoSombra;
											// Color del fondo actual para el item
		COLORREF                           _ColorFondo;
											// Objeto para realizar las transiciones de color
		DAnimacion                         _AniTransicion;
											// ListaEx padre
		DListaEx                          *_Lista;
											// Variable que guarda la ultima transici�n realizada
		DListaEx_TransicionItem		       _UltimaTransicion;
		friend class DListaEx;
	};
	
}
#endif

