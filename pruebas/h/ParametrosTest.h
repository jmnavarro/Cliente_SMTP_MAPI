//---------------------------------------------------------------------------
#ifndef ParametrosTestH
#define ParametrosTestH

#include "../../LibConfiguracion/h/Configuracion.h"


class Configuracion;


class ParametrosTest
{
public:
	static void terminate();

	static Configuracion* GetConfiguracion();

private:
	static Configuracion	*m_cfg;
	static Configuracion	*m_cfgData;
};
//---------------------------------------------------------------------------
#endif
