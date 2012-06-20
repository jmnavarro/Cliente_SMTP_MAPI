//+++===================================================================================
// ArchivoAdjuntoTest
//
// Descripcion:
//	Define la clase ArchivoAdjuntoTest, que realiza las pruebas unitarias
//	de la clase ArchivoAdjunto.
//
// Autor   Fecha       Observaciones
// ------  ----------  -----------------------------------------------------------------
// JM      16/10/2003  Versión inicial
// JM	   06/11/2003  Añadido test para la existencia del archivo en disco
//
//======================================================================================
#ifndef ARCHIVO_ADJUNTO_TEST_H
#define	ARCHIVO_ADJUNTO_TEST_H


#include "../../LibUtils/h/ArsCadena.h"

#include "TestCase.h"
#include "TestCaller.h"


class ArchivoAdjunto;


class ArchivoAdjuntoTest : public TestCase
{
public:
	ArchivoAdjuntoTest(std::string name);
	virtual ~ArchivoAdjuntoTest();

	void setUp();
	void tearDown();

	static Test *suite();

protected:
	void testSize();
	void testFilename();
	void testConstructorFilename();
	void testReadTextData();
	void testReadZeroTextData();
	void testReadDataNoExiste();
	void testEncode();
private:
	ArchivoAdjunto	*obj;
};

typedef TestCaller<ArchivoAdjuntoTest> ArchivoAdjuntoTestCaller;

#endif