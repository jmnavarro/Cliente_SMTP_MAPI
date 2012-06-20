//--------------------------------------------------------------------------------------
#include "../h/ServidoresDisponibles.h"
#include "../../LibUtils/h/ArsCadena.h"
#include "../h/ServidorSMTP.h"
#include "../h/ServidorMAPI.h"
#include "../../LibArsConfiguracion/h/ArsConfiguracion.h"
//--------------------------------------------------------------------------------------

ServidorEnvio* ServidoresDisponibles::CrearServidor(ArsConfiguracion *cfg)
{
	ArsCadena		servidor, perfil;
	ServidorEnvio	*ret;

	servidor = cfg->GetValor(CLAVE_SMTP_SERVIDOR);
	perfil   = cfg->GetValor(CLAVE_MAPI_PERFIL);

	if ( servidor.longitud() > 0 )
	{
		// es SMTP
		ret = CrearServidorSMTP(cfg);
	}
	else if ( perfil.longitud() > 0 )
	{
		// es MAPI
		ret = CrearServidorMAPI(cfg);
	}
	else
		ret = NULL;

	return ret;
}


ServidorEnvio*	ServidoresDisponibles::CrearServidorSMTP(ArsConfiguracion *cfg)
{
	ArsCadena  servidor, puerto;
	ArsCadena usuario, clave;
	ServidorSMTP *ret = new ServidorSMTP();

	servidor = cfg->GetValor(CLAVE_SMTP_SERVIDOR);
	ret->setHost(servidor);

	puerto = cfg->GetValor(CLAVE_SMTP_PUERTO);
	if ( puerto.longitud() > 0 )
		ret->setPort( atoi(puerto.cadena()) );

	usuario = cfg->GetValor(CLAVE_SMTP_USUARIO);
	if ( usuario.longitud() > 0 )
		ret->setUser(usuario);

	clave = cfg->GetValor(CLAVE_SMTP_CLAVE);
	if ( clave.longitud() > 0 )
		ret->setPassword(clave);

	return (ret);
}


ServidorEnvio*	ServidoresDisponibles::CrearServidorMAPI(ArsConfiguracion *cfg)
{
	ArsCadena    perfil, clave;
	ServidorMAPI *ret = new ServidorMAPI();

	perfil = cfg->GetValor(CLAVE_MAPI_PERFIL);
	if (perfil == "NULL")	// perfil por defecto
		perfil = "";

	ret->setProfile(perfil);

	clave = cfg->GetValor(CLAVE_MAPI_CLAVE);
	if ( clave.longitud() > 0 )
		ret->setPassword(clave);

	return (ret);
}

