#ifndef DANIMACION_H
#define DANIMACION_H

#define DANIMACION_MOSTRARDEBUG	FALSE

#include <functional>
#include <vector>
//#define PI 3.14159265359f

#define FUNCION_POR_DEFECTO			DWL::DAnimacion::FuncionesTiempo::CircularInOut


namespace DWL {

	class DAnimacion {
	   public:
		   typedef  const double(*FuncionTiempo)(const double);
		// Funciones de tiempo estaticas 
		class FuncionesTiempo {
		  public :
									FuncionesTiempo(void)					{ };
			                       ~FuncionesTiempo(void)					{ };
			  static const double	Linear(const double nTiempo)			{ return nTiempo; };
			  static const double	SinIn(const double nTiempo)				{ return 1.0f - cos(nTiempo * PI / 2.0f); };
			  static const double	SinOut(const double nTiempo)			{ return sin(nTiempo * PI / 2.0f); };
			  static const double	SinInOut(const double nTiempo)			{ return -0.5f * (cos(nTiempo * PI / 2.0f) - 1.0f); };
			  static const double   CubicInOut(const double nTiempo)		{ return (nTiempo < 0.5f) ? 4.0f * nTiempo * nTiempo * nTiempo : 0.5f * pow(2.0f * nTiempo - 2.0f, 3.0f) + 1.0f; }
			  static const double   CircularInOut(const double nTiempo)		{ return (nTiempo < 0.5f) ? 0.5f * (1.0f - sqrt(1.0f - 4.0f * nTiempo * nTiempo)) : 0.5f * (sqrt((3.0f - 2.0f * nTiempo) * (2.0f * nTiempo - 1.0f)) + 1.0f); }
			
			  static const double   BounceIn(const double nTiempo) {
										return (nTiempo < 0.5f) ? 
											nTiempo * 3.0f : 
											1.5f - (nTiempo * 0.5f);
									}
			  static const double   PI;
		};

		enum Valor_Tipo {
			Decimal,	// double
			RGB			// COLORREF
		};

//		class ValorFinal;
		// Objeto que contiene el valor inicial de la animación (el valor puede ser decimál o un color)
/*		class Valor {
		  public :
									Valor(void) : _Tipo(Valor_Tipo::Decimal), _Valor(NULL), _Color(NULL), _Propio(FALSE)				{ }
									// Constructores que crean sus punteros
									Valor(const COLORREF nCol) : _Tipo(Valor_Tipo::RGB), _Valor(NULL), _Color(NULL), _Propio(TRUE)		{
										_Color = new COLORREF; *_Color = nCol; 
									}
									Valor(const double nValor) : _Tipo(Valor_Tipo::Decimal), _Valor(NULL), _Color(NULL), _Propio(TRUE)	{ 
										_Valor = new double; *_Valor = nValor; 
									}
									// Constructores por "referencia"
									Valor(COLORREF *nCol) : _Tipo(RGB), _Color(nCol), _Valor(NULL), _Propio(FALSE)						{ }
									Valor(double *nValor) : _Tipo(Valor_Tipo::Decimal), _Valor(nValor), _Color(NULL), _Propio(FALSE)	{ }
								   ~Valor(void)																							{ 
									   if (_Propio == TRUE) { 
										   if (_Valor != NULL) { delete _Valor; } 
										   if (_Color != NULL) { delete _Color; } 
									   }
								   }
			inline void				operator = (const COLORREF nCol)	{ *_Color = nCol; }
			inline void				operator = (const double nValor)	{ *_Valor = nValor; }
			inline void				operator = (const Valor &nValor)	{ if (_Tipo == Valor_Tipo::Decimal) { *_Valor = *nValor._Valor; } else { *_Color = *nValor._Color;  } }
			inline bool             operator == (const Valor &Comp)		{ return (_Tipo == Valor_Tipo::Decimal) ? (*_Valor == *Comp._Valor) : (*_Color == *Comp._Color); }
			inline bool             operator != (const Valor &Comp)		{ return (_Tipo == Valor_Tipo::Decimal) ? (*_Valor != *Comp._Valor) : (*_Color != *Comp._Color); }
			inline bool             operator < (const Valor &Comp)		{ return (_Tipo == Valor_Tipo::Decimal) ? (*_Valor < *Comp._Valor) : (*_Color < *Comp._Color); }
			inline bool             operator > (const Valor &Comp)		{ return (_Tipo == Valor_Tipo::Decimal) ? (*_Valor > *Comp._Valor) : (*_Color > *Comp._Color); }
			inline bool             operator < (const double &Comp)		{ return (*_Valor < Comp); }
			inline bool             operator > (const double &Comp)		{ return (*_Valor > Comp); }
			inline const double		operator() (void)					{ return *_Valor; }
			inline const BYTE		R(void)								{ return GetRValue(*_Color); };
			inline const BYTE		G(void)								{ return GetGValue(*_Color); };
			inline const BYTE		B(void)								{ return GetBValue(*_Color); };
			inline const double		Decimal(void)			  			{ return *_Valor; }
			inline const COLORREF	Color(void)							{ return *_Color; }
			inline const Valor_Tipo Tipo(void)							{ return _Tipo; };
		  protected:
			double                *_Valor;
			COLORREF              *_Color;
			Valor_Tipo             _Tipo;
			BOOL                   _Propio;
			friend class ValorFinal;
		};*/

		// Objeto que contiene un valor de la animación (el valor puede ser decimál o un color)
		class Valor {
		  public :
									Valor(void) : _Tipo(Valor_Tipo::Decimal), _Valor(0.0f), _Color(0)					{ }
									Valor(const COLORREF nCol) : _Tipo(RGB), _Color(nCol), _Valor(0.0f)					{ }
									Valor(const double nValor) : _Tipo(Valor_Tipo::Decimal), _Valor(nValor), _Color(0)	{ }
			inline void				operator = (const COLORREF nCol)										{ _Color = nCol; }
			inline void				operator = (const double nValor)										{ _Valor = nValor; }
			inline bool             operator == (const Valor& Comp)											{ return (_Tipo == Valor_Tipo::Decimal) ? (_Valor == Comp._Valor) : (_Color == Comp._Color); }
			inline bool             operator != (const Valor& Comp)											{ return (_Tipo == Valor_Tipo::Decimal) ? (_Valor != Comp._Valor) : (_Color != Comp._Color); }
			inline bool             operator < (const Valor &Comp)											{ return (_Tipo == Valor_Tipo::Decimal) ? (_Valor < Comp._Valor) : (_Color < Comp._Color); }
			inline bool             operator > (const Valor &Comp)											{ return (_Tipo == Valor_Tipo::Decimal) ? (_Valor > Comp._Valor) : (_Color > Comp._Color); }
			inline bool             operator < (const double &Comp)											{ return (_Valor < Comp); }
			inline bool             operator > (const double &Comp)											{ return (_Valor > Comp); }
			inline const double		operator() (void)														{ return _Valor; }
			inline const BYTE		R(void)																	{ return GetRValue(_Color); };
			inline const BYTE		G(void)																	{ return GetGValue(_Color); };
			inline const BYTE		B(void)																	{ return GetBValue(_Color); };
			inline const double		Decimal(void)			  												{ return _Valor; }
			inline const long		Entero(void)			  												{ return static_cast<long>(_Valor); }
			inline const COLORREF	Color(void)																{ return _Color; }
			inline const Valor_Tipo Tipo(void)																{ return _Tipo; };
		  protected:
			double                 _Valor;
			COLORREF               _Color;
			Valor_Tipo             _Tipo;
		};

		// Objeto que contiene el valor final de la animación (el valor puede ser decimál o un color)
		// El valor final tiene que ser un puntero ya que puede cambiar durante la animación
/*		class ValorFinal {
		  public :
									ValorFinal(void) : _Tipo(Valor_Tipo::Decimal), _Valor(NULL), _Color(NULL)				{ }
									ValorFinal(COLORREF *nCol) : _Tipo(RGB), _Color(nCol), _Valor(NULL)						{ }
									ValorFinal(double *nValor) : _Tipo(Valor_Tipo::Decimal), _Valor(nValor), _Color(NULL)	{ }
			inline void				operator = (COLORREF *nCol)																{ _Color = nCol; }
			inline void				operator = (double *nValor)																{ _Valor = nValor; }
			inline bool             operator == (const Valor& Comp)												{ return (_Tipo == Valor_Tipo::Decimal) ? (*_Valor == Comp._Valor) : (*_Color == Comp._Color); }
			inline bool             operator != (const Valor& Comp)												{ return (_Tipo == Valor_Tipo::Decimal) ? (*_Valor != Comp._Valor) : (*_Color != Comp._Color); }
			inline bool             operator < (const Valor &Comp)															{ return (_Tipo == Valor_Tipo::Decimal) ? (*_Valor < Comp._Valor) : (*_Color < Comp._Color); }
			inline bool             operator > (const Valor &Comp)															{ return (_Tipo == Valor_Tipo::Decimal) ? (*_Valor > Comp._Valor) : (*_Color > Comp._Color); }
			inline bool             operator < (const double &Comp)															{ return (*_Valor < Comp); }
			inline bool             operator > (const double &Comp)															{ return (*_Valor > Comp); }
			inline const double		operator() (void)																		{ return *_Valor; }
			inline const BYTE		R(void)																					{ return GetRValue(*_Color); };
			inline const BYTE		G(void)																					{ return GetGValue(*_Color); };
			inline const BYTE		B(void)																					{ return GetBValue(*_Color); };
			inline const double		Decimal(void)			  																{ return *_Valor; }
			inline const COLORREF	Color(void)																				{ return *_Color; }
			inline const Valor_Tipo Tipo(void)																				{ return _Tipo; };
		  protected:
			double                *_Valor;
			COLORREF              *_Color;
			Valor_Tipo             _Tipo;
			friend class Valor;
		};*/

		// Objeto que contiene el valor origen y el valor destino de la animación
		class Dato {
	   		public:
										Dato(void) : Funcion(NULL)																															{ };
										Dato(const double nDesde,	double nHasta,		FuncionTiempo nFuncion = FuncionesTiempo::Linear) : Desde(nDesde), Hasta(nHasta), Funcion(nFuncion)	{ };
										Dato(const COLORREF nDesde, COLORREF nHasta,	FuncionTiempo nFuncion = FuncionesTiempo::Linear) : Desde(nDesde), Hasta(nHasta), Funcion(nFuncion)	{ };
//										Dato(const double nDesde,	double *nHasta,		FuncionTiempo nFuncion = FuncionesTiempo::Linear) : Desde(nDesde), Hasta(nHasta), Funcion(nFuncion)	{ };
//										Dato(const COLORREF nDesde, COLORREF *nHasta,	FuncionTiempo nFuncion = FuncionesTiempo::Linear) : Desde(nDesde), Hasta(nHasta), Funcion(nFuncion)	{ };
//										Dato(double *nDesde,		double *nHasta,		FuncionTiempo nFuncion = FuncionesTiempo::Linear) : Desde(nDesde), Hasta(nHasta), Funcion(nFuncion)	{ };
//										Dato(COLORREF *nDesde,		COLORREF *nHasta,	FuncionTiempo nFuncion = FuncionesTiempo::Linear) : Desde(nDesde), Hasta(nHasta), Funcion(nFuncion)	{ };
									   ~Dato(void)																																			{ };
		     Valor						Desde;
			 Valor						Hasta;
			 FuncionTiempo				Funcion;
			 inline const Valor_Tipo	Tipo(void) { return Desde.Tipo(); };
		};

		// Vector da Dato (que contiene Valor Desde, Hasta)
		class Datos {
		   public :
									Datos(void) { }
				                   ~Datos(void) { }
			void                    AgregarRGB(const COLORREF nDesde, const COLORREF nHasta,	FuncionTiempo nFuncion = FuncionesTiempo::Linear) { _Datos.push_back(Dato(nDesde, nHasta, nFuncion)); };
//			void                    AgregarRGB(const COLORREF nDesde, COLORREF *nHasta,			FuncionTiempo nFuncion = FuncionesTiempo::Linear) { _Datos.push_back(Dato(nDesde, nHasta, nFuncion)); };
			void                    AgregarDecimal(const double nDesde, const double nHasta,	FuncionTiempo nFuncion = FuncionesTiempo::Linear) { _Datos.push_back(Dato(nDesde, nHasta, nFuncion)); };
//			void                    AgregarDecimal(const double nDesde, double *nHasta,			FuncionTiempo nFuncion = FuncionesTiempo::Linear) { _Datos.push_back(Dato(nDesde, nHasta, nFuncion)); };
			inline Dato            &operator [] (const size_t Pos) { return _Datos[Pos]; }
			inline const size_t     Total(void) { return _Datos.size(); }
		   protected:
			std::vector<Dato>	   _Datos;
		};

		// Vector de valores visible
		class Valores {
		   public :
									Valores(void) { }
								   ~Valores(void) { }
			inline Valor           &operator [] (const size_t Pos) { return _Valores[Pos]; }
			inline void             Resize(const size_t nTam) { _Valores.resize(nTam); }
		   protected :
			std::vector<Valor>     _Valores;
		};


													DAnimacion(void);
		                                           ~DAnimacion(void);

													// Iniciar para un solo valor (desde - hasta)
		void										Iniciar(const double Desde, const double Hasta, const DWORD Milisegundos, std::function<void(DAnimacion::Valores &, const BOOL)> LambdaCallback, FuncionTiempo Funcion = FUNCION_POR_DEFECTO, const DWORD Intervalo = 16);
													// Iniciar para dos valores (desde - hasta)
		void										Iniciar(const double Desde0, const double Hasta0, const double Desde1, const double Hasta1, const DWORD Milisegundos, std::function<void(DAnimacion::Valores &, const BOOL)> LambdaCallback, FuncionTiempo Funcion = FUNCION_POR_DEFECTO);
													// Iniciar para 1 color
		void										Iniciar(const COLORREF Desde0, const COLORREF Hasta0, const DWORD Milisegundos, std::function<void(Valores &, const BOOL)> LambdaCallback, FuncionTiempo Funcion = FUNCION_POR_DEFECTO);
													// Iniciar para 2 colores
		void										Iniciar(const COLORREF Desde0, const COLORREF Hasta0, const COLORREF Desde1, const COLORREF Hasta1, const DWORD Milisegundos, std::function<void(Valores &, const BOOL)> LambdaCallback, FuncionTiempo Funcion = FUNCION_POR_DEFECTO);
													// Iniciar para 3 colores
		void										Iniciar(const COLORREF Desde0, const COLORREF Hasta0, const COLORREF Desde1, const COLORREF Hasta1, const COLORREF Desde2, const COLORREF Hasta2, const DWORD Milisegundos, std::function<void(Valores &, const BOOL)> LambdaCallback, FuncionTiempo Funcion = FUNCION_POR_DEFECTO);
													// Iniciar para 4 colores
		void										Iniciar(const COLORREF Desde0, const COLORREF Hasta0, const COLORREF Desde1, const COLORREF Hasta1, const COLORREF Desde2, const COLORREF Hasta2, const COLORREF Desde3, const COLORREF Hasta3, const DWORD Milisegundos, std::function<void(Valores &, const BOOL)> LambdaCallback, FuncionTiempo Funcion = FUNCION_POR_DEFECTO);

		void										Iniciar(DAnimacion::Datos &Datos, const DWORD Milisegundos, std::function<void(DAnimacion::Valores &, const BOOL)> LambdaCallback, const DWORD Intervalo = 16);
//		void                                        Invertir(void);
		void										Terminar(void);
		inline const BOOL                           Animando(void) { return (_Timer != NULL); }
		inline const DWORD							TiempoActual(void) { return _TiempoActual; }
	   protected:
 	    void                                       _ActualizarDecimal(const size_t i, const double Pos);
		void                                       _ActualizarRGB(const size_t i, const double Pos);
		static void CALLBACK                       _TimerProc(PVOID lpParameter, BOOLEAN TimerOrWaitFired);
		std::function<void(Valores &, const BOOL)> _Callback;
		HANDLE                                     _Timer;
		DWORD                                      _Duracion;
		DWORD                                      _TiempoActual;
		DWORD                                      _TickInicio;
		Datos                                      _Datos;
		Valores                                    _Valores;

	};

	/*

	class DAnimacionColor {
	   public:
		// Objeto que contiene el color origen y el color destino de la animación (separado en canales RGB)
		// En definitiva equivale a animar los 3 canales RGB del color partiendo de un único varlor RGB
		class Colores {
	   		public:
								Colores(void) : Desde(0), Hasta(0) { };
								Colores(const COLORREF nDesde, const COLORREF nHasta) : Desde(nDesde), Hasta(nHasta) { };
			                   ~Colores(void) { };
			inline const BYTE	DesdeR(void) { return GetRValue(Desde); };
			inline const BYTE	DesdeG(void) { return GetGValue(Desde); };
			inline const BYTE	DesdeB(void) { return GetBValue(Desde); };
			inline const BYTE	HastaR(void) { return GetRValue(Hasta); };
			inline const BYTE	HastaG(void) { return GetGValue(Hasta); };
			inline const BYTE	HastaB(void) { return GetBValue(Hasta); };
			COLORREF			Desde;
			COLORREF			Hasta;
		};
																	DAnimacionColor();
		                                                           ~DAnimacionColor();

																	// Iniciar para un solo valor (desde - hasta)
		void														Iniciar(const COLORREF Desde, const COLORREF Hasta, const DWORD Milisegundos, std::function<void(std::vector<COLORREF> &, const BOOL)> LambdaCallback);
																	// Iniciar para dos valores (desde - hasta)
		void														Iniciar(const COLORREF Desde1, const COLORREF Hasta1, const COLORREF Desde2, const COLORREF Hasta2, const DWORD Milisegundos, std::function<void(std::vector<COLORREF> &, const BOOL)> LambdaCallback);
																	// Iniciar para tres valores (desde - hasta)
		void														Iniciar(const COLORREF Desde1, const COLORREF Hasta1, const COLORREF Desde2, const COLORREF Hasta2, const COLORREF Desde3, const COLORREF Hasta3, const DWORD Milisegundos, std::function<void(std::vector<COLORREF> &, const BOOL)> LambdaCallback);
																	// Iniciar para cuatro valores (desde - hasta)
		void														Iniciar(const COLORREF Desde1, const COLORREF Hasta1, const COLORREF Desde2, const COLORREF Hasta2, const COLORREF Desde3, const COLORREF Hasta3, const COLORREF Desde4, const COLORREF Hasta4, const DWORD Milisegundos, std::function<void(std::vector<COLORREF> &, const BOOL)> LambdaCallback);
																	// Iniciar para un vector de valores (cada valor tiene un desde y un hasta)
		void														Iniciar(std::vector<Colores> &Valores, const DWORD Milisegundos, std::function<void(std::vector<COLORREF> &, const BOOL)> LambdaCallback);
																	// Invierte la animación (mientras se este ejecutando)
		void                                                        Invertir(void);
		void														Terminar(void);
		inline const BOOL                                           Animando(void) { return (_Timer != NULL);  }
		inline const DWORD                                          TiempoActual(void) { return _TiempoActual;  }
	protected:
		static void CALLBACK									   _TimerProc(PVOID lpParameter, BOOLEAN TimerOrWaitFired);
		std::function<void(std::vector<COLORREF> &, const BOOL)>   _Callback;
		HANDLE                                                     _Timer;
		DWORD                                                      _Duracion;
		DWORD                                                      _TiempoActual;
		std::vector<Colores>                                       _Datos;
		std::vector<COLORREF>                                      _Valores;

	};*/
};
#endif