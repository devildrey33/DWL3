#ifndef DSTRINGUTILS_H
	#define DSTRINGUTILS_H

//	#include "DObjeto.h"
	#include <vector>
	#include <string>
	#include <iomanip>
	// DString_ToStr, DString_StrTo
	#include <sstream>

	namespace DWL {
		class Strings {
		  public:
			/* Split para separar una cadena por un carácter delimitador 
				NOTA : si no encuentra el delimitador, devuelve la cadena completa en el operador [0] */
			class Split {
			  public:
											Split(std::wstring &Texto, const wchar_t Separador);
											Split(const wchar_t*Texto, const wchar_t Separador);
				                           ~Split(void);
				inline const size_t			Total(void) { return _TextoSeparado.size(); };
				std::wstring               &operator[] (const size_t Pos);
				const bool					operator() (void);
			  protected:
				void					   _SepararWide(const wchar_t *Texto, const wchar_t Separador);
				std::vector<std::wstring>  _TextoSeparado;
			};

			class SplitA {
			  public:
											SplitA(std::string &Texto, const char Separador);
											SplitA(const char *Texto, const char Separador);
				                           ~SplitA(void);
				inline const size_t			Total(void) { return _TextoSeparado.size(); };
				std::string                &operator[] (const size_t Pos);
				const bool					operator() (void);
			  protected:
				void					   _SepararAnsi(const char *Texto, const char Separador);
				std::vector<std::string>   _TextoSeparado;
			};


			/* Convierte cualquier valor decimal a cadena de caracteres, se puede especificar si se quiere un número de decimales. */
			template <typename T> static std::wstring ToStrF(T Tmp, int Decimales = 0) {
				std::wostringstream out;
				out << std::fixed << std::setprecision(Decimales) << Tmp;
				return out.str();
			}


			/* Convierte cualquier tipo básico a cadena, se puede especificar si se quiere un número fijo de digitos */
			// UTILIZAR std::to_wstring y no esta función
			template <typename T> static std::wstring ToStr(T Tmp, int Digitos = 0) {
				std::wostringstream out;

				if (Digitos > 1)
					out << std::setfill(L'0') << std::setw(Digitos) << Tmp;
				else
					out << Tmp;

				return out.str();
			}


			/* Convierte una cadena a un tipo básico */
			template <typename T> static T StrTo(std::wstring& Tmp, T& output) {
				//			T output;
				std::wistringstream in(Tmp);
				in >> output;
				return output;
			}


			// Cuenta dentro de un string las veces que aparece un carácter
			static const size_t ContarCaracter(std::wstring& Texto, wchar_t Caracter);

			// char a std::wstring
/*			static const int UTF8ToWide(const char* IN_UTF8, std::wstring& OUT_Wide);

			// wchar_t a std::string
			static const int WideToUTF8(const wchar_t* IN_Wide, std::string& OUT_UTF8);*/

			// char a std::wstring
			static const int AnsiToWide(const char* IN_Ansi, std::wstring& OUT_Wide);

			// wchar_t a std::string
			static const int WideToAnsi(const wchar_t* IN_Wide, std::string& OUT_Ansi);

			/* Devuelve un string con el numero de bytes en formato legible (Bytes, KiloBytes, MegaBytes, GigaBytes, TeraBytes) */
			static std::wstring& FormatoBytes(const unsigned long long nBytes, std::wstring& OUT_StrBytes);

			// Devuelve un string con el nombre de archivo sin el resto del path
			static std::wstring& Path_BaseName(const wchar_t* nPath, const BOOL Extension = FALSE);

			// Devuelve un string con el path sin el nombre de archivo
			// Puedes hacer que devuelva uno o varios directorios inferiores si ajustas la variable BajarPaths
			static std::wstring& Path_DirName(const wchar_t* nPath, const UINT BajarPaths = 0);
		};
	};
#endif