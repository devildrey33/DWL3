#include "pch.h"
#include "DAnimacion.h"

namespace DWL {
/*	 _____                _                      _
	|  __ \   /\         (_)                    (_)            
	| |  | | /  \   _ __  _ _ __ ___   __ _  ___ _  ___  _ __  
	| |  | |/ /\ \ | '_ \| | '_ ` _ \ / _` |/ __| |/ _ \| '_ \ 
	| |__| / ____ \| | | | | | | | | | (_| | (__| | (_) | | | |
	|_____/_/    \_\_| |_|_|_| |_| |_|\__,_|\___|_|\___/|_| |_|
*/

	// Constructor por defecto
	DAnimacion::DAnimacion(void) : _Timer(NULL), _TickInicio(NULL), _Duracion(0), _TiempoActual(0), _Eliminado(FALSE) {
	}
	
	// Destructor
	DAnimacion::~DAnimacion(void) {
		Terminar();
	}

	// Función para iniciar la animación con los valores por cópia
	// Los parámetros Desde y Hasta deben ser una lista de longs
	void DAnimacion::Iniciar(std::initializer_list<long> Desde, std::initializer_list<long> Hasta, const DWORD Milisegundos, std::function<void(DAnimacion::Valores&, const BOOL)> LambdaCallback, std::initializer_list <FuncionTiempo> FuncionesTiempo, const DWORD Intervalo) {
		Datos nDatos;
		for (size_t i = 0; i < Desde.size(); i++) {
			nDatos.AgregarDecimal(*(Desde.begin() + i), static_cast<double>(*(Hasta.begin() + i)), (FuncionesTiempo.size() > i) ? *(FuncionesTiempo.begin() + i) : DANIMACION_FUNCION_POR_DEFECTO);
		}
		Iniciar(nDatos, Milisegundos, LambdaCallback, Intervalo);
	}

	// Función para iniciar la animación con los valores por cópia
	// Los parámetros Desde y Hasta deben ser una lista de doubles
	void DAnimacion::Iniciar(std::initializer_list<double> Desde, std::initializer_list<double> Hasta, const DWORD Milisegundos, std::function<void(DAnimacion::Valores&, const BOOL)> LambdaCallback, std::initializer_list <FuncionTiempo> FuncionesTiempo, const DWORD Intervalo) {
		Datos nDatos;
		for (size_t i = 0; i < Desde.size(); i++) {
			nDatos.AgregarDecimal(*(Desde.begin() + i), *(Hasta.begin() + i), (FuncionesTiempo.size() > i) ? *(FuncionesTiempo.begin() + i) : DANIMACION_FUNCION_POR_DEFECTO);
		}
		Iniciar(nDatos, Milisegundos, LambdaCallback, Intervalo);
	}

	// Función para iniciar la animación con los valores por cópia
	// Los parámetros Desde y Hasta deben ser una lista de colorrefs
	void DAnimacion::Iniciar(std::initializer_list<COLORREF> Desde, std::initializer_list<COLORREF> Hasta, const DWORD Milisegundos, std::function<void(DAnimacion::Valores&, const BOOL)> LambdaCallback, std::initializer_list <FuncionTiempo> FuncionesTiempo, const DWORD Intervalo) {
		Datos nDatos;
		for (size_t i = 0; i < Desde.size(); i++) {
			nDatos.AgregarRGB(*(Desde.begin() + i), *(Hasta.begin() + i), (FuncionesTiempo.size() > i) ? *(FuncionesTiempo.begin() + i) : DANIMACION_FUNCION_POR_DEFECTO);
		}
		Iniciar(nDatos, Milisegundos, LambdaCallback, Intervalo);
	}

	// Función para iniciar la animación con los valores Hasta por referéncia
	// El parámetro Desde debe ser una lista de longs, y el parámetro Hasta debe ser una lista de punteros a longs.
	void DAnimacion::Iniciar(std::initializer_list<long> Desde, std::initializer_list<long*> Hasta, const DWORD Milisegundos, std::function<void(DAnimacion::Valores&, const BOOL)> LambdaCallback, std::initializer_list <FuncionTiempo> FuncionesTiempo, const DWORD Intervalo) {
		Datos nDatos;
		for (size_t i = 0; i < Desde.size(); i++) {
			nDatos.AgregarDecimal(*(Desde.begin() + i), reinterpret_cast<double *>(*(Hasta.begin() + i)), (FuncionesTiempo.size() > i) ? *(FuncionesTiempo.begin() + i) : DANIMACION_FUNCION_POR_DEFECTO);
		}
		Iniciar(nDatos, Milisegundos, LambdaCallback, Intervalo);
	}

	// Función para iniciar la animación con los valores Hasta por referéncia
	// El parámetro Desde debe ser una lista de doubles, y el parámetro Hasta debe ser una lista de punteros a doubles.
	void DAnimacion::Iniciar(std::initializer_list<double> Desde, std::initializer_list<double*> Hasta, const DWORD Milisegundos, std::function<void(DAnimacion::Valores&, const BOOL)> LambdaCallback, std::initializer_list <FuncionTiempo> FuncionesTiempo, const DWORD Intervalo) {
		Datos nDatos;
		for (size_t i = 0; i < Desde.size(); i++) {
			nDatos.AgregarDecimal(*(Desde.begin() + i), *(Hasta.begin() + i), (FuncionesTiempo.size() > i) ? *(FuncionesTiempo.begin() + i) : DANIMACION_FUNCION_POR_DEFECTO);
		}
		Iniciar(nDatos, Milisegundos, LambdaCallback, Intervalo);
	}

	// Función para iniciar la animación con los valores Hasta por referéncia
	// El parámetro Desde debe ser una lista de COLORREF, y el parámetro Hasta debe ser una lista de punteros a COLORREF.
	void DAnimacion::Iniciar(std::initializer_list<COLORREF> Desde, std::initializer_list<COLORREF*> Hasta, const DWORD Milisegundos, std::function<void(DAnimacion::Valores&, const BOOL)> LambdaCallback, std::initializer_list <FuncionTiempo> FuncionesTiempo, const DWORD Intervalo) {
		Datos nDatos;
		for (size_t i = 0; i < Desde.size(); i++) {
			nDatos.AgregarRGB(*(Desde.begin() + i), *(Hasta.begin() + i), (FuncionesTiempo.size() > i) ? *(FuncionesTiempo.begin() + i) : DANIMACION_FUNCION_POR_DEFECTO);
		}
		Iniciar(nDatos, Milisegundos, LambdaCallback, Intervalo);
	}

	// Función que inicia la animación a partir de un objeto DAnimacion::Datos
	void DAnimacion::Iniciar(Datos &nDatos, const DWORD Milisegundos, std::function<void(Valores &, const BOOL)> LambdaCallback, const DWORD Intervalo) {
		// Re-habilito que se pueda eliminar este timer UNA VEZ
		_Eliminado = FALSE;

		// Compruebo que los valores Desde sean distintos a los valores Hasta
		BOOL Iguales = TRUE;
		for (size_t i = 0; i < nDatos.Total(); i++) {
			if (nDatos[i].Desde != nDatos[i].Hasta) {
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
		_Valores.Resize(nDatos.Total());
		for (size_t i = 0; i < nDatos.Total(); i++) {
			_Valores[i] = nDatos[i].Desde;
		}

		_Datos			= nDatos;

		_Callback		= LambdaCallback;
		_Duracion		= Milisegundos;
		_TickInicio     = GetTickCount();
		_TiempoActual	= 0;																										// WT_EXECUTEINPERSISTENTTHREAD
		BOOL Ret = CreateTimerQueueTimer(&_Timer, NULL, reinterpret_cast<WAITORTIMERCALLBACK>(_TimerProc), this, Intervalo, Intervalo, WT_EXECUTEINTIMERTHREAD);
//		BOOL Ret = CreateTimerQueueTimer(&_Timer, NULL, reinterpret_cast<WAITORTIMERCALLBACK>(_TimerProc), this, Intervalo, Intervalo, WT_EXECUTEDEFAULT);
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

	// Función que termina la animación
	void DAnimacion::Terminar(void) {
		#if DANIMACION_MOSTRARDEBUG == TRUE
			Debug_Escribir(L"DAnimacion::Terminar\n");
		#endif
		if (_Timer != NULL && _Eliminado == FALSE) {
			// Aseguro que no se intenta eliminar 2 veces el mismo timer
			_Eliminado = TRUE;
			BOOL D = DeleteTimerQueueTimer(NULL, _Timer, NULL);
			// Amb INVALID_HANDLE_VALUE la funció no retorna fins que no acaba el timer, amb NULL pot ser que s'extigui executan mentres l'intento apagar...
//			BOOL D = DeleteTimerQueueTimer(NULL, _Timer, INVALID_HANDLE_VALUE); // INVALID HANDLE no funciona be amb CreateTimerQueueTimer WT_EXECUTEINTIMERTHREAD
			#if DANIMACION_MOSTRARDEBUG == TRUE
				if (D != TRUE) {				
					Debug_Escribir_Varg(L"DAnimacion::Terminar DeleteTimerQueueTimer Error : %s\n", _Aplicacion->UltimoError().c_str());				
	//				_Eliminado = FALSE;
	//				return;
				}
			#endif
		}
		_Timer = NULL;
	}

	// Callback para el temporizador
	void CALLBACK DAnimacion::_TimerProc(PVOID lpParameter, BOOLEAN TimerOrWaitFired) {
		DAnimacion *This = static_cast<DAnimacion *>(lpParameter);
		#if DANIMACION_MOSTRARDEBUG == TRUE 
			This->_TiempoActual += 16; // Para depurar avanzaremos 16 ms en cada iteración, asi puedo depurar los valores reales
		#else
			This->_TiempoActual = GetTickCount() - This->_TickInicio;
		#endif

		double Pos = static_cast<double>(This->_TiempoActual) / static_cast<double>(This->_Duracion);

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
			try {
				This->_Callback(This->_Valores, Terminado);
				This->Terminar();
			}
			catch (std::bad_function_call) {
				// El callback ya no existe por que se ha eliminado el objeto DAnimacion
			}
		}
		else {
			try {
				This->_Callback(This->_Valores, Terminado);
			}
			catch (std::bad_function_call) {
				// El callback ya no existe por que se ha eliminado el objeto DAnimacion
			}
		}
	}

	// Función que actualiza los valores decimales
	void DAnimacion::_ActualizarDecimal(const size_t i, const double Pos) {
		_Valores[i] = _Datos[i].Desde() - (_Datos[i].Desde() - _Datos[i].Hasta()) * _Datos[i].Funcion(Pos);
		#if DANIMACION_MOSTRARDEBUG == TRUE
			Debug_Escribir_Varg(L"DAnimacion::_ActualizarDecimal Desde : %.02f, Hasta : %.02f, Valor : %.02f, T : %d\n", _Datos[i].Desde.Decimal(), _Datos[i].Hasta.Decimal(), _Valores[i].Decimal(), _TiempoActual);
		#endif
	}


	// Función que actualiza los valores RGB
	void DAnimacion::_ActualizarRGB(const size_t i, const double Pos) {
		const BYTE DR = _Datos[i].Desde.R();	// Desde Rojo
		const BYTE DG = _Datos[i].Desde.G();	// Desde Verde
		const BYTE DB = _Datos[i].Desde.B();	// Desde Azul

		const BYTE HR = _Datos[i].Hasta.R();	// Hasta Rojo
		const BYTE HG = _Datos[i].Hasta.G();	// Hasta Verde
		const BYTE HB = _Datos[i].Hasta.B();	// Hasta Azul

		// Duración convertida a double
		const double dDuracion = static_cast<double>(_Duracion);

		// Parte = (Hasta - Desde) / Duracion
		double ParteR, ParteG, ParteB;
		// Valor = Desde + (Parte * TiempoActual)
		int    ValorR, ValorG, ValorB;
		
		// Calculo el canal R
		ParteR = static_cast<double>(HR - DR) / dDuracion;
		ValorR = DR + static_cast<int>(ParteR * _TiempoActual);
		if (HR > DR)	{ if (ValorR > HR) ValorR = HR; }
		else			{ if (ValorR < HR) ValorR = HR; }	

		// Calculo el canal G
		ParteG = (HG - DG) / dDuracion;
		ValorG = DG + static_cast<int>(ParteG * _TiempoActual);
		if (HG > DG)	{ if (ValorG > HG) ValorG = HG; }
		else			{ if (ValorG < HG) ValorG = HG; }

		// Calculo el canal B
		ParteB = (HB - DB) / dDuracion;
		ValorB = DB + static_cast<int>(ParteB * _TiempoActual);
		if (HB > DB)	{ if (ValorB > HB) ValorB = HB; }
		else			{ if (ValorB < HB) ValorB = HB; }

		// Asigno el color final
		_Valores[i] = RGB(ValorR, ValorG, ValorB);

		#if DANIMACION_MOSTRARDEBUG == TRUE
			Debug_Escribir_Varg(L"DAnimacion::_ActualizarRGB Desde : %03d %03d %03d, Hasta : %03d %03d %03d , Valor : %d, T : %d\n", DR, DG, DB, HR, HG, HB, _Valores[i].Decimal(), _TiempoActual);
		#endif
	}



/*
  _____                _                      _                      __      __   _            
 |  __ \   /\         (_)                    (_)              _   _  \ \    / /  | |           
 | |  | | /  \   _ __  _ _ __ ___   __ _  ___ _  ___  _ __   (_) (_)  \ \  / /_ _| | ___  _ __ 
 | |  | |/ /\ \ | '_ \| | '_ ` _ \ / _` |/ __| |/ _ \| '_ \            \ \/ / _` | |/ _ \| '__|
 | |__| / ____ \| | | | | | | | | | (_| | (__| | (_) | | | |  _   _     \  / (_| | | (_) | |   
 |_____/_/    \_\_| |_|_|_| |_| |_|\__,_|\___|_|\___/|_| |_| (_) (_)     \/ \__,_|_|\___/|_|   
                                                                                               
  */

	// Operador para comparar valores
	bool DAnimacion::Valor::operator == (Valor &Comp) {
		if (_Tipo == Valor_Tipo::Decimal) {
			if (_Valor == NULL) { return _ValorC == Comp.Decimal(); }
			else				{ return *_Valor == Comp.Decimal(); }
		}
		else {
			if (_Color == NULL) { return _ColorC == Comp.Color();	}
			else				{ return *_Color == Comp.Color();	}
		}
		return false;
	}

	// Operador para comparar valores
	bool DAnimacion::Valor::operator != (Valor &Comp) {
		if (_Tipo == Valor_Tipo::Decimal) {
			if (_Valor == NULL) { return _ValorC != Comp.Decimal(); }
			else				{ return *_Valor != Comp.Decimal(); }
		}
		else {
			if (_Color == NULL) { return _ColorC != Comp.Color();	}
			else				{ return *_Color != Comp.Color();	}
		}
		return false;
	}

	// Operador para comparar valores
	bool DAnimacion::Valor::operator < (Valor &Comp) {
		if (_Tipo == Valor_Tipo::Decimal) {
			if (_Valor == NULL) { return _ValorC < Comp.Decimal();	}
			else				{ return *_Valor < Comp.Decimal();	}
		}
		else {
			if (_Color == NULL) { return _ColorC < Comp.Color();	}
			else				{ return *_Valor < Comp.Color();	}
		}
	}

	// Operador para comparar valores
	bool DAnimacion::Valor::operator > (Valor &Comp) {
		if (_Tipo == Valor_Tipo::Decimal) {
			if (_Valor == NULL) { return _ValorC > Comp.Decimal();	}
			else				{ return *_Valor < Comp.Decimal();	}
		}
		else {
			if (_Color == NULL) { return _ColorC > Comp.Color();	}
			else				{ return *_Valor < Comp.Color();	}
		}
	}


};

