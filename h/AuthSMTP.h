/**
 * Define una clase base para la implementaci�n de distintos sistema de autenticaci�n.
 *
 * @author JM
 *
 * @date 04/11/2003 - JM  @li Versi�n inicial
**/
#ifndef AuthSMTPH
#define AuthSMTPH

#include "../../LibUtils/h/ArsCadena.h"
#include "../../LibRed/h/RedCliente.h"


//
// c�digos de error en autenticaci�n
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
	// C�DIGO DUPLICADO: deber�a crearse la clase base "SocketSMTP" de la que desciendan
	// las clases AuthSMTP y ServidorSMTP.
	//
	bool transmitir(LPSTR datos, DWORD lenDatos, int code = -1);
	int  getReturnCode();
};

#endif
