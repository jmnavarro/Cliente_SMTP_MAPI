/**
 * Implementa la clase Correo, que representa un correo electrónico.
 *
 * @author JM
 *
 * @date 16/10/2003 - JM  @li Versión inicial
 * @date 06/11/2003 - JM  @li Se añade el método "leerArchivosAdjuntos" para recuperar de disco los datos de todos los adjuntos de un correo.
 * @date 14/01/2004 - JM  @li Corregido bug en getCuerpoAjustado.
 * @date 19/01/2004 - JM  @li Añadidos métodos estáticos para obtener el nombre o dirección a partir de una dirección de correo.
**/
#ifndef CorreoH
#define CorreoH

#include <pshpack8.h>
	#include <vector>
	#include <rope>
#include <poppack.h>

#include "../../LibUtils/h/ArsCadena.h"

#include "ArchivoAdjunto.h"
//--------------------------------------------------------------------------------------

#define  ANCHO_LINEA  ANCHO_LINEA_ADJUNTO


typedef std::vector<ArsCadena> ListaDestinatarios;
typedef std::vector<ArsCadena>::iterator ListaDestinatariosIterator;

typedef std::rope<char> TextoMensaje;

//--------------------------------------------------------------------------------------

class Correo
{
public:
	typedef enum
	{
		PrioridadNormal,
		PrioridadBaja,
		PrioridadAlta
	} TipoPrioridad;

public:
	Correo();
	virtual ~Correo();

	// tratamiento de los tres tipos de destinos: normal, con copia y oculto
	void addDestino(const ArsCadena &value) 	 	{ addDestino(DestinoNormal, value); }
	void addDestinoCopia(const ArsCadena &value)   	{ addDestino(DestinoCopia,  value); }
	void addDestinoOculto(const ArsCadena &value)  	{ addDestino(DestinoOculto, value); }

	void removeDestino(const ArsCadena &value)	 	{ removeDestino(DestinoNormal, value); }
	void removeDestino(int index) 		 			{ removeDestino(DestinoNormal, index); }
	void removeDestinoCopia(const ArsCadena &value)	{ removeDestino(DestinoCopia,  value); }
	void removeDestinoCopia(int index)				{ removeDestino(DestinoCopia,  index); }
	void removeDestinoOculto(const ArsCadena &value){ removeDestino(DestinoOculto, value); }
	void removeDestinoOculto(int index)	  			{ removeDestino(DestinoOculto, index); }

	void clearDestinos()		{ clearDestinos(DestinoNormal); }
	void clearDestinosCopias() 	{ clearDestinos(DestinoCopia); }
	void clearDestinosOcultos() { clearDestinos(DestinoOculto); }

	DWORD getTotalDestinos() 		const { return getTotalDestinos(DestinoNormal); }
	DWORD getTotalDestinosCopias()  const { return getTotalDestinos(DestinoCopia); }
	DWORD getTotalDestinosOcultos() const { return getTotalDestinos(DestinoOculto); }

	ArsCadena& getDestino(int index) 	   { return getDestino(DestinoNormal, index); }
	ArsCadena& getDestinoCopia(int index)  { return getDestino(DestinoCopia, index); }
	ArsCadena& getDestinoOculto(int index) { return getDestino(DestinoOculto, index); }

	ArsCadena getCadenaDestinos()		 const { return getCadenaDestinos(DestinoNormal); }
	ArsCadena getCadenaDestinosCopias()  const { return getCadenaDestinos(DestinoCopia); }
	ArsCadena getCadenaDestinosOcultos() const { return getCadenaDestinos(DestinoOculto); }

	// tratamiento de archivos adjuntos
	void addArchivoAdjunto(const ArsCadena &fichero);
	void removeArchivoAdjunto(const ArsCadena &fichero);
	void clearArchivosAdjuntos();
	DWORD getTotalArchivosAdjuntos() const;
	ArchivoAdjunto& getArchivoAdjunto(int i) const { return *adjuntos[i]; }
	int leerArchivosAdjuntos();

	// prioridad
	void setPrioridad(TipoPrioridad value)	{ this->prioridad = value; }
	TipoPrioridad getPrioridad() const		{ return this->prioridad; }
	char getNumeroPrioridad() const;

	// asunto
	void setAsunto(const ArsCadena &value)	{ this->asunto = value; }
	ArsCadena& getAsunto() 					{ return asunto; }

	// cuerpo
	void setCuerpo(LPCSTR value) 		  	{ this->cuerpo = value; }
	const TextoMensaje& getCuerpo() const 	{ return cuerpo; }
	TextoMensaje getCuerpoAjustado() const;

	// remitente
	void setRemitente(const ArsCadena &value) { this->remitente = value; }
	ArsCadena& getRemitente() 				  { return remitente; }

	// métodos estáticos para obtener el nombre y dirección de una dirección de correo
	static ArsCadena getNombreCorreo(const ArsCadena &correo);
	static ArsCadena getDireccionCorreo(const ArsCadena &correo);
	static ArsCadena getCadenaCorreo(const ArsCadena &correo);

protected:
	typedef enum
	{
		DestinoNormal = 0,
		DestinoCopia  = 1,
		DestinoOculto = 2
	} TipoDestino;

	ListaDestinatarios		destinos[3];
	ArsCadena				asunto;
	TextoMensaje			cuerpo;
	ListaArchivosAdjuntos	adjuntos;
	TipoPrioridad			prioridad;
	ArsCadena				remitente;

	void addDestino(TipoDestino donde, const ArsCadena &quien);
	void removeDestino(TipoDestino donde, const ArsCadena &quien);
	void removeDestino(TipoDestino donde, int index);
	void clearDestinos(TipoDestino donde);

	DWORD getTotalDestinos(TipoDestino donde) const;

	ArsCadena& getDestino(TipoDestino donde, int i) { return destinos[donde][i]; }

	ArsCadena getCadenaDestinos(TipoDestino donde) const;
};


#endif

