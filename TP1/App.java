import java.util.*;

class App
{
    public static Random gerador = new Random(4);
    
    public static int str_cmp (String s, String f)
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
    
    public static void change (String s)
    {
 
        char a = (char)('a' + (Math.abs(gerador.nextInt()) % 26));
        char b = (char)('a' + (Math.abs(gerador.nextInt()) % 26));

     //   System.out.print(a);
      //  System.out.print(b);

        for(int i = 0; i < s.length(); i++)
        {

            if(s.charAt(i) == a)
            {
                System.out.print(b);

            }
            else
            {
                System.out.print(s.charAt(i));
            }

        }

            System.out.println( );
    }

  
    public static void main (String [ ] args)
    {

        
        String s;
        Scanner scanner = new Scanner(System.in);
        Boolean continuar = true;
        
        
        while (continuar)
        {
 
            s = scanner.nextLine();
        
            if (str_cmp(s , "FIM") != 0)
            {
            
                change (s);
        
            }
            else
            {
                continuar = false;
            }
            
        }
        
        scanner.close();

    }

}