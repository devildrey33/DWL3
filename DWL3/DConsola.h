#ifndef DCONSOLA_H
	#define DCONSOLA_H

	#include <string>

	namespace DWL {
		// Clase para mostrar valores por la consola (THREAD SAFE)
		class DConsola {
		  public: //////////////////// Constructor
									DConsola(void);
									// Destructor
								   ~DConsola(void);
									// Función que crea la consola
			void					Crear(const wchar_t *Titulo);
									// Función que lee el stdin de la consola y lo devuelve en el parámetro Texto
			const BOOL				Leer(std::wstring &Texto);
									// Función para escribir en la consola al estilo printf
			const BOOL				Escribir(const wchar_t *Texto, ...);
									// Función para escribir en la consola usando un std::wstring
			const BOOL				Escribir(std::wstring &Texto);
									// Función para escribir en la consola al estilo printf, que muestra los milisegundos que han pasado desde la ultima escritura en la consola
			const BOOL				EscribirMS(const wchar_t *Texto, ...);
									// Función para escribir en la consola usando un std::wstring, que muestra los milisegundos que han pasado desde la ultima escritura en la consola
			const BOOL				EscribirMS(std::wstring &Texto);
									// Función que muestra el ultimo error de windows en la consola (GetLastError)
			const BOOL              MostrarUltimoError();
		  protected: ///////////////// Ultimo tick en el que se ha escrito en la consola
			ULONGLONG			   _UltimoTick;
									// Identificador de windows para la consola
			HANDLE                 _Consola;
									// Semaforo para que las funciones sean multi hilo
			HANDLE                 _Mutex;
		};

	};
#endif