/**
 * Se define la clase ServidorSMTP, descendiente de ServidorEnvio
 *
 * @author: JM
 *
 * @date 16/10/2003  Versión inicial
**/
#ifndef ServidorSMTPH
#define ServidorSMTPH

#include "../../LibUtils/h/ArsCadena.h"
#include "../../LibRed/h/RedCliente.h"

#include "ServidorEnvio.h"
#include "Correo.h"
#include "AuthSMTP.h"


//
// códigos de retorno de error del método "enviar"
//
#define  SMTPERR_OK			 	ENVIO_OK
#define  SMTPERR_SINDESTINOS	ENVIO_SINDESTINOS

#define  SMTPERR_CONECTAR		-1
#define  SMTPERR_HELO			-2
#define  SMTPERR_MAIL			-3
#define  SMTPERR_RCPT_NORMAL	-4
#define  SMTPERR_RCPT_COPIA		-5
#define  SMTPERR_RCPT_OCULTO	-6
#define  SMTPERR_DATA			-7
#define  SMTPERR_CABECERA		-8
#define  SMTPERR_CUERPO			-9
#define  SMTPERR_FIN			-10
#define  SMTPERR_QUIT			-11
#define  SMTPERR_DESCONECTAR	-12
#define  SMTPERR_LOGIN			-13
#define  SMTPERR_AUTH			-14
#define  SMTPERR_ADJUNTOS		-15

#define YA_CONECTADO	0


/**
 * Se define la clase ServidorSMTP, descendiente de ServidorEnvio, que implementa
 * las operaciones propias para el envío de correos a través del protocolo SMTP.
 *
 * @author: JM
 *    @date 16/10/2003  Versión inicial
 *    @date 27/10/2003  Añadido constructor a partir de la configuración, y sistema de auto-conexión.
 *    @date 31/10/2003  Eliminadas todas las referencias a objetos de configuración y añadidos los atributos "host", "port", "user" y "password".
 *    @date 03/11/2003  Implementado el sistema de login propio del protocolo SMTP.
 *    @date 06/11/2003  Se comprueba la lectura de archivos adjuntos, por si no existen los archivos en disco.
 *    @date 03/12/2003  Corregido bug en "getFechaActual" cuando se utilizaba el mes 12
 *    @date 14/01/2004  Se descomponen los métodos "enviar", "getCabecera" y "getCuerpo" en varios métodos más pequeños y sencillos.
 *    @date 19/01/2004  Llamadas al método "getCadenaCorreo()" para formatear una dirección de correo con el formato "nombre <dirección>"
 *    @date 13/07/2004  Refactorizar el método "enviar", para dividirlo en las siguientes operaciones lógicas: comprobaciones, autoconexión, lectura de adjuntos, conversación SMTP y desconexión.
 *
**/
class ServidorSMTP : public ServidorEnvio
{
public:
	ServidorSMTP();
	virtual ~ServidorSMTP();

	virtual int enviar(Correo &correo);

	virtual int conectar();
	virtual bool desconectar();


	// setters y getters
	ArsCadena getHost() const			 	 { return host; }
	void setHost(const ArsCadena &value) 	 { host = value; }

	unsigned short getPort() const			 { return port; }
	void setPort(const unsigned short value) { port = value; }

	ArsCadena getUser() const 			  	 { return user; }
	void setUser(const ArsCadena &value)	 { user = value; }

	ArsCadena getPassword() const 			 { return password; }
	void setPassword(const ArsCadena &value) { password = value; }

	/// retorna la fecha actual en el formato propio del protocolo SMTP
	static ArsCadena getFechaActual();

protected:
	/// comandos del protocolo SMTP (en orden)
	int procesarConversacionSMTP(Correo &correo);

	int  enviarSaludo(Correo &correo);
	int  enviarAutenticacion();
	bool enviarOrigen(Correo &correo);
	bool enviarDestinos(Correo &correo);
	bool enviarDestinosCopia(Correo &correo);
	bool enviarDestinosOcultos(Correo &correo);
	bool enviarData();
	bool enviarCabecera(Correo &correo);
	bool enviarCuerpo(const Correo &correo);
	bool enviarFin();

	/// gestión del envío y recepción por el socket
	virtual bool enviarDatos(LPCSTR datos, DWORD size);
	virtual bool recibirDatos(ArsCadena &datos);
	virtual bool transmitir(ArsCadena &datos, int code = -1);

	/// gestión de errores
	virtual int  getReturnCode();
	virtual bool error(bool desconectar);

	/// construcción del contenido de la cabecera
	ArsCadena  getCabecera(Correo &correo);

	ArsCadena getCabeceraOrigen(Correo &correo);
	ArsCadena getCabeceraDestinoNormal(const Correo &correo);
	ArsCadena getCabeceraDestinoCopia(const Correo &correo);
	ArsCadena getCabeceraAsunto(Correo &correo);
	ArsCadena getCabeceraFecha(const Correo &correo);
	ArsCadena getCabeceraAdjuntos(const Correo &correo);
	ArsCadena getCabeceraPrioridad(const Correo &correo);
	ArsCadena getCabeceraCliente();
	ArsCadena getCabeceraSeparadorMIME(const Correo &correo);

	/// construcción del contenido del cuerpo
	TextoMensaje getCuerpo(const Correo &correo);
	TextoMensaje getCuerpoAdjunto(ArchivoAdjunto &adjunto, bool primerAdjunto);

private:
	ArsCadena	   host;
	unsigned short port;
	ArsCadena	   user;
	ArsCadena	   password;

	AuthSMTP	  *autenticacion;

	RedCliente	*socket;
};


#endif
