#ifndef DBARRATAREAS_H
	#define DBARRATAREAS_H

	#include <Shobjidl.h>
	#include <vector>
	#include <string>
//	#include "DTemporizador.h"

	namespace DWL {
		class DVentana;
		// Estados para los botones de la barra de tareas
		enum DBarraTareas_Estado {
			DBarraTareas_Estado_SinProgreso		= TBPF_NOPROGRESS,
			DBarraTareas_Estado_Indeterminado	= TBPF_INDETERMINATE,
			DBarraTareas_Estado_Normal			= TBPF_NORMAL,
			DBarraTareas_Estado_Pausado			= TBPF_PAUSED,
			DBarraTareas_Estado_Error			= TBPF_ERROR
		};


		//! Clase que contiene un bot�n de la TaskBar antes de su creaci�n
		class DBarraTareas_Boton {
			public : ///////// Constructor por defecto
							DBarraTareas_Boton(void) : IDIcono(0), ID(0) { };
							// Constructor que crea un boton con los par�metros especificados
							DBarraTareas_Boton(const UINT nIDIcono, const wchar_t *nTextoToolTip, const UINT nID) : IDIcono(nIDIcono), TextoToolTip(nTextoToolTip), ID(nID) { };
							// Constructor copia
							DBarraTareas_Boton(const DBarraTareas_Boton &nBoton) : IDIcono(nBoton.IDIcono), TextoToolTip(nBoton.TextoToolTip), ID(nBoton.ID) { };
							// Destructor
						   ~DBarraTareas_Boton(void) { };
						    // ID del icono
			INT_PTR			IDIcono;
							// Texto para el tooltip
			std::wstring	TextoToolTip;
							// ID del bot�n
			INT_PTR			ID;
		};


		class DBarraTareas {
		  public: //////////////////////////////// Constructor
												DBarraTareas(void);
												// Destructor
								               ~DBarraTareas(void);
												// Resalta la ventana y el bot�n de la barra de tareas
//			void								Resaltar(void);
												// No se mostrar� ninguna barra de progreso
			void								Estado_SinProgreso(void);
												// Se mostrar� un progreso indeterminado (haciendo cliclos de izquierda a derecha del tama�o del bot�n de la taskbar)
			void								Estado_Indeterminado(void);
												// Se mostrar� una barra de progreso normal
			void								Estado_Normal(void);
												// Se mostrar� una barra de progreso con error (de color rojo)
			void								Estado_Error(void);
												// Se mostrar� una barra de progreso en pausa (de color amarillo)
			void								Estado_Pausado(void);
												// Asigna un valor a la barra de progreso
			void								Valor(const UINT nValor, const UINT nMaximo);
												// Devuelve el estado de la barra de progreso
			inline const DBarraTareas_Estado	Estado(void) { return _Estado; }
												// Establece el �rea que se mostrara de la ventana en el Thumb
			const BOOL                          Clip(RECT *Recta);
												// Agrega botones al Thumb
			const BOOL                          AgregarBotones(std::vector<DBarraTareas_Boton> &Botones);
												// Funci�n que asigna un icono a un boton de la barra de tareas
			void                                Boton_Icono(const INT_PTR Id, const INT_PTR IDIconoRecursos);
												// Funci�n que devuelve el mensaje que indica que se ha creado el boton
			inline const UINT					WM_TASK_BUTTON_CREATED(void) { return _WM_TASK_BUTTON_CREATED; };
		protected: /////////////////////////////// Funci�n protegida que inicia este objeto en la ventana especificada
			void                               _Iniciar(HWND hWndVentana);
												// Funci�n que inicia la instancia para controlar la barra de tareas
			const BOOL						   _IniciarITaskBar(void);
												// HWND de la ventana a la que pertenece este objeto
			HWND                               _hWnd;
												// Estado de la barra de tareas
			DBarraTareas_Estado                _Estado;
												// Instancia estatica para controlar la barra de tareas
			static ITaskbarList4              *_BarraTareas;
												// Mensaje que indica la creaci�n de un boton en la barra de tareas
			static UINT                        _WM_TASK_BUTTON_CREATED;

			// Esta clase es amiga de DVentana
			friend class DVentana;
		};
	};

#endif