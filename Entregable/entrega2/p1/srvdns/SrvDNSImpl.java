package srvdns;

import java.rmi.RemoteException;
import java.rmi.server.UnicastRemoteObject;
//import java.util.concurrent.ArrayBlockingQueue;

// Import para trabajar con listas
//import java.util.List;
//import java.util.ArrayList;

/*
La clase SrvDNSImpl implementa el interfaz SrvDNSInterface:

obtenerValorNthDomNthTipoRec(): que será invocado desde un cliente RMI para obtener el número de consultas realizadas de un tipo de registro para un dominio concreto

obtenerNumeroDominios(): devuelve el numero de dominios para los cuales se pueden realizar consultas.

obtenerNumeroTiposRec(): devuelve el numero de tipos de registros con los cuales puede trabajar el servidor DNS.

obtenerNombreNthDom(): obtiene el nombre del dominio cuyo id le pasamos como argumento.

obtenerNombreNthTipoRec(): obtiene el nombre del tipo de registro cuyo id le pasamos como argumento.

*/

public class SrvDNSImpl extends UnicastRemoteObject implements SrvDNSInterface {
    private ContabilidadConsultas acct; // Objeto que registra la contabilidad de las consultas recibidas por el
                                        // servidor DNS
    public SrvDNSImpl(ContabilidadConsultas cc) throws RemoteException {
        super();
        this.acct = cc;
    }

    @Override
    public int obtenerValorNthDomNthTipoRec(int ndxdom, int ndxtipor) throws RemoteException {
        // Este método es invocado vía RMI desde el cliente
        // Devuelve el número de consultas recibidas por un dominio para un tipo de
        // registro

        // A RELLENAR:
        return acct.obtenerValorDominioTiporec(ndxdom, ndxtipor); // Llamada remota

    }

    @Override
    public int obtenerNumeroDominios() throws RemoteException {
        // Este método es invocado vía RMI desde el cliente
        // Devuelve el numero de dominios para los que responde consultas
        // el servidor DNS

        // A RELLENAR:
        return acct.obtenerNumeroDominios();
        
    }

    @Override
    public int obtenerNumeroTiposRec() throws RemoteException {
        // Este método es invocado vía RMI desde el cliente
        // Devuelve el numero de tipos distintos de registros para
        // los cuales es capaz de responder el servidor DNS

        // A RELLENAR:
        return acct.obtenerNumeroTiposRegistros();
        
    }

    @Override
    public String obtenerNombreNthDom(int ndxdom) throws RemoteException {
        // Este método es invocado vía RMI desde el cliente
        // Devuelve el nombre del dominio cuyo numero se pasa como argumento

        // A RELLENAR:
        return acct.obtenerNombreDominioNth(ndxdom);
        
    }

    @Override
    public String obtenerNombreNthTipoRec(int ndxrec) throws RemoteException {
        // Este método es invocado vía RMI desde el cliente
        // Devuelve el nombre del tipo de registro cuyo numero se pasa como argumento

        // A RELLENAR:
        return acct.obtenerNombreTipoRecNth(ndxrec);
        
    }
}
