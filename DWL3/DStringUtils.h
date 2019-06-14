#ifndef DSTRINGUTILS_H
	#define DSTRINGUTILS_H

//	#include "DObjeto.h"
	#include <vector>
	#include <string>
	#include <iomanip>
	// DString_ToStr, DString_StrTo
	#include <sstream>

	namespace DWL {
		namespace Strings {
			/* Split para separar una cadena por un carácter delimitador 
				NOTA : si no encuentra el delimitador, devuelve la cadena completa */
			class Split {
			  public:
											Split(std::wstring &Texto, const TCHAR Separador);
											Split(const TCHAR *Texto, const TCHAR Separador);
				                           ~Split(void);
				inline const size_t			Total(void) { return _TextoSeparado.size(); };
				std::wstring               &operator[] (const size_t Pos);
				const bool					operator() (void);
			  protected:
				void					   _SepararWide(const TCHAR *Texto, const TCHAR Separador);
				std::vector<std::wstring>  _TextoSeparado;
			};

			// Cuenta dentro de un string las veces que aparece un carácter
			const size_t ContarCaracter(std::wstring &Texto, wchar_t Caracter);


			/* Convierte cualquier valor decimal a cadena de caracteres, se puede especificar si se quiere un número de decimales. */
			template <typename T> std::wstring ToStrF(T Tmp, int Decimales = 0) {
				std::wostringstream out;
				out << std::fixed << std::setprecision(Decimales) << Tmp;
				return out.str();
			}


			/* Convierte cualquier tipo básico a cadena, se puede especificar si se quiere un número fijo de digitos */
			// UTILIZAR std::to_wstring y no esta función
			template <typename T> std::wstring ToStr(T Tmp, int Digitos = 0) {
				std::wostringstream out;

				if (Digitos > 1)
					out << std::setfill(L'0') << std::setw(Digitos) << Tmp;
				else
					out << Tmp;

				return out.str();
			}


			/* Convierte una cadena a un tipo básico */
			template <typename T> T StrTo(std::wstring &Tmp, T &output) {
				//			T output;
				std::wistringstream in(Tmp);
				in >> output;
				return output;
			}


			static const int AnsiToWide(const char *IN_Ansi, std::wstring &OUT_Wide) {
				wchar_t TmpWTxt[4096];
				int Ret = MultiByteToWideChar(CP_UTF8, MB_PRECOMPOSED, IN_Ansi, -1, TmpWTxt, 4096);
				OUT_Wide = TmpWTxt;
				return (Ret > 0) ? TRUE : FALSE;
			}


			static const int WideToAnsi(const wchar_t *IN_Wide, std::string &OUT_Ansi) {
				char TmpTxt[4096];
				int	Ret = WideCharToMultiByte(CP_UTF8, NULL, IN_Wide, -1, TmpTxt, 4096, NULL, NULL);
				OUT_Ansi = TmpTxt;
				return (Ret > 0) ? TRUE : FALSE;
			}



			/* Devuelve un string con el numero de bytes en formato legible (Bytes, KiloBytes, MegaBytes, GigaBytes, TeraBytes) */
			static std::wstring &FormatoBytes(const unsigned long long nBytes, std::wstring &OUT_StrBytes) {
				const wchar_t      *Medidas[5]	= { L"B", L"KB", L"MB", L"GB", L"TB" };
				int					PosMedidas	= 0;
				long double			Resultado	= static_cast<long double>(nBytes);
				unsigned long long  Bytes = nBytes;
				for (PosMedidas = 0; PosMedidas < 5 && Bytes >= 1024; PosMedidas++, Bytes /= 1024) {
					Resultado = static_cast<long double>(Bytes) / 1024.0f;
				}

				OUT_StrBytes = ToStrF(Resultado, 2) + Medidas[PosMedidas];

				return OUT_StrBytes;
			}

		};
	};
#endif