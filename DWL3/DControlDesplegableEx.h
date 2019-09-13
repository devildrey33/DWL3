
#ifndef DCONTROLDESPLEGABLEEX_H
#define DCONTROLDESPLEGABLEEX_H

#include "DBotonEx.h"
#include "DEdicionTextoEx.h"

namespace DWL {

	

	class DBotonDesplegable : public DBotonEx {
	  public :
				DBotonDesplegable(void) : DBotonEx() { };
		       ~DBotonDesplegable(void) { };
		void	Evento_Pintar(HDC DC);
	};

	// Base para los controles desplegables (viene a ser un control con un icono, un texto y un boton)
	class DControlDesplegableEx : public DControlEx {
	  public:
									DControlDesplegableEx(void);
		                           ~DControlDesplegableEx(void);
		virtual void				Evento_Desplegar(void) { };
		virtual LRESULT CALLBACK	GestorMensajes(UINT uMsg, WPARAM wParam, LPARAM lParam);
//		void						Pintar(HDC DC);
		void						CrearControlDesplegable(DhWnd *nPadre, const wchar_t *nTexto, const INT_PTR nID, const INT_PTR nIDIcono, const int cX, const int cY, const int cAncho, const int cAlto, const DEdicionTextoEx_Entrada Entrada);
//		inline const wchar_t       *Texto(void)						{ return _Edicion.Texto();	};
		const std::wstring		   &Texto(void)                     { return _Edicion.Texto(); }
		inline void                 Texto(std::wstring &nTexto)		{ _Edicion.Texto(nTexto);	};
		inline void                 Texto(const wchar_t *nTexto)	{ _Edicion.Texto(nTexto);	};

		virtual const DhWnd_Tipo	TipoWnd(void) { return DhWnd_Tipo_DesplegableEx; };
      protected:
		DBotonDesplegable          _Boton;
		DEdicionTextoEx            _Edicion;
		DIcono                     _Icono;
	};
};
#endif