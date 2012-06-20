#include "../h/AuthSMTPFactory.h"

#include "../h/AuthLogin.h"
#include "../h/AuthPlain.h"


AuthSMTP* AuthSMTPFactory::crearAuthSMTP(RedCliente *socket, LPCSTR welcome)
{
    const char *auth;
	AuthSMTP   *ret;

	if ( NULL != (auth = strstr(welcome, "AUTH ")) )
    {
    	if ( strstr(auth, AuthLogin::getIdentificador()) )
        	ret = new AuthLogin(socket);
        else if ( strstr(auth, AuthPlain::getIdentificador()) )
            ret = new AuthPlain(socket);
        else
        	ret = NULL;
    }
	else
    	ret = NULL;

    return (ret);
}

