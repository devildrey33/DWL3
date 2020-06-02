#pragma once

/* DWL 3.1000
	V Crear Events lambda que enllaçin als events virtuals, (Si sobre-escrius el event lambda omitirá el event virtual)
	V Ampliar DSplit i DSplitA per poder utilitzar multiples delimitadors
	V Capturar violacións d'acces i demes coses extranyes amb el catch
		- El problema es que els strings del std:::exception venen amb ansi / utf8, i s'ha de pasar a wchar_t .... i no te solució standard... osigui que s'ha de fer a cada catch...
	V Afegir events Lambda a DMenuEx (Mostrar i Click)
	V Fer funció DebugLvl per mostrar la consola, crear un arxiu de log, etc..
		X I amb diferents comportaments segons si es DEBUG O RELEASE (això ho defineix l'aplicació com vol)

	- Crear objecte per enviar correus amb TLS.
		- https://docs.microsoft.com/en-us/windows/win32/secauthn/using-sspi-with-a-windows-sockets-client
		- http://www.coastrd.com/c-schannel-smtp					(TLSclient.c)
		- http://www.coastrd.com/sspi-schannel-tls-example-in-basic (TLSclient.bas)

	- ControlsEx
		- DListaEx :
			- Crear Finestra per buscar strings i que quedin resaltats a la llista.
			- Afinar el scroll amb la rodeta... no se com ho calculo pero amb pocs items avança maça poc, i amb molts items queda mes o menys bé.
		- DArbolEx :
			- Crear Finestra per buscar strings i que quedin resaltats a l'arbre.
			- Afinar el scroll amb la rodeta... no se com ho calculo pero amb pocs items avança maça poc, i amb molts items queda mes o menys bé.
		- DMarcoScrollEx : 
			- Afinar el scroll amb la rodeta... no se com ho calculo pero amb pocs items avança maça poc, i amb molts items queda mes o menys bé.
			- S'ha de calcular un valor que quan hi hagi pocs items sigui mes gran, i que quan hi hagi molts items sigui com a maxim una cuarta part de la página.
*/
