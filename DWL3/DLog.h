#ifndef DLOG_H
	#define DLOG_H

	#include <string>

	namespace DWL {
		/* Clase base para generar logs de la aplicaci�n (Esta clase no genera ningun log, pero se puede usar DLogConsola o DLogArchivo */
		class DLog { 
		  public: //////////////////// Constructor
									DLog(void) : _UltimoTick(GetTickCount64()), _Mutex(NULL)	{ if (!_Mutex) _Mutex = CreateMutex(NULL, FALSE, TEXT("Mutex_DLog")); };
									// Destructor
								   ~DLog(void)									{ if (_Mutex) CloseHandle(_Mutex); };
									// Funci�n que crea la consola en un archivo de texto
			virtual const BOOL		Crear(const wchar_t *Path)					{ return FALSE; };
									// Funci�n que cierra el archivo)
			virtual void            Cerrar(void)								{ };
									// Funci�n que lee el stdin de la consola y lo devuelve en el par�metro Texto
//			virtual const BOOL		Leer(std::wstring& Texto)					{ return FALSE; };
									// Funci�n para escribir en la consola al estilo printf
			virtual const BOOL		Escribir(const wchar_t* Texto, ...)			{ return FALSE; };
									// Funci�n para escribir en la consola usando un std::wstring
			virtual const BOOL		Escribir(std::wstring& Texto)				{ return FALSE; };
									// Funci�n para escribir en la consola al estilo printf, que muestra los milisegundos que han pasado desde la ultima escritura en la consola
			virtual const BOOL		EscribirMS(const wchar_t* Texto, ...)		{ return FALSE; };
									// Funci�n para escribir en la consola usando un std::wstring, que muestra los milisegundos que han pasado desde la ultima escritura en la consola
			virtual const BOOL		EscribirMS(std::wstring& Texto)				{ return FALSE; };
									// Funci�n que muestra el ultimo error de windows en la consola (GetLastError)
			virtual const BOOL      MostrarUltimoError(void);
		protected: ///////////////// Ultimo tick en el que se ha escrito en la consola
			ULONGLONG			   _UltimoTick;
									// Semaforo para que las funciones sean multi hilo
			HANDLE                 _Mutex;

		};
	};

#endif