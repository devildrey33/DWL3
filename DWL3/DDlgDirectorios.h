#ifndef DDLGDIRECTORIOS_H
#define DDLGDIRECTORIOS_H

//#include "DhWnd.h"
#include "DBotonEx.h"
#include "DExplorarDirectoriosEx.h"
#include "DEdicionTextoEx.h"
#include "DVentana.h"

namespace DWL {

	class DDlgDirectorios : public DWL::DVentana {
	  public:
									DDlgDirectorios(void);
		                           ~DDlgDirectorios(void);

		const BOOL					Mostrar(DhWnd *nPadre, std::wstring &Path, const int cX, const int cY, const int cAncho, const int cAlto, const int ID_Icono);

									// Controles
		DExplorarDirectoriosEx      ArbolDirectorios;
		DEdicionTextoEx				EdicionSeleccion;
		DBotonEx                    BotonAceptar;
		DBotonEx                    BotonCancelar;

		void                        Evento_Cerrar(void);
		void						Evento_BotonEx_Mouse_Click(DWL::DEventoMouse &DatosMouse);
		void						Evento_BorraFondo(HDC DC);
		void                        Evento_CambiandoTam(const UINT Lado, RECT* Rectangulo);	
		virtual LRESULT CALLBACK	GestorMensajes(UINT uMsg, WPARAM wParam, LPARAM lParam);
	  protected :
		void                       _AjustarControles(RECT &RW);
		void                       _ActualizarAceptar(void);
		BOOL                       _Terminado;
		DhWnd                     *_Padre;
	};

};

#endif