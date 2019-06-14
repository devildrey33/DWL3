#ifndef DMENSAJES_H
    #define DMENSAJES_H
	// BarraEx
	#define DWL_BARRAEX_CAMBIANDO							WM_USER + 1	// MouseDown y MouseMove
	#define DWL_BARRAEX_CAMBIADO							WM_USER + 2	// MouseUp

	// BotonEx
	#define DWL_BOTONEX_MOUSEDOWN							WM_USER + 3
	#define DWL_BOTONEX_MOUSEUP								WM_USER + 4	// Se lanza incluso fuera del control
	#define DWL_BOTONEX_CLICK								WM_USER + 5	// Solo se lanza si el mouse está dentro del control al soltar el boton

	// EditBox /////////////////////////////////////////////////////////////
	#define DWL_EDITBOX_CLICK_IZQUIERDO						WM_USER + 12
	#define DWL_EDITBOX_CLICK_DERECHO						WM_USER + 13
	#define DWL_EDITBOX_CLICK_MEDIO							WM_USER + 14
	#define DWL_EDITBOX_DOBLECLICK_IZQUIERDO				WM_USER + 15
	#define DWL_EDITBOX_DOBLECLICK_DERECHO					WM_USER + 16
	#define DWL_EDITBOX_DOBLECLICK_MEDIO					WM_USER + 17
	#define DWL_EDITBOX_TECLADO_PRESIONADO					WM_USER + 18
	#define DWL_EDITBOX_TECLADO_SOLTADO						WM_USER + 19
	#define DWL_EDITBOX_TECLADO_INTRO						WM_USER + 20
	#define DWL_EDITBOX_FOCO_OBTENIDO						WM_USER + 21
	#define DWL_EDITBOX_FOCO_PERDIDO						WM_USER + 22

	// ListView ////////////////////////////////////////////////////////////
//	#define DWL_LISTVIEW_RESALTAR_ITEM     					WM_USER + 39
/*    #define DWL_LISTVIEW_TERMINAR_CAPTURA                   WM_USER + 39
	#define DWL_LISTVIEW_TECLADO_INTRO						WM_USER + 40
    #define DWL_LISTVIEW_CLICK                              WM_USER + 41
    #define DWL_LISTVIEW_DOBLECLICK                         WM_USER + 42
	#define DWL_LISTVIEW_EMPEZAR_EDICION					WM_USER + 43
	#define DWL_LISTVIEW_TERMINAR_EDICION					WM_USER + 44
	#define DWL_LISTVIEW_CANCELAR_EDICION    				WM_USER + 45
	#define DWL_LISTVIEW_FOCO_OBTENIDO						WM_USER + 46
	#define DWL_LISTVIEW_FOCO_PERDIDO						WM_USER + 47

	// TreeView ////////////////////////////////////////////////////////////

	#define DWL_TREEVIEW_BORRARNODO							WM_USER + 53
	#define DWL_TREEVIEW_CLICK_IZQUIERDO					WM_USER + 54
	#define DWL_TREEVIEW_CLICK_DERECHO						WM_USER + 55
	#define DWL_TREEVIEW_CLICK_MEDIO						WM_USER + 56
	#define DWL_TREEVIEW_DOBLECLICK_IZQUIERDO				WM_USER + 57
	#define DWL_TREEVIEW_DOBLECLICK_DERECHO					WM_USER + 58
	#define DWL_TREEVIEW_DOBLECLICK_MEDIO					WM_USER + 59
	#define DWL_TREEVIEW_TECLADO_PRESIONADO					WM_USER + 60
	#define DWL_TREEVIEW_TECLADO_SOLTADO					WM_USER + 61
	#define DWL_TREEVIEW_TECLADO_INTRO						WM_USER + 62
	#define DWL_TREEVIEW_LABELEDIT_INICIO					WM_USER + 63
	#define DWL_TREEVIEW_LABELEDIT_FIN						WM_USER + 64
	#define DWL_TREEVIEW_LABELEDIT_CANCELADO				WM_USER + 65
	#define DWL_TREEVIEW_FOCO_OBTENIDO						WM_USER + 66
	#define DWL_TREEVIEW_FOCO_PERDIDO						WM_USER + 67
	#define DWL_TREEVIEW_NODO_EXPANDIENDO   				WM_USER + 68
    #define DWL_TREEVIEW_NODO_CAMBIOSELECCION               WM_USER + 69

	#define DWL_TREEVIEW_TERMINAR_CAPTURA					WM_USER + 70
	#define DWL_TREEVIEW_CLICK								WM_USER + 71
	#define DWL_TREEVIEW_DOBLECLICK							WM_USER + 72*/



	// DWLBaseWnd //////////////////////////////////////////////////////////
	#define DWL_CUSTOM_DRAWDLG								WM_USER + 112 //! Mensaje para responder a notificaciones de colores en dialogos (solo para controles estandar de windows)
	#define DWL_CUSTOM_DRAWWND								WM_USER + 113 //! Mensaje para responder a notificaciones de colores en ventanas (solo para controles estandar de windows)
    ////////////////////////////////////////////////////////////////////////
    #define DWL_NOTIFICACION                                WM_USER + 114 // Indica que se ha recibido una notificacion de un control que se la devolveremos para especificar mejor que tipo de control la manda.

	// DArbolEx ////////////////////////////////////////////////////////////
	#define DWL_ARBOLEX_MOUSEPRESIONADO						WM_USER + 200
	#define DWL_ARBOLEX_MOUSESOLTADO						WM_USER + 201				// Click y MouseSoltado son lo mismo
	#define DWL_ARBOLEX_MOUSEMOVIMIENTO						WM_USER + 202
	#define	DWL_ARBOLEX_CLICK                               DWL_ARBOLEX_MOUSESOLTADO	// Click y MouseSoltado son lo mismo
	#define DWL_ARBOLEX_DOBLECLICK							WM_USER + 203

	// DListaEx ////////////////////////////////////////////////////////////
	#define DWL_LISTAEX_MOUSEPRESIONADO						WM_USER + 300
	#define DWL_LISTAEX_MOUSESOLTADO						WM_USER + 301				// Click y MouseSoltado son lo mismo
	#define DWL_LISTAEX_MOUSEMOVIMIENTO						WM_USER + 302
	#define	DWL_LISTAEX_CLICK                               DWL_LISTAEX_MOUSESOLTADO	// Click y MouseSoltado son lo mismo
	#define DWL_LISTAEX_DOBLECLICK							WM_USER + 303

//	#define DWL_USAR_GESTOR_POR_DEFECTO						WM_USER + 120 //! Macro para definir el valor que debera devolver el GestorMensajes si queremos llamar a la función por defecto del windowprocedure.
	
	// DMarcaEx /////////////////////////////////////////////////////////////
	#define	DWL_MARCAEX_CLICK								WM_USER + 320

	// DEdicionTextoEx //////////////////////////////////////////////////////
	#define	DWL_EDICIONTEXTOEX_CLICK						WM_USER + 330
	#define	DWL_EDICIONTEXTOEX_CAMBIO						WM_USER + 331

	// DControlDesplegable //////////////////////////////////////////////////
	#define DWL_DESPLEGABLEEX_CAMBIO						WM_USER + 340

	// DMenuEx //////////////////////////////////////////////////////////////
	#define DWL_MENUEX_CLICK								WM_USER + 350
	#define DWL_MENUEX_BARRA_CAMBIADO						WM_USER + 351
	#define DWL_MENUEX_BARRA_CAMBIANDO						WM_USER + 352
#endif