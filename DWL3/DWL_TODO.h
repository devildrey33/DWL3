#pragma once

/* DWL 3.1000
	V Crear Events lambda que enlla�in als events virtuals, (Si sobre-escrius el event lambda omitir� el event virtual)
	V Ampliar DSplit i DSplitA per poder utilitzar multiples delimitadors
	V Capturar violaci�ns d'acces i demes coses extranyes amb el catch
		- El problema es que els strings del std:::exception venen amb ansi / utf8, i s'ha de pasar a wchar_t .... i no te soluci� standard... osigui que s'ha de fer a cada catch...
	- Fer funci� DebugLvl per mostrar la consola, crear un arxiu de log, etc..
		- I amd diferents comportaments segons si es DEBUG O RELEASE

	- ControlsEx
		- DListaEx :
			- Crear Finestra per buscar strings i que quedin resaltats a la llista.
			- Afinar el scxroll amb la rodeta... no se com ho calculo pero amb pocs items avan�a ma�a poc, i amb molts items queda mes o menys b�.
		- DArbolEx :
			- Crear Finestra per buscar strings i que quedin resaltats a l'arbre.
			- Afinar el scxroll amb la rodeta... no se com ho calculo pero amb pocs items avan�a ma�a poc, i amb molts items queda mes o menys b�.
		- DMarcoScrollEx : 
			- Afinar el scxroll amb la rodeta... no se com ho calculo pero amb pocs items avan�a ma�a poc, i amb molts items queda mes o menys b�.
*/
