/**
 * Se define un descendiente de "AuthSMTP" para implementar el sistema de autenticaci�n basado en login.
 *
 * @author JM
 *
 * @date 04/11/2003 - JM  @li Versi�n inicial
**/
#ifndef AuthLoginH
#define AuthLoginH

#include "../h/AuthSMTP.h"



class AuthLogin : public AuthSMTP
{
public:
	AuthLogin(RedCliente *socket);
    virtual ~AuthLogin();

    virtual int doLogin(const ArsCadena &user, const ArsCadena &pass);

    static LPCSTR getIdentificador();
};

#endif

