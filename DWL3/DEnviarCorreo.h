#ifndef DENVIARCORREO_H
#define DENVIARCORREO_H

#define SECURITY_WIN32

#include <string>
#include <functional>
#include <vector>
#include <winsock2.h>
#include <sspi.h>
#include <wincrypt.h>
#include <schannel.h>



namespace DWL {
	// Objeto para enviar correos encriptados
	// Creado a partir de http://www.coastrd.com/c-schannel-smtp
	class DEnviarCorreo {
      public:
												DEnviarCorreo(void);
											   ~DEnviarCorreo(void);

												// Función que añade el archivo adjunto especificado
	    void									AgregarAdjunto(const wchar_t* Path);

		void									Servidor(const char *nServidor, const UINT Puerto = 0);

	    void									Texto(const char *nTexto);

	    void									Titulo(const char *nTitulo);

		void									CorreoOrigen(const char *nCorreo);

		void									CorreoDestino(const char *nCorreo);

												// Función que : conecta con el servidor, hace la autentificación, y envia el mensaje con sus adjuntos
		const BOOL								Enviar(	std::function<void()>					LambdaTerminado		= [=]()					{ },		// Función lambda que se ejecuta al terminar
														std::function<void(const int mError)>	LambdaError			= [=](const int nError) { },		// Función lambda que se ejecuta si hay un error
														std::function<void(const int Valor)>	LambdaPorcentaje	= [=](int Valor)		{ }		);  // Función lambda que se ejecuta periodicamente al aumentar el porcentaje de envio

	  protected:
		void                                   _Terminar(void);

												// Thread para enviar el correo
		static unsigned long                   _ThreadEnviar(void *pThis);

		std::wstring                          &_StrWinsockError(const DWORD nError);
		std::wstring                          &_StrCertVerifyCertificateError(const DWORD nError);

		const BOOL                             _IniciarCrypto(void);
		
		const BOOL                             _IniciarWinsock(void);
		void								   _ErrorWinsock(DWORD ErrCode);

		const BOOL                             _IniciarCredenciales(void);

		const BOOL                             _ObtenerNuevasCredenciales(void);

												// Función que conecta al servidor 
	    const BOOL							   _Conectar(void);

												// Función que se desconecta del servidor
		void								   _Desconectar(void);

												// Función para loguearse con la cuenta de correo origen
//		const BOOL							   _Autentificar(void);


		const BOOL                             _EnviarHelo(void);
//		const BOOL                             _EnviarStartTLS(void);
		const BOOL                             _EnviarAuth(void);
		const BOOL                             _EnviarHandShake(void);
		SECURITY_STATUS                        _BucleHandShake(const BOOL LecturaInicial);

		const BOOL                             _AutentificarCredencialesServidor(void);
		const DWORD							   _VerificarCertificadoServidor(PCCERT_CONTEXT pServerCert, PSTR pszServerName, DWORD dwCertFlags);
		void								   _DisplayCertChain(PCCERT_CONTEXT  pServerCert, BOOL fLocal);
		void								   _DisplayConnectionInfo(CtxtHandle* phContext);
		void								   _ErrorWinVerifyTrust(DWORD Status);

//		const BOOL                             _Enviar(const char *eTexto);
//		const BOOL                             _Recibir(void);

		const BOOL                             _Encriptar(PBYTE pbIoBuffer, SecPkgContext_StreamSizes Sizes);
		const BOOL                             _Desencriptar(PBYTE pbIoBuffer, DWORD cbIoBufferLength);

		std::string                            _Servidor;
	    std::string                            _Texto;
		std::string                            _Titulo;
		std::string                            _CorreoOrigen;
		std::string                            _CorreoDestino;
		std::string                            _Login;
		std::string                            _Password;
		std::vector<std::wstring>              _Adjuntos;

		HANDLE                                 _Thread;
		SCHANNEL_CRED                          _SchannelCred;
		DWORD                                  _Protocolo; // SP_PROT_TLS1;  SP_PROT_TLS1;  SP_PROT_PCT1;  SP_PROT_SSL2;  SP_PROT_SSL3;  0=default
		ALG_ID								   _aiKeyExch; // = default; CALG_DH_EPHEM; CALG_RSA_KEYX;
		SecBuffer                              _SeBufferData;

		SOCKET                                 _Socket;
		SOCKADDR_IN                            _SockAddr;
		PSecurityFunctionTableA                _SFT; // Security Funciton Table
		HCERTSTORE                             _Certificado;
		CredHandle                             _Credenciales;
		BOOL                                   _CredencialesCreadas;
		CtxtHandle                             _Contexto; 
		BOOL                                   _ContextoCreado;

		std::function<void()>				   _EventoTerminado;
		std::function<void(const int mError)>  _EventoError;
		std::function<void(const int Valor)>   _EventoPorcentaje;
	};

}

#endif

