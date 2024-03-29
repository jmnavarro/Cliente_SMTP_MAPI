/* Generated by Together */

#ifndef SERVIDORENVIO_H
#define SERVIDORENVIO_H
#include "Correo.h"
#include "ServidorMAPI.h"
#include "ServidorSMTP.h"
#include "Correo.h"

class ServidorEnvio {
public:

    static ServidorEnvio * Crear();

    void isConectado();

    virtual void conectar() =0;

    virtual void desconectar() =0;

    virtual int enviar(Correo) =0;

private:

    /** @link dependency */
    /*# Correo lnkCorreo; */
protected:
    void ServidorEnvio(ConfiguracionServidor p0);
    bool conectado;
protected:

protected:
};
#endif //SERVIDORENVIO_H