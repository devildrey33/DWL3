#include "pch.h"
#include "DControlDesplegableEx.h"
#include "DGDI.h"

#define TAM_ICONO     16

#define ID_BOTON	1000
#define ID_EDICION	1001

namespace DWL {

/*
	   _____            _             _ _____                  _                  _     _
	  / ____|          | |           | |  __ \                | |                | |   | |
	 | |     ___  _ __ | |_ _ __ ___ | | |  | | ___  ___ _ __ | | ___  __ _  __ _| |__ | | ___
	 | |    / _ \| '_ \| __| '__/ _ \| | |  | |/ _ \/ __| '_ \| |/ _ \/ _` |/ _` | '_ \| |/ _ \
	 | |___| (_) | | | | |_| | | (_) | | |__| |  __/\__ \ |_) | |  __/ (_| | (_| | |_) | |  __/
	  \_____\___/|_| |_|\__|_|  \___/|_|_____/ \___||___/ .__/|_|\___|\__, |\__,_|_.__/|_|\___|
														| |            __/ |
														|_|           |___/
*/


	DControlDesplegableEx::DControlDesplegableEx(void)  {
	}


	DControlDesplegableEx::~DControlDesplegableEx(void) {
	}


	void DControlDesplegableEx::CrearControlDesplegable(DhWnd *nPadre, const wchar_t *nTexto, const INT_PTR nID, const INT_PTR nIDIcono, const int cX, const int cY, const int cAncho, const int cAlto, const DEdicionTextoEx_Entrada Entrada) {
		CrearControlEx(nPadre, L"DControlDesplegable", nTexto, nID, cX, cY, cAncho, cAlto, WS_CHILD | WS_VISIBLE, NULL);
		_Boton.CrearBotonEx(this, L"", cAncho - cAlto, 0, cAlto, cAlto, ID_BOTON);
		_Icono.CrearIconoRecursos(nIDIcono, TAM_ICONO, TAM_ICONO);
		_Edicion.Entrada = Entrada;
		_Edicion.Alineacion = DEdicionTextoEx_Alineacion_Izquierda;
		_Edicion.CrearEdicionTextoEx(this, nTexto, 0, 0, cAncho - cAlto, cAlto, ID_EDICION, &_Icono);
	}


	LRESULT CALLBACK DControlDesplegableEx::GestorMensajes(UINT uMsg, WPARAM wParam, LPARAM lParam) {
		switch (uMsg) {
			case DWL_BOTONEX_CLICK:
			case DWL_EDICIONTEXTOEX_CLICK:
				Evento_Desplegar();
				return 0;
		}
		return DControlEx::GestorMensajes(uMsg, wParam, lParam);
	}



















/*
	  ____        _              _____                  _                  _     _
	 |  _ \      | |            |  __ \                | |                | |   | |
	 | |_) | ___ | |_ ___  _ __ | |  | | ___  ___ _ __ | | ___  __ _  __ _| |__ | | ___
	 |  _ < / _ \| __/ _ \| '_ \| |  | |/ _ \/ __| '_ \| |/ _ \/ _` |/ _` | '_ \| |/ _ \
	 | |_) | (_) | || (_) | | | | |__| |  __/\__ \ |_) | |  __/ (_| | (_| | |_) | |  __/
	 |____/ \___/ \__\___/|_| |_|_____/ \___||___/ .__/|_|\___|\__, |\__,_|_.__/|_|\___|
												 | |            __/ |
												 |_|           |___/                    	
*/



	void DBotonDesplegable::Evento_Pintar(HDC DC) {
		RECT RC;
		GetClientRect(_hWnd, &RC);
//		int nAncho = (RC.right - RC.bottom);
		int nAncho = (RC.right - 16) / 2;
		int nAlto = (RC.bottom - 16) / 2;
		DGDI::PintarFlecha(DC, nAncho, -nAlto, 90.0f, 8, 1, _ColorTexto);
	}

}