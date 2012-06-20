//---------------------------------------------------------------------------
#include "ParametrosTest.h"

#include "../../LibConfiguracion/h/ConfiguracionFactory.h"
#include "../../LibConfiguracion/h/ConfiguracionINI.h"
#include "../../LibConfiguracion/h/ConfiguracionModulo.h"

#include <vcl.h>
//---------------------------------------------------------------------------

#define FICHERO_INI ".\\data\\parametros.ini"

//---------------------------------------------------------------------------

Configuracion* ParametrosTest::m_cfg     = NULL;
Configuracion* ParametrosTest::m_cfgData = NULL;



Configuracion* ParametrosTest::GetConfiguracion()
{
	if (m_cfg == NULL)
	{
		SetCurrentDirectory(ExtractFilePath(Application->ExeName).c_str());

		m_cfgData = ConfiguracionFactory::CreateConfiguracion(FICHERO_INI);
		if (m_cfgData != NULL)
			m_cfg = ConfiguracionFactory::CreateConfiguracionModulo("[parametros]", m_cfgData);
		else
			MessageBox(GetActiveWindow(), "No se han encontrado el archivo de parámetros de las pruebas.", "Error", MB_ICONERROR);			
	}

	return (m_cfg);
}


void ParametrosTest::terminate()
{
	if (m_cfg != NULL)
		delete m_cfg;

	if (m_cfgData != NULL)
		delete m_cfgData;
}

