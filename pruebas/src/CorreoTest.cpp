#include <vcl.h>
#pragma hrdstop

#include "CorreoTest.h"
#include "TestSuite.h"
#include "ParametrosTest.h"
#include "../h/Correo.h"

CorreoTest::CorreoTest(std::string name)
	: TestCase(name)
{
}


CorreoTest::~CorreoTest()
{
}


void CorreoTest::setUp()
{
	obj = new Correo();
}

void CorreoTest::tearDown()
{
	delete obj;
}


void CorreoTest::testAsunto()
{

	obj->setAsunto(ParametrosTest::GetConfiguracion()->GetValor("asunto2"));

	assert(obj->getAsunto() == ParametrosTest::GetConfiguracion()->GetValor("asunto2"));
}

void CorreoTest::testCuerpo()
{

	ArsCadena ruta = ParametrosTest::GetConfiguracion()->GetValor("ruta") + "cuerpo.txt";
	const char* FICH_CUERPO = ruta.cadena();

	TStringList	*texto = new TStringList();
	try
	{
		texto->LoadFromFile(FICH_CUERPO);

		obj->setCuerpo(texto->Text.c_str());

		assert(obj->getCuerpo().length() == (size_t) texto->Text.Length());
		assert(0 == strcmp(obj->getCuerpo().c_str(), texto->Text.c_str()));
	}
	__finally {
		delete texto;
	}
}

void CorreoTest::testCuerpoAjustado()
{
	ArsCadena ruta = ParametrosTest::GetConfiguracion()->GetValor("ruta") + "cuerpo.txt";
	const char* FICH_CUERPO = ruta.cadena();
	ArsCadena ruta1 = ParametrosTest::GetConfiguracion()->GetValor("ruta") + "cuerpo_ajus.txt";
	const char* FICH_CUERPO_AJUS = ruta1.cadena();

	TStringList *fichAjus;
	TStringList	*fich;
	TextoMensaje msg;

	fich = new TStringList();
	try
	{
		fich->LoadFromFile(FICH_CUERPO);
		fich->Add("");

		obj->setCuerpo(fich->Text.c_str());

		assert(obj->getCuerpo().length() == (size_t) fich->Text.Length());
		assert(0 == strcmp(obj->getCuerpo().c_str(), fich->Text.c_str()));

		msg = obj->getCuerpoAjustado();

		fichAjus = new TStringList();
		try
		{
			fichAjus->LoadFromFile(FICH_CUERPO_AJUS);

			assert(0 == fichAjus->Text.AnsiCompare(msg.c_str()));
//			assert(0 == strncmp(nuevo, ok, msg.length()));
		}
		__finally {
			delete fichAjus;
		}

	}
	__finally {
		delete fich;
	}
}


void CorreoTest::testAddDestino()
{
	const DWORD total = obj->getTotalDestinos();

	obj->addDestino(ParametrosTest::GetConfiguracion()->GetValor("remitente"));
	try {
		assert(total + 1 == obj->getTotalDestinos());
	}
	__finally
	{
		obj->clearDestinos();
		assert(obj->getTotalDestinos() == 0);
	}
}

void CorreoTest::testAddDestinoCopia()
{
	const DWORD total = obj->getTotalDestinosCopias();

	obj->addDestinoCopia(ParametrosTest::GetConfiguracion()->GetValor("remitente"));
	try {
		assert(total + 1 == obj->getTotalDestinosCopias());
	}
	__finally
	{
		obj->clearDestinosCopias();
		assert(obj->getTotalDestinosCopias() == 0);
	}
}

void CorreoTest::testAddDestinoOculto()
{
	const DWORD total = obj->getTotalDestinosOcultos();

	obj->addDestinoOculto(ParametrosTest::GetConfiguracion()->GetValor("remitente"));
	try {
		assert(total + 1 == obj->getTotalDestinosOcultos());
	}
	__finally
	{
		obj->clearDestinosOcultos();
		assert(obj->getTotalDestinosOcultos() == 0);
	}
}

void CorreoTest::testRemoveDestinoName()
{
	const char* MAIL = "fulano@menagano.com";
	const int INSERTAR = 10;
	const DWORD total = obj->getTotalDestinos();

	try
	{
		for (int i=1; i<=INSERTAR; i++)
		{
			obj->addDestino(MAIL);
			assert(total + i == obj->getTotalDestinos());
		}
		assert(total + INSERTAR == obj->getTotalDestinos());

		for (int i=1; i<=INSERTAR; i++)
		{
			obj->removeDestino(MAIL);
			assert(total + INSERTAR - i == obj->getTotalDestinos());
		}
		assert(total == obj->getTotalDestinos());
	}
	__finally
	{
		obj->clearDestinos();
		assert(obj->getTotalDestinos() == 0);
	}
}

void CorreoTest::testRemoveDestinoCopiaName()
{
	const char* MAIL = "fulano@menagano.com";
	const int INSERTAR = 10;
	const DWORD total = obj->getTotalDestinosCopias();

	try
	{
		for (int i=1; i<=INSERTAR; i++)
		{
			obj->addDestinoCopia(MAIL);
			assert(total + i == obj->getTotalDestinosCopias());
		}
		assert(total + INSERTAR == obj->getTotalDestinosCopias());

		for (int i=1; i<=INSERTAR; i++)
		{
			obj->removeDestinoCopia(MAIL);
			assert(total + INSERTAR - i == obj->getTotalDestinosCopias());
		}
		assert(total == obj->getTotalDestinosCopias());
	}
	__finally
	{
		obj->clearDestinosCopias();
		assert(obj->getTotalDestinosCopias() == 0);
	}
}


void CorreoTest::testRemoveDestinoOcultoName()
{
	const char* MAIL = "fulano@menagano.com";
	const int INSERTAR = 10;
	const DWORD total = obj->getTotalDestinosOcultos();

	try
	{
		for (int i=1; i<=INSERTAR; i++)
		{
			obj->addDestinoOculto(MAIL);
			assert(total + i == obj->getTotalDestinosOcultos());
		}
		assert(total + INSERTAR == obj->getTotalDestinosOcultos());

		for (int i=1; i<=INSERTAR; i++)
		{
			obj->removeDestinoOculto(MAIL);
			assert(total + INSERTAR - i == obj->getTotalDestinosOcultos());
		}
		assert(total == obj->getTotalDestinosOcultos());
	}
	__finally
	{
		obj->clearDestinosOcultos();
		assert(obj->getTotalDestinosOcultos() == 0);
	}
}

void CorreoTest::testRemoveDestinoIndex()
{
	const char* MAIL = "fulano@menagano.com";
	const int INSERTAR = 10;
	const DWORD total = obj->getTotalDestinos();

	obj->clearDestinos();
	assert(0 == obj->getTotalDestinos());

	try
	{
		for (int i=1; i<=INSERTAR; i++)
		{
			obj->addDestino(MAIL);
			assert(total + i == obj->getTotalDestinos());
		}
		assert(INSERTAR == obj->getTotalDestinos());

		for (int i=1; i<=INSERTAR; i++)
		{
			obj->removeDestino(0);
			assert(total + INSERTAR - i == obj->getTotalDestinos());
		}
		assert(0 == obj->getTotalDestinos());
	}
	__finally
	{
		obj->clearDestinos();
		assert(0 == obj->getTotalDestinos());
	}
}

void CorreoTest::testRemoveDestinoCopiaIndex()
{
	const char* MAIL = "fulano@menagano.com";
	const int INSERTAR = 10;
	const DWORD total = obj->getTotalDestinosCopias();

	obj->clearDestinosCopias();
	assert(0 == obj->getTotalDestinosCopias());

	try
	{
		for (int i=1; i<=INSERTAR; i++)
		{
			obj->addDestinoCopia(MAIL);
			assert(total + i == obj->getTotalDestinosCopias());
		}
		assert(INSERTAR == obj->getTotalDestinosCopias());

		for (int i=1; i<=INSERTAR; i++)
		{
			obj->removeDestinoCopia(0);
			assert(total + INSERTAR - i == obj->getTotalDestinosCopias());
		}
		assert(0 == obj->getTotalDestinosCopias());
	}
	__finally
	{
		obj->clearDestinosCopias();
		assert(0 == obj->getTotalDestinosCopias());
	}
}

void CorreoTest::testRemoveDestinoOcultoIndex()
{
	const char* MAIL = "fulano@menagano.com";
	const int INSERTAR = 10;
	const DWORD total = obj->getTotalDestinosOcultos();

	obj->clearDestinosOcultos();
	assert(0 == obj->getTotalDestinosOcultos());

	try
	{
		for (int i=1; i<=INSERTAR; i++)
		{
			obj->addDestinoOculto(MAIL);
			assert(total + i == obj->getTotalDestinosOcultos());
		}
		assert(INSERTAR == obj->getTotalDestinosOcultos());

		for (int i=1; i<=INSERTAR; i++)
		{
			obj->removeDestinoOculto(0);
			assert(total + INSERTAR - i == obj->getTotalDestinosOcultos());
		}
		assert(0 == obj->getTotalDestinosOcultos());
	}
	__finally
	{
		obj->clearDestinosOcultos();
		assert(0 == obj->getTotalDestinosOcultos());
	}
}

void CorreoTest::testAddArchivoAdjunto()
{
	const char* FICH_ADJUNTO = "\\LibCorreoTest.exe";
	const int INSERTAR = 10;
	const DWORD total = obj->getTotalArchivosAdjuntos();

	try
	{
		for (int i=1; i<=INSERTAR; i++)
		{
			obj->addArchivoAdjunto(FICH_ADJUNTO);
			assert(total + i == obj->getTotalArchivosAdjuntos());
		}
		assert(INSERTAR == obj->getTotalArchivosAdjuntos());
	}
	__finally
	{
		obj->clearArchivosAdjuntos();
		assert(0 == obj->getTotalArchivosAdjuntos());
	}
}

void CorreoTest::testRemoveArchivoAdjunto()
{
	const char* FICH_ADJUNTO = "\\LibCorreoTest.exe";
	const int INSERTAR = 10;
	const DWORD total = obj->getTotalArchivosAdjuntos();

	try
	{
		// ojito, no debería poder insertarse dos veces el mismo adjunto,
		// porque a la hora de borrarlo por nombre, se comportará de forma
		// extraña (si insertamos 2 veces el mismo adjunto, al borrar el
		// primero, se borraran los dos).
		for (int i=1; i<=INSERTAR; i++)
		{
			obj->addArchivoAdjunto(FICH_ADJUNTO);
			assert(total + i == obj->getTotalArchivosAdjuntos());
		}
		assert(INSERTAR == obj->getTotalArchivosAdjuntos());

		for (int i=0; i<INSERTAR; i++)
		{
			obj->removeArchivoAdjunto(FICH_ADJUNTO);
			assert(total + INSERTAR - i == obj->getTotalArchivosAdjuntos());
		}
		assert(total == obj->getTotalArchivosAdjuntos());
	}
	__finally
	{
		obj->clearArchivosAdjuntos();
		assert(0 == obj->getTotalArchivosAdjuntos());
	}
}


void CorreoTest::testPrioridad()
{
	assert(obj->getPrioridad() == Correo::PrioridadNormal);

	obj->setPrioridad(Correo::PrioridadBaja);
	try
	{
		assert(obj->getPrioridad() == Correo::PrioridadBaja);

		obj->setPrioridad(Correo::PrioridadAlta);
		assert(obj->getPrioridad() == Correo::PrioridadAlta);
	}
	__finally {
		obj->setPrioridad(Correo::PrioridadNormal);
	}
}


// JM
// 19/01/2003
void CorreoTest::testNombreCorreo()
{
	const ArsCadena MAIL(ParametrosTest::GetConfiguracion()->GetValor("destinonombre"));
	const ArsCadena NOMBRE(ParametrosTest::GetConfiguracion()->GetValor("nombre"));
	const ArsCadena DIRECCION(ParametrosTest::GetConfiguracion()->GetValor("direccion"));
	ArsCadena  tmp;

	// nombre y dirección
	assert( NOMBRE == Correo::getNombreCorreo(MAIL) );
	assert( DIRECCION == Correo::getDireccionCorreo(MAIL) );

	// dirección sin nombre
	tmp = "<";
	tmp += DIRECCION;
	tmp += ">";
	assert( tmp == Correo::getCadenaCorreo(DIRECCION) );
}


Test* CorreoTest::suite()
{
	typedef struct _Entrada
	{
		char *nombre;
		void  (CorreoTest::*metodo)();
	} Entrada;

	static Entrada pruebas[] =
	{
			  {"testAsunto", &CorreoTest::testAsunto}
			, {"testCuerpo", &CorreoTest::testCuerpo}
			, {"testCuerpoAjustado", &CorreoTest::testCuerpoAjustado}
			, {"testAddDestino", &CorreoTest::testAddDestino}
			, {"testAddDestinoCopia", &CorreoTest::testAddDestinoCopia}
			, {"testAddDestinoOculto", &CorreoTest::testAddDestinoOculto}
			, {"testRemoveDestinoName", &CorreoTest::testRemoveDestinoName}
			, {"testRemoveDestinoCopiaName", &CorreoTest::testRemoveDestinoCopiaName}
			, {"testRemoveDestinoOcultoName", &CorreoTest::testRemoveDestinoOcultoName}
			, {"testRemoveDestinoIndex", &CorreoTest::testRemoveDestinoName}
			, {"testRemoveDestinoCopiaIndex", &CorreoTest::testRemoveDestinoCopiaIndex}
			, {"testRemoveDestinoOcultoIndex", &CorreoTest::testRemoveDestinoOcultoIndex}
			, {"testAddArchivoAdjunto", &CorreoTest::testRemoveDestinoCopiaIndex}
			, {"testRemoveArchivoAdjunto", &CorreoTest::testRemoveDestinoOcultoIndex}
			, {"testPrioridad", &CorreoTest::testPrioridad}
			, {"testNombreCorreo", &CorreoTest::testNombreCorreo}
	};

	int TotalEntradas = sizeof(pruebas)/sizeof(Entrada);
	TestSuite *suite = new TestSuite("Correo");

	for (int i=0; i<TotalEntradas; i++)
		suite->addTest( new CorreoTestCaller(pruebas[i].nombre, pruebas[i].metodo) );

	return suite;
}


