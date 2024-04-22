import java.rmi.Naming;

public class Servidor {

 public static void main(String[] args) {
   try{
      // Crear el objeto que implementa los servicios
      CalculadoraImpl obj=new CalculadoraImpl();
      // Registrarlo
      Naming.rebind("CalculadoraServer",obj);
      System.out.println("CalculadoraServer registrada");
   }
   catch (Exception e)
   {
      System.out.println("Error en CalculadoraImpl:"+e.getMessage());
      e.printStackTrace();
   }
 }
}
