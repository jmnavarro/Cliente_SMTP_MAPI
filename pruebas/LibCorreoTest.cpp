//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "ParametrosTest.h"

#include "ITestRunner.h"
#include "ArchivoAdjuntoTest.h"
#include "CorreoTest.h"
#include "ServidorSMTPTest.h"
#include "ServidorMAPITest.h"
#include "ServidoresDisponiblesTest.h"
//---------------------------------------------------------------------------


WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	try
	{
		ITestRunner runner;

		runner.addTest( ArchivoAdjuntoTest::suite() );
		runner.addTest( CorreoTest::suite() );
		runner.addTest( ServidorSMTPTest::suite() );
		runner.addTest( ServidorMAPITest::suite() );
		runner.addTest( ServidoresDisponiblesTest::suite() );

		runner.run ();
	}
	catch (Exception &exception) {
		Application->ShowException(&exception);
	}
	catch (...)
	{
		try {
			throw Exception("");
		}
		catch (Exception &exception) {
			Application->ShowException(&exception);
		}
	}

	ParametrosTest::terminate();
	
	return 0;
}
//---------------------------------------------------------------------------
