#pragma once

#include "DControlEx.h"
#include "DFuente.h"

namespace DWL {

	class DEtiquetaEx_Skin {
	  public:
						DEtiquetaEx_Skin(void);
		COLORREF		Fondo;
		COLORREF		Texto;
		COLORREF		TextoSombra;
						// Fuente
		int				FuenteTam;
		std::wstring	FuenteNombre;
		BOOL			FuenteNegrita;
		BOOL			FuenteCursiva;
		BOOL			FuenteSubrayado;
		BOOL			FuenteSombraTexto;
	};

	enum DEtiquetaEx_Alineacion {
		DEtiquetaEx_Alineacion_Izquierda = DT_LEFT,
		DEtiquetaEx_Alineacion_Centrado  = DT_CENTER,
		DEtiquetaEx_Alineacion_Derecha   = DT_RIGHT
	};

	class DEtiquetaEx : public DControlEx {
	  public:

								DEtiquetaEx(void);
				               ~DEtiquetaEx(void);
		HWND					CrearEtiquetaEx(DhWnd *nPadre, const TCHAR *nTxt, const int cX, const int cY, const int cAncho, const int cAlto, const int cID, const DEtiquetaEx_Alineacion nAlineacion = DEtiquetaEx_Alineacion_Izquierda, const long Estilos = WS_CHILD | WS_VISIBLE);
		void					Pintar(HDC DC);
		void					Texto(std::wstring &nTexto);
		LRESULT CALLBACK		GestorMensajes(UINT uMsg, WPARAM wParam, LPARAM lParam);

/*		COLORREF                ColorFondo;
		COLORREF                ColorTexto;
		COLORREF                ColorTextoSombra;*/
	
		DFuente		            Fuente;
		DEtiquetaEx_Skin		Skin;
		DEtiquetaEx_Alineacion  Alineacion;
	  protected:
//		UINT				   _Formato;
		std::wstring           _Texto;
	};

}