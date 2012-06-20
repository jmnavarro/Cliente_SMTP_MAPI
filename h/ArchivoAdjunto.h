/**
 * Define la clase ArchivoAdjunto, la cual representa un archivo que se
 * puede enviar junto con un correo electrónico.
 *
 * @author JM
 *
 * @date 16/10/2003 - JM  @li Versión inicial
**/
#ifndef ARCHIVO_ADJUNTO_H
#define	ARCHIVO_ADJUNTO_H

#include <pshpack8.h>
#include <vector>
#include <windows.h>

#include "../../LibUtils/h/ArsCadena.h"
#include "../h/Base64.h"
//--------------------------------------------------------------------------------------

#define  ANCHO_LINEA_ADJUNTO  70

//
// Errores de lectura del adjunto
//
#define ERROR_READ_DATA	0xFFFFFFFF

//--------------------------------------------------------------------------------------

class ArchivoAdjunto
{

public:
	ArchivoAdjunto(const ArsCadena &fich);
	ArchivoAdjunto();

	virtual ~ArchivoAdjunto();

	// getters
	ArsCadena getName() const;
	ArsCadena getPath() const;
	DWORD getSize() const 			{ return size; }
	BYTE *getData() const			{ return data; }
	ArsCadena& getFilename() 		{ return filename; }

	void setFilename(const ArsCadena &value);

	/// Leer el archivo y los almacena en "data". Retorna el tamaño leído.
	DWORD readData();

	/// codifica el archivo en base64
	BufferSalida encode();

	/// libera el buffer "data"
	void freeData();

private:
	/// almacena el nombre y ruta del fichero origen
	ArsCadena filename;

	/// almacena el tamaño y contenido (si ha sido leído)
	DWORD size;
	BYTE *data;
};


typedef std::vector<ArchivoAdjunto*>  ListaArchivosAdjuntos;
typedef std::vector<ArchivoAdjunto*>::iterator  ListaArchivosAdjuntosIterator;


#include <poppack.h>
#endif
