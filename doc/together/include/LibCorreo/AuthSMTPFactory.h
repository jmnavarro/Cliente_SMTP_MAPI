/* Generated by Together */

#ifndef AUTHSMTPFACTORY_H
#define AUTHSMTPFACTORY_H
#include "AuthSMTP.h"
#include "AuthLogin.h"
#include "AuthPlain.h"
class AuthSMTPFactory {
public:

    static void crearAuthSMTP();

private:    

    /** @link dependency */
    /*# AuthSMTP lnkAuthSMTP; */

    /** @link dependency */
    /*# AuthLogin lnkAuthLogin; */

    /** @link dependency */
    /*# AuthPlain lnkAuthPlain; */
};
#endif //AUTHSMTPFACTORY_H