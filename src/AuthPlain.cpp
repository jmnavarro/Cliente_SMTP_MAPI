#include "../h/AuthPlain.h"

#include "../h/Base64.h"


#define PLAIN_OK_AUTH	200


AuthPlain::AuthPlain(RedCliente *socket)
	: AuthSMTP(socket)
{
}


AuthPlain::~AuthPlain()
{
}


int AuthPlain::doLogin(const ArsCadena &user, const ArsCadena &pass)
{
    int   		  len = 1;
    int 		  &ret = len;
    char  		  buff[512] = "";
    BufferSalida  codificado;
    CBase64		  base64;
    ArsCadena	  comando("AUTH PLAIN ");

    //
    // Hay que componer una cadena de login así:
    //  AUTH PLAIN \0user\0password\0
    //
    strcpy(buff + len, user.cadena());
    len += user.longitud() + 1;

    strcpy(buff + len, pass.cadena());
    len += pass.longitud() + 1;

    codificado = base64.Encode(buff, len);

    comando += codificado.c_str();
	comando += "\r\n";

    //
	// enviar comando de login
    //
	if (!transmitir(comando.cadena(), comando.longitud()+1, PLAIN_OK_AUTH))
        ret = ERR_AUTH;
    else
    	ret = ERR_AUTH_OK;

    return ret;
}


LPCSTR AuthPlain::getIdentificador()
{
	return "PLAIN";
}

