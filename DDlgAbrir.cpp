#include "pch.h"
#include "DDlgAbrir.h"

#include <commdlg.h>
#include <shlobj.h>             // Libreria para los objetos Shell


namespace DWL {

	DDlgAbrir::DDlgAbrir(void) {

	}

	DDlgAbrir::DDlgAbrir(const wchar_t* PathSh, const wchar_t* Filtro, const wchar_t* Titulo, const bool MultiSeleccion, HWND hWndPadre) {
		Mostrar(PathSh, Filtro, Titulo, MultiSeleccion, hWndPadre);
	}

	DDlgAbrir::~DDlgAbrir(void) {

	}

	const size_t DDlgAbrir::Mostrar(const wchar_t* PathSh, const wchar_t* Filtro, const wchar_t* Titulo, const bool MultiSeleccion, HWND hWndPadre) {
		OPENFILENAME	ofn;
		wchar_t			szFile[4096];
		bool		    Multi = false;
		wchar_t			Path[512];
		wchar_t			Archi[512];
		wchar_t		    UltimoDir[MAX_PATH + 1];
		std::wstring	Tmp;
		int				N = 0;
		int				i;
		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lpstrCustomFilter	= NULL;
		ofn.nFilterIndex		= 1;
		ofn.lpstrFile			= szFile;
		ofn.nMaxFile			= 4096;
		ofn.lpstrFileTitle		= NULL;
		ofn.nMaxFileTitle		= 4096;
		ofn.lpstrInitialDir		= TEXT(".\0");
		ofn.lpstrTitle			= Titulo;
		ofn.lpstrFilter			= Filtro;
		ofn.lStructSize			= sizeof(OPENFILENAME);
		ofn.hwndOwner			= hWndPadre;
		if (MultiSeleccion == true) ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT;
		else						ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
		Archivos.clear();
		for (i = 0; i < 4096; i++) szFile[i] = 0;
		szFile[0] = '\0';
		// Obtengo el directorio actual para luego volverlo a asignar
		GetCurrentDirectory(MAX_PATH + 1, UltimoDir);
		SetCurrentDirectory(PathSh);
		if (GetOpenFileName(&ofn)) {
			wcscpy_s(Path, 512, szFile);
			i = static_cast<int>(wcslen(Path));

			while (szFile[i] != 0 || szFile[i + 1] != 0) {
//				i++;
				if (szFile[++i] != 0) {
					Archi[N++] = szFile[i];
//					N++;
				}
				else {
					Archi[N] = 0;
					N = 0;
					Multi = true;
					Tmp = Path;
					if (Tmp[Tmp.size() - 1] != L'\\') {
						Tmp += L'\\';
					}
					Tmp += Archi;
					Archivos.push_back(Tmp);
				}
			}
			if (Multi == false) {
				Tmp = szFile;   
				Archivos.push_back(Tmp);
			}
		}
		SetCurrentDirectory(UltimoDir);
		return Archivos.size();
	}
};