import java.util.*;

class App
{

    public static boolean vogal (String s, int i , int tam, boolean res)
    {

        

        if( i < tam)
        {
            if(s.charAt(i) >= ' ' && s.charAt(i) <= 'z')
            {
                if (s.charAt(i) == 'a' || s.charAt(i) == 'e' || s.charAt(i) == 'i' || s.charAt(i) == 'o' || s.charAt(i) == 'u' ||
                s.charAt(i) == 'A' || s.charAt(i) == 'E' || s.charAt(i) == 'I' || s.charAt(i) == 'O' || s.charAt(i) == 'U' ) 
                {
                    
                }
                else 
                {
                    res = false;
                    i = tam;
                }
            }
            else 
            {
                res = false;
                i = tam;
            }

            res = vogal(s, i + 1, tam, res);

        }

        return res;

    }
   
    public static boolean Isletra (char s )
    {
        boolean res = false;

       if(s >= 'a' && s <= 'z' || s >= 'A' && s <= 'Z' )
       {
            res = true;
       }

        return res;
    }

    public static boolean consoante (String s, int i , int tam, boolean res)
    {
       

        if( i < tam)
        {
            if(s.charAt(i) >= ' ' && s.charAt(i) <= 'z')
            {
                if(Isletra(s.charAt(i)))
                {
                    if( (s.charAt(i) == 'a' || s.charAt(i) == 'e' || s.charAt(i) == 'i' || s.charAt(i) == 'o' || s.charAt(i) == 'u' ||
                    s.charAt(i) == 'A' || s.charAt(i) == 'E' || s.charAt(i) == 'I' || s.charAt(i) == 'O' || s.charAt(i) == 'U' ) )
                    {
                        res = false;
                        i = tam;
                    }
                }
                else 
                {
                    res = false;
                    i = tam;
                }
            }

            else 
            {
                res = false;
                i = tam;
            }

            res = consoante(s, i + 1, tam, res);

        }

        return res;
    }
    
    public static boolean numeroInt (String s, int i , int tam, boolean res)
    {

        if(i < tam)
        {
            if(s.charAt(i) >= ' ' && s.charAt(i) <= 'z')
            {
                if(s.charAt(i) >= '0' && s.charAt(i) <= '9')
                {
                    
                }
                else 
                {
                    res = false;
                    i = tam;
                }
            }

            else 
            {
                res = false;
                i = tam;
            }

            res = numeroInt(s, i + 1, tam, res);

        }

        return res;
    }
    
    public static boolean numeroReal (String s, int i , int tam, boolean res, int ponto, int virgula)
    {


        if(i < tam)
        {
            if(s.charAt(i) >= ' ' && s.charAt(i) <= 'z')
            {
                if (s.charAt(i) == ',')
                {
                    virgula++;
                    
                }

                else if (s.charAt(i) == '.')
                {
                    ponto++;
                   
                }

                else if(s.charAt(i) >= '0' && s.charAt(i) <= '9')
                {
                    
                }
                else 
                {
                    res = false;
                    i = tam;
                }
            
                if((ponto == 1 && virgula == 1) || (virgula > 1) || (ponto > 1))
                {
                    res = false;
                    i = tam;
                }
            
            }

            else 
            {
                res = false;
                i = tam;
            }

            res = numeroReal(s, i + 1, tam, res, ponto, virgula);
        }

        return res;
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

    public static void falseEtrue (boolean x)
    {
        if(x)
        System.out.print("SIM ");

        else
        System.out.print("NAO ");

    }
    public static void main (String [ ] args)
    {

        
        String s;
        Scanner scanner = new Scanner(System.in);
        App app = new App();
        Boolean continuar = true;
        Boolean x1, x2, x3, x4;
        
        while (continuar)
        {
 
            s = scanner.nextLine();
        
            if (app.str_cmp(s , "FIM") != 0)
            {
            
                x1 = vogal(s, 0, s.length(), true);
                x2 = consoante(s, 0, s.length(), true);
                x3 = numeroInt(s, 0, s.length(), true);
                x4 = numeroReal(s, 0, s.length(), true, 0, 0);
                

                falseEtrue(x1);
                falseEtrue(x2);
                falseEtrue(x3);
        
                if(x4)
                System.out.print("SIM");

                else
                System.out.print("NAO");

                System.out.println();
               
            }
            else
            {
                continuar = false;
            }
            
        }
        
        scanner.close();

    }

}