#pragma once

#include <DVentana.h>
#include <DListaEx.h>


// Ventana principal para el ejemplo
class VentanaPrincipal : public DWL::DVentana {
  public:
							VentanaPrincipal(void) : DWL::DVentana() {	}

	void                    Crear(void);

	LRESULT CALLBACK		GestorMensajes(UINT uMsg, WPARAM wParam, LPARAM lParam);

	DWL::DListaEx			Lista;
};

