/**
 * Define una clase factory para crear descendientes de "AuthSMTP".
 *
 * @author JM
 *
 * @date 04/11/2003 - JM  @li Versión inicial
**/
#ifndef AuthSMTPFactoryH
#define AuthSMTPFactoryH

#include "../../LibRed/h/RedCliente.h"

#include "AuthSMTP.h"

/**
 * Define una clase factory para crear descendientes de "AuthSMTP".
 * A partir de la respuesta de bienvenida de un servido SMTP, se construye el objeto de autenticación correspondiente.
 * Por ahora se soportan 2 modos de autenticación: AuthLogin y AuthPlain
 * La función "crearAutoSMTP" retorna NULL si el servidor no soporta ninguno de estos
 * modos, o si el servidor no requiere autenticación.
 *
 * @author JM
 *
 * @date 04/11/2003 - JM  @li Versión inicial
**/
class AuthSMTPFactory
{
public:
	static AuthSMTP* crearAuthSMTP(RedCliente *socket, LPCSTR welcome);
};


#endif
