import java.rmi.RemoteException;
import java.rmi.server.UnicastRemoteObject;

public class CalculadoraImpl extends UnicastRemoteObject implements Calculadora {

    public CalculadoraImpl() throws RemoteException {
        super();
    }

    public int suma (int op1, int op2) {
        return op1 + op2;
    }

    public int resta (int op1, int op2) {
        return op1 - op2;
    }
}
