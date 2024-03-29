/* Generated by Together */

#ifndef SERVIDORMAPI_H
#define SERVIDORMAPI_H
#include "ServidorEnvio.h"

class ServidorMAPI : public ServidorEnvio {
public:

    static ServidorEnvio * Crear();

    virtual void conectar();

    virtual void desconectar();

    void enviar();

    static void isMapiInstalled();

    bool loadMapiDLL();

    bool unloadMapiDLL();
private:    
    int profile;
    int password;
};
#endif //SERVIDORMAPI_H