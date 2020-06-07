#ifndef DLOG_H
	#define DLOG_H

	#include <string>

	namespace DWL {
		/* Clase base para generar logs de la aplicación (Esta clase no genera ningun log, pero se puede usar DLogConsola o DLogArchivo */
		class DLog { 
		  public: //////////////////// Constructor
									DLog(void) : _UltimoTick(GetTickCount64()), _Mutex(NULL)	{ if (!_Mutex) _Mutex = CreateMutex(NULL, FALSE, TEXT("Mutex_DLog")); };
									// Destructor
								   ~DLog(void)									{ if (_Mutex) CloseHandle(_Mutex); };
									// Función que crea la consola en un archivo de texto
			virtual const BOOL		Crear(const wchar_t *Path)					{ return FALSE; };
									// Función que cierra el log
			virtual void            Cerrar(void)								{ };
									// Función que lee el stdin de la consola y lo devuelve en el parámetro Texto
//			virtual const BOOL		Leer(std::wstring& Texto)					{ return FALSE; };
									// Función para escribir en el log al estilo printf
			virtual const BOOL		Escribir(const wchar_t* Texto, ...)			{ return FALSE; };
									// Función para escribir en el log usando un std::wstring
			virtual const BOOL		Escribir(std::wstring& Texto)				{ return FALSE; };
									// Función para escribir en el log al estilo printf, que muestra los milisegundos que han pasado desde la ultima escritura en el log
			virtual const BOOL		EscribirMS(const wchar_t* Texto, ...)		{ return FALSE; };
									// Función para escribir en el log usando un std::wstring, que muestra los milisegundos que han pasado desde la ultima escritura en el log
			virtual const BOOL		EscribirMS(std::wstring& Texto)				{ return FALSE; };
									// Función que muestra el ultimo error de windows en el log (GetLastError)
			virtual const BOOL      MostrarUltimoError(void);
		protected: ///////////////// Ultimo tick en el que se ha escrito en el log
			ULONGLONG			   _UltimoTick;
									// Semaforo para que las funciones sean multi hilo
			HANDLE                 _Mutex;

		};
	};

#endif