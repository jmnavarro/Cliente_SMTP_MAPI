#include "../h/ServidorMAPI.h"
#include "..\..\..\Base\LibUtilidades\h\Log.h"

//
// Constantes para el nombre de la DLL y las funciones a importar
//

// esto debería leerse de la clave del registro:
// HKEY_LOCAL_MACHINE/software/microsoft/Windows Messaging Subsystem/CMCDLLNAME32
#define MAPI_DLL "MAPI32.DLL"

#define MAPILogon	  "MAPILogon"
#define MAPILogoff	  "MAPILogoff"
#define MAPISendMail  "MAPISendMail"

//--------------------------------------------------------------------------------------

ServidorMAPI::ServidorMAPI()
	: ServidorEnvio()
{
	hDLL	= 0;
    hSesion = 0;

    pMAPILogon	  = NULL;
    pMAPILogoff	  = NULL;
    pMAPISendMail = NULL;

	loadMapiDLL();
}


ServidorMAPI::~ServidorMAPI()
{
	// Esto debería ir en el destructor de ServidorEnvio.
	// Como en el destructor no podemos poner llamadas a métodos polimórficos, es
    // necesario poner esta línea en el destructor de todos los descendientes de
    // ServidorEnvio, para que se ejecute la versión correcta de "desconectar".
	desconectar();

    unloadMapiDLL();
}


bool ServidorMAPI::loadMapiDLL()
{
	bool ret;

	if ( isMapiInstalled() )
    {
        // Cargar la DLL y los punteros a función
        hDLL = LoadLibrary(MAPI_DLL);

        ret = (hDLL != NULL);
        if (ret)
        {
            pMAPILogon 	  = (LPMAPILOGON) 	 GetProcAddress(hDLL, MAPILogon);
            pMAPILogoff   = (LPMAPILOGOFF) 	 GetProcAddress(hDLL, MAPILogoff);
            pMAPISendMail = (LPMAPISENDMAIL) GetProcAddress(hDLL, MAPISendMail);

            if ( !isMapiLoaded() )
            {
            	unloadMapiDLL();

                ERROR_ "Error cargando las funciones de MAPI" _ERROR

                ret = false;
            }
		}
        else
            ERROR_ "Error cargando la librería MAPI32.DLL" _ERROR
    }
    else
    	ret = false;

    return (ret);
}


bool ServidorMAPI::unloadMapiDLL()
{
	bool ret;

    if (hDLL)
    {
        ret = FreeLibrary(hDLL);
        if (ret)
        {
            hDLL = NULL;

            pMAPILogon 	  = NULL;
            pMAPILogoff   = NULL;
            pMAPISendMail = NULL;

            LOG_ "Librería MAPI32.DLL descargada correctamente" _LOG
        }
        else
            WARNING_ "La librería MAPI32.DLL no ha podido ser descargada" _WARNING
    }
    else
    	ret = false;

    return (ret);
}


bool ServidorMAPI::isMapiLoaded()
{
	return (hDLL 		  != NULL &&
			pMAPILogon 	  != NULL &&
			pMAPILogoff   != NULL &&
			pMAPISendMail != NULL);
}


int ServidorMAPI::enviar(Correo &correo)
{
	bool desconectar = false;
	MapiMessage	mensaje;
	ULONG		error;
	int			ret;

	// comprobaciones iniciales
	if ( !isMapiLoaded() )
	{
		ret = MAPIERR_CARGAR;
	}
	else
	{
		// comprobación de destinatarios
		if (correo.getTotalDestinos() +
			correo.getTotalDestinosCopias() +
			correo.getTotalDestinosOcultos() == 0)
		{
			ret = MAPIERR_SINDESTINOS;
		}
		else
		{
			ret = MAPIERR_OK;

			// autoconexión
			if ( !isConectado() )
			{
				if ( conectar() != CONECTAR_OK )
					ret = MAPIERR_CONECTAR;
				else
					desconectar = true;
			}

			if (ret == MAPIERR_OK)
			{
				// convertir una clase "Correo" a una estructura "MapiMessage"
				error = convertirCorreo(correo, mensaje);
				if (error != 0)
				{
					ret = error;
				}
				else
				{
					// enviar
					error = pMAPISendMail(hSesion, 0, &mensaje, 0, 0);

					// se liberan los recursos objtenidos desde "convertirCorreo"
					// (antes de retornar por un posible error)
					liberarMensaje(mensaje);

					if (error != SUCCESS_SUCCESS)
						ret = GetErrorFromMapi(error);
					else
						ret = MAPIERR_OK;
				}

				// desconectar
				if (desconectar)
				{
					if ( !this->desconectar() )
						ret = MAPIERR_DESCONECTAR;
				}
			}
		}
	}

	return (ret);
}


int ServidorMAPI::conectar()
{
	int	   ret = CONECTAR_ERROR;
	FLAGS  flags = 0;
	ULONG  hVentana;
	ULONG  error;
	LPSTR  profile;
	LPSTR  password;
	char   dirActivo[MAX_PATH];


	// JM - 15/09/2004
	// MAPI hace un cambio del directorio activo a donde esté instalado el cliente
	// de correo. Hay que asegurar que el directorio activo no cambia por el uso
	// de esta librería.
	GetCurrentDirectory(MAX_PATH, dirActivo);

	if ( !isMapiLoaded() )
	{
		ret = CONECTAR_MAPINOTLOADED;
	}
	else
	{
		if ( isConectado() )
			desconectar();

		if ( this->profile.longitud() > 0 )
		{
			profile  = this->profile.cadena();
			password = this->password.cadena();
		}
		else
		{
			profile  = NULL;
			password = NULL;

			// Se intenta abrir una nueva sesión del cliente, aunque ya esté abierto.
			// Además, se muestran posibles ventanas de login
			flags = MAPI_NEW_SESSION | MAPI_LOGON_UI;
		}

		// es posible que el cliente de correo muestre una ventana pidiendo el perfil y
		// contraseña a utilizar.
		hVentana = reinterpret_cast<ULONG> (::GetActiveWindow());


		// primero se intenta iniciar una sesión con estos parámetros
		error = pMAPILogon(hVentana, profile, password, flags, 0, &hSesion);

		if (error != SUCCESS_SUCCESS && error != MAPI_E_USER_ABORT)
		{
			// si falla al crear una nueva sesión, se intenta obtener una sesión compartida
			error = pMAPILogon(hVentana, NULL, NULL, 0, 0, &hSesion);
			if (error == SUCCESS_SUCCESS)
				ret = CONECTAR_OK;
			else {
				ERROR_ "No se puede iniciar una sesión compartida en MAPI" _ERROR
			}
		}
		else if (error == SUCCESS_SUCCESS)
			ret = CONECTAR_OK;

		// llamar al padre para que establezca el flag "conectado"
		if (ret == CONECTAR_OK)
			ServidorEnvio::conectar();
	}

	// restablecer el directorio activo
	SetCurrentDirectory(dirActivo);

	return (ret);
}


bool ServidorMAPI::desconectar()
{
	bool   ret = false;
	ULONG  error;

	if (!isMapiLoaded())
		return ret;

	if ( isConectado() && hSesion)
	{
		error = pMAPILogoff(hSesion, 0, 0, 0);

		if (error != SUCCESS_SUCCESS)
        	ERROR_ "Error al cerrar la sesión en MAPI: %d", error _ERROR
        else
        {
			ret = true;
	        hSesion = 0;
        }
    }

    if (ret)
    	ServidorEnvio::desconectar();

	return ret;
}



bool ServidorMAPI::isMapiInstalled()
{
	//
	// Para ver si MAPI está instalado tenemos que buscar la entrada
    // correspondiente en el "win.ini" y ver que la librería MAPI32.DLL existe.
    // Deberíamos pensar hacer la búsqueda en el registro, en
	// HKEY_LOCAL_MACHINE/software/microsoft/Windows Messaging Subsystem
    //
	bool ret = (::GetProfileInt("MAIL", "MAPI", 0) != 0) &&
    		   (::SearchPath(NULL, MAPI_DLL, NULL, 0, NULL, NULL) != 0);

    if (ret)
    	LOG__ "MAPI instalado correctamente en la máquina" __LOG
    else
    	WARNING__ "MAPI no está instalado correctamente" __WARNING;

	return (ret);
}


ULONG ServidorMAPI::convertirCorreo(Correo &correo, MapiMessage &mensaje) const
{
	int destinoActual = 0;
    int max;
    MapiRecipDesc *destino;
    ArsCadena	  direccion;

	ZeroMemory(&mensaje, sizeof(MapiMessage));
	mensaje.lpszSubject  = correo.getAsunto().cadena();
	mensaje.lpszNoteText = (char *) correo.getCuerpo().c_str();
	mensaje.nRecipCount  = correo.getTotalDestinos() +
    					   correo.getTotalDestinosCopias() +
                           correo.getTotalDestinosOcultos();

	// Hay que reservar un array dinámico de destinatarios.
	// Se liberará en la función "liberarMensaje"
	mensaje.lpRecips = new MapiRecipDesc[mensaje.nRecipCount];

	// destinos normales
	max = correo.getTotalDestinos();
	for (int i=0; i<max; i++)
	{
		destino = &mensaje.lpRecips[destinoActual];
	    ZeroMemory(destino, sizeof(MapiRecipDesc));
		destino->ulRecipClass = MAPI_TO;

		// en este punto podríamos intentar resolver el destino, es decir: convertirlo
        // de nombre normal a dirección de mail. Para ello podemos utilizar la
        // función de "MAPIResolve", que a su vez hace uso de la libreta de direcciones
        // para buscar las equivalencias. Por ahora pasamos del tema.
		destino->lpszName = correo.getDestino(i).cadena();

		++destinoActual;
	}

	// destinos con copia
    max = correo.getTotalDestinosCopias();
	for (int i=0; i<max; i++)
	{
		destino   = &mensaje.lpRecips[destinoActual];
	    ZeroMemory(destino, sizeof(MapiRecipDesc));
		destino->ulRecipClass = MAPI_CC;

        direccion = correo.getDestino(i);
		destino->lpszName = direccion.cadena();

		++destinoActual;
	}

	// destinos ocultos
    max = correo.getTotalDestinosOcultos();
	for (int i=0; i<max; i++)
	{
		destino   = &mensaje.lpRecips[destinoActual];
	    ZeroMemory(destino, sizeof(MapiRecipDesc));
		destino->ulRecipClass = MAPI_BCC;

        direccion = correo.getDestino(i);
		destino->lpszName = direccion.cadena();

		++destinoActual;
	}


	// Adjuntos
    mensaje.nFileCount = correo.getTotalArchivosAdjuntos();
	if ( mensaje.nFileCount > 0 )
	{
		MapiFileDesc *adjunto;

    	// esto lo liberará el método "liberarMensaje"
		mensaje.lpFiles = new MapiFileDesc[mensaje.nFileCount];

	    for (DWORD i=0; i<mensaje.nFileCount; i++)
		{
			adjunto = &mensaje.lpFiles[i];
			ZeroMemory(adjunto, sizeof(MapiFileDesc));

			adjunto->nPosition = 0xFFFFFFFF;
			adjunto->lpszPathName = correo.getArchivoAdjunto(i).getFilename().cadena();
		}
	}

	return 0;
}


void ServidorMAPI::liberarMensaje(MapiMessage &mensaje) const
{
	if (mensaje.lpFiles != NULL)
	{
		delete [] mensaje.lpFiles;
		mensaje.lpFiles = NULL;
	}

	delete [] mensaje.lpRecips;
	mensaje.lpRecips = NULL;
}


int ServidorMAPI::GetErrorFromMapi(ULONG errorMapi)
{
	int error;

	switch (errorMapi)
	{
		case MAPI_E_AMBIGUOUS_RECIPIENT:
			error = MAPIERR_AMBIGUOUS_RECIPIENT;
			break;

		case MAPI_E_ATTACHMENT_NOT_FOUND:
			error = MAPIERR_ATTACHMENT_NOT_FOUND;
			break;

		case MAPI_E_ATTACHMENT_OPEN_FAILURE:
			error = MAPIERR_ATTACHMENT_OPEN_FAILURE;
			break;

		case MAPI_E_BAD_RECIPTYPE:
			error = MAPIERR_BAD_RECIPTYPE;
			break;

		case MAPI_E_FAILURE:
			error = MAPIERR_FAILURE;
			break;

		case MAPI_E_INSUFFICIENT_MEMORY:
			error = MAPIERR_INSUFFICIENT_MEMORY;
			break;

		case MAPI_E_INVALID_RECIPS:
			error = MAPIERR_INVALID_RECIPS;
			break;

		case MAPI_E_LOGIN_FAILURE:
			error = MAPIERR_LOGIN_FAILURE;
			break;

		case MAPI_E_TEXT_TOO_LARGE:
			error = MAPIERR_TEXT_TOO_LARGE;
			break;

		case MAPI_E_TOO_MANY_FILES:
			error = MAPIERR_TOO_MANY_FILES;
			break;

		case MAPI_E_TOO_MANY_RECIPIENTS:
			error = MAPIERR_TOO_MANY_RECIPIENTS;
			break;

		case MAPI_E_UNKNOWN_RECIPIENT:
			error = MAPIERR_UNKNOWN_RECIPIENT;
			break;

		case MAPI_E_USER_ABORT:
			error = MAPIERR_USER_ABORT;
			break;

		default:
			error = MAPIERR_ENVIAR;
			break;
	}

	return (error);
}
