//--------------------------------------------------------------------------------------
#include "../h/Correo.h"
//--------------------------------------------------------------------------------------

Correo::Correo()
{
	prioridad = PrioridadNormal;
}

Correo::~Correo()
{
	// liberar memoria de los adjuntos
	clearArchivosAdjuntos();
}


void Correo::addDestino(TipoDestino donde, const ArsCadena &quien)
{
	destinos[donde].push_back(quien);
}


void Correo::removeDestino(TipoDestino donde, const ArsCadena &quien)
{
    for (ListaDestinatariosIterator it = destinos[donde].begin();
         it != destinos[donde].end(); ++it)
	{
		if ((*it) == quien)
        {
			destinos[donde].erase(it);
            break;
        }
    }
}


void Correo::removeDestino(TipoDestino donde, int index)
{
	destinos[donde].erase(&destinos[donde][index]);
}


void Correo::clearDestinos(TipoDestino donde)
{
	destinos[donde].clear();
}


DWORD Correo::getTotalDestinos(TipoDestino donde) const
{
	return destinos[donde].size();
}


void Correo::addArchivoAdjunto(const ArsCadena &fichero)
{
	adjuntos.push_back( new ArchivoAdjunto(fichero) );
}


void Correo::removeArchivoAdjunto(const ArsCadena &fichero)
{
    for (ListaArchivosAdjuntosIterator it = adjuntos.begin();
         it != adjuntos.end(); ++it)
	{
		if ((*it)->getFilename() == fichero)
        {
			delete (*it);
			adjuntos.erase(it);
            break;
        }
    }
}


void Correo::clearArchivosAdjuntos()
{
    for (ListaArchivosAdjuntosIterator it = adjuntos.begin();
         it != adjuntos.end(); ++it)
	{
		delete (*it);
    }

	adjuntos.clear();
}


DWORD Correo::getTotalArchivosAdjuntos() const
{
	return adjuntos.size();
}


int Correo::leerArchivosAdjuntos()
{
	int ret = 0;
    
    for (ListaArchivosAdjuntosIterator it = adjuntos.begin();
         it != adjuntos.end(); ++it)
	{
		if ( ERROR_READ_DATA == (*it)->readData() )
        	ret = -1;
    }

    return (ret);
}


ArsCadena Correo::getCadenaDestinos(TipoDestino donde) const
{
	ArsCadena ret;

    for (ListaDestinatariosIterator it = (ArsCadena *) destinos[donde].begin();
         it != destinos[donde].end(); ++it)
	{
		if (it != destinos[donde].begin())
        	ret += ",";

		ret += getCadenaCorreo(*it);
    }

    return ret;
}


char Correo::getNumeroPrioridad() const
{
	switch (prioridad)
    {
    	case PrioridadBaja:
        	return '5';

        case PrioridadNormal:
        	return '3';

        case PrioridadAlta:
        	return '1';

        default:
        	return '3';
	}
}


TextoMensaje Correo::getCuerpoAjustado() const
{
	const TextoMensaje &tmp = this->getCuerpo();
	const int len = tmp.length();
	TextoMensaje ret;
	int chrs, ini, esp, i;
	bool isR = false;
	char c;

	// trocear a líneas de ANCHO_LINEA caracteres, ya que algunos servidores
	// SMTP no soportan longitudes de línea muy grandes.
	// Cuidado con no romper las palabras
	ini  = 0;
    chrs = 0;
    i	 = 0;
    esp = -1;

    while (i < len)
    {
		c = tmp[i];

        if (c == '\r')
            isR = true;
        else if ( chrs >= ANCHO_LINEA || (isR && c == '\n') )
        {
        	if (isR)
                chrs++;
			else if (esp != -1)
            	chrs = esp - ini;

			ret += tmp.substr(ini, chrs+1);

            if (isR)
            	isR = false;
            else
                ret += "\r\n";

            ini += chrs + 1;
			i = ini;
            esp = -1;
			chrs = 0;
		}
		else if (c == ' ')
		{
			esp = i;
			chrs++;
		}
		else
			chrs++;

		i++;
	}

	// esto ya no hace falta
	//	ret += '\n';

	//
	// JM - 14/01/2004
	// Corregido porque no se añadía la parte final de la cadena
	//
	ret += tmp.substr(ini, chrs+1);

	return (ret);
}


ArsCadena Correo::getNombreCorreo(const ArsCadena &correo)
{
	ArsCadena  ret;
	char	   *ptr;

	ptr = strchr(correo.cadena(), '<');
	if (ptr)
	{
		int pos = ptr - correo.cadena() - 1;
		ret = correo.subCadena(0, pos);
		ret.quitarEspaciosDerecha();
	}

	return (ret);
}


ArsCadena Correo::getDireccionCorreo(const ArsCadena &correo)
{
	ArsCadena  ret;
	char	   *ptr;

	ptr = strchr(correo.cadena(), '<');
	if (ptr)
	{
		int pos = ptr - correo.cadena() + 1;
		ret = correo.subCadena(pos, correo.longitud() - 2);
		ret.quitarEspaciosIzquierda();
	}

	return (ret);
}


ArsCadena Correo::getCadenaCorreo(const ArsCadena &correo)
{
	ArsCadena  ret;

	if ( strchr(correo.cadena(), '<') )
		ret.asignar(correo.cadena());
	else
		ret.asignar("<%s>", correo.cadena());

	return (ret);
}

