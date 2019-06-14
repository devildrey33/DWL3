#pragma once

namespace DWL {

	class DGDI {
	  public:
					DGDI() {};
			       ~DGDI() { };

		static void PintarFlecha(HDC hDC, const int cX, const int cY, const float nAngulo, const int cTamRadio, const int cTamPluma, COLORREF cColor);
	};

}