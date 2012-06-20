//---------------------------------------------------------------------------
#ifndef ServidorMAPITestH
#define ServidorMAPITestH

#include "TestCase.h"
#include "TestCaller.h"
#include "../h/ServidorMAPI.h"
//---------------------------------------------------------------------------


class ServidorMAPITest : public TestCase
{
public:
	ServidorMAPITest(std::string name);
	virtual ~ServidorMAPITest();

	void setUp();
	void tearDown();

	static Test *suite();

protected:
	void testProfile();
	void testPassword();

	void testConectar();
	void testConectarSinConfiguracion();
	void testDesconectar();

	void testEnviarCorreo();
	void testEnviarCorreoConexion();
	void testEnviarCorreoSinDestinos();
	void testEnviarCorreoLargo();
	void testEnviarCorreoConAdjunto();
	void testEnviarCorreoCon2Adjuntos();
	void testEnviarCorreoCon3Adjuntos();


private:
	ServidorMAPI	*obj;
};

typedef TestCaller<ServidorMAPITest> ServidorMAPITestCaller;

#endif
