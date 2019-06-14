#ifndef DBARRATAREAS_H
	#define DBARRATAREAS_H

	#include <Shobjidl.h>
	#include <vector>
	#include <string>
//	#include "DTemporizador.h"

	namespace DWL {
		class DVentana;

		enum DBarraTareas_Estado {
			DBarraTareas_Estado_SinProgreso		= TBPF_NOPROGRESS,
			DBarraTareas_Estado_Indeterminado	= TBPF_INDETERMINATE,
			DBarraTareas_Estado_Normal			= TBPF_NORMAL,
			DBarraTareas_Estado_Pausado			= TBPF_PAUSED,
			DBarraTareas_Estado_Error			= TBPF_ERROR
		};


		//! Clase que contiene un botón de la TaskBar antes de su creación
		class DBarraTareas_Boton {
			public :
							DBarraTareas_Boton(void) : IDIcono(0), ID(0) { };
							DBarraTareas_Boton(const UINT nIDIcono, const wchar_t *nTextoToolTip, const UINT nID) : IDIcono(nIDIcono), TextoToolTip(nTextoToolTip), ID(nID) { };
							DBarraTareas_Boton(const DBarraTareas_Boton &nBoton) : IDIcono(nBoton.IDIcono), TextoToolTip(nBoton.TextoToolTip), ID(nBoton.ID) { };
						   ~DBarraTareas_Boton(void) { };
			INT_PTR			IDIcono;
			std::wstring	TextoToolTip;
			INT_PTR			ID;
		};


		class DBarraTareas {
		  public:
												DBarraTareas(void);
								               ~DBarraTareas(void);
												// Resalta la ventana y el botón de la barra de tareas
//			void								Resaltar(void);
												// No se mostrará ninguna barra de progreso
			void								Estado_SinProgreso(void);
												// Se mostrará un progreso indeterminado (haciendo cliclos de izquierda a derecha del tamaño del botón de la taskbar)
			void								Estado_Indeterminado(void);
												// Se mostrará una barra de progreso normal
			void								Estado_Normal(void);
												// Se mostrará una barra de progreso con error (de color rojo)
			void								Estado_Error(void);
												// Se mostrará una barra de progreso en pausa (de color amarillo)
			void								Estado_Pausado(void);
												// Asigna un valor a la barra de progreso
			void								Valor(const UINT nValor, const UINT nMaximo);
												// Devuelve el estado de la barra de progreso
			inline const DBarraTareas_Estado	Estado(void) { return _Estado; }
												// Establece el área que se mostrara de la ventana en el Thumb
			const BOOL                          Clip(RECT *Recta);
												// Agrega botones al Thumb
			const BOOL                          AgregarBotones(std::vector<DBarraTareas_Boton> &Botones);
			void                                Boton_Icono(const INT_PTR Id, const INT_PTR IDIconoRecursos);

			inline const UINT					WM_TASK_BUTTON_CREATED(void) { return _WM_TASK_BUTTON_CREATED; };

		protected:

//			void								Evento_Temporizador(const UINT cID);

			void                               _Iniciar(HWND hWndVentana);
			const BOOL						   _IniciarITaskBar(void);
			HWND                               _hWnd;
			DBarraTareas_Estado                _Estado;

			static ITaskbarList4              *_BarraTareas;
			static UINT                        _WM_TASK_BUTTON_CREATED;

			// Esta clase es amiga de DVentana
			friend class DVentana;
		};
	};

#endif