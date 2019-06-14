#include "pch.h"
#include "DTemporizador.h"
#include <functional>

namespace DWL {

	DTemporizador::DTemporizador(void) {
	}


	DTemporizador::~DTemporizador(void) {
		TerminarTemporizadores();
	}

	void DTemporizador::CrearTemporizador(const DWORD Milisegundos, const BOOL UnaVez, std::function<void(void)> Callback) {
		HANDLE nTimer = NULL;
		DTemporizador_Unico* Temporizador = new DTemporizador_Unico(0, this, UnaVez, Callback);
		_Temporizadores.push_back(Temporizador);

		BOOL Ret = CreateTimerQueueTimer(&nTimer, NULL, reinterpret_cast<WAITORTIMERCALLBACK>(_TimerProcLambda), Temporizador, Milisegundos, Milisegundos, WT_EXECUTEINTIMERTHREAD);
		Temporizador->Timer = nTimer;
	}


	void DTemporizador::CrearTemporizador(const UINT_PTR nID, const DWORD Milisegundos, const BOOL UnaVez) {
		HANDLE nTimer = NULL;
		DTemporizador_Unico* Temporizador = new DTemporizador_Unico(nID, this, UnaVez);
		_Temporizadores.push_back(Temporizador);

		BOOL Ret = CreateTimerQueueTimer(&nTimer, NULL, reinterpret_cast<WAITORTIMERCALLBACK>(_TimerProc), Temporizador, Milisegundos, UnaVez, WT_EXECUTEINTIMERTHREAD);
		Temporizador->Timer = nTimer;
	}

	void CALLBACK DTemporizador::_TimerProc(PVOID lpParameter, BOOLEAN TimerOrWaitFired) {
		DTemporizador_Unico* ClaseTemporizador = static_cast<DTemporizador_Unico*>(lpParameter);
		ClaseTemporizador->Padre->Evento_Temporizador(ClaseTemporizador->ID);
		// Elimino el temporizador si solo es de una vez
		if (ClaseTemporizador->UnaVez == TRUE) {
			ClaseTemporizador->Padre->TerminarTemporizador(ClaseTemporizador->ID);
		}
	}


	void CALLBACK DTemporizador::_TimerProcLambda(PVOID lpParameter, BOOLEAN TimerOrWaitFired) {
		DTemporizador_Unico* ClaseTemporizador = static_cast<DTemporizador_Unico*>(lpParameter);
		ClaseTemporizador->Callback();
		// Elimino el temporizador si solo es de una vez
		if (ClaseTemporizador->UnaVez == TRUE) {
			ClaseTemporizador->Padre->TerminarTemporizador(ClaseTemporizador->ID);
		}
	}

	void DTemporizador::TerminarTemporizador(const UINT_PTR nID) {
		size_t Pos = 0;
		if (_BuscarTemporizadorPos(nID, Pos) == TRUE) {
			if (DeleteTimerQueueTimer(NULL, _Temporizadores[Pos]->Timer, NULL) != 0) {
				delete _Temporizadores[Pos];
			}
			_Temporizadores.erase(_Temporizadores.begin() + Pos);
		}
	}

	void DTemporizador::TerminarTemporizadores(void) {
		//	size_t Pos = 0;
		for (size_t i = 0; i < _Temporizadores.size(); i++) {
			if (DeleteTimerQueueTimer(NULL, _Temporizadores[i]->Timer, NULL) != 0) {
				delete _Temporizadores[i];
			}
			//		_Temporizadores.erase(_Temporizadores.begin() + i);
		}
		_Temporizadores.resize(0);
	}

	const BOOL DTemporizador::_BuscarTemporizadorPos(const UINT_PTR bID, size_t& cPos) {
		for (size_t i = 0; i < _Temporizadores.size(); i++) {
			if (_Temporizadores[i]->ID == bID) {
				cPos = i;
				return TRUE;
			}
		}
		return FALSE;
	}
}