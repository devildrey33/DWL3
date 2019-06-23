#include "pch.h"
#include "DBarraScrollEx.h"

namespace DWL {

	DBarraScrollEx_Skin::DBarraScrollEx_Skin(void) :
		// Colores para la barra de scroll
		FondoScrollNormal		(COLOR_SCROLL_FONDO),
		FondoScrollResaltado	(COLOR_SCROLL_FONDO_RESALTADO),
		FondoScrollPresionado	(COLOR_SCROLL_FONDO_PRESIONADO),
		BarraScrollNormal		(COLOR_SCROLL_BARRA),
		BarraScrollResaltado	(COLOR_SCROLL_BARRA_RESALTADO),
		BarraScrollPresionado	(COLOR_SCROLL_BARRA_PRESIONADO),
		// Colores del orde del control
		BordeNormal				(COLOR_BORDE),
		BordeResaltado			(COLOR_BORDE_RESALTADO),
	//	COLORREF    DBarraScrollEx_Skin::BordePresionado		= COLOR_BORDE_PRESIONADO;
		// Colores del fondo del control
		FondoNormal				(COLOR_FONDO_CLARO),
		FondoResaltado			(COLOR_FONDO_CLARO_RESALTADO) {
	}


	// Minimo de tamaño para la barra de scroll
	#define BARRA_MIN_TAM 20

	DBarraScrollEx::DBarraScrollEx(void) : DControlEx(),
											_Scroll_PosPresionado({ 0 ,0 })						, _Scroll_PosInicio(0.0f),
											_ColorBorde(SkinScroll.BordeNormal)					, _ColorFondoV(SkinScroll.FondoScrollNormal)	,  _ColorFondoH(SkinScroll.FondoScrollNormal),
											_ColorBarraV(SkinScroll.BarraScrollNormal)			, _ColorBarraH(SkinScroll.BarraScrollNormal)	,	 _ColorFondo(SkinScroll.FondoNormal),
											_ScrollV_Estado(DBarraScrollEx_Estado_Invisible)	, _ScrollV_Pagina(1.0f)							,	 _ScrollV_Posicion(0.0f),
											_ScrollH_Estado(DBarraScrollEx_Estado_Invisible)	, _ScrollH_Pagina(1.0f)							,	 _ScrollH_Posicion(0.0f)   {
		_ScrollH_Alto  = GetSystemMetrics(SM_CYHSCROLL);	
		_ScrollV_Ancho = GetSystemMetrics(SM_CXVSCROLL);
	}


	DBarraScrollEx::~DBarraScrollEx(void) {
	}

	void DBarraScrollEx::PintarBorde(RECT *Recta, HDC hDC) {
		HBRUSH BrochaBorde = CreateSolidBrush(_ColorBorde);
		FrameRect(hDC, Recta, BrochaBorde);
		DeleteObject(BrochaBorde);
	}

	void DBarraScrollEx::Scrolls_Pintar(HDC hDC, RECT &RC) {
		// Si no hay un scroll visible salgo
		if (_ScrollV_Estado == DBarraScrollEx_Estado_Invisible && _ScrollH_Estado == DBarraScrollEx_Estado_Invisible) return;

		BOOL PintarRecuadro = TRUE;		// Determina si hay que pintar el recuadro inferior derecha
		RECT RCV, RCH, RCBV, RCBH;		// Rectas para la barra y el fondo tanto vertical como horizontal
		// Obtengo las rectas para el scroll H y V
		ObtenerRectasScroll(RC, RCH, RCV);
		
		// Pinto la barra de scroll si no es invisible
		if (_ScrollV_Estado != DBarraScrollEx_Estado_Invisible) {
			// Obtengo las rectas para las barras de scroll V
			ObtenerRectaBarraScrollV(RCV, RCBV);
			// Pinto la barra de scroll V
			_PintarBarraScrollEx(hDC, RCV, RCBV, _ColorBarraV, _ColorFondoV);
		}
		// La barra es invisible, no será necesario pintar el recuadro
		else {
			PintarRecuadro = FALSE;
		}
		// Pinto la barra de scroll si no es invisible
		if (_ScrollH_Estado != DBarraScrollEx_Estado_Invisible) {
			// Obtengo las rectas para las barras de scroll H
			ObtenerRectaBarraScrollH(RCH, RCBH);
			// Pinto la barra de scroll H
			_PintarBarraScrollEx(hDC, RCH, RCBH, _ColorBarraH, _ColorFondoH);
		}
		// La barra es invisible, no será necesario pintar el recuadro
		else {
			PintarRecuadro = FALSE;
		}

		// Si las dos barras son visibles hay que pintar un recuadro en la parte inferior derecha del control 
		if (PintarRecuadro == TRUE) {
			RECT Recuadro = { RCV.left, RCH.top, RCV.right, RCH.bottom };
			HBRUSH BrochaRecuadro = CreateSolidBrush(SkinScroll.FondoScrollNormal);
			FillRect(hDC, &Recuadro, BrochaRecuadro);
			DeleteObject(BrochaRecuadro);
		}
	}

	/* Obtiene el área que pertenece al control (RectaCliente es el resultado de GetClientRect, y RectaClienteSinScroll es el área del control excluyendo las barras de scroll) */
	void DBarraScrollEx::ObtenerRectaCliente(RECT *RectaCliente, RECT *RectaClienteSinScroll, RECT *RectaBorde) {
		GetClientRect(hWnd(), RectaBorde);
		*RectaCliente = *RectaBorde;
		// Quito el borde a la recta cliente
		RectaCliente->left += 2; RectaCliente->top += 2; RectaCliente->right -= 2; RectaCliente->bottom -= 2;
		*RectaClienteSinScroll = *RectaCliente;
		// Si la barra V no es invisible quito su espacio de la recta cliente
		if (_ScrollV_Estado != DBarraScrollEx_Estado_Invisible) RectaClienteSinScroll->right -= _ScrollV_Ancho;
		// Si la barra H no es invisible quito su espacio de la recta cliente
		if (_ScrollH_Estado != DBarraScrollEx_Estado_Invisible) RectaClienteSinScroll->bottom -= _ScrollH_Alto;
	}


	/* Obtiene el área que pertenece los scrolls Horizontal y Vertical
		IN_RC : debe ser la recta cliente del control previamente obtenida con GetClientRect
	*/
	void DBarraScrollEx::ObtenerRectasScroll(RECT &IN_RC, RECT &OUT_RectaH, RECT &OUT_RectaV) {
		RECT RC = IN_RC;
		// Quito el borde
//		RC.left += 2; RC.top += 2; RC.right -= 2; RC.bottom -= 2;
		// Calculo las rectas V y H
		OUT_RectaH = RC; OUT_RectaH.top = OUT_RectaH.bottom - _ScrollH_Alto;
		OUT_RectaV = RC; OUT_RectaV.left = OUT_RectaV.right - _ScrollV_Ancho;
		// Si las dos barras están visibles, hago espacio para el recuadro inferior izquierda
		if (_ScrollH_Estado != DBarraScrollEx_Estado_Invisible && _ScrollV_Estado != DBarraScrollEx_Estado_Invisible) {
			OUT_RectaH.right  -= _ScrollH_Alto;
			OUT_RectaV.bottom -= _ScrollV_Ancho;
		}
	}

	/* Obtiene el área que pertenece los scrolls Horizontal y Vertical */
	void DBarraScrollEx::ObtenerRectasScroll(RECT &RectaH, RECT &RectaV) {
		// Obtengo la recta cliente
		RECT Recta;
		GetClientRect(hWnd(), &Recta);
		// Quito el borde
//		Recta.left += 2; Recta.top += 2; Recta.right -= 2; Recta.bottom -= 2;
		// Calculo las rectas V y H
		RectaH = Recta; RectaH.top = RectaH.bottom - _ScrollH_Alto;
		RectaV = Recta; RectaV.left = RectaV.right - _ScrollV_Ancho;
		// Si las dos barras están visibles, hago espacio para el recuadro inferior izquierda
		if (_ScrollH_Estado != DBarraScrollEx_Estado_Invisible && _ScrollV_Estado != DBarraScrollEx_Estado_Invisible) {
			RectaH.right  -= _ScrollH_Alto;
			RectaV.bottom -= _ScrollV_Ancho;
		}
	}

	/* Obtiene el área de la barra de scroll vertical (sin el fondo) */
	void DBarraScrollEx::ObtenerRectaBarraScrollV(RECT &RectaScroll, RECT &RectaBarra) {
		// Calculo el tamaño de la barra V
		int TamBarra = static_cast<int>(static_cast<float>(RectaScroll.bottom) * _ScrollV_Pagina);
		if (TamBarra < BARRA_MIN_TAM) TamBarra = BARRA_MIN_TAM;
		// Calculo la recta de la barra V
		RectaBarra = {  2 + RectaScroll.left,																		// left
						4 + static_cast<int>(_ScrollV_Posicion * (RectaScroll.bottom - TamBarra)),					// top
					   -2 + RectaScroll.right,																		// right
					   -2 + static_cast<int>((_ScrollV_Posicion * (RectaScroll.bottom - TamBarra)) + TamBarra) };	// bottom
	}

	/* Obtiene el área de la barra de scroll horizontal (sin el fondo) */
	void DBarraScrollEx::ObtenerRectaBarraScrollH(RECT &RectaScroll, RECT &RectaBarra) {
		// Calculo el tamaño de la barra H
		int TamBarra = static_cast<int>(static_cast<float>(RectaScroll.right) * _ScrollH_Pagina);
		if (TamBarra < BARRA_MIN_TAM) TamBarra = BARRA_MIN_TAM;
		// Calculo la recta de la barra H
		RectaBarra = {  4 + static_cast<int>(_ScrollH_Posicion * (RectaScroll.right - TamBarra)),					// left
						2 + RectaScroll.top,																		// top
					   -2 + static_cast<int>((_ScrollH_Posicion * (RectaScroll.right - TamBarra)) + TamBarra),		// right
					   -2 + RectaScroll.bottom																	};	// bottom
	}

	// S'ha de recalcular de forma que el maxim sigui 1, i li haig de restar al tamany total, el tamany d'una pagina
	const float DBarraScrollEx::_CalcularPosScrollH(const UINT nTam, const int nPos) {		
		// Calculo el tamaño de la barra
		int TamBarra = static_cast<int>(static_cast<float>(nTam) * _ScrollH_Pagina);
		if (TamBarra < BARRA_MIN_TAM) TamBarra = BARRA_MIN_TAM;
		// Tamaño restante para el scroll
		float tam = static_cast<float>(nTam) - TamBarra;
		// Calculo la nueva posición
		float NuevaPos = _Scroll_PosInicio + (1.0f / tam) * static_cast<float>(nPos - _Scroll_PosPresionado.x);
		// Si la posición se sale de 0.0 o 1.0 la reajusto
		if (NuevaPos > 1.0f)	NuevaPos = 1.0f;
		if (NuevaPos < 0.0f)	NuevaPos = 0.0f;

		#if DBARRASCROLLEX_MOSTRARDEBUG == TRUE
			Debug_Escribir_Varg(L"DBarraScrollEx::_CalcularPosScrollH  NuevaPos = %0.2f\n", NuevaPos);
		#endif

		return NuevaPos;
	}

	const float DBarraScrollEx::_CalcularPosScrollV(const UINT nTam, const int nPos) {
		// Calculo el tamaño de la barra
		int TamBarra = static_cast<int>(static_cast<float>(nTam) * _ScrollV_Pagina);
		if (TamBarra < BARRA_MIN_TAM) TamBarra = BARRA_MIN_TAM;
		// Tamaño restante para el scroll
		float tam = static_cast<float>(nTam) - TamBarra;
		// Calculo la nueva posición
		float NuevaPos = _Scroll_PosInicio + (1.0f / tam) * static_cast<float>(nPos - _Scroll_PosPresionado.y);
		// Si la posición se sale de 0.0 o 1.0 la reajusto
		if (NuevaPos > 1.0f)	NuevaPos = 1.0f;
		if (NuevaPos < 0.0f)	NuevaPos = 0.0f;

		#if DBARRASCROLLEX_MOSTRARDEBUG == TRUE
			Debug_Escribir_Varg(L"DBarraScrollEx::_CalcularPosScrollV  NuevaPos = %0.2f\n", NuevaPos);
		#endif

		return NuevaPos;
	}

	// Devuelve TRUE si el mouse está dentro de alguna barra de scroll
	const BOOL DBarraScrollEx::Scrolls_MouseMovimiento(DEventoMouse &DatosMouse) {
		RECT RCV, RCH, RectaBarra;
		ObtenerRectasScroll(RCH, RCV);
		POINT Pt = { DatosMouse.X(), DatosMouse.Y() };
		BOOL  RetV = FALSE, RetH = FALSE, RetB = FALSE;
		// Compruebo el estado vertical
		if (_ScrollV_Estado != DBarraScrollEx_Estado_Invisible) {
			// Si el estado no es presionado
			if (_ScrollV_Estado != DBarraScrollEx_Estado_Presionado) {
				ObtenerRectaBarraScrollV(RCV, RectaBarra);
				RetV = PtInRect(&RCV, Pt);			// Espacio del fondo del scroll vertical
				RetB = PtInRect(&RectaBarra, Pt);	// Espacio de la barra del scroll vertical
				// Dentro de la barra de scroll vertical 
				if (RetV == TRUE && RetB == TRUE && _ScrollV_Estado != DBarraScrollEx_Estado_BarraResaltada) {
					_ScrollV_Estado = DBarraScrollEx_Estado_BarraResaltada;
					#if DBARRASCROLLEX_MOSTRARDEBUG == TRUE
						Debug_Escribir(L"DBarraScrollEx::Scrolls_MouseMovimiento  _ScrollV_Estado = DBarraScrollEx_Estado_BarraResaltada\n");
					#endif
					ScrollV_Transicion(DBarraScrollEx_Transicion_BarraResaltada);
					return TRUE;
				}
				// Dentro del fondo del scroll vertical 
				else if (RetV == TRUE && RetB == FALSE && _ScrollV_Estado != DBarraScrollEx_Estado_FondoResaltado) {
					_ScrollV_Estado = DBarraScrollEx_Estado_FondoResaltado;
					#if DBARRASCROLLEX_MOSTRARDEBUG == TRUE
						Debug_Escribir(L"DBarraScrollEx::Scrolls_MouseMovimiento  _ScrollV_Estado = DBarraScrollEx_Estado_FondoResaltado\n");
					#endif
					ScrollV_Transicion(DBarraScrollEx_Transicion_FondoResaltado);
					return TRUE;
				}
				// Fuera del scroll vertical
				if (RetV == FALSE && _ScrollV_Estado != DBarraScrollEx_Estado_Normal) {
					_ScrollV_Estado = DBarraScrollEx_Estado_Normal;
					#if DBARRASCROLLEX_MOSTRARDEBUG == TRUE
						Debug_Escribir(L"DBarraScrollEx::Scrolls_MouseMovimiento  _ScrollV_Estado = DBarraScrollEx_Estado_Normal\n");
					#endif
					ScrollV_Transicion(DBarraScrollEx_Transicion_Normal);
					return TRUE;
				}
			}
			// ScrollV presionado
			else { 
				_ScrollV_Posicion = _CalcularPosScrollV(RCH.bottom, Pt.y);
				Scrolls_EventoCambioPosicion();
				Repintar();
				return TRUE;
			}
		}
		// Compruebo el estado horizontal
		if (_ScrollH_Estado != DBarraScrollEx_Estado_Invisible) {
			// Si el estado no es presionado
			if (_ScrollH_Estado != DBarraScrollEx_Estado_Presionado) {
				ObtenerRectaBarraScrollH(RCH, RectaBarra);
				RetB = PtInRect(&RectaBarra, Pt);	// Espacio de la barra del scroll vertical
				RetH = PtInRect(&RCH, Pt);
				// Dentro de la barra de scroll horizontal 
				if (RetH == TRUE && RetB == TRUE && _ScrollH_Estado != DBarraScrollEx_Estado_BarraResaltada) {
					_ScrollH_Estado = DBarraScrollEx_Estado_BarraResaltada;
					ScrollH_Transicion(DBarraScrollEx_Transicion_BarraResaltada);
					//Repintar();
					return TRUE;
				}
				// Dentro del fondo del scroll horizontal 
				else if (RetH == TRUE && RetB == FALSE && _ScrollH_Estado != DBarraScrollEx_Estado_FondoResaltado) {
					_ScrollH_Estado = DBarraScrollEx_Estado_FondoResaltado;
					ScrollH_Transicion(DBarraScrollEx_Transicion_FondoResaltado);
					//Repintar();
					return TRUE;
				}
				// Fuera del scroll horizontal
				if (RetH == FALSE && _ScrollH_Estado != DBarraScrollEx_Estado_Normal) {
					_ScrollH_Estado = DBarraScrollEx_Estado_Normal;
					//Repintar();
					ScrollH_Transicion(DBarraScrollEx_Transicion_Normal);
					return TRUE;
				}
			}
			// ScrollH presionado
			else { 
				_ScrollH_Posicion = _CalcularPosScrollH(RCH.right, Pt.x);
				#if DBARRASCROLLEX_MOSTRARDEBUG == TRUE
					Debug_Escribir_Varg(L"Scrolls_MouseMovimiento H:%.02f\n", _ScrollH_Posicion);
				#endif
				Scrolls_EventoCambioPosicion();
				Repintar();
				return TRUE;
			}
		}
		// El mouse no está encima de ningún scroll devuelvo FALSE
		return FALSE;
	}


	void DBarraScrollEx::ScrollV_Transicion(const DBarraScrollEx_Transicion nTransicion) {
		DWORD Duracion = DhWnd::TiempoAnimaciones;
		if (_ScrollV_AniTransicion.Animando() == TRUE) {
//			Duracion = _ScrollV_AniTransicion.TiempoActual();
			_ScrollV_AniTransicion.Terminar();
		}
//		_ColorFondoV = &_ColorFondoV_Real;
		COLORREF FondoHasta = 0, BarraHasta = 0;
		switch (nTransicion) {
			case DBarraScrollEx_Transicion_Normal:
				FondoHasta = SkinScroll.FondoScrollNormal;
				BarraHasta = SkinScroll.BarraScrollNormal;
				#if DBARRASCROLLEX_MOSTRARDEBUG == TRUE
					Debug_Escribir(L"DBarraScrollEx::ScrollV_Transicion DBarraScrollEx_Transicion_Normal\n");
				#endif
				break;
			case DBarraScrollEx_Transicion_BarraResaltada:
				FondoHasta = SkinScroll.FondoScrollNormal;
				BarraHasta = SkinScroll.BarraScrollResaltado;
				#if DBARRASCROLLEX_MOSTRARDEBUG == TRUE
					Debug_Escribir(L"DBarraScrollEx::ScrollV_Transicion DBarraScrollEx_Transicion_BarraResaltada\n");
				#endif
				break;
			case DBarraScrollEx_Transicion_FondoResaltado:
				FondoHasta = SkinScroll.FondoScrollResaltado;
				BarraHasta = SkinScroll.BarraScrollNormal;
				#if DBARRASCROLLEX_MOSTRARDEBUG == TRUE
					Debug_Escribir(L"DBarraScrollEx::ScrollV_Transicion DBarraScrollEx_Transicion_FondoResaltado\n");
				#endif
				break;
			case DBarraScrollEx_Transicion_Presionado:
				FondoHasta = SkinScroll.FondoScrollPresionado;
				BarraHasta = SkinScroll.BarraScrollPresionado;
				#if DBARRASCROLLEX_MOSTRARDEBUG == TRUE
					Debug_Escribir(L"DBarraScrollEx::ScrollV_Transicion DBarraScrollEx_Transicion_Presionado\n");
				#endif
				break;
		}

		_ScrollV_AniTransicion.Iniciar(_ColorFondoV, FondoHasta, _ColorBarraV, BarraHasta, Duracion, [=](DAnimacion::Valores &Datos, const BOOL Terminado) {
			_ColorFondoV = Datos[0].Color();
			_ColorBarraV = Datos[1].Color();
			RepintarAni();
/*			if (Terminado == TRUE) {
				_ColorFondoV = &_ColorFondo;
			}*/
		});
	}

	void DBarraScrollEx::ScrollH_Transicion(const DBarraScrollEx_Transicion nTransicion) {
		DWORD Duracion = DhWnd::TiempoAnimaciones;
		if (_ScrollH_AniTransicion.Animando() == TRUE) {
//			Duracion = _ScrollH_AniTransicion.TiempoActual();
			_ScrollH_AniTransicion.Terminar();
		}
//		_ColorFondoH = &_ColorFondoH_Real;
		COLORREF FondoHasta = 0, BarraHasta = 0;
		switch (nTransicion) {
			case DBarraScrollEx_Transicion_Normal:
				FondoHasta = SkinScroll.FondoScrollNormal;
				BarraHasta = SkinScroll.BarraScrollNormal;
				break;
			case DBarraScrollEx_Transicion_BarraResaltada:
				FondoHasta = SkinScroll.FondoScrollNormal;
				BarraHasta = SkinScroll.BarraScrollResaltado;
				break;
			case DBarraScrollEx_Transicion_FondoResaltado:
				FondoHasta = SkinScroll.FondoScrollResaltado;
				BarraHasta = SkinScroll.BarraScrollNormal;
				break;
			case DBarraScrollEx_Transicion_Presionado:
				FondoHasta = SkinScroll.FondoScrollPresionado;
				BarraHasta = SkinScroll.BarraScrollPresionado;
				break;
		}

		_ScrollH_AniTransicion.Iniciar(_ColorFondoH, FondoHasta, _ColorBarraH, BarraHasta, Duracion, [=](DAnimacion::Valores &Datos, const BOOL Terminado) {
			_ColorFondoH	= Datos[0].Color();
			_ColorBarraH		= Datos[1].Color();
			RepintarAni();
			/*if (Terminado == TRUE) {
				_ColorFondoH = &_ColorFondo;
			}*/
		});

	}

	
	void DBarraScrollEx::Scrolls_TransicionBorde(const DBarraScrollEx_Transicion_Borde nTransicion) {
		DWORD Duracion = DhWnd::TiempoAnimaciones;
		if (_Scrolls_AniTransicionBorde.Animando() == TRUE) {
//			Duracion = _ScrollH_AniTransicion.TiempoActual();
			_Scrolls_AniTransicionBorde.Terminar();
		}

		COLORREF BordeHasta = 0, FondoHasta = 0;
		switch (nTransicion) {
			case DBarraScrollEx_Transicion_Borde_Normal:
				BordeHasta = SkinScroll.BordeNormal;
				FondoHasta = SkinScroll.FondoNormal;
				break;
			case DBarraScrollEx_Transicion_Borde_Resaltado:
				BordeHasta = SkinScroll.BordeResaltado;
				FondoHasta = SkinScroll.FondoResaltado;
				break;
		}

		_Scrolls_AniTransicionBorde.Iniciar(_ColorBorde, BordeHasta, _ColorFondo, FondoHasta, Duracion + 25, [=](DAnimacion::Valores &Datos, const BOOL Terminado) {
			_ColorBorde = Datos[0].Color();
			_ColorFondo = Datos[1].Color();
			if (Terminado == FALSE) RepintarAni();	
			else                    Repintar();		// Aseguro que se repinte el ultimo frame
		});

	}


	// Devuelve TRUE si el mouse está dentro de alguna barra de scroll
	const BOOL DBarraScrollEx::Scrolls_MousePresionado(DEventoMouse &DatosMouse) {
		int cX		= DatosMouse.X(),
			cY		= DatosMouse.Y();

		if (DatosMouse.Boton != 0) return FALSE;
		RECT RCV, RCH, RCBV, RCBH;
		ObtenerRectasScroll(RCH, RCV);
		POINT Pt = { cX, cY };
		BOOL  RetV = FALSE, RetH = FALSE, RetBV = FALSE, RetBH = FALSE;
//		float NuevaPos = 0.0f;
		if (_ScrollV_Estado != DBarraScrollEx_Estado_Invisible) {
			RetV = PtInRect(&RCV, Pt);
			if (RetV == TRUE) { // Está dentro del área del scroll vertical
				ObtenerRectaBarraScrollV(RCV, RCBV);
				RetBV = PtInRect(&RCBV, Pt);
				if (RetBV == FALSE) { // Está en la parte del fondo (recalculo la posición de la barra antes de empezar el drag)
					_ScrollV_Posicion = _CalcularPosScrollV(RCH.bottom, cY);
				}
				SetCapture(hWnd());
				_Scroll_PosPresionado = { cX, cY };			// Posición del mouse desde donde se inicia el drag de la barra
				_Scroll_PosInicio     = _ScrollV_Posicion;	// Posición del scroll al iniciar el drag
				_ScrollV_Estado		  = DBarraScrollEx_Estado_Presionado;
				#if DBARRASCROLLEX_MOSTRARDEBUG == TRUE
					Debug_Escribir_Varg(L"Scrolls_MousePresionado V:%.02f\n", _ScrollV_Posicion);
				#endif
				Scrolls_EventoCambioPosicion();
				ScrollV_Transicion(DBarraScrollEx_Transicion_Presionado);
				return TRUE;
			}
		}
		if (_ScrollH_Estado != DBarraScrollEx_Estado_Invisible) {
			RetH = PtInRect(&RCH, Pt);
			if (RetH == TRUE) { // Está dentro del área del scroll horizontal
				ObtenerRectaBarraScrollH(RCH, RCBH);
				RetBH = PtInRect(&RCBH, Pt);
				if (RetBH == FALSE) { // Está en la parte del fondo (recalculo la posición de la barra antes de empezar el drag)
					_ScrollH_Posicion = _CalcularPosScrollH(RCH.right, cX);
				}
				SetCapture(hWnd());
				_Scroll_PosPresionado = { cX, cY };			// Posición desde donde se inicia el drag de la barra
				_Scroll_PosInicio	  = _ScrollH_Posicion;	// Posición del scroll al iniciar el drag
				_ScrollH_Estado		  = DBarraScrollEx_Estado_Presionado;
				#if DBARRASCROLLEX_MOSTRARDEBUG == TRUE
					Debug_Escribir_Varg(L"Scrolls_MousePresionado H:%.02f\n", _ScrollH_Posicion);
				#endif
				Scrolls_EventoCambioPosicion();
				ScrollH_Transicion(DBarraScrollEx_Transicion_Presionado);
				return TRUE;
			}
		}
		
		return FALSE;		
	}

	// Devuelve TRUE si el mouse está dentro de alguna barra de scroll
	const BOOL DBarraScrollEx::Scrolls_MouseSoltado(DEventoMouse &DatosMouse) {
		int cX		= DatosMouse.X(),
			cY		= DatosMouse.Y();

		if (DatosMouse.Boton != 0) return FALSE;
		RECT RCV, RCH, RectaBarra;
		ObtenerRectasScroll(RCH, RCV);
		POINT Pt = { cX, cY };
		BOOL  RetV = FALSE, RetH = FALSE, RetB = FALSE;
		ReleaseCapture();
		DBarraScrollEx_Transicion Trans = DBarraScrollEx_Transicion_Normal;
		if (_ScrollV_Estado == DBarraScrollEx_Estado_Presionado) {
			ObtenerRectaBarraScrollV(RCV, RectaBarra);
			RetV = PtInRect(&RCV, Pt);
			RetB = PtInRect(&RectaBarra, Pt);
			if		(RetV == TRUE && RetB == TRUE)	{ _ScrollV_Estado = DBarraScrollEx_Estado_BarraResaltada;	Trans = DBarraScrollEx_Transicion_BarraResaltada;	}
			else if (RetV == TRUE && RetB == FALSE) { _ScrollV_Estado = DBarraScrollEx_Estado_FondoResaltado;	Trans = DBarraScrollEx_Transicion_FondoResaltado;	}
			else									{ _ScrollV_Estado = DBarraScrollEx_Estado_Normal;			Trans = DBarraScrollEx_Transicion_Normal;			}
			_ScrollV_Posicion = _CalcularPosScrollV(RCH.bottom, cY);
			#if DBARRASCROLLEX_MOSTRARDEBUG == TRUE
				Debug_Escribir_Varg(L"Scrolls_MouseSoltado V:%.02f\n", _ScrollV_Posicion);
			#endif
			Scrolls_EventoCambioPosicion();
			ScrollV_Transicion(Trans);
			//Repintar();
			return TRUE;
		}
		if (_ScrollH_Estado == DBarraScrollEx_Estado_Presionado) {
			ObtenerRectaBarraScrollH(RCH, RectaBarra);
			RetH = PtInRect(&RCH, Pt);
			RetB = PtInRect(&RectaBarra, Pt);
			if		(RetH == TRUE && RetB == TRUE)	{ _ScrollH_Estado = DBarraScrollEx_Estado_BarraResaltada;	Trans = DBarraScrollEx_Transicion_BarraResaltada;	}
			else if (RetH == TRUE && RetB == FALSE) { _ScrollH_Estado = DBarraScrollEx_Estado_FondoResaltado;	Trans = DBarraScrollEx_Transicion_FondoResaltado;	}
			else									{ _ScrollH_Estado = DBarraScrollEx_Estado_Normal;			Trans = DBarraScrollEx_Transicion_Normal;			}
			_ScrollH_Posicion = _CalcularPosScrollH(RCH.right, cX);
			#if DBARRASCROLLEX_MOSTRARDEBUG == TRUE
				Debug_Escribir_Varg(L"Scrolls_MouseSoltado H:%.02f\n", _ScrollH_Posicion);
			#endif
			Scrolls_EventoCambioPosicion();
			ScrollH_Transicion(Trans);
			//Repintar();
			return TRUE;
		}

		return FALSE;		
	}

	void DBarraScrollEx::_PintarBarraScrollEx(HDC hDC, RECT &RectaScroll, RECT &RectaBarra, const COLORREF pColorBarra, const COLORREF pColorFondo) {
		// Creo las brochas 
		HBRUSH BrochaColorFondo = CreateSolidBrush(pColorFondo);
		HBRUSH BrochaColorBarra = CreateSolidBrush(pColorBarra);

		// Pinto el scrollbar
		FillRect(hDC, &RectaScroll, BrochaColorFondo);
		FillRect(hDC, &RectaBarra, BrochaColorBarra);

		// Borro las brochas de la memória
		DeleteObject(BrochaColorFondo);
		DeleteObject(BrochaColorBarra);
	}


	void DBarraScrollEx::ScrollH_Pagina(const float nPagina) {
		_ScrollH_Pagina = nPagina;
		Repintar();
	}

	void DBarraScrollEx::ScrollH_Posicion(const float nPosicion) {
		_ScrollH_Posicion = nPosicion;
		Repintar();
	}

	// Función para avanzar una página 
	void DBarraScrollEx::AvPag(void) {
		_ScrollV_Posicion += _ScrollV_Pagina;
		if (_ScrollV_Posicion > 1.0f) _ScrollV_Posicion = 1.0f;
		Scrolls_EventoCambioPosicion();
	}

	// Función para retroceder una página 
	void DBarraScrollEx::RePag(void) {
		_ScrollV_Posicion -= _ScrollV_Pagina;
		if (_ScrollV_Posicion < 0.0f) _ScrollV_Posicion = 0.0f;
		Scrolls_EventoCambioPosicion();
	}


	const float DBarraScrollEx::ScrollH_Pagina(void) {
		return _ScrollH_Pagina;
	}

	const float DBarraScrollEx::ScrollH_Posicion(void) {
		return _ScrollH_Posicion;
	}

	void DBarraScrollEx::ScrollH_Visible(const BOOL nVisible) {
		if (nVisible == TRUE)	_ScrollH_Estado = DBarraScrollEx_Estado_Normal;
		else                    _ScrollH_Estado = DBarraScrollEx_Estado_Invisible;
	}

	void DBarraScrollEx::ScrollV_Pagina(const float nPagina) {
		_ScrollV_Pagina = nPagina;
		Repintar();
	}

	void DBarraScrollEx::ScrollV_Posicion(const float nPosicion) {
		_ScrollV_Posicion = nPosicion;
		Repintar();
	}

	const float DBarraScrollEx::ScrollV_Pagina(void) {
		return _ScrollV_Pagina;
	}

	const float DBarraScrollEx::ScrollV_Posicion(void) {
		return _ScrollV_Posicion;
	}

	void DBarraScrollEx::ScrollV_Visible(const BOOL nVisible) {
		if (nVisible == TRUE)	_ScrollV_Estado = DBarraScrollEx_Estado_Normal;
		else                    _ScrollV_Estado = DBarraScrollEx_Estado_Invisible;
		#if DBARRASCROLLEX_MOSTRARDEBUG == TRUE
			Debug_Escribir_Varg(L"DBarraScrollEx::ScrollV_Visible  _ScrollV_Estado = %d\n", _ScrollV_Estado);
		#endif
	}

	// Devuelve TRUE si es necesario repintar, FALSE en caso contrario
	const BOOL DBarraScrollEx::Scrolls_MouseEntrando(void) {
		Scrolls_TransicionBorde(DBarraScrollEx_Transicion_Borde_Resaltado);
		#if DBARRASCROLLEX_MOSTRARDEBUG == TRUE
			Debug_Escribir(L"DBarraScrollEx::Scrolls_MouseEntrando\n");
		#endif
		return FALSE;
	}

	// Devuelve TRUE si es necesario repintar, FALSE en caso contrario
	const BOOL DBarraScrollEx::Scrolls_MouseSaliendo(void) {
		//BOOL nRepintar = FALSE;
		if (_ScrollV_Estado != DBarraScrollEx_Estado_Presionado && _ScrollH_Estado != DBarraScrollEx_Estado_Presionado) {
			if (_ScrollV_Estado != DBarraScrollEx_Estado_Invisible && _ScrollV_Estado != DBarraScrollEx_Estado_Normal) {
				#if DBARRASCROLLEX_MOSTRARDEBUG == TRUE
					Debug_Escribir(L"DBarraScrollEx::Scrolls_MouseSaliendo  _ScrollV_Estado = DBarraScrollEx_Estado_Normal\n");
				#endif
				_ScrollV_Estado = DBarraScrollEx_Estado_Normal;
				ScrollV_Transicion(DBarraScrollEx_Transicion_Normal);
				//nRepintar = TRUE;
			}
			if (_ScrollH_Estado != DBarraScrollEx_Estado_Invisible && _ScrollH_Estado != DBarraScrollEx_Estado_Normal) {
				_ScrollH_Estado = DBarraScrollEx_Estado_Normal;
				ScrollH_Transicion(DBarraScrollEx_Transicion_Normal);
				//nRepintar = TRUE;
			}			
		}
		#if DBARRASCROLLEX_MOSTRARDEBUG == TRUE
			Debug_Escribir(L"DBarraScrollEx::Scrolls_MouseSaliendo\n");
		#endif

		Scrolls_TransicionBorde(DBarraScrollEx_Transicion_Borde_Normal);
		return FALSE;
	}

	/*
	LRESULT CALLBACK DBarraScrollEx::GestorMensajes(UINT uMsg, WPARAM wParam, LPARAM lParam) {
		return DefWindowProc(hWnd(), uMsg, wParam, lParam);
	}*/

}