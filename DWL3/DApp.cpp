#include "pch.h"
#include "DApp.h"
#include <versionhelpers.h>

DWL::DApp *_Aplicacion = NULL;

namespace DWL {

	DApp::DApp(void) {
		_Aplicacion = this;
	}

	/* Vacia la cola de mensajes para este hilo */
	void DApp::Eventos_Mirar(void) {
		static MSG msg;
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}


	// TODO : fer que retorni un TCHAR *
	const TCHAR* DApp::ObtenerSO(void) {
		UINT Version = 0;

		if (IsWindowsXPOrGreater())			Version++;
		if (IsWindowsXPSP1OrGreater())		Version++;
		if (IsWindowsXPSP2OrGreater())		Version++;
		if (IsWindowsXPSP3OrGreater())		Version++;
		if (IsWindowsVistaOrGreater())		Version++;
		if (IsWindowsVistaSP1OrGreater())	Version++;
		if (IsWindowsVistaSP2OrGreater())	Version++;
		if (IsWindows7OrGreater())			Version++;
		if (IsWindows7SP1OrGreater())		Version++;
		if (IsWindows8OrGreater())			Version++;
		if (IsWindows8Point1OrGreater())	Version++;
		if (IsWindows10OrGreater())			Version++;

		switch (Version) {
		case  0: return L"";
		case  1: return L"Windows XP";
		case  2: return L"Windows XP SP1";
		case  3: return L"Windows XP SP2";
		case  4: return L"Windows XP SP3";
		case  5: return L"Windows Vista";
		case  6: return L"Windows Vista SP1";
		case  7: return L"Windows Vista SP2";
		case  8: return L"Windows 7";
		case  9: return L"Windows 7 SP1";
		case 10: return L"Windows 8";
		case 11: return L"Windows 8.1";
		case 12: return L"Windows 10";
		default: return L"Windows 10 o superior";
		}

		return L"";
	}


	//	DApp* _Aplicacion = NULL;
};

