#include <vcl.h>
#pragma hrdstop

#include "ArchivoAdjuntoTest.h"
#include "../h/ArchivoAdjunto.h"

#include "TestSuite.h"

#include "ParametrosTest.h"

ArchivoAdjuntoTest::ArchivoAdjuntoTest(std::string name)
	: TestCase(name)
{
}


ArchivoAdjuntoTest::~ArchivoAdjuntoTest()
{
}


void ArchivoAdjuntoTest::setUp()
{
	obj = new ArchivoAdjunto();
}


void ArchivoAdjuntoTest::tearDown()
{
	delete obj;
}


Test *ArchivoAdjuntoTest::suite()
{
	typedef struct _Entrada
	{
		char *nombre;
		void  (ArchivoAdjuntoTest::*metodo)();
	} Entrada;

	static Entrada pruebas[] =
	{
			{"testSize", &ArchivoAdjuntoTest::testSize},
			{"testFilename", &ArchivoAdjuntoTest::testFilename},
			{"testConstructorFilename", &ArchivoAdjuntoTest::testConstructorFilename},
			{"testReadTextData", &ArchivoAdjuntoTest::testReadTextData},
			{"testReadZeroTextData", &ArchivoAdjuntoTest::testReadZeroTextData},
			{"testReadDataNoExiste", &ArchivoAdjuntoTest::testReadDataNoExiste},
			{"testEncode", &ArchivoAdjuntoTest::testEncode}
	};

	int TotalEntradas = sizeof(pruebas)/sizeof(Entrada);
	TestSuite *suite = new TestSuite("ArchivoAdjunto");

	for (int i=0; i<TotalEntradas; i++)
		suite->addTest( new ArchivoAdjuntoTestCaller(pruebas[i].nombre, pruebas[i].metodo) );

	return suite;
}



void ArchivoAdjuntoTest::testSize()
{
	ArsCadena ruta = ParametrosTest::GetConfiguracion()->GetValor("ruta") + "adjunto2.zip";
	const char *SRC_NAME = ruta.cadena();

	DWORD size, size2;
	HANDLE fich;

	SetCurrentDirectory(ExtractFilePath(Application->ExeName).c_str());

	//
	// leo el fichero y me guardo su tamaño
	//
	fich = CreateFile(SRC_NAME, GENERIC_READ, 0, NULL,
				OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	assert(INVALID_HANDLE_VALUE != fich);

	try
	{
	size = GetFileSize(fich, NULL);
	assert(0xFFFFFFFF != size);
	}
	__finally {
		CloseHandle(fich);
	}

	//
	// añado el adjunto, lo leo y compruebo el tamaño
	//
	obj->setFilename(SRC_NAME);
	size2 = obj->readData();
	try
	{
		assert(obj->getSize() == size2);
		assert(obj->getSize() == size);
		assert(size2 == size);
	}
	__finally {
		obj->freeData();
		assert(obj->getSize() == 0);
	}
}


void ArchivoAdjuntoTest::testFilename()
{
	obj->setFilename("c:\\ruta\\ruta con espacios\\fichero.ext");
	assert(obj->getName().igual("fichero.ext"));
	assert(obj->getPath().igual("c:\\ruta\\ruta con espacios\\"));
}


void ArchivoAdjuntoTest::testConstructorFilename()
{
	ArchivoAdjunto	*obj2;

	obj2 = new ArchivoAdjunto("c:\\ruta\\ruta\\fichero.ext");
	try
	{
		assert(obj2->getName() == "fichero.ext");
		assert(obj2->getPath() == "c:\\ruta\\ruta\\");
	}
	__finally {
		delete obj2;
	}
}

void ArchivoAdjuntoTest::testReadTextData()
{
	DWORD guardado;
	const char *FILENAME = "C:\\test_jm.tmp";
	const char *DATA = "este es el contenido de mi archivo, un texto normal y corriente que debe ser guardado y leido cienb.";
	const unsigned int LEN = strlen(DATA);

	HANDLE fich = CreateFile(FILENAME, GENERIC_WRITE, 0, NULL,
							CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	assert(INVALID_HANDLE_VALUE != fich);

	try
	{
		try
		{
			WriteFile(fich, DATA, LEN, &guardado, NULL);
			assert(LEN == guardado);
		}
		__finally {
			CloseHandle(fich);
		}

		obj->setFilename(FILENAME);
		DWORD size = obj->readData();
		try
		{
			assert(size != 0XFFFFFFFF);
			assert(size == obj->getSize());
			assert(size == LEN);
			assert(CompareMem((void*) DATA, (void*) obj->getData(), LEN));
		}
		__finally
		{
			obj->freeData();
			assert(obj->getSize() == 0);
			assert(obj->getData() == NULL);
		}
	}
	__finally {
		DeleteFile(FILENAME);
	}
}

void ArchivoAdjuntoTest::testReadZeroTextData()
{
	DWORD guardado;
	const char *filename = "C:\\test_jm.tmp";

	HANDLE fich = CreateFile(filename,	GENERIC_WRITE, 0, NULL,
							CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	assert(INVALID_HANDLE_VALUE != fich);

	CloseHandle(fich);
	try
	{
		obj->setFilename(filename);
		DWORD size = obj->readData();
		try
		{
			assert(size != 0XFFFFFFFF);
			assert(size == obj->getSize());
			assert(size == 0);
			assert(obj->getData() == NULL);
		}
		__finally
		{
			obj->freeData();
			assert(obj->getSize() == 0);
			assert(obj->getData() == NULL);
		}
	}
	__finally {
		DeleteFile(filename);
	}
}


void ArchivoAdjuntoTest::testReadDataNoExiste()
{
	obj->setFilename("c:\\este_fichero_no_debe_existir.zzz");
	int ret = obj->readData();

	assert(ret == ERROR_READ_DATA);
	assert(obj->getSize() == 0);
}


void ArchivoAdjuntoTest::testEncode()
{
	// utilizo una cadena que previamente he codificado desde Outlook para comparar
	// con la que voy a codificar ahora.
	ArsCadena ruta = ParametrosTest::GetConfiguracion()->GetValor("ruta") + "texto.zip";
	const char *SRC_NAME = ruta.cadena();

	ArsCadena ruta1 = ParametrosTest::GetConfiguracion()->GetValor("ruta") + "zip_outlook.txt";
	const char *OK_NAME = ruta1.cadena();

	DWORD size, leido = 0;
	BYTE *src;
	char *ok, *tmp;
	HANDLE fich;
	BufferSalida serial;

	SetCurrentDirectory(ExtractFilePath(Application->ExeName).c_str());

	//
	// leo el fichero que contiene la cadena correcta.
	//
	fich = CreateFile(OK_NAME, GENERIC_READ, 0, NULL,
					OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	assert(INVALID_HANDLE_VALUE != fich);

	try
	{
		size = GetFileSize(fich, NULL);
		assert(0xFFFFFFFF != size);

		ok = new char[size+1];
		ok[size] = '\0';
		tmp = ok;
		while (tmp < ok + size)
		{
			assert( ReadFile(fich, tmp, 512, &leido, NULL) );

			if (leido == 0)
				tmp = (char*)0xFFFFFFFF;
			else
				tmp += leido;
		}
	}
	__finally {
		CloseHandle(fich);
	}

	//
	// leo el fichero origen y lo codifico
	//
	obj->setFilename(SRC_NAME);

	obj->readData();
	assert(obj->getSize() != 0XFFFFFFFF);

	serial = obj->encode();

	char *nuevo = new char[size+1];
	try
	{
		nuevo[size] = '\0';

		CopyMemory(nuevo, serial.c_str(), size);

		assert( lstrcmp(ok, nuevo) == 0 );
	}
	__finally
	{
		delete[] ok;
		delete[] nuevo;
	}
}



