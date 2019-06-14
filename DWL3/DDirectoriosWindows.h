#ifndef DDIRECTORIOSWINDOWS_H
	#define DDIRECTORIOSWINDOWS_H

//	#include "DObjeto.h"
	#include <string>
	#include <Shlobj.h>

	namespace DWL {

		class DDirectoriosWindows {
		  public :
								DDirectoriosWindows(void) { };
							   ~DDirectoriosWindows(void) { };
	//		const DEnum_Objeto	Objeto_Tipo(void) { return DEnum_Objeto_DirectoriosWindows; };


			static const BOOL	Comun_AppData(std::wstring &StrOut)							{ return ObtenerDirectorio(FOLDERID_ProgramData, StrOut); };
			static const BOOL	UsuarioActual_AppData(std::wstring &StrOut)					{ return ObtenerDirectorio(FOLDERID_RoamingAppData, StrOut); };
			static const BOOL	Comun_Escritorio(std::wstring &StrOut)						{ return ObtenerDirectorio(FOLDERID_PublicDesktop, StrOut); };
			static const BOOL	UsuarioActual_Escritorio(std::wstring &StrOut)				{ return ObtenerDirectorio(FOLDERID_Desktop, StrOut); };
			static const BOOL	Comun_MenuInicio_Programas(std::wstring &StrOut)			{ return ObtenerDirectorio(FOLDERID_CommonPrograms, StrOut); };
			static const BOOL	UsuarioActual_MenuInicio_Programas(std::wstring &StrOut)	{ return ObtenerDirectorio(FOLDERID_Programs, StrOut); };
			static const BOOL	Comun_MisDocumentos(std::wstring &StrOut)					{ return ObtenerDirectorio(FOLDERID_PublicDocuments, StrOut); };
			static const BOOL	UsuarioActual_MisDocumentos(std::wstring &StrOut)			{ return ObtenerDirectorio(FOLDERID_Documents, StrOut); };
			static const BOOL	Comun_MisImagenes(std::wstring &StrOut)						{ return ObtenerDirectorio(FOLDERID_PublicPictures, StrOut); };
			static const BOOL	UsuarioActual_MisImagenes(std::wstring &StrOut)				{ return ObtenerDirectorio(FOLDERID_Pictures, StrOut); };
			static const BOOL	Comun_MiMusica(std::wstring &StrOut)						{ return ObtenerDirectorio(FOLDERID_PublicMusic, StrOut); };
			static const BOOL	UsuarioActual_MiMusica(std::wstring &StrOut)				{ return ObtenerDirectorio(FOLDERID_Music, StrOut); };
			static const BOOL	Comun_MisVideos(std::wstring &StrOut)						{ return ObtenerDirectorio(FOLDERID_PublicVideos, StrOut); };
			static const BOOL	UsuarioActual_MisVideos(std::wstring &StrOut)				{ return ObtenerDirectorio(FOLDERID_Videos, StrOut); };
			static const BOOL	Windows(std::wstring &StrOut)								{ return ObtenerDirectorio(FOLDERID_Windows, StrOut); };



			static const BOOL	ObtenerDirectorio(const GUID& rfid, std::wstring &StrOut) {
				PWSTR Tmp = NULL;
				if (S_OK == SHGetKnownFolderPath(rfid, NULL, 0, &Tmp)) {
					StrOut = Tmp;
					CoTaskMemFree(Tmp);
					return TRUE;
				}
				return FALSE;
			}


/*			static const BOOL	Comun_AppData(std::wstring &StrOut);
			static const BOOL	UsuarioActual_AppData(std::wstring &StrOut);
			static const BOOL	Comun_Escritorio(std::wstring &StrOut);
			static const BOOL	UsuarioActual_Escritorio(std::wstring &StrOut);
			static const BOOL	Comun_MenuInicio_Programas(std::wstring &StrOut);
			static const BOOL	UsuarioActual_MenuInicio_Programas(std::wstring &StrOut);
			static const BOOL	Comun_MisDocumentos(std::wstring &StrOut);
			static const BOOL	UsuarioActual_MisDocumentos(std::wstring &StrOut);
			static const BOOL	Comun_MisImagenes(std::wstring &StrOut);
			static const BOOL	UsuarioActual_MisImagenes(std::wstring &StrOut);
			static const BOOL	Comun_MiMusica(std::wstring &StrOut);
			static const BOOL	UsuarioActual_MiMusica(std::wstring &StrOut);
			static const BOOL	Comun_MisVideos(std::wstring &StrOut);
			static const BOOL	UsuarioActual_MisVideos(std::wstring &StrOut);
			static const BOOL	Windows(std::wstring &StrOut);

			static const BOOL	ObtenerDirectorio(const GUID& rfid, std::wstring &StrOut);*/
		};

	};

#endif
