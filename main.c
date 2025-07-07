#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h> // Para usleep() em sistemas UNIX-like. Para Windows, pode ser necessario <windows.h>

// --- Constantes do Jogo ---
#define TAM 4
#define INICIO 0 // Representa o espaco vazio

// --- Variaveis Globais ---
int tabuleiro[TAM][TAM];
int linhaVazia;
int colunaVazia;

// --- Prototipos das Funcoes ---
void inicializar();
void imprimirTabuleiro();
bool posicaoValida(int l, int c);
bool mover(int numero);
void embaralhar();
bool verificarVitoria();
bool verificarCombinacaoDoEmbaralhamento(); // Bonus: verifica se o estado e solucionavel
void jogar();
void menu();

/**
 * @brief Preenche o tabuleiro com os numeros de 1 a 15 e define a posicao inicial do espaco vazio.
 */
void inicializar() {
    int valor = 1;
    int i, j;
    for (i = 0; i < TAM; i++) {
        for (j = 0; j < TAM; j++) {
            tabuleiro[i][j] = valor++;
        }
    }
    // Define a ultima posicao como o espaco vazio
    linhaVazia = TAM - 1;
    colunaVazia = TAM - 1;
    tabuleiro[linhaVazia][colunaVazia] = INICIO;
}

/**
 * @brief Limpa a tela e imprime o estado atual do tabuleiro com um design.
 */
void imprimirTabuleiro() {
    // system("cls") para Windows, system("clear") para Linux/Mac
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif

    printf("----------------------\n");
    printf("*   15 Puzzle   *\n");
    printf("----------------------\n");

    int i, j;
    for (i = 0; i < TAM; i++) {
        for (j = 0; j < TAM; j++) {
            if (tabuleiro[i][j] == INICIO) {
                printf(" __ "); // Espaco vazio destacado
            } else {
                printf("%3d ", tabuleiro[i][j]);
            }
        }
        printf("\n\n");
    }
    printf("----------------------\n");
}

/**
 * @brief Verifica se uma dada posicao (linha, coluna) esta dentro dos limites do tabuleiro.
 * @param l Linha a ser verificada.
 * @param c Coluna a ser verificada.
 * @return true se a posicao for valida, false caso contrario.
 */
bool posicaoValida(int l, int c) {
    return l >= 0 && l < TAM && c >= 0 && c < TAM;
}

/**
 * @brief Move a peca com o numero especificado para o espaco vazio, se o movimento for valido.
 * @param numero O numero da peca a ser movida.
 * @return true se o movimento foi realizado, false caso contrario.
 */
bool mover(int numero) {
    int i, j, linhaNumero, colunaNumero;
    bool achou = false;

    // Encontra a posicao do numero que o jogador quer mover
    for (i = 0; i < TAM; i++) {
        for (j = 0; j < TAM; j++) {
            if (tabuleiro[i][j] == numero) {
                linhaNumero = i;
                colunaNumero = j;
                achou = true;
                break;
            }
        }
        if (achou) break;
    }

    if (!achou) return false; // Numero nao esta no tabuleiro

    // Verifica se a peca esta adjacente ao espaco vazio
    if ((linhaNumero == linhaVazia && (colunaNumero == colunaVazia + 1 || colunaNumero == colunaVazia - 1)) ||
        (colunaNumero == colunaVazia && (linhaNumero == linhaVazia + 1 || linhaNumero == linhaVazia - 1))) {

        // Troca a peca com o espaco vazio
        tabuleiro[linhaVazia][colunaVazia] = numero;
        tabuleiro[linhaNumero][colunaNumero] = INICIO;

        // Atualiza a nova posicao do espaco vazio
        linhaVazia = linhaNumero;
        colunaVazia = colunaNumero;
        return true;
    }

    return false; // Movimento invalido
}

/**
 * @brief Embaralha o tabuleiro realizando varios movimentos aleatorios validos.
 * Isso garante que o quebra-cabeca resultante seja sempre solucionavel.
 */
void embaralhar() {
    int movimentos = 150; // Aumenta a aleatoriedade
    srand(time(NULL));

    int k;
    for (k = 0; k < movimentos; k++) {
        int direcao = rand() % 4; // 0: Cima, 1: Baixo, 2: Esquerda, 3: Direita
        int novaLinha = linhaVazia;
        int novaColuna = colunaVazia;

        if (direcao == 0) novaLinha--;      // Mover para cima
        else if (direcao == 1) novaLinha++; // Mover para baixo
        else if (direcao == 2) novaColuna--;// Mover para a esquerda
        else novaColuna++;                  // Mover para a direita

        // Se a nova posicao for valida, troca o espaco vazio com a peca vizinha
        if (posicaoValida(novaLinha, novaColuna)) {
            tabuleiro[linhaVazia][colunaVazia] = tabuleiro[novaLinha][novaColuna];
            tabuleiro[novaLinha][novaColuna] = INICIO;
            linhaVazia = novaLinha;
            colunaVazia = novaColuna;
        }
    }
}

/**
 * @brief Verifica se o tabuleiro esta na configuracao de vitoria (numeros em ordem crescente).
 * @return true se o jogador venceu, false caso contrario.
 */
bool verificarVitoria() {
    int valorEsperado = 1;
    int i, j;
    for (i = 0; i < TAM; i++) {
        for (j = 0; j < TAM; j++) {
            if (i == TAM - 1 && j == TAM - 1) {
                return tabuleiro[i][j] == INICIO;
            }
            if (tabuleiro[i][j] != valorEsperado) {
                return false;
            }
            valorEsperado++;
        }
    }
    return true;
}

/**
 * @brief Conta o numero de inversoes para verificar se o quebra-cabeca tem solucao.
 * Uma inversao e um par de pecas (a, b) tal que 'a' aparece antes de 'b', mas a > b.
 * @return O numero de inversoes.
 */
int contarInversoes() {
    int arr[TAM * TAM];
    int k = 0;
    int i, j;

    for (i = 0; i < TAM; i++) {
        for (j = 0; j < TAM; j++) {
            arr[k++] = tabuleiro[i][j];
        }
    }

    int inv_count = 0;
    for (i = 0; i < TAM * TAM - 1; i++) {
        for (j = i + 1; j < TAM * TAM; j++) {
            if (arr[i] != INICIO && arr[j] != INICIO && arr[i] > arr[j]) {
                inv_count++;
            }
        }
    }
    return inv_count;
}

/**
 * @brief Verifica se a combinacao atual do tabuleiro e solucionavel.
 * A regra depende do tamanho do grid, do numero de inversoes e da posicao do espaco vazio.
 * @return true se for solucionavel, false caso contrario.
 */
bool verificarCombinacaoDoEmbaralhamento() {
    int invCount = contarInversoes();
    int linhaVaziaAPartirDeBaixo = TAM - linhaVazia;
    if (linhaVaziaAPartirDeBaixo % 2 == 0) {
        return (invCount % 2 != 0);
    } else {
        return (invCount % 2 == 0);
    }
}

/**
 * @brief Controla o fluxo principal do jogo: inicializa, embaralha e gerencia o loop de jogadas.
 */
void jogar() {
    inicializar();
    embaralhar();

    int numero;

    while (true) {
        imprimirTabuleiro();

        if (verificarVitoria()) {
            printf("\nPARABENS! VOCE VENCEU!\n\n");
            #ifdef _WIN32
                system("pause");
            #else
                printf("Pressione Enter para continuar...");
                getchar();
                getchar();
            #endif
            break;
        }

        printf("Digite o numero que deseja mover (0 para voltar ao menu): ");
        if (scanf("%d", &numero) != 1) {
             while (getchar() != '\n');
             numero = -1;
        }

        if (numero == 0) {
            printf("Voltando ao menu...\n");
            usleep(1000000);
            break;
        }

        if (numero < 1 || numero > 15) {
            printf("Numero invalido! Digite um valor entre 1 e 15.\n");
            usleep(1500000);
            continue;
        }

        if (!mover(numero)) {
            printf("Movimento invalido! A peca nao esta ao lado do espaco vazio.\n");
            usleep(1500000);
        }
    }
}

/**
 * @brief Exibe o menu principal e gerencia a navegacao do usuario.
 */
void menu() {
    int escolha = 0;
    while (true) {
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif

        printf("----------------------\n");
        printf("*   Menu Principal   *\n");
        printf("----------------------\n");
        printf("-> 1. Jogar\n");
        printf("-> 2. Sair\n");
        printf("----------------------\n");
        printf("Escolha uma opcao: ");

        if (scanf("%d", &escolha) != 1) {
            while (getchar() != '\n');
            escolha = -1;
        }

        switch (escolha) {
            case 1:
                jogar();
                break;
            case 2:
                printf("Saindo... Ate logo!\n");
                usleep(1500000);
                return;
            default:
                printf("Escolha invalida! As opcoes disponiveis sao 1 ou 2.\n");
                usleep(1500000);
                break;
        }
    }
}

/**
 * @brief Funcao principal que inicia o programa.
 */
int main() {
    menu();
    return 0;
}

