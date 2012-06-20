#include "../h/AuthLogin.h"
#include "../h/Base64.h"


#define LOGIN_OK_AUTH	   334
#define LOGIN_OK_USER	   334
#define LOGIN_OK_PASSWORD  235


AuthLogin::AuthLogin(RedCliente *socket)
	: AuthSMTP(socket)
{
}


AuthLogin::~AuthLogin()
{
}

int AuthLogin::doLogin(const ArsCadena &user, const ArsCadena &pass)
{
	ArsCadena		aux("AUTH LOGIN\r\n");
    CBase64			base64;
    BufferSalida	codificado;

	//
	// enviar comando de login
    //
	if (!transmitir(aux.cadena(), aux.longitud()+1, LOGIN_OK_AUTH))
        return ERR_AUTH;

	//
  	// enviar usuario en base64
    //
	codificado = base64.Encode(user.cadena(), user.longitud());

    aux = codificado.c_str();
    aux += "\r\n";
	if (!transmitir(aux.cadena(), aux.longitud()+1, LOGIN_OK_USER))
        return ERR_AUTH_USER;

	//
    // enviar contraseña en base64
    //
	codificado = base64.Encode(pass.cadena(), pass.longitud());

    aux = codificado.c_str();
    aux += "\r\n";
	if (!transmitir(aux.cadena(), aux.longitud()+1, LOGIN_OK_PASSWORD))
        return ERR_AUTH_PASSWORD;

    return ERR_AUTH_OK;
}


LPCSTR AuthLogin::getIdentificador()
{
	return "LOGIN";
}
