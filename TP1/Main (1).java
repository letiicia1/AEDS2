import java.io.RandomAccessFile;
import java.io.IOException;
import java.util.Scanner;

class Main
{

    
        public static void arquivo(String fileName)
         {
      
            try {
                // Passo 2: Lendo de trás para frente
                RandomAccessFile file = new RandomAccessFile(fileName, "r");
                long fileLength = file.length();
                
                // Movendo o ponteiro para o final do arquivo e lendo de trás para frente
                for (long i = fileLength - 8; i >= 0; i -= 8) { // double ocupa 8 bytes
                    file.seek(i);
                    double value = file.readDouble();

                    if (value == (long) value) {
                        
                        System.out.println((long) value);
                    } 
                    else
                     {
                        
                        System.out.println(value);
                    }
                }
                
                file.close();
                } 
            catch (IOException e)
             {
                e.printStackTrace();
            } 
        }
    
    public static void main (String [ ] args)
    {
        Scanner scanner = new Scanner(System.in);
        int n;
        String fileName = "valores.txt";


        n = scanner.nextInt();

            try {

                // Passo 1: Criando e escrevendo no arquivo
                RandomAccessFile file = new RandomAccessFile(fileName, "rw");
                
                
                for(int i = 0; i < n; i++)
                {
                    double value = scanner.nextDouble();
                    file.writeDouble(value);
                }
                file.close();
                
                arquivo(fileName);
            } catch (IOException e)
             {
                e.printStackTrace();
            } 
            finally 
            {
                scanner.close();
            }
            
        }

    }
