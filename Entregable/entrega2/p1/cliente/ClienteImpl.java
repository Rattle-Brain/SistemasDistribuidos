package cliente;

import java.rmi.RemoteException;
import java.rmi.server.UnicastRemoteObject;
import java.util.concurrent.ArrayBlockingQueue;

/*
La clase ClienteImpl expone vía RMI el método:

setRespuesta(): que será invocado desde el servidor DNS cuando se resuelve
una consulta del cliente

Además proporciona el método getRespuesta() para que pueda ser llamado
desde el método enviar_consultas del cliente, para así obtener la respuesta a la última
petición de resolución enviada.

Estos métodos se ejecutan en diferentes hilos (ej.: setRespuesta() se
ejecuta en el hilo que atiende peticiones RMI, mientras que getRespuesta() se
ejecuta en el hilo del cliente. Se comunican entre sí mediante
una cola bloqueante. getRespuesta() intenta leer de esa cola, y si está vacía
se bloquea. setRespuesta() pone en esa cola la respuesta a la última consulta, 
desbloqueando así al hilo que estaba esperando en getRespuesta())
*/

public class ClienteImpl extends UnicastRemoteObject implements ClienteInterface {
    private ArrayBlockingQueue<String> cola; // Cola bloqueante para comunicar los hilos

    public ClienteImpl() throws RemoteException {
        super();
        // La cola a través de la cual se comunican los hilos que ejecutan 
        // los métodos de esta clase
        cola = new ArrayBlockingQueue<String>(1); // Basta que tenga tamaño 1
    }

    @Override
    public void setRespuesta(String answer) throws RemoteException {
        // Este método es invocado vía RMI desde el servidor DNS
        // Recibe la respuesta a la consulta y se limita a guardarla en la cola interna
        try {
            // A RELLENAR:
            cola.put(answer);

        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }

    public String getRespuesta() throws InterruptedException {
        // Este método es invocado desde el cliente (método enviar_consultas)
        // Espera a que aparezca la respuesta a la última consulta
        // y retorna su valor
        // A RELLENAR:
        return cola.take();
        
    }

}
