#ifndef DICONO_H
#define DICONO_H

#include <vector>
#include <string>

namespace DWL {
	// Objeto que re-emplaza a DListaIconos 

	// Se ha diseñado para no cargar 20 veces el mismo icono de forma que guarda todos los iconos en memória, 
	// y si vamos a crear un icono que ya existe nos devuelve el existente sin necesidad de crear uno nuevo.

	// Los iconos se borrarán al terminar la aplicación (NO ES MULTITHREAD ya que en teoría todo el tema del GDI se debería llevar desde el hilo principal)
	class DIcono {
	  protected:
		// Datos de un icono
		class DIcono_Datos {
		  public: //////////////////// Constructor por defecto
									DIcono_Datos(void) : Icono(nullptr), ID(0), Ancho(0), Alto(0), IDStrPos(0) { };
									// Constructor para un icono de los recurosos
									DIcono_Datos(HICON nIcono, const INT_PTR nID, const int nAncho, const int nAlto) : Icono(nIcono), ID(nID), Ancho(nAncho), Alto(nAlto), IDStrPos(0) { };
									// Constructor para un icono externo
									DIcono_Datos(HICON nIcono, const INT_PTR nID, const int nAncho, const int nAlto, std::wstring &nIDStr, const int nIDStrPos) : Icono(nIcono), ID(nID), Ancho(nAncho), Alto(nAlto), IDStrPos(nIDStrPos), IDStr(nIDStr) { };
									// Destructor
								   ~DIcono_Datos(void) { DestroyIcon(Icono); };
									// Identificador de windows para el icono
			HICON				    Icono;
									// ID del icono (positiva si es un icono de los recursos, negativa si es un icono externo o un icono KnownFolder)
			INT_PTR			        ID;			
									// Ancho en pixeles del icono
			int			            Ancho;
									// Altura en pixeles del icono
			int				        Alto;
									// Posición del icono externo (un archivo de iconos externo puede tener varios iconos dentro)
			int					    IDStrPos;	
									// Si es un icono externo ademas de la id tiene el path
			std::wstring	        IDStr;		
		};

	  public: //////////////////////////////////// Constructor
												DIcono(void);
												// Constructor para un icono de los recursos
												DIcono(const INT_PTR IDRecursos, const int nAncho, const int nAlto);
												// Constructor para un icono de windows
												DIcono(const GUID &rfid, const int nAncho, const int nAlto);
												// Constructor para un icono externo
												DIcono(const wchar_t *Path, const int nAncho, const int nAlto, const int nPosicionIco = 0);
												// Destructor
											   ~DIcono(void);
												// Función que crea un icono de los recursos
		void									CrearIconoRecursos(const INT_PTR IDRecursos, const int nAncho, const int nAlto);
												// Función que crea un icono de una carpeta conocida de widnows
		void									CrearIconoKnownFolder(const GUID &rfid, const int nAncho, const int nAlto);
												// Función que crea un icono externo
		void									CrearIconoExterno(const wchar_t *Path, const int nAncho, const int nAlto, const int nPosicionIco = 0);
												// Funciones que devuelven las propiedades del icono
		inline HICON							Icono(void)				{ return (_Icono == nullptr) ? nullptr	: _Icono->Icono; }
		inline const INT_PTR					ID(void)                { return (_Icono == nullptr) ? 0		: _Icono->ID;    }
		inline HICON							operator()(void)		{ return (_Icono == nullptr) ? nullptr  : _Icono->Icono; }
		inline const int						Ancho(void)             { return (_Icono == nullptr) ? 0		: _Icono->Ancho; }
		inline const int						Alto(void)              { return (_Icono == nullptr) ? 0		: _Icono->Alto;  }

												// Función que elimina todos los iconos en memória
		static void                             EliminarIconos(void);
	  protected: ///////////////////////////////// Datos del icono
		DIcono_Datos                          *_Icono;
												// Busca un icono KnownFolder o un icono externo
		static DIcono_Datos		     		  *_BuscarIDStr(const wchar_t *nIDStr, const int nPosicionStr);
												// Vector para los iconos
		static std::vector <DIcono_Datos *>    _Iconos;
												// ID negativa para los iconos KnowFolder o externos, que se va incrementando por cada uno de ellos
		static INT_PTR                         _IDNegativa; 
	};
};
#endif