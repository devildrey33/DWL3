#pragma once

#include "resource.h"
#include <DApp.h>

// Archivo lib de la DWL según la configuración
#ifdef _DEBUG
	#if _WIN64
		#pragma comment(lib, "..\\..\\Build\\DWL\\x64\\Debug\\DWL3.lib")
	#else
		#pragma comment(lib, "..\\..\\Build\\DWL\\x86\\Debug\\DWL3.lib")
	#endif
#else
	#if _WIN64
		#pragma comment(lib, "..\\..\\Build\\DWL\\x64\\Release\\DWL3.lib")
	#else
		#pragma comment(lib, "..\\..\\Build\\DWL\\x86\\Release\\DWL3.lib")
	#endif
#endif
