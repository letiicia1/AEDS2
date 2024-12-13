import java.util.Scanner;

//palindromo 
class App
{
    public static int palindromo (String s, int i, int n, int res)
    {
      
        
            if (i < n)
            {
               if
                (!(s.charAt(i) >= ' ' && s.charAt(i) <= 'z') ) 
                res = 0;
                
                if (s.charAt(i) != s.charAt(n)) 
                {	
                    res = 0;
                }
                
                res = palindromo(s, i + 1, n - 1, res); 
            
                
            }
            
            
            return res;
                
    }

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

    public static void main(String[] args) 
    {
        
        String s;
        Scanner scanner = new Scanner(System.in);
        Boolean continuar = true;
        
        while (continuar)
        {
 
            s = scanner.nextLine();
        
            if (str_cmp(s , "FIM") != 0)
            {
            
                
                if( palindromo(s, 0, s.length() - 1, 1) == 1)
                {
                     System.out.println ("SIM");
                }
                else
                {
                    System.out.println ("NAO");
                }
            }
            else
            {
                continuar = false;
            }
            
        }
        
        scanner.close();
    
    }

}