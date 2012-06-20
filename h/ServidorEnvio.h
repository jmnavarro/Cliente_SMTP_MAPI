/**
 * Se define la clase ServidorEnvio, que sirve como base para implementar cualquier tipo
 * de servidor (físico o lógico) a través del cual se puede enviar un correo electrónico.
 *
 * @author JM
 *
 * @date 16/10/2003 - JM  @li Versión inicial
 * @date 27/10/2003 - JM  @li Añadido constructor a partir de la configuración y sistema de auto-conexión.
 * @date 31/10/2003 - JM  @li Eliminadas todas las referencias a objetos configuración
**/
#ifndef ServidorEnvioH
#define ServidorEnvioH

#include "Correo.h"


#define ENVIO_OK	  		0
#define ENVIO_NO_IMPL 		1
#define ENVIO_SINDESTINOS	2

#define CONECTAR_OK	  		0
#define CONECTAR_ERROR 		-1
#define CONECTAR_NO_IMPL 	1



class ServidorEnvio
{
public:
	ServidorEnvio();
	virtual ~ServidorEnvio();

	virtual int enviar(Correo &correo);

	virtual int  conectar();
	virtual bool desconectar();
	virtual bool isConectado()	{ return conectado; }

protected:
	bool conectado;
};


#endif
