const LENMSG     = 1024;

typedef string cadena<LENMSG>;

struct Lista{
  cadena dato;
  Lista *siguiente;
};

struct datini {
   Lista *nomdominios;
   Lista *nomtiporecords;
};

struct paramconsulta {
   cadena nomdominio;
   cadena tiporecord;
   cadena clave;
};

struct domrecord {
   int ndxdom;
   int ndxrecord;
};

union Resultado switch (int caso){
   case 0: cadena msg;
   case 1: int val;
   case 2: cadena err;
};

program SRVDNS{
   version PRIMERA{
     Resultado inicializar_srvdns(datini)=1;
     Resultado consulta_record(paramconsulta)=2;
     Resultado obtener_total_dominio(int)=3;
     Resultado obtener_total_registro(int)=4;
     Resultado obtener_total_dominioregistro(domrecord)=5;
     Resultado obtener_num_dominios(void)=6;
     Resultado obtener_num_records(void)=7;
     Resultado obtener_nombre_dominio(int)=8;
     Resultado obtener_nombre_record(int)=9;
   }=1;
}=0x2023f001;

