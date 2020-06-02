#ifndef DLOGCONSOLA_H
	#define DLOGCONSOLA_H

	#include "DLog.h"
	#include <string>

	namespace DWL {
		// Clase para mostrar valores por la consola (THREAD SAFE)
		class DLogConsola : public DLog {
		  public: //////////////////// Constructor
									DLogConsola(void);
									// Destructor
								   ~DLogConsola(void);
									// Función que crea la consola
			const BOOL				Crear(const wchar_t *Titulo);
									// Función para escribir en la consola al estilo printf
			const BOOL				Escribir(const wchar_t *Texto, ...);
									// Función para escribir en la consola usando un std::wstring
			const BOOL				Escribir(std::wstring &Texto);
									// Función para escribir en la consola al estilo printf, que muestra los milisegundos que han pasado desde la ultima escritura en la consola
			const BOOL				EscribirMS(const wchar_t *Texto, ...);
									// Función para escribir en la consola usando un std::wstring, que muestra los milisegundos que han pasado desde la ultima escritura en la consola
			const BOOL				EscribirMS(std::wstring &Texto);
		  protected: ///////////////// 
			HANDLE                 _Consola;
		};

	};
#endif