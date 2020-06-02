#ifndef DLOGARCHIVO_H
	#define DLOGARCHIVO_H

	#include "DLog.h"
	#include <string>

	namespace DWL {
		/* Clase que simula una consola en un archivo (para las macros Debug_ */
		class DLogArchivo : public DLog {
		  public:  /////////////////// Constructor
									DLogArchivo(void);
									// Destructor
			                       ~DLogArchivo(void);
									// Funci�n que crea la consola en un archivo de texto
			const BOOL				Crear(const wchar_t* Path);
									// Funci�n que cierra el archivo)
			void                    Cerrar(void);
									// Funci�n para escribir en la consola al estilo printf
			const BOOL				Escribir(const wchar_t* Texto, ...);
									// Funci�n para escribir en la consola usando un std::wstring
			const BOOL				Escribir(std::wstring& Texto);
									// Funci�n para escribir en la consola al estilo printf, que muestra los milisegundos que han pasado desde la ultima escritura en la consola
			const BOOL				EscribirMS(const wchar_t* Texto, ...);
									// Funci�n para escribir en la consola usando un std::wstring, que muestra los milisegundos que han pasado desde la ultima escritura en la consola
			const BOOL				EscribirMS(std::wstring& Texto);
		  protected: ///////////////// 
			HANDLE                 _Archivo;

		};
	};

#endif