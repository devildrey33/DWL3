#include "pch.h"
#include "DMouse.h"

namespace DWL {

	HCURSOR	DMouse::CambiarCursor(const DMouse_Cursor nCursor) {
		HCURSOR Cur = LoadCursor(NULL, MAKEINTRESOURCE(nCursor));
		return SetCursor(Cur);
	};

	HCURSOR	DMouse::CambiarCursor(HCURSOR nCursor) {
		return SetCursor(nCursor);
	};

	int DMouse::Visible(const BOOL nVisible) {
		if (nVisible == false) {
			while (ShowCursor(nVisible) > -1);
		}
		else {
			while (ShowCursor(nVisible) < 0);
		}
		return 0;
	};

	BOOL DMouse::ObtenerPosicion(POINT *Pos) {
		return GetCursorPos(Pos);
	};

	BOOL DMouse::AsignarPosicion(const int cX, const int cY) {
		return SetCursorPos(cX, cY);
	};

	BOOL DMouse::AsignarPosicion(const POINT *nPos) {
		return SetCursorPos(nPos->x, nPos->y);
	};

	BOOL DMouse::AreaRaton(const RECT *nArea) {
		return ClipCursor(nArea);
	};

};