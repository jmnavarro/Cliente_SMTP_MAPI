//---------------------------------------------------------------------------
#ifndef ServidorSMTPTestH
#define ServidorSMTPTestH

#include "TestCase.h"
#include "TestCaller.h"
#include "../h/ServidorSMTP.h"
//---------------------------------------------------------------------------


class ServidorSMTPTest : public TestCase
{
public:
	ServidorSMTPTest(std::string name);
	virtual ~ServidorSMTPTest();

	void setUp();
	void tearDown();

	static Test *suite();

protected:
	void testHost();
	void testPort();
	void testUser();
	void testPassword();

	void testConectar();
	void testDesconectar();

	void testEnviarCorreo();
	void testEnviarCorreoSinRemitente();
	void testEnviarCorreoConexion();
	void testEnviarCorreoLogin();
	void testEnviarCorreoFaltaLogin();
	void testEnviarCorreoSinDestinos();
	void testEnviarCorreoConNombre();
	void testEnviarCorreoLargo();
	void testEnviarCorreoConAdjunto();
	void testEnviarCorreoConAdjuntoError();
	void testEnviarCorreoCon2Adjuntos();
	void testEnviarCorreoCon3Adjuntos();

private:
	ServidorSMTP	*obj;
};

typedef TestCaller<ServidorSMTPTest> ServidorSMTPTestCaller;

#endif
