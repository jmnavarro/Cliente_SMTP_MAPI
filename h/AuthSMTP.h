/**
 * Define una clase base para la implementación de distintos sistema de autenticación.
 *
 * @author JM
 *
 * @date 04/11/2003 - JM  @li Versión inicial
**/
#ifndef AuthSMTPH
#define AuthSMTPH

#include "../../LibUtils/h/ArsCadena.h"
#include "../../LibRed/h/RedCliente.h"


//
// códigos de error en autenticación
//
#define ERR_AUTH_OK		 	0
#define ERR_AUTH			-1
#define ERR_AUTH_USER		-2
#define ERR_AUTH_PASSWORD	-3


class AuthSMTP
{
public:
	AuthSMTP(RedCliente *socket);
	virtual ~AuthSMTP();

	virtual int doLogin(const ArsCadena &user, const ArsCadena &pass) = 0;

	static LPCSTR getIdentificador();

protected:
	RedCliente	*socket;

	//
	// CÓDIGO DUPLICADO: debería crearse la clase base "SocketSMTP" de la que desciendan
	// las clases AuthSMTP y ServidorSMTP.
	//
	bool transmitir(LPSTR datos, DWORD lenDatos, int code = -1);
	int  getReturnCode();
};

#endif
