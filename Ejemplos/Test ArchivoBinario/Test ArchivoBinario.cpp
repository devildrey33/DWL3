// Test ArchivoBinario.cpp : Define el punto de entrada de la aplicación.
//

#include "framework.h"
#include "Test ArchivoBinario.h"
#include <DDirectoriosWindows.h>
#include <DArchivoBinario.h>



int APIENTRY _tWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPTSTR lpCmdLine, _In_ int nCmdShow) {
	// Inicio la DWL con la clase aplicación por defecto
	INICIAR_DWL3(DWL::DApp);
	
	// Creo la consola de depuración
//	App.ConsolaDebug.Crear(L"DEBUG");

	// Construyo el string "c:\\ProgramData\\Archivo.test"
	std::wstring PathArchivo;
	DWL::DDirectoriosWindows::Comun_AppData(PathArchivo);
	PathArchivo += L"\\Archivo.test";

	// Abro el archivo para escritura
	DWL::DArchivoBinario Archivo;
	Archivo.AbrirArchivoEscritura(PathArchivo.c_str());

	// Testos de prueba de 8 y 16 bits
	std::string  TestString	 = "Texto para testear la std::string";
	std::wstring TestWString = L"Texto para testear la std::wstring";

	// Guardo un std::string con su tamaño dentro del binario
	Archivo.GuardarString(TestString, TRUE);

	// Guardo un std::wstring con su tamaño dentro del binario
	Archivo.GuardarString(TestWString, TRUE);

	// Guardo un valor long
	long Valor = -33;
	Archivo.Guardar<long>(&Valor);


	// Cierro el archivo de escritura
	Archivo.Cerrar();

	Archivo.AbrirArchivoLectura(PathArchivo.c_str());
	
	// Strings para leer los datos
	std::string  RetTestString;
	std::wstring RetTestWString;

	// Leo los strings
	Archivo.LeerString(RetTestString);
	Archivo.LeerString(RetTestWString);

	// Leo el long
	long RetValor = 0;
	Archivo.Leer<long>(&RetValor);

	// Cierro el archivo de lectura
	Archivo.Cerrar();

	// Borro el archivo binario de prueba
	DeleteFile(PathArchivo.c_str());

	std::wstring Texto;
	if (TestString.compare(RetTestString) == 0)		Texto = L"Test std::string correcto.\n";
	else											Texto = L"Error en el test std::string\n";
	if (TestWString.compare(RetTestWString) == 0)	Texto += L"Test std::wstring correcto.\n";
	else											Texto += L"Error en el test std::wstring\n";
	if (Valor == RetValor)							Texto += L"Test del valor long correcto";
	else											Texto += L"Error en el test del valor long";

	MessageBox(NULL, Texto.c_str(), L"Test terminado", MB_OK);

	return 0;
}


