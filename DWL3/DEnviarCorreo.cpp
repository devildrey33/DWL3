#include "pch.h"
#include "DEnviarCorreo.h"
#include <Ws2tcpip.h>


#define IO_BUFFER_SIZE 0x10000



namespace DWL {
	// Constructor
	DEnviarCorreo::DEnviarCorreo(void) : _Contexto({ 0, 0 }), _Credenciales({ 0, 0 }), _Puerto(0), _Socket(NULL), _SFT(NULL), _Certificado(NULL), _Protocolo(0), _aiKeyExch(0), _CredencialesCreadas(FALSE), _Thread(NULL), _ContextoCreado(FALSE) {

	}

	// Destructor
	DEnviarCorreo::~DEnviarCorreo(void) {

	}

	// Función que envia el correo
	const BOOL DEnviarCorreo::Enviar(	std::function<void()>					LambdaTerminado,		// Función lambda que se ejecuta al terminar
										std::function<void(const int mError)>	LambdaError,			// Función lambda que se ejecuta si hay un error
										std::function<void(const int Valor)>	LambdaPorcentaje)	{   // Función lambda que se ejecuta periodicamente al aumentar el porcentaje de envio
		// Guardo los eventos lambda
		_EventoTerminado  = LambdaTerminado;
		_EventoError      = LambdaError;
		_EventoPorcentaje = LambdaPorcentaje;

		// Si ya existe un thread, salgo de la función
		if (_Thread != NULL) {
			return FALSE; // Ya existe un thread
		}

		// Inicio el thread para enviar el correo
		DEnviarCorreo *This = this;
		_Thread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)this->_ThreadEnviar, reinterpret_cast<void *>(this), 0, NULL);
		// Si se ha creado el thread, asigno su prioridad
		if (_Thread) SetThreadPriority(_Thread, 0);
		// Si no se ha creado el thread devuelvo false
		else		 return FALSE;

		// Devuelvo TRUE conforme todo ha salido bien, y se está enviado el mensaje en un nuevo thread
		return TRUE;
	}



	// Thread para enviar el correo
	unsigned long  DEnviarCorreo::_ThreadEnviar(void *pThis) {
//		DEnviarCorreo *This = static_cast<DEnviarCorreo *>(pThis);
		DEnviarCorreo *This = reinterpret_cast<DEnviarCorreo *>(pThis);
		// Cargo la librería para la criptografia
		if (This->_IniciarCrypto() == FALSE) {
			This->_Terminar();
			return FALSE;
		}

		// Inicio el Winsock
		if (This->_IniciarWinsock() == FALSE) {
			This->_Terminar();
			return FALSE;
		}

		// Inicio las credenciales
		if (This->_IniciarCredenciales() == FALSE) {
			This->_Terminar();
			return FALSE;
		}

		// Conecto al servidor
		if (This->_Conectar() == FALSE) {
			This->_Terminar();
			return FALSE;
		}

		// Realizo el HandShake para la encriptación
		if (This->_EnviarHandShake() == FALSE) {
			This->_Terminar();
			return FALSE;
		}

		// Autentifico las credenciales del servidor
		if (This->_AutentificarCredencialesServidor() == FALSE) {
			This->_Terminar();
			return FALSE;
		}




		// Envio el EHLO y logueo al usuario
		if (This->_EnviarHelo() == FALSE) {
			This->_Terminar();
			return FALSE;
		}

		return TRUE;
	}



	/*****************************************************************************/
	const BOOL DEnviarCorreo::_EnviarHelo(void) {
		SecPkgContext_StreamSizes Sizes;            // unsigned long cbBuffer;    // Size of the buffer, in bytes
		SECURITY_STATUS           scRet;            // unsigned long BufferType;  // Type of the buffer (below)        
		PBYTE                     pbIoBuffer;		// void    SEC_FAR * pvBuffer;   // Pointer to the buffer
		DWORD                     cbIoBufferLength, cbData;


		// Read stream encryption properties.
		scRet = _SFT->QueryContextAttributesA(&_Contexto, SECPKG_ATTR_STREAM_SIZES, &Sizes);
		if (scRet != SEC_E_OK)	{
			printf("**** Error 0x%x reading SECPKG_ATTR_STREAM_SIZES\n", scRet); return scRet;
		}


		// Create a buffer.
		cbIoBufferLength	= Sizes.cbHeader + Sizes.cbMaximumMessage + Sizes.cbTrailer;
		pbIoBuffer			= (PBYTE)LocalAlloc(LMEM_FIXED, cbIoBufferLength);
		if (pbIoBuffer == NULL) { printf("**** Out of memory (2)\n"); return SEC_E_INTERNAL_ERROR; }


		// Receive a Response
		scRet = _Desencriptar(pbIoBuffer, cbIoBufferLength);
		if (scRet != SEC_E_OK) return scRet;


		// Build the request - must be < maximum message size
		sprintf_s((char *)pbIoBuffer + Sizes.cbHeader, cbIoBufferLength, "%s", "EHLO \r\n"); // message begins after the header


		// Send a request.
		cbData = _Encriptar(pbIoBuffer, Sizes);
		if (cbData == SOCKET_ERROR || cbData == 0) {
			printf("**** Error %d sending data to server (3)\n", WSAGetLastError());
			return FALSE; // SEC_E_INTERNAL_ERROR;
		}


		// Receive a Response
		scRet = _Desencriptar(pbIoBuffer, cbIoBufferLength);
		if (scRet != SEC_E_OK) return scRet;




		// Build the request - must be < maximum message size
		sprintf_s((char *)(pbIoBuffer + Sizes.cbHeader), cbIoBufferLength, "%s", "QUIT \r\n"); // message begins after the header


		// Send a request.
		cbData = _Encriptar(pbIoBuffer, Sizes);
		if (cbData == SOCKET_ERROR || cbData == 0) {
			printf("**** Error %d sending data to server (3)\n", WSAGetLastError()); 
			return FALSE; // SEC_E_INTERNAL_ERROR;
		}


		// Receive a Response
		scRet = _Desencriptar(pbIoBuffer, cbIoBufferLength);
		
//		if (scRet != SEC_E_OK) return scRet;


		return (scRet == SEC_E_OK) ? TRUE : FALSE; //  SEC_E_OK;
	}


	const BOOL DEnviarCorreo::_AutentificarCredencialesServidor(void) {
		// Autentifico las credenciales del servidor
		PCCERT_CONTEXT  pRemoteCertContext	= NULL;
		SECURITY_STATUS Status				= _SFT->QueryContextAttributesA(&_Contexto, SECPKG_ATTR_REMOTE_CERT_CONTEXT, (PVOID)& pRemoteCertContext);
		if (Status != SEC_E_OK)	{
			Debug_Escribir_Varg(L"DEnviarCorreo::_AutentificarCredencialesServidor Error 0x%x obteniendo el certificado remoto\n", Status); 
			return FALSE;
		} 

		// Muestro el certificado del servidor
		_DisplayCertChain(pRemoteCertContext, FALSE); 


		// Display connection info.
		_DisplayConnectionInfo(&_Contexto); 



		return TRUE;
	}



	/*****************************************************************************/
	const DWORD DEnviarCorreo::_VerificarCertificadoServidor(PCCERT_CONTEXT pServerCert, PSTR pszServerName, DWORD dwCertFlags) {
		HTTPSPolicyCallbackData  polHttps;
		CERT_CHAIN_POLICY_PARA   PolicyPara;
		CERT_CHAIN_POLICY_STATUS PolicyStatus;
		CERT_CHAIN_PARA          ChainPara;
		PCCERT_CHAIN_CONTEXT     pChainContext = NULL;
		DWORD                    cchServerName, Status;
		char a1[] = szOID_PKIX_KP_SERVER_AUTH;
		char a2[] = szOID_SERVER_GATED_CRYPTO;
		char a3[] = szOID_SGC_NETSCAPE;


/*		LPSTR rgszUsages[] = { szOID_PKIX_KP_SERVER_AUTH,
								   szOID_SERVER_GATED_CRYPTO,
								   szOID_SGC_NETSCAPE };*/

/*		char rgszUsages[3][64] = {
			szOID_PKIX_KP_SERVER_AUTH,
			szOID_SERVER_GATED_CRYPTO,
			szOID_SGC_NETSCAPE
		};*/

		LPSTR rgszUsages[] = { a1, a2, a3 };

		DWORD cUsages = sizeof(rgszUsages) / sizeof(LPSTR);

		PWSTR   pwszServerName = NULL;


		if (pServerCert == NULL) {
			Status = SEC_E_WRONG_PRINCIPAL; goto cleanup;
		}

		// Convert server name to unicode.
		if (pszServerName == NULL || strlen(pszServerName) == 0) {
			Status = SEC_E_WRONG_PRINCIPAL; goto cleanup;
		}

		cchServerName = MultiByteToWideChar(CP_ACP, 0, pszServerName, -1, NULL, 0);
		pwszServerName = (PWSTR)LocalAlloc(LMEM_FIXED, cchServerName * sizeof(WCHAR));
		if (pwszServerName == NULL) {
			Status = SEC_E_INSUFFICIENT_MEMORY; goto cleanup;
		}

		cchServerName = MultiByteToWideChar(CP_ACP, 0, pszServerName, -1, pwszServerName, cchServerName);
		if (cchServerName == 0) 	{
			Status = SEC_E_WRONG_PRINCIPAL; goto cleanup;
		}


		// Build certificate chain.
		ZeroMemory(&ChainPara, sizeof(ChainPara));
		ChainPara.cbSize = sizeof(ChainPara);
		ChainPara.RequestedUsage.dwType = USAGE_MATCH_TYPE_OR;
		ChainPara.RequestedUsage.Usage.cUsageIdentifier = cUsages;
		ChainPara.RequestedUsage.Usage.rgpszUsageIdentifier = rgszUsages;

		if (!CertGetCertificateChain(NULL,
			pServerCert,
			NULL,
			pServerCert->hCertStore,
			&ChainPara,
			0,
			NULL,
			&pChainContext))
		{
			Status = GetLastError();
			printf("Error 0x%x returned by CertGetCertificateChain!\n", Status);
			goto cleanup;
		}


		// Validate certificate chain.
		ZeroMemory(&polHttps, sizeof(HTTPSPolicyCallbackData));
		polHttps.cbStruct = sizeof(HTTPSPolicyCallbackData);
		polHttps.dwAuthType = AUTHTYPE_SERVER;
		polHttps.fdwChecks = dwCertFlags;
		polHttps.pwszServerName = pwszServerName;

		memset(&PolicyPara, 0, sizeof(PolicyPara));
		PolicyPara.cbSize = sizeof(PolicyPara);
		PolicyPara.pvExtraPolicyPara = &polHttps;

		memset(&PolicyStatus, 0, sizeof(PolicyStatus));
		PolicyStatus.cbSize = sizeof(PolicyStatus);

		if (!CertVerifyCertificateChainPolicy(CERT_CHAIN_POLICY_SSL,
			pChainContext,
			&PolicyPara,
			&PolicyStatus))
		{
			Status = GetLastError();
			printf("Error 0x%x returned by CertVerifyCertificateChainPolicy!\n", Status);
			goto cleanup;
		}

		if (PolicyStatus.dwError)
		{
			Status = PolicyStatus.dwError;
			_ErrorWinVerifyTrust(Status);
			goto cleanup;
		}

		Status = SEC_E_OK;


	cleanup:
		if (pChainContext)  CertFreeCertificateChain(pChainContext);
		if (pwszServerName) LocalFree(pwszServerName);

		return Status;
	}



	/*****************************************************************************/
	void DEnviarCorreo::_DisplayConnectionInfo(CtxtHandle* phContext) {

		SECURITY_STATUS Status;
		SecPkgContext_ConnectionInfo ConnectionInfo;

		Status = _SFT->QueryContextAttributesA(phContext, SECPKG_ATTR_CONNECTION_INFO, (PVOID)& ConnectionInfo);
		if (Status != SEC_E_OK) { 
			printf("Error 0x%x querying connection info\n", Status); return; 
		}

		printf("\n");

		switch (ConnectionInfo.dwProtocol) {
			case SP_PROT_TLS1_CLIENT:
				printf("Protocol: TLS1\n");
				break;
			case SP_PROT_SSL3_CLIENT:
				printf("Protocol: SSL3\n");
				break;
			case SP_PROT_PCT1_CLIENT:
				printf("Protocol: PCT\n");
				break;
			case SP_PROT_SSL2_CLIENT:
				printf("Protocol: SSL2\n");
				break;
			default:
				printf("Protocol: 0x%x\n", ConnectionInfo.dwProtocol);
				break;
		}

		switch (ConnectionInfo.aiCipher) {
			case CALG_RC4:
				printf("Cipher: RC4\n");
				break;
			case CALG_3DES:
				printf("Cipher: Triple DES\n");
				break;
			case CALG_RC2:
				printf("Cipher: RC2\n");
				break;
			case CALG_DES:
			case CALG_CYLINK_MEK:
				printf("Cipher: DES\n");
				break;
			case CALG_SKIPJACK:
				printf("Cipher: Skipjack\n");
				break;
			default:
				printf("Cipher: 0x%x\n", ConnectionInfo.aiCipher);
				break;
		}

		printf("Cipher strength: %d\n", ConnectionInfo.dwCipherStrength);

		switch (ConnectionInfo.aiHash)	{
			case CALG_MD5:
				printf("Hash: MD5\n");
				break;
			case CALG_SHA:
				printf("Hash: SHA\n");
				break;
			default:
				printf("Hash: 0x%x\n", ConnectionInfo.aiHash);
				break;
		}

		printf("Hash strength: %d\n", ConnectionInfo.dwHashStrength);

		switch (ConnectionInfo.aiExch) {
			case CALG_RSA_KEYX:
			case CALG_RSA_SIGN:
				printf("Key exchange: RSA\n");
				break;
			case CALG_KEA_KEYX:
				printf("Key exchange: KEA\n");
				break;
			case CALG_DH_EPHEM:
				printf("Key exchange: DH Ephemeral\n");
				break;
			default:
				printf("Key exchange: 0x%x\n", ConnectionInfo.aiExch);
				break;
		}

		printf("Key exchange strength: %d\n", ConnectionInfo.dwExchStrength);
	}



	/*****************************************************************************/
	void DEnviarCorreo::_DisplayCertChain(PCCERT_CONTEXT  pServerCert, BOOL fLocal) {
		CHAR szName[1000];
		PCCERT_CONTEXT pCurrentCert, pIssuerCert;
		DWORD dwVerificationFlags;

		printf("\n");

		// display leaf name
		if (!CertNameToStrA(pServerCert->dwCertEncodingType,	&pServerCert->pCertInfo->Subject, CERT_X500_NAME_STR | CERT_NAME_STR_NO_PLUS_FLAG, szName, sizeof(szName))) {
			printf("**** Error 0x%x building subject name\n", GetLastError());
		}

		if (fLocal) printf("Client subject: %s\n", szName);
		else		printf("Server subject: %s\n", szName);

		if (!CertNameToStrA(pServerCert->dwCertEncodingType, &pServerCert->pCertInfo->Issuer, CERT_X500_NAME_STR | CERT_NAME_STR_NO_PLUS_FLAG, szName, sizeof(szName)))	{
			printf("**** Error 0x%x building issuer name\n", GetLastError());
		}

		if (fLocal) printf("Client issuer: %s\n", szName);
		else printf("Server issuer: %s\n\n", szName);


		// display certificate chain
		pCurrentCert = pServerCert;
		while (pCurrentCert != NULL) {
			dwVerificationFlags = 0;
			pIssuerCert = CertGetIssuerCertificateFromStore(pServerCert->hCertStore, pCurrentCert, NULL, &dwVerificationFlags);
			if (pIssuerCert == NULL) {
				if (pCurrentCert != pServerCert) CertFreeCertificateContext(pCurrentCert);
				break;
			}

			if (!CertNameToStrA(pIssuerCert->dwCertEncodingType, &pIssuerCert->pCertInfo->Subject,	CERT_X500_NAME_STR | CERT_NAME_STR_NO_PLUS_FLAG, szName, sizeof(szName))) {
				printf("**** Error 0x%x building subject name\n", GetLastError());
			}

			printf("CA subject: %s\n", szName);

			if (!CertNameToStrA(pIssuerCert->dwCertEncodingType, &pIssuerCert->pCertInfo->Issuer, CERT_X500_NAME_STR | CERT_NAME_STR_NO_PLUS_FLAG, szName, sizeof(szName)))	{
				printf("**** Error 0x%x building issuer name\n", GetLastError());
			}

			printf("CA issuer: %s\n\n", szName);

			if (pCurrentCert != pServerCert) CertFreeCertificateContext(pCurrentCert);
			pCurrentCert = pIssuerCert;
			pIssuerCert = NULL;
		}
	}


	const BOOL DEnviarCorreo::_IniciarWinsock(void) {
		WSADATA				WSData;
		// Attempt to intialize WinSock (2.2 or later)
		if (WSAStartup(MAKEWORD(2, 2), &WSData)) {
			Debug_Escribir_Varg(L"DEnviarCorreo::_IniciarWinsock Error cargando Winsock.\n");
			return FALSE;
		}
		return TRUE;
	}



	/*****************************************************************************/
	void DEnviarCorreo::_ErrorWinsock(DWORD ErrCode)	{
		const wchar_t *StrError = nullptr; // http://www.sockets.com/err_lst1.htm#WSANO_DATA

		switch (ErrCode) {// http://msdn.microsoft.com/en-us/library/ms740668(VS.85).aspx
		
			case    10035:  StrError = L"WSAEWOULDBLOCK    "; break;
			case    10036:  StrError = L"WSAEINPROGRESS    "; break;
			case    10037:  StrError = L"WSAEALREADY       "; break;
			case    10038:  StrError = L"WSAENOTSOCK       "; break;
			case    10039:  StrError = L"WSAEDESTADDRREQ   "; break;
			case    10040:  StrError = L"WSAEMSGSIZE       "; break;
			case    10041:  StrError = L"WSAEPROTOTYPE     "; break;
			case    10042:  StrError = L"WSAENOPROTOOPT    "; break;
			case	10043:  StrError = L"WSAEPROTONOSUPPORT"; break;
			case	10044:  StrError = L"WSAESOCKTNOSUPPORT"; break;
			case    10045:  StrError = L"WSAEOPNOTSUPP     "; break;
			case    10046:  StrError = L"WSAEPFNOSUPPORT   "; break;
			case    10047:  StrError = L"WSAEAFNOSUPPORT   "; break;
			case    10048:  StrError = L"WSAEADDRINUSE     "; break;
			case    10049:  StrError = L"WSAEADDRNOTAVAIL  "; break;
			case    10050:  StrError = L"WSAENETDOWN       "; break;
			case    10051:  StrError = L"WSAENETUNREACH    "; break;
			case    10052:  StrError = L"WSAENETRESET      "; break;
			case    10053:  StrError = L"WSAECONNABORTED   "; break;
			case    10054:  StrError = L"WSAECONNRESET     "; break;
			case    10055:  StrError = L"WSAENOBUFS        "; break;
			case    10056:  StrError = L"WSAEISCONN        "; break;
			case    10057:  StrError = L"WSAENOTCONN       "; break;
			case    10058:  StrError = L"WSAESHUTDOWN      "; break;
			case    10059:  StrError = L"WSAETOOMANYREFS   "; break;
			case    10060:  StrError = L"WSAETIMEDOUT      "; break;
			case    10061:  StrError = L"WSAECONNREFUSED   "; break;
			case    10062:  StrError = L"WSAELOOP          "; break;
			case    10063:  StrError = L"WSAENAMETOOLONG   "; break;
			case    10064:  StrError = L"WSAEHOSTDOWN      "; break;
			case    10065:  StrError = L"WSAEHOSTUNREACH   "; break;
			case    10066:  StrError = L"WSAENOTEMPTY      "; break;
			case    10067:  StrError = L"WSAEPROCLIM       "; break;
			case    10068:  StrError = L"WSAEUSERS         "; break;
			case    10069:  StrError = L"WSAEDQUOT         "; break;
			case    10070:  StrError = L"WSAESTALE         "; break;
			case    10071:  StrError = L"WSAEREMOTE        "; break;
			case    10091:  StrError = L"WSASYSNOTREADY    "; break;
			case    10092:  StrError = L"WSAVERNOTSUPPORTED"; break;
			case    10093:  StrError = L"WSANOTINITIALISED "; break;
			case    11001:  StrError = L"WSAHOST_NOT_FOUND "; break;
			case    11002:  StrError = L"WSATRY_AGAIN      "; break;
			case    11003:  StrError = L"WSANO_RECOVERY    "; break;
			case    11004:  StrError = L"WSANO_DATA        "; break;
		}
		
		Debug_Escribir_Varg(L"Error WinSock : 0x%x %s", ErrCode, StrError);
	}



	/*****************************************************************************/
	void DEnviarCorreo::_ErrorWinVerifyTrust(DWORD Status) {
		const wchar_t * StrError = NULL;

		switch (Status) {
			case CERT_E_EXPIRED:                StrError = L"CERT_E_EXPIRED";                 break;
			case CERT_E_VALIDITYPERIODNESTING:  StrError = L"CERT_E_VALIDITYPERIODNESTING";   break;
			case CERT_E_ROLE:                   StrError = L"CERT_E_ROLE";                    break;
			case CERT_E_PATHLENCONST:           StrError = L"CERT_E_PATHLENCONST";            break;
			case CERT_E_CRITICAL:               StrError = L"CERT_E_CRITICAL";                break;
			case CERT_E_PURPOSE:                StrError = L"CERT_E_PURPOSE";                 break;
			case CERT_E_ISSUERCHAINING:         StrError = L"CERT_E_ISSUERCHAINING";          break;
			case CERT_E_MALFORMED:              StrError = L"CERT_E_MALFORMED";               break;
			case CERT_E_UNTRUSTEDROOT:          StrError = L"CERT_E_UNTRUSTEDROOT";           break;
			case CERT_E_CHAINING:               StrError = L"CERT_E_CHAINING";                break;
			case TRUST_E_FAIL:                  StrError = L"TRUST_E_FAIL";                   break;
			case CERT_E_REVOKED:                StrError = L"CERT_E_REVOKED";                 break;
			case CERT_E_UNTRUSTEDTESTROOT:      StrError = L"CERT_E_UNTRUSTEDTESTROOT";       break;
			case CERT_E_REVOCATION_FAILURE:     StrError = L"CERT_E_REVOCATION_FAILURE";      break;
			case CERT_E_CN_NO_MATCH:            StrError = L"CERT_E_CN_NO_MATCH";             break;
			case CERT_E_WRONG_USAGE:            StrError = L"CERT_E_WRONG_USAGE";             break;
			default:                            StrError = L"(unknown)";                      break;
		}
		Debug_Escribir_Varg(L"Error WinVerifyTrust : 0x%x", Status, StrError);
	}

	// Función que inicia la librería de Cryptografia
	// Dado que los correos trabajan en ANSI inicio la Cryptografia con las funciones ANSI
	const BOOL DEnviarCorreo::_IniciarCrypto(void) {
		// Ya está iniciado
		if (_SFT != NULL) return TRUE;

		_SFT = InitSecurityInterfaceA();

		return (_SFT == NULL) ? FALSE : TRUE;
	}


	// Función que genera las credenciales iniciales
	const BOOL DEnviarCorreo::_IniciarCredenciales(void) {
		// Ya está iniciado
		if (_CredencialesCreadas == TRUE) return TRUE;

		TimeStamp        tsExpiry;
		SECURITY_STATUS  Status;
		DWORD            cSupportedAlgs = 0;
		ALG_ID           rgbSupportedAlgs[16];
		PCCERT_CONTEXT   pCertContext = NULL;


		// Open the "MY" certificate store, where IE stores client certificates.
		// Windows maintains 4 stores -- MY, CA, ROOT, SPC.
		if (_Certificado == NULL) {
			_Certificado = CertOpenSystemStoreA(0, "MY");
			if (!_Certificado) {
				printf("**** Error 0x%x returned by CertOpenSystemStore\n", GetLastError());
				return FALSE; // SEC_E_NO_CREDENTIALS;
			}
		}


		// If a user name is specified, then attempt to find a client
		// certificate. Otherwise, just create a NULL credential.
		if (_Login.size() > 0) {
			// Find client certificate. Note that this sample just searches for a
			// certificate that contains the user name somewhere in the subject name.
			// A real application should be a bit less casual.
			pCertContext = CertFindCertificateInStore(	_Certificado,			    // hCertStore
														X509_ASN_ENCODING,			// dwCertEncodingType
														0,							// dwFindFlags
														CERT_FIND_SUBJECT_STR_A,	// dwFindType
														_Login.c_str(),				// *pvFindPara
														NULL);						// pPrevCertContext


			if (pCertContext == NULL)	{
				printf("**** Error 0x%x returned by CertFindCertificateInStore\n", GetLastError());
				if (GetLastError() == CRYPT_E_NOT_FOUND) printf("CRYPT_E_NOT_FOUND - property doesn't exist\n");
				return FALSE; //  SEC_E_NO_CREDENTIALS;
			}
		}


		// Build Schannel credential structure. Currently, this sample only
		// specifies the protocol to be used (and optionally the certificate,
		// of course). Real applications may wish to specify other parameters as well.
		ZeroMemory(&_SchannelCred, sizeof(_SchannelCred));

		_SchannelCred.dwVersion = SCHANNEL_CRED_VERSION;
		if (pCertContext) {
			_SchannelCred.cCreds = 1;
			_SchannelCred.paCred = &pCertContext;
		}


		_SchannelCred.grbitEnabledProtocols = _Protocolo;

		if (_aiKeyExch) rgbSupportedAlgs[cSupportedAlgs++] = _aiKeyExch;

		
		if (cSupportedAlgs)	{
			_SchannelCred.cSupportedAlgs = cSupportedAlgs;
			_SchannelCred.palgSupportedAlgs = rgbSupportedAlgs;
		}

		_SchannelCred.dwFlags |= SCH_CRED_NO_DEFAULT_CREDS;

		// The SCH_CRED_MANUAL_CRED_VALIDATION flag is specified because
		// this sample verifies the server certificate manually.
		// Applications that expect to run on WinNT, Win9x, or WinME
		// should specify this flag and also manually verify the server
		// certificate. Applications running on newer versions of Windows can
		// leave off this flag, in which case the InitializeSecurityContext
		// function will validate the server certificate automatically.
		_SchannelCred.dwFlags |= SCH_CRED_MANUAL_CRED_VALIDATION;

		char PN[] = UNISP_NAME_A;
		// Create an SSPI credential.
		Status = _SFT->AcquireCredentialsHandleA(	NULL,                 // Name of principal    
													PN,					  // Name of package
													SECPKG_CRED_OUTBOUND, // Flags indicating use
													NULL,                 // Pointer to logon ID
													&_SchannelCred,       // Package specific data
													NULL,                 // Pointer to GetKey() func
													NULL,                 // Value to pass to GetKey()
													&_Credenciales,       // (out) Cred Handle
													&tsExpiry);           // (out) Lifetime (optional)

		if (Status != SEC_E_OK) printf("**** Error 0x%x returned by AcquireCredentialsHandle\n", Status);
		else					_CredencialesCreadas = TRUE;

		// cleanup: Free the certificate context. Schannel has already made its own copy.
		if (pCertContext) CertFreeCertificateContext(pCertContext);
		

		return (Status == SEC_E_OK) ? TRUE : FALSE;
	}


	const BOOL DEnviarCorreo::_ObtenerNuevasCredenciales(void) {
		CredHandle                        hCreds;
		SecPkgContext_IssuerListInfoEx    IssuerListInfo;
		PCCERT_CHAIN_CONTEXT              pChainContext;
		CERT_CHAIN_FIND_BY_ISSUER_PARA    FindByIssuerPara;
		PCCERT_CONTEXT                    pCertContext;
		TimeStamp                         tsExpiry;
		SECURITY_STATUS                   Status;


		// Read list of trusted issuers from schannel.
		Status = _SFT->QueryContextAttributesA(&_Contexto, SECPKG_ATTR_ISSUER_LIST_EX, (PVOID)& IssuerListInfo);
		if (Status != SEC_E_OK) {
			printf("Error 0x%x querying issuer list info\n", Status);
			return FALSE; 
		}

		// Enumerate the client certificates.
		ZeroMemory(&FindByIssuerPara, sizeof(FindByIssuerPara));

		FindByIssuerPara.cbSize = sizeof(FindByIssuerPara);
		FindByIssuerPara.pszUsageIdentifier = szOID_PKIX_KP_CLIENT_AUTH;
		FindByIssuerPara.dwKeySpec = 0;
		FindByIssuerPara.cIssuer = IssuerListInfo.cIssuers;
		FindByIssuerPara.rgIssuer = IssuerListInfo.aIssuers;

		pChainContext = NULL;

		while (TRUE) {	  // Find a certificate chain.
			pChainContext = CertFindChainInStore(&_Certificado,
				X509_ASN_ENCODING,
				0,
				CERT_CHAIN_FIND_BY_ISSUER,
				&FindByIssuerPara,
				pChainContext);

			if (pChainContext == NULL) { 
				printf("Error 0x%x finding cert chain\n", GetLastError()); 
				// Salgo del bucle
				break; 
			}

			printf("\ncertificate chain found\n");

			// Get pointer to leaf certificate context.
			pCertContext = pChainContext->rgpChain[0]->rgpElement[0]->pCertContext;

			// Create schannel credential.
			_SchannelCred.dwVersion = SCHANNEL_CRED_VERSION;
			_SchannelCred.cCreds = 1;
			_SchannelCred.paCred = &pCertContext;

			char NP[] = UNISP_NAME_A;
			Status = _SFT->AcquireCredentialsHandleA(	NULL,                   // Name of principal
														NP,						// Name of package
														SECPKG_CRED_OUTBOUND,   // Flags indicating use
														NULL,                   // Pointer to logon ID
														&_SchannelCred,         // Package specific data
														NULL,                   // Pointer to GetKey() func
														NULL,                   // Value to pass to GetKey()
														&hCreds,                // (out) Cred Handle
														&tsExpiry);             // (out) Lifetime (optional)

			if (Status != SEC_E_OK) { 
				printf("**** Error 0x%x returned by AcquireCredentialsHandle\n", Status);
				// Salta lo que queda del while, y vuelve al principio del bucle
				continue; 
			}

			printf("\nnew schannel credential created\n");

			_SFT->FreeCredentialsHandle(&_Credenciales); // Destroy the old credentials.

			_Credenciales = hCreds;

			// Este bucle solo se termina si hay error !?!?! (TLSClient.cpp -> GetNewClientCredentials)
		}

		return TRUE;
	}


	const BOOL DEnviarCorreo::_Conectar(void) {
		// Ya está conectado
		if (_Socket != NULL) return TRUE;

		LPSERVENT	lpServEntry;
		int			iProtocolPort	= _Puerto;
		addrinfo    *result			= NULL;
		addrinfo    *p				= NULL;
		addrinfo     hints;
		sockaddr_in *remote;

		char ip_addr[INET6_ADDRSTRLEN];


		//--------------------------------
		// Setup the hints address info structure
		// which is passed to the getaddrinfo() function
		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = AF_UNSPEC;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;

		//--------------------------------
		// Call getaddrinfo(). If the call succeeds,
		// the result variable will hold a linked list
		// of addrinfo structures containing response
		// information
		DWORD dwRetval = getaddrinfo(_Servidor.c_str(), "smtp", &hints, &result);

		for (p = result; p != NULL; p = p->ai_next) {
			remote = (struct sockaddr_in*)p->ai_addr;

			// Convert IP to string
			inet_ntop(p->ai_family, &remote->sin_addr, ip_addr, sizeof(ip_addr));
			_SockAddr.sin_addr = remote->sin_addr;
		}

		// Create a TCP/IP socket, no specific protocol
		_Socket = socket(PF_INET, SOCK_STREAM, 0);
		if (_Socket == INVALID_SOCKET) {
			//		Debug_Escribir_Varg(L"EnviarDump::Enviar Error creando el socket.\n");
			WSACleanup();
			return FALSE;
		}

		// Get the mail service port
		lpServEntry = getservbyname("mail", 0);

		// Use the SMTP default port if no other port is specified
		if (!lpServEntry)		iProtocolPort = htons(IPPORT_SMTP);
		else					iProtocolPort = lpServEntry->s_port;

		//inet_pton(AF_INET, ip_addr, Bufer);
		// Setup a Socket Address structure
		_SockAddr.sin_family = AF_INET;
		_SockAddr.sin_port = iProtocolPort;

		// Conecto el socket
		if (connect(_Socket, (PSOCKADDR)& _SockAddr, sizeof(_SockAddr))) {
			int err = WSAGetLastError();
			return FALSE;
		}

		return TRUE;
	}


	void DEnviarCorreo::Servidor(const char *nServidor, const UINT nPuerto) {
		_Servidor = nServidor;
		_Puerto   = nPuerto;
	}


	void DEnviarCorreo::_Terminar(void) {
		
		// Si se ha creado el contexto SPPI
		if (_ContextoCreado == TRUE) {
			_SFT->DeleteSecurityContext(&_Contexto);
			_ContextoCreado = FALSE;
		}

		// Si se han creado credenciales SSPI, las elimino
		if (_CredencialesCreadas == TRUE) {
			_SFT->FreeCredentialsHandle(&_Credenciales);
			_CredencialesCreadas = FALSE;
		}

		// Si se ha creado el socket, lo elimino.
		if (_Socket != INVALID_SOCKET) 
			closesocket(_Socket);

		// Termino el subsistema del winsock
		WSACleanup();

		// Si existe el certificado, Cierro el "MY" certificate store.
		if (_Certificado != NULL) {
			CertCloseStore(_Certificado, 0);
			_Certificado = NULL;
		}

	}


	const BOOL DEnviarCorreo::_EnviarHandShake(void) {

		SecBufferDesc   OutBuffer;
		SecBuffer       OutBuffers[1];
		DWORD           dwSSPIFlags, dwSSPIOutFlags, cbData;
		TimeStamp       tsExpiry;
		SECURITY_STATUS scRet;


		dwSSPIFlags =	ISC_REQ_SEQUENCE_DETECT | ISC_REQ_REPLAY_DETECT | ISC_REQ_CONFIDENTIALITY |
						ISC_RET_EXTENDED_ERROR | ISC_REQ_ALLOCATE_MEMORY | ISC_REQ_STREAM;


		//  Initiate a ClientHello message and generate a token.
		OutBuffers[0].pvBuffer = NULL;
		OutBuffers[0].BufferType = SECBUFFER_TOKEN;
		OutBuffers[0].cbBuffer = 0;

		OutBuffer.cBuffers = 1;
		OutBuffer.pBuffers = OutBuffers;
		OutBuffer.ulVersion = SECBUFFER_VERSION;

		char NombreServidor[1024];
		strcpy_s(NombreServidor, _Servidor.c_str());

		scRet = _SFT->InitializeSecurityContextA(&_Credenciales,
			NULL,
			NombreServidor,
			dwSSPIFlags,
			0,
			SECURITY_NATIVE_DREP,
			NULL,
			0,
			&_Contexto,
			&OutBuffer,
			&dwSSPIOutFlags,
			&tsExpiry);

		if (scRet != SEC_I_CONTINUE_NEEDED) { 
			printf("**** Error %d returned by InitializeSecurityContext (1)\n", scRet); return scRet;
		}

		// Send response to server if there is one.
		if (OutBuffers[0].cbBuffer != 0 && OutBuffers[0].pvBuffer != NULL)	{
			cbData = send(_Socket, (char *)OutBuffers[0].pvBuffer, OutBuffers[0].cbBuffer, 0);
			if (cbData == SOCKET_ERROR || cbData == 0)	{
				printf("**** Error %d sending data to server (1)\n", WSAGetLastError());
				_SFT->FreeContextBuffer(OutBuffers[0].pvBuffer);
				_SFT->DeleteSecurityContext(&_Contexto);
				_ContextoCreado = FALSE;
				return FALSE; // SEC_E_INTERNAL_ERROR;
			}
			printf("%d bytes of handshake data sent\n", cbData);
			//if (fVerbose) { PrintHexDump(cbData, OutBuffers[0].pvBuffer); printf("\n"); }
			_SFT->FreeContextBuffer(OutBuffers[0].pvBuffer); // Free output buffer.
			OutBuffers[0].pvBuffer = NULL;
		}


		_BucleHandShake(TRUE);

		return TRUE;
	}

	// BUG
	SECURITY_STATUS DEnviarCorreo::_BucleHandShake(const BOOL LecturaInicial) {
		SecBufferDesc   OutBuffer, InBuffer;
		SecBuffer       InBuffers[2], OutBuffers[1];
		DWORD           dwSSPIFlags, dwSSPIOutFlags, cbData, cbIoBuffer;
		TimeStamp       tsExpiry;
		SECURITY_STATUS scRet;
		PUCHAR          IoBuffer;
		BOOL            fDoRead;
		


		dwSSPIFlags = ISC_REQ_SEQUENCE_DETECT | ISC_REQ_REPLAY_DETECT | ISC_REQ_CONFIDENTIALITY | ISC_RET_EXTENDED_ERROR | ISC_REQ_ALLOCATE_MEMORY | ISC_REQ_STREAM;


		// Allocate data buffer.
		IoBuffer = (PUCHAR)LocalAlloc(LMEM_FIXED, IO_BUFFER_SIZE);
		if (IoBuffer == NULL) { printf("**** Out of memory (1)\n"); return SEC_E_INTERNAL_ERROR; }
		cbIoBuffer = 0;
		fDoRead = LecturaInicial;



		// Loop until the handshake is finished or an error occurs.
		scRet = SEC_I_CONTINUE_NEEDED;

		while (scRet == SEC_I_CONTINUE_NEEDED || scRet == SEC_E_INCOMPLETE_MESSAGE || scRet == SEC_I_INCOMPLETE_CREDENTIALS) {
			if (0 == cbIoBuffer || scRet == SEC_E_INCOMPLETE_MESSAGE) { // Read data from server.			
				if (fDoRead) {
					cbData = recv(_Socket, (char *)IoBuffer + cbIoBuffer, IO_BUFFER_SIZE - cbIoBuffer, 0);
					if (cbData == SOCKET_ERROR)	{
						printf("**** Error %d reading data from server\n", WSAGetLastError());
						scRet = SEC_E_INTERNAL_ERROR;
						break;
					}
					else if (cbData == 0) {
						printf("**** Server unexpectedly disconnected\n");
						scRet = SEC_E_INTERNAL_ERROR;
						break;
					}
					printf("%d bytes of handshake data received\n", cbData);
//					if (fVerbose) { PrintHexDump(cbData, IoBuffer + cbIoBuffer); printf("\n"); }
					cbIoBuffer += cbData;
				}
				else {
					fDoRead = TRUE;
				}
			}



			// Set up the input buffers. Buffer 0 is used to pass in data
			// received from the server. Schannel will consume some or all
			// of this. Leftover data (if any) will be placed in buffer 1 and
			// given a buffer type of SECBUFFER_EXTRA.
			InBuffers[0].pvBuffer = IoBuffer;
			InBuffers[0].cbBuffer = cbIoBuffer;
			InBuffers[0].BufferType = SECBUFFER_TOKEN;

			InBuffers[1].pvBuffer = NULL;
			InBuffers[1].cbBuffer = 0;
			InBuffers[1].BufferType = SECBUFFER_EMPTY;

			InBuffer.cBuffers = 2;
			InBuffer.pBuffers = InBuffers;
			InBuffer.ulVersion = SECBUFFER_VERSION;


			// Set up the output buffers. These are initialized to NULL
			// so as to make it less likely we'll attempt to free random
			// garbage later.
			OutBuffers[0].pvBuffer = NULL;
			OutBuffers[0].BufferType = SECBUFFER_TOKEN;
			OutBuffers[0].cbBuffer = 0;

			OutBuffer.cBuffers = 1;
			OutBuffer.pBuffers = OutBuffers;
			OutBuffer.ulVersion = SECBUFFER_VERSION;


			// Call InitializeSecurityContext.
			scRet = _SFT->InitializeSecurityContextA(&_Credenciales,
				&_Contexto,
				NULL,
				dwSSPIFlags,
				0,
				SECURITY_NATIVE_DREP,
				&InBuffer,
				0,
				NULL,
				&OutBuffer,
				&dwSSPIOutFlags,
				&tsExpiry);

			int a = 0;
			switch (scRet) {
				case SEC_E_INSUFFICIENT_MEMORY:
					a = 1;
					break;
				case SEC_E_INTERNAL_ERROR:
					a = 2;
					break;
				case SEC_E_INVALID_HANDLE:
					a = 3;
					break;
				case SEC_E_INVALID_TOKEN:
					a = 4;
					break;
				case SEC_E_LOGON_DENIED:
					a = 5;
					break;
				case SEC_E_NO_AUTHENTICATING_AUTHORITY:
					a = 6;
					break;
				case SEC_E_NO_CREDENTIALS:
					a = 7;
					break;
				case SEC_E_TARGET_UNKNOWN:
					a = 8;
					break;
				case SEC_E_UNSUPPORTED_FUNCTION:
					a = 9;
					break;
				case SEC_E_WRONG_PRINCIPAL:
					a = 10;
					break;
			}


			// If InitializeSecurityContext was successful (or if the error was
			// one of the special extended ones), send the contends of the output
			// buffer to the server.
			if (scRet == SEC_E_OK || scRet == SEC_I_CONTINUE_NEEDED ||	FAILED(scRet) && (dwSSPIOutFlags & ISC_RET_EXTENDED_ERROR))	{
				if (OutBuffers[0].cbBuffer != 0 && OutBuffers[0].pvBuffer != NULL)	{
					cbData = send(_Socket, (const char *)OutBuffers[0].pvBuffer, OutBuffers[0].cbBuffer, 0);
					if (cbData == SOCKET_ERROR || cbData == 0)	{
						printf("**** Error %d sending data to server (2)\n", WSAGetLastError());
						_ErrorWinsock(WSAGetLastError());
						_SFT->FreeContextBuffer(OutBuffers[0].pvBuffer);
						_SFT->DeleteSecurityContext(&_Contexto);
						_ContextoCreado = FALSE;
						return SEC_E_INTERNAL_ERROR;
					}
					printf("%d bytes of handshake data sent\n", cbData);

					//if (fVerbose) { PrintHexDump(cbData, OutBuffers[0].pvBuffer); printf("\n"); }

					// Free output buffer.
					_SFT->FreeContextBuffer(OutBuffers[0].pvBuffer);
					OutBuffers[0].pvBuffer = NULL;
				}
			}



			// If InitializeSecurityContext returned SEC_E_INCOMPLETE_MESSAGE,
			// then we need to read more data from the server and try again.
			if (scRet == SEC_E_INCOMPLETE_MESSAGE) {
				continue;
			}


			// If InitializeSecurityContext returned SEC_E_OK, then the
			// handshake completed successfully.
			if (scRet == SEC_E_OK) {
				// If the "extra" buffer contains data, this is encrypted application
				// protocol layer stuff. It needs to be saved. The application layer
				// will later decrypt it with DecryptMessage.
				printf("Handshake was successful\n");

				if (InBuffers[1].BufferType == SECBUFFER_EXTRA)	{
					_SeBufferData.pvBuffer = LocalAlloc(LMEM_FIXED, InBuffers[1].cbBuffer);

					if (_SeBufferData.pvBuffer == NULL) {
						printf("**** Out of memory (2)\n"); 
						return SEC_E_INTERNAL_ERROR; 
					}

					MoveMemory(_SeBufferData.pvBuffer, IoBuffer + (cbIoBuffer - InBuffers[1].cbBuffer), InBuffers[1].cbBuffer);

					_SeBufferData.cbBuffer = InBuffers[1].cbBuffer;
					_SeBufferData.BufferType = SECBUFFER_TOKEN;

					printf("%d bytes of app data was bundled with handshake data\n", _SeBufferData.cbBuffer);
				}
				else {
					_SeBufferData.pvBuffer = NULL;
					_SeBufferData.cbBuffer = 0;
					_SeBufferData.BufferType = SECBUFFER_EMPTY;
				}
				break; // Bail out to quit
			}



			// Check for fatal error.
			if (FAILED(scRet)) { 
				printf("**** Error 0x%x returned by InitializeSecurityContext (2)\n", scRet);
				break; 
			}

			// If InitializeSecurityContext returned SEC_I_INCOMPLETE_CREDENTIALS,
			// then the server just requested client authentication.
			if (scRet == SEC_I_INCOMPLETE_CREDENTIALS) {
				// Busted. The server has requested client authentication and
				// the credential we supplied didn't contain a client certificate.
				// This function will read the list of trusted certificate
				// authorities ("issuers") that was received from the server
				// and attempt to find a suitable client certificate that
				// was issued by one of these. If this function is successful,
				// then we will connect using the new certificate. Otherwise,
				// we will attempt to connect anonymously (using our current credentials).
				_ObtenerNuevasCredenciales();

				// Go around again.
				fDoRead = FALSE;
				scRet = SEC_I_CONTINUE_NEEDED;
				continue;
			}

			// Copy any leftover data from the "extra" buffer, and go around again.
			if (InBuffers[1].BufferType == SECBUFFER_EXTRA)	{
				MoveMemory(IoBuffer, IoBuffer + (cbIoBuffer - InBuffers[1].cbBuffer), InBuffers[1].cbBuffer);
				cbIoBuffer = InBuffers[1].cbBuffer;
			}
			else {
				cbIoBuffer = 0;
			}
		}

		// Delete the security context in the case of a fatal error.
		if (FAILED(scRet)) {
			_SFT->DeleteSecurityContext(&_Contexto);
		}
		LocalFree(IoBuffer);

		return scRet;
	}


	/*
	const BOOL DEnviarCorreo::_Enviar(const char* eTxt, const BOOL nRecibir) {
		std::string		Texto = eTxt; // +std::string("\r\n");
#ifdef RAVE_MOSTRAR_CONSOLA
		size_t			out;
		static wchar_t	WBuffer[4096];
		ZeroMemory(WBuffer, sizeof(wchar_t) * 4096);
		mbstowcs_s(&out, WBuffer, 4096, Texto.c_str(), Texto.size());
		Debug_Escribir_Varg(L"EnviarDump::_Enviar %s\n", WBuffer);
#endif
		Texto += "\r\n";
		int iStatus = send(_Socket, Texto.c_str(), static_cast<int>(Texto.size()), 0);
		if ((iStatus != SOCKET_ERROR) && (iStatus)) {
			if (nRecibir == TRUE)	return _Recibir();
			else					return FALSE;
		}
		
		_Error = TRUE;

		return TRUE;
	}

	const BOOL DEnviarCorreo::_Recibir(void) {
		static char Buffer[4096];
		ZeroMemory(Buffer, sizeof(char) * 4096);
		int iStatus = recv(_Socket, Buffer, 4096, 0);
#ifdef RAVE_MOSTRAR_CONSOLA
		std::string		Texto = Buffer;
		size_t			out;
		static wchar_t	WBuffer[4096];
		ZeroMemory(WBuffer, sizeof(wchar_t) * 4096);
		mbstowcs_s(&out, WBuffer, 4096, Texto.c_str(), Texto.size());
		Debug_Escribir_Varg(L"EnviarDump::_Recibir %s\n", WBuffer);
#endif
		if ((iStatus != SOCKET_ERROR) && (iStatus))	return FALSE;
		
		_Error = TRUE;

		return TRUE;
	}
	*/


	const BOOL DEnviarCorreo::_Encriptar(PBYTE pbIoBuffer, SecPkgContext_StreamSizes Sizes) {
		// http://msdn.microsoft.com/en-us/library/aa375378(VS.85).aspx
		
		SECURITY_STATUS    scRet;				
		SecBufferDesc      Message;				
		SecBuffer          Buffers[4];			
		DWORD              cbMessage, cbData;
		PBYTE              pbMessage;


		pbMessage = pbIoBuffer + Sizes.cbHeader; // Offset by "header size"
		cbMessage = (DWORD)strlen((char *)pbMessage);
		printf("Sending %d bytes of plaintext:", cbMessage); 
//		PrintText(cbMessage, pbMessage);
		//if (fVerbose) { PrintHexDump(cbMessage, pbMessage); printf("\n"); }


		// Encrypt the HTTP request.
		Buffers[0].pvBuffer = pbIoBuffer;                   // Pointer to buffer 1
		Buffers[0].cbBuffer = Sizes.cbHeader;               // length of header
		Buffers[0].BufferType = SECBUFFER_STREAM_HEADER;    // Type of the buffer

		Buffers[1].pvBuffer = pbMessage;                    // Pointer to buffer 2
		Buffers[1].cbBuffer = cbMessage;                    // length of the message
		Buffers[1].BufferType = SECBUFFER_DATA;             // Type of the buffer

		Buffers[2].pvBuffer = pbMessage + cbMessage;        // Pointer to buffer 3
		Buffers[2].cbBuffer = Sizes.cbTrailer;              // length of the trailor
		Buffers[2].BufferType = SECBUFFER_STREAM_TRAILER;   // Type of the buffer

		Buffers[3].pvBuffer = SECBUFFER_EMPTY;              // Pointer to buffer 4
		Buffers[3].cbBuffer = SECBUFFER_EMPTY;              // length of buffer 4
		Buffers[3].BufferType = SECBUFFER_EMPTY;            // Type of the buffer 4


		Message.ulVersion = SECBUFFER_VERSION;				// Version number
		Message.cBuffers = 4;                               // Number of buffers - must contain four SecBuffer structures.
		Message.pBuffers = Buffers;							// Pointer to array of buffers

		scRet = _SFT->EncryptMessage(&_Contexto, 0, &Message, 0); // must contain four SecBuffer structures.
		if (FAILED(scRet)) { 
			printf("**** Error 0x%x returned by EncryptMessage\n", scRet); 
			return FALSE; //scRet;
		}

		// Send the encrypted data to the server.                                            len                                                                         flags
		cbData = send(_Socket, (char *)pbIoBuffer, Buffers[0].cbBuffer + Buffers[1].cbBuffer + Buffers[2].cbBuffer, 0);

		printf("%d bytes of encrypted data sent\n", cbData);
		//if (fVerbose) { PrintHexDump(cbData, pbIoBuffer); printf("\n"); }


		return TRUE;


	}

	const BOOL DEnviarCorreo::_Desencriptar(PBYTE pbIoBuffer, DWORD cbIoBufferLength) {


		// calls recv() - blocking socket read
		// http://msdn.microsoft.com/en-us/library/ms740121(VS.85).aspx

		// The encrypted message is decrypted in place, overwriting the original contents of its buffer.
		// http://msdn.microsoft.com/en-us/library/aa375211(VS.85).aspx

		SecBuffer                ExtraBuffer;
		SecBuffer* pDataBuffer, *pExtraBuffer;

		SECURITY_STATUS    scRet;            // unsigned long cbBuffer;    // Size of the buffer, in bytes
		SecBufferDesc        Message;        // unsigned long BufferType;  // Type of the buffer (below)
		SecBuffer                Buffers[4];    // void    SEC_FAR * pvBuffer;   // Pointer to the buffer

		DWORD                        cbIoBuffer, cbData, length;
		PBYTE                        buff;
		int i;

		ExtraBuffer.cbBuffer   = 0;
		ExtraBuffer.BufferType = 0;
		ExtraBuffer.pvBuffer   = 0;

		// Read data from server until done.
		cbIoBuffer = 0;
		scRet = 0;
		while (TRUE) { // Read some data.
			if (cbIoBuffer == 0 || scRet == SEC_E_INCOMPLETE_MESSAGE) { // get the data		
				cbData = recv(_Socket, (char *)pbIoBuffer + cbIoBuffer, cbIoBufferLength - cbIoBuffer, 0);
				if (cbData == SOCKET_ERROR)	{
					printf("**** Error %d reading data from server\n", WSAGetLastError());
					scRet = SEC_E_INTERNAL_ERROR;
					break;
				}
				else if (cbData == 0) {// Server disconnected.
					if (cbIoBuffer)	{
						printf("**** Server unexpectedly disconnected\n");
						scRet = SEC_E_INTERNAL_ERROR;
						return scRet;
					}
					else
						break; // All Done
				}
				else {// success				
					printf("%d bytes of (encrypted) application data received\n", cbData);
//					if (fVerbose) { PrintHexDump(cbData, pbIoBuffer + cbIoBuffer); printf("\n"); }
					cbIoBuffer += cbData;
				}
			}


			// Decrypt the received data.
			Buffers[0].pvBuffer = pbIoBuffer;
			Buffers[0].cbBuffer = cbIoBuffer;
			Buffers[0].BufferType = SECBUFFER_DATA;  // Initial Type of the buffer 1
			Buffers[1].BufferType = SECBUFFER_EMPTY; // Initial Type of the buffer 2
			Buffers[2].BufferType = SECBUFFER_EMPTY; // Initial Type of the buffer 3
			Buffers[3].BufferType = SECBUFFER_EMPTY; // Initial Type of the buffer 4

			Message.ulVersion = SECBUFFER_VERSION;    // Version number
			Message.cBuffers = 4;                                    // Number of buffers - must contain four SecBuffer structures.
			Message.pBuffers = Buffers;                        // Pointer to array of buffers
			scRet = _SFT->DecryptMessage(&_Contexto, &Message, 0, NULL);
			if (scRet == SEC_I_CONTEXT_EXPIRED) break; // Server signalled end of session
	//      if( scRet == SEC_E_INCOMPLETE_MESSAGE - Input buffer has partial encrypted record, read more
			if (scRet != SEC_E_OK && scRet != SEC_I_RENEGOTIATE &&	scRet != SEC_I_CONTEXT_EXPIRED)	{
				printf("**** DecryptMessage ");
//				DisplaySECError((DWORD)scRet);
				return scRet;
			}

			// Locate data and (optional) extra buffers.
			pDataBuffer = NULL;
			pExtraBuffer = NULL;
			for (i = 1; i < 4; i++)	{
				if (pDataBuffer == NULL && Buffers[i].BufferType == SECBUFFER_DATA) pDataBuffer = &Buffers[i];
				if (pExtraBuffer == NULL && Buffers[i].BufferType == SECBUFFER_EXTRA) pExtraBuffer = &Buffers[i];
			}


			// Display the decrypted data.
			if (pDataBuffer) {
				length = pDataBuffer->cbBuffer;
				if (length) {// check if last two chars are CR LF				
					buff = (PBYTE)pDataBuffer->pvBuffer; // printf( "n-2= %d, n-1= %d \n", buff[length-2], buff[length-1] );
					printf("Decrypted data: %d bytes", length);
					//PrintText(length, buff);
					//if (fVerbose) { PrintHexDump(length, buff); printf("\n"); }
					if (buff[length - 2] == 13 && buff[length - 1] == 10) break; // printf("Found CRLF\n");
				}
			}



			// Move any "extra" data to the input buffer.
			if (pExtraBuffer) {
				MoveMemory(pbIoBuffer, pExtraBuffer->pvBuffer, pExtraBuffer->cbBuffer);
				cbIoBuffer = pExtraBuffer->cbBuffer; // printf("cbIoBuffer= %d  \n", cbIoBuffer);
			}
			else
				cbIoBuffer = 0;


			// The server wants to perform another handshake sequence.
			if (scRet == SEC_I_RENEGOTIATE)	{
				printf("Server requested renegotiate!\n");

				scRet = _BucleHandShake(FALSE);
				if (scRet != SEC_E_OK) return scRet;

				if (ExtraBuffer.pvBuffer) { // Move any "extra" data to the input buffer.				
					MoveMemory(pbIoBuffer, ExtraBuffer.pvBuffer, ExtraBuffer.cbBuffer);
					cbIoBuffer = ExtraBuffer.cbBuffer;
				}
			}
		} // Loop till CRLF is found at the end of the data

		return 0;
	}

	
}