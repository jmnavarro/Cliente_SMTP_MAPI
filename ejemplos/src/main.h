//---------------------------------------------------------------------------
#ifndef mainH
#define mainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>

#include "../../LibConfiguracion/h/Configuracion.h"
#include "../h/ServidorEnvio.h"
#include "../h/Correo.h"
#include <Dialogs.hpp>


//---------------------------------------------------------------------------
class TMainForm : public TForm
{
__published:	// IDE-managed Components
	TLabel *Label2;
	TLabel *Label3;
	TLabel *Label4;
	TGroupBox *GroupBox1;
	TLabel *Label1;
	TLabel *Label5;
	TEdit *e_host;
	TEdit *e_puerto;
	TEdit *e_de;
	TEdit *e_para;
	TLabel *Label6;
	TEdit *e_copia;
	TEdit *e_oculto;
	TLabel *Label7;
	TLabel *Label8;
	TEdit *e_asunto;
	TListBox *lb_adjuntos;
	TLabel *Label9;
	TMemo *e_texto;
	TSpeedButton *SpeedButton1;
	TSpeedButton *SpeedButton2;
	TButton *b_enviar;
	TLabel *Label10;
	TComboBox *cb_prioridad;
	TOpenDialog *od;
	TLabel *Label11;
	TComboBox *cb_tipo;
	TGroupBox *GroupBox2;
	TLabel *Label12;
	TEdit *e_perfil;
	TLabel *Label13;
	TEdit *e_clave;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall b_enviarClick(TObject *Sender);
	void __fastcall SpeedButton1Click(TObject *Sender);
	void __fastcall SpeedButton2Click(TObject *Sender);
	void __fastcall cb_tipoChange(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
private:
	// los objetos del sistema.
	Configuracion	*m_cfg;
	Configuracion	*m_cfgData;
	ServidorEnvio	*m_srv;
	Correo			 m_correo;

	// funcionalidades básicas
	bool crearObjetos();
	bool liberarObjetos();

	bool cargarDatosConfiguracion();
	bool vaciarDatosCorreo();
	bool cargarDatosCorreo();
	bool enviarCorreo();

	// auxiliares
	DWORD getListaDestinos(TStringList *lista, TEdit *edit);

public:		// User declarations
	__fastcall TMainForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TMainForm *MainForm;
//---------------------------------------------------------------------------
#endif
