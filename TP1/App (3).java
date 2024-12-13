
import java.util.*;
//palindromo 
class App
{

    public static void cifraCesar (String s, int i , int n)
    {
        
        if(i < n )
        {
              char letra = s.charAt(i);

            if( (s.charAt(i) >= ' ' && s.charAt(i) <= 'z' ) )
            {
                 
                System.out.print( (char) (letra +3) );
           }
            else
            {
                    System.out.print(letra);
            }
            //i++ nao resolve e fica repeticao infinita
            cifraCesar ( s, i + 1 , n);
        }
       
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
        App ciframentoCesar = new App();
        Boolean continuar = true;

        while (continuar)
        {

           s = scanner.nextLine();

            if (ciframentoCesar.str_cmp(s , "FIM") != 0)
            {


            cifraCesar(s, 0 , s.length());
            
            System.out.println( );
            }
            else
            {
                continuar = false;
            }

        }

        scanner.close();

    }

}