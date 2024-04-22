

import java.rmi.Naming;
import java.rmi.RemoteException;
import java.io.*;

public class Cliente {
public static void main(String[] args) {
        int op1;
        int op2;
        int res;

        try
        {
          Calculadora obj = (Calculadora) Naming.lookup("CalculadoraServer");
          BufferedReader in =
                      new BufferedReader (new InputStreamReader (System.in));
          try{
            System.out.println("Operando1: ");
            op1=Integer.parseInt(in.readLine());
            System.out.println("Operando2: ");
            op2=Integer.parseInt(in.readLine());
            res=obj.suma(op1, op2);
            System.out.println("El resultado de la suma es: "+res);
            res=obj.resta(op1, op2);
            System.out.println("El resultado de la resta es: "+res);
          }
         catch (RemoteException e) {
               System.out.println("Error en la invocación remota: "+
                                   e.getMessage());
         }
        catch (Exception e) {
               System.out.println("Error en lectura de datos: "+
                                   e.getMessage());
        }
       }
       catch (Exception e) {
               System.out.println("Error en lookup: "+e.getMessage());
       } //fin del primer try
    } //fin de main
} // fin de la clase


