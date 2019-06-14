#pragma once

#define RGBA(R,G,B,A)          (COLORREF) (R | (G<<8) | (B <<16) | (A <<24))

#define AZUL_CLARO									RGB(214, 219, 233)
#define AZUL_CLARO_RESALTADO						RGB(234, 239, 253)
#define AZUL_CLARO_PRESIONADO						RGB(194, 199, 213)

#define AZUL_OSCURO									RGB( 41,  57,  85)
#define AZUL_OSCURO_RESALTADO						RGB(  0, 152, 234)//RGB( 71,  87, 115)
#define AZUL_OSCURO_PRESIONADO						RGB( 11,  27,  55)
#define AZUL_OSCURO_MARCADO							RGB(  0, 122, 204)
//#define AZUL_OSCURO_MARCADO_RESALTADO				RGB(  0, 152, 234)


/* Fuentes permitidas :
	- Georgia					// Numeros raros i es molt roman NO
	- Palatino					// no em convenç
	- Linotype					// NO
	- Times new roman			// maça romana
	- Cambria					// nostamal, un pel roman..																						[*]
	- Constantia				// ni de conya
	- Arial						// no
	- Arial balck				// NO
	- Calibri					// M'agrada i els numeros tb, tot i que l'ample es queda algo reduît i la altura (comparan amb la Tahoma)		[*]
	- Candara					// M'agrada pero els numeros no...
	- Corbel					// M'agrada pero els numeros no...
	- Lucida sans				// no em convenç
	- Microsoft sans serif		// No esta mal, falta un pel de marge de l'ample a vegades..													[*]
	- Tahoma					// La de sempre..																								[*]
	- Trebutchet MS				// Maça gothica
	- Verdana					// Nostamal te un marge vertical molt apurat, però es surt un pel del marge en les etiquetes de les opcions		[*]
	- Franklin gothic medium	// NO
	- Gill Sans MT				// NO
	- Comic sansn MS			// NO
	- Courier new				// NO
	- Lucida console			// NO
	- Segoe Script				// NO, però es pot utilitzar per algo especial
	- Segoe Print				// Same as before
	- Gabriola					// Es xula peró te maça marge vertical
*/
// Fuente por defecto
/*#define FUENTE_NOMBRE			L"Microsoft sans serif"
#define FUENTE_PEQUE			16
#define FUENTE_NORMAL			18
#define FUENTE_GRANDE			20*/

/*#define FUENTE_NOMBRE			L"Lucida sans"
#define FUENTE_PEQUE			17
#define FUENTE_NORMAL			19
#define FUENTE_GRANDE			21*/

#define FUENTE_NOMBRE			L"Calibri"
#define FUENTE_PEQUE			18
#define FUENTE_NORMAL			20
#define FUENTE_GRANDE			23
/*
#define FUENTE_NOMBRE			L"Verdana"
#define FUENTE_PEQUE			14
#define FUENTE_NORMAL			16
#define FUENTE_GRANDE			18*/
/*
#define FUENTE_NOMBRE			L"Cambria"
#define FUENTE_PEQUE			16
#define FUENTE_NORMAL			18
#define FUENTE_GRANDE			20*/

/*#define FUENTE_NOMBRE			L"Tahoma"
#define FUENTE_PEQUE			16
#define FUENTE_NORMAL			18
#define FUENTE_GRANDE			20*/
/*
#define FUENTE_NOMBRE			L"Gabriola"
#define FUENTE_PEQUE			32
#define FUENTE_NORMAL			36
#define FUENTE_GRANDE			40*/


#define COLOR_ROJO									RGB(234,  80,  78)
#define COLOR_ROJO_RESALTADO						RGB(254, 100,  98)
#define COLOR_ROJO_PRESIONADO						RGB(214,  60,  58)
#define COLOR_ROJO_MARCADO							RGB(155,   0,   0)

// Colores básicos para las ventanas y los controles

#define COLOR_FONDO									RGB( 35,  35,  35)
#define COLOR_FONDO_RESALTADO						RGB( 86,  84,  89)
#define COLOR_FONDO_PRESIONADO						RGB( 11,   9,  14)
#define COLOR_FONDO_CLARO							RGB(240, 240, 240)
#define COLOR_FONDO_CLARO_RESALTADO					RGB(255, 255, 255)
#define COLOR_FONDO_CLARO_PRESIONADO				RGB(200, 200, 200)
#define COLOR_FONDO_CLARO_DESACTIVADO				RGB(210, 210, 210)

#define COLOR_BORDE									RGB( 53,  51,  56)	//RGB(100, 100, 100)
#define COLOR_BORDE_RESALTADO						RGB(255,   0,   0)
#define COLOR_BORDE_PRESIONADO						RGB(150,   0,   0)
#define COLOR_TEXTO									RGB(235, 235, 235) //RGB(255, 255, 255)
#define COLOR_TEXTO_SOMBRA							RGB(  0,   0,   0) 
#define COLOR_TEXTO_RESALTADO						RGB(255, 255, 255)
#define COLOR_TEXTO_PRESIONADO						RGB(100, 100, 100) //RGB(255, 255, 255)
#define COLOR_TEXTO_DESACTIVADO						RGB(100, 100, 100) //RGB(255, 255, 255)

#define COLOR_SEPARADOR								RGB(128, 128, 128)
#define COLOR_BORDE_POPUP                           RGB(210, 210, 210)

 
/*
------------------------------------------------------------------------------------------
  Creador de texte ascii : http://patorjk.com/software/taag/#p=display&f=Big&t=ToolTipEx
------------------------------------------------------------------------------------------

____                       ______
|  _ \                     |  ____|
| |_) | __ _ _ __ _ __ __ _| |__  __  __
|  _ < / _` | '__| '__/ _` |  __| \ \/ /
| |_) | (_| | |  | | | (_| | |____ >  <
|____/ \__,_|_|  |_|  \__,_|______/_/\_\

*/
#define COLOR_BARRA									COLOR_ROJO
#define COLOR_BARRA_RESALTADO						RGB(244,  90,  88) //RGB(  0,   0, 230)
#define COLOR_BARRA_PRESIONADO						RGB(224,  70,  68) //RGB(  0,   0, 160)
#define COLOR_BARRA_DESACTIVADO						RGB(100, 80, 80)
#define COLOR_BARRA_FONDO							COLOR_FONDO_CLARO  //RGB(190, 190, 190)
#define COLOR_BARRA_FONDO_RESALTADO					COLOR_FONDO_CLARO_RESALTADO //RGB(225, 225, 225)
#define COLOR_BARRA_FONDO_PRESIONADO				COLOR_FONDO_CLARO_PRESIONADO //RGB(160, 160, 160)
#define COLOR_BARRA_FONDO_DESACTIVADO				COLOR_FONDO_CLARO_DESACTIVADO
#define COLOR_BARRA_TEXTO							RGB( 30,  30,  30)
#define COLOR_BARRA_TEXTO_SOMBRA					RGB(225, 225, 225)

/*
  ____        _              ______
 |  _ \      | |            |  ____|
 | |_) | ___ | |_ ___  _ __ | |__  __  __
 |  _ < / _ \| __/ _ \| '_ \|  __| \ \/ /
 | |_) | (_) | || (_) | | | | |____ >  <
 |____/ \___/ \__\___/|_| |_|______/_/\_\

*/
#define COLOR_BOTON									COLOR_FONDO //RGB( 17,  15,  20) //RGB(220, 220, 220)
#define COLOR_BOTON_RESALTADO						COLOR_ROJO
#define COLOR_BOTON_PRESIONADO						RGB(204,  50,  48)
#define COLOR_BOTON_MARCADO							COLOR_ROJO_MARCADO
#define COLOR_BOTON_TEXTO							RGB(235, 235, 235)           //RGB(235, 235, 235) //RGB(  0,   0,   0)
#define COLOR_BOTON_TEXTO_RESALTADO					RGB(255, 255, 255)		//RGB(100, 100, 100)
#define COLOR_BOTON_TEXTO_PRESIONADO				RGB(155, 155, 155)	//RGB( 50,  50,  50)
#define COLOR_BOTON_TEXTO_SOMBRA					RGB(  0,   0,   0) 
#define COLOR_BOTON_TEXTO_DESACTIVADO               RGB(100, 100, 100)
#define COLOR_BOTON_BORDE							COLOR_FONDO //COLOR_BORDE
#define COLOR_BOTON_BORDE_RESALTADO					COLOR_BORDE_RESALTADO
#define COLOR_BOTON_BORDE_PRESIONADO				COLOR_BORDE_PRESIONADO


/*
  _______          _ _______ _       ______
 |__   __|        | |__   __(_)     |  ____|
	| | ___   ___ | |  | |   _ _ __ | |__  __  __
	| |/ _ \ / _ \| |  | |  | | '_ \|  __| \ \/ /
	| | (_) | (_) | |  | |  | | |_) | |____ >  <
	|_|\___/ \___/|_|  |_|  |_| .__/|______/_/\_\
							  | |
							  |_|
*/
#define COLOR_TOOLTIP_BORDE							COLOR_BORDE_POPUP //RGB(155, 155, 155)
#define COLOR_TOOLTIP_FONDO							COLOR_BOTON //COLOR_FONDO
#define COLOR_TOOLTIP_FONDO_ERROR					COLOR_ROJO_MARCADO
#define COLOR_TOOLTIP_TEXTO							COLOR_BOTON_TEXTO
#define COLOR_TOOLTIP_TEXTO_SOMBRA					COLOR_BOTON_TEXTO_SOMBRA

/*
  ____                        _____                _ _ ______      
 |  _ \                      / ____|              | | |  ____|     
 | |_) | __ _ _ __ _ __ __ _| (___   ___ _ __ ___ | | | |__  __  __
 |  _ < / _` | '__| '__/ _` |\___ \ / __| '__/ _ \| | |  __| \ \/ /
 | |_) | (_| | |  | | | (_| |____) | (__| | | (_) | | | |____ >  < 
 |____/ \__,_|_|  |_|  \__,_|_____/ \___|_|  \___/|_|_|______/_/\_\

*/
#define COLOR_SCROLL_FONDO							COLOR_FONDO_CLARO // RGB(225, 225, 225)
#define COLOR_SCROLL_FONDO_RESALTADO				RGB(220, 220, 220)
#define COLOR_SCROLL_FONDO_PRESIONADO				RGB(190, 190, 190)
#define COLOR_SCROLL_BARRA							RGB(180, 180, 180)
#define COLOR_SCROLL_BARRA_RESALTADO				RGB(100, 100, 100)
#define COLOR_SCROLL_BARRA_PRESIONADO				RGB( 20,  20,  20)

/*              _           _ ______      
     /\        | |         | |  ____|     
    /  \   _ __| |__   ___ | | |__  __  __
   / /\ \ | '__| '_ \ / _ \| |  __| \ \/ /
  / ____ \| |  | |_) | (_) | | |____ >  < 
 /_/    \_\_|  |_.__/ \___/|_|______/_/\_\    

*/
#define COLOR_ARBOL_FONDO							COLOR_FONDO_CLARO
#define COLOR_ARBOL_FONDO_RESALTADO					RGB(255, 220, 220)
#define COLOR_ARBOL_FONDO_PRESIONADO				COLOR_FONDO_CLARO_PRESIONADO
#define COLOR_ARBOL_TEXTO							RGB( 30,  30,  30)
#define COLOR_ARBOL_TEXTO_DESACTIVADO				RGB(128, 128, 128)
#define COLOR_ARBOL_TEXTO_SOMBRA					RGB(255, 255, 255)
#define COLOR_ARBOL_TEXTO_RESALTADO					RGB( 50,  50,  50)

#define COLOR_ARBOL_SELECCION						COLOR_ROJO
#define COLOR_ARBOL_SELECCION_RESALTADO				COLOR_ROJO_RESALTADO
#define COLOR_ARBOL_SELECCION_PRESIONADO			COLOR_ROJO_PRESIONADO
#define COLOR_ARBOL_SUBSELECCION					RGB(220,  170, 168)
#define COLOR_ARBOL_SUBSELECCION_RESALTADO			RGB(245,  190, 188)

#define COLOR_ARBOL_SELECCION_TEXTO					RGB(230, 230, 230)
#define COLOR_ARBOL_SELECCION_TEXTO_SOMBRA			RGB( 64,  64,  64)
#define COLOR_ARBOL_SELECCION_TEXTO_RESALTADO		RGB(255, 255, 255)
#define COLOR_ARBOL_SELECCION_TEXTO_PRESIONADO		RGB(190, 190, 190)
#define COLOR_ARBOL_SUBSELECCION_TEXTO				RGB(230, 230, 230)
#define COLOR_ARBOL_SUBSELECCION_TEXTO_SOMBRA		RGB(  0,   0,   0)
#define COLOR_ARBOL_SUBSELECCION_TEXTO_RESALTADO	RGB(255, 255, 255)

#define COLOR_ARBOL_EXPANSOR_NORMAL					RGB(200, 200, 200)
#define COLOR_ARBOL_EXPANSOR_RESALTADO				COLOR_ROJO_RESALTADO
#define COLOR_ARBOL_EXPANSOR_PRESIONADO				RGB( 32,  32,  32)

#define COLOR_ARBOL_NODO_MARCA						COLOR_BORDE_RESALTADO //RGB(255,   0,   0)

/*
  _      _     _        ______      
 | |    (_)   | |      |  ____|     
 | |     _ ___| |_ __ _| |__  __  __
 | |    | / __| __/ _` |  __| \ \/ /
 | |____| \__ \ || (_| | |____ >  < 
 |______|_|___/\__\__,_|______/_/\_\                                                                

*/
#define COLOR_LISTA_FONDO							COLOR_FONDO_CLARO
#define COLOR_LISTA_FONDO_RESALTADO					RGB(255, 220, 220)
#define COLOR_LISTA_FONDO_PRESIONADO                COLOR_FONDO_CLARO_PRESIONADO		// Solo cuando MostrarSeleccion está desactivado

#define COLOR_LISTA_TEXTO							RGB( 30,  30,  30)
#define COLOR_LISTA_TEXTO_SOMBRA					RGB(255, 255, 255)
#define COLOR_LISTA_TEXTO_RESALTADO					RGB( 50,  50,  50)

#define COLOR_LISTA_SELECCION						COLOR_ROJO
#define COLOR_LISTA_SELECCION_RESALTADO				COLOR_ROJO_RESALTADO
#define COLOR_LISTA_SELECCION_PRESIONADO			COLOR_ROJO_PRESIONADO

#define COLOR_LISTA_SELECCION_TEXTO					RGB(230, 230, 230)
#define COLOR_LISTA_SELECCION_TEXTO_SOMBRA			RGB( 64,  64,  64)
#define COLOR_LISTA_SELECCION_TEXTO_RESALTADO		RGB(255, 255, 255)
#define COLOR_LISTA_SELECCION_TEXTO_PRESIONADO		RGB(190, 190, 190)

#define COLOR_LISTA_MARCA_ITEM						COLOR_BORDE_RESALTADO//RGB(255,   0,   0)

/* 
______    _ _      _          _______        _        ______
|  ____|  | (_)    (_)        |__   __|      | |      |  ____|
| |__   __| |_  ___ _  ___  _ __ | | _____  _| |_ ___ | |__  __  __
|  __| / _` | |/ __| |/ _ \| '_ \| |/ _ \ \/ / __/ _ \|  __| \ \/ /
| |___| (_| | | (__| | (_) | | | | |  __/>  <| || (_) | |____ >  <
|______\__,_|_|\___|_|\___/|_| |_|_|\___/_/\_\\__\___/|______/_/\_\

*/
#define COLOR_EDICION_CURSOR						COLOR_ROJO
#define COLOR_EDICION_FONDO							COLOR_FONDO_CLARO
#define COLOR_EDICION_FONDO_RESALTADO				COLOR_FONDO_CLARO_RESALTADO
#define COLOR_EDICION_FONDO_PRESIONADO				COLOR_FONDO_CLARO_PRESIONADO
#define COLOR_EDICION_FONDO_DESACTIVADO				COLOR_FONDO_CLARO_DESACTIVADO
#define COLOR_EDICION_BORDE							COLOR_BORDE
#define COLOR_EDICION_BORDE_RESALTADO				COLOR_BORDE_RESALTADO
#define COLOR_EDICION_BORDE_PRESIONADO				COLOR_BORDE_PRESIONADO
#define COLOR_EDICION_TEXTO							RGB(  0,   0,   0)
#define COLOR_EDICION_TEXTO_SOMBRA					RGB(255, 255, 255)
#define COLOR_EDICION_TEXTO_RESALTADO				RGB( 30,  30,  30)
#define COLOR_EDICION_TEXTO_RESALTADO_SOMBRA		COLOR_EDICION_TEXTO_SOMBRA
#define COLOR_EDICION_TEXTO_PRESIONADO				RGB( 60,  60,  60)
#define COLOR_EDICION_TEXTO_PRESIONADO_SOMBRA		COLOR_EDICION_TEXTO_SOMBRA
#define COLOR_EDICION_TEXTO_DESACTIVADO				RGB( 90,  90,  90)
#define COLOR_EDICION_TEXTO_DESACTIVADO_SOMBRA		COLOR_FONDO_CLARO_DESACTIVADO
#define COLOR_EDICION_SELECCION						COLOR_ROJO
#define COLOR_EDICION_SELECCION_TEXTO				RGB(230, 230, 230)
#define COLOR_EDICION_SELECCION_TEXTO_SOMBRA		RGB( 64,  64,  64)



/*
  __  __                  ______      
 |  \/  |                |  ____|     
 | \  / | ___ _ __  _   _| |__  __  __
 | |\/| |/ _ \ '_ \| | | |  __| \ \/ /
 | |  | |  __/ | | | |_| | |____ >  < 
 |_|  |_|\___|_| |_|\__,_|______/_/\_\
                                      
*/                                      
#define COLOR_MENU_BORDE                            COLOR_BORDE_POPUP
#define COLOR_MENU_SEPARADOR                        COLOR_BORDE_POPUP
#define COLOR_MENU_FONDO							COLOR_BOTON
#define COLOR_MENU_FONDO_RESALTADO					COLOR_FONDO_RESALTADO			// No utilizar COLOR_BOTON_RESALTADO, ya que las barras del menú utilizan ese color
#define COLOR_MENU_FONDO_PRESIONADO					COLOR_BOTON_PRESIONADO
#define COLOR_MENU_TEXTO							COLOR_BOTON_TEXTO
#define COLOR_MENU_TEXTO_SOMBRA						COLOR_BOTON_TEXTO_SOMBRA
#define COLOR_MENU_TEXTO_RESALTADO					COLOR_BOTON_TEXTO_RESALTADO
#define COLOR_MENU_TEXTO_PRESIONADO					COLOR_BOTON_TEXTO_PRESIONADO
#define COLOR_MENU_TEXTO_DESACTIVADO				COLOR_BOTON_TEXTO_DESACTIVADO

/* 
  __  __                     ______
 |  \/  |                   |  ____|
 | \  / | __ _ _ __ ___ __ _| |__  __  __
 | |\/| |/ _` | '__/ __/ _` |  __| \ \/ /
 | |  | | (_| | | | (_| (_| | |____ >  <
 |_|  |_|\__,_|_|  \___\__,_|______/_/\_\

*/
#define COLOR_MARCA_BORDE							COLOR_BORDE
#define COLOR_MARCA_BORDE_RESALTADO					COLOR_BORDE_RESALTADO
#define COLOR_MARCA_BORDE_PRESIONADO				COLOR_BORDE_PRESIONADO
#define COLOR_MARCA_FONDO							COLOR_FONDO
#define COLOR_MARCA_FONDO_RESALTADO					COLOR_FONDO_RESALTADO
#define COLOR_MARCA_FONDO_PRESIONADO				COLOR_FONDO_PRESIONADO
#define COLOR_MARCA_FONDO_MARCA						COLOR_FONDO_CLARO
#define COLOR_MARCA_FONDO_MARCA_RESALTADO			COLOR_FONDO_CLARO_RESALTADO
#define COLOR_MARCA_FONDO_MARCA_PRESIONADO			COLOR_FONDO_CLARO_PRESIONADO
#define COLOR_MARCA_FONDO_MARCA_DESACTIVADO			COLOR_FONDO_CLARO_DESACTIVADO
#define COLOR_MARCA_TEXTO							COLOR_TEXTO
#define COLOR_MARCA_TEXTO_RESALTADO					COLOR_TEXTO_RESALTADO
#define COLOR_MARCA_TEXTO_PRESIONADO				COLOR_TEXTO_PRESIONADO
#define COLOR_MARCA_TEXTO_DESACTIVADO				COLOR_TEXTO_DESACTIVADO
#define COLOR_MARCA_TEXTO_SOMBRA					COLOR_TEXTO_SOMBRA

/*
  ______ _   _                  _        ______
 |  ____| | (_)                | |      |  ____|
 | |__  | |_ _  __ _ _   _  ___| |_ __ _| |__  __  __
 |  __| | __| |/ _` | | | |/ _ \ __/ _` |  __| \ \/ /
 | |____| |_| | (_| | |_| |  __/ || (_| | |____ >  <
 |______|\__|_|\__, |\__,_|\___|\__\__,_|______/_/\_\
				  | |
				  |_|
*/
#define COLOR_ETIQUETA_FONDO						COLOR_FONDO
#define COLOR_ETIQUETA_TEXTO						COLOR_TEXTO
#define COLOR_ETIQUETA_TEXTO_SOMBRA					COLOR_TEXTO_SOMBRA

