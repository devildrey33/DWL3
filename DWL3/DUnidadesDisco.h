#ifndef DUNIDADESDISCO_H
    #define DUNIDADESDISCO_H

//	#include "DObjeto.h"
    #include <string>
	#include <vector>

    namespace DWL {
		// Tipos de unidad
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
          public: //////////////////////////// Constructor
											DUnidadDisco(void);
											// Constructor con path
											DUnidadDisco(TCHAR *nPath);
											// Destructor
										   ~DUnidadDisco(void);
										    // Función que obtiene la unidad partiendo del path
			BOOL					        Obtener(const TCHAR *nPath);
											// Función que devuelve si la unidad es de solo lectura
			const BOOL				        Solo_Lectura(void);
											// Función que devuelve el número de serie de la unidad
			const DWORD				        Numero_Serie(void);
											// Función que devuelve el máximo de caracteres que puede tener un nombre de archivo en esta unidad
			const DWORD				        Longitud_Componente(void);
											// Función que devuelve el numero de serie de la unidad
			const TCHAR			           *Nombre_Volumen(void);
											// Función que devuelve el tipo de partición de la uinidad
			const TCHAR			           *Tipo_Particion(void);
							                // Retorna la letra de la unidad en mayuscula
			const TCHAR				        Letra(void);
											// Función que devuelve el tipo de unidad
			const DEnum_UnidadDisco_Tipo	Tipo_Disco(void);
          private: /////////////////////////// String con el nombre de la unidad
            std::wstring                   _Nombre_Volumen;
											// String con el tipo de partición utilizada en la unidad
			std::wstring       	           _Tipo_Particion;
											// Numero de serie de la unidad
            DWORD			        	   _Numero_Serie;
			                                // Máximo de caracteres para nombres de archivo en la unidad
            DWORD			        	   _Longitud_Componente;
											// Otras opciones
            DWORD			        	   _Flags;
											// Letra de la unidad
            TCHAR				           _Letra;
											// Tipo de unidad
            DEnum_UnidadDisco_Tipo         _Tipo_Disco;
        };


		// Objeto que contiene todas las unidades de disco disponibles
		class DUnidadesDisco  {
          public : /////////////////////////// Constructor
											DUnidadesDisco(void);
											// Destructor
										   ~DUnidadesDisco(void);
								            // Mira las unidades locales de la 'C' a la 'Z' evitando las disqueteras.
			const unsigned int			    Escanear_Unidades_Locales(void);
											// Función que devuelve la unidad por la posición especificada
			DUnidadDisco			       *Unidad(const size_t Pos);
											// Función que devuelve la unidad por su numero de serie especificado
			DUnidadDisco			       *Buscar_Numero_Serie(const DWORD Numero);
											// Función que devuelve la unidad por su letra especifcada
			DUnidadDisco			       *Buscar_Letra(const wchar_t Letra);
											// Función que devuelve el total de unidades disponibles
			const size_t      			    TotalUnidades(void);
          private : ////////////////////////// Función que elimina las unidades de memória
  		    void							BorrarMemoria(void);
											// Vector de unidades disponibles
            std::vector<DUnidadDisco *>    _Unidades;
        };

    };

#endif