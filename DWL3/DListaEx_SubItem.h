#ifndef DLISTAEX_SUBITEM_H
#define DLISTAEX_SUBITEM_H

#include <string>

namespace DWL {
	class DListaEx_SubItem	{
	  public: //////////// Constructor vacio
						DListaEx_SubItem(void)					{ };
						// Constructor asignador de texto
						DListaEx_SubItem(const TCHAR *nTexto)	{ Texto = nTexto; };
						// Destructor
		               ~DListaEx_SubItem(void)					{ };
					    // Texto para el subitem
		std::wstring	Texto;
	};
}
#endif