import java.util.Scanner;

public class TP311 {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);

        int n = sc.nextInt();
        int linha, coluna, linha2, coluna2;

        for (int i = 0; i < n; i++) {
            linha = sc.nextInt();
            coluna = sc.nextInt();
            Matriz matriz1 = new Matriz(linha, coluna);
            matriz1.preencher(sc);

            linha2 = sc.nextInt();
            coluna2 = sc.nextInt();
            Matriz matriz2 = new Matriz(linha2, coluna2);
            matriz2.preencher(sc);

            // Exibir diagonais da primeira matriz
           // System.out.println("Diagonal Principal:");
            matriz1.mostrarDiagonalPrincipal();
            //System.out.println("Diagonal Secundária:");
            matriz1.mostrarDiagonalSecundaria();

            // Somar e multiplicar matrizes, se possível
            Matriz soma = matriz1.soma(matriz2);
            if (soma != null) {
               // System.out.println("Matriz Soma:");
                soma.mostrar();
            } 
            
            
            Matriz multiplicacao = matriz1.multiplicacao(matriz2);
            if (multiplicacao != null) {
               // System.out.println("Matriz Multiplicação:");
                multiplicacao.mostrar(); } 
            
        
                }

        sc.close();
    }
}

class Celula {
    public int elemento;
    public Celula inf, sup, esq, dir;

    public Celula() {
        this(0);
    }

    public Celula(int elemento) {
        this(elemento, null, null, null, null);
    }

    public Celula(int elemento, Celula inf, Celula sup, Celula esq, Celula dir) {
        this.elemento = elemento;
        this.inf = inf;
        this.sup = sup;
        this.esq = esq;
        this.dir = dir;
    }
}

class Matriz {
    private Celula inicio;
    private int linha, coluna;

    public Matriz(int linha, int coluna) {
        this.linha = linha;
        this.coluna = coluna;
    
        inicio = new Celula();
        Celula linhaAtual = inicio;
        Celula colunaAtual;
    
        // Preenchendo a matriz linha por linha
        for (int i = 0; i < linha; i++) {
            colunaAtual = linhaAtual;
    
            for (int j = 1; j < coluna; j++) {
                // Cria a nova célula à direita
                colunaAtual.dir = new Celula();
                colunaAtual.dir.esq = colunaAtual;
                colunaAtual = colunaAtual.dir;
    
                // Conecta a nova célula com a célula da linha anterior, se houver
                if (i > 0) {
                    colunaAtual.sup = colunaAtual.esq.sup.dir;
                    colunaAtual.sup.inf = colunaAtual;
                }
            }
    
            // Avança para a próxima linha
            if (i < linha - 1) {
                linhaAtual.inf = new Celula();
                linhaAtual.inf.sup = linhaAtual;
                linhaAtual = linhaAtual.inf;
            }
        }
    }
    
    public void preencher(Scanner sc) {
        Celula linhaAtual = inicio;
        for (int i = 0; i < linha; i++) {
            Celula colunaAtual = linhaAtual;
            for (int j = 0; j < coluna; j++) {
                // Preenche o elemento da célula
                colunaAtual.elemento = sc.nextInt();
                // Avança para a célula da próxima coluna
                if (colunaAtual.dir != null) {
                    colunaAtual = colunaAtual.dir;
                }
            }
            // Avança para a próxima linha
            if (linhaAtual.inf != null) {
                linhaAtual = linhaAtual.inf;
            }
        }
    }
    

    public Matriz soma(Matriz m) {
        if (this.linha != m.linha || this.coluna != m.coluna) return null;
        Matriz resp = new Matriz(this.linha, this.coluna);

        Celula linhaA = this.inicio, linhaB = m.inicio, linhaC = resp.inicio;
        while (linhaA != null) {
            Celula colunaA = linhaA, colunaB = linhaB, colunaC = linhaC;
            while (colunaA != null) {
                colunaC.elemento = colunaA.elemento + colunaB.elemento;
                colunaA = colunaA.dir;
                colunaB = colunaB.dir;
                colunaC = colunaC.dir;
            }
            linhaA = linhaA.inf;
            linhaB = linhaB.inf;
            linhaC = linhaC.inf;
        }
        return resp;
    }

    public Matriz multiplicacao(Matriz m) {
        if (this.coluna != m.linha) return null; // Verifica a compatibilidade das dimensões para multiplicação
        Matriz resp = new Matriz(this.linha, m.coluna);
    
        Celula linhaA = this.inicio;
        
        for (int i = 0; i < this.linha; i++) {
            Celula colunaC = resp.inicio;
    
            // Move colunaC para a linha correta na matriz resp
            for (int x = 0; x < i; x++) {
                colunaC = colunaC.inf;
            }
        
            for (int j = 0; j < m.coluna; j++) {
                Celula linhaB = m.inicio;
                int soma = 0;
    
                // Move linhaB para a coluna correta
                for (int y = 0; y < j; y++) {
                    linhaB = linhaB.dir;
                }
    
                Celula colunaA = linhaA;
    
                for (int k = 0; k < this.coluna; k++) {
                    if (colunaA != null && linhaB != null) {
                        soma += colunaA.elemento * linhaB.elemento;
                        colunaA = colunaA.dir;
                        linhaB = linhaB.inf;
                    }
                }
    
                colunaC.elemento = soma;
                colunaC = colunaC.dir; // Avança para a próxima célula na linha de resp
            }
    
            linhaA = linhaA.inf; // Avança para a próxima linha de 'this'
        }
    
        return resp;
    }

    
    public boolean isQuadrada() {
        return this.linha == this.coluna;
    }

    public void mostrarDiagonalPrincipal() {
        if (isQuadrada()) {
            Celula atual = inicio;
            for (int i = 0; i < linha; i++) {
                System.out.print(atual.elemento + " ");
                if (atual.inf != null) atual = atual.inf.dir;
            }
            System.out.println();
        }
    }

    public void mostrarDiagonalSecundaria() {
        if (isQuadrada()) {
            Celula atual = inicio;
            for (int i = 0; i < coluna - 1; i++) atual = atual.dir;
            for (int i = 0; i < linha; i++) {
                System.out.print(atual.elemento + " ");
                if (atual.inf != null) atual = atual.inf.esq;
            }
            System.out.println();
        }
    }

    public void mostrar() {
        Celula linhaAtual = inicio;
        while (linhaAtual != null) {
            Celula colunaAtual = linhaAtual;
            while (colunaAtual != null) {
                System.out.print(colunaAtual.elemento + " ");
                colunaAtual = colunaAtual.dir;
            }
            System.out.println();
            linhaAtual = linhaAtual.inf;
        }
    }
}
