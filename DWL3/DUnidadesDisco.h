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

		// Objeto que engloba una unidad de disco
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


		// Objeto que contiene todas las unidades de disco disponibles
		class DUnidadesDisco  {
          public :
											DUnidadesDisco(void);
										   ~DUnidadesDisco(void);

								            // Mira las unidades locales de la 'C' a la 'Z' evitando las disqueteras.
			const unsigned int			    Escanear_Unidades_Locales(void);
											// Función que devuelve la unidad por la posición especificada
			DUnidadDisco			       *Unidad(const size_t Pos);
											// Función que devuelve la unidad por su numero de serie especificado
			DUnidadDisco			       *Buscar_Numero_Serie(const DWORD Numero);
											// Función que devuelve la unidad por su letra especifcada
			DUnidadDisco			       *Buscar_Letra(const wchar_t Letra);
			const size_t      			    TotalUnidades(void);
          private :
  		    void							BorrarMemoria(void);

            std::vector<DUnidadDisco *>    _Unidades;
        };

    };

#endif