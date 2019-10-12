#ifndef DCONSOLA_H
	#define DCONSOLA_H

	#include <string>

	namespace DWL {

		class DConsola {
		  public:
									DConsola(void);
								   ~DConsola(void);
			void					Crear(const wchar_t *Titulo);

			const BOOL				Leer(std::wstring &Texto);
//			const BOOL				Escribir(const wchar_t *Texto);
			const BOOL				Escribir(const wchar_t *Texto, ...);
			const BOOL				Escribir(std::wstring &Texto);
//			const BOOL				EscribirMS(const wchar_t *Texto);
			const BOOL				EscribirMS(const wchar_t *Texto, ...);
			const BOOL				EscribirMS(std::wstring &Texto);

			const BOOL              MostrarUltimoError();
		protected:
			ULONGLONG			   _UltimoTick;
			HANDLE                 _Consola;
			HANDLE                 _Mutex;
		};

	};
#endif