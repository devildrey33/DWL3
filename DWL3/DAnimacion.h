#ifndef DANIMACION_H
#define DANIMACION_H


#include <functional>
#include <vector>

// Depuraci�n
#define DANIMACION_MOSTRARDEBUG				FALSE
// Constante PI
#define DANIMACION_PI						3.14159265359f
// Funci�n de tiempo por defecto
#define DANIMACION_FUNCION_POR_DEFECTO		DWL::DAnimacion::FuncionesTiempo::CircularInOut


namespace DWL {

	class DAnimacion {
	   public:
		// Tipo para guardar una funci�n de tiempo
		typedef  const double(*FuncionTiempo)(const double);

		// Funciones de tiempo estaticas 
		class FuncionesTiempo {
		  public : /////////////////// Constructor
									FuncionesTiempo(void)					{ };
									// Destructor
			                       ~FuncionesTiempo(void)					{ };
			  static const double	Linear(const double nTiempo)			{ return nTiempo; };
			  static const double	SinIn(const double nTiempo)				{ return 1.0f - cos(nTiempo * DANIMACION_PI / 2.0f); };
			  static const double	SinOut(const double nTiempo)			{ return sin(nTiempo * DANIMACION_PI / 2.0f); };
			  static const double	SinInOut(const double nTiempo)			{ return -0.5f * (cos(nTiempo * DANIMACION_PI / 2.0f) - 1.0f); };
			  static const double   CubicInOut(const double nTiempo)		{ return (nTiempo < 0.5f) ? 4.0f * nTiempo * nTiempo * nTiempo : 0.5f * pow(2.0f * nTiempo - 2.0f, 3.0f) + 1.0f; }
			  static const double   CircularInOut(const double nTiempo)		{ return (nTiempo < 0.5f) ? 0.5f * (1.0f - sqrt(1.0f - 4.0f * nTiempo * nTiempo)) : 0.5f * (sqrt((3.0f - 2.0f * nTiempo) * (2.0f * nTiempo - 1.0f)) + 1.0f); }			
			  static const double   BounceIn(const double nTiempo)			{ return (nTiempo < 0.5f) ? nTiempo * 3.0f : 	1.5f - (nTiempo * 0.5f); }
		};

		// Tipo de valor para la animaci�n (valor o color)
		enum Valor_Tipo {
			Decimal,	// double
			RGB			// COLORREF
		};



		// Objeto que contiene un valor por refer�ncia para la animaci�n (el valor puede ser decim�l o un color)
		// Esta clase puede guardar un valor por c�pia o por referencia mediante punteros, y tiene prioridad para los punteros de forma que si el puntero no es NULL lo devolvera antes que el valor por cop�a.
		class Valor {
		  public : /////////////////// Constructor vacio
									Valor(void)							: _Tipo(Valor_Tipo::Decimal), _ValorC(0),		_ColorC(0),		_Valor(NULL),	_Color(NULL)		{ }
									// Constructores para valores
									Valor(const COLORREF nCol)			: _Tipo(Valor_Tipo::RGB)    , _ValorC(0),		_ColorC(nCol),	_Valor(NULL),	_Color(NULL)		{ }
									Valor(const double nValor)			: _Tipo(Valor_Tipo::Decimal), _ValorC(nValor),  _ColorC(0),		_Valor(NULL),	_Color(NULL)		{ }
									// Constructores para punteros
									Valor(COLORREF* nCol)				: _Tipo(Valor_Tipo::RGB)	, _ValorC(0),		_ColorC(0),		_Valor(NULL),	_Color(nCol)		{ }
									Valor(double* nValor)				: _Tipo(Valor_Tipo::Decimal), _ValorC(0),		_ColorC(0),		_Valor(nValor), _Color(NULL)		{ }

									// Asignan valores
			inline void				operator = (const COLORREF nCol)	{ _ColorC = nCol;		}
			inline void				operator = (const double nValor)	{ _ValorC = nValor; 	}
									// Asignan punteros
			inline void				operator = (COLORREF *nCol)			{ _Color = nCol;		}
			inline void				operator = (double *nValor)			{ _Valor = nValor;		}

									// Compara objetos Valor
			bool					operator == (Valor& Comp);
			bool					operator != (Valor& Comp);
			bool					operator < (Valor& Comp);
			bool					operator > (Valor& Comp);

									// Compara valores decimales
			inline bool             operator < (const double& Comp)		{ return (_Valor == NULL) ? (_ValorC < Comp) : (*_Valor < Comp); }
			inline bool             operator > (const double& Comp)		{ return (_Valor == NULL) ? (_ValorC > Comp) : (*_Valor > Comp); }

									// Devuelve el valor decimal
			inline const double		operator() (void)					{ return (_Valor == NULL) ? _ValorC : *_Valor; }
									// Devuelven los canales RGB
			inline const BYTE		R(void)								{ return (_Color == NULL) ? GetRValue(_ColorC) : GetRValue(*_Color); };
			inline const BYTE		G(void)								{ return (_Color == NULL) ? GetGValue(_ColorC) : GetGValue(*_Color); };
			inline const BYTE		B(void)								{ return (_Color == NULL) ? GetBValue(_ColorC) : GetBValue(*_Color); };
									// Devuelven los valores
			inline const double		Decimal(void)			  			{ return (_Valor == NULL) ? _ValorC : *_Valor; }
			inline const long		Entero(void)			  			{ return (_Valor == NULL) ? static_cast<long>(_ValorC) : static_cast<long>(*_Valor); }
									// Devuelve el color
			inline const COLORREF	Color(void)							{ return (_Color == NULL) ? _ColorC : *_Color; }
									// Devuelve el tipo de dato (valor o color)
			inline const Valor_Tipo Tipo(void)							{ return _Tipo; };
		  protected: /////////////////
			double                *_Valor;	// Puntero a un valor (Si es NULL hay que mirar el _ValorC)
			COLORREF              *_Color;	// Puntero a un color (Si es NULL hay que mirar el _ColorC)
			double                 _ValorC;	// Valor por c�pia
			COLORREF               _ColorC;	// Color por c�pia
			Valor_Tipo             _Tipo;	// Tipo (valor o color)
		};


		// Objeto que contiene el valor origen y el valor destino de la animaci�n
		class Dato {
	   		public: ////////////////////// Constructor por defecto
										Dato(void) : Funcion(NULL)																																{ };
										// Constructor double con el valor hasta por puntero
										Dato(const double    nDesde, double        *nHasta, FuncionTiempo nFuncion = FuncionesTiempo::Linear) : Desde(nDesde), Hasta(nHasta), Funcion(nFuncion)	{ };
										// Constructor COLORREF con el valor hasta por puntero
										Dato(const COLORREF  nDesde, COLORREF      *nHasta, FuncionTiempo nFuncion = FuncionesTiempo::Linear) : Desde(nDesde), Hasta(nHasta), Funcion(nFuncion)	{ };
										// Constructor double con el valor hasta por copia
										Dato(const double    nDesde, const double   nHasta, FuncionTiempo nFuncion = FuncionesTiempo::Linear) : Desde(nDesde), Hasta(nHasta), Funcion(nFuncion)	{ };
										// Constructor COLORREF con el valor hasta por copia
										Dato(const COLORREF  nDesde, const COLORREF nHasta, FuncionTiempo nFuncion = FuncionesTiempo::Linear) : Desde(nDesde), Hasta(nHasta), Funcion(nFuncion)	{ };
										// Destructor
									   ~Dato(void)																																				{ };
									    // Valor desde
		     Valor						Desde;
										// Valor hasta
			 Valor						Hasta;
										// Funci�n de tiempo
			 FuncionTiempo				Funcion;
										// Tipo de datos (Decimal / RGB)
			 inline const Valor_Tipo	Tipo(void)																																				{ return Desde.Tipo();	};
		};

		// Objeto para almacenar un vector de datos Desde y Hasta
		// Puedes agregar un valor decimal con las funciones AgregarDecimal, o un color con las funciones AgregarRGB
		class Datos {
		   public : ////////////////// Constructor
									Datos(void)																							{ }
									// Destructor
				                   ~Datos(void)																							{ }
									// Funciones para valores por c�pia
			void                    AgregarRGB(COLORREF    nDesde, COLORREF  nHasta, FuncionTiempo nFuncion = FuncionesTiempo::Linear)	{ _Datos.push_back(Dato(nDesde, nHasta, nFuncion)); };
			void                    AgregarDecimal(double  nDesde, double    nHasta, FuncionTiempo nFuncion = FuncionesTiempo::Linear)	{ _Datos.push_back(Dato(nDesde, nHasta, nFuncion)); };
									// Funciones para valores Hasta por refer�ncia
			void                    AgregarRGB(COLORREF    nDesde, COLORREF *nHasta, FuncionTiempo nFuncion = FuncionesTiempo::Linear)  { _Datos.push_back(Dato(nDesde, nHasta, nFuncion)); };
			void                    AgregarDecimal(double  nDesde, double   *nHasta, FuncionTiempo nFuncion = FuncionesTiempo::Linear)  { _Datos.push_back(Dato(nDesde, nHasta, nFuncion)); };
									// Operador que devuelve el dato de la posici�n especificada
			inline Dato            &operator [] (const size_t Pos)																		{ return _Datos[Pos]; }
									// Funci�n que devuelve el total de datos de este objeto
			inline const size_t     Total(void)																							{ return _Datos.size(); }
		   protected: //////////////// Vector que contiene los datos
			std::vector<Dato>	   _Datos;
		};


		// Vector de valores actuales de la animaci�n
		class Valores {
		   public : ////////////////// Constructor
									Valores(void) { }
									// Destructor
								   ~Valores(void) { }
								    // Operador que devuelve el valor de la posici�n especificada
			inline Valor           &operator [] (const size_t Pos)	{ return _Valores[Pos];		}
									// Funci�n que redimensiona el vector de valores
			inline void             Resize(const size_t nTam)		{ _Valores.resize(nTam);	}
		   protected : /////////////// Vector de valores
			std::vector<Valor>     _Valores;
		};

		////////////////////////////////////////////// Constructor
													DAnimacion(void);
													// Destructor
		                                           ~DAnimacion(void);
												    // Funciones para iniciar la animaci�n con una lista de par�metros del mismo tipo
		void										Iniciar(std::initializer_list<long>     Desde, std::initializer_list<long>     Hasta, const DWORD Milisegundos, std::function<void(DAnimacion::Valores&, const BOOL)> LambdaCallback, std::initializer_list <FuncionTiempo> FuncionesTiempo = { DANIMACION_FUNCION_POR_DEFECTO }, const DWORD Intervalo = 16);
		void										Iniciar(std::initializer_list<double>   Desde, std::initializer_list<double>   Hasta, const DWORD Milisegundos, std::function<void(DAnimacion::Valores&, const BOOL)> LambdaCallback, std::initializer_list <FuncionTiempo> FuncionesTiempo = { DANIMACION_FUNCION_POR_DEFECTO }, const DWORD Intervalo = 16);
		void										Iniciar(std::initializer_list<COLORREF> Desde, std::initializer_list<COLORREF> Hasta, const DWORD Milisegundos, std::function<void(DAnimacion::Valores&, const BOOL)> LambdaCallback, std::initializer_list <FuncionTiempo> FuncionesTiempo = { DANIMACION_FUNCION_POR_DEFECTO }, const DWORD Intervalo = 16);
												    // Funciones para iniciar la animaci�n con la lista de par�metros Hasta por refer�ncia (de esta forma puedes modificar el valor Hasta durante la animaci�n)
		void										Iniciar(std::initializer_list<long>     Desde, std::initializer_list<long *>     Hasta, const DWORD Milisegundos, std::function<void(DAnimacion::Valores&, const BOOL)> LambdaCallback, std::initializer_list <FuncionTiempo> FuncionesTiempo = { DANIMACION_FUNCION_POR_DEFECTO }, const DWORD Intervalo = 16);
		void										Iniciar(std::initializer_list<double>   Desde, std::initializer_list<double *>   Hasta, const DWORD Milisegundos, std::function<void(DAnimacion::Valores&, const BOOL)> LambdaCallback, std::initializer_list <FuncionTiempo> FuncionesTiempo = { DANIMACION_FUNCION_POR_DEFECTO }, const DWORD Intervalo = 16);
		void										Iniciar(std::initializer_list<COLORREF> Desde, std::initializer_list<COLORREF *> Hasta, const DWORD Milisegundos, std::function<void(DAnimacion::Valores&, const BOOL)> LambdaCallback, std::initializer_list <FuncionTiempo> FuncionesTiempo = { DANIMACION_FUNCION_POR_DEFECTO }, const DWORD Intervalo = 16);
													// Funcion para iniciar la animaci�n con una lista de par�metros a medida que puede contener enteros, decimales y colores en la misma lista.
		void										Iniciar(DAnimacion::Datos &Datos, const DWORD Milisegundos, std::function<void(DAnimacion::Valores &, const BOOL)> LambdaCallback, const DWORD Intervalo = 16);
													// Funci�n que termina la animaci�n
		void										Terminar(void);
													// Funci�n que devuelve si la animaci�n se est� ejecutando
		inline const BOOL                           Animando(void) { return (_Timer != NULL); }
													// Funci�n que devuelve el tiempo actual de la animaci�n
		inline const DWORD							TiempoActual(void) { return _TiempoActual; }
	   protected: //////////////////////////////////// Funci�n protegida que actualiza los valores decimales
 	    void                                       _ActualizarDecimal(const size_t i, const double Pos);
													// Funci�n protegida que actualiza los valores de color
		void                                       _ActualizarRGB(const size_t i, const double Pos);
													// Callback estatico para el temporizador
		static void CALLBACK                       _TimerProc(PVOID lpParameter, BOOLEAN TimerOrWaitFired);
													// Funci�n lambda que se ejecuta durante el callback estatico
		std::function<void(Valores &, const BOOL)> _Callback;
													// Temporizador
		HANDLE                                     _Timer;
													// Duraci�n de la animaci�n
		DWORD                                      _Duracion;
													// Tiempo actual de la animaci�n
		DWORD                                      _TiempoActual;
													// Tick en el que ha empezado la animaci�n
		DWORD                                      _TickInicio;
													// Objeto que contiene los datos de la animaci�n
		Datos                                      _Datos;
													// Objeto que contiene los valores de la animaci�n
		Valores                                    _Valores;
													// Valor que determina si se puede eliminar el temporizador
		BOOL                                       _Eliminado;

	};
};

#endif
