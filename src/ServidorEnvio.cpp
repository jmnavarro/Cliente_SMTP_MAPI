#include "../h/ServidorEnvio.h"

//--------------------------------------------------------------------------------------

ServidorEnvio::ServidorEnvio()
{
    conectado = false;
}


ServidorEnvio::~ServidorEnvio()
{
}


int ServidorEnvio::enviar(Correo &correo)
{
	if ( !isConectado() )
    	conectar();

    return ENVIO_NO_IMPL;
}


int ServidorEnvio::conectar()
{
	conectado = true;
	return CONECTAR_NO_IMPL;
};


bool ServidorEnvio::desconectar()
{
	conectado = false;
	return false;
};

