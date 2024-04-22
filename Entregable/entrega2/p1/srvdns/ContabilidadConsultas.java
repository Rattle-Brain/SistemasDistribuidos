package srvdns;

import java.util.List;
import java.util.ArrayList;

// Clase que mantiene la contabilidad de las consultas DNS recibidas. 
// Ya que algunos de sus métodos pueden ser invocados desde diferentes hilos
// pues podrían estar atendiéndose a varios clientes a la vez, 
// se sincronizan entre sí mediante bloques synchronized (exclusión mutua) 

class ContabilidadConsultas {
    private int val[][]; // Matriz de enteros que contabiliza los eventos recibidos
    private List<String> lnomdominios; // Lista de nombres de dominios
    private List<String> lnomtrec; // Lista de nombres de tipos de registros

    public ContabilidadConsultas(List<RecordDNS> lista) {
        // Inicializar las variables internas y la matriz de contadores
        // según los datos de la lista de registros
        RecordDNS r;
        int filas;
        int columnas;

        lnomdominios = new ArrayList<>();
        lnomtrec = new ArrayList<>();

        // Meter en lnomdominios y lnomtrec los nombres de dominios y tipos de registros
        // que aparecen en la lista de registros, evitando duplicados
        for (int i = 0; i < lista.size(); i++) {
            r = lista.get(i);
            if (lnomdominios.indexOf(r.getNombreDominio()) == -1) {
                lnomdominios.add(r.getNombreDominio());
            }
            if (lnomtrec.indexOf(r.getTipoRecord()) == -1) {
                lnomtrec.add(r.getTipoRecord());
            }
        }
        // Inicializar con ceros la matriz de contadores (dándole el tamaño apropiado)
        // A RELLENAR:
        |
        |
        |
        |
        |
        |

    }

    // Metodo que contabiliza una consulta
    public void contabilizaConsulta(RecordDNS r) {
        int fila;
        int columna;

        if (r != null) {
            fila = lnomdominios.indexOf(r.getNombreDominio());
            columna = lnomtrec.indexOf(r.getTipoRecord());
            if (fila != -1 && columna != -1) {
                synchronized (this) { // Exclusión mutua
                    val[fila][columna]++;
                }
                // Fin exclusión
            }
        }

    }

    // Metodo que devuelve el número de consultas realizados para
    // un dominio y tipo de registro
    public int obtenerValorDominioTiporec(int ndxdom, int ndxtipor) {
        int ret;

        // A RELLENAR:
        |
        |
        |
        |

        return ret;
    }

    int obtenerNumeroDominios() {
        return lnomdominios.size();
    }

    int obtenerNumeroTiposRegistros() {
        return lnomtrec.size();
    }

    String obtenerNombreDominioNth(int ndx) {
        return lnomdominios.get(ndx);
    }

    String obtenerNombreTipoRecNth(int ndx) {
        return lnomtrec.get(ndx);
    }
}
