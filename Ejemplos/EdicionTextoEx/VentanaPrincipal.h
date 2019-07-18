#pragma once

#include <DVentana.h>
#include <DEdicionTextoEx.h>


// Ventana principal para el ejemplo
class VentanaPrincipal : public DWL::DVentana {
  public:
							VentanaPrincipal(void) : DWL::DVentana() {	}

	void                    Crear(void);

	LRESULT CALLBACK		GestorMensajes(UINT uMsg, WPARAM wParam, LPARAM lParam);

	DWL::DEdicionTextoEx	EditIzquierda;
	DWL::DEdicionTextoEx	EditCentrado;
	DWL::DEdicionTextoEx	EditDerecha;

	DWL::DEdicionTextoEx	EditEnteros;
	DWL::DEdicionTextoEx	EditDecimales;
	DWL::DEdicionTextoEx	EditTiempo;

};

