#include "pch.h"
#include "DGDI.h"
#include <math.h>

namespace DWL {

	void DGDI::PintarFlecha(HDC hDC, const int cX, const int cY, const float nAngulo, const int cTamRadio, const int cTamPluma, COLORREF cColor) {
		HPEN Pluma = CreatePen(PS_SOLID, cTamPluma, cColor);
		HPEN PlumaVieja = static_cast<HPEN>(SelectObject(hDC, Pluma));
		
		const float PI = 3.14159265359f;
		const float RAD = PI / 180;
		
		/*
		int x1 = static_cast<int>(round(cos((nAngulo - 90.0f) * RAD) * (cTamRadio / 2)));
		int y1 = static_cast<int>(round(sin((nAngulo - 90.0f) * RAD) * (cTamRadio / 2)));
		int x2 = static_cast<int>(round(cos((nAngulo) * RAD) * cTamRadio));
		int y2 = static_cast<int>(round(sin((nAngulo) * RAD) * cTamRadio));
		int x3 = static_cast<int>(round(cos((nAngulo + 90.0f) * RAD) * (cTamRadio / 2)));
		int y3 = static_cast<int>(round(sin((nAngulo + 90.0f) * RAD) * (cTamRadio / 2)));

		int x0 = cX + cTamRadio;
		int y0 = cY + cTamRadio;

		MoveToEx(hDC, x0 + x1, y0 + y1, NULL);
		LineTo(hDC, x0 + x2, y0 + y2);
		LineTo(hDC, x0 + x3, y0 + y3);*/
		POINT Pt[3];
/*		Pt[0].x = cX + ((cos(nAngulo - 120.0f) * RAD) * cTamRadio);
		Pt[0].y = cY + ((sin(nAngulo - 120.0f) * RAD) * cTamRadio);
		Pt[1].x = cX + ((cos(nAngulo) * RAD) * cTamRadio);
		Pt[1].y = cY + ((sin(nAngulo) * RAD) * cTamRadio);
		Pt[2].x = cX + ((cos(nAngulo + 120.0f) * RAD) * cTamRadio);
		Pt[2].y = cY + ((sin(nAngulo + 120.0f) * RAD) * cTamRadio);*/

		Pt[0].x = (cX + cTamRadio) + static_cast<int>((cos((nAngulo - 90.0f) * RAD) * (cTamRadio / 1.5f)));
		Pt[0].y = (cY + cTamRadio) + static_cast<int>((sin((nAngulo - 90.0f) * RAD) * (cTamRadio / 1.5f)));
		Pt[1].x = (cX + cTamRadio) + static_cast<int>((cos((nAngulo) * RAD) * cTamRadio));
		Pt[1].y = (cY + cTamRadio) + static_cast<int>((sin((nAngulo) * RAD) * cTamRadio));
		Pt[2].x = (cX + cTamRadio) + static_cast<int>((cos((nAngulo + 90.0f) * RAD) * (cTamRadio / 1.5f)));
		Pt[2].y = (cY + cTamRadio) + static_cast<int>((sin((nAngulo + 90.0f) * RAD) * (cTamRadio / 1.5f)));

		Polygon(hDC, Pt, 3);

		SelectObject(hDC, PlumaVieja);
		DeleteObject(Pluma);
	}

}