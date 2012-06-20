/**
 * Se define un descendiente de "AuthSMTP" para implementar el sistema de autenticaci�n basado en "plain".
 *
 * @author JM
 *
 * @date 04/11/2003 - JM  @li Versi�n inicial
**/
#ifndef AuthPlainH
#define AuthPlainH

#include "../h/AuthSMTP.h"



class AuthPlain : public AuthSMTP
{
public:
	AuthPlain(RedCliente *socket);
    virtual ~AuthPlain();

    virtual int doLogin(const ArsCadena &user, const ArsCadena &pass);

    static LPCSTR getIdentificador();
};

#endif


 