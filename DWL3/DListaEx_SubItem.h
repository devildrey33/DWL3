#ifndef DLISTAEX_SUBITEM_H
#define DLISTAEX_SUBITEM_H

#include <string>

namespace DWL {
	class DListaEx_SubItem	{
	  public:
						DListaEx_SubItem(void) { };
						DListaEx_SubItem(const TCHAR *nTexto)  { Texto = nTexto; };
//						DListaEx_SubItem(std::wstring &nTexto) { Texto = nTexto; };
		               ~DListaEx_SubItem(void) { };
		std::wstring	Texto;
	};
}
#endif