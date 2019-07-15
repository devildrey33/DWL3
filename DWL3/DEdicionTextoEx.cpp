#include "pch.h"
#include "DEdicionTextoEx.h"
#include "DStringUtils.h"
#include "DEventoMouse.h"
#include "DMarcaEx.h"
#include "DEventoTeclado.h"
#include "DPortapapeles.h"

#define DEDICIONTEXTOEX_TAMICONO			16
#define DEDICIONTEXTOEX_MARGEN_X			 4
#define DEDICIONTEXTOEX_MARGEN_CURSOR_Y		 3

#define ID_TIMER_CURSOR				1000

namespace DWL {
/*
	  _____  ______    _ _      _          _______        _        ______      
	 |  __ \|  ____|  | (_)    (_)        |__   __|      | |      |  ____|     
	 | |  | | |__   __| |_  ___ _  ___  _ __ | | _____  _| |_ ___ | |__  __  __
	 | |  | |  __| / _` | |/ __| |/ _ \| '_ \| |/ _ \ \/ / __/ _ \|  __| \ \/ /
	 | |__| | |___| (_| | | (__| | (_) | | | | |  __/>  <| || (_) | |____ >  < 
	 |_____/|______\__,_|_|\___|_|\___/|_| |_|_|\___/_/\_\\__\___/|______/_/\_\
                                                                          																		  
*/
	DEdicionTextoEx_Skin::DEdicionTextoEx_Skin(void) :
		// Colores por defecto
		FondoNormal				(COLOR_EDICION_FONDO),
		FondoResaltado			(COLOR_EDICION_FONDO_RESALTADO),
		FondoPresionado			(COLOR_EDICION_FONDO_PRESIONADO),
		FondoDesactivado		(COLOR_EDICION_FONDO_DESACTIVADO),
		FondoSeleccion			(COLOR_EDICION_FONDO_SELECCION),
		Texto					(COLOR_EDICION_TEXTO),
		TextoSombra				(COLOR_EDICION_TEXTO_SOMBRA),
		TextoResaltado			(COLOR_EDICION_TEXTO_RESALTADO),
		TextoResaltadoSombra	(COLOR_EDICION_TEXTO_RESALTADO_SOMBRA),
		TextoPresionado			(COLOR_EDICION_TEXTO_PRESIONADO),
		TextoPresionadoSombra	(COLOR_EDICION_TEXTO_PRESIONADO_SOMBRA),
		TextoDesactivado		(COLOR_EDICION_TEXTO_DESACTIVADO),
		TextoDesactivadoSombra  (COLOR_EDICION_TEXTO_DESACTIVADO_SOMBRA),
		BordeNormal				(COLOR_EDICION_BORDE),
		BordeResaltado			(COLOR_EDICION_BORDE_RESALTADO),
		BordePresionado			(COLOR_EDICION_BORDE_PRESIONADO),
		Cursor					(COLOR_EDICION_CURSOR),

		// Fuente por defecto
		FuenteTam				(FUENTE_NORMAL),
		FuenteNombre			(FUENTE_NOMBRE),
		FuenteNegrita			(FALSE),
		FuenteCursiva			(FALSE),
		FuenteSubrayado			(FALSE),
		FuenteSombraTexto		(TRUE) {
	}

	DEdicionTextoEx::DEdicionTextoEx(void) : DControlEx(), Entrada(DEdicionTextoEx_Entrada_Texto), Alineacion(DEdicionTextoEx_Alineacion_Izquierda), _Presionado(FALSE), _PosCursor(0), _ColorTexto(Skin.Texto), _ColorTextoSombra(Skin.TextoSombra),  _ColorFondo(Skin.FondoNormal), _ColorBorde(Skin.BordeNormal), _ColorCursor(Skin.FondoNormal), _PosSeleccion(0) {
	}


	DEdicionTextoEx::~DEdicionTextoEx(void) {
	}

	HWND DEdicionTextoEx::CrearEdicionTextoEx(DhWnd *nPadre, const TCHAR *nTxt, const int cX, const int cY, const int cAncho, const int cAlto, const int cID, DIcono *nIcono, const long Estilos) {
//		if (hWnd()) { Debug_Escribir(L"DEdicionTextoEx::CrearEdicionTextoEx() Error : ya se ha creado el control...\n"); return hWnd(); }
		_hWnd = CrearControlEx(nPadre, L"DEdicionTextoEx", L"", cID, cX, cY, cAncho, cAlto, Estilos, NULL, CS_HREDRAW | CS_VREDRAW);
		Fuente.CrearFuente(Skin.FuenteTam, Skin.FuenteNombre.c_str(), Skin.FuenteNegrita, Skin.FuenteCursiva, Skin.FuenteSubrayado);
		_Texto				= nTxt;
		_PosCursor			= _Texto.size();		
		_PosSeleccion		= _PosCursor;
		_ColorTexto			= Skin.Texto;
		_ColorTextoSombra	= Skin.TextoSombra;
		_ColorFondo			= Skin.FondoNormal;
		_ColorBorde			= Skin.BordeNormal;
		_ColorCursor		= Skin.FondoNormal; // Inicialmente el color del cursor es el del fondo, y tiene que llegar con una animación a COLOR_EDICION_CURSOR;
		Icono(nIcono, FALSE);
		return hWnd();
	}

	void DEdicionTextoEx::Icono(DIcono *nIcono, const BOOL nRepintar) {
		if (nIcono == NULL) return;
		_Icono = *nIcono;		
		if (nRepintar != FALSE) Repintar();
	}

	void DEdicionTextoEx::Pintar(HDC DC, const int cX, const int cY) {
		RECT RC;
		GetClientRect(hWnd(), &RC);

		HDC		Buffer			= CreateCompatibleDC(NULL);								// Creo un buffer en memória para pintar el control
		HBITMAP Bmp				= CreateCompatibleBitmap(DC, RC.right, RC.bottom);		// Creo un DC compatible para el buffer
		HBITMAP vBmp			= static_cast<HBITMAP>(SelectObject(Buffer, Bmp));		// Bitmap compatible para el DC
		HFONT	vFuente			= static_cast<HFONT>(SelectObject(Buffer, Fuente()));	// Selecciono la fuente del control
		RECT    EspacioTexto	= { 0, 0, 0, 0 };

		// Pinto el fondo
		HBRUSH BrochaFondo = CreateSolidBrush(_ColorFondo);
		FillRect(Buffer, &RC, BrochaFondo);
		DeleteObject(BrochaFondo);

		// Pinto el borde
		HBRUSH BrochaBorde = CreateSolidBrush(_ColorBorde);
		FrameRect(Buffer, &RC, BrochaBorde);
		DeleteObject(BrochaBorde);

		// Pinto el icono (si hay icono)
		if (_Icono() != NULL) {
			DrawIconEx(Buffer, 2, 2, _Icono(), DEDICIONTEXTOEX_TAMICONO, DEDICIONTEXTOEX_TAMICONO, 0, 0, DI_NORMAL);
		}

		// Obtengo el rectangulo que ocupará el texto
		RECT PosTexto = _PosicionTexto(RC);

		// Pinto la selección (si existe)
		if (_PosCursor != _PosSeleccion) {
			RECT FondoSeleccion = { 0,0,0,0 };
			SIZE TamTexto1 = Fuente.ObtenerTamTexto(Buffer, _Texto.substr(0, _PosCursor).c_str());
			SIZE TamTexto2 = Fuente.ObtenerTamTexto(Buffer, _Texto.substr(0, _PosSeleccion).c_str());
			int Alto	   = (TamTexto1.cy != 0) ? TamTexto1.cy : TamTexto2.cy; // Altura del texto
			int Top		   = ((RC.bottom - Alto) / 2) + 2;						// valor Top del RECT
			int Bottom	   = (((RC.bottom - Alto) / 2) + Alto) - 2;				// valor Bottom del RECT
			if (TamTexto1.cx > TamTexto2.cx) FondoSeleccion = { PosTexto.left + TamTexto2.cx, Top, PosTexto.left + TamTexto1.cx, Bottom };
			else                             FondoSeleccion = { PosTexto.left + TamTexto1.cx, Top, PosTexto.left + TamTexto2.cx, Bottom };

			HBRUSH BrochaSeleccion = CreateSolidBrush(Skin.FondoSeleccion);
			FillRect(Buffer, &FondoSeleccion, BrochaSeleccion);
			DeleteObject(BrochaSeleccion);
		}


		// Asigno el fondo transparente al pintar el texto
		SetBkMode(Buffer, TRANSPARENT);
		// Pinto la sombra del texto
		if (Skin.FuenteSombraTexto == TRUE) {
			SetTextColor(Buffer, _ColorTextoSombra);
			TextOut(Buffer, PosTexto.left + 1, PosTexto.top + 1, _Texto.c_str(), static_cast<int>(_Texto.size()));
		}

		// Pinto el texto
		SetTextColor(Buffer, _ColorTexto);
		TextOut(Buffer, PosTexto.left, PosTexto.top, _Texto.c_str(), static_cast<int>(_Texto.size()));

		// Pinto el cursor (si el tipo de entrada lo permite y el control tiene el foco)
		if (Entrada != DEdicionTextoEx_Entrada_SinEntrada && GetFocus() == _hWnd) {
			HPEN			PlumaCursor			= CreatePen(PS_SOLID, 1, _ColorCursor);
			HPEN			VPluma				= static_cast<HPEN>(SelectObject(Buffer, PlumaCursor));
			SIZE			TamTextoHC			= Fuente.ObtenerTamTexto(Buffer, _Texto.substr(0, _PosCursor).c_str());
			
			MoveToEx(Buffer, PosTexto.left + TamTextoHC.cx, DEDICIONTEXTOEX_MARGEN_CURSOR_Y, NULL);
			LineTo(Buffer, PosTexto.left + TamTextoHC.cx, PosTexto.bottom - DEDICIONTEXTOEX_MARGEN_CURSOR_Y);
			SelectObject(Buffer, VPluma);
		}

		// Copio el buffer al DC
		BitBlt(DC, cX + RC.left, cY + RC.top, RC.right - cX, RC.bottom - cY, Buffer, 0, 0, SRCCOPY);

		// Elimino el buffer de la memória	
		SelectObject(Buffer, vFuente);
		SelectObject(Buffer, vBmp);
		DeleteObject(Bmp);
		DeleteDC(Buffer);
	}


	const RECT DEdicionTextoEx::_PosicionTexto(RECT &RC) {
		// Calculo el tamaño del icono
		long TamIcono = (_Icono() != NULL) ? DEDICIONTEXTOEX_TAMICONO + DEDICIONTEXTOEX_MARGEN_X : DEDICIONTEXTOEX_MARGEN_X;

		// Espacio disponible para el texto (por si hay un icono)
		RECT EspacioTexto = { RC.left + TamIcono, RC.top, RC.right - TamIcono, RC.bottom };

		// Calculo la posición del texto
		SIZE TamTexto = Fuente.ObtenerTamTexto(_Texto.c_str());
		switch (Alineacion) {
			case DEdicionTextoEx_Alineacion_Izquierda:
				return { EspacioTexto.left, EspacioTexto.top, EspacioTexto.left + TamTexto.cx, EspacioTexto.bottom };
			case DEdicionTextoEx_Alineacion_Derecha:
				return { EspacioTexto.right - TamTexto.cx, 0, EspacioTexto.right, EspacioTexto.bottom };
			case DEdicionTextoEx_Alineacion_Centrado:
				return { (EspacioTexto.right - TamTexto.cx) / 2, 0, ((EspacioTexto.right - TamTexto.cx) / 2) + TamTexto.cx, EspacioTexto.bottom };
		}

		return { 0, 0, 0, 0 };
	}


	// Función que calcula la posición del cursor según las coordenadas especificadas
	const size_t DEdicionTextoEx::HitTest(const int cX, const int cY) {
		RECT	RC = { 0, 0, 0, 0 };
		GetClientRect(_hWnd, &RC);
		RECT	PosTexto = _PosicionTexto(RC);
		// La X es mas pequeña que la izquierda del rectangulo del texto
		if (cX < PosTexto.left) {
			#if DEDICIONTEXTOEX_MOSTRARDEBUG == TRUE
				Debug_Escribir_Varg(L"DEdicionTextoEx::HitTest(%d, %d) 0\n", cX, cY);
			#endif
			return 0;
		}
		// La X es mas grande que la derecha del rectangulo del texto
		if (cX > PosTexto.right) {
			#if DEDICIONTEXTOEX_MOSTRARDEBUG == TRUE
				Debug_Escribir_Varg(L"DEdicionTextoEx::HitTest(%d, %d) %d\n", cX, cY, _Texto.size() - 1);
			#endif
			return _Texto.size();
		}
		// La X está dentro del rectangulo del texto, recorro el texto carácter a carácter hasta que sume más que la X
		for (size_t i = 0; i < _Texto.size(); i++) {
			SIZE TamTexto = Fuente.ObtenerTamTexto(_Texto.substr(0, i).c_str());
			if (cX < PosTexto.left + TamTexto.cx) {
				#if DEDICIONTEXTOEX_MOSTRARDEBUG == TRUE
					Debug_Escribir_Varg(L"DEdicionTextoEx::HitTest(%d, %d) %d\n", cX, cY, i);
				#endif
				return i - 1;
			}
		}

		#if DEDICIONTEXTOEX_MOSTRARDEBUG == TRUE
			Debug_Escribir_Varg(L"DEdicionTextoEx::HitTest(%d, %d) %d\n", cX, cY, _Texto.size() - 1);
		#endif
		return _Texto.size();
	}



	void DEdicionTextoEx::_Evento_TeclaPresionada(WPARAM wParam, LPARAM lParam) {
		DEventoTeclado DatosTeclado(wParam, lParam, this);
		switch (DatosTeclado.TeclaVirtual()) {
//			case VK_SHIFT:
//				break;
			case VK_HOME:
				_PosCursor = 0;
				break;
			case VK_END:
				_PosCursor = _Texto.size();
				break;
			case VK_LEFT:
				if (_PosCursor > 0) _PosCursor--;
				if (DatosTeclado.Shift() == FALSE) _PosSeleccion = _PosCursor;
					break;
			case VK_RIGHT:
				if (_PosCursor < _Texto.size()) _PosCursor++;
				if (DatosTeclado.Shift() == FALSE) _PosSeleccion = _PosCursor;
				break;
			case VK_DELETE: // Suprimir
				if (_Texto.size() > 0 && _PosCursor < _Texto.size()) _Texto.erase(_PosCursor, 1);
				break;
			case L'c': case L'C':	
				if (DatosTeclado.Control() == TRUE)	_ControlC();		
				break;
			case L'x': case L'X':	
				if (DatosTeclado.Control() == TRUE)	_ControlX();	
				break;
			case L'v': case L'V':
				if (DatosTeclado.Control() == TRUE)	_ControlV();
				break;
			case L'z': case L'Z':
				if (DatosTeclado.Control() == TRUE)	_ControlZ();
				break;
			case L'y': case L'Y':
				if (DatosTeclado.Control() == TRUE)	_ControlY();
				break;

		}
		#if DEDICIONTEXTOEX_MOSTRARDEBUG == TRUE
			Debug_Escribir_Varg(L"DEdicionTextoEx::_Evento_TeclaPresionada %d.\n", DatosTeclado.Caracter());
		#endif
		Repintar();
	}

	void DEdicionTextoEx::_Evento_TeclaSoltada(WPARAM wParam, LPARAM lParam) {
		DEventoTeclado DatosTeclado(wParam, lParam, this);
/*		switch (DatosTeclado.TeclaVirtual()) {
			case VK_SHIFT:
				break;
		}*/
		#if DEDICIONTEXTOEX_MOSTRARDEBUG == TRUE
			Debug_Escribir_Varg(L"DEdicionTextoEx::_Evento_TeclaSoltada %d.\n", DatosTeclado.Caracter());
		#endif

	}

	void DEdicionTextoEx::_ControlC(void) {
		size_t Desde = 0;
		size_t Hasta = 0;
		if (_PosCursor > _PosSeleccion) {
			Desde = _PosSeleccion;
			Hasta = _PosCursor;
		}
		else {
			Desde = _PosCursor;
			Hasta = _PosSeleccion;
		}
		std::wstring TextoSeleccionado = _Texto.substr(Desde, Hasta - Desde);
		DPortapapeles::AsignarTexto(TextoSeleccionado);
		#if DEDICIONTEXTOEX_MOSTRARDEBUG == TRUE
			Debug_Escribir_Varg(L"DEdicionTextoEx::_ControlC %s.\n", TextoSeleccionado.c_str());
		#endif
	}

	void DEdicionTextoEx::_ControlX(void) {
		size_t Desde = 0;
		size_t Hasta = 0;
		if (_PosCursor > _PosSeleccion) {
			Desde = _PosSeleccion;
			Hasta = _PosCursor;
		}
		else {
			Desde = _PosCursor;
			Hasta = _PosSeleccion;
		}
		std::wstring TextoSeleccionado = _Texto.substr(Desde, Hasta - Desde);
		DPortapapeles::AsignarTexto(TextoSeleccionado);
		#if DEDICIONTEXTOEX_MOSTRARDEBUG == TRUE
			Debug_Escribir_Varg(L"DEdicionTextoEx::_ControlX %s.\n", TextoSeleccionado.c_str());
		#endif
		std::wstring Tmp = _Texto.substr(0, Desde) + _Texto.substr(Hasta, _Texto.size() - Hasta);
		_Texto = Tmp;
		_PosCursor = Desde;
		_PosSeleccion = _PosCursor;
		Repintar();
	}

	void DEdicionTextoEx::_ControlV(void) {
		size_t Desde = 0;
		size_t Hasta = 0;
		if (_PosCursor > _PosSeleccion) {
			Desde = _PosSeleccion;
			Hasta = _PosCursor;
		}
		else {
			Desde = _PosCursor;
			Hasta = _PosSeleccion;
		}

		std::wstring P, Tmp;
		DPortapapeles::ObtenerTexto(_hWnd, P);
		Tmp = _Texto.substr(0, Desde) + P + _Texto.substr(Hasta, _Texto.size() - Hasta);
		_Texto = Tmp;
		_PosSeleccion = Desde;
		_PosCursor += P.size();
		Repintar();

		#if DEDICIONTEXTOEX_MOSTRARDEBUG == TRUE
			Debug_Escribir_Varg(L"DEdicionTextoEx::_ControlV %s.\n", P.c_str());
		#endif
	}

	void DEdicionTextoEx::_ControlZ(void) {
	}

	void DEdicionTextoEx::_ControlY(void) {
	}

	void DEdicionTextoEx::_Evento_Tecla(WPARAM wParam, LPARAM lParam) {
		DEventoTeclado DatosTeclado(wParam, lParam, this);
		switch (DatosTeclado.TeclaVirtual()) {
			case VK_RETURN:
				if (Entrada == DEdicionTextoEx_Entrada_SinEntrada) return;
				break;
			case VK_BACK: // Borrar
				if (Entrada == DEdicionTextoEx_Entrada_SinEntrada) return;
				if (_PosCursor > 0) _Texto.erase(--_PosCursor, 1);				
				break;
			case VK_ESCAPE: // Desselecciono todo
				_PosSeleccion = _PosCursor;
				break;
			default:
				// Si el carácter presionado no es válido para el tipo de entrada actual salgo
				if (_EntradaPermitida(DatosTeclado.TeclaVirtual()) == FALSE) {
					return;
				}
				// No se está presionado el control
				if (DatosTeclado.Control() != TRUE) {
					if (_PosCursor == _Texto.size()) {
						_Texto += DatosTeclado.Caracter();
					}
					else {
						_Texto.insert(_PosCursor, 1, static_cast<wchar_t>(DatosTeclado.TeclaVirtual()));
					}
					_PosCursor++;
					_PosSeleccion = _PosCursor;
				}
/*				// Se está presionado el control
				else {
					wchar_t ccc = DatosTeclado.Caracter();
					switch (DatosTeclado.Caracter()) {
						case L'c': case L'C':		_ControlC();		break;
						case L'x': case L'X':		_ControlX();		break;
						case L'v': case L'V':		_ControlV();		break;
					}
				}
				break;*/
		}		
		Repintar();
		SendMessage(GetParent(hWnd()), DWL_EDICIONTEXTOEX_CAMBIO, static_cast<WPARAM>(ID()), 0);
		#if DEDICIONTEXTOEX_MOSTRARDEBUG == TRUE
			Debug_Escribir_Varg(L"DEdicionTextoEx::_Evento_Tecla %d.\n", DatosTeclado.TeclaVirtual());
		#endif

	}

	const BOOL DEdicionTextoEx::_EntradaPermitida(const wchar_t Caracter) {
		switch (Entrada) {
			case DEdicionTextoEx_Entrada_SinEntrada :
				return FALSE;
			case DEdicionTextoEx_Entrada_Texto :
				return TRUE;
			case DEdicionTextoEx_Entrada_ValoresEnteros :
				if (Caracter == L'0' || Caracter == L'1' || Caracter == L'2' || Caracter == L'3' || Caracter == L'4' ||
					Caracter == L'5' || Caracter == L'6' || Caracter == L'7' || Caracter == L'8' || Caracter == L'9') {
					return TRUE;
				}
			case DEdicionTextoEx_Entrada_ValoresDecimales:
				if (Caracter == L'0' || Caracter == L'1' || Caracter == L'2' || Caracter == L'3' || Caracter == L'4' ||
					Caracter == L'5' || Caracter == L'6' || Caracter == L'7' || Caracter == L'8' || Caracter == L'9' || Caracter == L'.') {
					return TRUE;
				}
			case DEdicionTextoEx_Entrada_ValoresTiempo:
				if (Caracter == L'0' || Caracter == L'1' || Caracter == L'2' || Caracter == L'3' || Caracter == L'4' ||
					Caracter == L'5' || Caracter == L'6' || Caracter == L'7' || Caracter == L'8' || Caracter == L'9' || Caracter == L':') {
					return TRUE;
				}
		}
		return FALSE;
	}

	void DEdicionTextoEx::_Evento_Pintar(void) {
		HDC         DC;
		PAINTSTRUCT PS;
		DC = BeginPaint(hWnd(), &PS);
		Pintar(DC);
		EndPaint(hWnd(), &PS);
	}

	void DEdicionTextoEx::Activado(const BOOL nActivar) {
		BOOL Ret = FALSE;
		Ret = EnableWindow(_hWnd, nActivar);
		Transicion((nActivar == TRUE) ? DEdicionTextoEx_Transicion_Normal : DEdicionTextoEx_Transicion_Desactivado);
	}


/*	void DEdicionTextoEx::_Evento_Size(void) {
		RECT RC;
		GetClientRect(_hWnd, &RC);
		if (_Icono != NULL) RC.left += ESPACIO_ICONO;
		IniciarEdicionTextoEx(RC);
		Repintar();
	}*/

	void DEdicionTextoEx::Texto(const LONG_PTR ValorEntero, const BOOL nRepintar) {
		_Texto = DWL::Strings::ToStrF(ValorEntero, 0);
		_PosCursor = _Texto.size();
		_PosSeleccion = _PosCursor;

		if (nRepintar != FALSE) Repintar();
	}

	void DEdicionTextoEx::Texto(const double ValorDecimal, const int Decimales, const BOOL nRepintar) {
		_Texto = DWL::Strings::ToStrF(ValorDecimal, Decimales);
		_PosCursor = _Texto.size();
		_PosSeleccion = _PosCursor;

		if (nRepintar != FALSE) Repintar();
	}


	void DEdicionTextoEx::Texto(std::wstring &nTexto, const BOOL nRepintar) {
		_Texto = nTexto;
		_PosCursor = _Texto.size();
		_PosSeleccion = _PosCursor;

		if (nRepintar != FALSE) Repintar();
	}

	void DEdicionTextoEx::Texto(const wchar_t *nTexto, const BOOL nRepintar) {
		_Texto = nTexto;
		_PosCursor = _Texto.size();
		_PosSeleccion = _PosCursor;

		if (nRepintar != FALSE) Repintar();
	}

	void DEdicionTextoEx::_Evento_MouseMovimiento(const WPARAM wParam, const LPARAM lParam) {
		if (_MouseEntrando() == TRUE) {
			// Mouse enter
			Transicion(DEdicionTextoEx_Transicion_Resaltado);
		}
		if (_Presionado == TRUE) {
			DEventoMouse DatosMouse(wParam, lParam, this);
			_PosCursor = HitTest(DatosMouse.X(), DatosMouse.Y());
			#if DEDICIONTEXTOEX_MOSTRARDEBUG == TRUE
				Debug_Escribir_Varg(L"DEdicionTextoEx::_Evento_MouseMovimiento %d, %d\n", _PosSeleccion, _PosCursor);
			#endif

			Repintar();
		}
	}

	void DEdicionTextoEx::_Evento_MousePresionado(const WPARAM wParam, const LPARAM lParam, const int Boton) {
		DEventoMouse DatosMouse(wParam, lParam, this, Boton);

		if (Entrada != DEdicionTextoEx_Entrada_SinEntrada) SetFocus(_hWnd);	// REVISADO
		SetCapture(_hWnd);
		Transicion(DEdicionTextoEx_Transicion_Presionado);
		_Presionado = TRUE;

		_PosCursor = HitTest(DatosMouse.X(), DatosMouse.Y());
		_PosSeleccion = _PosCursor;
		Repintar();
		#if DEDICIONTEXTOEX_MOSTRARDEBUG == TRUE
			Debug_Escribir_Varg(L"DEdicionTextoEx::_Evento_MousePresionado %d, %d\n", _PosSeleccion, _PosCursor);
		#endif
	}

	void DEdicionTextoEx::_Evento_MouseSoltado(const WPARAM wParam, const LPARAM lParam, const int Boton) {
		ReleaseCapture();	
		DEventoMouse DatosMouse(wParam, lParam, this, Boton);
		RECT RC;
		GetClientRect(hWnd(), &RC);
		POINT Pt = { DatosMouse.X(), DatosMouse.Y() };
		if (PtInRect(&RC, Pt) != 0) {
			Transicion(DEdicionTextoEx_Transicion_Resaltado);
		}
		SendMessage(GetParent(hWnd()), DWL_EDICIONTEXTOEX_CLICK, DEVENTOMOUSE_TO_WPARAM(DatosMouse), 0);
		_Presionado = FALSE;
		_PosCursor = HitTest(DatosMouse.X(), DatosMouse.Y());
		Repintar();
		#if DEDICIONTEXTOEX_MOSTRARDEBUG == TRUE
			Debug_Escribir_Varg(L"DEdicionTextoEx::_Evento_MouseSoltado %d, %d\n", _PosSeleccion, _PosCursor);
		#endif
	}

	void DEdicionTextoEx::_Evento_MouseSaliendo(void) {
		if (_Presionado == FALSE)	Transicion(DEdicionTextoEx_Transicion_Normal);
		_MouseDentro = FALSE;
	}

	void DEdicionTextoEx::_Evento_FocoAsignado(void) {
		if (Entrada != DEdicionTextoEx_Entrada_SinEntrada) {
			SetTimer(_hWnd, ID_TIMER_CURSOR, 1000, NULL);
			_Evento_Temporizador(ID_TIMER_CURSOR);
		}
	}

	void DEdicionTextoEx::_Evento_FocoPerdido(void) {
		KillTimer(_hWnd, ID_TIMER_CURSOR);
		// Si se piede el foco, hay que retirar el cursor
		_AniCursor.Terminar();
		_AniCursor.Iniciar({ _ColorCursor }, { &_ColorFondo }, 400, [=](DAnimacion::Valores& Datos, const BOOL Terminado) {
			_ColorCursor = Datos[0].Color();
			Repintar();
		});
	}

	void DEdicionTextoEx::_Evento_Temporizador(const INT_PTR tID) {
		// Si es el temporizador del cursor
		if (tID == ID_TIMER_CURSOR) {			
			static BOOL  CursorVisible = FALSE;
			COLORREF	*Hasta = 0;
			// Asigno el color final del cursor
			if (CursorVisible == FALSE) Hasta = &Skin.Cursor;
			else						Hasta = &_ColorFondo;
			_AniCursor.Terminar();
			_AniCursor.Iniciar({ _ColorCursor }, { Hasta }, 400, [=](DAnimacion::Valores& Datos, const BOOL Terminado) {
				_ColorCursor = Datos[0].Color();
				Repintar();
			});
			CursorVisible = !CursorVisible;
		}
	}


	void DEdicionTextoEx::Transicion(const DEdicionTextoEx_Transicion nTransicion) {
		DWORD Duracion = DhWnd::TiempoAnimaciones;
		if (_AniTransicion.Animando() == TRUE) {
			Duracion = _AniTransicion.TiempoActual();
			_AniTransicion.Terminar();
		}

		COLORREF *FondoHasta = 0, *BordeHasta = 0, *TextoHasta = 0, *TextoSombraHasta = 0;
		switch (nTransicion) {
			case DEdicionTextoEx_Transicion_Normal:
				#if DEDICIONTEXTOEX_MOSTRARDEBUG == TRUE
					Debug_Escribir(L"DEdicionTextoEx::Transicion DEdicionTextoEx_Transicion_Normal.\n");
				#endif
				FondoHasta		 = &Skin.FondoNormal;
				BordeHasta		 = &Skin.BordeNormal;
				TextoHasta	 	 = &Skin.Texto;
				TextoSombraHasta = &Skin.TextoSombra;
				break;
			case DEdicionTextoEx_Transicion_Resaltado:
				#if DEDICIONTEXTOEX_MOSTRARDEBUG == TRUE
					Debug_Escribir(L"DEdicionTextoEx::Transicion DEdicionTextoEx_Transicion_Resaltado.\n");
				#endif			
				FondoHasta		 = &Skin.FondoResaltado;
				BordeHasta		 = &Skin.BordeResaltado;
				TextoHasta		 = &Skin.TextoResaltado;
				TextoSombraHasta = &Skin.TextoResaltadoSombra;
				break;
			case DEdicionTextoEx_Transicion_Presionado:
				#if DEDICIONTEXTOEX_MOSTRARDEBUG == TRUE
					Debug_Escribir(L"DEdicionTextoEx::Transicion DEdicionTextoEx_Transicion_Presionado.\n");
				#endif
				FondoHasta		 = &Skin.FondoPresionado;
				BordeHasta	 	 = &Skin.BordePresionado;
				TextoHasta		 = &Skin.TextoPresionado;
				TextoSombraHasta = &Skin.TextoPresionadoSombra;
				break;
			case DEdicionTextoEx_Transicion_Desactivado:
				#if DEDICIONTEXTOEX_MOSTRARDEBUG == TRUE
					Debug_Escribir(L"DEdicionTextoEx::Transicion DEdicionTextoEx_Transicion_Desactivado.\n");
				#endif			
				FondoHasta		 = &Skin.FondoDesactivado;
				BordeHasta		 = &Skin.BordeNormal;
				TextoHasta		 = &Skin.TextoDesactivado;
				TextoSombraHasta = &Skin.TextoDesactivadoSombra;
				break;
		}

		_AniTransicion.Iniciar({ _ColorFondo, _ColorBorde, _ColorTexto, _ColorTextoSombra }, { FondoHasta, BordeHasta, TextoHasta, TextoSombraHasta }, Duracion, [=](DAnimacion::Valores& Datos, const BOOL Terminado) {
			_ColorFondo			= Datos[0].Color();
			_ColorBorde			= Datos[1].Color();
			_ColorTexto			= Datos[2].Color();
			_ColorTextoSombra	= Datos[3].Color();
			Repintar();
		});
	}


	LRESULT CALLBACK DEdicionTextoEx::GestorMensajes(UINT uMsg, WPARAM wParam, LPARAM lParam) {
		switch (uMsg) {
			case WM_SETFOCUS	:		_Evento_FocoAsignado();									return 0;
			case WM_KILLFOCUS	:		_Evento_FocoPerdido();									return 0;

			case WM_KEYDOWN		:		_Evento_TeclaPresionada(wParam, lParam);				return 0;
			case WM_KEYUP		:		_Evento_TeclaSoltada(wParam, lParam);					return 0;
			case WM_CHAR		: 		_Evento_Tecla(wParam, lParam);							return 0;

//			case WM_SIZE		:		Repintar();																																	return 0;

			case WM_PAINT		:		_Evento_Pintar();										return 0;
					
			case WM_MOUSEMOVE	:		_Evento_MouseMovimiento(wParam, lParam);				return 0;
			case WM_MOUSELEAVE	:		_Evento_MouseSaliendo();								return 0;
			case WM_LBUTTONDOWN	:		_Evento_MousePresionado(wParam, lParam, 0);				return 0;
			case WM_RBUTTONDOWN	:		_Evento_MousePresionado(wParam, lParam, 1);				return 0;
			case WM_MBUTTONDOWN	:		_Evento_MousePresionado(wParam, lParam, 2);				return 0;
			case WM_LBUTTONUP	:		_Evento_MouseSoltado(wParam, lParam, 0);				return 0;
			case WM_RBUTTONUP	:		_Evento_MouseSoltado(wParam, lParam, 1);				return 0;
			case WM_MBUTTONUP	:		_Evento_MouseSoltado(wParam, lParam, 2);				return 0;

			case WM_TIMER		:		_Evento_Temporizador(static_cast<UINT_PTR>(wParam));	return 0;

		}
		return DControlEx::GestorMensajes(uMsg, wParam, lParam);
	}


}