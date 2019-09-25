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


/*		_________ _______  ______   _______																											 _  _
		\__   __/(  ___  )(  __  \ (  ___  )																					 			        ( )( )
		   ) (   | (   ) || (  \  )| (   ) |   _   - Scroll al sobresalir el texto del control														| || |
		   | |   | |   | || |   ) || |   | |  (_)  																									| || |
		   | |   | |   | || |   | || |   | |          																								| || |
		   | |   | |   | || |   ) || |   | |   _																									(_)(_)
		   | |   | (___) || (__/  )| (___) |  (_)	    																							 _  _
		   )_(   (_______)(______/ (_______)           																								(_)(_)
*/


namespace DWL {
	DEdicionTextoEx_Skin::DEdicionTextoEx_Skin(void) :
		// Colores por defecto ///////////////////////////////////////////////
		FondoNormal				(COLOR_EDICION_FONDO),						// Color del fondo normal
		FondoResaltado			(COLOR_EDICION_FONDO_RESALTADO),			// Color del fondo resaltado
		FondoPresionado			(COLOR_EDICION_FONDO_PRESIONADO),			// Color del fondo presionado
		FondoDesactivado		(COLOR_EDICION_FONDO_DESACTIVADO),			// Color del fondo desactivado
		FondoSeleccion			(COLOR_EDICION_FONDO_SELECCION),			// Color del fondo de la selección
		Texto					(COLOR_EDICION_TEXTO),						// Color del texto
		TextoSombra				(COLOR_EDICION_TEXTO_SOMBRA),				// Color de la sombra del texto
		TextoResaltado			(COLOR_EDICION_TEXTO_RESALTADO),			// Color del texto resaltado
		TextoResaltadoSombra	(COLOR_EDICION_TEXTO_RESALTADO_SOMBRA),		// Color de la sombra del texto resaltado
		TextoPresionado			(COLOR_EDICION_TEXTO_PRESIONADO),			// Color del texto presionado
		TextoPresionadoSombra	(COLOR_EDICION_TEXTO_PRESIONADO_SOMBRA),	// Color de la sombra del texto presionado
		TextoDesactivado		(COLOR_EDICION_TEXTO_DESACTIVADO),			// Color del texto desactivado
		TextoDesactivadoSombra  (COLOR_EDICION_TEXTO_DESACTIVADO_SOMBRA),	// Color de la sombra del texto desactivado
		TextoPlaceholder		(COLOR_EDICION_TEXTO_PLACEHOLDER),			// Color del placeholder
		TextoPlaceholderSombra  (COLOR_EDICION_TEXTO_PLACEHOLDER_SOMBRA),	// Color de la sombra del placeholder
		BordeNormal				(COLOR_EDICION_BORDE),						// Color del borde nomral
		BordeResaltado			(COLOR_EDICION_BORDE_RESALTADO),			// Color del borde resaltado
		BordePresionado			(COLOR_EDICION_BORDE_PRESIONADO),			// Color del borde presionado
		Cursor					(COLOR_EDICION_CURSOR),						// Color del cursor
		// Fuente por defecto ////////////////////////////////////////////////
		FuenteTam				(FUENTE_NORMAL),							// Tamaño de la fuente  (SOLO SE USA PARA CREAR EL CONTROL)
		FuenteNombre			(FUENTE_NOMBRE),							// Nombre de la fuente  (SOLO SE USA PARA CREAR EL CONTROL)
		FuenteNegrita			(FALSE),									// Fuente en negrita	(SOLO SE USA PARA CREAR EL CONTROL)
		FuenteCursiva			(FALSE),									// Fuente en cursiva	(SOLO SE USA PARA CREAR EL CONTROL)
		FuenteSubrayado			(FALSE),									// Fuente subrayada		(SOLO SE USA PARA CREAR EL CONTROL)
		FuenteSombraTexto		(TRUE)										// Pintar sombras a los textos
	{ }

	/*
		  _____  ______    _ _      _          _______        _        ______
		 |  __ \|  ____|  | (_)    (_)        |__   __|      | |      |  ____|
		 | |  | | |__   __| |_  ___ _  ___  _ __ | | _____  _| |_ ___ | |__  __  __
		 | |  | |  __| / _` | |/ __| |/ _ \| '_ \| |/ _ \ \/ / __/ _ \|  __| \ \/ /
		 | |__| | |___| (_| | | (__| | (_) | | | | |  __/>  <| || (_) | |____ >  <
		 |_____/|______\__,_|_|\___|_|\___/|_| |_|_|\___/_/\_\\__\___/|______/_/\_\

	*/

	// Constructor
	DEdicionTextoEx::DEdicionTextoEx(void) : 
		DControlEx(), 
		Entrada(DEdicionTextoEx_Entrada_Texto)									, 
		Alineacion(DEdicionTextoEx_Alineacion_Izquierda)						, 
		_Presionado(FALSE)														, 		
		_PosCursor(0)															, 
		_ColorTexto(Skin.Texto)													, 
		_ColorTextoSombra(Skin.TextoSombra)										, 
		_ColorFondo(Skin.FondoNormal)											, 
		_ColorBorde(Skin.BordeNormal)											, 
		_ColorCursor(Skin.FondoNormal)											,
		_PosSeleccion(0), _PosRedoUndo(0)										, 
		_ColorTextoPH(Skin.TextoPlaceholder)									,
		_ColorTextoSombraPH(Skin.TextoPlaceholderSombra)						,
		// Eventos Lambda enlazados a los eventos virtuales por defecto
		EventoCambio([=]						{ Evento_Cambio();				}),
		EventoMouseClick([=](DEventoMouse& e)	{ Evento_MouseClick(e);			})  {
	}

	// Destructor
	DEdicionTextoEx::~DEdicionTextoEx(void) {
		Destruir();
	}

	// Función que crea el control DEdicionTextoEx sin icono
	HWND DEdicionTextoEx::CrearEdicionTextoEx(DhWnd* nPadre, const TCHAR* nTxt, const int cX, const int cY, const int cAncho, const int cAlto, const int cID, const long Estilos) {
		return CrearEdicionTextoEx(nPadre, nullptr, nTxt, cX, cY, cAncho, cAlto, cID, Estilos);
	}


	// Función que crea el control DEdicionTextoEx utilizando un icono de los recursos
	HWND DEdicionTextoEx::CrearEdicionTextoEx(DhWnd* nPadre, const UINT IDI_Icono, const TCHAR* nTxt, const int cX, const int cY, const int cAncho, const int cAlto, const int cID, const long Estilos) {
		DIcono Ico(IDI_Icono, 16, 16);
		return CrearEdicionTextoEx(nPadre, &Ico, nTxt, cX, cY, cAncho, cAlto, cID, Estilos);
	}

	// Función que crea el control DEdicionTextoEx utilizando un icono del sistema
	HWND DEdicionTextoEx::CrearEdicionTextoEx(DhWnd* nPadre, const GUID nIconoKnownFolder, const TCHAR* nTxt, const int cX, const int cY, const int cAncho, const int cAlto, const int cID, const long Estilos) {
		DIcono Ico(nIconoKnownFolder, 16, 16);
		return CrearEdicionTextoEx(nPadre, &Ico, nTxt, cX, cY, cAncho, cAlto, cID, Estilos);
	}

	// Función que crea el control DEdicionTextoEx utilizando un icono de los recursos
	HWND DEdicionTextoEx::CrearEdicionTextoEx(DhWnd* nPadre, const wchar_t* nPathIcono, const int nPosIcono, const TCHAR* nTxt, const int cX, const int cY, const int cAncho, const int cAlto, const int cID, const long Estilos) {
		DIcono Ico(nPathIcono, 16, 16, nPosIcono);
		return CrearEdicionTextoEx(nPadre, &Ico, nTxt, cX, cY, cAncho, cAlto, cID, Estilos);
	}

	// Función que crea el control DEdicionTextoEx partiendo de una clase DIcono que puede ser nullptr
	HWND DEdicionTextoEx::CrearEdicionTextoEx(DhWnd *nPadre, DIcono *nIcono, const TCHAR *nTxt, const int cX, const int cY, const int cAncho, const int cAlto, const int cID, const long Estilos) {
		_hWnd = CrearControlEx(nPadre, L"DEdicionTextoEx", L"", cID, cX, cY, cAncho, cAlto, Estilos, NULL, CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW);
		Fuente.CrearFuente(Skin.FuenteTam, Skin.FuenteNombre.c_str(), Skin.FuenteNegrita, Skin.FuenteCursiva, Skin.FuenteSubrayado);
		_Texto				= nTxt;
		_PosCursor			= _Texto.size();		
		_PosSeleccion		= _PosCursor;
		_ColorTexto			= Skin.Texto;
		_ColorTextoSombra	= Skin.TextoSombra;
		_ColorFondo			= Skin.FondoNormal;
		_ColorBorde			= Skin.BordeNormal;
		_ColorCursor		= Skin.FondoNormal; // Inicialmente el color del cursor es el del fondo, y tiene que llegar con una animación a COLOR_EDICION_CURSOR;
		_ColorTextoPH		= Skin.TextoPlaceholder;
		_ColorTextoSombraPH = Skin.TextoPlaceholderSombra;
		_AgregarTextoUndo();
		Icono(nIcono, FALSE);
		return hWnd();
	}

	// Función que asigna el icono a mostrar en la izquierda, NULL para no mostrar ningun icono
	void DEdicionTextoEx::Icono(DIcono *nIcono, const BOOL nRepintar) {
		if (nIcono == nullptr) return;
		_Icono = *nIcono;		
		if (nRepintar != FALSE) Repintar();
	}

	// Función que pinta el control en el DC y las coordenadas especificadas
	void DEdicionTextoEx::Pintar(HDC DC, const int cX, const int cY) {
		RECT RC;
		GetClientRect(hWnd(), &RC);

		HDC		Buffer			= CreateCompatibleDC(NULL);								// Creo un buffer en memória para pintar el control
		HBITMAP Bmp				= CreateCompatibleBitmap(DC, RC.right, RC.bottom);		// Creo un DC compatible para el buffer
		HBITMAP vBmp			= static_cast<HBITMAP>(SelectObject(Buffer, Bmp));		// Bitmap compatible para el DC
		HFONT	vFuente			= static_cast<HFONT>(SelectObject(Buffer, Fuente()));	// Selecciono la fuente del control
		RECT    EspacioTexto	= { 0, 0, 0, 0 };										// Espacio para el texto (OJO es el espacio disponible)

		// Pinto el fondo
		HBRUSH BrochaFondo = CreateSolidBrush(_ColorFondo);
		FillRect(Buffer, &RC, BrochaFondo);
		DeleteObject(BrochaFondo);

		// Pinto el borde
		HBRUSH BrochaBorde = CreateSolidBrush(_ColorBorde);
		FrameRect(Buffer, &RC, BrochaBorde);
		DeleteObject(BrochaBorde);

		// Pinto el icono (si hay icono)
		if (_Icono() != nullptr) {
			DrawIconEx(Buffer, 2, (RC.bottom - DEDICIONTEXTOEX_TAMICONO) / 2, _Icono(), DEDICIONTEXTOEX_TAMICONO, DEDICIONTEXTOEX_TAMICONO, 0, 0, DI_NORMAL);
		}

		// Obtengo el rectangulo que ocupará el texto
		RECT PosTexto		= _PosicionTexto(_Texto, RC);

		// Pinto la selección (si existe)
		if (_PosCursor != _PosSeleccion) {
			RECT FondoSeleccion = { 0,0,0,0 };
			// Calculo los tamaños de texto desde el principio hasta _PosCursor y _PosSeleccion
			SIZE TamTexto1 = Fuente.ObtenerTamTexto(Buffer, _Texto.substr(0, _PosCursor).c_str());
			SIZE TamTexto2 = Fuente.ObtenerTamTexto(Buffer, _Texto.substr(0, _PosSeleccion).c_str());
			// Calculo la altura para el texto
			int Alto	   = (TamTexto1.cy != 0) ? TamTexto1.cy : TamTexto2.cy; // Altura del texto
			int Top		   = ((RC.bottom - Alto) / 2) + 2;						// valor Top del RECT
			int Bottom	   = (((RC.bottom - Alto) / 2) + Alto) - 2;				// valor Bottom del RECT
			// Miro que tamaño de texto es mas grande para extraer el tamaño real de la selección
			if (TamTexto1.cx > TamTexto2.cx) FondoSeleccion = { PosTexto.left + TamTexto2.cx, Top, PosTexto.left + TamTexto1.cx, Bottom };
			else                             FondoSeleccion = { PosTexto.left + TamTexto1.cx, Top, PosTexto.left + TamTexto2.cx, Bottom };
			// Pinto el rectangulo de la selección
			HBRUSH BrochaSeleccion = CreateSolidBrush(Skin.FondoSeleccion);
			FillRect(Buffer, &FondoSeleccion, BrochaSeleccion);
			DeleteObject(BrochaSeleccion);
		}

		// Asigno el fondo transparente al pintar el texto
		SetBkMode(Buffer, TRANSPARENT);

		// SI hay texto o tiene el foco, pinto el texto normal
		if (_Texto.size() > 0 || GetFocus() == _hWnd) {
			// Pinto la sombra del texto
			if (Skin.FuenteSombraTexto == TRUE) {
				SetTextColor(Buffer, _ColorTextoSombra);
				TextOut(Buffer, PosTexto.left + 1, PosTexto.top + 1, _Texto.c_str(), static_cast<int>(_Texto.size()));
			}
			// Pinto el texto
			SetTextColor(Buffer, _ColorTexto);
			TextOut(Buffer, PosTexto.left, PosTexto.top, _Texto.c_str(), static_cast<int>(_Texto.size()));
		}
		// Pinto el texto del placeholder (el control no tiene el foco y no hay texto
		else {
			RECT PosPlaceHoder = _PosicionTexto(Placeholder, RC);

			// Pinto la sombra del texto
			if (Skin.FuenteSombraTexto == TRUE) {
				SetTextColor(Buffer, _ColorTextoSombraPH);
				TextOut(Buffer, PosPlaceHoder.left + 1, PosPlaceHoder.top + 1, Placeholder.c_str(), static_cast<int>(Placeholder.size()));
			}

			// Pinto el texto
			SetTextColor(Buffer, _ColorTextoPH);
			TextOut(Buffer, PosPlaceHoder.left, PosPlaceHoder.top, Placeholder.c_str(), static_cast<int>(Placeholder.size()));
		}


		// Pinto el cursor (si el tipo de entrada lo permite y el control tiene el foco)
		if (Entrada != DEdicionTextoEx_Entrada_SinEntrada && GetFocus() == _hWnd) {			
			HPEN			PlumaCursor			= CreatePen(PS_SOLID, 1, _ColorCursor);
			HPEN			VPluma				= static_cast<HPEN>(SelectObject(Buffer, PlumaCursor));
			SIZE			TamTextoHC			= Fuente.ObtenerTamTexto(Buffer, _Texto.substr(0, _PosCursor).c_str());
			
			MoveToEx(Buffer, PosTexto.left + TamTextoHC.cx, DEDICIONTEXTOEX_MARGEN_CURSOR_Y, NULL);
			LineTo(Buffer, PosTexto.left + TamTextoHC.cx, (PosTexto.bottom == PosTexto.top) ? RC.bottom - DEDICIONTEXTOEX_MARGEN_CURSOR_Y : PosTexto.bottom - DEDICIONTEXTOEX_MARGEN_CURSOR_Y);
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


	// Función que devuelve un RECT con el tamaño y posición del texto
	const RECT DEdicionTextoEx::_PosicionTexto(std::wstring &nTexto, RECT &RC) {
		// Calculo el tamaño del icono
		long TamIcono = (_Icono() != NULL) ? DEDICIONTEXTOEX_TAMICONO + DEDICIONTEXTOEX_MARGEN_X : DEDICIONTEXTOEX_MARGEN_X;
		// Espacio disponible para el texto (por si hay un icono)
		RECT EspacioTexto = { RC.left + TamIcono, RC.top, RC.right - TamIcono, RC.bottom };
		// Calculo la posición del texto
		SIZE TamTexto = Fuente.ObtenerTamTexto(nTexto.c_str());
		// Calculo la altura del texto
		int Top    = ((EspacioTexto.bottom - EspacioTexto.top) - TamTexto.cy) / 2;
		int Bottom = Top + TamTexto.cy;
		// Calculo el espacio según la alineación
		switch (Alineacion) {
			case DEdicionTextoEx_Alineacion_Izquierda:	return { EspacioTexto.left,							Top,	EspacioTexto.left + TamTexto.cx,							Bottom };
			case DEdicionTextoEx_Alineacion_Derecha:	return { EspacioTexto.right - TamTexto.cx,			Top,	EspacioTexto.right,											Bottom };
			case DEdicionTextoEx_Alineacion_Centrado:	return { (EspacioTexto.right - TamTexto.cx) / 2,	Top,	((EspacioTexto.right - TamTexto.cx) / 2) + TamTexto.cx,		Bottom };
		}

		return { 0, 0, 0, 0 };
	}


	// Función que calcula la posición del cursor según las coordenadas especificadas
	const size_t DEdicionTextoEx::HitTest(const int cX, const int cY) {
		RECT	RC = { 0, 0, 0, 0 };
		GetClientRect(_hWnd, &RC);
		RECT	PosTexto = _PosicionTexto(_Texto, RC);
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


	//  Función para el mensaje WM_CHAR
	void DEdicionTextoEx::_Evento_Tecla(WPARAM wParam, LPARAM lParam) {
		DEventoTeclado DatosTeclado(wParam, lParam, this);
		switch (DatosTeclado.TeclaVirtual()) {
/*			case VK_RETURN:
				if (Entrada == DEdicionTextoEx_Entrada_SinEntrada) return;
				break;*/
			case VK_BACK: // Borrar
				_Borrar();
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
					if (_PosCursor >= _Texto.size()) {
						_Texto += DatosTeclado.Caracter();
						_PosCursor = _Texto.size();
					}
					else {
						_Texto.insert(_PosCursor, 1, static_cast<wchar_t>(DatosTeclado.TeclaVirtual()));
						_PosCursor++;
					}
					
					_PosSeleccion = _PosCursor;
					// Añado el texto a la lista de textos para el undo / redo
					_AgregarTextoUndo();
				}
		}		
		Repintar();
		
		#if DEDICIONTEXTOEX_MOSTRARDEBUG == TRUE
			Debug_Escribir_Varg(L"DEdicionTextoEx::_Evento_Tecla %d.\n", DatosTeclado.TeclaVirtual());
		#endif
	}

	//  Función para el mensaje WM_KEYDOWN
	void DEdicionTextoEx::_Evento_TeclaPresionada(WPARAM wParam, LPARAM lParam) {
		DEventoTeclado DatosTeclado(wParam, lParam, this);
		switch (DatosTeclado.TeclaVirtual()) {
			case VK_HOME:			// Inicio
				_PosCursor = 0;
				if (DatosTeclado.Shift() == FALSE) _PosSeleccion = _PosCursor;
				break;
			case VK_END:			// Fin
				_PosCursor = _Texto.size();
				if (DatosTeclado.Shift() == FALSE) _PosSeleccion = _PosCursor;
				break;
			case VK_LEFT:			// Cursor izquierda
				if (_PosCursor > 0) _PosCursor--;
				if (DatosTeclado.Shift() == FALSE) _PosSeleccion = _PosCursor;
				break;
			case VK_RIGHT:			// Cursor derecha
				if (_PosCursor < _Texto.size()) _PosCursor++;
				if (DatosTeclado.Shift() == FALSE) _PosSeleccion = _PosCursor;
				break;
			case VK_DELETE:			// Suprimir
				_Suprimir();
				break;
			case L'c': case L'C':	// Tecla C
				if (DatosTeclado.Control() == TRUE)	_ControlC();		
				break;
			case L'x': case L'X':	// Tecla X	
				if (DatosTeclado.Control() == TRUE)	_ControlX();	
				break;
			case L'v': case L'V':	// Tecla V
				if (DatosTeclado.Control() == TRUE)	_ControlV();
				break;
			case L'z': case L'Z':	// Tecla Z
				if (DatosTeclado.Control() == TRUE)	_ControlZ();
				break;
			case L'y': case L'Y':	// Tecla Y
				if (DatosTeclado.Control() == TRUE)	_ControlY();
				break;

		}
		#if DEDICIONTEXTOEX_MOSTRARDEBUG == TRUE
			Debug_Escribir_Varg(L"DEdicionTextoEx::_Evento_TeclaPresionada %d.\n", DatosTeclado.Caracter());
		#endif
		Repintar();
	}

	//  Función para el mensaje WM_KEYUP
	void DEdicionTextoEx::_Evento_TeclaSoltada(WPARAM wParam, LPARAM lParam) {
		DEventoTeclado DatosTeclado(wParam, lParam, this);
		#if DEDICIONTEXTOEX_MOSTRARDEBUG == TRUE
			Debug_Escribir_Varg(L"DEdicionTextoEx::_Evento_TeclaSoltada %d.\n", DatosTeclado.Caracter());
		#endif
	}

	// Teclado Control + C (copy)
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

	// Función que borra el caracter antes del cursor, o la selección
	void DEdicionTextoEx::_Borrar(void) {
		// No hay texto o no se permite la entrada del teclado
		if (Entrada == DEdicionTextoEx_Entrada_SinEntrada || _Texto.size() == 0) return;
		// No hay selección
		if (_PosCursor == _PosSeleccion) {
			if (_PosCursor > 0) {
				_Texto.erase(--_PosCursor, 1);
				_PosSeleccion = _PosCursor;
				_AgregarTextoUndo();
			}
		}
		else {
			_BorrarTextoSeleccionado();
		}
	}

	// Función que borra el caracter por despues del cursor, o la selección
	void DEdicionTextoEx::_Suprimir(void) {
		// No hay texto o no se permite la entrada del teclado
		if (Entrada == DEdicionTextoEx_Entrada_SinEntrada || _Texto.size() == 0) return;
		// No hay selección
		if (_PosCursor == _PosSeleccion) {
			if (_PosCursor < _Texto.size()) {
				_Texto.erase(_PosCursor, 1);
				_AgregarTextoUndo();
			}
		}
		else {
			_BorrarTextoSeleccionado();
		}
	}

	// Función que borra el texto seleccionado
	void DEdicionTextoEx::_BorrarTextoSeleccionado(void) {
		if (_PosCursor > _PosSeleccion) {
			_Texto.erase(_PosSeleccion, _PosCursor - _PosSeleccion);
			_PosCursor = _PosSeleccion;
		}
		else {
			_Texto.erase(_PosCursor, _PosSeleccion - _PosCursor);
			_PosSeleccion = _PosCursor;
		}
	}

	// Teclado Control + X (cut)
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
		std::wstring Tmp = _Texto.substr(0, Desde) + _Texto.substr(Hasta, _Texto.size() - Hasta);
		_Texto = Tmp;
		_PosCursor = Desde;
		_PosSeleccion = _PosCursor;

		_AgregarTextoUndo();
		#if DEDICIONTEXTOEX_MOSTRARDEBUG == TRUE
			Debug_Escribir_Varg(L"DEdicionTextoEx::_ControlX %s.\n", TextoSeleccionado.c_str());
		#endif
	}

	// Teclado Control + V (paste)
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

		std::wstring P, PFiltrado, Tmp;
		DPortapapeles::ObtenerTexto(_hWnd, P);
		// Filtro los caracteres de P para que no den problemas según el tipo de entrada establecido en el control
		// Por ejemplo si el tipo de entrada es decimal no deberiamos aceptar letras
		for (size_t i = 0; i < P.size(); i++) {
			if (_EntradaPermitida(P[i] == TRUE)) PFiltrado += P[i];
		}

		// Construyo el texto final
		Tmp = _Texto.substr(0, Desde) + PFiltrado + _Texto.substr(Hasta, _Texto.size() - Hasta);
		_Texto = Tmp;
		// Establezco la posición del cursor
		_PosSeleccion = Desde;
		_PosCursor += PFiltrado.size() - 1;

		_AgregarTextoUndo();

		#if DEDICIONTEXTOEX_MOSTRARDEBUG == TRUE
			Debug_Escribir_Varg(L"DEdicionTextoEx::_ControlV %s.\n", P.c_str());
		#endif
	}

	// Teclado Control + Z (undo)
	void DEdicionTextoEx::_ControlZ(void) {
		if (_PosRedoUndo > 1) {
			_Texto		  = _Textos[(--_PosRedoUndo) - 1];
			_PosCursor	  = _PosCursores[_PosRedoUndo - 1];
			_PosSeleccion = _PosCursor;
		}
		#if DEDICIONTEXTOEX_MOSTRARDEBUG == TRUE
			Debug_Escribir_Varg(L"DEdicionTextoEx::_ControlZ %d %d.\n", _PosRedoUndo, _PosCursor);
		#endif
	}

	// Teclado Control + Y (redo)
	void DEdicionTextoEx::_ControlY(void) {
		if (_PosRedoUndo < _Textos.size()) {
			_Texto		  = _Textos[(++_PosRedoUndo) - 1];
			_PosCursor	  = _PosCursores[_PosRedoUndo - 1];
			_PosSeleccion = _PosCursor;
		}
		#if DEDICIONTEXTOEX_MOSTRARDEBUG == TRUE
			Debug_Escribir_Varg(L"DEdicionTextoEx::_ControlY %d %d.\n", _PosRedoUndo, _PosCursor);
		#endif
	}

	// Función que añade la ultima modificación al texto para poder hacer Control + Z y Control + Y
	void DEdicionTextoEx::_AgregarTextoUndo(void) {
		if (_PosRedoUndo < _Textos.size()) {
			_Textos.resize(_PosRedoUndo);
			_PosCursores.resize(_PosRedoUndo);

		}
		_Textos.push_back(_Texto);
		_PosCursores.push_back(_PosCursor);
		_PosRedoUndo = static_cast<long>(_Textos.size());
		
		// Notifico que el texto ha cambiado a la ventana padre
		SendMessage(GetParent(hWnd()), DWL_EDICIONTEXTOEX_CAMBIO, static_cast<WPARAM>(ID()), 0);
		EventoCambio();

		#if DEDICIONTEXTOEX_MOSTRARDEBUG == TRUE
			Debug_Escribir_Varg(L"DEdicionTextoEx::_AgregarTextoUndo %d.\n", _PosRedoUndo);
		#endif
	}

	// Función que determina si el carácter especificado es válido para el tipo de entrada del control
	const BOOL DEdicionTextoEx::_EntradaPermitida(const wchar_t Caracter) {
		switch (Entrada) {
			case DEdicionTextoEx_Entrada_SinEntrada :		// Nada
				return FALSE;
			case DEdicionTextoEx_Entrada_Texto :			// Todo
				return TRUE;
			case DEdicionTextoEx_Entrada_ValoresEnteros :	// Solo números
				if (Caracter == L'0' || Caracter == L'1' || Caracter == L'2' || Caracter == L'3' || Caracter == L'4' ||
					Caracter == L'5' || Caracter == L'6' || Caracter == L'7' || Caracter == L'8' || Caracter == L'9') {
					return TRUE;
				}
				return FALSE;
			case DEdicionTextoEx_Entrada_ValoresDecimales:	// Solo números y puntos
				if (Caracter == L'0' || Caracter == L'1' || Caracter == L'2' || Caracter == L'3' || Caracter == L'4' ||
					Caracter == L'5' || Caracter == L'6' || Caracter == L'7' || Caracter == L'8' || Caracter == L'9') {
					return TRUE;
				}
				// Si es un punto, miro que no existan mas puntos en el texto
				if (Caracter == L'.') {
					// No se ha encontrado ningún punto en el texto
					if (_Texto.find(L'.') == std::wstring::npos) return TRUE;
				}
				return FALSE;
			case DEdicionTextoEx_Entrada_ValoresTiempo:		// Solo números y dos puntos
				if (Caracter == L'0' || Caracter == L'1' || Caracter == L'2' || Caracter == L'3' || Caracter == L'4' ||
					Caracter == L'5' || Caracter == L'6' || Caracter == L'7' || Caracter == L'8' || Caracter == L'9' || Caracter == L':') {
					return TRUE;
				}
				return FALSE;
		}
		return FALSE;
	}


	//  Función para el mensaje WM_PAINT
	void DEdicionTextoEx::_Evento_Pintar(void) {
		HDC         DC;
		PAINTSTRUCT PS;
		DC = BeginPaint(hWnd(), &PS);
		Pintar(DC);
		EndPaint(hWnd(), &PS);
	}

	// Función que activa / desactiva el control
	void DEdicionTextoEx::Activado(const BOOL nActivar) {
		BOOL Ret = FALSE;
		Ret = EnableWindow(_hWnd, nActivar);
		Transicion((nActivar == TRUE) ? DEdicionTextoEx_Transicion_Normal : DEdicionTextoEx_Transicion_Desactivado);
	}

	// Función que asigna el texto al control utilizando el valor entero especificado
	void DEdicionTextoEx::Texto(const LONG_PTR ValorEntero, const BOOL nRepintar) {
		_Texto = DWL::Strings::ToStrF(ValorEntero, 0);
		_PosCursor = _Texto.size();
		_PosSeleccion = _PosCursor;
		_AgregarTextoUndo();

		if (nRepintar != FALSE) Repintar();
	}

	// Función que asigna el texto al control utilizando el valor decimal especificado
	void DEdicionTextoEx::Texto(const double ValorDecimal, const int Decimales, const BOOL nRepintar) {
		_Texto = DWL::Strings::ToStrF(ValorDecimal, Decimales);
		_PosCursor = _Texto.size();
		_PosSeleccion = _PosCursor;
		_AgregarTextoUndo();

		if (nRepintar != FALSE) Repintar();
	}

	// Función que asigna el texto al control 
	void DEdicionTextoEx::Texto(std::wstring &nTexto, const BOOL nRepintar) {
		_Texto = nTexto;
		_PosCursor = _Texto.size();
		_PosSeleccion = _PosCursor;
		_AgregarTextoUndo();

		if (nRepintar != FALSE) Repintar();
	}

	// Función que asigna el texto al control 
	void DEdicionTextoEx::Texto(const wchar_t *nTexto, const BOOL nRepintar) {
		_Texto = nTexto;
		_PosCursor = _Texto.size();
		_PosSeleccion = _PosCursor;
		_AgregarTextoUndo();

		if (nRepintar != FALSE) Repintar();
	}

	//  Función para el mensaje WM_MOUSEMOVE
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

	//  Función para los mensajes WM_MOUSEDOWN*
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

	//  Función para los mensajes WM_MOUSEUP*
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
		EventoMouseClick(DatosMouse);
		_Presionado = FALSE;
		_PosCursor = HitTest(DatosMouse.X(), DatosMouse.Y());
		Repintar();
		#if DEDICIONTEXTOEX_MOSTRARDEBUG == TRUE
			Debug_Escribir_Varg(L"DEdicionTextoEx::_Evento_MouseSoltado %d, %d\n", _PosSeleccion, _PosCursor);
		#endif
	}

	// Función para los mensajes WM_DOUBLECLICK*
	void DEdicionTextoEx::_Evento_MouseDobleClick(const WPARAM wParam, const LPARAM lParam, const int Boton) {
		_PosSeleccion = 0;
		_PosCursor = _Texto.size();
		Repintar();
		#if DEDICIONTEXTOEX_MOSTRARDEBUG == TRUE
			Debug_Escribir_Varg(L"DEdicionTextoEx::_Evento_MouseDobleClick %d, %d\n", _PosSeleccion, _PosCursor);
		#endif
	}

	// Función para el mensaje WM_MOUSELEAVE
	void DEdicionTextoEx::_Evento_MouseSaliendo(void) {
		if (_Presionado == FALSE)	Transicion(DEdicionTextoEx_Transicion_Normal);
		_MouseDentro = FALSE;
	}

	// Función para el mensaje WM_SETFOCUS
	void DEdicionTextoEx::_Evento_FocoAsignado(void) {
		if (Entrada != DEdicionTextoEx_Entrada_SinEntrada) {
			_ColorCursor = Skin.Cursor;
			SetTimer(_hWnd, ID_TIMER_CURSOR, 1000, NULL);
			_Evento_Temporizador(ID_TIMER_CURSOR);
		}
	}

	// Función para el mensaje WM_KILLFOCUS
	void DEdicionTextoEx::_Evento_FocoPerdido(void) {
		KillTimer(_hWnd, ID_TIMER_CURSOR);
		// Si se piede el foco, hay que retirar el cursor
		_AniCursor.Terminar();
		_AniCursor.Iniciar({ _ColorCursor }, { &_ColorFondo }, 400, [=](DAnimacion::Valores& Datos, const BOOL Terminado) {
			_ColorCursor = Datos[0].Color();
			Repintar();
		});

		Repintar();
	}

	// Función para el mensaje WM_TIMER
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

	// Función que inicia una animación desde el estado actual al estado especifcado
	void DEdicionTextoEx::Transicion(const DEdicionTextoEx_Transicion nTransicion) {
		DWORD Duracion = DhWnd::TiempoAnimaciones;
		if (_AniTransicion.Animando() == TRUE) {
			Duracion = _AniTransicion.TiempoActual();
			_AniTransicion.Terminar();
		}

		COLORREF *FondoHasta = 0, *BordeHasta = 0, *TextoHasta = 0, *TextoSombraHasta = 0, *TextoPHHasta = 0, *TextoPHSombraHasta = 0;
		switch (nTransicion) {
			case DEdicionTextoEx_Transicion_Normal:
				#if DEDICIONTEXTOEX_MOSTRARDEBUG == TRUE
					Debug_Escribir(L"DEdicionTextoEx::Transicion DEdicionTextoEx_Transicion_Normal.\n");
				#endif
				FondoHasta			= &Skin.FondoNormal;
				BordeHasta			= &Skin.BordeNormal;
				TextoHasta	 		= &Skin.Texto;
				TextoSombraHasta	= &Skin.TextoSombra;
				TextoPHHasta		= &Skin.TextoPlaceholder;
				TextoPHSombraHasta	= &Skin.TextoPlaceholderSombra;
				break;
			case DEdicionTextoEx_Transicion_Resaltado:
				#if DEDICIONTEXTOEX_MOSTRARDEBUG == TRUE
					Debug_Escribir(L"DEdicionTextoEx::Transicion DEdicionTextoEx_Transicion_Resaltado.\n");
				#endif			
				FondoHasta			= &Skin.FondoResaltado;
				BordeHasta			= &Skin.BordeResaltado;
				TextoHasta			= &Skin.TextoResaltado;
				TextoSombraHasta	= &Skin.TextoResaltadoSombra;
				TextoPHHasta		= &Skin.TextoPlaceholder;
				TextoPHSombraHasta	= &Skin.TextoPlaceholderSombra;
				break;
			case DEdicionTextoEx_Transicion_Presionado:
				#if DEDICIONTEXTOEX_MOSTRARDEBUG == TRUE
					Debug_Escribir(L"DEdicionTextoEx::Transicion DEdicionTextoEx_Transicion_Presionado.\n");
				#endif
				FondoHasta			= &Skin.FondoPresionado;
				BordeHasta	 		= &Skin.BordePresionado;
				TextoHasta			= &Skin.TextoPresionado;
				TextoSombraHasta	= &Skin.TextoPresionadoSombra;
				TextoPHHasta		= &Skin.TextoPlaceholder;
				TextoPHSombraHasta	= &Skin.TextoPlaceholderSombra;
				break;
			case DEdicionTextoEx_Transicion_Desactivado:
				#if DEDICIONTEXTOEX_MOSTRARDEBUG == TRUE
					Debug_Escribir(L"DEdicionTextoEx::Transicion DEdicionTextoEx_Transicion_Desactivado.\n");
				#endif			
				FondoHasta			= &Skin.FondoDesactivado;
				BordeHasta			= &Skin.BordeNormal;
				TextoHasta			= &Skin.TextoDesactivado;
				TextoSombraHasta	= &Skin.TextoDesactivadoSombra;
				TextoPHHasta		= &Skin.TextoDesactivado;
				TextoPHSombraHasta	= &Skin.TextoDesactivadoSombra;

				break;
		}

		_AniTransicion.Iniciar({ _ColorFondo, _ColorBorde, _ColorTexto, _ColorTextoSombra, _ColorTextoPH, _ColorTextoSombraPH }, { FondoHasta, BordeHasta, TextoHasta, TextoSombraHasta, TextoPHHasta, TextoPHSombraHasta }, Duracion, [=](DAnimacion::Valores& Datos, const BOOL Terminado) {
			_ColorFondo			= Datos[0].Color();
			_ColorBorde			= Datos[1].Color();
			_ColorTexto			= Datos[2].Color();
			_ColorTextoSombra	= Datos[3].Color();
			_ColorTextoPH		= Datos[4].Color();
			_ColorTextoSombraPH = Datos[5].Color();
			Repintar();
		});
	}

	// WindowProcedure
	LRESULT CALLBACK DEdicionTextoEx::GestorMensajes(UINT uMsg, WPARAM wParam, LPARAM lParam) {
		switch (uMsg) {
			case WM_SETFOCUS	:		_Evento_FocoAsignado();									return 0;
			case WM_KILLFOCUS	:		_Evento_FocoPerdido();									return 0;

			case WM_KEYDOWN		:		_Evento_TeclaPresionada(wParam, lParam);				return 0;
			case WM_KEYUP		:		_Evento_TeclaSoltada(wParam, lParam);					return 0;
			case WM_CHAR		: 		_Evento_Tecla(wParam, lParam);							return 0;

			case WM_PAINT		:		_Evento_Pintar();										return 0;
					
			case WM_MOUSEMOVE	:		_Evento_MouseMovimiento(wParam, lParam);				return 0;
			case WM_MOUSELEAVE	:		_Evento_MouseSaliendo();								return 0;
			case WM_LBUTTONDOWN	:		_Evento_MousePresionado(wParam, lParam, 0);				return 0;
			case WM_RBUTTONDOWN	:		_Evento_MousePresionado(wParam, lParam, 1);				return 0;
			case WM_MBUTTONDOWN	:		_Evento_MousePresionado(wParam, lParam, 2);				return 0;
			case WM_LBUTTONUP	:		_Evento_MouseSoltado(wParam, lParam, 0);				return 0;
			case WM_RBUTTONUP	:		_Evento_MouseSoltado(wParam, lParam, 1);				return 0;
			case WM_MBUTTONUP	:		_Evento_MouseSoltado(wParam, lParam, 2);				return 0;
				// Mouse doble click
			case WM_LBUTTONDBLCLK:		_Evento_MouseDobleClick(wParam, lParam, 0);				return 0;
			case WM_RBUTTONDBLCLK:		_Evento_MouseDobleClick(wParam, lParam, 1);				return 0;
			case WM_MBUTTONDBLCLK:		_Evento_MouseDobleClick(wParam, lParam, 2);				return 0;

			case WM_TIMER		:		_Evento_Temporizador(static_cast<UINT_PTR>(wParam));	return 0;

		}
		return DControlEx::GestorMensajes(uMsg, wParam, lParam);
	}


}