import java.util.Scanner;

//palindromo 
class Palindromo 
{
    public boolean palindromo (String s)
    {
        int n = s.length( ) - 1;
        int i;
        int fim = s.length( )/2;
        boolean res = true;
        
        for(i = 0; i < fim; i++)
        {
            /* 
            if( !(s.charAt(i) >= ' ' && s.charAt(i) <= 'z' ) )
            {
            s.charAt(i) = 'a';	
            }
            */
            
            //printf ("%c%c" , s.charAt(i) , s[n]);
            if(s.charAt(i) != s.charAt(n) )
            {
                i = fim;
                res = false;
                
            }
            
            n--;
        }
        
        
        return(res);
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
        Palindromo palindromo = new Palindromo();
        Boolean continuar = true;
        
        while (continuar)
        {
 
            s = scanner.nextLine();
        
            if (palindromo.str_cmp(s , "FIM") != 0)
            {
            
                
                if(palindromo.palindromo(s))
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