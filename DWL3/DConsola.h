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
									// Funci�n que crea la consola
			void					Crear(const wchar_t *Titulo);
									// Funci�n que lee el stdin de la consola y lo devuelve en el par�metro Texto
			const BOOL				Leer(std::wstring &Texto);
									// Funci�n para escribir en la consola al estilo printf
			const BOOL				Escribir(const wchar_t *Texto, ...);
									// Funci�n para escribir en la consola usando un std::wstring
			const BOOL				Escribir(std::wstring &Texto);
									// Funci�n para escribir en la consola al estilo printf, que muestra los milisegundos que han pasado desde la ultima escritura en la consola
			const BOOL				EscribirMS(const wchar_t *Texto, ...);
									// Funci�n para escribir en la consola usando un std::wstring, que muestra los milisegundos que han pasado desde la ultima escritura en la consola
			const BOOL				EscribirMS(std::wstring &Texto);
									// Funci�n que muestra el ultimo error de windows en la consola (GetLastError)
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