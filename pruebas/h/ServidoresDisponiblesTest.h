//---------------------------------------------------------------------------
#ifndef ServidoresDisponiblesH
#define ServidoresDisponiblesH

#include "TestCase.h"
#include "TestCaller.h"
#include "../h/ServidoresDisponibles.h"
//---------------------------------------------------------------------------


class ServidoresDisponiblesTest : public TestCase
{
public:
	ServidoresDisponiblesTest(std::string name);
	virtual ~ServidoresDisponiblesTest();

	void setUp();
	void tearDown();

	static Test *suite();

protected:
	void testServidorSMTP();
	void testServidorMAPI();

	void testEnvioDinamicoSMTP();
	void testEnvioDinamicoMAPI();
};

typedef TestCaller<ServidoresDisponiblesTest> ServidoresDisponiblesTestCaller;

#endif
