#include "../h/AuthSMTP.h"



AuthSMTP::AuthSMTP(RedCliente *socket)
{
    this->socket   = socket;
}


AuthSMTP::~AuthSMTP()
{
}


LPCSTR AuthSMTP::getIdentificador()
{
	return "";
}


bool AuthSMTP::transmitir(LPSTR datos, DWORD lenDatos, int code /*= -1*/)
{
	bool ret = socket->enviar(datos, lenDatos - 1);

    if (ret)
		ret = (code != -1 && getReturnCode() == code);

 	return ret;
}


int AuthSMTP::getReturnCode()
{
	const int BUFFER_RESPUESTA_SIZE = 128;

    char buff[BUFFER_RESPUESTA_SIZE];
    int size = BUFFER_RESPUESTA_SIZE;

	while (size >= BUFFER_RESPUESTA_SIZE)
	    if (!socket->recibir(buff, size))
	    	return -1;

	buff[3] = '\0';

    return atoi(buff);
}
