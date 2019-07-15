# Devildrey33 Windows Library
Librería para facilitarme la vida con el uso del API de windows en C++

* Pensada para crear ventanas y controles personalizados con relativa facilidad.

* Controles propios que simulan el comportamiento de la mayoria de controles estándar de windows, con varios añadidos.

* Configuración de colores de los controles totalmente editable en tiempo de ejecución.


### Requisitos

* Sistema operativo Windows 7 o superior (x86. x64)

* Visual Studio 2019 o superior.

### Empezando

Para crear un proyecto utilizando la DWL3 debes seguir los siguientes pasos :

* Clonar / Descargar esta librería.

* Crear una solución con un nuevo proyecto, e agregar la DWL3 a tu proyecto.

* En las dependencias de tu proyecto debes marcar la DWL3 (Proyecto -> Dependencias del proyecto...)

* Debes configurar los directorios de inclusión de tu proyecto para añadir el directorio de la DWL3 con el código.

* Tienes que enlazar los lib, puedes hacer-lo desde las propiedades del proyecto / vinculador, o utilizando la directiva pragma de la siguiente forma :
```c++
// Archivo lib de la DWL según la configuración suponiendo que el directorio DWL y el directorio de tu proyecto comparten el mismo directorio padre.
#ifdef _DEBUG
    #if _WIN64
        #pragma comment(lib, "..\\Build\\DWL\\x64\\Debug\\DWL3.lib")
    #else
        #pragma comment(lib, "..\\Build\\DWL\\x86\\Debug\\DWL3.lib")
    #endif
#else
    #if _WIN64
        #pragma comment(lib, "..\\Build\\DWL\\x64\\Release\\DWL3.lib")
    #else
        #pragma comment(lib, "..\\Build\\DWL\\x86\\Release\\DWL3.lib")
    #endif
#endif
''''

* Necesitas crear un objeto aplicación al que despues podrás acceder desde qualquier parte del código.
```c++
#include <DApp.h>
// Paso 1 : crear un objeto que herede de DApp para que sea nuestra clase aplicación
class MiApp : public DWL::DApp {
  public :
				MiApp(void) { };
				// Por ejemplo puedes añadir tu ventana principal dentro de esta clase
	VentanaPrincipal	MiVentana;
};

''''
* Y por ultimo debes añadir el siguiente código en el MAIN :
```c++
int APIENTRY _tWinMain(_In_     HINSTANCE hInstance, 
		       _In_opt_ HINSTANCE hPrevInstance,
		       _In_     LPTSTR    lpCmdLine, 
		       _In_     int       nCmdShow) {
	// Inicio la DWL con la clase aplicación de este proyecto
	INICIAR_DWL3(MiApp);
	...
```
 
### Propósito
 
 

### FAQ
