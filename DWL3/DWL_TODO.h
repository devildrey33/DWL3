#pragma once

/* DWL 3.1000
	V Crear Events lambda que enllaçin als events virtuals, (Si sobre-escrius el event lambda omitirá el event virtual)
	V Ampliar DSplit i DSplitA per poder utilitzar multiples delimitadors
	V Capturar violacións d'acces i demes coses extranyes amb el catch
		- El problema es que els strings del std:::exception venen amb ansi / utf8, i s'ha de pasar a wchar_t .... i no te solució standard... osigui que s'ha de fer a cada catch...
	- Fer funció DebugLvl per mostrar la consola, crear un arxiu de log, etc..
		- I amd diferents comportaments segons si es DEBUG O RELEASE

	- ControlsEx
		- DListaEx :
			- Crear Finestra per buscar strings i que quedin resaltats a la llista.
			- Afinar el scxroll amb la rodeta... no se com ho calculo pero amb pocs items avança maça poc, i amb molts items queda mes o menys bé.
		- DArbolEx :
			- Crear Finestra per buscar strings i que quedin resaltats a l'arbre.
			- Afinar el scxroll amb la rodeta... no se com ho calculo pero amb pocs items avança maça poc, i amb molts items queda mes o menys bé.
		- DMarcoScrollEx : 
			- Afinar el scxroll amb la rodeta... no se com ho calculo pero amb pocs items avança maça poc, i amb molts items queda mes o menys bé.
*/
