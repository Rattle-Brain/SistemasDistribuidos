package srvdns;

import java.rmi.Remote;
import java.rmi.RemoteException;

/*
Interfaz remoto del SrvDNS que ha de hacer público los metodos siguientes 
*/
public interface SrvDNSInterface extends Remote {
    public int obtenerValorNthDomNthTipoRec(int nthdom, int nthtipor) throws RemoteException;

    public int obtenerNumeroDominios() throws RemoteException;

    public int obtenerNumeroTiposRec() throws RemoteException;

    public String obtenerNombreNthDom(int nthdom) throws RemoteException;

    public String obtenerNombreNthTipoRec(int nthtipor) throws RemoteException;
}
