#pragma once


#include "DEventoTeclado.h"
#include "DConsola.h"
#include <memory>

namespace DWL {

	// Singleton básico para contener la clase aplicación
	template<typename T> class DSingleton {
	  public:
		static T   &Instancia(void) {
						static T instancia;
						return instancia;
					};

					DSingleton(const DSingleton&) = delete;
		DSingleton &operator= (const DSingleton) = delete;

	  protected:
					DSingleton(void) {}
	};



	class DApp {
	  public :
										DApp(void);

		void							Eventos_Mirar(void);
	
		const TCHAR 				   *ObtenerSO(void);


										// Evento Tecla soltada general de todas las ventanas
		virtual void                    Evento_TeclaPresionada(DWL::DEventoTeclado& DatosTeclado)	{ };
										// Evento Tecla soltada general de todas las ventanas
		virtual void                    Evento_TeclaSoltada(DWL::DEventoTeclado& DatosTeclado)		{ };

										//! Función que devuelve el HINSTANCE de nuestra aplicación.
										/*! Esta función devuelve el HINSTANCE de nuestra aplicación.
										    \fn			inline HINSTANCE Instancia(void);
										    \return		Devuelve el HINSTANCE de nuestra aplicación.
										*/											
		inline HINSTANCE				Instancia(void)												{ return GetModuleHandle(NULL);	};

										// Ventana que muestra una consola al estilo MS-DOS para depuración
		DConsola						ConsolaDebug;
										// Función que termina el bucle de mensajes y por ende termina la aplicación
		inline void						TerminarBucleMensajes(int CodigoSalilda = 0)				{ PostQuitMessage(CodigoSalilda); }
										// Función que inicia un bucle de mensajes hasta que se llama a PostQuitMessage o App.TerminarBucleMensajes()
		virtual int						BucleMensajes(void);
	};

};

// Variable externa con la clase DApp (para uso de la DWL)
extern DWL::DApp *_Aplicacion;

// Crea un singleton de la clase aplicación especificada
#define INICIAR_DWL3(DAPP) class _DWLApp : public DWL::DSingleton<DAPP> { };
// Devuelve la instancia de la clase aplicación
#define App _DWLApp::Instancia()

//#define TERMINAR_DWL3()  _DWLApp::Eliminar();


// Fer un extern MiApp *_App al projecte dependent
// Despres fer una macro #define App (*_App) 

/*template <class TApp = DWL::DApp> TApp *DWL_Iniciar(TApp) {
	_Aplicacion = new TApp;
	return static_cast<TApp>(_Aplicacion);
}*/


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