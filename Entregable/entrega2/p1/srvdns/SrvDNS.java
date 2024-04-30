package srvdns;

// Imports necesarios para usar RabbitMQ
import com.rabbitmq.client.Channel;
import com.rabbitmq.client.Connection;
import com.rabbitmq.client.ConnectionFactory;
import com.rabbitmq.client.Consumer;
import com.rabbitmq.client.DefaultConsumer;
import com.rabbitmq.client.Envelope;
import com.rabbitmq.client.AMQP;

// Imports necesarios para usar RMI
import java.io.IOException;
import java.rmi.Naming;
import java.rmi.RemoteException;

// Import necesario para leer fichero registros y escribir en fichero de log
import java.io.FileWriter;
import java.io.FileReader;
import java.io.BufferedWriter;
import java.io.BufferedReader;

// Import necesarios para obtener la fecha y hora actuales
import java.util.Date;

// Import para trabajar con listas
import java.util.List;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.ListIterator;

// Cola bloqueante para comunicar el hilo ReceptorConsultas y los hilos Worker
import java.util.concurrent.ArrayBlockingQueue;

import cliente.Cliente;
// Import necesario para acceder a los métodos del cliente
import cliente.ClienteInterface;

// ===================================================================
// Las dos clases siguientes son hilos que se ejecutarán de forma concurrente
//
// - ReceptorConsultas es el hilo que espera mensajes de RabbitMQ e introduce
//   los mensajes recibidos en la cola interna (cola bloqueante)
// - Worker son los hilos que leen de la cola interna, resuelven y
//   contabilizan las consultas DNS recibidas y las registran en el fichero
//   de log.

// Clase ReceptorConsultas. Recibe mensajes por RabbitMQ, y los mete en la cola bloqueante 
// de eventos para que sean procesados por los hilos Worker
class ReceptorConsultas extends Thread {
    private final static String NOMBRE_COLA_RABBIT = "cola_dns"; // A RELLENAR (cambiar nombre)
    private ArrayBlockingQueue<String> qrequest;  // Cola bloqueante para comunicar con los workers

    // El constructor recibe una referencia a la cola bloqueante
    // que le permiten comunicarse con los hilos Worker
    public ReceptorConsultas(ArrayBlockingQueue<String> qrequest) {
        this.qrequest = qrequest;
    }

    // La función run es la que se ejecuta al poner en marcha el hilo
    public void run() {
        ConnectionFactory factory = new ConnectionFactory();
        factory.setHost("localhost");
        try {
            // Conectar con rabbitMQ
            // A RELLENAR:
            final Connection connection = factory.newConnection();
            final Channel channel = connection.createChannel();
            channel.queueDeclare(NOMBRE_COLA_RABBIT, true, false, false, null);
            System.out.println(" [*] Waiting for messages. To exit press CTRL+C");
            channel.basicQos(1);

            // Espera por peticiones en la cola rabbitMQ
            Consumer consumer = new DefaultConsumer(channel) {
                @Override
                public void handleDelivery(String consumerTag, Envelope envelope, AMQP.BasicProperties properties,
                        byte[] body)
                        throws IOException {
                    // ************************************************************
                    // Recepción y manejo del mensaje que llega por RabbitMQ
                    // ************************************************************

                    // Convertir en cadena el mensaje recibido y meterlo en la cola bloqueante
                    String msg = new String(body, "UTF-8");
                    System.out.println("ReceptorConsultas: Recibida consulta = " + msg);

                    // A RELLENAR:
                    |
                    |
                    |
                    |
                    |
                    |

                }
            };
            System.out.println("ReceptorConsultas. Esperando llegada de consultas de resolución DNS");
            // Arrancar la espera de mensajes en la cola RabbitMQ
            // A RELLENAR:
            channel.basicConsume(NOMBRE_COLA_RABBIT, true, consumer);

        } catch (Exception e) { // No manejamos excepciones, simplemente abortamos
            e.printStackTrace();
            System.exit(7);
        }
    }
}

// Clase Worker espera en una cola bloqueante a que el hilo ReceptorConsultas le
// envíe una consulta para procesarla.

class Worker extends Thread {
    private List<RecordDNS> rcache;          // Registros DNS leídos del disco
    private ContabilidadConsultas act;       // Objeto que permite registrar
                                             // el total de consultas recibidas
    private ArrayBlockingQueue<String> cola; // Cola bloqueante para comunicarse con hilo ReceptorConsultas
    private String nflog;                    // Nombre del fichero de log

    // El constructor recibe una referencia al objeto
    // que contabiliza las consultas recibidas y la cola

    public Worker(List<RecordDNS> cache, ContabilidadConsultas cc, ArrayBlockingQueue<String> cola, String nomflog) {
        this.rcache = cache;
        this.act = cc;
        this.cola = cola;
        this.nflog = nomflog;
    }

    // El método run es el que se ejecuta al arrancar el hilo
    public void run() {
        List<RecordDNS> lret;
        RecordDNS r;
        Iterator<RecordDNS> iter;
        String cadena = "";

        try {
            while (true) { // Bucle infinito
                // Esperar consulta en la cola bloqueante
                // A RELLENAR:
                cadena = cola.take();
                String consulta = cadena;

                // Procesar consulta separando sus campos por el caracter ","
                String partes[] = consulta.split(",");
                if (partes.length < 3 || partes.length > 4) {
                    System.out.println(
                            String.format("Ignorada consulta por no tener el formato esperado '%s'", consulta));
                } else {
                    try {
                        Boolean primera = true;

                        // Localizar al cliente RMI al que hay que devolver la respuesta. Su nombre está en
                        // la primera parte del mensaje recibido desde la cola
                        // A RELLENAR:
                        String nombreCliente = partes[0]; // El primer elemento es el nombre del cliente RMI
                        Cliente rmiCli = (ClienteRMI)

                        // Procesar la consulta
                        if (partes[2].equals("MX") || partes[2].equals("NS")) {
                            // puede haber más de un registro para una consult
                            // //puede haber más de un registro para una consulta
                            lret = buscarRegistros(partes[1], partes[2], null);
                            iter = lret.iterator();
                            cadena = String.format("%s,%s", partes[1], partes[2]);
                            while (iter.hasNext()) {
                                r = iter.next();
                                if (primera) {
                                    act.contabilizaConsulta(r);
                                    cadena = cadena + "," + r.getValorRecord();
                                    primera = false;
                                } else {
                                    cadena = cadena + ":" + r.getValorRecord();
                                }
                            }
                        } else // buscamos un registro que no es "NS" o "MX"
                        {
                            lret = buscarRegistros(partes[1], partes[2], partes[3]);
                            cadena = String.format("%s,%s,%s", partes[1], partes[2], partes[3]);
                            r = null;
                            if (lret.size() > 0) {
                                r = lret.get(0);
                                if (r != null) {
                                    cadena = cadena + "," + r.getValorRecord();
                                }
                            }
                            if (r != null)
                                act.contabilizaConsulta(r);
                        }
                        // Enviar la respuesta al cliente
                        // A RELLENAR:
                        |
                        |

                    } catch (Exception e) {
                        // Cualquier excepción simplemente se imprime
                        System.out.println("Error en SrvDNS al devolver respuesta al cliente" + e.getMessage());
                        e.printStackTrace();
                    }

                    // Registrar la consulta en el fichero de log
                    Date fecha = new Date();
                    try {
                        String linea;
                        // Preparar la línea a volcar
                        // A RELLENAR:
                        |
                        |

                        BufferedWriter bw = new BufferedWriter(new FileWriter(nflog, true));
                        bw.write(linea);
                        bw.close();
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
            }
        } catch (Exception e) {
            e.printStackTrace();
            System.exit(8);
        }
    }

    private List<RecordDNS> buscarRegistros(String nomdom, String nomtrec, String clave) {
        List<RecordDNS> ret = new ArrayList<>();
        RecordDNS r;
        Iterator<RecordDNS> iterador = rcache.iterator();

        while (iterador.hasNext()) {
            r = iterador.next();

            if ((nomtrec.equals("NS")) || (nomtrec.equals("MX"))) {
                // Estamos buscando un registro NS o MX
                if ((nomdom.equals(r.getNombreDominio())) && (nomtrec.equals(r.getTipoRecord()))) {
                    ret.add(r);
                }
            } else // se trata de un registro de tipo A, AAAA, PTR o CNAME
            {
                if ((nomdom.equals(r.getNombreDominio())) && (nomtrec.equals(r.getTipoRecord()))
                        && (clave.equals(r.getClaveRecord()))) {
                    ret.add(r);
                    break; // salimos porque ya no hay más registros
                }
            }
        }
        return ret;
    }
}

// Clase principal que instancia el hilo de recepción y los hilos worker y los
// arranca
public class SrvDNS {

    public static void main(String[] argv) throws Exception {

        BufferedReader frecords = null; // Para leer el fichero de registros
        String nameflog = null; // Para registrar las consultas procesadas
        List<RecordDNS> cacherecords = new ArrayList<>();

        // Los valores de estas variables se leen de línea de comandos
        int tam_cola = 0; // Tamaño de la array blocking queue
        int num_workers = 0; // Numero de hilos trabajadores

        // Cola interna de sincronización entre el hilo ReceptorConsultas y los Worker
        ArrayBlockingQueue<String> cola_interna;

        // Variable que representa el hilo receptor de eventos
        ReceptorConsultas receptor_consultas;
        // Variable que representa a los hilos trabajadores
        Worker[] workers;

        // Lectura de la línea de comandos
        if (argv.length < 4) {
            System.out.println("Uso: SrvDNS <tam_cola> <num_workers> <fichero_registros> <fichero_log>");
            System.exit(1);
        }
        try {
            tam_cola = Integer.parseInt(argv[0]);
            num_workers = Integer.parseInt(argv[1]);
        } catch (NumberFormatException e) {
            System.out.println("Los argumentos tam_cola y numworkers deben ser enteros");
            System.exit(2);
        }

        if (tam_cola < 1) {
            System.out.println("tam_cola debe ser un valor >=1");
            System.exit(5);
        }
        if (num_workers < 1) {
            System.out.println("num_workers debe ser un valor >=1");
            System.exit(6);
        }
        try {
            frecords = new BufferedReader(new FileReader(argv[2]));
            cachearRegistros(frecords, cacherecords);
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            // En el finally cerramos el fichero, para asegurarnos
            // que se cierra tanto si todo va bien como si salta
            // una excepcion.
            try {
                if (frecords != null) {
                    frecords.close();
                }

            } catch (Exception e2) {
                e2.printStackTrace();
            }
        }
        nameflog = argv[3];

        // Creamos el objeto que nos permite llevar la contabilidad de las consultas
        ContabilidadConsultas accountq = new ContabilidadConsultas(cacherecords);

        // Primero se crea la cola interna de sincronización entre hilos
        // A RELLENAR:
        |
        |

        try {
            // Arrancar el servidor RMI para el cliente Estadis y registrarlo
            // A RELLENAR:
            |
            |
            |

            System.out.println("SrvDNS registrado para RMI");
        } catch (Exception e) {
            // Cualquier excepción simplemente se imprime
            System.out.println("Error al registrar el objeto RMI interfaz con el SrvDNS: " + e.getMessage());
            e.printStackTrace();
        }

        // Crear los hilos Worker y arrancarlos
        workers = new Worker[num_workers];
        // A RELLENAR:
        |
        |
        |
        |

        // Crear el hilo receptor de consultas y arrancarlo
        receptor_consultas = new ReceptorConsultas(cola_interna);
        receptor_consultas.start();

        // Esperamos a que finalice el hilo receptor de eventos (nunca finalizará, hay
        // que parar con Ctrl+C)
        receptor_consultas.join();
    }

    private static void cachearRegistros(BufferedReader br, List<RecordDNS> l) {
        RecordDNS item;
        try {
            String r = br.readLine();

            while (r != null) {
                // Vamos a extraer los tokens de la linea leida

                String campos[] = r.split(",");
                System.out.println("Linea leida: " + r);
                // Primer token es el dominio
                item = new RecordDNS();
                item.setNombreDominio(campos[0]);
                item.setTipoRecord(campos[1]);
                if (campos[1].equals("NS") || campos[1].equals("MX")) {
                    item.setClaveRecord(null);
                    item.setValorRecord(campos[2]);
                } else {
                    item.setClaveRecord(campos[2]);
                    item.setValorRecord(campos[3]);
                }
                l.add(item);
                r = br.readLine();
            }
        } catch (Exception e) {
            // Cualquier excepción simplemente se imprime
            System.out.println("Error en SrvDNS al procesar el fichero de registros" + e.getMessage());
            e.printStackTrace();
        }
    }
}
