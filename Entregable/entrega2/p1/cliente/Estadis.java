package cliente;

// Imports necesarios para RabbitMQ
//import com.rabbitmq.client.Channel;
//import com.rabbitmq.client.Connection;
//import com.rabbitmq.client.ConnectionFactory;

// Imports necesarios para RMI
//import java.io.IOException;
import java.rmi.Naming;
//import java.rmi.RemoteException;

// Imports necesarios para invocar via RMI métodos del sislog
import srvdns.SrvDNSInterface;

public class Estadis {

    public static void main(String[] argv) throws Exception {
        int nfils, ncols, suma, n, total;

        // =================================================
        // Parte principal, toda dentro de un try para capturar cualquier excepción
        try {
            // Localizar via rmiregistry al servidor DNS
            // A RELLENAR:
            SrvDNSInterface srv = (SrvDNSInterface) Naming.lookup("ServidorDNS");
            

            ncols = srv.obtenerNumeroTiposRec();
            nfils = srv.obtenerNumeroDominios();

            System.out.println("**************************  RECUENTO CONSULTAS  ********************************");

            // Imprimir cabeceras de la tabla, cno los nombres de los tipos de recursos
            System.out.print("\t\t");

            // A RELLENAR:
            for(int i = 0; i < ncols; i++){
                System.out.print(srv.obtenerNombreNthTipoRec(i) + "\t");
            }

            // Imprimir cabecera de la última columna "TOTAL"
            System.out.println("TOTAL");

            // Iterar por filas (cada fila es un nombre de dominio)
            suma = 0;
            for (int i = 0; i < nfils; i++) {
                System.out.print(srv.obtenerNombreNthDom(i) + "\t");
                suma = 0;
                for (int j = 0; j < ncols; j++) {
                    // Obtener el elemento de la fila i, columna j
                    // A RELLENAR:
                    n = srv.obtenerValorNthDomNthTipoRec(i, j);

                    // Imprimir el valor y acumularlo en la suma
                    System.out.print(n + "\t");
                    suma += n;
                }
                System.out.println(suma);
            }

            // Recorrer de nuevo toda la tabla ahora por columnas
            // para ir copmutando el total por columna y el total agregado final
            System.out.print("TOTALES\t\t");
            total = 0;
            for (int j = 0; j < ncols; j++) {
                suma = 0;
                for (int i = 0; i < nfils; i++) {
                    // Obtener el elemento de la fila i, columna j
                    // A RELLENAR:
                    n = srv.obtenerValorNthDomNthTipoRec(i, j);
                    suma += n;
                }
                // Imprimir el total de esa columna y agregarlo al total final
                // A RELLENAR:
                System.out.print(suma + "\t");
                total += suma;
            }
            // Imprimir total final
            System.out.println(total);

        } catch (Exception e) {
            // Cualquier excepción simplemente se imprime
            System.out.println("Error en Estadis" + e.getMessage());
            e.printStackTrace();
        }
    }
}
