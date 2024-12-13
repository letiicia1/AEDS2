
import java.util.*;
//palindromo 
class CiframentoCesar
{

    public void cifraCesar (String s)
    {
        int n = s.length( );

        for(int i = 0; i < n; i++)
        {
              char letra = s.charAt(i);

            if( (s.charAt(i) >= ' ' && s.charAt(i) <= 'z' ) )
            {
                 //converter para caractere
             
                System.out.print( (char) (letra +3) );
           }
            else
            {
                    System.out.print(letra);
            }

        }

         System.out.println( );
       
    }

    public int str_cmp (String s, String f)
    {

        int tamanho = s.length( );
        int res = 0;
        if(tamanho == f.length( ))
        {

            for(int i = 0; i < tamanho; i++)
            {
                if(s.charAt(i) != f.charAt(i))
                {
                    res = s.charAt(i) - f.charAt(i);
                    i = tamanho;
                }
            }

        }
        else
        {
            res = -1;
        }


        return res;
    }

    public static void main(String[] args) 
    {

        String s;
        Scanner scanner = new Scanner(System.in);
        CiframentoCesar ciframentoCesar = new CiframentoCesar();
        Boolean continuar = true;

        while (continuar)
        {

           s = scanner.nextLine();

            if (ciframentoCesar.str_cmp(s , "FIM") != 0)
            {


            ciframentoCesar.cifraCesar(s);
            

            }
            else
            {
                continuar = false;
            }

        }

        scanner.close();

    }

}