//+++============================================================================
// CorreoTest
//
// Descripcion:
//	Define la clase CoreoTest, que realiza las pruebas unitarias
//	de la clase Correo.
//
// Autor   Fecha       Observaciones
// ------  ------      ------------------------------------------------------------
// JM      17/10/2003  Versión inicial
//
//==============================================================================
#ifndef CORREO_TEST_H
#define	CORREO_TEST_H

#include "TestCase.h"
#include "TestCaller.h"

class Correo;

class CorreoTest : public TestCase
{
public:
	CorreoTest(std::string name);
	virtual ~CorreoTest();

	void setUp();
	void tearDown();

	static Test *suite();

protected:
	void testAsunto();
	void testCuerpo();
	void testCuerpoAjustado();
	void testAddDestino();
	void testAddDestinoCopia();
	void testAddDestinoOculto();
	void testRemoveDestinoName();
	void testRemoveDestinoCopiaName();
	void testRemoveDestinoOcultoName();
	void testRemoveDestinoIndex();
	void testRemoveDestinoCopiaIndex();
	void testRemoveDestinoOcultoIndex();
	void testAddArchivoAdjunto();
	void testRemoveArchivoAdjunto();
	void testPrioridad();
	void testNombreCorreo();
private:
	Correo	*obj;
};

typedef TestCaller<CorreoTest> CorreoTestCaller;

#endif
