import java.util.Date;
import java.util.ArrayList;
import java.util.List;
import java.io.BufferedReader;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.text.SimpleDateFormat;
import java.util.Scanner;
import java.text.ParseException; 


// Classe principal
public class OrdenacaoMergesort {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);

        String entrada = new String();

        List<Pokemon> pokemons = LerArquivo.lerTodoArquivo("/tmp/pokemon.csv");
        List<Pokemon> selecionados = new ArrayList<>();

        // Lê as entradas do usuário
        while (!(entrada = sc.nextLine()).equals("FIM")) {
            try {
                int pokemonID = Integer.parseInt(entrada);
                Pokemon encontrado = ProcurarPokemon.procurar(pokemons, pokemonID);

                if (encontrado != null) {
                    selecionados.add(encontrado);
                }
            } catch (NumberFormatException e) {
                e.printStackTrace();
            }
        }

        OrdenacaoMergesortAlg ordenacao = new OrdenacaoMergesortAlg();

        long startTime = System.nanoTime(); // Início da contagem do tempo
        // Ordenar os Pokémon
        ordenacao.mergesort(selecionados, 0, selecionados.size() - 1);
        long endTime = System.nanoTime(); // Fim da contagem do tempo

        long tempoExecucao = endTime - startTime;

        int numComparacoes = ordenacao.getNumComparacoes();
        int numMovimentacoes = ordenacao.getNumMovimentacoes();

        // Mostrar os Pokémon selecionados
        for (Pokemon pokemon : selecionados) {
            Pokemon.printar(pokemon);
        }

        // Criar o arquivo de log
        String matricula = "840757";
        String logFileName = "840757_mergesort.txt";

        try (FileWriter writer = new FileWriter(logFileName)) {
            writer.write(matricula + "\t" + numComparacoes + "\t" + numMovimentacoes + "\t" + tempoExecucao);
        } catch (IOException e) {
            System.out.println("Erro ao escrever o arquivo de log: " + e.getMessage());
        }

        sc.close();
    }
}

// Classe de ordenação usando Mergesort
class OrdenacaoMergesortAlg {
    private int numComparacoes = 0;
    private int numMovimentacoes = 0;

    // Função de Mergesort
    public void mergesort(List<Pokemon> pokemons, int esq, int dir) {
        if (esq < dir) {
            int meio = (esq + dir) / 2;
            mergesort(pokemons, esq, meio);
            mergesort(pokemons, meio + 1, dir);
            merge(pokemons, esq, meio, dir);
        }
    }

    // Função para combinar as duas metades ordenadas
    private void merge(List<Pokemon> pokemons, int esq, int meio, int dir) {
        List<Pokemon> aux = new ArrayList<>();

        int i = esq, j = meio + 1;

        while (i <= meio && j <= dir) {
            numComparacoes++;
            if (compararPokemons(pokemons.get(i), pokemons.get(j)) <= 0) {
                aux.add(pokemons.get(i++));
            } else {
                aux.add(pokemons.get(j++));
            }
            numMovimentacoes++;
        }

        while (i <= meio) {
            aux.add(pokemons.get(i++));
            numMovimentacoes++;
        }

        while (j <= dir) {
            aux.add(pokemons.get(j++));
            numMovimentacoes++;
        }

        for (i = esq; i <= dir; i++) {
            pokemons.set(i, aux.get(i - esq));
        }
    }

    // Função para comparar Pokémons apenas pelo primeiro tipo, ignorando o segundo
    private int compararPokemons(Pokemon p1, Pokemon p2) {
        String tipo1P1 = p1.getTypes().isEmpty() ? "" : p1.getTypes().get(0);
        String tipo1P2 = p2.getTypes().isEmpty() ? "" : p2.getTypes().get(0);
        
        int comparacaoTipos = tipo1P1.compareTo(tipo1P2);
        if (comparacaoTipos != 0) {
            return comparacaoTipos;
        } else {
            return p1.getName().compareTo(p2.getName());
        }
    }

    public int getNumComparacoes() {
        return numComparacoes;
    }

    public int getNumMovimentacoes() {
        return numMovimentacoes;
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
            // Lida com a exceção, como imprimir uma mensagem de erro ou definir uma data padrão
            System.err.println("Erro ao analisar a data: " + e.getMessage());
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
            data = "ERRO na data";
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
                    i = pokemons.size();
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