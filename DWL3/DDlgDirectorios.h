#ifndef DDLGDIRECTORIOS_H
#define DDLGDIRECTORIOS_H

#include <string>

namespace DWL {

	class DDlgDirectorios {
	  public:
									DDlgDirectorios(void);
		                           ~DDlgDirectorios(void);

		const BOOL					Mostrar(const wchar_t *Titulo, const wchar_t *PathInicial, std::wstring &PathFinal);

	  protected :
		BOOL                       _Terminado;
		DhWnd                     *_Padre;
	};

};

#endif