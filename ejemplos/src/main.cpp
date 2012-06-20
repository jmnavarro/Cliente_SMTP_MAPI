//--------------------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "main.h"

#include "../h/ServidorSMTP.h"
#include "../h/ServidorMAPI.h"
#include "../h/ServidoresDisponibles.h"

#include "../../LibConfiguracion/h/ConfiguracionFactory.h"
//--------------------------------------------------------------------------------------

#define FICHERO_CFG  ".\\correo.ini"

//--------------------------------------------------------------------------------------

#pragma package(smart_init)
#pragma resource "*.dfm"
TMainForm *MainForm;

//--------------------------------------------------------------------------------------
__fastcall TMainForm::TMainForm(TComponent* Owner)
	: TForm(Owner)
{
}

//--------------------------------------------------------------------------------------
bool TMainForm::crearObjetos()
{
	ArsCadena seccion;

	liberarObjetos();

	if (cb_tipo->ItemIndex == 0)
		seccion = "[smtp]";
	else
		seccion = "[mapi]";

	SetCurrentDirectory(ExtractFilePath(Application->ExeName).c_str());

	m_cfgData = ConfiguracionFactory::CreateConfiguracion(FICHERO_CFG);
	m_cfg     = ConfiguracionFactory::CreateConfiguracionModulo(seccion.cadena(), m_cfgData);

	m_srv = ServidoresDisponibles::CrearServidor(m_cfg);

	return (m_srv != NULL);
}

bool TMainForm::liberarObjetos()
{
	if (m_cfg != NULL)
	{
		delete m_cfg;
		m_cfg = NULL;
	}

	if (m_cfgData != NULL)
	{
		delete m_cfgData;
		m_cfgData = NULL;
	}

	if (m_srv != NULL)
	{
		delete m_srv;
		m_srv = NULL;
	}

	return true;
}

bool TMainForm::cargarDatosConfiguracion()
{
	// obtener el objeto configuracion
	if ( cb_tipo->ItemIndex == 0)
	{
		e_host->Text   = m_cfg->GetValor(CLAVE_SMTP_SERVIDOR).cadena();
		e_puerto->Text = m_cfg->GetValor(CLAVE_SMTP_PUERTO).cadena();
	}
	else
	{
		e_perfil->Text = m_cfg->GetValor(CLAVE_MAPI_PERFIL).cadena();
		e_clave->Text  = m_cfg->GetValor(CLAVE_SMTP_CLAVE).cadena();
	}

	return true;
}


bool TMainForm::vaciarDatosCorreo()
{
	m_correo.setRemitente("");

	m_correo.clearDestinos();
	m_correo.clearDestinosCopias();
	m_correo.clearDestinosOcultos();

	m_correo.setAsunto("");

	m_correo.clearArchivosAdjuntos();

	m_correo.setCuerpo("");

	m_correo.setPrioridad(Correo::PrioridadNormal);

	return true;
}


//
// se "mete" en los objetos del sistema todos los datos del interfaz de usuario
//
bool TMainForm::cargarDatosCorreo()
{
	m_correo.setRemitente(e_de->Text.c_str());

	TStringList	*lista = new TStringList();
	try
	{
		// destinatarios normales
		if (getListaDestinos(lista, e_para) > 0)
			for (int i=0; i<lista->Count; i++)
				m_correo.addDestino(lista->Strings[i].c_str());

		// destinatarios copiados
		if (getListaDestinos(lista, e_copia) > 0)
			for (int i=0; i<lista->Count; i++)
				m_correo.addDestinoCopia(lista->Strings[i].c_str());

		// destinatarios ocultos
		if (getListaDestinos(lista, e_oculto) > 0)
			for (int i=0; i<lista->Count; i++)
				m_correo.addDestinoOculto(lista->Strings[i].c_str());
	}
	__finally {
		delete lista;
	}

	m_correo.setAsunto(e_asunto->Text.c_str());

	for (int i=0; i<lb_adjuntos->Items->Count; i++)
		m_correo.addArchivoAdjunto(lb_adjuntos->Items->Strings[i].c_str());

	m_correo.setCuerpo(e_texto->Lines->Text.c_str());

	Correo::TipoPrioridad	prioridad;
	switch (cb_prioridad->ItemIndex)
	{
		case 0:
			prioridad = Correo::PrioridadBaja;
			break;

		case 1:
			prioridad = Correo::PrioridadNormal;
			break;

		case 2:
			prioridad = Correo::PrioridadAlta;
			break;
	}
	m_correo.setPrioridad(prioridad);

	return true;
}

bool TMainForm::enviarCorreo()
{
	int  ret;
	AnsiString msg;

	ret = m_srv->enviar(m_correo);

	if (ret != SMTPERR_OK && ret != MAPIERR_OK)
		msg.sprintf("Se ha producido un error en el envío: %d", ret);
	else
		msg = "Correo enviado correctamente";

	MessageBox(Handle, msg.c_str(), "Enviar", (ret == 0) ? MB_ICONINFORMATION : MB_ICONERROR);

	return (ret == 0);
}


DWORD TMainForm::getListaDestinos(TStringList *lista, TEdit *edit)
{
	AnsiString aux(edit->Text);
	int i;

	lista->Clear();

	while(aux.Length() > 0)
	{
		i = aux.Pos(",");
		if (i > 0)
		{
			lista->Add(aux.SubString(1, i).Trim());
			aux.Delete(1, i);
			aux.Trim();
		}
		else
		{
			lista->Add(aux);
			aux = "";
		}
	}

	return lista->Count;
}

void __fastcall TMainForm::FormCreate(TObject *Sender)
{
	cb_prioridad->ItemIndex = 1;
	cb_tipo->ItemIndex = 0;
	cb_tipoChange(cb_tipo);
}
//--------------------------------------------------------------------------------------

void __fastcall TMainForm::b_enviarClick(TObject *Sender)
{
	Screen->Cursor = crHourGlass;
	try
	{
		vaciarDatosCorreo();
		cargarDatosCorreo();

		enviarCorreo();
	}
	__finally {
		Screen->Cursor = crDefault;
	}
}
//--------------------------------------------------------------------------------------

void __fastcall TMainForm::SpeedButton1Click(TObject *Sender)
{
	if ( od->Execute() )
		lb_adjuntos->Items->Add(od->FileName);
}
//--------------------------------------------------------------------------------------

void __fastcall TMainForm::SpeedButton2Click(TObject *Sender)
{
	if (lb_adjuntos->ItemIndex != -1)
	{
		lb_adjuntos->Items->Delete(lb_adjuntos->ItemIndex);
	}
}
//--------------------------------------------------------------------------------------

void __fastcall TMainForm::cb_tipoChange(TObject *Sender)
{
	crearObjetos();

	GroupBox1->Visible = (cb_tipo->ItemIndex == 0);
	GroupBox2->Visible = (cb_tipo->ItemIndex == 1);

	cargarDatosConfiguracion();
}
//--------------------------------------------------------------------------------------

void __fastcall TMainForm::FormDestroy(TObject *Sender)
{
	liberarObjetos();
}
//--------------------------------------------------------------------------------------

