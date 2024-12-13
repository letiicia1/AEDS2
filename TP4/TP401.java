import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;


//classe principal
public class TP401 
{
    private static long startTime;
    private static long endTime;
    public static int comparisonCount = 0;
 public static void main(String[] args) throws Exception 
 {
     Scanner sc = new Scanner(System.in);

        String entrada = new String();

        List<Pokemon> pokemons = LerArquivo.lerTodoArquivo("/tmp/pokemon.csv");
        //List<Pokemon> pokemons = LerArquivo.lerTodoArquivo("pokemon.csv");
        ArvoreBinaria selecionados = new ArvoreBinaria();

        
while (!(entrada = sc.nextLine()).equals("FIM")) 
{
    try 
    {
        // Converter a entrada para um número inteiro
        int pokemonID = Integer.parseInt(entrada);

        // Procurar o Pokémon na lista e adicionar aos selecionados
        Pokemon encontrado = ProcurarPokemon.procurar(pokemons, pokemonID);

        if (encontrado != null) 
        {
            selecionados.inserir(encontrado);
        } 
       
    } 
    catch (NumberFormatException e) 
    {
        e.printStackTrace();
    }
}

    startTime = System.nanoTime();

while (!(entrada = sc.nextLine()).equals("FIM")) 
{
    try 
    {
        
        String pokemonName = entrada;

        System.out.println(pokemonName);
        System.out.print("=>raiz ");

        if (selecionados.pesquisar(pokemonName)) 
        {
            System.out.println("SIM");
        } 
        else
        {
            System.out.println("NAO");
        }
       
    } 
    catch (NumberFormatException e) 
    {
        e.printStackTrace();
    }
}

    endTime = System.nanoTime();
    sc.close();

    criarLog("840757", (endTime - startTime) / 1e9, comparisonCount); 
}

    private static void criarLog(String matricula, double tempoExecucao, int comparacoes) 
    {
        String nomeArquivo = matricula + "_arvoreBinaria.txt";
        try (BufferedWriter writer = new BufferedWriter(new FileWriter(nomeArquivo))) 
        {
            writer.write(matricula + "\t" + tempoExecucao + "\t" + comparacoes);
        } 
        catch (IOException e) 
        {
            e.printStackTrace();
        }
}
}

class No {
    public Pokemon elemento; // Conteudo do no.
    public No esq, dir;  // Filhos da esq e dir.

    /**
     * Construtor da classe.
     * @param elemento Conteudo do no.
     */
    public No(Pokemon elemento) {
        this(elemento, null, null);
    }

    /**
     * Construtor da classe.
     * @param elemento Conteudo do no.
     * @param esq No da esquerda.
     * @param dir No da direita.
     */
    public No(Pokemon elemento, No esq, No dir) 
    {
        this.elemento = elemento;
        this.esq = esq;
        this.dir = dir;
    }
}
//Classe da arvore 
class ArvoreBinaria
{
    private No raiz; // Raiz da arvore.

	/**
	 * Construtor da classe.
	 */
	public ArvoreBinaria() {
		raiz = null;
	}

	/**
	 * Metodo publico iterativo para pesquisar elemento.
	 * @param x Elemento que sera procurado.
	 * @return <code>true</code> se o elemento existir,
	 * <code>false</code> em caso contrario.
	 */
	public boolean pesquisar(String x) {
		return pesquisar(x, raiz);
	}

	/**
	 * Metodo privado recursivo para pesquisar elemento.
	 * @param x Elemento que sera procurado.
	 * @param i No em analise.
	 * @return <code>true</code> se o elemento existir,
	 * <code>false</code> em caso contrario.
	 */
	private boolean pesquisar(String x, No i) {
      boolean resp;

      TP401.comparisonCount++;

		if (i == null) {
         resp = false;

      } else if (x.equals(i.elemento.getName())) {
         resp = true;

      } else if (x.compareTo(i.elemento.getName()) < 0) {
        System.out.print("esq ");
         resp = pesquisar(x, i.esq);

      } else {
        System.out.print("dir ");
         resp = pesquisar(x, i.dir);
      }
      return resp;
	}

	
	/**
	 * Metodo publico iterativo para inserir elemento.
	 * @param x Elemento a ser inserido.
	 * @throws Exception Se o elemento existir.
	 */
	public void inserir(Pokemon x) throws Exception {
		raiz = inserir(x, raiz);
	}

	private No inserir(Pokemon x, No i) throws Exception {
		if (i == null) {
         i = new No(x);

      } else if (x.getName().compareTo(i.elemento.getName()) < 0) {
         i.esq = inserir(x, i.esq);

      } else if (x.getName().compareTo(i.elemento.getName()) > 0) {
         i.dir = inserir(x, i.dir);

      } else {
         throw new Exception("outo ao inserir!");
      }

		return i;
	}
}


//classe do Pokemon 
class Pokemon 
{
    //definir dados
    private int id;
    private int generation;
    private String name;
    private String description;
    private ArrayList<String> types;
    private ArrayList<String> abilities;
    private double weight;
    private double height;
    private int captureRate;
    private boolean isLegendary;  
    private Date captureDate;
    
    //iniciar construtor
    public Pokemon () 
    {
     id = 0;
     generation = 0;
     name = " ";
     description = " ";
      types = new ArrayList<>();
      abilities = new ArrayList<>();
      weight = 0;
      height = 0;
     captureRate = 0;
      isLegendary = false;  
      captureDate = new Date();

    }

    //construtor para inserir dados
    public Pokemon (int id, int generation, String name, String description, ArrayList<String> types, 
    ArrayList<String> abilities, double weight, double height, int captureRate, 
    boolean isLegendary, Date captureDate)
    {
        this.id = id;
        this.generation = generation;
        this.name = name;
        this.description = description;
        this.types = types;
        this.abilities = abilities;
        this.weight = weight;
        this.height = height;
        this.captureRate = captureRate;
        this.isLegendary = isLegendary;
        this.captureDate = captureDate;
    }

    public Pokemon (String[] partes)
    {
        

        for(int i = 0; i < partes.length; i++)
        {
            if(partes[i].isEmpty())
            {
                partes[i] = "0";
            }
        }
        this.id = Integer.parseInt(partes[0]);

        this.generation = Integer.parseInt(partes[1]);
        
        this.name = partes[2];

        this.description = partes[3];

        this.types = new ArrayList<>();
        String tmp = "'";
        tmp += partes[4];
        tmp += "'";
        this.types.add(tmp);
        if(partes[5] != "0")
        {
            tmp = "'";
            tmp += partes[5];
            tmp += "'";
            this.types.add(tmp);
        }

         //arrumar formato da string
         String temp = " ";
         for(int i = 0; i < partes[6].length(); i++)
         {
            char c = partes[6].charAt(i);
            if(c != '\"' && c != '[' && c != ']')
            {
                temp += c;
            }
          
         }

         String[] tmp1 = temp.split(",");
         this.abilities = new ArrayList<>();

         for (int i = 0; i < tmp1.length; i++) 
         {
            abilities.add(tmp1[i].trim());
        }

        this.weight = Double.parseDouble(partes[7]);
        this.height = Double.parseDouble(partes[8]);

        this.captureRate = Integer.parseInt(partes[9]);

        if (partes[10].equals("1")) 
        {

            this.isLegendary = true;
        }
        else
        {
            this.isLegendary = false;
        }

        SimpleDateFormat formatodaData = new SimpleDateFormat("dd/MM/yyyy");

        try {
            // Verifica se partes[11] não é nulo ou vazio
            if (partes[11] != null && !partes[11].isEmpty()) {
                this.captureDate = formatodaData.parse(partes[11]);
            } else {
                this.captureDate = null; 
            }
        } catch (ParseException e) {
            // Lida com a exceção, como imprimir uma mensagem de outo ou definir uma data padrão
            System.out.println("outo ao analisar a data: " + e.getMessage());
            this.captureDate = null; // ou outra lógica que você queira
        }

    }
      //Métodos Get e Set
      public int getId() { return id; }
      public void setId(int id) { this.id = id; }
  
      public int getGeneration() { return generation; }
      public void setGeneration(int generation) { this.generation = generation; }
  
      public String getName() { return name; }
      public void setName(String name) { this.name = name; }
  
      public String getDescription() { return description; }
      public void setDescription(String description) { this.description = description; }
  
      public ArrayList<String> getTypes() { return types; }
      public void setTypes(ArrayList<String> types) { this.types = types; }
  
      public ArrayList<String> getAbilities() { return abilities; }
      public void setAbilities(ArrayList<String> abilities) { this.abilities = abilities; }
  
      public double getWeight() { return weight; }
      public void setWeight(double weight) { this.weight = weight; }
  
      public double getHeight() { return height; }
      public void setHeight(double height) { this.height = height; }
  
      public int getCaptureRate() { return captureRate; }
      public void setCaptureRate(int captureRate) { this.captureRate = captureRate; }
  
      public boolean getIsLegendary() { return isLegendary; }
      public void setIsLegendary(boolean isLegendary) { this.isLegendary = isLegendary; }
  
      public Date getCaptureDate() { return captureDate; }
      public void setCaptureDate(Date captureDate) { this.captureDate = captureDate; }

       //metodo clone
       // Construtor de cópia
    public Pokemon(Pokemon original) 
    {
        this.id = original.id;
        this.generation = original.generation;
        this.name = original.name;
        this.description = original.description;
        
        
        this.types = new ArrayList<>(original.types);
        this.abilities = new ArrayList<>(original.abilities);
        
        this.weight = original.weight;
        this.height = original.height;
        this.captureRate = original.captureRate;
        this.isLegendary = original.isLegendary;
        
        
        this.captureDate = (Date) original.captureDate.clone();
    }
     // Cria e retorna um novo objeto Pokemon baseado no objeto atual (this)
      public Pokemon clonar() {
       
        return new Pokemon(this); 
    }

    public static void printar(Pokemon pokemon)
    {
        //definir formato da saida da data
        SimpleDateFormat formatoSaida = new SimpleDateFormat("dd/MM/yyyy");

        String data = " ";

        //verificar se é válida
        if(pokemon.captureDate != null)
        {
            data = formatoSaida.format(pokemon.captureDate);
        }
        else
        {
            data = "outO na data";
        }

        //printar dados
        System.out.println( "[#" + pokemon.id + " -> " + pokemon.name + ": " + pokemon.description + " - " + pokemon.types + " - " + pokemon.abilities + 
        " - " + pokemon.weight + "kg - " + pokemon.height + "m - " + pokemon.captureRate + "% - " + pokemon.isLegendary + " - " + pokemon.generation + 
        " gen] - " + data);
    }

}

class ProcurarPokemon 
{
    // Função estática que busca um Pokémon pelo ID
    public static Pokemon procurar(List<Pokemon> pokemons, int id)
    {
        Pokemon foundPokemon = null;

            for (int i = 0; i < pokemons.size(); i++) 
            { 
                //pegar o pokemon da lista na posicao i
                Pokemon pokemon = pokemons.get(i); 
                if (pokemon.getId() == id) 
                { // Verifica se o ID do Pokémon é igual ao ID procurado
                    foundPokemon =  pokemon; // Retorna o Pokémon encontrado
                }
            }
            return foundPokemon; // Retorna null se o Pokémon não for encontrado
    }
}
//classe para ler o arquivo do Pokemon

class LerArquivo 
{
    // Criar uma lista de objetos com todos os pokemons
    public static List<Pokemon> lerTodoArquivo(final String fileName) 
    {
        // Criar a lista de pokemons
        List<Pokemon> pokemons = new ArrayList<>();

        try 
        {
            BufferedReader br = new BufferedReader(new FileReader(fileName));

            // Pular o cabeçalho
            br.readLine();

            // Ler linha por linha
            String Line;
            while ((Line = br.readLine()) != null) 
            {
                // Trocar vírgula por espaço
                Line = trocarVirgula(Line);

                // Criar o objeto Pokemon com os dados da linha
                Pokemon pokemon = new Pokemon(Line.split(";"));
                pokemons.add(pokemon);
            }

            br.close();
        } 
        catch (Exception e) 
        {
            e.printStackTrace();
            pokemons = null;
        }

        return pokemons;
    }

    // Trocar a vírgula para outro caractere, pois existem vírgulas em outros lugares e não somente para dividir
    private static String trocarVirgula(String line) 
    {
        // Substituir para char[] para ser possível a manipulação
        char[] array = line.toCharArray();

        // Verificar se está com aspas
        boolean aspas = false;

        // Percorrer todo o arranjo
        for (int i = 0; i < array.length; i++) 
        {
            // Se as aspas forem falsas e for uma vírgula, trocar por espaço
            if (!aspas && array[i] == ',') 
            {
                array[i] = ';';
            } 
            // Abrir e fechar aspas
            else if (array[i] == '"') 
            {
                aspas = !aspas;
            }
        }

        // Converter novamente para String
        return new String(array);
    }
}

