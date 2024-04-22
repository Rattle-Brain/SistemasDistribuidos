package srvdns;

// Imports  necesarios
import java.util.ArrayList;
import java.util.List;

// La clase siguiente almacena información de los registros DNS
class RecordDNS {
    String nombredominio;
    String tiporecord;
    String claverecord;
    String valorrecord;

    public RecordDNS() {
        nombredominio = null;
        tiporecord = null;
        claverecord = null;
        valorrecord = null;
    }

    public RecordDNS(String nomdom,
            String tipor,
            String claver,
            String valorr) {
        nombredominio = nomdom;
        tiporecord = tipor;
        claverecord = claver;
        valorrecord = valorr;
    }

    public void setNombreDominio(String cad) {
        nombredominio = cad;
    }

    public String getNombreDominio() {
        return nombredominio;
    }

    public void setTipoRecord(String cad) {
        tiporecord = cad;
    }

    public String getTipoRecord() {
        return tiporecord;
    }

    public void setClaveRecord(String cad) {
        claverecord = cad;
    }

    public String getClaveRecord() {
        return claverecord;
    }

    public void setValorRecord(String cad) {
        valorrecord = cad;
    }

    public String getValorRecord() {
        return valorrecord;
    }
}
