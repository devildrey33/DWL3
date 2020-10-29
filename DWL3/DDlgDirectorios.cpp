#include "pch.h"
#include "DMensajesWnd.h"
#include "DDlgDirectorios.h"
//#include "resource.h"

namespace DWL {

	#define ID_ARBOLDIRECTORIOS		WM_USER + 1000
	#define ID_EDICIONDIRECTORIO	WM_USER + 1001
	#define ID_BOTONACEPTAR			WM_USER + 1002
	#define ID_BOTONCANCELAR		WM_USER + 1003

	#define MIN_DLGDIRECTORIOS_ANCHO 300
	#define MIN_DLGDIRECTORIOS_ALTO  300

	DDlgDirectorios::DDlgDirectorios(void) : _Padre(NULL), _Terminado(FALSE){
	}


	DDlgDirectorios::~DDlgDirectorios(void) {
	}


	const BOOL DDlgDirectorios::Mostrar(const wchar_t *Titulo, const wchar_t* PathInicial, std::wstring &PathFinal) {

		IFileDialog* pfd = NULL;
		bool bSuccess = false;

		if (CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pfd)) != S_OK)
			return bSuccess;

		DWORD dwOptions;

		if (pfd->GetOptions(&dwOptions) != S_OK) {
			pfd->Release();
			return bSuccess;
		}

		pfd->SetOptions(dwOptions | FOS_PICKFOLDERS);

		//
		// TODO: SetDefaultFolder -> StartPath
		//

		if (pfd->Show(NULL) == S_OK) {
			IShellItem* psi;

			if (pfd->GetResult(&psi) == S_OK) {
				WCHAR *strOut = NULL;

				if (psi->GetDisplayName(SIGDN_DESKTOPABSOLUTEPARSING, &strOut) != S_OK)	{
					return bSuccess;
				}

				PathFinal = strOut;
				CoTaskMemFree(strOut);
				psi->Release();
				bSuccess = true;
			}
		}

		pfd->Release();

		return bSuccess;
	}


};