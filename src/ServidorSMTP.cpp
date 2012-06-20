//--------------------------------------------------------------------------------------

#include "../h/ServidorSMTP.h"

#include "..\..\..\Base\LibUtilidades\h\Log.h"
#include "../h/AuthSMTPFactory.h"

//--------------------------------------------------------------------------------------

//
// cabecera temporal
//
#define  SEPARADOR_MIME  "----=_NextPart_999_002C_01C39803.0C3A0A40"


//
// códigos esperados para cada uno de los comandos SMTP
//
#define CODIGO_OK_WELCOME	220
#define CODIGO_OK_HELO		250
#define CODIGO_OK_MAIL		250
#define CODIGO_OK_RCPT		250
#define CODIGO_OK_DATA		354
#define CODIGO_OK_FINDATA	250
#define CODIGO_OK_QUIT		221
#define CODIGO_OK_RSET		220

#define DEFAULT_SMTP_PORT	25

// retorno de "conectar()"
#define CONECTAR_YA_CONECTADO  2

//--------------------------------------------------------------------------------------

ServidorSMTP::ServidorSMTP()
	: ServidorEnvio()
{
	port = DEFAULT_SMTP_PORT;
	autenticacion = NULL;
	socket = NULL;
}


ServidorSMTP::~ServidorSMTP()
{
	//
	// esto debería codificarse en la clase padre, pero no podemos debido a la
	// siguiente limitación:
	// cuando se llama a un método polimórfico desde el constructor o destructor, no se
	// ejecuta la versión correcta del método, sino la de la clase donde estemos.
	// Es decir: si ponemos la llamada a "desconectar" en el destructor de la clase
	// ServidorEnvio, no lograremos ejecutar la desconexión de SMTP, aún cuando el
	// objeto que se esté liberando sea de tipo ServidorSMTP.
	//
	desconectar();
}


int ServidorSMTP::enviar(Correo &correo)
{
	int code;
	bool desconecta;

	// comprobación de destinatarios
	//
	if (correo.getTotalDestinos()        == 0 &&
		correo.getTotalDestinosCopias()  == 0 &&
		correo.getTotalDestinosOcultos() == 0)
	{
		return SMTPERR_SINDESTINOS;
	}

	// auto-conexión
	//
	code = conectar();
	desconecta = (code == CONECTAR_OK);
	if (code == SMTPERR_CONECTAR)
		return SMTPERR_CONECTAR;


	// leer los archivos adjuntos (si hay)
	//
	if ( correo.getTotalArchivosAdjuntos() > 0 )
	{
		code = correo.leerArchivosAdjuntos();
		if ( code != 0 )
		{
			error(conectado);
			return SMTPERR_ADJUNTOS;
		}
	}


	// se realiza la conversación con el servidor, siguiendo el
	// protocolo SMTP.
	code = procesarConversacionSMTP(correo);
	if (code != SMTPERR_OK)
	{
		error(conectado);
		return code;
	}

	// desconexión
	//
	if (desconecta)
		if (!this->desconectar())
			return SMTPERR_DESCONECTAR;

	LOG_ "***TODO OK" _LOG

	return SMTPERR_OK;
}


int ServidorSMTP::procesarConversacionSMTP(Correo &correo)
{
	//
	// el protocolo SMTP consta de los siguientes pasos:
	//

	// 1: enviamos el saludo al servidor de correo
	//

	if (enviarSaludo(correo) != 0)
		return SMTPERR_HELO;

	// 2: procesamos el caso en el que haya que autenticarse
	//
	if (enviarAutenticacion() != 0)
		return SMTPERR_AUTH;

	// 3: origen del envío
	//
	if( !enviarOrigen(correo) )
		return SMTPERR_MAIL;


	// 4: destinos
	//
	if ( !enviarDestinos(correo) )
		return SMTPERR_RCPT_NORMAL;


	// 5: destinos copia
	//
	if ( !enviarDestinosCopia(correo) )
		return SMTPERR_RCPT_COPIA;


	// 6: destinos ocultos
	//
	if ( !enviarDestinosOcultos(correo) )
		return SMTPERR_RCPT_OCULTO;


	// 7: empezamos con los datos:
	//
	if ( !enviarData() )
		return SMTPERR_DATA;


	// 8: cabecera
	//
	if ( !enviarCabecera(correo) )
		return SMTPERR_CABECERA;


	// 9: body
	//
	if ( !enviarCuerpo(correo) )
		return SMTPERR_CUERPO;


	// 10: fin de datos
	//
	if ( !enviarFin() )
		return SMTPERR_FIN;


	return SMTPERR_OK;
}


int ServidorSMTP::enviarSaludo(Correo &correo)
{
	ArsCadena  aux;
	ArsCadena  temp;
	ArsCadena  token;

	char  buff[4];

	//
	// hacer HELO para login normal o EHLO para login con autenticación
	//

	//
	// Realizamos una copia de la cadena para evitar perder datos
	//

	// Extraemos el dominio del usuario que envia el mensaje
	temp = correo.getRemitente();
	temp.extraerToken('@',token);

	if (temp == "")
		temp = "[127.0.0.1]";

	if (user.longitud() == 0)
		aux.asignar("HELO %s", temp);
	else
		aux.asignar("EHLO %s", temp);

	if ( !transmitir(aux) )
	{
		error(conectado);
		return -1;
	}

	//
	// leer la respuesta y comprobar si se soporta algún tipo de autenticación
	//
	aux = "";
	if ( !this->recibirDatos(aux) )
	{
		error(conectado);
		return -2;
	}

	//
	// comprobar que el código de error es correcto.
	//
	strncpy(buff, aux.cadena(), 3);
	buff[3] = '\0';
	if (atoi(buff) != CODIGO_OK_HELO)
	{
		error(conectado);
		return -3;
	}

	//
	// hay que crear un objeto de autenticación acorde a las posibilidades del servidor.
	// Si retorna NULL, no hay autenticación.
	// Habría que distinguir entre en caso de no soportar la autenticación, del caso de
	// no haber autenticación.
	//
	if (user.longitud() > 0)
		autenticacion = AuthSMTPFactory::crearAuthSMTP(socket, aux.cadena());

	return 0;
}

int ServidorSMTP::enviarAutenticacion()
{
	//
	// En este punto tenemos tres estados posibles:
	// 		1.- Se ha configurado usuario pero el servidor no necesita
	//      autenticación:
	//			Se ignora la autenticación.
	//		2.- El servidor requiere autenticación pero no se ha configurado
	//      usuario:
	//			Retornar error de login SMTPERR_AUTH
	//		3.- El servidor requiere un tipo de autenticación que no soporta el
	//      SDK:
	//			Por ahora continuamos, aunque dará un error más adelante.
	//		4.- Soportamos el tipo de autenticación y tenemos nombre de usuario:
	//			Proceder a hacer la autenticación.
	//

	if (autenticacion != NULL && user.longitud() == 0)
	{
		error(true);
		return SMTPERR_AUTH;
	}
	else if (autenticacion != NULL && user.longitud() > 0)
	{
		// hacer login si tenemos usuario y el servidor lo soporta (caso 4)
		//

		if ( autenticacion->doLogin(user, password) != ERR_AUTH_OK )
		{
			error(true);
			return SMTPERR_LOGIN;
		}
	}
	return 0;
};

bool ServidorSMTP::enviarOrigen(Correo &correo)
{
	ArsCadena  aux;
	aux.asignar("MAIL From: %s", Correo::getCadenaCorreo(correo.getRemitente()));
	return transmitir(aux, CODIGO_OK_MAIL);
}


bool ServidorSMTP::enviarDestinos(Correo &correo)
{
	ArsCadena  aux;
	bool	   ret = true;

	for (DWORD i=0; i<correo.getTotalDestinos() && ret; i++)
	{
		aux.asignar("RCPT TO: %s", Correo::getCadenaCorreo(correo.getDestino(i).cadena()));
		ret = transmitir(aux, CODIGO_OK_RCPT);
	}

	return ret;
}


bool ServidorSMTP::enviarDestinosCopia(Correo &correo)
{
	ArsCadena  aux;
	bool	   ret = true;

	for (DWORD i=0; i<correo.getTotalDestinosCopias() && ret; i++)
	{
		aux.asignar("RCPT TO: %s", Correo::getCadenaCorreo(correo.getDestinoCopia(i)));
		ret = transmitir(aux, CODIGO_OK_RCPT);
	}

	return ret;
}


bool ServidorSMTP::enviarDestinosOcultos(Correo &correo)
{
	ArsCadena  aux;
	bool	   ret = true;

	for (DWORD i=0; i<correo.getTotalDestinosOcultos() && ret; i++)
	{
		aux.asignar("RCPT TO: %s", Correo::getCadenaCorreo(correo.getDestinoOculto(i)));

		ret = transmitir(aux, CODIGO_OK_RCPT);
	}

	return ret;
}


bool ServidorSMTP::enviarData()
{
	ArsCadena  aux("DATA");
	bool	   ret;

	LOG_ "***ahora empieza el DATA" _LOG

	ret = transmitir(aux, CODIGO_OK_DATA);
	if (!ret)
		ERROR_ "***error al transmitir DATA" _ERROR

	return ret;
}


bool ServidorSMTP::enviarCabecera(Correo &correo)
{
	ArsCadena  aux(getCabecera(correo));
	bool	   ret;

	LOG_ "***Enviar la cabecera" _LOG

	ret = transmitir(aux);
	if (!ret)
		ERROR_ "***error al transmitir CABECERA" _ERROR

	return ret;
}


bool ServidorSMTP::enviarCuerpo(const Correo &correo)
{
	ArsCadena aux(getCuerpo(correo).c_str());
	bool  ret;

	LOG_ "***Enviar el BODY" _LOG
	LOG_ "el BODY es: \"%s\"", aux.cadena() _LOG

	ret = transmitir(aux);
	if (!ret)
		ERROR_ "***error al transmitir BODY" _ERROR

	return ret;
}


bool ServidorSMTP::enviarFin()
{
	ArsCadena  aux("\r\n.");
	bool ret;

	LOG_ "***Enviar el fin de datos" _LOG

	ret = transmitir(aux, CODIGO_OK_FINDATA);
	if (!ret)
		ERROR_ "***error al transmitir FIN DE DATOS" _ERROR

	return ret;
}


int ServidorSMTP::conectar()
{
	int  ret;

	if (isConectado())
		return CONECTAR_YA_CONECTADO;

	// Conexión física con el socket
	//
	socket = new RedCliente();

	if ( socket->conectar(host.cadena(), port) )
	{
		LOG_ "Conexión de socket ok!" _LOG
		ServidorEnvio::conectar();

		// leer el mensaje de bienvenida del servidor
		//
		if (getReturnCode() != CODIGO_OK_WELCOME)
		{
			error(true);
			ret = SMTPERR_CONECTAR;
		}
		else
		{
			// Colocamos como codigo de retorno que se ha establecido correctamente
			// la conexión
			//

			ret = CONECTAR_OK;
		}
	}
	else
	{
		ERROR_ "Error en la conexión del socket" _ERROR
		ret = SMTPERR_CONECTAR;
	}

	return ret;
}


bool ServidorSMTP::desconectar()
{
	bool  ret;
	ArsCadena  aux;

	LOG_ "***desconectar el socket" _LOG

	// desconectar socket
	if (isConectado())
	{
		// desconexión lógica
		LOG_ "***Enviar el QUIT" _LOG

		aux = "QUIT";
		if (!transmitir(aux, CODIGO_OK_QUIT))
		{
			ERROR_ "***error al transmitir QUIT" _ERROR
		}

		// desconexión física
		ret = socket->desconectar();

		if (ret)
			ServidorEnvio::desconectar();

		if (ret) {
			INFO_ "socket->desconectar() OK" _INFO
		}
		else {
			ERROR_ "error en socket->desconectar()" _ERROR
		}
	}
	else
		ret = true;

	// liberar socket
	if (socket)
	{
		delete socket;
		socket = NULL;
	}

	// liberar objeto de autenticación
	if (autenticacion)
	{
		delete autenticacion;
		autenticacion = NULL;
	}

	if (!ret)
		ERROR_ "***error al DESCONECTAR" _ERROR

	return (ret);
}


bool ServidorSMTP::enviarDatos(LPCSTR datos, DWORD size)
{
	//
	// importante: en conexiones vía sockets *no* hay que enviar el NULL final.
	// Por eso se quita 1 a la longitud de la cadena
	//
	LOG_ "Se van a enviar %d bytes", size _LOG

	return socket->enviar((LPSTR)datos, size);
}


bool ServidorSMTP::recibirDatos(ArsCadena &datos)
{
	const int BUFFER_SIZE = 128;

	char  buff[BUFFER_SIZE];
	int   size = BUFFER_SIZE;
	int   indLinea;
	char  *ptr;
	char  fin = '-';
	ArsCadena	linea;

	datos = "";

	LOG_ "Recibiendo respuesta..." _LOG
	//
	// Según el RFC 821 (Apéndice E), se considera la última línea aquella que
	// lleva un espacio después del código de retorno:
	//		xxx-texto descriptivo\r\n
	//		xxx-texto descriptivo\r\n
	//		xxx-texto descriptivo\r\n
	//		xxx último texto descriptivo\r\n
	//
	while (fin != ' ')
	{
		if (socket->recibir(buff, size))
		{
			buff[size] = '\0';

			LOG_ "Buffer -> \"%s\"", buff _LOG

			datos += buff;

			//
			// busco la última línea de los datos, para comprobar su 4º carácter.
			// Si el 4º carácter es ' ', se trata de la última línea.
			//
			ptr = strrchr(datos.cadena(), '\r');
			if (ptr)
			{
				while (*--ptr != '\n' && ptr > datos.cadena())
					;
				if (*ptr == '\n')
					ptr++;

				indLinea = ptr - datos.cadena();
			}
			else
				indLinea = 0;

			if (indLinea == 0)
				linea = datos;
			else
				linea = datos.subCadena(indLinea, datos.longitud() - 1);

			if (linea.longitud() >= 4)
				fin = linea[3];
			else
				fin = '-';
		}
		else
			return false;
	}

	return true;
}



bool ServidorSMTP::transmitir(ArsCadena &datos, int code /*= -1*/)
{
	datos += "\r\n";

	bool ret = this->enviarDatos(datos.cadena(), datos.longitud());

	if (ret)
	{
		if (-1 != code)
		{
			int recib = getReturnCode();

			INFO_ "Recibido %d - Esperado %d", recib, code _INFO

			ret = (recib == code);
		}
		else
			ret = true;
	}

	return ret;
}


int ServidorSMTP::getReturnCode()
{
	ArsCadena  respuesta;
	char	   cod[4];

	if ( this->recibirDatos(respuesta) )
	{
		strncpy(cod, respuesta.cadena(), 3);
		cod[3] = '\0';
	}

	return atoi(cod);
}


bool ServidorSMTP::error(bool desconectar)
{
	// se hace el RESET para dar por finalizado el correo
	ArsCadena  aux("RSET");

	bool ret = transmitir(aux, CODIGO_OK_RSET);
	if (ret && desconectar)
		ret = this->desconectar();

	return ret;
}


ArsCadena ServidorSMTP::getCabecera(Correo &correo)
{
	ArsCadena ret, aux;

	ret = getCabeceraOrigen(correo);
	ret += getCabeceraDestinoNormal(correo);
	ret += getCabeceraDestinoCopia(correo);
	ret += getCabeceraAsunto(correo);
	ret += getCabeceraFecha(correo);
	ret += getCabeceraAdjuntos(correo);
	ret += getCabeceraPrioridad(correo);
	ret += getCabeceraCliente();
	ret += getCabeceraSeparadorMIME(correo);

	return (ret);
}

ArsCadena ServidorSMTP::getCabeceraOrigen(Correo &correo)
{
	ArsCadena ret;

	ret.asignar("From: %s\r\n", Correo::getCadenaCorreo(correo.getRemitente()));

	return ret;
}

ArsCadena ServidorSMTP::getCabeceraDestinoNormal(const Correo &correo)
{
	ArsCadena  ret;

	if (correo.getTotalDestinos() > 0)
		ret.asignar("To: %s\r\n", correo.getCadenaDestinos().cadena());

	return ret;
}

ArsCadena ServidorSMTP::getCabeceraDestinoCopia(const Correo &correo)
{
	ArsCadena  ret;

	if (correo.getTotalDestinosCopias() > 0)
		ret.asignar("Cc: %s\r\n", correo.getCadenaDestinosCopias().cadena());

	return ret;
}

ArsCadena ServidorSMTP::getCabeceraAsunto(Correo &correo)
{
	ArsCadena  ret;
	ret.asignar("Subject: %s\r\n", correo.getAsunto().cadena());
	return ret;
}

ArsCadena ServidorSMTP::getCabeceraFecha(const Correo &correo)
{
	ArsCadena  ret;
	ret.asignar("Date: %s\r\n", getFechaActual().cadena());
	return ret;
}

ArsCadena ServidorSMTP::getCabeceraAdjuntos(const Correo &correo)
{
	ArsCadena  ret;

	if (correo.getTotalArchivosAdjuntos() > 0)
		ret.asignar("MIME-Version: 1.0\r\nContent-Type: multipart/mixed; \r\n\tboundary=\"%s\"\r\n",
					SEPARADOR_MIME);
	return ret;
}

ArsCadena ServidorSMTP::getCabeceraPrioridad(const Correo &correo)
{
	ArsCadena  ret;
	ret.asignar("X-Priority: %c\r\n", correo.getNumeroPrioridad());
	return ret;
}

ArsCadena ServidorSMTP::getCabeceraCliente()
{
	ArsCadena  ret("X-Mailer: LibCorreo - ArsNova\r\n");
	return ret;
}

ArsCadena ServidorSMTP::getCabeceraSeparadorMIME(const Correo &correo)
{
	ArsCadena  ret;

	if (correo.getTotalArchivosAdjuntos() > 0)
		ret.asignar("\r\nThis is a multi-part message in MIME format.\r\n\r\n--%s\r\nContent-Type: text/plain;\r\n\tcharset=\"iso-8859-1\"\r\nContent-Transfer-Encoding: 7bit\r\n",
					SEPARADOR_MIME);

	return ret;
}


TextoMensaje ServidorSMTP::getCuerpo(const Correo &correo)
{
	TextoMensaje cuerpo = correo.getCuerpoAjustado();

	cuerpo += "\r\n\r\n";

	if (correo.getTotalArchivosAdjuntos() > 0)
	{
		for (DWORD i=0; i<correo.getTotalArchivosAdjuntos(); i++)
			cuerpo += getCuerpoAdjunto( correo.getArchivoAdjunto(i), (i==0) );

		cuerpo += "\r\n\r\n";
		cuerpo += SEPARADOR_MIME;
	}

	return cuerpo;
}

TextoMensaje ServidorSMTP::getCuerpoAdjunto(ArchivoAdjunto &adjunto, bool primerAdjunto)
{
	TextoMensaje  ret;

	LOG_ "preparando adjunto %s", adjunto.getFilename().cadena() _LOG

	if (!primerAdjunto)
		ret += "\r\n\r\n";

	ret += "--";
	ret += SEPARADOR_MIME;
	ret += "\r\nContent-Type: application/octet-stream;\r\n\tname=\"";

	ret += adjunto.getName().cadena();
	LOG_ "\tobtenido nombre: %s", adjunto.getName().cadena() _LOG

	ret += "\"\r\nContent-Transfer-Encoding: base64\r\n";
	ret += "Content-Disposition: attachment;\r\n\tfilename=\"";
	ret += adjunto.getName().cadena();
	ret += "\"\r\n\r\n";

	LOG_ "\tcodificando... %u", GetTickCount() _LOG
	ret += adjunto.encode().c_str();
	LOG_ "\tcodificado ok! %u", GetTickCount() _LOG

	return ret;
}



ArsCadena ServidorSMTP::getFechaActual()
{
	const char diasSemana[7][4] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
	const char meses[12][4] = 	  {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

	SYSTEMTIME	fecha;
	char buff[64];

	GetSystemTime(&fecha);

	// Formato: Mon, 01 Jun 98 01:10:30 GMT
	//
	//  JM - 03/12/2003: Corregido bug cuando era diciembre. Se accedía al índice 12
	//
	sprintf(buff, "%s, %d %s %d %d:%d:%d GMT",
			diasSemana[fecha.wDayOfWeek],
			fecha.wDay,
			meses[fecha.wMonth-1],
			fecha.wYear,
			fecha.wHour, fecha.wMinute, fecha.wSecond);

	ArsCadena ret(buff);

	return (ret);
}

