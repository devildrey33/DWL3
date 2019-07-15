#pragma once

#include <DVentana.h>
#include <DEdicionTextoEx.h>

class VentanaPrincipal : public DWL::DVentana {
  public:
							VentanaPrincipal(void) : DWL::DVentana() {	}

	void                    Crear(void);

	LRESULT CALLBACK		GestorMensajes(UINT uMsg, WPARAM wParam, LPARAM lParam);

	DWL::DEdicionTextoEx	Edit1;
	DWL::DEdicionTextoEx	Edit2;
	DWL::DEdicionTextoEx	Edit3;
};

