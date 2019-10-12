#pragma once


#include "DEventoTeclado.h"
#include "DConsola.h"
#include <memory>

namespace DWL {

	class DApp;

	// Singleton básico para contener la clase aplicación que hereda de DApp
	template<class T = DApp> class DSingletonApp {
	  public: //////////// Función que crea una instancia estática y la devuelve (al ser estatica la instancia solo se crea la pirmera vez que se accede a esta función)
		static T	   &Instancia(void) {
							static T instancia;
							return instancia;
						};
						// Elimino el constructor copia
						DSingletonApp(const DSingletonApp &) = delete;
						// Elimino el operador =
		DSingletonApp  &operator= (const DSingletonApp) = delete;
	  private: /////////// Constructor por defecto des-habilitado
						DSingletonApp(void) { }
	};

	class DVentana;
	class DMarcoScrollEx;
	class DMarcoScrollEx_Marco;
	class DMarcoScrollEx_Pagina;

	class DApp : protected DhWnd {
	  public : /////////////////////////// Constructor
										DApp(void);
										// Función que vacia la cola de mensajes de esta aplicación
		void							Eventos_Mirar(void);
										// Función que obtiene el sistema operativo actual y lo devuelve en un string
		const wchar_t 				   *ObtenerSO(void);
										// Evento Tecla soltada general de todas las ventanas
		virtual void                    Evento_TeclaPresionada(DWL::DEventoTeclado& DatosTeclado)	{ };
										// Evento Tecla soltada general de todas las ventanas
		virtual void                    Evento_TeclaSoltada(DWL::DEventoTeclado& DatosTeclado)		{ };
										// Función que devuelve la instancia de esta aplicación										
		inline HINSTANCE				Instancia(void)												{ return GetModuleHandle(NULL);	};
										// Ventana que muestra una consola al estilo MS-DOS para depuración
		DConsola						ConsolaDebug;
										// Función que termina el bucle de mensajes y por ende termina la aplicación
		inline void						TerminarBucleMensajes(int CodigoSalida = 0)					{ PostQuitMessage(CodigoSalida); }
										// Función que inicia un bucle de mensajes hasta que se llama a PostQuitMessage o App.TerminarBucleMensajes()
		virtual int						BucleMensajes(void);
										// Función que devuelve la linea de comandos especificada (si especificas una posición inválida se devolvera la linea 0)
		inline std::wstring			   &LineaComandos(const size_t Pos)								{ return (Pos >= _LineaComandos.size()) ? _LineaComandos[0] :  _LineaComandos[Pos]; }
										// Función que devuelve el total de comandos introducidos al arrancar la aplicación
		inline const size_t				TotalLineaComandos(void)									{ return _LineaComandos.size(); };
										// Función que devuelve el path completo de la aplicación sin el ejecutable
		std::wstring				   &Path(const BOOL SinEjecutable = TRUE);
										// Función que devuelve el path actual del sistema
		std::wstring                   &PathActual(void);
										// Función que asigna el path del sistema donde se está ejecutando la aplicación 
		const BOOL						PathActual(const wchar_t *Path);
										// Función que asigna el path del sistema donde se está ejecutando la aplicación 
		const BOOL						PathActual(std::wstring &Path);
										// Obtiene la descripción del ultimo error de windows
		std::wstring                   &UltimoError(void);
										// Gestor de mensajes virtual para la ventana invisible de mensajes
		virtual LRESULT CALLBACK		GestorMensajes(UINT uMsg, WPARAM wParam, LPARAM lParam)		{ return DefWindowProc(_hWnd, uMsg, wParam, lParam); };
										// Función que devuelve el HWND de la ventana para los mensajes multi thread
		inline HWND						hWndMensajes(void)											{ return _hWnd; }
 	  protected: ///////////////////////// Función que obtiene la linea de comandos y la almacena en el vector _LineaComandos
		void                           _ObtenerLineaComandos(void);
										// Gestor de mensajes predeterminado para la DWL
		LRESULT CALLBACK		       _GestorMensajes(UINT uMsg, WPARAM wParam, LPARAM lParam);
										// Función que crea una ventana invisible para manejar mensajes de multiples threads
		void						   _CrearVentanaMensajes(void);
										// Gestor de mensajes estatico inicial
		static LRESULT CALLBACK        _GestorMensajesEstatico(HWND HandleVentana, UINT uMsg, WPARAM wParam, LPARAM lParam);
										// Función para eliminar peticiones de descarga
		void						   _Internet_Eliminar(const UINT nID);
										// Función para los errores en una petición de descarga
		void						   _Internet_Error(const UINT nError, const UINT nID);
										// Función que recibe el porcentaje de la descarga
		void						   _Internet_Porcentaje(const float nValor, const UINT nID);
										// Función que indica que la descarga ha finalizado correctamente
		void						   _Internet_Terminado(const UINT nID);
										// Función que inicia el set_se_translator para poder hacer catch en todas las excepciones (0xC0000005 : Access Violation) en especial
		void						   _Catch_Exceptions(void);
										// Color para le fondo de las ventanas
		HBRUSH                         _ColorFondoVentana;
										// Vector que contiene la linea de comandos separada
		std::vector<std::wstring>	   _LineaComandos;

		typedef unsigned int exception_code_t;
										// Funcion que devuelve el texto de la despreición de una excepción
		static const char             *_seDescripcion(const exception_code_t& code);
										// Función que devuelve el texto de la la operación ejecutada durante una excepción
		static const char             *_opDescripcion(const ULONG opcode);
										// Función que convierte una excepción a string ANSI / UTF-8
		static std::string			   _ErrorStr(struct _EXCEPTION_POINTERS* ep, bool has_exception_code = false, exception_code_t code = 0);

		friend class DVentana;
		friend class DMarcoScrollEx;
		friend class DMarcoScrollEx_Marco;
		friend class DMarcoScrollEx_Pagina;
	};

};

// Variable externa con la clase DApp (para uso interno de la DWL, si necesitas acceder al objeto aplicación utiliza la macro App)
extern DWL::DApp *_Aplicacion;

// Crea un singleton de la clase aplicación especificada
#define INICIAR_DWL3(DAPP) class _DWLApp : public DWL::DSingletonApp<DAPP> { };

// Devuelve la instancia de la clase aplicación
#define App _DWLApp::Instancia()



#ifdef DWL_MOSTRAR_CONSOLA 
	// Macros para escribir datos en la consola de depuración 
	// Macro que escribe datos con argumentos variables (al estilo printf), sin mostrar milisegundos (OJO! REQUIERE TERMINAR LA LÍNEA CON '\n')
	#define Debug_EscribirSinMS_Varg(TEXTO, ...)	_Aplicacion->ConsolaDebug.Escribir(TEXTO, __VA_ARGS__);
	// Macro que escribe datos con argumentos variables (al estilo printf), mostrando los milisegundos desde el último Debug_Escribir (OJO! REQUIERE TERMINAR LA LÍNEA CON '\n')
	#define Debug_Escribir_Varg(TEXTO, ...)			_Aplicacion->ConsolaDebug.EscribirMS(TEXTO, __VA_ARGS__);
	// Macro que escribe datos, sin mostrar milisegundos (OJO! REQUIERE TERMINAR LA LÍNEA CON '\n')
	#define Debug_EscribirSinMS(TEXTO)				_Aplicacion->ConsolaDebug.Escribir(TEXTO);
	// Macro que escribe datos, sin mostrar milisegundos, mostrando los milisegundos desde el último Debug_Escribir (OJO! REQUIERE TERMINAR LA LÍNEA CON '\n')
	#define Debug_Escribir(TEXTO)					_Aplicacion->ConsolaDebug.EscribirMS(TEXTO);
	// WUT??
	#define Debug_Leer(TEXTO)						App.ConsolaDebug.Leer(TEXTO);
	// Marco que muestra si la función WINAPI tiene un error por la consola (GetLastError)
	#define Debug_MostrarUltimoError()				_Aplicacion->ConsolaDebug.MostrarUltimoError()
	#define Debug_UltimoError(NUM)					SetLastError(NUM)
#else
	#define Debug_EscribirSinMS_Varg(TEXTO, ...)	
	#define Debug_Escribir_Varg(TEXTO, ...)		
	#define Debug_EscribirSinMS(TEXTO)				
	#define Debug_Escribir(TEXTO)				
	#define Debug_Leer(TEXTO)					
	// Marco que muestra si la función WINAPI tiene un error por la consola (GetLastError)
	#define Debug_MostrarUltimoError()				
	#define Debug_UltimoError(NUM)					SetLastError(NUM);	
#endif


/* --- PRINTF_BYTE_TO_BINARY macro's --- */
#define PRINTF_BINARY_PATTERN_INT8 "%c%c%c%c%c%c%c%c"
#define PRINTF_BYTE_TO_BINARY_INT8(i)   (((i) & 0x80ll) ? '1' : '0'), \
										(((i) & 0x40ll) ? '1' : '0'), \
										(((i) & 0x20ll) ? '1' : '0'), \
										(((i) & 0x10ll) ? '1' : '0'), \
										(((i) & 0x08ll) ? '1' : '0'), \
										(((i) & 0x04ll) ? '1' : '0'), \
										(((i) & 0x02ll) ? '1' : '0'), \
										(((i) & 0x01ll) ? '1' : '0')

#define PRINTF_BINARY_PATTERN_INT16			PRINTF_BINARY_PATTERN_INT8              PRINTF_BINARY_PATTERN_INT8
#define PRINTF_BYTE_TO_BINARY_INT16(i)		PRINTF_BYTE_TO_BINARY_INT8((i) >> 8),   PRINTF_BYTE_TO_BINARY_INT8(i)
#define PRINTF_BINARY_PATTERN_INT32			PRINTF_BINARY_PATTERN_INT16             PRINTF_BINARY_PATTERN_INT16
#define PRINTF_BYTE_TO_BINARY_INT32(i)		PRINTF_BYTE_TO_BINARY_INT16((i) >> 16), PRINTF_BYTE_TO_BINARY_INT16(i)
#define PRINTF_BINARY_PATTERN_INT64			PRINTF_BINARY_PATTERN_INT32             PRINTF_BINARY_PATTERN_INT32
#define PRINTF_BYTE_TO_BINARY_INT64(i)		PRINTF_BYTE_TO_BINARY_INT32((i) >> 32), PRINTF_BYTE_TO_BINARY_INT32(i)
/* --- end macros --- */