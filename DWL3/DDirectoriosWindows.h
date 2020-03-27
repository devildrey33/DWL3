#ifndef DDIRECTORIOSWINDOWS_H
	#define DDIRECTORIOSWINDOWS_H

	#include <string>
	#include <Shlobj.h>

	namespace DWL {

		class DDirectoriosWindows {
		  public : /////////////// Constructor
								DDirectoriosWindows(void) { };
								// Destructor
							   ~DDirectoriosWindows(void) { };
								// Función que obtiene el directorio AppData para todos los usuarios
			static const BOOL	Comun_AppData(std::wstring &StrOut)							{ return ObtenerDirectorio(FOLDERID_ProgramData, StrOut);		};
								// Función que obtiene el directorio AppData del usuario actual
			static const BOOL	UsuarioActual_AppData(std::wstring &StrOut)					{ return ObtenerDirectorio(FOLDERID_RoamingAppData, StrOut);	};
								// Función que obtiene el directorio Escritorio para todos los usuarios
			static const BOOL	Comun_Escritorio(std::wstring &StrOut)						{ return ObtenerDirectorio(FOLDERID_PublicDesktop, StrOut);		};
								// Función que obtiene el directorio Escritorio del usuario actual 
			static const BOOL	UsuarioActual_Escritorio(std::wstring &StrOut)				{ return ObtenerDirectorio(FOLDERID_Desktop, StrOut);			};
								// Función que obtiene el directorio del menu de inicio para todos los usuarios
			static const BOOL	Comun_MenuInicio_Programas(std::wstring &StrOut)			{ return ObtenerDirectorio(FOLDERID_CommonPrograms, StrOut);	};
								// Función que obtiene el directorio del menu de inicio del usuario actual
			static const BOOL	UsuarioActual_MenuInicio_Programas(std::wstring &StrOut)	{ return ObtenerDirectorio(FOLDERID_Programs, StrOut);			};
								// Función que obtiene el directorio Mis Documentos para todos los usuarios
			static const BOOL	Comun_MisDocumentos(std::wstring &StrOut)					{ return ObtenerDirectorio(FOLDERID_PublicDocuments, StrOut);	};
								// Función que obtiene el directorio Mis Documentos del usuario actual
			static const BOOL	UsuarioActual_MisDocumentos(std::wstring &StrOut)			{ return ObtenerDirectorio(FOLDERID_Documents, StrOut);			};
								// Función que obtiene el directorio Mis Imagenes para todos los usuarios
			static const BOOL	Comun_MisImagenes(std::wstring &StrOut)						{ return ObtenerDirectorio(FOLDERID_PublicPictures, StrOut);	};
								// Función que obtiene el directorio Mis Imagenes del usuario actual
			static const BOOL	UsuarioActual_MisImagenes(std::wstring &StrOut)				{ return ObtenerDirectorio(FOLDERID_Pictures, StrOut);			};
								// Función que obtiene el directorio Mi Musica para todos los usuarios
			static const BOOL	Comun_MiMusica(std::wstring &StrOut)						{ return ObtenerDirectorio(FOLDERID_PublicMusic, StrOut);		};
								// Función que obtiene el directorio Mi Musica del usuario actual
			static const BOOL	UsuarioActual_MiMusica(std::wstring &StrOut)				{ return ObtenerDirectorio(FOLDERID_Music, StrOut);				};
								// Función que obtiene el directorio Mis Videos para todos los usuarios
			static const BOOL	Comun_MisVideos(std::wstring &StrOut)						{ return ObtenerDirectorio(FOLDERID_PublicVideos, StrOut);		};
								// Función que obtiene el directorio Mis Videos del usuario actual
			static const BOOL	UsuarioActual_MisVideos(std::wstring &StrOut)				{ return ObtenerDirectorio(FOLDERID_Videos, StrOut);			};
								// Función que obtiene el directorio de Windows
			static const BOOL	Windows(std::wstring &StrOut)								{ return ObtenerDirectorio(FOLDERID_Windows, StrOut);			};
								// Función que obtiene el directorio especificado en rfid
			static const BOOL	ObtenerDirectorio(const GUID &rfid, std::wstring &StrOut) {
				PWSTR Tmp = NULL;
				if (S_OK == SHGetKnownFolderPath(rfid, NULL, 0, &Tmp)) {
					StrOut = Tmp;
					CoTaskMemFree(Tmp);
					return TRUE;
				}
				return FALSE;
			}

		};

	};

#endif
