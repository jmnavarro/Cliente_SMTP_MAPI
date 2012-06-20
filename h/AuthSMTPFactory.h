/**
 * Define una clase factory para crear descendientes de "AuthSMTP".
 *
 * @author JM
 *
 * @date 04/11/2003 - JM  @li Versi�n inicial
**/
#ifndef AuthSMTPFactoryH
#define AuthSMTPFactoryH

#include "../../LibRed/h/RedCliente.h"

#include "AuthSMTP.h"

/**
 * Define una clase factory para crear descendientes de "AuthSMTP".
 * A partir de la respuesta de bienvenida de un servido SMTP, se construye el objeto de autenticaci�n correspondiente.
 * Por ahora se soportan 2 modos de autenticaci�n: AuthLogin y AuthPlain
 * La funci�n "crearAutoSMTP" retorna NULL si el servidor no soporta ninguno de estos
 * modos, o si el servidor no requiere autenticaci�n.
 *
 * @author JM
 *
 * @date 04/11/2003 - JM  @li Versi�n inicial
**/
class AuthSMTPFactory
{
public:
	static AuthSMTP* crearAuthSMTP(RedCliente *socket, LPCSTR welcome);
};


#endif
