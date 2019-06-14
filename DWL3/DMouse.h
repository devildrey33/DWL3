#ifndef DMOUSE_H
    #define DMOUSE_H
    
//    #include "DObjeto.h"

    namespace DWL {

        enum DMouse_Cursor {
	        DMouse_Cursor_FlechaReloj		= 32650,	// IDC_APPSTARTING
	        DMouse_Cursor_FlechaVertical	= 32516,	// IDC_UPARROW
			DMouse_Cursor_Flecha			= 32512,	// IDC_ARROW
			DMouse_Cursor_Cruz				= 32515,    // IDC_CROSS
			DMouse_Cursor_Mano				= 32649,	// IDC_HAND
			DMouse_Cursor_Ayuda				= 32651,	// IDC_HELP
			DMouse_Cursor_IBeam				= 32513,	// IDC_IBEAM
			DMouse_Cursor_Cancelar			= 32648,	// IDC_NO
			DMouse_Cursor_Tam				= 32646,	// IDC_SIZEALL
			DMouse_Cursor_TamNESO			= 32643,	// IDC_SIZENESW
			DMouse_Cursor_TamNS				= 32645,	// IDC_SIZENS
			DMouse_Cursor_TamNOSE			= 32642,	// IDC_SIZENWSE
			DMouse_Cursor_TamEO				= 32644,	// IDC_SIZEWE
			DMouse_Cursor_Procesando		= 32514     // IDC_WAIT
        }; 

		class DMouse {
          public:
											DMouse(void) { };

			static HCURSOR					CambiarCursor(const DMouse_Cursor nCursor = DMouse_Cursor_Flecha);

			static HCURSOR					CambiarCursor(HCURSOR nCursor);

			static int						Visible(const BOOL nVisible);

			static BOOL						ObtenerPosicion(POINT *Pos);

			static BOOL						AsignarPosicion(const int cX, const int cY);

			static BOOL						AsignarPosicion(const POINT *nPos);

			static BOOL						AreaRaton(const RECT *nArea);
        };
    };

#endif