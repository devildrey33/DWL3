#include "pch.h"
#include "DAnimacion.h"

namespace DWL {
	
	const double DAnimacion::FuncionesTiempo::PI = 3.14159265359f;

/*	 _____                _                      _
	|  __ \   /\         (_)                    (_)            
	| |  | | /  \   _ __  _ _ __ ___   __ _  ___ _  ___  _ __  
	| |  | |/ /\ \ | '_ \| | '_ ` _ \ / _` |/ __| |/ _ \| '_ \ 
	| |__| / ____ \| | | | | | | | | | (_| | (__| | (_) | | | |
	|_____/_/    \_\_| |_|_|_| |_| |_|\__,_|\___|_|\___/|_| |_|
*/

	DAnimacion::DAnimacion(void) : _Timer(NULL), _TickInicio(NULL), _Duracion(0), _TiempoActual(0) {
	}
	

	DAnimacion::~DAnimacion(void) {
		Terminar();
	}

	void DAnimacion::Iniciar(const COLORREF Desde0, const COLORREF Hasta0, const DWORD Milisegundos, std::function<void(Valores &, const BOOL)> LambdaCallback, FuncionTiempo Funcion) {
		Datos nDatos;
		nDatos.AgregarRGB(Desde0, Hasta0, Funcion);
		Iniciar(nDatos, Milisegundos, LambdaCallback);
	}

	void DAnimacion::Iniciar(const COLORREF Desde0, const COLORREF Hasta0, const COLORREF Desde1, const COLORREF Hasta1, const DWORD Milisegundos, std::function<void(Valores &, const BOOL)> LambdaCallback, FuncionTiempo Funcion) {
		Datos nDatos;
		nDatos.AgregarRGB(Desde0, Hasta0, Funcion);
		nDatos.AgregarRGB(Desde1, Hasta1, Funcion);
		Iniciar(nDatos, Milisegundos, LambdaCallback);
	}

	void DAnimacion::Iniciar(const COLORREF Desde0, const COLORREF Hasta0, const COLORREF Desde1, const COLORREF Hasta1, const COLORREF Desde2, const COLORREF Hasta2, const DWORD Milisegundos, std::function<void(Valores &, const BOOL)> LambdaCallback, FuncionTiempo Funcion) {
		Datos nDatos;
		nDatos.AgregarRGB(Desde0, Hasta0, Funcion);
		nDatos.AgregarRGB(Desde1, Hasta1, Funcion);
		nDatos.AgregarRGB(Desde2, Hasta2, Funcion);
		Iniciar(nDatos, Milisegundos, LambdaCallback);
	}

	void DAnimacion::Iniciar(const COLORREF Desde0, const COLORREF Hasta0, const COLORREF Desde1, const COLORREF Hasta1, const COLORREF Desde2, const COLORREF Hasta2, const COLORREF Desde3, const COLORREF Hasta3, const DWORD Milisegundos, std::function<void(Valores &, const BOOL)> LambdaCallback, FuncionTiempo Funcion) {
		Datos nDatos;
		nDatos.AgregarRGB(Desde0, Hasta0, Funcion);
		nDatos.AgregarRGB(Desde1, Hasta1, Funcion);
		nDatos.AgregarRGB(Desde2, Hasta2, Funcion);
		nDatos.AgregarRGB(Desde3, Hasta3, Funcion);
		Iniciar(nDatos, Milisegundos, LambdaCallback);
	}

	void DAnimacion::Iniciar(const double Desde, const double Hasta, const DWORD Milisegundos, std::function<void(Valores &, const BOOL)> LambdaCallback, FuncionTiempo Funcion, const DWORD Intervalo) {
		Datos nDatos;
		nDatos.AgregarDecimal(Desde, Hasta, Funcion);
		Iniciar(nDatos, Milisegundos, LambdaCallback, Intervalo);
	}

	void DAnimacion::Iniciar(const double Desde0, const double Hasta0, const double Desde1, const double Hasta1, const DWORD Milisegundos, std::function<void(Valores &, const BOOL)> LambdaCallback, FuncionTiempo Funcion) {
		Datos nDatos;
		nDatos.AgregarDecimal(Desde0, Hasta0, Funcion);
		nDatos.AgregarDecimal(Desde1, Hasta1, Funcion);
		Iniciar(nDatos, Milisegundos, LambdaCallback);
	}

	void DAnimacion::Iniciar(Datos &Datos, const DWORD Milisegundos, std::function<void(Valores &, const BOOL)> LambdaCallback, const DWORD Intervalo) {
		// Compruebo que los valores Desde sean distintos a los valores Hasta
		BOOL Iguales = TRUE;
		for (size_t i = 0; i < Datos.Total(); i++) {
			if (Datos[i].Desde != Datos[i].Hasta) {
				Iguales = FALSE;
				break;
			}
		}
		// No es necesario crear la animación, todos los valores Desde son iguales a sus valores Hasta
		if (Iguales == TRUE) return;

		if (_Timer != NULL) {
			#if DANIMACION_MOSTRARDEBUG == TRUE
				Debug_Escribir(L"DAnimacion::Iniciar -> ERROR la animación ya se está ejecutando!\n");
			#endif
			return;
		}
		// Copio los valores iniciales en el vector de valores (Si se hace invertir y no ha pasado ni una iteración los valores no existen)
		_Valores.Resize(Datos.Total());
		for (size_t i = 0; i < Datos.Total(); i++) {
			_Valores[i] = Datos[i].Desde;
		}

		_Datos			= Datos;
		_Callback		= LambdaCallback;
		_Duracion		= Milisegundos;
		_TickInicio     = GetTickCount();
		_TiempoActual	= 0;
		BOOL Ret = CreateTimerQueueTimer(&_Timer, NULL, reinterpret_cast<WAITORTIMERCALLBACK>(_TimerProc), this, Intervalo, Intervalo, WT_EXECUTEINTIMERTHREAD);
		#if DANIMACION_MOSTRARDEBUG == TRUE
			Debug_Escribir_Varg(L"DAnimacion::Iniciar %d milisegundos.\n", Milisegundos);
		#endif
	}

/*	void DAnimacion::Invertir(void) {
		if (_Timer == NULL) return;
		#if DANIMACION_MOSTRARDEBUG == TRUE
			Debug_Escribir_Varg(L"DAnimacion::Invertir %d milisegundos.\n", _TiempoActual);
		#endif

		_Duracion = _TiempoActual;
		_TiempoActual = 0;
		// El Desde pasa a ser el Hasta, y el valor actual se convierte en el Desde
		for (size_t i = 0; i < _Datos.Total(); i++) {
			_Datos[i].Hasta = _Datos[i].Desde;
			_Datos[i].Desde = _Valores[i];
		}
	}*/

	void DAnimacion::Terminar(void) {
		#if DANIMACION_MOSTRARDEBUG == TRUE
			Debug_Escribir(L"DAnimacion::Terminar\n");
		#endif
		if (_Timer != NULL) {
			BOOL D = DeleteTimerQueueTimer(NULL, _Timer, NULL);
		}
		_Timer = NULL;
	}

	void CALLBACK DAnimacion::_TimerProc(PVOID lpParameter, BOOLEAN TimerOrWaitFired) {
		DAnimacion *This = static_cast<DAnimacion *>(lpParameter);
		#if DANIMACION_MOSTRARDEBUG == TRUE 
			This->_TiempoActual += 16; // Para depurar avanzaremos 16 ms en cada iteración, asi puedo depurar los valores reales
		#else
			This->_TiempoActual = GetTickCount() - This->_TickInicio;
		#endif

//		double Pos = This->_Duracion * (This->_TiempoActual / 100.0f);

		double Pos = static_cast<double>(This->_TiempoActual) / static_cast<double>(This->_Duracion);

//		double FT = FuncionesTiempo::Linear(Pos);
		for (size_t i = 0; i < This->_Datos.Total(); i++) {
			// Valor decimal
			if (This->_Datos[i].Tipo() == Decimal)  {	This->_ActualizarDecimal(i, Pos);			}
			else									{	This->_ActualizarRGB(i, Pos);				}
		}
		
		BOOL Terminado = (This->_TiempoActual >= This->_Duracion);		
		if (Terminado == TRUE) {
			// Asigno el valor final (algunas veces con decimales se puede pasar..)
			for (size_t i = 0; i < This->_Datos.Total(); i++) 
				This->_Valores[i] = This->_Datos[i].Hasta;			
			This->_Callback(This->_Valores, Terminado);
			This->Terminar();
		}
		else {
			This->_Callback(This->_Valores, Terminado);
		}
	}

	void DAnimacion::_ActualizarDecimal(const size_t i, const double Pos) {
/*		double Parte = (_Datos[i].Hasta() - _Datos[i].Desde()) / static_cast<double>(_Duracion);
		double FN = _Datos[i].Funcion(Pos);
		double D = _Datos[i].Desde(); 
		double H = _Datos[i].Hasta();*/
//		double Valor = (_Datos[i].Desde() + (Parte * _TiempoActual));
//		double Valor = _Datos[i].Desde() - (_Datos[i].Desde() - _Datos[i].Hasta()) * _Datos[i].Funcion(Pos);
	//		double Valor = ((_Datos[i].Hasta() - _Datos[i].Desde()) + (Parte * _TiempoActual));
		//double Valor = ((_Datos[i].Hasta() - _Datos[i].Desde()) + (Parte * _TiempoActual)) * FN;

		// Tope para los valores
/*		if (_Datos[i].Hasta() > _Datos[i].Desde())	{
			if (_Datos[i].Hasta() < Valor) 
				Valor = _Datos[i].Hasta(); 
		}
		else { 
			if (_Datos[i].Hasta() > Valor) 
				Valor = _Datos[i].Hasta(); 
		}*/
		_Valores[i] = _Datos[i].Desde() - (_Datos[i].Desde() - _Datos[i].Hasta()) * _Datos[i].Funcion(Pos);
		#if DANIMACION_MOSTRARDEBUG == TRUE
			Debug_Escribir_Varg(L"DAnimacion::_ActualizarDecimal Desde : %.02f, Hasta : %.02f, Valor : %.02f, T : %d\n", _Datos[i].Desde.Decimal(), _Datos[i].Hasta.Decimal(), _Valores[i].Decimal(), _TiempoActual);
		#endif

	}

	void DAnimacion::_ActualizarRGB(const size_t i, const double Pos) {
		float ParteR, ParteG, ParteB;
		int ValorR, ValorG, ValorB;
		// Canal R
		ParteR = static_cast<float>(_Datos[i].Hasta.R() - _Datos[i].Desde.R()) / static_cast<float>(_Duracion);
		ValorR = _Datos[i].Desde.R() + static_cast<int>(ParteR * _TiempoActual);
		if (_Datos[i].Hasta.R() > _Datos[i].Desde.R())	{ if (ValorR > _Datos[i].Hasta.R()) 	ValorR = _Datos[i].Hasta.R(); }
		else											{ if (ValorR < _Datos[i].Hasta.R()) 	ValorR = _Datos[i].Hasta.R(); }

		// Canal G
		ParteG = (_Datos[i].Hasta.G() - _Datos[i].Desde.G()) / static_cast<float>(_Duracion);
		ValorG = _Datos[i].Desde.G() + static_cast<int>(ParteG * _TiempoActual);
		if (_Datos[i].Hasta.G() > _Datos[i].Desde.G())	{ if (ValorG > _Datos[i].Hasta.G())		ValorG = _Datos[i].Hasta.G(); }
		else											{ if (ValorG < _Datos[i].Hasta.G())		ValorG = _Datos[i].Hasta.G(); }

		// Canal B
		ParteB = (_Datos[i].Hasta.B() - _Datos[i].Desde.B()) / static_cast<float>(_Duracion);
		ValorB = _Datos[i].Desde.B() + static_cast<int>(ParteB * _TiempoActual);
		if (_Datos[i].Hasta.B() > _Datos[i].Desde.B())	{ if (ValorB > _Datos[i].Hasta.B())		ValorB = _Datos[i].Hasta.B(); }
		else											{ if (ValorB < _Datos[i].Hasta.B())		ValorB = _Datos[i].Hasta.B(); }

		_Valores[i] = RGB(ValorR, ValorG, ValorB);

		#if DANIMACION_MOSTRARDEBUG == TRUE
			Debug_Escribir_Varg(L"DAnimacion::_ActualizarRGB Desde : %d, Hasta : %d, Valor : %d, T : %d\n", _Datos[i].Desde, _Datos[i].Hasta, _Valores[i].Decimal(), _TiempoActual);
		#endif

	}







/*   _____                _                      _              _____      _            
	|  __ \   /\         (_)                    (_)            / ____|    | |           
	| |  | | /  \   _ __  _ _ __ ___   __ _  ___ _  ___  _ __ | |     ___ | | ___  _ __ 
	| |  | |/ /\ \ | '_ \| | '_ ` _ \ / _` |/ __| |/ _ \| '_ \| |    / _ \| |/ _ \| '__|
	| |__| / ____ \| | | | | | | | | | (_| | (__| | (_) | | | | |___| (_) | | (_) | |   
	|_____/_/    \_\_| |_|_|_| |_| |_|\__,_|\___|_|\___/|_| |_|\_____\___/|_|\___/|_|   
*/
                                                                                     
                 /*
	
	DAnimacionColor::DAnimacionColor(void) : _Timer(NULL) {
	}


	DAnimacionColor::~DAnimacionColor(void) {
		Terminar();
	}

	// Función para un color
	void DAnimacionColor::Iniciar(const COLORREF Desde, const COLORREF Hasta, const DWORD Milisegundos, std::function<void(std::vector<COLORREF> &, const BOOL)> LambdaCallback) {
		std::vector<Colores> vValores;
		vValores.push_back(Colores(Desde, Hasta));
		Iniciar(vValores, Milisegundos, LambdaCallback);
	}

	// Función para dos colores
	void DAnimacionColor::Iniciar(const COLORREF Desde1, const COLORREF Hasta1, const COLORREF Desde2, const COLORREF Hasta2, const DWORD Milisegundos, std::function<void(std::vector<COLORREF> &, const BOOL)> LambdaCallback) {
		std::vector<Colores> vValores;
		vValores.push_back(Colores(Desde1, Hasta1));
		vValores.push_back(Colores(Desde2, Hasta2));
		Iniciar(vValores, Milisegundos, LambdaCallback);
	}

	// Función para tres colores
	void DAnimacionColor::Iniciar(const COLORREF Desde1, const COLORREF Hasta1, const COLORREF Desde2, const COLORREF Hasta2, const COLORREF Desde3, const COLORREF Hasta3, const DWORD Milisegundos, std::function<void(std::vector<COLORREF> &, const BOOL)> LambdaCallback) {
		std::vector<Colores> vValores;
		vValores.push_back(Colores(Desde1, Hasta1));
		vValores.push_back(Colores(Desde2, Hasta2));
		vValores.push_back(Colores(Desde3, Hasta3));
		Iniciar(vValores, Milisegundos, LambdaCallback);
	}

	// Función para cuatro colores
	void DAnimacionColor::Iniciar(const COLORREF Desde1, const COLORREF Hasta1, const COLORREF Desde2, const COLORREF Hasta2, const COLORREF Desde3, const COLORREF Hasta3, const COLORREF Desde4, const COLORREF Hasta4, const DWORD Milisegundos, std::function<void(std::vector<COLORREF> &, const BOOL)> LambdaCallback) {
		std::vector<Colores> vValores;
		vValores.push_back(Colores(Desde1, Hasta1));
		vValores.push_back(Colores(Desde2, Hasta2));
		vValores.push_back(Colores(Desde3, Hasta3));
		vValores.push_back(Colores(Desde4, Hasta4));
		Iniciar(vValores, Milisegundos, LambdaCallback);
	}


	void DAnimacionColor::Iniciar(std::vector<Colores> &Datos, const DWORD Milisegundos, std::function<void(std::vector<COLORREF> &, const BOOL)> LambdaCallback) {
		// Compruebo que los valores Desde sean distintos a los valores Hasta
		BOOL Iguales = TRUE;
		for (size_t i = 0; i < Datos.size(); i++) {
			if (Datos[i].Desde != Datos[i].Hasta) {
				Iguales = FALSE;
				break;
			}
		}
		// No es necesario crear la animación, todos los valores Desde son iguales a sus valores Hasta
		if (Iguales == TRUE) return;
		
		if (_Timer != NULL) {
			#if DANIMACION_MOSTRARDEBUG == TRUE
				Debug_Escribir(L"DAnimacionColor::Iniciar -> ERROR la animación ya se está ejecutando!\n");
			#endif
			return;
		}
		_Valores.resize(Datos.size());
		// Copio los valores iniciales en el vector de valores (Si se hace invertir y no ha pasado ni una iteración los valores no existen)
		for (size_t i = 0; i < Datos.size(); i++) {
			_Valores[i] = Datos[i].Desde;
		}

		_Datos			= Datos;
		_Callback		= LambdaCallback;
		_Duracion		= Milisegundos;
		_TiempoActual	= 0;
		BOOL Ret = CreateTimerQueueTimer(&_Timer, NULL, reinterpret_cast<WAITORTIMERCALLBACK>(_TimerProc), this, 16, 16, WT_EXECUTEINTIMERTHREAD);

		#if DANIMACION_MOSTRARDEBUG == TRUE
			Debug_Escribir_Varg(L"DAnimacionColor::Iniciar %d milisegundos ID : %d\n", Milisegundos, _Timer);
		#endif
	}

	void DAnimacionColor::Invertir(void) {
		if (_Timer == NULL) return;

		#if DANIMACION_MOSTRARDEBUG == TRUE
			Debug_Escribir_Varg(L"DAnimacionColor::Invertir %d milisegundos ID : %d\n", _TiempoActual, _Timer);
		#endif
		_Duracion = _TiempoActual;
		_TiempoActual = 0;
		// El Desde pasa a ser el Hasta, y el valor actual se convierte en el Desde
		for (size_t i = 0; i < _Datos.size(); i++) {
			_Datos[i].Hasta = _Datos[i].Desde;
			_Datos[i].Desde = _Valores[i];
		}
	}

	void DAnimacionColor::Terminar(void) {
		if (_Timer != NULL) {
			#if DANIMACION_MOSTRARDEBUG == TRUE
				Debug_Escribir(L"DAnimacionColor::Terminar\n");
			#endif
			DeleteTimerQueueTimer(NULL, _Timer, NULL);
		}
		_Timer = NULL;
	}

	void CALLBACK DAnimacionColor::_TimerProc(PVOID lpParameter, BOOLEAN TimerOrWaitFired) {
		DAnimacionColor *This = static_cast<DAnimacionColor *>(lpParameter);
		This->_TiempoActual += 16;
//		if (This->_TiempoActual > This->_Duracion) This->_TiempoActual = This->_Duracion;
		float ParteR, ParteG, ParteB;
		int ValorR, ValorG, ValorB;
		for (size_t i = 0; i < This->_Datos.size(); i++) {
			// Canal R
			ParteR = (This->_Datos[i].HastaR() - This->_Datos[i].DesdeR()) / static_cast<float>(This->_Duracion);
			ValorR = This->_Datos[i].DesdeR() + static_cast<int>(ParteR * This->_TiempoActual);
			if (This->_Datos[i].HastaR() > This->_Datos[i].DesdeR()) { if (ValorR > This->_Datos[i].HastaR()) 		ValorR = This->_Datos[i].HastaR(); }
			else                                                     { if (ValorR < This->_Datos[i].HastaR()) 		ValorR = This->_Datos[i].HastaR(); }

			// Canal G
			ParteG = (This->_Datos[i].HastaG() - This->_Datos[i].DesdeG()) / static_cast<float>(This->_Duracion);
			ValorG = This->_Datos[i].DesdeG() + static_cast<int>(ParteG * This->_TiempoActual);
			if (This->_Datos[i].HastaG() > This->_Datos[i].DesdeG()) { if (ValorG > This->_Datos[i].HastaG())		ValorG = This->_Datos[i].HastaG(); }
			else                                                     { if (ValorG < This->_Datos[i].HastaG())		ValorG = This->_Datos[i].HastaG(); }

			// Canal B
			ParteB = (This->_Datos[i].HastaB() - This->_Datos[i].DesdeB()) / static_cast<float>(This->_Duracion);
			ValorB = This->_Datos[i].DesdeB() + static_cast<int>(ParteB * This->_TiempoActual);
			if (This->_Datos[i].HastaB() > This->_Datos[i].DesdeB()) { if (ValorB > This->_Datos[i].HastaB())		ValorB = This->_Datos[i].HastaB(); }
			else                                                     { if (ValorB < This->_Datos[i].HastaB())		ValorB = This->_Datos[i].HastaB(); }

			This->_Valores[i] = RGB(ValorR, ValorG, ValorB);
/*			#if DANIMACION_MOSTRARDEBUG == TRUE
				Debug_Escribir_Varg(L"DAnimacionColor::_TimerProc _TimerProc ID : %d,, Desde : %d, Hasta : %d, Valor : %d, T : %d\n", This->_Timer, This->_Datos[i].Desde, This->_Datos[i].Hasta, This->_Valores[i], This->_TiempoActual);
			#endif*//*
		}
		BOOL Terminado = (This->_TiempoActual >= This->_Duracion);
		This->_Callback(This->_Valores, Terminado);
		if (Terminado == TRUE) {
			This->Terminar();
		}
	}
	*/


};