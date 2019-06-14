#include "pch.h"
#include "DBarraTareas.h"
#include "DIcono.h"

#define DBARRATAREAS_MOSTRARDEBUG FALSE
#define DBARRATAREAS_TEMPORIZADOR 10

namespace DWL {
	// Miembros estaticos
	UINT			DBarraTareas::_WM_TASK_BUTTON_CREATED	= 0;
	ITaskbarList4  *DBarraTareas::_BarraTareas				= NULL;

	DBarraTareas::DBarraTareas(void) : _hWnd(NULL), _Estado(DBarraTareas_Estado_SinProgreso) /*, _BarraTareas(NULL)*/ {
	}

	DBarraTareas::~DBarraTareas(void) {
	}
	/*
	void DBarraTareas::Resaltar(void) {
		FlashWindow(_hWnd, TRUE);
		CrearTemporizador(DBARRATAREAS_TEMPORIZADOR, 1000, TRUE);
	}

	void DBarraTareas::Evento_Temporizador(const UINT cID) {
		if (cID == DBARRATAREAS_TEMPORIZADOR) FlashWindow(_hWnd, TRUE);
	}*/


	// No se mostrará ninguna barra de progreso
	void DBarraTareas::Estado_SinProgreso(void) {		
		if (_IniciarITaskBar() == TRUE) 
			_BarraTareas->SetProgressState(_hWnd, TBPF_NOPROGRESS);
		_Estado = DBarraTareas_Estado_SinProgreso;
		#if DBARRATAREAS_MOSTRARDEBUG == TRUE
			Debug_Escribir(L"DBarraTareas::Estado_SinProgreso\n");
		#endif
	}

	// Se mostrará un progreso indeterminado (haciendo cliclos de izquierda a derecha del tamaño del botón de la taskbar)
	void DBarraTareas::Estado_Indeterminado(void) {
		if (_IniciarITaskBar() == TRUE) 
			_BarraTareas->SetProgressState(_hWnd, TBPF_INDETERMINATE);
		_Estado = DBarraTareas_Estado_Indeterminado;
		#if DBARRATAREAS_MOSTRARDEBUG == TRUE
			Debug_Escribir(L"DBarraTareas::Estado_Indeterminado\n");
		#endif
	}

	// Se mostrará una barra de progreso normal (de color verde)
	void DBarraTareas::Estado_Normal(void) {
		if (_IniciarITaskBar() == TRUE) 
			_BarraTareas->SetProgressState(_hWnd, TBPF_NORMAL);
		_Estado = DBarraTareas_Estado_Normal;
		#if DBARRATAREAS_MOSTRARDEBUG == TRUE
			Debug_Escribir(L"DBarraTareas::Estado_Normal\n");
		#endif
	}

	// Se mostrará una barra de progreso con error (de color rojo)
	void DBarraTareas::Estado_Error(void) {
		if (_IniciarITaskBar() == TRUE)
			_BarraTareas->SetProgressState(_hWnd, TBPF_ERROR);
		_Estado = DBarraTareas_Estado_Error;
		#if DBARRATAREAS_MOSTRARDEBUG == TRUE
			Debug_Escribir(L"DBarraTareas::Estado_Error\n");
		#endif
	}

	// Se mostrará una barra de progreso en pausa (de color amarillo)
	void DBarraTareas::Estado_Pausado(void) {
		if (_IniciarITaskBar() == TRUE) _BarraTareas->SetProgressState(_hWnd, TBPF_PAUSED);
		_Estado = DBarraTareas_Estado_Pausado;
		#if DBARRATAREAS_MOSTRARDEBUG == TRUE
			Debug_Escribir(L"DBarraTareas::Estado_Pausado\n");
		#endif
	}

	// Asigna un valor a la barra de progreso
	void DBarraTareas::Valor(const UINT nValor, const UINT nMaximo) {
		if (_IniciarITaskBar() == TRUE) _BarraTareas->SetProgressValue(_hWnd, nValor, nMaximo);
	}


	// Asocia este objeto con su ventana/objeto padre
	void DBarraTareas::_Iniciar(HWND hWndVentana) {
		_hWnd = hWndVentana;
	}


	const BOOL DBarraTareas::Clip(RECT *Recta) {
//		RECT RC;
//		if (Recta == NULL) GetClientRect(App.VentanaRave.hWnd(), &RC);
//		else               RC = *Recta;
		HRESULT Ret = _BarraTareas->SetThumbnailClip(_hWnd, Recta);
		return (Ret == S_OK) ? TRUE : FALSE;
	}


	const BOOL DBarraTareas::AgregarBotones(std::vector<DBarraTareas_Boton> &Botones) {
		if (_IniciarITaskBar() == TRUE && Botones.size() > 0) {
//			int o = GetSystemMetrics(SM_CXICON);
			THUMBBUTTON *nBotones = new THUMBBUTTON[Botones.size()];
			for (size_t i = 0; i < Botones.size(); i++) {
				DIcono Ico;
				Ico.CrearIconoRecursos(Botones[i].IDIcono, GetSystemMetrics(SM_CXICON), GetSystemMetrics(SM_CYICON));
				nBotones[i].dwMask	= THB_ICON | THB_TOOLTIP | THB_FLAGS;
				nBotones[i].iId		= static_cast<UINT>(Botones[i].ID);
				nBotones[i].hIcon	= Ico();
				nBotones[i].dwFlags = THBF_ENABLED;
				wcscpy_s(nBotones[i].szTip, 260, Botones[i].TextoToolTip.c_str());				
			}
			HRESULT Ret = _BarraTareas->ThumbBarAddButtons(_hWnd, static_cast<UINT>(Botones.size()), nBotones);
			delete [] nBotones;
			return (Ret == S_OK) ? TRUE : FALSE;
		}
		return FALSE;
	}

	void DBarraTareas::Boton_Icono(const INT_PTR nID, const INT_PTR IDIconoRecursos) {
		if (_BarraTareas != NULL) {
			DIcono Ico;
			Ico.CrearIconoRecursos(IDIconoRecursos, GetSystemMetrics(SM_CXICON), GetSystemMetrics(SM_CYICON));
			THUMBBUTTON Boton;
			Boton.dwMask	= THB_ICON;
			Boton.iId		= static_cast<UINT>(nID);
			Boton.hIcon		= Ico();
			HRESULT Ret		= _BarraTareas->ThumbBarUpdateButtons(_hWnd, 1, &Boton);
		}
	}

	const BOOL DBarraTareas::_IniciarITaskBar(void) {
		if (_BarraTareas == NULL) {
			// Inicio la instancia para controlar la barra de tareas
			if (CoCreateInstance(CLSID_TaskbarList, NULL, CLSCTX_INPROC_SERVER, IID_ITaskbarList4, (void**)&_BarraTareas) == S_OK) {
				_BarraTareas->HrInit();
				_WM_TASK_BUTTON_CREATED = RegisterWindowMessage(L"TaskbarButtonCreated");
				#if DBARRATAREAS_MOSTRARDEBUG == TRUE
					Debug_Escribir_Varg(L"DBarraTareas::_IniciarITaskBar Iniciado por primera vez %d\n", _BarraTareas);
				#endif
				return TRUE;
			}
			else {
				#if DBARRATAREAS_MOSTRARDEBUG == TRUE
					Debug_Escribir(L"DBarraTareas::_IniciarITaskBar -> Error iniciando CLSID_TaskbarList.\n");
				#endif
				_BarraTareas = NULL;
				return FALSE;
			}
		}
		else {
			#if DBARRATAREAS_MOSTRARDEBUG == TRUE
			Debug_Escribir_Varg(L"DBarraTareas::_IniciarITaskBar Ya se ha iniciado anteriormente %d\n", _BarraTareas);
			#endif
			return TRUE;
		}
	}
};