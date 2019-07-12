#ifndef DUNIDADESDISCO_H
    #define DUNIDADESDISCO_H

//	#include "DObjeto.h"
    #include <string>
	#include <vector>

    namespace DWL {

        enum DEnum_UnidadDisco_Tipo {
	        DEnum_UnidadDisco_Tipo_Desconocido		= 0,
	        DEnum_UnidadDisco_Tipo_Path_Invalido	= 1,
	        DEnum_UnidadDisco_Tipo_Extraible		= 2,
	        DEnum_UnidadDisco_Tipo_Fijo				= 3,
	        DEnum_UnidadDisco_Tipo_Remoto			= 4,
	        DEnum_UnidadDisco_Tipo_CDRom			= 5,
	        DEnum_UnidadDisco_Tipo_RamDisk			= 6
        };

        class DUnidadDisco  {
          public:
			//virtual const DEnum_Objeto		Objeto_Tipo(void) { return DEnum_Objeto_UnidadDisco; };
											DUnidadDisco(void);
											DUnidadDisco(TCHAR *nPath);
										   ~DUnidadDisco(void);

			BOOL					        Obtener(const TCHAR *nPath);
			const BOOL				        Solo_Lectura(void);
			const DWORD				        Numero_Serie(void);
			const DWORD				        Longitud_Componente(void);
			const TCHAR			           *Nombre_Volumen(void);
			const TCHAR			           *Tipo_Particion(void);
							                // Retorna la letra de la unidad en mayuscula
			const TCHAR				        Letra(void);
			const DEnum_UnidadDisco_Tipo	Tipo_Disco(void);
          private:
            std::wstring                   _Nombre_Volumen;
			std::wstring       	           _Tipo_Particion;
            DWORD			        	   _Numero_Serie;
            DWORD			        	   _Longitud_Componente;
            DWORD			        	   _Flags;
            TCHAR				           _Letra;
            DEnum_UnidadDisco_Tipo         _Tipo_Disco;
        };



		class DUnidadesDisco  {
          public :
//			virtual const DEnum_Objeto		Objeto_Tipo(void) { return DEnum_Objeto_UnidadesDisco; };
											DUnidadesDisco(void);
										   ~DUnidadesDisco(void);

								            // Mira las unidades locales de la 'C' a la 'Z' evitando las disqueteras.
			const unsigned int			    Escanear_Unidades_Locales(void);
			DUnidadDisco			       *Unidad(const size_t Pos);
			DUnidadDisco			       *Buscar_Numero_Serie(const DWORD Numero);
			DUnidadDisco			       *Buscar_Letra(const TCHAR Letra);
			const size_t      			    TotalUnidades(void);
          private :
  		    void							BorrarMemoria(void);

            std::vector<DUnidadDisco *>    _Unidades;
        };

    };

#endif