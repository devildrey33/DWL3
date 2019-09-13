#pragma once

#include "DArbolEx.h"

#pragma comment(lib, "Mpr.lib")		// Libreria para WNetOpenEnum (necesario para explorar la red)


namespace DWL {

	#define DEXPLORARDIRECTORIOSEX_MOSTRARDEBUG	TRUE

	enum DExplorarDirectoriosEx_TipoNodo {
		DExplorarDirectoriosEx_TipoNodo_INDEFINIDO = -1,
		DExplorarDirectoriosEx_TipoNodo_Directorio,
		DExplorarDirectoriosEx_TipoNodo_Directorio_PorExplorar,
		DExplorarDirectoriosEx_TipoNodo_Escritorio,
		DExplorarDirectoriosEx_TipoNodo_MiPC,
		DExplorarDirectoriosEx_TipoNodo_MisDocumentos,
		DExplorarDirectoriosEx_TipoNodo_Red,
		DExplorarDirectoriosEx_TipoNodo_Red_PorExplorar,
		DExplorarDirectoriosEx_TipoNodo_EquipoRed,
		DExplorarDirectoriosEx_TipoNodo_DiscoDuro,
		DExplorarDirectoriosEx_TipoNodo_DiscoDuroRed, // No se usa???
		DExplorarDirectoriosEx_TipoNodo_CDROM,
		DExplorarDirectoriosEx_TipoNodo_Disquetera
	};

	class DExplorarDirectoriosEx_Nodo : public DArbolEx_Nodo {
	  public:
											DExplorarDirectoriosEx_Nodo(void) : Tipo(DExplorarDirectoriosEx_TipoNodo_INDEFINIDO) { };
		                                   ~DExplorarDirectoriosEx_Nodo(void) { };
		DExplorarDirectoriosEx_TipoNodo		Tipo;
		std::wstring                        Path;
	};


	class DExplorarDirectoriosEx : public DWL::DArbolEx {
	  public:
												DExplorarDirectoriosEx(void);
											   ~DExplorarDirectoriosEx(void);

		DExplorarDirectoriosEx_Nodo			   *AgregarEntrada(const TCHAR *nTexto, DExplorarDirectoriosEx_Nodo *nPadre = NULL, const int IconoRecursos = 0, DExplorarDirectoriosEx_TipoNodo nTipo = DExplorarDirectoriosEx_TipoNodo_INDEFINIDO, const wchar_t *nPath = L"", const BOOL nActivado = TRUE);
		DExplorarDirectoriosEx_Nodo			   *AgregarEntrada(const TCHAR *nTexto, DExplorarDirectoriosEx_Nodo *nPadre, const GUID &KnowFolderId, DExplorarDirectoriosEx_TipoNodo nTipo = DExplorarDirectoriosEx_TipoNodo_INDEFINIDO, const wchar_t *nPath = L"", const BOOL nActivado = TRUE);
		DExplorarDirectoriosEx_Nodo			   *AgregarEntrada(const TCHAR *nTexto, DExplorarDirectoriosEx_Nodo *nPadre, const wchar_t *PathIcono, const int PosIcono, DExplorarDirectoriosEx_TipoNodo nTipo = DExplorarDirectoriosEx_TipoNodo_INDEFINIDO, const wchar_t *nPath = L"", const BOOL nActivado = TRUE);
		HWND									CrearExplorarDirectoriosEx(DhWnd *nPadre, const int cX, const int cY, const int cAncho, const int cAlto, const int cID, DWORD nEstilos = WS_CHILD | WS_VISIBLE);
		void									ActualizarDirectorios(void);
												// Opciones
		BOOL									MostrarArchivosOcultos;			// Por defecto FALSE
		BOOL									MostrarRed;						// Por defecto TRUE

		const wchar_t                          *PathSeleccionado(void);
		DIcono                                 *IconoSeleccionado(void);
		inline DExplorarDirectoriosEx_Nodo     *PathResaltado(void)				{ return static_cast<DExplorarDirectoriosEx_Nodo *>(_NodoResaltado); };
		inline DExplorarDirectoriosEx_Nodo     *PathEscritorio(void)			{ return _NodoEscritorio; };
		inline DExplorarDirectoriosEx_Nodo     *PathMisDocumentos(void)			{ return _NodoMisDocumentos; };
		inline DExplorarDirectoriosEx_Nodo     *PathMiPC(void)					{ return _NodoMiPC; };
		inline DExplorarDirectoriosEx_Nodo     *PathRed(void)					{ return _NodoRed; };

		virtual const DhWnd_Tipo				TipoWnd(void) { return DhWnd_Tipo_ExplorarDirectoriosEx; };

	  protected:
//		void								   _ObtenerPathNodo(DExplorarDirectoriosEx_Nodo *nNodo, std::wstring &oPath);
		void									Evento_Nodo_Expandido(DWL::DArbolEx_Nodo *nNodo, const BOOL nExpandido);
		const BOOL						       _EnumerarRed(LPNETRESOURCE lpnr, DExplorarDirectoriosEx_Nodo *nNodo);
		const BOOL							   _EnumerarDirectorios(DExplorarDirectoriosEx_Nodo *nNodo);
#if DEXPLORARDIRECTORIOSEX_MOSTRARDEBUG == TRUE
		void							       _DEBUG_EnumerarRed(int i, LPNETRESOURCE lpnrLocal);
#endif
		const BOOL						       _TieneDirectorios(const wchar_t *nPath);

		DExplorarDirectoriosEx_Nodo			  *_NodoEscritorio;
		DExplorarDirectoriosEx_Nodo			  *_NodoMisDocumentos;
		DExplorarDirectoriosEx_Nodo			  *_NodoMiPC;
		DExplorarDirectoriosEx_Nodo			  *_NodoRed;

	};

};