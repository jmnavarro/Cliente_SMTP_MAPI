#include "../h/ArchivoAdjunto.h"

#include <pshpack8.h>
	#include <rope>
#include <poppack.h>

//--------------------------------------------------------------------------------------


ArchivoAdjunto::ArchivoAdjunto(const ArsCadena &fich)
{
	size = 0;
	data = NULL;

	setFilename(fich);
}


ArchivoAdjunto::ArchivoAdjunto()
{
	size = 0;
	data = NULL;
}


ArchivoAdjunto::~ArchivoAdjunto()
{
	freeData();
}

//
// Leer el contenido del fichero y lo almacena en "data"
//
DWORD ArchivoAdjunto::readData()
{
	const DWORD CHUNK = 512; // !!

	// liberar recursos anteriores
	freeData();

	HANDLE fich = CreateFile(filename.cadena(),
				   			GENERIC_READ, FILE_SHARE_READ, NULL,
			                OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL);

    if (INVALID_HANDLE_VALUE == fich)
    	return ERROR_READ_DATA;

	// obtener el tamaño del archivo (hasta 2 GB)
	size = GetFileSize(fich, NULL);
	if (0xFFFFFFFF == size)
    {
		// no se puede averiguar el tamaño del archivo
		CloseHandle(fich);
		size = 0;
        return ERROR_READ_DATA;
    }
    else if (size > 0)
    {
        this->data = new BYTE[size];
        BYTE *tmp = data;
        DWORD leido = 0;

        while (tmp < data + size)
        {
            if (!ReadFile(fich, tmp, CHUNK, &leido, NULL))
            {
                // error leyendo en fichero
                CloseHandle(fich);
                freeData();
                return ERROR_READ_DATA;
            }
            if (leido == 0)
                tmp = reinterpret_cast<BYTE*> (0xFFFFFFFF);
            else
                tmp += leido;
        }
    }

	CloseHandle(fich);

    return size;
}


BufferSalida ArchivoAdjunto::encode()
{
	BufferSalida  ret;

    bool error = false;

	if (size == 0 || data == NULL)
    	readData();

	CBase64  *coder = new CBase64();
    try
    {
    	// los tiempo de ejecución de esto deberían revisarse.
		// Tarda 4 sg. para codificar un archivo de 70 KB.
        try {
            ret = coder->Encode(data, size);
        } catch(...) {
            error = true;
        }
    }
    __finally {
		delete coder;
    }

	if (!error)
    {
        // ahora se ajusta a un ancho de linea especificado
        const int l = ret.length();
        int max = l / ANCHO_LINEA_ADJUNTO;
        BufferSalida tmp;

        if (l % ANCHO_LINEA_ADJUNTO > 0)
        	max++;

        for (int i = 1; i <= max; ++i)
        {
            tmp += ret.substr((i-1) * ANCHO_LINEA_ADJUNTO, ANCHO_LINEA_ADJUNTO);
            if (i < max)
	            tmp += "\r\n";
        }
		// se retorna aquí por eficiencia, para evitar hacer toda la copia de los datos
        // desde "tmp" a "ret".
        return tmp;
    }

    return ret;
}


void ArchivoAdjunto::freeData()
{
	if (size > 0 && data)
    {
		delete[] data;
        data = NULL;
        size = 0;
    }
}


void ArchivoAdjunto::setFilename(const ArsCadena &value)
{
	ArsCadena aux;

	// si es ruta relativa, hay que convertila a absoluta.
	if (value[1] != ':')
    {
		char path[MAX_PATH], **name;
        char value_str[MAX_PATH];

        strcpy(value_str, value.cadena());
        name = NULL;

        if ( 0 == ::GetFullPathName(value_str, MAX_PATH, path, name))
			aux = value;
        else
			aux.asignar(path);
    }
	else
		aux = value;

    this->filename = aux;
}


//
// getters
//
ArsCadena ArchivoAdjunto::getName() const
{
	int len = filename.longitud();
	int pos;
	ArsCadena ret;

    for (pos=len-1; pos>0 && filename[pos] != '\\'; pos--)
    	;

	if (pos == 0)
    	ret = filename;
    else
		ret = filename.subCadena(pos + 1, len - 1);

    return ret;
}

ArsCadena ArchivoAdjunto::getPath() const
{
	int pos;
	ArsCadena ret;

    for (pos=filename.longitud()-1; pos>0 && filename[pos] != '\\'; pos--)
	    ;

	if (pos == 0)
    	ret = "";
    else
		ret = filename.subCadena(0, pos);

    return ret;
}

