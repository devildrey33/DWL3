#ifndef DDLGABRIR_H
#define DDLGABRIR_H

#include <vector>
#include <string>

namespace DWL {

	class DDlgAbrir {
	  public:
									DDlgAbrir(void);
									DDlgAbrir(const wchar_t *PathSh, const wchar_t *Filtro, const wchar_t *Titulo, const bool MultiSeleccion = true, HWND hWndPadre = NULL);
					               ~DDlgAbrir(void);

								   //! Función que muestra el diálogo para abrir archivos.
									/*!	Función que muestra el diálogo para abrir archivos según los parametros especificados.
											\fn			const size_t Mostrar(const TCHAR *PathSh, const TCHAR *Filtro, const TCHAR *Titulo, const bool MultiSeleccion = true, HWND hWndPadre = NULL) {
											\param[in]	PathSh			: Ruta inicial desde donde empezara el dialogo abrir
											\param[in]  Filtro			: Filtro de archivos
											\param[in]  Titulo			: Titulo del dialogo
											\param[in]  MultiSeleccion	: Habilitar multiselección
											\param[in]  hWndPadre		: Ventana padre, puede ser NULL. Si especificamos una ventana padre, esta se desactivara mientras el dialogo este activo.
											\return		Devuelve el numero de archivos seleccionados.
											\remarks    Filtro debera estar formateado de la siguiente forma :\n
														\code Todos los archivos\0*.*\0\0 \endcode
														Observad que primero hay un string con el nombre del filtro, luego el tipo de filtro, y por ultimo termina con un doble caracter <b>\\0</b>. \n\n
														Si por ejemplo queremos tener un filtro para documentos de texto debemos construirlo de una forma similar a este : \n
														\code Todos los archivos\0*.*\0Documentos de texto\0*.doc;*.txt;*.rtf\0\0 \endcode
														Observad que esta vez hemos añadido 3 tipos de archivo para el filtro Documentos de texto, esos tipos de archivo deben estar separados por el caracter <b>;</b>
									*/
		  const size_t				Mostrar(const wchar_t *PathSh, const wchar_t *Filtro, const wchar_t *Titulo, const bool MultiSeleccion = true, HWND hWndPadre = NULL);
		  
		  inline std::wstring      &operator [] (const size_t Pos) { return Archivos[Pos]; }

		  std::vector<std::wstring>	Archivos;
	};

}

#endif