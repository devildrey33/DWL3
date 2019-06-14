#pragma once

#include "DControlEx.h"
#include "DFuente.h"

namespace DWL {

	class DEtiquetaEx_Skin {
	  public:
							DEtiquetaEx_Skin(void) { };
		static COLORREF     Fondo;
		static COLORREF     Texto;
		static COLORREF     TextoSombra;
							// Fuente
		static int			FuenteTam;
		static std::wstring	FuenteNombre;
		static BOOL         FuenteNegrita;
		static BOOL         FuenteCursiva;
		static BOOL         FuenteSubrayado;
		static BOOL			FuenteSombraTexto;
	};

	enum DEtiquetaEx_Alineacion {
		DEtiquetaEx_Alineacion_Izquierda,
		DEtiquetaEx_Alineacion_Centrado,
		DEtiquetaEx_Alineacion_Derecha
	};

	class DEtiquetaEx : public DControlEx {
	  public:

								DEtiquetaEx(void);
				               ~DEtiquetaEx(void);
		HWND					CrearEtiquetaEx(DhWnd *nPadre, const TCHAR *nTxt, const int cX, const int cY, const int cAncho, const int cAlto, const int cID, const DEtiquetaEx_Alineacion nAlineacion = DEtiquetaEx_Alineacion_Izquierda, const long Estilos = WS_CHILD | WS_VISIBLE);
		void					Pintar(HDC DC);
		void					Texto(std::wstring &nTexto);
		LRESULT CALLBACK		GestorMensajes(UINT uMsg, WPARAM wParam, LPARAM lParam);

		COLORREF                ColorFondo;
		COLORREF                ColorTexto;
		COLORREF                ColorTextoSombra;
	
		DFuente		            Fuente;
	  protected:
		UINT				   _Formato;
		std::wstring           _Texto;
	};

}