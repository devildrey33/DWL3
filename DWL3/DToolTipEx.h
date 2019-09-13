#pragma once

#include "DVentana.h"
#include "DControlEx.h"
#include "DFuente.h"

namespace DWL {

	#define DTOOLTIPEX_PADDING 4

	class DToolTipEx_Skin {
	  public:
						DToolTipEx_Skin(void);
						// Colores para el fondo (OJO los colores del fondo y del borde del control están en DBarraSroll_Skin)
		COLORREF		Fondo;
		COLORREF		FondoError;
		COLORREF		Texto;
		COLORREF		TextoSombra;
		COLORREF		Borde;
						// Fuente
		int				FuenteTam;
		std::wstring	FuenteNombre;
		BOOL			FuenteNegrita;
		BOOL			FuenteCursiva;
		BOOL			FuenteSubrayado;
		BOOL			FuenteSombraTexto;
	};


	class DToolTipEx : public DVentana {
	  public:
									DToolTipEx(void);
								   ~DToolTipEx(void);
									// Función que crea la ventana del tooltip para futuras interacciones
		HWND						CrearToolTipEx(DhWnd *nPadre);
		SIZE					    CalcularTam(std::wstring &Str);
		void						Mostrar(const int cX, const int cY, std::wstring &Str);
		void						Mostrar(const int cX, const int cY, std::wstring &Str, const int cAncho, const int cAlto);
		void						Ocultar(void);
		void						OcultarAnimado(void);
		virtual void				Pintar(HDC DC);
		virtual const DhWnd_Tipo	TipoWnd(void) { return DhWnd_Tipo_MenuEx; };
		LRESULT CALLBACK			GestorMensajes(UINT uMsg, WPARAM wParam, LPARAM lParam);
		HWND					    Padre;

		DToolTipEx_Skin				Skin;
	  protected:
		std::wstring	           _Str;
		DFuente                    _Fuente;
	};

}