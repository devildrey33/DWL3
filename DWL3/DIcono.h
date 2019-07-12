#ifndef DICONO_H
#define DICONO_H

#include <vector>
#include <string>

namespace DWL {
	// Objeto que re-emplaza a DListaIconos 

	// Se ha diseñado para cargar no cargar 20 veces el mismo icono de forma que guarda todos los iconos en memória, 
	// y si vamos a crear un icono que ya existe nos devuelve el existente sin necesidad de crear uno nuevo.
	class DIcono {
	  protected:
		// Datos de un icono
		class DIcono_Datos {
		  public:
									DIcono_Datos(void) : Icono(NULL), ID(0), Ancho(0), Alto(0), IDStrPos(0) { };
									DIcono_Datos(HICON nIcono, const INT_PTR nID, const int nAncho, const int nAlto) : Icono(nIcono), ID(nID), Ancho(nAncho), Alto(nAlto), IDStrPos(0) { };
									DIcono_Datos(HICON nIcono, const INT_PTR nID, const int nAncho, const int nAlto, std::wstring &nIDStr, const int nIDStrPos) : Icono(nIcono), ID(nID), Ancho(nAncho), Alto(nAlto), IDStrPos(nIDStrPos), IDStr(nIDStr) { };
								   ~DIcono_Datos(void) { DestroyIcon(Icono); };
			HICON				    Icono;
			INT_PTR			        ID;			// ID del icono (positiva si es un icono de los recursos, negativa si es un icono externo o un icono KnownFolder)
			int			            Ancho;
			int				        Alto;
			int					    IDStrPos;	// Posición del icono externo (un archivo de iconos externo puede tener varios iconos dentro)
			std::wstring	        IDStr;		// Si es un icono externo ademas de la id tiene el path
		};

	  public:
												DIcono(void);
												DIcono(const INT_PTR IDRecursos, const int nAncho, const int nAlto);
												DIcono(const GUID &rfid, const int nAncho, const int nAlto);
												DIcono(const wchar_t *Path, const int nAncho, const int nAlto, const int nPosicionIco = 0);
											   ~DIcono(void);
		void									CrearIconoRecursos(const INT_PTR IDRecursos, const int nAncho, const int nAlto);
		void									CrearIconoKnownFolder(const GUID &rfid, const int nAncho, const int nAlto);
		void									CrearIconoExterno(const wchar_t *Path, const int nAncho, const int nAlto, const int nPosicionIco = 0);

		inline HICON							Icono(void)				{ return (_Icono == NULL) ? NULL	: _Icono->Icono; }
		inline const INT_PTR					ID(void)                { return (_Icono == NULL) ? 0		: _Icono->ID;    }
		inline HICON							operator()(void)		{ return (_Icono == NULL) ? 0		: _Icono->Icono; }
		inline const int						Ancho(void)             { return (_Icono == NULL) ? 0		: _Icono->Ancho; }
		inline const int						Alto(void)              { return (_Icono == NULL) ? 0		: _Icono->Alto;  }

												// Función que elimina todos los iconos en memória
		static void                             EliminarIconos(void);
	  protected:
		DIcono_Datos                          *_Icono;
												// Busca un icono KnownFolder o un icono externo
		static DIcono_Datos		     		  *_BuscarIDStr(const wchar_t *nIDStr, const int nPosicionStr);
		static std::vector <DIcono_Datos *>    _Iconos;
		static INT_PTR                         _IDNegativa; // ID negativa para los iconos KnowFolder o externos, que se va incrementando por cada uno de ellos
	};
};
#endif