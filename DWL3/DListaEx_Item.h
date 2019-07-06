#ifndef DLISTAEX_ITEM
#define DLISTAEX_ITEM

#include "DListaEx_SubItem.h"
#include "DIcono.h"
#include "DAnimacion.h"

namespace DWL {

	#define DLISTAEX_PADDING				(int)2 // Espacio entre el marco de la selección y el texto (tambien se utiliza para espaciar el expansor, el icono y el texto horizontalmente)
	#define DLISTAEX_MARGEN_Y_SELECCION		(int)1 // Espacio entre la Y inicial / final del texto y el marco de seleccion
	#define DLISTAEX_TAMICONO				(int)16 // Tamaño del icono

	class DListaEx;

	enum DListaEx_TransicionItem {
		DListaEx_TransicionItem_Normal,
		DListaEx_TransicionItem_Resaltado,
		DListaEx_TransicionItem_Seleccionado,
		DListaEx_TransicionItem_SeleccionadoResaltado,
		DListaEx_TransicionItem_Presionado
	};

	/* Clase que engloba un item de la lista (puede tener varias columnas) 
		NOTA : Cualquier cambio que se realize en un DListaEx_Item necesitara una llamada a la función Repintar() o a la función PintarItem() para actualizar los cambios */
	class DListaEx_Item {
	  public:
											DListaEx_Item(void);
										   ~DListaEx_Item(void);
										    // Función que devuelve una referencia al std::wstring que contiene el texto del subitem especificado 
		inline std::wstring                &Texto(const size_t nSubItem = 0) {
												return _SubItems[nSubItem]->Texto;
											};		
										    // Función que devuelve una referencia al std::wstring que contiene el texto del subitem especificado 
		inline std::wstring                &Texto(const size_t nSubItem, std::wstring &nTexto) {
												_SubItems[nSubItem]->Texto = nTexto;
												return _SubItems[nSubItem]->Texto;
											};		

		void                                Icono(const int nIconoRecursos);

		BOOL							    Seleccionado;
	  protected : 
		void                               _TransicionNormal(void);
		void                               _TransicionResaltado(void);
		std::vector<DListaEx_SubItem *>    _SubItems;
		DIcono			                   _Icono;

		void                               _Transicion(const DListaEx_TransicionItem nTransicion);

		COLORREF                           _ColorTexto;
		COLORREF                           _ColorTextoSombra;
		COLORREF                           _ColorFondo;
		DAnimacion                         _AniTransicion;

		DListaEx                          *_Lista;
		DListaEx_TransicionItem		       _UltimaTransicion;
		friend class DListaEx;
	};
	
}
#endif

