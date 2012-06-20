//---------------------------------------------------------------------------
#include "TestSuite.h"

#include "ServidoresDisponiblesTest.h"
#include "ParametrosTest.h"

#include "../../LibConfiguracion/h/ConfiguracionFactory.h"
#include "../../LibConfiguracion/h/ConfiguracionINI.h"
#include "../../LibConfiguracion/h/ConfiguracionModulo.h"

#include "../h/ServidorSMTP.h"
#include "../h/ServidorMAPI.h"

//---------------------------------------------------------------------------


ServidoresDisponiblesTest::ServidoresDisponiblesTest(std::string name)
	: TestCase(name)
{
}

ServidoresDisponiblesTest::~ServidoresDisponiblesTest()
{
}

void ServidoresDisponiblesTest::setUp()
{
}

void ServidoresDisponiblesTest::tearDown()
{
}


void ServidoresDisponiblesTest::testServidorSMTP()
{
	ArsCadena ruta = ParametrosTest::GetConfiguracion()->GetValor("ruta") + "correo.ini";
	const char* FICHERO_CFG = ruta.cadena();

	Configuracion		*cfg;
	ConfiguracionINI	*impl;
	ServidorEnvio		*srv;

	SetCurrentDirectory(ExtractFilePath(Application->ExeName).c_str());
	impl = new ConfiguracionINI(FICHERO_CFG);
	cfg  = new ConfiguracionModulo("[smtp]", impl);
	try
	{
		srv = ServidoresDisponibles::CrearServidor(cfg);
		try
		{
			assert(srv != NULL);

			// el dinamic cast retorna NULL si el objeto creado es de distinto tipo.
			assert(dynamic_cast<ServidorSMTP*> (srv) != NULL);
		}
		__finally {
			delete srv;
		}
	}
	__finally {
		delete cfg;
		delete impl;
	}
}

void ServidoresDisponiblesTest::testServidorMAPI()
{
	ArsCadena ruta = ParametrosTest::GetConfiguracion()->GetValor("ruta") + "correo.ini";
	const char* FICHERO_CFG = ruta.cadena();

	Configuracion		*cfg;
	ConfiguracionINI	*impl;
	ServidorEnvio		*srv;

	SetCurrentDirectory(ExtractFilePath(Application->ExeName).c_str());
	impl = new ConfiguracionINI(FICHERO_CFG);
	cfg  = new ConfiguracionModulo("[mapi]", impl);
	try
	{
		srv = ServidoresDisponibles::CrearServidor(cfg);
		try
		{
			assert(srv != NULL);
			assert(dynamic_cast<ServidorMAPI*> (srv) != NULL);
		}
		__finally {
			delete srv;
		}
	}
	__finally {
		delete cfg;
		delete impl;
	}
}

void ServidoresDisponiblesTest::testEnvioDinamicoSMTP()
{
	ArsCadena ruta = ParametrosTest::GetConfiguracion()->GetValor("ruta") + "correo.ini";
	const char* FICHERO_CFG = ruta.cadena();

	Configuracion		*cfg;
	ConfiguracionINI	*impl;
	ServidorEnvio		*srv;
	Correo  c;
	int     ret;

	SetCurrentDirectory(ExtractFilePath(Application->ExeName).c_str());
	impl = new ConfiguracionINI(FICHERO_CFG);
	cfg  = new ConfiguracionModulo("[smtp]", impl);
	try
	{
		srv = ServidoresDisponibles::CrearServidor(cfg);
		try
		{
			assert(srv != NULL);

			// el dinamic cast retorna NULL si el objeto creado es de distinto tipo.
			assert(dynamic_cast<ServidorSMTP*> (srv) != NULL);

			c.addDestino(ParametrosTest::GetConfiguracion()->GetValor("destinatario1"));
			c.setAsunto(ParametrosTest::GetConfiguracion()->GetValor("asunto1") + " dinamica SMTP");
			c.setRemitente(ParametrosTest::GetConfiguracion()->GetValor("remitente"));
			c.setCuerpo(ParametrosTest::GetConfiguracion()->GetValor("textomensaje").cadena());

			ret = srv->enviar(c);
			assert( ENVIO_OK == ret );
		}
		__finally {
			delete srv;
		}
	}
	__finally {
		delete cfg;
		delete impl;
	}
}

void ServidoresDisponiblesTest::testEnvioDinamicoMAPI()
{
	ArsCadena ruta = ParametrosTest::GetConfiguracion()->GetValor("ruta") + "correo.ini";
	const char* FICHERO_CFG = ruta.cadena();

	Configuracion		*cfg;
	ConfiguracionINI	*impl;
	ServidorEnvio		*srv;
	Correo  c;
	int     ret;

	SetCurrentDirectory(ExtractFilePath(Application->ExeName).c_str());
	impl = new ConfiguracionINI(FICHERO_CFG);
	cfg  = new ConfiguracionModulo("[mapi]", impl);
	try
	{
		srv = ServidoresDisponibles::CrearServidor(cfg);
		try
		{
			assert(srv != NULL);

			// el dinamic cast retorna NULL si el objeto creado es de distinto tipo.
			assert(dynamic_cast<ServidorMAPI*> (srv) != NULL);

			c.addDestino(ParametrosTest::GetConfiguracion()->GetValor("destinatario1"));
			c.setAsunto(ParametrosTest::GetConfiguracion()->GetValor("asunto1") + " dinamica MAPI");
			c.setRemitente(ParametrosTest::GetConfiguracion()->GetValor("remitente"));
			c.setCuerpo(ParametrosTest::GetConfiguracion()->GetValor("textomensaje").cadena());

			ret = srv->enviar(c);
			assert( ENVIO_OK == ret );
		}
		__finally {
			delete srv;
		}
	}
	__finally {
		delete cfg;
		delete impl;
	}
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
Test* ServidoresDisponiblesTest::suite()
{
	typedef struct _Entrada
	{
		char *nombre;
		void  (ServidoresDisponiblesTest::*metodo)();
	} Entrada;

	static Entrada pruebas[] =
	{
				{"testServidorSMTP",		&ServidoresDisponiblesTest::testServidorSMTP}
			  , {"testServidorMAPI",		&ServidoresDisponiblesTest::testServidorMAPI}
			  , {"testEnvioDinamicoSMTP",	&ServidoresDisponiblesTest::testEnvioDinamicoSMTP}
			  , {"testEnvioDinamicoMAPI",	&ServidoresDisponiblesTest::testEnvioDinamicoMAPI}
	};

	int TotalEntradas = sizeof(pruebas)/sizeof(Entrada);
	TestSuite *suite = new TestSuite("ServidoresDisponibles");

	for (int i=0; i<TotalEntradas; i++)
		suite->addTest( new ServidoresDisponiblesTestCaller(pruebas[i].nombre, pruebas[i].metodo) );

	return suite;
}


