import java.util.Scanner;
import java.net.HttpURLConnection;
import java.net.URL;
import java.io.*;
import java.nio.charset.*;

class Main {
    private static BufferedReader in = new BufferedReader(new InputStreamReader(System.in, Charset.forName("UTF-8")));
    private static String charset = "UTF-8";

    public static void println(String x) {
        try {
            PrintStream out = new PrintStream(System.out, true, charset);
            out.println(x);
        } catch (UnsupportedEncodingException e) {
            System.out.println("Erro: charset invalido");
        }
    }

    public static void println(char x){

        try {
            System.setOut(new PrintStream(System.out, true, StandardCharsets.UTF_8));
        } catch (Exception e) {
            e.printStackTrace();
        }
        
        try {
           PrintStream out = new PrintStream(System.out, true, charset);
           out.println(x);
        }catch(UnsupportedEncodingException e){ System.out.println("Erro: charset invalido"); }
     }

    public static int str_cmp(String s, String f) {
        int tamanho = s.length();
        int res = 0;
        if (tamanho == f.length()) {
            for (int i = 0; i < tamanho; i++) {
                if (s.charAt(i) != f.charAt(i)) {
                    res = s.charAt(i) - f.charAt(i);
                    i = tamanho;
                }
            }
        } else {
            res = -1;
        }
        return res;
    }

    public static void procurar(String endereco, String nomePagina) {
        // Inicialização das variáveis de contagem
        int a = 0, e = 0, i = 0, o = 0, u = 0;
        int aacute = 0, eacute = 0, iacute = 0, oacute = 0, uacute = 0;
        int agrave = 0, egrave = 0, igrave = 0, ograve = 0, ugrave = 0;
        int tilde_a = 0, tilde_o = 0;
        int circumflex_a = 0, circumflex_e = 0, circumflex_i = 0, circumflex_o = 0, circumflex_u = 0;
        int consoante = 0, br = 0, table = 0;

        StringBuffer resp = new StringBuffer();

        try {
            URL obj = new URL(endereco);
            HttpURLConnection con = (HttpURLConnection) obj.openConnection();

            con.setRequestMethod("GET");

            int responseCode = con.getResponseCode();
            //System.out.println("Código de resposta: " + responseCode);

            if (responseCode == HttpURLConnection.HTTP_OK) {
                BufferedReader in = new BufferedReader(new InputStreamReader(con.getInputStream()));
                String inputLine;

                while ((inputLine = in.readLine()) != null) {
                    resp.append(inputLine);

                    for (char c : inputLine.toCharArray()) {
                        switch (c) {
                            case 'a': a++; break;
                            case 'e': e++; break;
                            case 'i': i++; break;
                            case 'o': o++; break;
                            case 'u': u++; break;
    
                            case '\u00E1': aacute++; break; // á
                            case '\u00E9': eacute++; break; // é
                            case '\u00ED': iacute++; break; // í
                            case '\u00F3': oacute++; break; // ó
                            case '\u00FA': uacute++; break; // ú

                            case '\u00E0': agrave++; break; // à
                            case '\u00E8': egrave++; break; // è
                            case '\u00EC': igrave++; break; // ì
                            case '\u00F2': ograve++; break; // ò
                            case '\u00F9': ugrave++; break; // ù

                            case '\u00E3': tilde_a++; break; // ã
                            case '\u00F5': tilde_o++; break; // õ

                            case '\u00E2': circumflex_a++; break; // â
                            case '\u00EA': circumflex_e++; break; // ê
                            case '\u00EE': circumflex_i++; break; // î
                            case '\u00F4': circumflex_o++; break; // ô
                            case '\u00FB': circumflex_u++; break; // û
                            default:
                                if  ( (c >= 'b' && c <= 'z') && (c != 'a' && c != 'e' && c != 'i' && c != 'o' && c != 'u') ) {
                                    consoante++;
                                }
                                break;
                        }
                    }

                    if (inputLine.contains("<table>")) {
                        table++;
                    }
                
                    if (inputLine.contains("<br>")) {
                        br++;
                    }
            
                }

                in.close();

               
                a = a - table;
                e = e - table;
                consoante -= 3;
            
                System.out.println("a(" + a + ") e(" + e + ") i(" + i + ") o(" + o + ") u(" + u + ") " +
                "\u00E1(" + aacute + ") \u00E9(" + eacute + ") \u00ED(" + iacute + ") \u00F3(" + oacute + ") \u00FA(" + uacute + ") " +
                "\u00E0(" + agrave + ") \u00E8(" + egrave + ") \u00EC(" + igrave + ") \u00F2(" + ograve + ") \u00F9(" + ugrave + ") " +
                "\u00E3(" + tilde_a + ") \u00F5(" + tilde_o + ") \u00E2(" + circumflex_a + ") \u00EA(" + circumflex_e + ") " +
                "\u00EE(" + circumflex_i + ") \u00F4(" + circumflex_o + ") \u00FB(" + circumflex_u + ") " +
                "consoante(" + consoante + ") <br>(" + br +") <table>(" + table + ") " + nomePagina);


                
            } else {
                System.out.println("Erro na conexão: " + responseCode);
            }

        } catch (Exception E) {
            E.printStackTrace();
        }
    }

    public static void main(String[] args) {
        String urlStr;
        String nome;
        Scanner scanner = new Scanner(System.in);

        boolean continuar = true;

        while (continuar) {
            nome = scanner.nextLine();

            if (str_cmp(nome, "FIM") != 0) {
                urlStr = scanner.nextLine();
                procurar(urlStr, nome);
            } else {
                continuar = false;
            }
        }

        scanner.close();
    }
}
