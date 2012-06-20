/**
 * Se define la clase ServidorEnvio, que sirve como base para implementar cualquier tipo
 * de servidor (f�sico o l�gico) a trav�s del cual se puede enviar un correo electr�nico.
 *
 * @author JM
 *
 * @date 16/10/2003 - JM  @li Versi�n inicial
 * @date 27/10/2003 - JM  @li A�adido constructor a partir de la configuraci�n y sistema de auto-conexi�n.
 * @date 31/10/2003 - JM  @li Eliminadas todas las referencias a objetos configuraci�n
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
