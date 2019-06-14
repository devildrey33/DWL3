#include "pch.h"
#include "DExplorarDirectoriosEx.h"
#include "DDirectoriosWindows.h"
#include "DMouse.h"

namespace DWL {

	DExplorarDirectoriosEx::DExplorarDirectoriosEx(void) : _NodoEscritorio(NULL), _NodoMisDocumentos(NULL), _NodoMiPC(NULL), _NodoRed(NULL), MostrarArchivosOcultos(FALSE) {
	}

	DExplorarDirectoriosEx::~DExplorarDirectoriosEx(void) {
	}

	HWND DExplorarDirectoriosEx::CrearExplorarDirectoriosEx(DhWnd *nPadre, const int cX, const int cY, const int cAncho, const int cAlto, const int cID, DWORD nEstilos) {
		HWND rhWnd = CrearArbolEx(nPadre, cX, cY, cAncho, cAlto, cID, nEstilos);
		ActualizarDirectorios();
		return rhWnd;
	}

	DExplorarDirectoriosEx_Nodo *DExplorarDirectoriosEx::AgregarEntrada(const TCHAR *nTexto, DExplorarDirectoriosEx_Nodo *nPadre, const int IconoRecursos, DExplorarDirectoriosEx_TipoNodo nTipo, const wchar_t *nPath, const BOOL nActivado) {
		DExplorarDirectoriosEx_Nodo *Ret = AgregarNodo<DExplorarDirectoriosEx_Nodo>(nTexto, nPadre, IconoRecursos);
		Ret->Tipo		= nTipo;
		Ret->Path		= nPath;
		Ret->Activado(nActivado);
		// Añado la barra final al path (si no existe)
		if (Ret->Path.size() > 0) {
			if (Ret->Path[Ret->Path.size() - 1] != L'\\') Ret->Path += L'\\';
		}
		return Ret;
	}

	DExplorarDirectoriosEx_Nodo *DExplorarDirectoriosEx::AgregarEntrada(const TCHAR *nTexto, DExplorarDirectoriosEx_Nodo *nPadre, const GUID &KnowFolderId, DExplorarDirectoriosEx_TipoNodo nTipo, const wchar_t *nPath, const BOOL nActivado) {
		DExplorarDirectoriosEx_Nodo *Ret = AgregarNodo<DExplorarDirectoriosEx_Nodo>(nTexto, nPadre, KnowFolderId);
		Ret->Tipo		= nTipo;
		Ret->Path		= nPath;
		Ret->Activado(nActivado);
		// Añado la barra final al path (si no existe)
		if (Ret->Path.size() > 0) {
			if (Ret->Path[Ret->Path.size() - 1] != L'\\') Ret->Path += L'\\';
		}
		return Ret;
	}

	DExplorarDirectoriosEx_Nodo *DExplorarDirectoriosEx::AgregarEntrada(const TCHAR *nTexto, DExplorarDirectoriosEx_Nodo *nPadre, const wchar_t *PathIcono, const int PosIcono, DExplorarDirectoriosEx_TipoNodo nTipo, const wchar_t *nPath, const BOOL nActivado) {
		DExplorarDirectoriosEx_Nodo *Ret = AgregarNodo<DExplorarDirectoriosEx_Nodo>(nTexto, nPadre, PathIcono, PosIcono);
		Ret->Tipo		= nTipo;
		Ret->Path		= nPath;
		Ret->Activado(nActivado);
		// Añado la barra final al path (si no existe)
		if (Ret->Path.size() > 0) {
			if (Ret->Path[Ret->Path.size() - 1] != L'\\') Ret->Path += L'\\';
		}
		return Ret;
	}


	void DExplorarDirectoriosEx::Evento_Nodo_Expandido(DWL::DArbolEx_Nodo *nNodo, const BOOL nExpandido) {
		if (nNodo->TotalHijos() == 0) return;
		DExplorarDirectoriosEx_Nodo *TmpHijo = static_cast<DExplorarDirectoriosEx_Nodo *>(nNodo->Hijo(0));
		DExplorarDirectoriosEx_Nodo *TmpNodo = static_cast<DExplorarDirectoriosEx_Nodo *>(nNodo);

		if (nExpandido == TRUE) { // Expandir
			// Si no es el nodo de MiPC
			if (TmpNodo->Tipo != DExplorarDirectoriosEx_TipoNodo_MiPC) {
				DMouse::CambiarCursor(DMouse_Cursor_Procesando);
				if		(TmpHijo->Tipo == DExplorarDirectoriosEx_TipoNodo_Directorio_PorExplorar)	{	_EnumerarDirectorios(TmpNodo);	}
				else if (TmpHijo->Tipo == DExplorarDirectoriosEx_TipoNodo_Red_PorExplorar)			{	_EnumerarRed(NULL, TmpNodo);	}
				DMouse::CambiarCursor();
				// Elimino el nodo que hace visible al expansor
				if (TmpHijo->Texto[0] == L'+' || TmpHijo->Texto[0] == L'-') {
					EliminarNodo(TmpHijo);
				}
			}
		}
		else { // Contraer
			if (TmpNodo->Tipo != DExplorarDirectoriosEx_TipoNodo_MiPC && TmpNodo->Tipo != DExplorarDirectoriosEx_TipoNodo_EquipoRed) {
				nNodo->BorrarHijos();					
				if (TmpNodo->Tipo == DExplorarDirectoriosEx_TipoNodo_Red) {
					AgregarEntrada(L"-", TmpNodo, 0, DExplorarDirectoriosEx_TipoNodo_Red_PorExplorar);
				}
				else {
					AgregarEntrada(L"+", TmpNodo, 0, DExplorarDirectoriosEx_TipoNodo_Directorio_PorExplorar);
				}
			}
		}
	}


	void DExplorarDirectoriosEx::ActualizarDirectorios(void) {
		BorrarTodo();

		std::wstring PathEscritorio, PathDocumentos;
		DDirectoriosWindows::ObtenerDirectorio(FOLDERID_Desktop, PathEscritorio);
		DDirectoriosWindows::ObtenerDirectorio(FOLDERID_Documents, PathDocumentos);

		_NodoEscritorio		= AgregarEntrada(L"Escritorio",		NULL, FOLDERID_Desktop,			DExplorarDirectoriosEx_TipoNodo_Escritorio, PathEscritorio.c_str());
		_NodoMisDocumentos	= AgregarEntrada(L"Mis Documentos", NULL, FOLDERID_PublicDocuments, DExplorarDirectoriosEx_TipoNodo_MisDocumentos, PathDocumentos.c_str()); // FOLDERID_Documents
		_NodoMiPC			= AgregarEntrada(L"Mi PC",			NULL, FOLDERID_ComputerFolder,  DExplorarDirectoriosEx_TipoNodo_MiPC);
		_NodoRed			= AgregarEntrada(L"Red",			NULL, FOLDERID_NetworkFolder,   DExplorarDirectoriosEx_TipoNodo_Red);

		AgregarEntrada(L"+", _NodoEscritorio,	 0, DExplorarDirectoriosEx_TipoNodo_Directorio_PorExplorar);
		AgregarEntrada(L"+", _NodoMisDocumentos, 0, DExplorarDirectoriosEx_TipoNodo_Directorio_PorExplorar);
		AgregarEntrada(L"-", _NodoRed,			 0, DExplorarDirectoriosEx_TipoNodo_Red_PorExplorar);

		// Exploro las unidades fisicas disponibles 
		DWORD							MascaraBitsUnidades = GetLogicalDrives();
		UINT							TipoUnidad			= 0;
		DExplorarDirectoriosEx_TipoNodo TipoNodo			= DExplorarDirectoriosEx_TipoNodo_INDEFINIDO;
		wchar_t                         Letra				= 0;
		wchar_t							NombreDisco[256]	= TEXT("");
		wchar_t							Unidad[5]			= TEXT("?:\\");
		wchar_t							NombreSistema[128]	= TEXT("");
		DExplorarDirectoriosEx_Nodo    *TmpNodo				= NULL;
		std::wstring                    TextoNodo;
		BOOL                            Ret;
		DWORD							Component;
		DWORD							Flags;

		// De la C a la Z
		for (TCHAR i = 2; i < 32; i++) {
			if (MascaraBitsUnidades & (1 << i)) {
				Letra = i + 'A';				
				TipoUnidad = GetDriveType(Unidad);
				switch (TipoUnidad) {
/*					case DRIVE_REMOVABLE : // Disquetera
						TipoNodo = DExplorarDirectoriosEx_TipoNodo_Disquetera;
						TextoNodo = L"Disquetera ("; TextoNodo += Letra; TextoNodo += L":)";
						break;*/
					case DRIVE_FIXED: // Disco duro
						TipoNodo = DExplorarDirectoriosEx_TipoNodo_DiscoDuro;
						TextoNodo = L"Disco Duro ("; TextoNodo += Letra; TextoNodo += L":)";
						break;
					case DRIVE_REMOTE: // Disco duro de red
						TipoNodo = DExplorarDirectoriosEx_TipoNodo_DiscoDuroRed;
						TextoNodo = L"Disco de Red ("; TextoNodo += Letra; TextoNodo += L":)";
						break;
					case DRIVE_CDROM: // Unidad optica
						TipoNodo = DExplorarDirectoriosEx_TipoNodo_CDROM;
						TextoNodo = L"Unidad Óptica ("; TextoNodo += Letra; TextoNodo += L":)";
						break;
					default : // Desconocido
						TipoNodo = DExplorarDirectoriosEx_TipoNodo_INDEFINIDO;
						TextoNodo = L"Unidad ("; TextoNodo += Letra; TextoNodo += L":)";
						break;
				}

				Unidad[0] = Letra;

				// Obtengo el nombre del volumen (si es posible)
				Ret = TRUE;
				if (Unidad[0] != L'A' && Unidad[0] != L'B') { // Con las unidades de disquete ni lo intento, para no bloquear el PC
					Ret = GetVolumeInformation(Unidad, NombreDisco, 256, NULL, &Component, &Flags, NombreSistema, 128);
				}
				// Si la unidad tiene un nombre de volumen
				if (Ret && NombreDisco[0] != 0) {
					TextoNodo = NombreDisco; TextoNodo += L"("; TextoNodo += Letra; TextoNodo += L":)";
				}
				if (Ret == TRUE) {
					TmpNodo = AgregarEntrada(TextoNodo.c_str(), _NodoMiPC, Unidad, 0, TipoNodo, Unidad);
					//TmpNodo = AgregarNodo<DExplorarDirectoriosEx_Nodo>(TextoNodo.c_str(), NodoMiPC, Unidad);
					//TmpNodo->Tipo = TipoNodo;
//					AgregarNodo<DExplorarDirectoriosEx_Nodo>(L"+", TmpNodo);
					AgregarEntrada(L"+", TmpNodo, 0, DExplorarDirectoriosEx_TipoNodo_Directorio_PorExplorar);
				}
			}
		}
	}

	const BOOL DExplorarDirectoriosEx::_EnumerarDirectorios(DExplorarDirectoriosEx_Nodo *nNodo) {
		WIN32_FIND_DATA					FindInfoPoint;
		HANDLE							hFind = NULL;
		std::wstring					nPath = nNodo->Path; nPath += (nPath[nPath.size() - 1] == L'\\') ? L"*.*" : L"\\*.*";

		std::wstring					sPath;
		hFind = FindFirstFile(nPath.c_str(), &FindInfoPoint);
		BOOL							Agregar = FALSE;
		DExplorarDirectoriosEx_Nodo    *TmpNodo = NULL;
		std::wstring                    PathWindows;
		
		while (FindNextFile(hFind, &FindInfoPoint) != 0) {
			// Si es un directorio y no es ni "." ni ".."
			if (FindInfoPoint.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY &&															// Si es un directorio
				(FindInfoPoint.cFileName[0] != L'.' && FindInfoPoint.cFileName[1] != L'\0') &&											// Si no es el directorio ".\0" (puede ser .algo)
				(FindInfoPoint.cFileName[0] != L'.' && FindInfoPoint.cFileName[1] != L'.' && FindInfoPoint.cFileName[2] != L'\0')) {	// Si no es el directorio "..\0" (puede ser ..algo)
				// Miro si es un directorio oculto y si hay que mostrarlos
				Agregar = false;
				if (FindInfoPoint.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN && MostrarArchivosOcultos == TRUE)	{	Agregar = TRUE;	 } // Directorio oculto, y hay que mostrar directorios ocultos
				else																							{	Agregar = TRUE;	 } // Es un directorio normal
				
				if (FindInfoPoint.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT)								{	Agregar = FALSE; } // Es un directorio link/junction				

				// Agrego el directorio
				sPath = nNodo->Path + FindInfoPoint.cFileName;
				if (Agregar == TRUE) {
					TmpNodo = AgregarEntrada(FindInfoPoint.cFileName, nNodo, sPath.c_str(), 0, DExplorarDirectoriosEx_TipoNodo_Directorio, sPath.c_str(), TRUE);
					BOOL Ret = _TieneDirectorios(sPath.c_str());
					if		(Ret == TRUE)	{	AgregarEntrada(L"+", TmpNodo, 0, DExplorarDirectoriosEx_TipoNodo_Directorio_PorExplorar);	} // Si tiene subdirectorios agrego el expansor
					else if (Ret == 2)		{	TmpNodo->Activado(FALSE);																	} // Aceso denegado (desactivo el item)
				}
				
			}
		}

		FindClose(hFind);
		return TRUE;
	}


	const BOOL DExplorarDirectoriosEx::_TieneDirectorios(const wchar_t *tPath) {
		WIN32_FIND_DATA					FindInfoPoint;
		HANDLE							hFind = NULL;
		std::wstring					nPath = tPath; nPath += (nPath[nPath.size() - 1] == L'\\') ? L"*.*" : L"\\*.*";
		BOOL                            nTieneDirectorios = FALSE;
		hFind = FindFirstFile(nPath.c_str(), &FindInfoPoint);

		if (hFind == INVALID_HANDLE_VALUE && GetLastError() == ERROR_ACCESS_DENIED) {			
			return 2;
		}

		while (FindNextFile(hFind, &FindInfoPoint) != 0) {
			// Si es un directorio y no es ni "." ni ".."
			if (FindInfoPoint.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY &&															// Si es un directorio
				(FindInfoPoint.cFileName[0] != L'.' && FindInfoPoint.cFileName[1] != L'\0') &&											// Si no es el directorio ".\0" (puede ser .algo)
				(FindInfoPoint.cFileName[0] != L'.' && FindInfoPoint.cFileName[1] != L'.' && FindInfoPoint.cFileName[2] != L'\0')) {	// Si no es el directorio "..\0" (puede ser ..algo)
				// Devuelvo que tiene subdirectorios
				nTieneDirectorios = TRUE;
				break;
			}
		}
		FindClose(hFind);
		return nTieneDirectorios;
	}

	const BOOL DExplorarDirectoriosEx::_EnumerarRed(LPNETRESOURCE lpnr, DExplorarDirectoriosEx_Nodo *nNodo) {
		HANDLE								hEnum;
		DWORD								cbBuffer = 16384;   // 16K is a good size
		DWORD								cEntries = -1;      // enumerate all possible entries
		LPNETRESOURCE						lpnrLocal;			// pointer to enumerated structures
		DWORD								dwResult = WNetOpenEnum(RESOURCE_GLOBALNET, RESOURCETYPE_DISK, 0, lpnr, &hEnum);
		size_t								Separador = 0, Contador = 0;
		if (dwResult != NO_ERROR) {
			//SOLO_DEBUG(DWL::DWLDebug::ImprimirDebug(TEXT("WnetOpenEnum failed with error %d\n"), dwResult));
			return FALSE;
		}

		lpnrLocal = (LPNETRESOURCE)GlobalAlloc(GPTR, cbBuffer + 1);

		DWORD dwResultEnum = NULL;
		if (lpnrLocal == NULL) {
//			SOLO_DEBUG(DWL::DWLDebug::ImprimirDebug(TEXT("WnetOpenEnum failed with error %d\n"), dwResult));
			return FALSE;
		}

		do {
			if (lpnrLocal) ZeroMemory(lpnrLocal, cbBuffer);
			dwResultEnum = WNetEnumResource(hEnum, &cEntries, lpnrLocal, &cbBuffer);

			if (dwResultEnum == NO_ERROR) {
				for (DWORD i = 0; i < cEntries; i++) {
					#if DEXPLORARDIRECTORIOSEX_MOSTRARDEBUG == TRUE
						_DEBUG_EnumerarRed(i, &lpnrLocal[i]);
					#endif
					// Busco donde empieza la ultima barra del nombre
					for (Contador = wcslen(lpnrLocal[i].lpRemoteName); Contador > 0; Contador--) {
						if (lpnrLocal[i].lpRemoteName[Contador - 1] == TEXT('\\')) {
							Separador = Contador;
							break;
						}
					}
					// Guardo el nombre sin barras
					std::wstring NombreStr = &lpnrLocal[i].lpRemoteName[Separador];
					// Nodo temporal donde se añadirán los nuevos nodos
					DExplorarDirectoriosEx_Nodo *TmpNodo = nNodo;
					// Es un servidor
					if (lpnrLocal[i].dwDisplayType == RESOURCEDISPLAYTYPE_SERVER) { 
						TmpNodo = AgregarEntrada(NombreStr.c_str(), nNodo, FOLDERID_ComputerFolder, DExplorarDirectoriosEx_TipoNodo_EquipoRed, lpnrLocal[i].lpRemoteName);
					}
					// Directorio de red
					if (lpnrLocal[i].dwDisplayType == RESOURCEDISPLAYTYPE_SHARE && lpnrLocal[i].dwType == RESOURCETYPE_DISK) {
						TmpNodo = AgregarEntrada(NombreStr.c_str(), nNodo, FOLDERID_NetHood, DExplorarDirectoriosEx_TipoNodo_Directorio, lpnrLocal[i].lpRemoteName);
						AgregarEntrada(L"+", TmpNodo, 0, DExplorarDirectoriosEx_TipoNodo_Directorio_PorExplorar);
					}

					// Es un contenedor (lo volvemos a escanear)
					if (RESOURCEUSAGE_CONTAINER == (lpnrLocal[i].dwUsage & RESOURCEUSAGE_CONTAINER)) {
						_EnumerarRed(&lpnrLocal[i], TmpNodo);
					}
				}
			}

		} while (dwResultEnum != ERROR_NO_MORE_ITEMS);

		GlobalFree((HGLOBAL)lpnrLocal);
		dwResult = WNetCloseEnum(hEnum);

		if (dwResult != NO_ERROR) {
			//SOLO_DEBUG(DWL::DWLDebug::ImprimirDebug(TEXT("WNetCloseEnum failed with error %d\n"), dwResult));
			return FALSE;
		}


		return TRUE;
	}

	const wchar_t *DExplorarDirectoriosEx::PathSeleccionado(void) {
		if (_NodoMarcado != NULL)	{	return _NodoMarcado->Texto.c_str();		}
		else						{	return NULL;							}
	}

	DIcono *DExplorarDirectoriosEx::IconoSeleccionado(void) {
		if (_NodoMarcado != NULL)	{	return &_NodoMarcado->Icono();	}
		else						{	return NULL;					}
	}

	/*
	void DExplorarDirectoriosEx::_ObtenerPathNodo(DExplorarDirectoriosEx_Nodo *nNodo, std::wstring &oPath) {
		DExplorarDirectoriosEx_Nodo *TmpNodo = nNodo;
		std::wstring				 TmpStr;
		BOOL                         Raiz = false;
		while (TmpNodo->_Padre != NULL) {
			switch (TmpNodo->Tipo) {
				case DExplorarDirectoriosEx_TipoNodo_Escritorio: 
					DDirectoriosWindows::ObtenerDirectorio(FOLDERID_Desktop, TmpStr);
					break;
				case DExplorarDirectoriosEx_TipoNodo_MisDocumentos:
					DDirectoriosWindows::ObtenerDirectorio(FOLDERID_Documents, TmpStr);
					oPath = TmpStr + oPath;
					break;
				case DExplorarDirectoriosEx_TipoNodo_MiPC: // No puede ser...
					return;
				case DExplorarDirectoriosEx_TipoNodo_DiscoDuro:
				case DExplorarDirectoriosEx_TipoNodo_CDROM : 
					break; // MiPC
			}
			oPath = L"\\" + TmpNodo->Texto;
			TmpNodo = static_cast<DExplorarDirectoriosEx_Nodo *>(TmpNodo->_Padre);
		}
	}*/

#if DEXPLORARDIRECTORIOSEX_MOSTRARDEBUG == TRUE
	void DExplorarDirectoriosEx::_DEBUG_EnumerarRed(int i, LPNETRESOURCE lpnrLocal) {
		Debug_Escribir_Varg(L"Local[%d] dwScope       : ", i);
		switch (lpnrLocal->dwScope) {
			case (RESOURCE_CONNECTED):							Debug_EscribirSinMS(L"RESOURCE_CONNECTED\n");												break;
			case (RESOURCE_GLOBALNET):							Debug_EscribirSinMS(L"RESOURCE_GLOBALNET\n");												break;
			case (RESOURCE_REMEMBERED):							Debug_EscribirSinMS(L"RESOURCE_REMEMBERED\n");												break;
			default:											Debug_EscribirSinMS_Varg(L"Alcance desconocido %d\n", lpnrLocal->dwScope);					break;
		}

		Debug_Escribir_Varg(L"Local[%d] dwType        : ", i);
		switch (lpnrLocal->dwType) {
			case (RESOURCETYPE_ANY):							Debug_EscribirSinMS(L"RESOURCETYPE_ANY\n");													break;
			case (RESOURCETYPE_DISK):							Debug_EscribirSinMS(L"RESOURCETYPE_DISK\n");												break;
			case (RESOURCETYPE_PRINT):							Debug_EscribirSinMS(L"RESOURCETYPE_PRINT\n");												break;
			default:											Debug_EscribirSinMS_Varg(L"Tipo desconocido %d\n", lpnrLocal->dwType);						break;
		}

		Debug_Escribir_Varg(L"Local[%d] dwDisplayType : ", i);
		switch (lpnrLocal->dwDisplayType) {
			case (RESOURCEDISPLAYTYPE_GENERIC):					Debug_EscribirSinMS(L"RESOURCEDISPLAYTYPE_GENERIC\n");										break;
			case (RESOURCEDISPLAYTYPE_DOMAIN):					Debug_EscribirSinMS(L"RESOURCEDISPLAYTYPE_DOMAIN\n");										break;
			case (RESOURCEDISPLAYTYPE_SERVER):					Debug_EscribirSinMS(L"RESOURCEDISPLAYTYPE_SERVER\n");										break;
			case (RESOURCEDISPLAYTYPE_SHARE):					Debug_EscribirSinMS(L"RESOURCEDISPLAYTYPE_SHARE\n");										break;
			case (RESOURCEDISPLAYTYPE_FILE):					Debug_EscribirSinMS(L"RESOURCEDISPLAYTYPE_FILE\n");											break;
			case (RESOURCEDISPLAYTYPE_GROUP):					Debug_EscribirSinMS(L"RESOURCEDISPLAYTYPE_GROUP\n");										break;
			case (RESOURCEDISPLAYTYPE_NETWORK):					Debug_EscribirSinMS(L"RESOURCEDISPLAYTYPE_NETWORK\n");										break;
			default:											Debug_EscribirSinMS_Varg(L"Tipo visualización desconocido %d\n", lpnrLocal->dwDisplayType);	break;
		}

		Debug_Escribir_Varg(L"Local[%d] dwUsage       : 0x%x = ", i, lpnrLocal->dwUsage);
		#ifdef _DEBUG
			if (lpnrLocal->dwUsage & RESOURCEUSAGE_CONNECTABLE)		Debug_EscribirSinMS(L"CONNECTABLE ");
			if (lpnrLocal->dwUsage & RESOURCEUSAGE_CONTAINER)		Debug_EscribirSinMS(L"CONTAINER ");
		#endif
		Debug_EscribirSinMS(L"\n");


		Debug_Escribir_Varg(L"Local[%d] lpLocalname   : %s\n", i, lpnrLocal->lpLocalName);
		Debug_Escribir_Varg(L"Local[%d] lpRemotename  : %s\n", i, lpnrLocal->lpRemoteName);
		Debug_Escribir_Varg(L"Local[%d] lpComment     : %s\n", i, lpnrLocal->lpComment);
		Debug_Escribir_Varg(L"Local[%d] lpProvider    : %s\n", i, lpnrLocal->lpProvider);
		Debug_Escribir(L"------------------------------\n");

	}
#endif

};