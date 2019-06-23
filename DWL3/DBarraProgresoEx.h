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
						   ~DBarraEx_Skin(void) { }
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
							// Fuente
		int					FuenteTam;
		std::wstring		FuenteNombre;
		BOOL				FuenteNegrita;
		BOOL				FuenteCursiva;
//		BOOL				FuenteSubrayado;
		BOOL				FuenteSombraTexto;

	};

	enum DBarraEx_Estado {
		DBarraEx_Estado_Normal,
		DBarraEx_Estado_Resaltado,
		DBarraEx_Estado_Presionado
	};

	enum DBarraEx_Transicion {
		DBarraEx_Transicion_Normal,
		DBarraEx_Transicion_Resaltado,
		DBarraEx_Transicion_Presionado,
		DBarraEx_Transicion_Desactivado
	};

	enum DBarraEx_MostrarValor {
		DBarraEx_MostrarValor_Nada,							//
		DBarraEx_MostrarValor_Valor2Decimales,				// 0.00
		DBarraEx_MostrarValor_ValorInt,						// 0
		DBarraEx_MostrarValor_ValorMaximo2Decimales,		// 0.00 / 1.00
		DBarraEx_MostrarValor_ValorMaximoInt,				// 0 / 1
	};

	enum DBarraEx_Alineacion {
		IzquierdaDerecha,
		DerechaIzquierda,
		ArribaAbajo,
		AbajoArriba
	};

	class DBarraProgresoEx : public DControlEx {
	  public:
											DBarraProgresoEx(void);
							               ~DBarraProgresoEx(void);
		HWND								CrearBarraProgresoEx(DhWnd *nPadre, const int cX, const int cY, const int cAncho, const int cAlto, const INT_PTR cID, const float nMinimo = 0.0f, const float nMaximo = 1.0f, const float nValor = 0.0, const DBarraEx_Alineacion nAlineacion = IzquierdaDerecha);

		virtual void						Activado(const BOOL nActivar);
		inline const BOOL					Activado(void) { return DhWnd::Activado(); }


											// Mínimo, Máximo, Valor
		void								Minimo(const float nMinimo);
		virtual void						Valor(const float nValor);
		void								Maximo(const float nMaximo);
		inline const float					Minimo(void)											{ return _Minimo;					}
		inline const float					Maximo(void)											{ return _Maximo;					}
		virtual inline const float			Valor(void)												{ return _Valor;					}

		void								PintarBarraEx(HDC DC, const int nX = 0, const int nY = 0);

		// virtuales
		virtual void						Evento_PintarBorde(HDC DC, RECT &RBorde);
		virtual void						Evento_PintarBarra(HDC DC, RECT &RBarra);
		virtual void						Evento_PintarFondo(HDC DC, RECT &RFondo);
		virtual void						Evento_PintarValor(HDC DC, RECT &RFondo);
											// Permite modificar el valor formateado antes de pintarlo en el Evento_PintarValor
		virtual void						Evento_FormatearValor(std::wstring &ValorFormateado)	{ };

		virtual void						Transicion(const DBarraEx_Transicion nTransicion);
		LRESULT CALLBACK					GestorMensajes(UINT uMsg, WPARAM wParam, LPARAM lParam);

		inline void							MostrarValor(const DBarraEx_MostrarValor nMostrarValor)	{ _MostrarValor = nMostrarValor;	}
		inline const DBarraEx_MostrarValor  MostrarValor(void)										{ return _MostrarValor;				}

		void                                Alineacion(const DBarraEx_Alineacion nAlineacion);

		DBarraEx_Skin						Skin;
	  protected:
		const float                        _ValorMouse(RECT &RC, int cX, int cY);
		void				               _Evento_MouseMovimiento(WPARAM wParam, LPARAM lParam);
		void		                       _Evento_MouseSaliendo(void);

		COLORREF				           _ColorBarra;
		COLORREF		                   _ColorBorde;
		COLORREF		                   _ColorFondo;
		float			         		   _Minimo;
		float					           _Maximo;
		float		         			   _Valor;
		DBarraEx_Estado		         	   _Estado;
		DAnimacion                         _AniTransicion;
		DBarraEx_MostrarValor              _MostrarValor;
		BOOL                               _Activado;
		DBarraEx_Alineacion                _Alineacion;

		DFuente                            _Fuente;

		friend class DMenuEx;
	};	


}
#endif