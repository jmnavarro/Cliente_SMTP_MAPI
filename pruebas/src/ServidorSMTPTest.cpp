//---------------------------------------------------------------------------

#include "ServidorSMTPTest.h"
#include "TestSuite.h"
#include "../h/Correo.h"
#include "ParametrosTest.h"

//---------------------------------------------------------------------------

ServidorSMTPTest::ServidorSMTPTest(std::string name)
	: TestCase(name)
{
}

ServidorSMTPTest::~ServidorSMTPTest()
{
}

void ServidorSMTPTest::setUp()
{
	obj = new ServidorSMTP();
}

void ServidorSMTPTest::tearDown()
{
	if (obj)
	{
		delete obj;
		obj = NULL;
	}
}


void ServidorSMTPTest::testHost()
{
	//LPCSTR HOST_IP = "192.168.0.111";
	//LPCSTR HOST_NAME = "melon2";

	obj->setHost(ParametrosTest::GetConfiguracion()->GetValor("servidor"));
	assert(obj->getHost() == ParametrosTest::GetConfiguracion()->GetValor("servidor"));

	obj->setHost(ParametrosTest::GetConfiguracion()->GetValor("ipservidor"));
	assert(obj->getHost() == ParametrosTest::GetConfiguracion()->GetValor("ipservidor"));
}


void ServidorSMTPTest::testPort()
{
	const unsigned short int newPort = atoi(ParametrosTest::GetConfiguracion()->GetValor("puerto").cadena());

	assert(obj->getPort() == 25);
	try
	{
		obj->setPort(newPort);
		assert(obj->getPort() == newPort);

		obj->setPort(newPort + 10);
		assert(obj->getPort() == newPort + 10);

		obj->setPort(newPort - 10);
		assert(obj->getPort() == newPort - 10);
	}
	__finally
	{
		obj->setPort(25);
		assert(obj->getPort() == 25);
	}
}

void ServidorSMTPTest::testUser()
{
	//LPCSTR USER = "fulanito";

	assert(obj->getUser() == "");
	try
	{
		obj->setUser(ParametrosTest::GetConfiguracion()->GetValor("usuario"));
		assert(obj->getUser() == ParametrosTest::GetConfiguracion()->GetValor("usuario"));
	}
	__finally
	{
		obj->setUser("");
		assert(obj->getUser() == "");
	}
}

void ServidorSMTPTest::testPassword()
{
	//LPCSTR PASS = "la_clave_de_fulanito";

	assert(obj->getPassword() == "");
	try
	{
		obj->setPassword(ParametrosTest::GetConfiguracion()->GetValor("claveusuario"));
		assert(obj->getPassword() == ParametrosTest::GetConfiguracion()->GetValor("claveusuario"));
	}
	__finally
	{
		obj->setPassword("");
		assert(obj->getPassword() == "");
	}
}

void ServidorSMTPTest::testConectar()
{
	obj->setHost(ParametrosTest::GetConfiguracion()->GetValor("servidor"));

	int ret = obj->conectar();
	try {
		assert(ret == CONECTAR_OK);
		assert(obj->isConectado());
	}
	__finally {
		obj->desconectar();
	}
}

void ServidorSMTPTest::testDesconectar()
{
	obj->setHost(ParametrosTest::GetConfiguracion()->GetValor("servidor"));

	int ret = obj->conectar();
	try {
		assert(ret == CONECTAR_OK);
		assert(obj->isConectado());
	}
	__finally {
		ret = obj->desconectar();
		assert(ret);
		assert(!obj->isConectado());

	}
}

void ServidorSMTPTest::testEnviarCorreo()
{
	Correo c;
	int ret;
	char dir[MAX_PATH], dir2[MAX_PATH];

	obj->setHost(ParametrosTest::GetConfiguracion()->GetValor("servidor"));
//    obj->setHost("smtp.ars-nova.net");

	c.addDestino(ParametrosTest::GetConfiguracion()->GetValor("destinatario1"));
	c.setAsunto(ParametrosTest::GetConfiguracion()->GetValor("asunto"));
	c.setRemitente(ParametrosTest::GetConfiguracion()->GetValor("remitente"));
	c.setCuerpo(ParametrosTest::GetConfiguracion()->GetValor("textomensaje").cadena());

	// se comprueba que no cambie el directorio activo
	GetCurrentDirectory(MAX_PATH, dir);

	ret = obj->enviar(c);

	GetCurrentDirectory(MAX_PATH, dir2);
	assertEquals(dir, dir2);

	assert( SMTPERR_CONECTAR  	 != ret );
	assert( SMTPERR_HELO  		 != ret );
	assert( SMTPERR_MAIL  		 != ret );
	assert( SMTPERR_RCPT_NORMAL  != ret );
	assert( SMTPERR_RCPT_COPIA   != ret );
	assert( SMTPERR_RCPT_OCULTO  != ret );
	assert( SMTPERR_DATA  		 != ret );
	assert( SMTPERR_CABECERA  	 != ret );
	assert( SMTPERR_CUERPO  	 != ret );
	assert( SMTPERR_FIN 		 != ret );
	assert( SMTPERR_QUIT 		 != ret );
	assert( SMTPERR_DESCONECTAR  != ret );

	assert( SMTPERR_OK == ret );
}

void ServidorSMTPTest::testEnviarCorreoSinRemitente()
{
	Correo c;
	int ret;

	obj->setHost(ParametrosTest::GetConfiguracion()->GetValor("servidor"));
//    obj->setHost("smtp.ars-nova.net");

	c.addDestino(ParametrosTest::GetConfiguracion()->GetValor("destinatario1"));
	c.setAsunto(ParametrosTest::GetConfiguracion()->GetValor("asunto") + " correo anonimo");
	c.setCuerpo(ParametrosTest::GetConfiguracion()->GetValor("textomensaje").cadena());

	ret = obj->enviar(c);

	assert( SMTPERR_CONECTAR  	 != ret );
	assert( SMTPERR_HELO  		 != ret );
	assert( SMTPERR_MAIL  		 != ret );
	assert( SMTPERR_RCPT_NORMAL  != ret );
	assert( SMTPERR_RCPT_COPIA   != ret );
	assert( SMTPERR_RCPT_OCULTO  != ret );
	assert( SMTPERR_DATA  		 != ret );
	assert( SMTPERR_CABECERA  	 != ret );
	assert( SMTPERR_CUERPO  	 != ret );
	assert( SMTPERR_FIN 		 != ret );
	assert( SMTPERR_QUIT 		 != ret );
	assert( SMTPERR_DESCONECTAR  != ret );

	assert( SMTPERR_OK == ret );
}

void ServidorSMTPTest::testEnviarCorreoConexion()
{
	Correo c;
	int ret;

	obj->setHost(ParametrosTest::GetConfiguracion()->GetValor("servidor"));

	ret = obj->conectar();
	try
	{
		assert(ret == CONECTAR_OK);
		assert(obj->isConectado());

		c.addDestino(ParametrosTest::GetConfiguracion()->GetValor("destinatario1"));
		c.setAsunto(ParametrosTest::GetConfiguracion()->GetValor("asunto2"));
		c.setRemitente(ParametrosTest::GetConfiguracion()->GetValor("remitente"));
		c.setCuerpo(ParametrosTest::GetConfiguracion()->GetValor("textomensaje").cadena());

		ret = obj->enviar(c);

		assert( SMTPERR_CONECTAR  	 != ret );
		assert( SMTPERR_HELO  		 != ret );
		assert( SMTPERR_MAIL  		 != ret );
		assert( SMTPERR_RCPT_NORMAL  != ret );
		assert( SMTPERR_RCPT_COPIA   != ret );
		assert( SMTPERR_RCPT_OCULTO  != ret );
		assert( SMTPERR_DATA  		 != ret );
		assert( SMTPERR_CABECERA  	 != ret );
		assert( SMTPERR_CUERPO  	 != ret );
		assert( SMTPERR_FIN 		 != ret );
		assert( SMTPERR_QUIT 		 != ret );
		assert( SMTPERR_DESCONECTAR  != ret );

		assert( SMTPERR_OK == ret );
	}
	__finally
	{
		assert(obj->desconectar());
		assert(!obj->isConectado());
	}
}


void ServidorSMTPTest::testEnviarCorreoLogin()
{
	Correo c;
	bool ok;
	int ret;

	// Aquí hay que configurar un servidor que tenga activa la autenticación.
	// melon2, no la tiene así que por ahora pruebo con un servidor local.

	//    obj->setHost("melon2");
	obj->setHost(ParametrosTest::GetConfiguracion()->GetValor("servidorlocal"));

	obj->setUser(ParametrosTest::GetConfiguracion()->GetValor("login"));
	obj->setPassword(ParametrosTest::GetConfiguracion()->GetValor("password"));
	try
	{
		c.addDestino(ParametrosTest::GetConfiguracion()->GetValor("login") + '@' + ParametrosTest::GetConfiguracion()->GetValor("iplocal"));
		c.setAsunto(ParametrosTest::GetConfiguracion()->GetValor("asunto7"));
		c.setRemitente(ParametrosTest::GetConfiguracion()->GetValor("login") + '@' + ParametrosTest::GetConfiguracion()->GetValor("iplocal"));
		c.setCuerpo(ParametrosTest::GetConfiguracion()->GetValor("textomensaje").cadena());

		ret = obj->enviar(c);

		assert( SMTPERR_CONECTAR  	 != ret );
		assert( SMTPERR_HELO  		 != ret );
		assert( SMTPERR_MAIL  		 != ret );
		assert( SMTPERR_RCPT_NORMAL  != ret );
		assert( SMTPERR_RCPT_COPIA   != ret );
		assert( SMTPERR_RCPT_OCULTO  != ret );
		assert( SMTPERR_DATA  		 != ret );
		assert( SMTPERR_CABECERA  	 != ret );
		assert( SMTPERR_CUERPO  	 != ret );
		assert( SMTPERR_FIN 		 != ret );
		assert( SMTPERR_QUIT 		 != ret );
		assert( SMTPERR_DESCONECTAR  != ret );
		assert( SMTPERR_LOGIN		 != ret );

		assert( SMTPERR_OK == ret );
	}
	__finally
	{
		obj->setUser("");
		obj->setPassword("");
	}
}


void ServidorSMTPTest::testEnviarCorreoFaltaLogin()
{
	// Aquí hay que configurar un servidor que tenga activa la autenticación.
	// melon2, no la tiene así que por ahora pruebo con un servidor local.

	//    obj->setHost("melon2");
	obj->setHost(ParametrosTest::GetConfiguracion()->GetValor("servidorlocal"));

	obj->setUser("");
	obj->setPassword("");

	assert(obj->conectar() == SMTPERR_AUTH);
	assert(!obj->isConectado());
}


void ServidorSMTPTest::testEnviarCorreoSinDestinos()
{
	Correo c;
	int ret;

	obj->setHost(ParametrosTest::GetConfiguracion()->GetValor("servidor"));

	c.setAsunto(ParametrosTest::GetConfiguracion()->GetValor("asunto1"));
	c.setRemitente(ParametrosTest::GetConfiguracion()->GetValor("remitente"));
	c.setCuerpo(ParametrosTest::GetConfiguracion()->GetValor("textomensaje").cadena());

	ret = obj->enviar(c);

	assert( ENVIO_SINDESTINOS == ret );
}


void ServidorSMTPTest::testEnviarCorreoConNombre()
{
	Correo c;
	int ret;

	obj->setHost(ParametrosTest::GetConfiguracion()->GetValor("servidor"));

	c.addDestino(ParametrosTest::GetConfiguracion()->GetValor("destinatario1"));
	c.setAsunto(ParametrosTest::GetConfiguracion()->GetValor("asunto1"));
	c.setRemitente(ParametrosTest::GetConfiguracion()->GetValor("destinatarionombre"));
	c.setCuerpo(ParametrosTest::GetConfiguracion()->GetValor("textomensaje").cadena());

	ret = obj->enviar(c);

	assert( SMTPERR_CONECTAR  	 != ret );
	assert( SMTPERR_HELO  		 != ret );
	assert( SMTPERR_MAIL  		 != ret );
	assert( SMTPERR_RCPT_NORMAL  != ret );
	assert( SMTPERR_RCPT_COPIA   != ret );
	assert( SMTPERR_RCPT_OCULTO  != ret );
	assert( SMTPERR_DATA  		 != ret );
	assert( SMTPERR_CABECERA  	 != ret );
	assert( SMTPERR_CUERPO  	 != ret );
	assert( SMTPERR_FIN 		 != ret );
	assert( SMTPERR_QUIT 		 != ret );
	assert( SMTPERR_DESCONECTAR  != ret );

	assert( SMTPERR_OK == ret );
}


void ServidorSMTPTest::testEnviarCorreoLargo()
{

	ArsCadena ruta = ParametrosTest::GetConfiguracion()->GetValor("ruta")+ "texto_correo.txt";

	const char* FICHERO_CORREO = ruta.cadena();

	Correo c;
	TStringList	*lista = new TStringList();
	try
	{
		c.addDestino(ParametrosTest::GetConfiguracion()->GetValor("destinatario1"));
		c.setAsunto(ParametrosTest::GetConfiguracion()->GetValor("asunto1"));
		c.setRemitente(ParametrosTest::GetConfiguracion()->GetValor("remitente"));
		c.setCuerpo(ParametrosTest::GetConfiguracion()->GetValor("textomensaje").cadena());

		SetCurrentDirectory(ExtractFilePath(Application->ExeName).c_str());
		lista->LoadFromFile(FICHERO_CORREO);

		c.setCuerpo(lista->Text.c_str());

		obj->setHost(ParametrosTest::GetConfiguracion()->GetValor("ipservidor"));

		int ret = obj->enviar(c);

		assert( SMTPERR_CONECTAR  	 != ret );
		assert( SMTPERR_HELO  		 != ret );
		assert( SMTPERR_MAIL  		 != ret );
		assert( SMTPERR_RCPT_NORMAL  != ret );
		assert( SMTPERR_RCPT_COPIA   != ret );
		assert( SMTPERR_RCPT_OCULTO  != ret );
		assert( SMTPERR_DATA  		 != ret );
		assert( SMTPERR_CABECERA  	 != ret );
		assert( SMTPERR_CUERPO  	 != ret );
		assert( SMTPERR_FIN 		 != ret );
		assert( SMTPERR_QUIT 		 != ret );
		assert( SMTPERR_DESCONECTAR  != ret );

		assert( SMTPERR_OK == ret );
	}
	__finally {
		delete lista;
	}
}


void ServidorSMTPTest::testEnviarCorreoConAdjunto()
{
	ArsCadena ruta = ParametrosTest::GetConfiguracion()->GetValor("ruta") + "texto.zip";
	const char* FICHERO_ADJUNTO = ruta.cadena();
	Correo c;

	c.addDestino(ParametrosTest::GetConfiguracion()->GetValor("destinatario1"));
	c.setAsunto(ParametrosTest::GetConfiguracion()->GetValor("adjunto1"));
	c.setRemitente(ParametrosTest::GetConfiguracion()->GetValor("remitente"));
	c.setCuerpo(ParametrosTest::GetConfiguracion()->GetValor("textomensaje").cadena());

	SetCurrentDirectory(ExtractFilePath(Application->ExeName).c_str());
	c.addArchivoAdjunto(FICHERO_ADJUNTO);

	obj->setHost(ParametrosTest::GetConfiguracion()->GetValor("servidor"));

	int ret = obj->enviar(c);

	assert( SMTPERR_CONECTAR  	 != ret );
	assert( SMTPERR_HELO  		 != ret );
	assert( SMTPERR_MAIL  		 != ret );
	assert( SMTPERR_RCPT_NORMAL  != ret );
	assert( SMTPERR_RCPT_COPIA   != ret );
	assert( SMTPERR_RCPT_OCULTO  != ret );
	assert( SMTPERR_DATA  		 != ret );
	assert( SMTPERR_CABECERA  	 != ret );
	assert( SMTPERR_CUERPO  	 != ret );
	assert( SMTPERR_FIN 		 != ret );
	assert( SMTPERR_QUIT 		 != ret );
	assert( SMTPERR_DESCONECTAR  != ret );

	assert( SMTPERR_OK == ret );
}


void ServidorSMTPTest::testEnviarCorreoConAdjuntoError()
{
	const char* FICHERO_ADJUNTO = "C:\\este_archivo_no_puede_existir.xxx";
	Correo c;

	c.addDestino(ParametrosTest::GetConfiguracion()->GetValor("destinatario1"));
	c.setAsunto(ParametrosTest::GetConfiguracion()->GetValor("asunto6"));
	c.setRemitente(ParametrosTest::GetConfiguracion()->GetValor("remitente"));
	c.setCuerpo(ParametrosTest::GetConfiguracion()->GetValor("textomensaje").cadena());
	c.addArchivoAdjunto(FICHERO_ADJUNTO);

	ArchivoAdjunto &a = c.getArchivoAdjunto(0);

	obj->setHost(ParametrosTest::GetConfiguracion()->GetValor("servidor"));

	int ret = obj->enviar(c);

	assert( SMTPERR_ADJUNTOS == ret );
	assert(a.getSize() == 0);
}


void ServidorSMTPTest::testEnviarCorreoCon2Adjuntos()
{
	ArsCadena ruta1 = ParametrosTest::GetConfiguracion()->GetValor("ruta") + "texto.zip";
	const char* FICHERO_ADJUNTO_1 = ruta1.cadena();
	ArsCadena ruta2 = ParametrosTest::GetConfiguracion()->GetValor("ruta") + "adjunto.zip";
	const char* FICHERO_ADJUNTO_2 = ruta2.cadena();

	Correo c;

	c.addDestino(ParametrosTest::GetConfiguracion()->GetValor("destinatario1"));
	c.setAsunto(ParametrosTest::GetConfiguracion()->GetValor("asunto4"));
	c.setRemitente(ParametrosTest::GetConfiguracion()->GetValor("remitente"));
	c.setCuerpo(ParametrosTest::GetConfiguracion()->GetValor("textomensaje").cadena());

	SetCurrentDirectory(ExtractFilePath(Application->ExeName).c_str());
	c.addArchivoAdjunto(FICHERO_ADJUNTO_1);
	c.addArchivoAdjunto(FICHERO_ADJUNTO_2);

	obj->setHost(ParametrosTest::GetConfiguracion()->GetValor("ipservidor"));

	int ret = obj->enviar(c);

	assert( SMTPERR_CONECTAR  	 != ret );
	assert( SMTPERR_HELO  		 != ret );
	assert( SMTPERR_MAIL  		 != ret );
	assert( SMTPERR_RCPT_NORMAL  != ret );
	assert( SMTPERR_RCPT_COPIA   != ret );
	assert( SMTPERR_RCPT_OCULTO  != ret );
	assert( SMTPERR_DATA  		 != ret );
	assert( SMTPERR_CABECERA  	 != ret );
	assert( SMTPERR_CUERPO  	 != ret );
	assert( SMTPERR_FIN 		 != ret );
	assert( SMTPERR_QUIT 		 != ret );
	assert( SMTPERR_DESCONECTAR  != ret );

	assert( SMTPERR_OK == ret );
}


void ServidorSMTPTest::testEnviarCorreoCon3Adjuntos()
{
	ArsCadena ruta1 = ParametrosTest::GetConfiguracion()->GetValor("ruta") + "adjunto.zip";
	const char* FICHERO_ADJUNTO_1 = ruta1.cadena();
	ArsCadena ruta2 = ParametrosTest::GetConfiguracion()->GetValor("ruta") + "adjunto.zip";
	const char* FICHERO_ADJUNTO_2 = ruta2.cadena();
	ArsCadena ruta3 = ParametrosTest::GetConfiguracion()->GetValor("ruta") + "adjunto2.zip";
	const char* FICHERO_ADJUNTO_3 = ruta3.cadena();

	Correo c;

	c.addDestino(ParametrosTest::GetConfiguracion()->GetValor("destinatario1"));
	c.setAsunto(ParametrosTest::GetConfiguracion()->GetValor("asunto5"));
	c.setRemitente(ParametrosTest::GetConfiguracion()->GetValor("remitente"));
	c.setCuerpo(ParametrosTest::GetConfiguracion()->GetValor("textomensaje").cadena());

	SetCurrentDirectory(ExtractFilePath(Application->ExeName).c_str());

	c.addArchivoAdjunto(FICHERO_ADJUNTO_1);
	c.addArchivoAdjunto(FICHERO_ADJUNTO_2);
	c.addArchivoAdjunto(FICHERO_ADJUNTO_3);

	obj->setHost(ParametrosTest::GetConfiguracion()->GetValor("servidor"));

	int ret = obj->enviar(c);

	assert( SMTPERR_CONECTAR  	 != ret );
	assert( SMTPERR_HELO  		 != ret );
	assert( SMTPERR_MAIL  		 != ret );
	assert( SMTPERR_RCPT_NORMAL  != ret );
	assert( SMTPERR_RCPT_COPIA   != ret );
	assert( SMTPERR_RCPT_OCULTO  != ret );
	assert( SMTPERR_DATA  		 != ret );
	assert( SMTPERR_CABECERA  	 != ret );
	assert( SMTPERR_CUERPO  	 != ret );
	assert( SMTPERR_FIN 		 != ret );
	assert( SMTPERR_QUIT 		 != ret );
	assert( SMTPERR_DESCONECTAR  != ret );

	assert( SMTPERR_OK == ret );
}


//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
Test* ServidorSMTPTest::suite()
{
	typedef struct _Entrada
	{
		char *nombre;
		void  (ServidorSMTPTest::*metodo)();
	} Entrada;

	static Entrada pruebas[] =
	{
			  {"testHost", 						&ServidorSMTPTest::testHost}
			, {"testPort", 	     				&ServidorSMTPTest::testPort}
			, {"testUser",	   	 				&ServidorSMTPTest::testUser}
			, {"testPassword",   				&ServidorSMTPTest::testPassword}
			, {"testConectar",					&ServidorSMTPTest::testConectar}
			, {"testDesconectar",				&ServidorSMTPTest::testDesconectar}
			, {"testEnviarCorreo", 				&ServidorSMTPTest::testEnviarCorreo}
			, {"testEnviarCorreoAnonimoSinRemitente",	&ServidorSMTPTest::testEnviarCorreoSinRemitente}
			, {"testEnviarCorreoConexion",		&ServidorSMTPTest::testEnviarCorreoConexion}
			, {"testEnviarCorreoLogin",			&ServidorSMTPTest::testEnviarCorreoLogin}
			, {"testEnviarCorreoFaltaLogin", 	&ServidorSMTPTest::testEnviarCorreoFaltaLogin}
			, {"testEnviarCorreoSinDestinos",	&ServidorSMTPTest::testEnviarCorreoSinDestinos}
			, {"testEnviarCorreoConNombre",		&ServidorSMTPTest::testEnviarCorreoConNombre}
			, {"testEnviarCorreoLargo", 		&ServidorSMTPTest::testEnviarCorreoLargo}
			, {"testEnviarCorreoConAdjunto", 	&ServidorSMTPTest::testEnviarCorreoConAdjunto}
			, {"testEnviarCorreoConAdjuntoError",&ServidorSMTPTest::testEnviarCorreoConAdjuntoError}
			, {"testEnviarCorreoCon2Adjuntos",	&ServidorSMTPTest::testEnviarCorreoCon2Adjuntos}
			, {"testEnviarCorreoCon3Adjuntos", 	&ServidorSMTPTest::testEnviarCorreoCon3Adjuntos}
	};

	int TotalEntradas = sizeof(pruebas)/sizeof(Entrada);
	TestSuite *suite = new TestSuite("ServidorSMTP");

	for (int i=0; i<TotalEntradas; i++)
		suite->addTest( new ServidorSMTPTestCaller(pruebas[i].nombre, pruebas[i].metodo) );

	return suite;
}


