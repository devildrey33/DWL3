#ifndef DPORTAPAPELES_H
#define DPORTAPAPELES_H
#include <string>

namespace DWL {
	// Objeto estático para acceder al portapapeles de windows
	class DPortapapeles {
	  public:
					// Función para obtener el texto del portapapeles
		static void ObtenerTexto(HWND hWndPaste, std::wstring &OUT_Texto);
					// Función para asignar texto al portapapeles utilizando el wchar_t especificado
		static void AsignarTexto(const wchar_t *Texto);
					// Función para asignar texto al portapapeles utilizando el std::wstring especificado.
		static void AsignarTexto(std::wstring &Texto);
	  private :		// Constructor privado
					DPortapapeles(void) { };
	};

};

#endif