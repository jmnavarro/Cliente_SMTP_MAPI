/**
 *
 * Se define la clase ServidorMAPI, descendiente de ServidorEnvio
 *
**/
#ifndef ServidorMAPIH
#define ServidorMAPIH


#include "../../LibUtils/h/ArsCadena.h"
#include <mapi.h>

#include "ServidorEnvio.h"
#include "Correo.h"


//
// códigos de retorno de error del método "enviar"
//
#define  MAPIERR_OK			 	 		 ENVIO_OK
#define  MAPIERR_SINDESTINOS			 ENVIO_SINDESTINOS

#define  MAPIERR_CARGAR					 -1
#define  MAPIERR_CONECTAR				 -2
#define  MAPIERR_DESCONECTAR			 -3
#define  MAPIERR_ENVIAR					 -4
#define	 MAPIERR_AMBIGUOUS_RECIPIENT	 -5
#define  MAPIERR_ATTACHMENT_NOT_FOUND	 -6
#define  MAPIERR_ATTACHMENT_OPEN_FAILURE -7
#define  MAPIERR_BAD_RECIPTYPE			 -8
#define  MAPIERR_FAILURE				 -9
#define  MAPIERR_INSUFFICIENT_MEMORY	 -10
#define  MAPIERR_INVALID_RECIPS			 -11
#define  MAPIERR_LOGIN_FAILURE			 -12
#define  MAPIERR_TEXT_TOO_LARGE			 -13
#define  MAPIERR_TOO_MANY_FILES			 -14
#define  MAPIERR_TOO_MANY_RECIPIENTS	 -15
#define  MAPIERR_UNKNOWN_RECIPIENT		 -16
#define  MAPIERR_USER_ABORT				 -17

//
// códigos de error de "conectar"
//
#define CONECTAR_MAPINOTLOADED	-10


/**
 * La clase ServidorMAPI, descendiente de ServidorEnvio, que implementa
 * las operaciones propias para el envío de correos a través del protocolo MAPI.
 *
 * @author: JM
 *
 * @date 29/10/2003 - JM @li Versión inicial
 * @date 31/10/2003 - JM @li Eliminadas todas las referencias a objetos de configuración y añadidos los atributos "profile" y "password"
 * @date 15/09/2004 - JM @li Se restablece el directorio activo, ya que el envío con MAPI cambiada el directorio activo al del cliente de correo.
**/
class ServidorMAPI : public ServidorEnvio
{
public:
	ServidorMAPI();
	virtual ~ServidorMAPI();

	virtual int enviar(Correo &correo);

	virtual int  conectar();
	virtual bool desconectar();

	// getters y setters
	ArsCadena getProfile() const		 	 { return profile; }
	void setProfile(const ArsCadena &value)  { profile = value; }

	ArsCadena getPassword() const 			 { return password; }
	void setPassword(const ArsCadena &value) { password = value; }

	static bool isMapiInstalled();

protected:
	// cargar y descargar MAPI32.DLL
	bool loadMapiDLL();
	bool unloadMapiDLL();
	bool isMapiLoaded();

	/// convertir una clase "Correo" a estructura "MapiMessage"
	ULONG convertirCorreo(Correo &correo, MapiMessage &mensaje) const;
	void liberarMensaje(MapiMessage &mensaje) const;

	int GetErrorFromMapi(ULONG errorMapi);

private:
	ArsCadena	   profile;
	ArsCadena	   password;

	// punteros a funciones exportadas en MAPI32.DLL
	LPMAPILOGON  	pMAPILogon;
	LPMAPILOGOFF  	pMAPILogoff;
	LPMAPISENDMAIL	pMAPISendMail;

	HINSTANCE  hDLL;	///< handle de MAPI32.DLL. Se carga al crear un ServidorMAPI.
	LHANDLE	 hSesion;	///< handle de la sesión en MAPI. Se crea en el método "conectar".
};


#endif
