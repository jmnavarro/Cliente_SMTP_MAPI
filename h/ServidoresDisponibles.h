#ifndef ServidoresDisponibles_H
#define ServidoresDisponibles_H


#include "../h/ServidorEnvio.h"


class ArsConfiguracion;


#define CLAVE_SMTP_SERVIDOR	"servidor"
#define CLAVE_SMTP_PUERTO	"puerto"
#define CLAVE_SMTP_USUARIO	"usuario"
#define CLAVE_SMTP_CLAVE	"clave"

#define CLAVE_MAPI_PERFIL	"perfil"
#define CLAVE_MAPI_CLAVE	"clave"


/**
 * 	Se implementa una clase de tipo <i>Factory</i> para la creaci�n de un servidor concreto,
 *	a partir de los datos contenidos en un objeto Configuracion.
 *	Esta clase se usa de forma est�tica (sin crear instancias) a trav�s del m�todo CrearServidor.
 *
 *	@author JM  -  31/10/2003  -   Versi�n inicial
 *	@author JM	-  29/01/2004  -   Se cambia para utilizar objetos descendientes de Configuracion (LibConfiguracion) en vez de ArsConfiguracionComun. Para permitir que las claves de configuraci�n est�n anidadas dentro de la configuraci�n de un m�dulo, se debe pasar un objeto de tipo "ConfiguracionModulo"
 *	@author JM  -  26/05/2004  -   Se refactoriza el c�digo de CrearServidor, a�adiendo los m�todos CrearServidorSMTP y CrearServidorMAPI
**/
class ServidoresDisponibles
{
public:
	static ServidorEnvio* CrearServidor(ArsConfiguracion *cfg);

private:
	static ServidorEnvio* CrearServidorSMTP(ArsConfiguracion *cfg);
	static ServidorEnvio* CrearServidorMAPI(ArsConfiguracion *cfg);
};


#endif
