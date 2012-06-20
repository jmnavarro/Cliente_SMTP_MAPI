//---------------------------------------------------------------------------

#include "ServidorMAPITest.h"
#include "TestSuite.h"
#include "../h/Correo.h"
#include "ParametrosTest.h"
#include <Forms.hpp>

//---------------------------------------------------------------------------

ServidorMAPITest::ServidorMAPITest(std::string name)
	: TestCase(name)
{
	obj = NULL;
}

ServidorMAPITest::~ServidorMAPITest()
{
}

void ServidorMAPITest::setUp()
{
	obj = new ServidorMAPI();
}

void ServidorMAPITest::tearDown()
{
	if (obj)
	{
		delete obj;
		obj = NULL;
	}
}


void ServidorMAPITest::testProfile()
{
	obj->setProfile(ParametrosTest::GetConfiguracion()->GetValor("profile"));
	assert(obj->getProfile() == ParametrosTest::GetConfiguracion()->GetValor("profile"));

	obj->setProfile(ParametrosTest::GetConfiguracion()->GetValor("sinprofile"));
	assert(obj->getProfile() == ParametrosTest::GetConfiguracion()->GetValor("sinprofile"));
}


void ServidorMAPITest::testPassword()
{
	assert(obj->getPassword() == "");

	obj->setPassword(ParametrosTest::GetConfiguracion()->GetValor("claveusuario"));
	assert(obj->getPassword() == ParametrosTest::GetConfiguracion()->GetValor("claveusuario"));

	obj->setPassword("");
	assert(obj->getPassword() == "");
}

void ServidorMAPITest::testConectar()
{
	char dir[MAX_PATH], dir2[MAX_PATH];

	obj->setProfile(ParametrosTest::GetConfiguracion()->GetValor("sinprofile"));
	obj->setPassword(ParametrosTest::GetConfiguracion()->GetValor("sinprofile"));

	// se comprueba que no cambie el directorio activo
	GetCurrentDirectory(MAX_PATH, dir);

	int ret = obj->conectar();

	GetCurrentDirectory(MAX_PATH, dir2);

	try {
		assertEquals(dir, dir2);

		assert(ret == CONECTAR_OK);
		assert(obj->isConectado());
	}
	__finally {
		obj->desconectar();
	}
}

void ServidorMAPITest::testConectarSinConfiguracion()
{
	int ret = obj->conectar();
	try
	{
		assert(ret == CONECTAR_OK);
		assert(obj->isConectado());
	}
	__finally {
		obj->desconectar();
	}
}

void ServidorMAPITest::testDesconectar()
{
	obj->setProfile(ParametrosTest::GetConfiguracion()->GetValor("sinprofile"));
	obj->setPassword(ParametrosTest::GetConfiguracion()->GetValor("sinprofile"));

	int ret = obj->conectar();
	try
	{
		assert(ret == CONECTAR_OK);
		assert(obj->isConectado());
	}
	__finally
	{
		ret = obj->desconectar();
		assert(ret);
		assert(!obj->isConectado());
	}
}

void ServidorMAPITest::testEnviarCorreo()
{
	Correo c;
	int ret;
	char dir[MAX_PATH], dir2[MAX_PATH];

	obj->setProfile(ParametrosTest::GetConfiguracion()->GetValor("sinprofile"));
	obj->setPassword(ParametrosTest::GetConfiguracion()->GetValor("sinprofile"));

	c.addDestino(ParametrosTest::GetConfiguracion()->GetValor("destinatario1"));
	c.setAsunto(ParametrosTest::GetConfiguracion()->GetValor("asunto1") + " MAPI");
	c.setRemitente(ParametrosTest::GetConfiguracion()->GetValor("remitente"));
	c.setCuerpo(ParametrosTest::GetConfiguracion()->GetValor("textomensaje").cadena());

	// se comprueba que no cambie el directorio activo
	GetCurrentDirectory(MAX_PATH, dir);

	ret = obj->enviar(c);

	GetCurrentDirectory(MAX_PATH, dir2);
	assertEquals(dir, dir2);

	assert( MAPIERR_CARGAR  	!= ret );
	assert( MAPIERR_CONECTAR  	!= ret );
	assert( MAPIERR_DESCONECTAR != ret );
	assert( MAPIERR_ENVIAR		!= ret );
	assert( MAPIERR_AMBIGUOUS_RECIPIENT != ret );
	assert( MAPIERR_ATTACHMENT_NOT_FOUND != ret );
	assert( MAPIERR_ATTACHMENT_OPEN_FAILURE != ret );
	assert( MAPIERR_BAD_RECIPTYPE != ret );
	assert( MAPIERR_FAILURE != ret );
	assert( MAPIERR_INSUFFICIENT_MEMORY != ret );
	assert( MAPIERR_INVALID_RECIPS != ret );
	assert( MAPIERR_LOGIN_FAILURE != ret );
	assert( MAPIERR_TEXT_TOO_LARGE != ret );
	assert( MAPIERR_TOO_MANY_FILES != ret );
	assert( MAPIERR_TOO_MANY_RECIPIENTS != ret );
	assert( MAPIERR_UNKNOWN_RECIPIENT != ret );
	assert( MAPIERR_USER_ABORT != ret );

	assert( MAPIERR_OK == ret );
}


void ServidorMAPITest::testEnviarCorreoConexion()
{
	Correo c;
	int ret;

	obj->setProfile(ParametrosTest::GetConfiguracion()->GetValor("sinprofile"));
	obj->setPassword(ParametrosTest::GetConfiguracion()->GetValor("sinprofile"));

	ret = obj->conectar();
	try
	{
		assert(ret == CONECTAR_OK);
		assert(obj->isConectado());

		c.addDestino(ParametrosTest::GetConfiguracion()->GetValor("destinatario1"));
		c.setAsunto(ParametrosTest::GetConfiguracion()->GetValor("asunto1") + " MAPI con conexion");
		c.setRemitente(ParametrosTest::GetConfiguracion()->GetValor("remitente"));
		c.setCuerpo(ParametrosTest::GetConfiguracion()->GetValor("textomensaje").cadena());

		ret = obj->enviar(c);

		assert( MAPIERR_CARGAR  	!= ret );
		assert( MAPIERR_CONECTAR  	!= ret );
		assert( MAPIERR_DESCONECTAR != ret );
		assert( MAPIERR_ENVIAR		!= ret );
		assert( MAPIERR_AMBIGUOUS_RECIPIENT != ret );
		assert( MAPIERR_ATTACHMENT_NOT_FOUND != ret );
		assert( MAPIERR_ATTACHMENT_OPEN_FAILURE != ret );
		assert( MAPIERR_BAD_RECIPTYPE != ret );
		assert( MAPIERR_FAILURE != ret );
		assert( MAPIERR_INSUFFICIENT_MEMORY != ret );
		assert( MAPIERR_INVALID_RECIPS != ret );
		assert( MAPIERR_LOGIN_FAILURE != ret );
		assert( MAPIERR_TEXT_TOO_LARGE != ret );
		assert( MAPIERR_TOO_MANY_FILES != ret );
		assert( MAPIERR_TOO_MANY_RECIPIENTS != ret );
		assert( MAPIERR_UNKNOWN_RECIPIENT != ret );
		assert( MAPIERR_USER_ABORT != ret );

		assert( MAPIERR_OK == ret );
	}
	__finally
	{
		assert(obj->desconectar());
		assert(!obj->isConectado());
	}
}


void ServidorMAPITest::testEnviarCorreoSinDestinos()
{
	Correo c;
	int ret;

	obj->setProfile("");
	obj->setPassword("");

	c.setAsunto(ParametrosTest::GetConfiguracion()->GetValor("asunto1") + " MAPI sin destinos");
	c.setRemitente(ParametrosTest::GetConfiguracion()->GetValor("remitente"));
	c.setCuerpo(ParametrosTest::GetConfiguracion()->GetValor("textomensaje").cadena());

	ret = obj->enviar(c);

	assert( ENVIO_SINDESTINOS == ret );
}


void ServidorMAPITest::testEnviarCorreoLargo()
{

	ArsCadena ruta = ParametrosTest::GetConfiguracion()->GetValor("ruta") + "texto_correo.txt";
	const char* FICHERO_CORREO = ruta.cadena();

	Correo c;
	TStringList	*lista = new TStringList();
	try
	{
		c.addDestino(ParametrosTest::GetConfiguracion()->GetValor("destinatario1"));
		c.setAsunto(ParametrosTest::GetConfiguracion()->GetValor("asunto1") + " MAPI con conexion");
		c.setRemitente(ParametrosTest::GetConfiguracion()->GetValor("remitente"));
		c.setCuerpo(ParametrosTest::GetConfiguracion()->GetValor("textomensaje").cadena());

		SetCurrentDirectory(ExtractFilePath(Application->ExeName).c_str());
		lista->LoadFromFile(FICHERO_CORREO);

		c.setCuerpo(lista->Text.c_str());

		obj->setProfile(ParametrosTest::GetConfiguracion()->GetValor("sinprofile"));
		obj->setPassword(ParametrosTest::GetConfiguracion()->GetValor("sinprofile"));

		int ret = obj->enviar(c);

		assert( MAPIERR_CARGAR  	!= ret );
		assert( MAPIERR_CONECTAR  	!= ret );
		assert( MAPIERR_DESCONECTAR != ret );
		assert( MAPIERR_ENVIAR		!= ret );
		assert( MAPIERR_AMBIGUOUS_RECIPIENT != ret );
		assert( MAPIERR_ATTACHMENT_NOT_FOUND != ret );
		assert( MAPIERR_ATTACHMENT_OPEN_FAILURE != ret );
		assert( MAPIERR_BAD_RECIPTYPE != ret );
		assert( MAPIERR_FAILURE != ret );
		assert( MAPIERR_INSUFFICIENT_MEMORY != ret );
		assert( MAPIERR_INVALID_RECIPS != ret );
		assert( MAPIERR_LOGIN_FAILURE != ret );
		assert( MAPIERR_TEXT_TOO_LARGE != ret );
		assert( MAPIERR_TOO_MANY_FILES != ret );
		assert( MAPIERR_TOO_MANY_RECIPIENTS != ret );
		assert( MAPIERR_UNKNOWN_RECIPIENT != ret );
		assert( MAPIERR_USER_ABORT != ret );

		assert( MAPIERR_OK == ret );
	}
	__finally {
		delete lista;
	}
}


void ServidorMAPITest::testEnviarCorreoConAdjunto()
{
//	const char* FICHERO_ADJUNTO = "C:\\src\\LibCorreo\\lib\\data\\texto.zip";
	ArsCadena ruta = ParametrosTest::GetConfiguracion()->GetValor("ruta") + "texto.zip";
	const char* FICHERO_ADJUNTO = ruta.cadena();
	Correo c;

	c.addDestino(ParametrosTest::GetConfiguracion()->GetValor("destinatario1"));
	c.setAsunto(ParametrosTest::GetConfiguracion()->GetValor("asunto1") + " MAPI con 2 adjuntos");
	c.setRemitente(ParametrosTest::GetConfiguracion()->GetValor("remitente"));
	c.setCuerpo(ParametrosTest::GetConfiguracion()->GetValor("textomensaje").cadena());

	SetCurrentDirectory(ExtractFilePath(Application->ExeName).c_str());
	c.addArchivoAdjunto(FICHERO_ADJUNTO);

	obj->setProfile(ParametrosTest::GetConfiguracion()->GetValor("sinprofile"));
	obj->setPassword(ParametrosTest::GetConfiguracion()->GetValor("sinprofile"));

	int ret = obj->enviar(c);

	assert( MAPIERR_CARGAR  	!= ret );
	assert( MAPIERR_CONECTAR  	!= ret );
	assert( MAPIERR_DESCONECTAR != ret );
	assert( MAPIERR_ENVIAR		!= ret );
	assert( MAPIERR_AMBIGUOUS_RECIPIENT != ret );
	assert( MAPIERR_ATTACHMENT_NOT_FOUND != ret );
	assert( MAPIERR_ATTACHMENT_OPEN_FAILURE != ret );
	assert( MAPIERR_BAD_RECIPTYPE != ret );
	assert( MAPIERR_FAILURE != ret );
	assert( MAPIERR_INSUFFICIENT_MEMORY != ret );
	assert( MAPIERR_INVALID_RECIPS != ret );
	assert( MAPIERR_LOGIN_FAILURE != ret );
	assert( MAPIERR_TEXT_TOO_LARGE != ret );
	assert( MAPIERR_TOO_MANY_FILES != ret );
	assert( MAPIERR_TOO_MANY_RECIPIENTS != ret );
	assert( MAPIERR_UNKNOWN_RECIPIENT != ret );
	assert( MAPIERR_USER_ABORT != ret );

	assert( MAPIERR_OK == ret );
}

void ServidorMAPITest::testEnviarCorreoCon2Adjuntos()
{

	ArsCadena ruta1 = ParametrosTest::GetConfiguracion()->GetValor("ruta") + "texto.zip";
	const char* FICHERO_ADJUNTO_1 = ruta1.cadena();
	ArsCadena ruta2 = ParametrosTest::GetConfiguracion()->GetValor("ruta") + "adjunto.zip";
	const char* FICHERO_ADJUNTO_2 = ruta2.cadena();
	Correo c;

	c.addDestino(ParametrosTest::GetConfiguracion()->GetValor("destinatario1"));
	c.setAsunto(ParametrosTest::GetConfiguracion()->GetValor("asunto1") + " MAPI con adjunto");
	c.setRemitente(ParametrosTest::GetConfiguracion()->GetValor("remitente"));
	c.setCuerpo(ParametrosTest::GetConfiguracion()->GetValor("textomensaje").cadena());

	SetCurrentDirectory(ExtractFilePath(Application->ExeName).c_str());
	c.addArchivoAdjunto(FICHERO_ADJUNTO_1);
	c.addArchivoAdjunto(FICHERO_ADJUNTO_2);


	obj->setProfile(ParametrosTest::GetConfiguracion()->GetValor("sinprofile"));
	obj->setPassword(ParametrosTest::GetConfiguracion()->GetValor("sinprofile"));


	int ret = obj->enviar(c);

	assert( MAPIERR_CARGAR  	!= ret );
	assert( MAPIERR_CONECTAR  	!= ret );
	assert( MAPIERR_DESCONECTAR != ret );
	assert( MAPIERR_ENVIAR		!= ret );
	assert( MAPIERR_AMBIGUOUS_RECIPIENT != ret );
	assert( MAPIERR_ATTACHMENT_NOT_FOUND != ret );
	assert( MAPIERR_ATTACHMENT_OPEN_FAILURE != ret );
	assert( MAPIERR_BAD_RECIPTYPE != ret );
	assert( MAPIERR_FAILURE != ret );
	assert( MAPIERR_INSUFFICIENT_MEMORY != ret );
	assert( MAPIERR_INVALID_RECIPS != ret );
	assert( MAPIERR_LOGIN_FAILURE != ret );
	assert( MAPIERR_TEXT_TOO_LARGE != ret );
	assert( MAPIERR_TOO_MANY_FILES != ret );
	assert( MAPIERR_TOO_MANY_RECIPIENTS != ret );
	assert( MAPIERR_UNKNOWN_RECIPIENT != ret );
	assert( MAPIERR_USER_ABORT != ret );

	assert( MAPIERR_OK == ret );
}


void ServidorMAPITest::testEnviarCorreoCon3Adjuntos()
{
	ArsCadena ruta1 = ParametrosTest::GetConfiguracion()->GetValor("ruta") + "texto.zip";
	const char* FICHERO_ADJUNTO_1 = ruta1.cadena();
	ArsCadena ruta2 = ParametrosTest::GetConfiguracion()->GetValor("ruta") + "adjunto.zip";
	const char* FICHERO_ADJUNTO_2 = ruta2.cadena();
	ArsCadena ruta3 = ParametrosTest::GetConfiguracion()->GetValor("ruta") + "adjunto2.zip";
	const char* FICHERO_ADJUNTO_3 = ruta3.cadena();
	Correo c;

	c.addDestino(ParametrosTest::GetConfiguracion()->GetValor("destinatario1"));
	c.setAsunto(ParametrosTest::GetConfiguracion()->GetValor("asunto1") + " MAPI con 3 adjuntos");
	c.setRemitente(ParametrosTest::GetConfiguracion()->GetValor("remitente"));
	c.setCuerpo(ParametrosTest::GetConfiguracion()->GetValor("textomensaje").cadena());


	SetCurrentDirectory(ExtractFilePath(Application->ExeName).c_str());
	c.addArchivoAdjunto(FICHERO_ADJUNTO_1);
	c.addArchivoAdjunto(FICHERO_ADJUNTO_2);
	c.addArchivoAdjunto(FICHERO_ADJUNTO_3);

	obj->setProfile(ParametrosTest::GetConfiguracion()->GetValor("sinprofile"));
	obj->setPassword(ParametrosTest::GetConfiguracion()->GetValor("sinprofile"));

	int ret = obj->enviar(c);

	assert( MAPIERR_CARGAR  	!= ret );
	assert( MAPIERR_CONECTAR  	!= ret );
	assert( MAPIERR_DESCONECTAR != ret );
	assert( MAPIERR_ENVIAR		!= ret );
	assert( MAPIERR_AMBIGUOUS_RECIPIENT != ret );
	assert( MAPIERR_ATTACHMENT_NOT_FOUND != ret );
	assert( MAPIERR_ATTACHMENT_OPEN_FAILURE != ret );
	assert( MAPIERR_BAD_RECIPTYPE != ret );
	assert( MAPIERR_FAILURE != ret );
	assert( MAPIERR_INSUFFICIENT_MEMORY != ret );
	assert( MAPIERR_INVALID_RECIPS != ret );
	assert( MAPIERR_LOGIN_FAILURE != ret );
	assert( MAPIERR_TEXT_TOO_LARGE != ret );
	assert( MAPIERR_TOO_MANY_FILES != ret );
	assert( MAPIERR_TOO_MANY_RECIPIENTS != ret );
	assert( MAPIERR_UNKNOWN_RECIPIENT != ret );
	assert( MAPIERR_USER_ABORT != ret );

	assert( MAPIERR_OK == ret );
}


//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
Test* ServidorMAPITest::suite()
{
	typedef struct _Entrada
	{
		char *nombre;
		void  (ServidorMAPITest::*metodo)();
	} Entrada;

	static Entrada pruebas[] =
	{
			  {"testProfile", 	   				&ServidorMAPITest::testProfile}
			, {"testPassword",     				&ServidorMAPITest::testPassword}
			, {"testConectar",					&ServidorMAPITest::testConectar}
			, {"testConectarSinConfiguracion",  &ServidorMAPITest::testConectarSinConfiguracion}
			, {"testDesconectar",				&ServidorMAPITest::testDesconectar}
			, {"testEnviarCorreo", 				&ServidorMAPITest::testEnviarCorreo}
			, {"testEnviarCorreoConexion",		&ServidorMAPITest::testEnviarCorreoConexion}
			, {"testEnviarCorreoSinDestinos",	&ServidorMAPITest::testEnviarCorreoSinDestinos}
			, {"testEnviarCorreoLargo", 		&ServidorMAPITest::testEnviarCorreoLargo}
			, {"testEnviarCorreoConAdjunto", 	&ServidorMAPITest::testEnviarCorreoConAdjunto}
			, {"testEnviarCorreoCon2Adjuntos",	&ServidorMAPITest::testEnviarCorreoCon2Adjuntos}
			, {"testEnviarCorreoCon3Adjuntos", 	&ServidorMAPITest::testEnviarCorreoCon3Adjuntos}
	};

	int TotalEntradas = sizeof(pruebas)/sizeof(Entrada);
	TestSuite *suite = new TestSuite("ServidorMAPI");

	for (int i=0; i<TotalEntradas; i++)
		suite->addTest( new ServidorMAPITestCaller(pruebas[i].nombre, pruebas[i].metodo) );

	return suite;
}
