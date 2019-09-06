#include "pch.h"
#include "DBarraProgresoEx.h"
//#include "Rave_Skin.h"
#include "DMensajesWnd.h"
#include "DStringUtils.h"

namespace DWL {

	DBarraEx_Skin::DBarraEx_Skin(void) :
		// Colores para la barra de scroll (para la transición normal / hover / presionado)
		BarraNormal			(COLOR_BARRA),
		BarraResaltado		(COLOR_BARRA_RESALTADO),
		BarraPresionado		(COLOR_BARRA_PRESIONADO),
		BarraDesactivado	(COLOR_BARRA_DESACTIVADO),
		// Colores del borde del control (para la transición normal / hover / presionado)
		BordeNormal			(COLOR_BORDE),
		BordeResaltado		(COLOR_BORDE_RESALTADO),
		BordePresionado		(COLOR_BORDE_PRESIONADO),
		// Colores del fondo del control (para la transición normal / hover / presionado)
		FondoNormal			(COLOR_BARRA_FONDO),
		FondoResaltado		(COLOR_BARRA_FONDO_RESALTADO),
		FondoPresionado		(COLOR_BARRA_FONDO_PRESIONADO),
		FondoDesactivado	(COLOR_BARRA_FONDO_DESACTIVADO),
		// Color del texto	
		Texto				(COLOR_BARRA_TEXTO),
		TextoSombra			(COLOR_BARRA_TEXTO_SOMBRA),
		// Fuente
		FuenteTam			(FUENTE_PEQUE),
		FuenteNombre		(FUENTE_NOMBRE),
		FuenteNegrita		(FALSE),
		FuenteCursiva		(FALSE),
	//	FuenteSubrayado		(FALSE),
		// Pintar sombra al texto
		FuenteSombraTexto	(FALSE) { 
	}


	DBarraProgresoEx::DBarraProgresoEx(void) : _Minimo(0), _Maximo(0), _Valor(0), _Estado(DBarraEx_Estado_Normal), _ColorBarra(Skin.BarraNormal), _ColorFondo(Skin.FondoNormal), _ColorBorde(Skin.BordeNormal), _MostrarValor(DBarraEx_MostrarValor_Nada), _Activado(TRUE), _Alineacion(IzquierdaDerecha) {
	}


	DBarraProgresoEx::~DBarraProgresoEx(void) {
	}


	HWND DBarraProgresoEx::CrearBarraProgresoEx(DhWnd *nPadre, const int cX, const int cY, const int cAncho, const int cAlto, const INT_PTR cID, const float nMinimo, const float nMaximo, const float nValor, const DBarraEx_Alineacion nAlineacion) {
//		if (hWnd()) { Debug_Escribir(L"DBarraProgresoEx::CrearBarraProgresoEx() Error : ya se ha creado la barra\n"); return hWnd(); }
		_hWnd = CrearControlEx(nPadre, L"DBarraProgresoEx", L"", cID, cX, cY, cAncho, cAlto, WS_CHILD | WS_VISIBLE, NULL, CS_HREDRAW | CS_VREDRAW);
		_Fuente.CrearFuente(Skin.FuenteTam, Skin.FuenteNombre.c_str(), Skin.FuenteNegrita, Skin.FuenteCursiva);
//		ColorFondo = COLOR_BARRA_FONDO;
//		ColorBarra = COLOR_BARRA;
		_Alineacion = nAlineacion;
		_Minimo = nMinimo;
		_Maximo = nMaximo;
		_Valor = nValor;
		if (_Activado == TRUE) {
			_ColorBarra = Skin.BarraNormal;
			_ColorFondo = Skin.FondoNormal;
			_ColorBorde = Skin.BordeNormal;
		}
		else {
			_ColorBarra = Skin.BarraDesactivado;
			_ColorFondo = Skin.FondoDesactivado;
			_ColorBorde = Skin.BordeNormal;
			EnableWindow(_hWnd, FALSE);
		}
		return hWnd();
	}

	void DBarraProgresoEx::Minimo(const float nMinimo) {
		_Minimo = nMinimo;
		Repintar();
	}

	void DBarraProgresoEx::Maximo(const float nMaximo) {
		_Maximo = nMaximo;
		Repintar();
	}

	void DBarraProgresoEx::Valor(const float nValor) {
		_Valor = nValor;
		Repintar();
	}

	void DBarraProgresoEx::CalcularBarra(float nMinimo, float nMaximo, float nValor, RECT &IN_RectaCliente, RECT &OUT_Barra) {
		float Parte = 0.0f;		
		OUT_Barra = IN_RectaCliente;
/*		OUT_Barra.left++;
		OUT_Barra.top++;
		OUT_Barra.right--;
		OUT_Barra.bottom--;*/
		switch (_Alineacion) {
			case IzquierdaDerecha:
				Parte = static_cast<float>(IN_RectaCliente.right - IN_RectaCliente.left) / (nMaximo - nMinimo);
				OUT_Barra.right = static_cast<int>(Parte * (nValor - nMinimo));
				break;
			case DerechaIzquierda :
				Parte = static_cast<float>(IN_RectaCliente.right - IN_RectaCliente.left) / (nMaximo - nMinimo);
				OUT_Barra.left = OUT_Barra.right - static_cast<int>(Parte * (nValor - nMinimo));
				break;
			case ArribaAbajo:
				Parte = static_cast<float>(IN_RectaCliente.bottom - IN_RectaCliente.top) / (nMaximo - nMinimo);
				OUT_Barra.bottom = static_cast<int>(Parte * (nValor - nMinimo));
				break;
			case AbajoArriba :
				Parte = static_cast<float>(IN_RectaCliente.bottom - IN_RectaCliente.top) / (nMaximo - nMinimo);
				OUT_Barra.top = OUT_Barra.bottom - static_cast<int>(Parte * (nValor - nMinimo));
				break;
		}
	}

	void DBarraProgresoEx::PintarBarraEx(HDC DC, const int nX, const int nY) {
		static RECT RC;
		GetClientRect(hWnd(), &RC);
		// Creo un buffer en memória para pintar el control
		HDC		Buffer = CreateCompatibleDC(NULL);
		// Creo un DC compatible para el buffer
		HBITMAP Bmp = CreateCompatibleBitmap(DC, RC.right, RC.bottom);
		HBITMAP BmpViejo = static_cast<HBITMAP>(SelectObject(Buffer, Bmp));

		// Calculo el tamaño de la barra para el valor actual
		_RFondo = RC;
//		RBarra.left++; RBarra.top++; RBarra.bottom--; RBarra.right--; // Evito los bordes
//		RFondo.top++; RFondo.bottom--; RFondo.left++; RFondo.right--; // Evito los bordes
		
		CalcularBarra(_Minimo, _Maximo, _Valor, RC, _RBarra);
		// 
		switch (_Alineacion) {
			case IzquierdaDerecha	:	_RFondo.left	= _RBarra.right;		break;
			case DerechaIzquierda	:	_RFondo.right	= _RBarra.left;			break;
			case ArribaAbajo		:	_RFondo.top		= _RBarra.bottom;		break;
			case AbajoArriba		:	_RFondo.bottom	= _RBarra.top;			break;
		}


//		RBarra.right = 1 + static_cast<int>(Parte * (_Valor - _Minimo));
		// Aseguro el valor máximo (a veces con los decimales redondeados no se ve la barra llena)
//		if (_Valor >= _Maximo) RBarra.right = RC.right - 1;
// 		RFondo.left = RBarra.right;


		// Pinto la barra
		Evento_PintarBarra(Buffer, _RBarra);

		// Pinto el fondo
		Evento_PintarFondo(Buffer, _RFondo);

		// Evento de post pintado personalizado 
		Evento_PintarPersonalizado(Buffer, RC);

		// Pinto el valor (si es necesario)
		Evento_PintarValor(Buffer, RC);

		// Pinto el borde
		Evento_PintarBorde(Buffer, RC);

		// Copio el buffer al DC
		BitBlt(DC, nX + RC.left, nY + RC.top, RC.right, RC.bottom, Buffer, 0, 0, SRCCOPY);

		// Elimino el buffer de la memória
		SelectObject(Buffer, BmpViejo);
		DeleteObject(Bmp);
		DeleteDC(Buffer);
	}


	void DBarraProgresoEx::Evento_PintarBarra(HDC DC, RECT &RBarra) {
		HBRUSH BrochaBarra = CreateSolidBrush(_ColorBarra);
		FillRect(DC, &RBarra, BrochaBarra);
		DeleteObject(BrochaBarra);
	}

	void DBarraProgresoEx::Evento_PintarFondo(HDC DC, RECT &RFondo) {
		HBRUSH BrochaFondo = CreateSolidBrush(_ColorFondo);
		FillRect(DC, &RFondo, BrochaFondo);
		DeleteObject(BrochaFondo);
	}

	void DBarraProgresoEx::Evento_PintarBorde(HDC DC, RECT &RBorde) {
		HBRUSH BrochaBorde = CreateSolidBrush(_ColorBorde);
		FrameRect(DC, &RBorde, BrochaBorde);
		DeleteObject(BrochaBorde);
	}

	void DBarraProgresoEx::Evento_PintarValor(HDC DC, RECT &RC) {
		if (_MostrarValor != DBarraEx_MostrarValor_Nada) {
			HFONT VFuente = static_cast<HFONT>(SelectObject(DC, _Fuente()));
			std::wstring TxtValor;
			switch (_MostrarValor) {
				case DBarraEx_MostrarValor_Valor2Decimales :			TxtValor = DWL::Strings::ToStrF(_Valor, 2);													break;
				case DBarraEx_MostrarValor_ValorInt:					TxtValor = DWL::Strings::ToStrF(_Valor, 0);													break;
				case DBarraEx_MostrarValor_ValorMaximo2Decimales:		TxtValor = DWL::Strings::ToStrF(_Valor, 2) + L" / " + DWL::Strings::ToStrF(_Maximo, 2);		break;
				case DBarraEx_MostrarValor_ValorMaximoInt:				TxtValor = DWL::Strings::ToStrF(_Valor, 0) + L" / " + DWL::Strings::ToStrF(_Maximo, 0);		break;
			}
			Evento_FormatearValor(TxtValor);
			SetBkMode(DC, TRANSPARENT);
			if (Skin.FuenteSombraTexto == TRUE) {
				SetTextColor(DC, Skin.TextoSombra);
				RECT RCS = { RC.left + 1, RC.top + 1, RC.right + 1, RC.bottom + 1 };
				DrawText(DC, TxtValor.c_str(), static_cast<int>(TxtValor.size()), &RCS, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			}

			SetTextColor(DC, Skin.Texto);
			DrawText(DC, TxtValor.c_str(), static_cast<int>(TxtValor.size()), &RC, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			SelectObject(DC, VFuente);
		}
	}

	const float DBarraProgresoEx::_ValorMouse(RECT &RC, int cX, int cY) {
		// Las coordenadas no pueden sobresalir de la recta
		if (cX > RC.right)	cX = RC.right;
		if (cX < 0)			cX = 0;
		if (cY > RC.bottom)	cY = RC.bottom;
		if (cY < 0)			cY = 0;

		float Parte = 0.0f, nValor = 0.0f; // _Minimo + (static_cast<float>(cX - RC.left) * Parte);

		switch (_Alineacion) {
			case IzquierdaDerecha:
				Parte = (_Maximo - _Minimo) / static_cast<float>(((RC.right - RC.left) - 2));
				nValor = _Minimo + (static_cast<float>(cX - RC.left) * Parte);
				#if DBARRAEX_DEBUG == TRUE
					Debug_Escribir_Varg(L"DBarraProgresoEx::_ValorMouse X : %d, left : %d, Parte : %f, Valor : %f\n", cX, RC.left, Parte, nValor);
				#endif
				break;
			case DerechaIzquierda :
				Parte = (_Maximo - _Minimo) / static_cast<float>(((RC.right - RC.left) - 2));
				nValor = _Minimo + abs(static_cast<float>(cX - RC.right) * Parte);
				#if DBARRAEX_DEBUG == TRUE
					Debug_Escribir_Varg(L"DBarraProgresoEx::_ValorMouse X : %d, right : %d, Parte : %f, Valor : %f\n", cX, RC.right, Parte, nValor);
				#endif
				break;
			case ArribaAbajo:
				Parte = (_Maximo - _Minimo) / static_cast<float>(((RC.bottom - RC.top) - 2));
				nValor = _Minimo + (static_cast<float>(cY - RC.top) * Parte);
				#if DBARRAEX_DEBUG == TRUE
					Debug_Escribir_Varg(L"DBarraProgresoEx::_ValorMouse Y : %d, top : %d, Parte : %f, Valor : %f\n", cY, RC.top, Parte, nValor);
				#endif
				break;
			case AbajoArriba :
				Parte = (_Maximo - _Minimo) / static_cast<float>(((RC.bottom - RC.top) - 2));
				nValor = _Minimo + abs(static_cast<float>(cY - RC.bottom) * Parte);
				#if DBARRAEX_DEBUG == TRUE
					Debug_Escribir_Varg(L"DBarraProgresoEx::_ValorMouse Y : %d, bottom : %d, Parte : %f, Valor : %f\n", cY, RC.bottom, Parte, nValor);
				#endif
				break;
		}

		if (nValor > _Maximo) { nValor = _Maximo; }
		if (nValor < _Minimo) { nValor = _Minimo; }
		return nValor;
	}

	void DBarraProgresoEx::_Evento_MouseMovimiento(WPARAM wParam, LPARAM lParam) {
		if (_MouseEntrando() == TRUE) {
			// Mouse enter
			if (_Estado != DBarraEx_Estado_Presionado) {
				_Estado = DBarraEx_Estado_Resaltado;
				//Repintar();
//				Resaltar(TRUE);
				Transicion(DBarraEx_Transicion_Resaltado);
			}
		}
	}
	void DBarraProgresoEx::_Evento_MouseSaliendo(void) {
		_MouseDentro = FALSE;
		if (_Estado != DBarraEx_Estado_Presionado) {
			_Estado = DBarraEx_Estado_Normal;
			Transicion(DBarraEx_Transicion_Normal);
		}
	}

	void DBarraProgresoEx::Activado(const BOOL nActivar) {
		BOOL Ret = FALSE;
		_Activado = nActivar;
		Ret = EnableWindow(_hWnd, nActivar);
		Transicion((nActivar == TRUE) ? DBarraEx_Transicion_Normal : DBarraEx_Transicion_Desactivado);
	}

	void DBarraProgresoEx::Alineacion(const DBarraEx_Alineacion nAlineacion) {
		_Alineacion = nAlineacion;
		Repintar();
	}

	void DBarraProgresoEx::Transicion(const DBarraEx_Transicion nTransicion) {
		DWORD Duracion = DhWnd::TiempoAnimaciones;
		if (_AniTransicion.Animando() == TRUE) {
			Duracion = _AniTransicion.TiempoActual();
			_AniTransicion.Terminar();
		}

		COLORREF *FondoHasta = 0, *BordeHasta = 0, *BarraHasta = 0;
		switch (nTransicion) {
			case DBarraEx_Transicion_Normal:
				FondoHasta = &Skin.FondoNormal;
				BarraHasta = &Skin.BarraNormal;
				BordeHasta = &Skin.BordeNormal;
				break;
			case DBarraEx_Transicion_Resaltado:
				FondoHasta = &Skin.FondoResaltado;
				BarraHasta = &Skin.BarraResaltado;
				BordeHasta = &Skin.BordeResaltado;
				break;
			case DBarraEx_Transicion_Presionado:
				FondoHasta = &Skin.FondoPresionado;
				BarraHasta = &Skin.BarraPresionado;
				BordeHasta = &Skin.BordePresionado;
				break;
			case DBarraEx_Transicion_Desactivado:
				FondoHasta = &Skin.FondoDesactivado;
				BarraHasta = &Skin.BarraDesactivado;
				BordeHasta = &Skin.BordeNormal;
				break;
		}

		_AniTransicion.Iniciar({ _ColorFondo, _ColorBorde, _ColorBarra }, { FondoHasta, BordeHasta, BarraHasta }, Duracion, [=](DAnimacion::Valores& Datos, const BOOL Terminado) {
			_ColorFondo = Datos[0].Color();
			_ColorBorde = Datos[1].Color();
			_ColorBarra = Datos[2].Color();
			Repintar();
		});

	}

	LRESULT CALLBACK DBarraProgresoEx::GestorMensajes(UINT uMsg, WPARAM wParam, LPARAM lParam) {
		switch (uMsg) {
			case WM_ERASEBKGND:
				PintarBarraEx(reinterpret_cast<HDC>(wParam));
				return TRUE;
			case WM_PAINT:
				HDC         DC;
				PAINTSTRUCT PS;
				DC = BeginPaint(hWnd(), &PS);
				PintarBarraEx(DC);
				EndPaint(hWnd(), &PS);
				return 0;
			case WM_MOUSEMOVE:
				_Evento_MouseMovimiento(wParam, lParam);
				return 0;
			case WM_MOUSELEAVE:
				_Evento_MouseSaliendo();
				return 0;

		}
		return DControlEx::GestorMensajes(uMsg, wParam, lParam);
	}
}