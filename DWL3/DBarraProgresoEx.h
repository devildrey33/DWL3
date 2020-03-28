#ifndef DBARRAPROGRESOEX_H
#define DBARRAPROGRESOEX_H

#include "DControlEx.h"
#include "DAnimacion.h"
#include "DFuente.h"

namespace DWL {

	#define DBARRAEX_DEBUG FALSE

	class DMenuEx;
//	class DMenuEx2;

	// Colores por defecto de un control EX con barra de scroll
	class DBarraEx_Skin {
	   public :
							DBarraEx_Skin(void);
						   ~DBarraEx_Skin(void)		{ }
							// Colores para la barra de scroll (para la transición normal / hover / presionado)
		COLORREF			BarraNormal;
		COLORREF			BarraResaltado;
		COLORREF			BarraPresionado;
		COLORREF			BarraDesactivado;
							// Colores del borde del control (para la transición normal / hover / presionado)
		COLORREF			BordeNormal;
		COLORREF			BordeResaltado;
		COLORREF			BordePresionado;
							// Colores del fondo del control (para la transición normal / hover / presionado)
		COLORREF			FondoNormal;
		COLORREF			FondoResaltado;
		COLORREF			FondoPresionado;
		COLORREF			FondoDesactivado;
							// Color del texto
		COLORREF			Texto;
		COLORREF			TextoSombra;
							// Fuente (estos miembros solo se usan al crear la barra, una vez creado si queremos modificar la fuente hay que usar el miembro DBarraProgresoEx::Fuente)
		int					FuenteTam;
		std::wstring		FuenteNombre;
		BOOL				FuenteNegrita;
		BOOL				FuenteCursiva;
//		BOOL				FuenteSubrayado;
		BOOL				FuenteSombraTexto;
	};

	// Estado de la barra
	enum DBarraEx_Estado {
		DBarraEx_Estado_Normal,
		DBarraEx_Estado_Resaltado,
		DBarraEx_Estado_Presionado
	};

	// Transiciones para la barra
	enum DBarraEx_Transicion {
		DBarraEx_Transicion_Normal,
		DBarraEx_Transicion_Resaltado,
		DBarraEx_Transicion_Presionado,
		DBarraEx_Transicion_Desactivado
	};

	// Como se mostrará el valor en la barra
	enum DBarraEx_MostrarValor {
		DBarraEx_MostrarValor_Nada,							//
		DBarraEx_MostrarValor_Valor2Decimales,				// 0.00
		DBarraEx_MostrarValor_ValorInt,						// 0
		DBarraEx_MostrarValor_ValorMaximo2Decimales,		// 0.00 / 1.00
		DBarraEx_MostrarValor_ValorMaximoInt,				// 0 / 1
	};

	// Alineación de la barra
	enum DBarraEx_Alineacion {
		IzquierdaDerecha,
		DerechaIzquierda,
		ArribaAbajo,
		AbajoArriba
	};

	class DBarraProgresoEx : public DControlEx {
	  public: //////////////////////////////////// Constructor
												DBarraProgresoEx(void);
												// Destructor
								               ~DBarraProgresoEx(void);
											    // Función que crea la barra de progreso
		HWND									CrearBarraProgresoEx(DhWnd *nPadre, const int cX, const int cY, const int cAncho, const int cAlto, const INT_PTR cID, const float nMinimo = 0.0f, const float nMaximo = 1.0f, const float nValor = 0.0, const DBarraEx_Alineacion nAlineacion = IzquierdaDerecha);
												// Función para activar / desactivar la barra
		virtual void							Activado(const BOOL nActivar);
												// Función que devuelve si la barra está activada
		inline const BOOL						Activado(void) { return DhWnd::Activado(); }
												// Función para asignar el valor mínimo de la barra
		void									Minimo(const float nMinimo);
												// Función para asignar el valor actual de la barra
		virtual void							Valor(const float nValor);
												// Función para asignar el valor máximo de la barra
		void									Maximo(const float nMaximo);
												// Función que devuelve el valor mínimo de la barra
		inline const float						Minimo(void)											{ return _Minimo;					}
												// Función que devuelve el valor máximo de la barra
		inline const float						Maximo(void)											{ return _Maximo;					}
												// Función que devuelve el valor actual de la barra
		virtual inline const float				Valor(void)												{ return _Valor;					}
												// Función que pinta la barra en el DC y coordenadas especificadas
		void									PintarBarraEx(HDC DC, const int nX = 0, const int nY = 0);
												// Calcula la recta de la barra con los valores especificados según la alineación actual)
		void									CalcularBarra(float nMinimo, float nMaximo, float nValor, RECT &IN_RectaCliente, RECT &OUT_Barra);

		////////////////////////////////////////// Los eventos de pintado se ejecutan en el mismo orden que están declarados											
												// Evento virtual para pintar la parte de la barra ocupada
		virtual void							Evento_PintarBarra(HDC DC, RECT &RBarra);
												// Evento virtual para pintar el fondo
		virtual void							Evento_PintarFondo(HDC DC, RECT &RFondo);
												// Evento virtual en el que puedes pintar lo que quieras encima de la barra ya pintada
		virtual void							Evento_PintarPersonalizado(HDC DC, RECT &RFondo)		{ }
												// Permite modificar el valor formateado antes de pintarlo en el Evento_PintarValor
		virtual void							Evento_FormatearValor(std::wstring &ValorFormateado)	{ };
												// Evento lambda que permite modificar el valor formateado antes de pintar-lo en el Evento_PintarValor
		std::function<void(std::wstring&)>		EventoFormatearValor;		
												// Evento virtual para pintar el valor
		virtual void							Evento_PintarValor(HDC DC, RECT &RFondo);
												// Evento virtual para pintar el borde de la barra
		virtual void							Evento_PintarBorde(HDC DC, RECT &RBorde);
												// Función que aplica la transición de estado de la barra
		virtual void							Transicion(const DBarraEx_Transicion nTransicion);
												// Función que devuelve el tipo de DhWnd 
		virtual const DhWnd_Tipo				TipoWnd(void) { return DhWnd_Tipo_BarraProgresoEx; };
												// Gestor de mensajes para la barra
		LRESULT CALLBACK						GestorMensajes(UINT uMsg, WPARAM wParam, LPARAM lParam);
												// Función que permite especificar si se mostrará y como el valor de la barra
		inline void								MostrarValor(const DBarraEx_MostrarValor nMostrarValor)	{ _MostrarValor = nMostrarValor;	}
												// Función que devuelve como se muestra el valor de la barra
		inline const DBarraEx_MostrarValor		MostrarValor(void)										{ return _MostrarValor;				}
												// Función que permite especificar la alineación de la barra
		void									Alineacion(const DBarraEx_Alineacion nAlineacion);
												// Objeto que contiene la configuración de colores para la barra
		DBarraEx_Skin							Skin;
	  protected: ///////////////////////////////// Función que devuelve el porcentaje de la barra partiendo del tamaño y coordenadas especificadas
		const float                            _ValorMouse(RECT &RC, int cX, int cY);
												// Función que procesa el mensaje WM_MOUSEMOVE
		void				                   _Evento_MouseMovimiento(WPARAM wParam, LPARAM lParam);
												// Función que procesa el mensaje WM_MOUSELEAVE
		void		                           _Evento_MouseSaliendo(void);
												// Color actual de la barra
		COLORREF				               _ColorBarra;
												// Color actual del borde
		COLORREF		                       _ColorBorde;
												// Color actual del fondo
		COLORREF    		                   _ColorFondo;
												// Valor mínimo
		float		    	         		   _Minimo;
												// Valor máximo
		float			    		           _Maximo;
												// Valor actual
		float		             			   _Valor;
												// Estado de la barra
		DBarraEx_Estado		             	   _Estado;
												// Objeto para realizar las transiciones de color
		DAnimacion                             _AniTransicion;
												// Variable que controla si hay que mostrar el valor y como
		DBarraEx_MostrarValor                  _MostrarValor;
												// Variable que controla si el control está activado
		BOOL                                   _Activado;
												// Variable que contiene la alineación de la barra
		DBarraEx_Alineacion                    _Alineacion;
												// Fuente para la barra
		DFuente                                _Fuente;
												// Recta del area roja de la barra
		RECT                                   _RBarra;
												// Recta solo con el fondo blanco restante
		RECT							       _RFondo;

		friend class DMenuEx;					// Esta clase es amiga de DMenuEx
	};	


}
#endif