#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h> // Para usleep() em sistemas UNIX-like. Para Windows, pode ser necessário <windows.h>

// --- Constantes do Jogo ---
#define TAM 4
#define INICIO 0 // Representa o espaço vazio

// --- Variáveis Globais ---
int tabuleiro[TAM][TAM];
int linhaVazia;
int colunaVazia;

// --- Protótipos das Funções ---
void inicializar();
void imprimirTabuleiro();
bool posicaoValida(int l, int c);
bool mover(int numero);
void embaralhar();
bool verificarVitoria();
bool verificarCombinacaoDoEmbaralhamento(); // Bônus, para verificar se um estado é solucionável
void jogar();
void menu();

/**
 * @brief Preenche o tabuleiro com os números de 1 a 15 e define a posição inicial do espaço vazio.
 */
void inicializar() {
    int valor = 1;
    for (int i = 0; i < TAM; i++) {
        for (int j = 0; j < TAM; j++) {
            tabuleiro[i][j] = valor++;
        }
    }
    // Define a última posição como o espaço vazio
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

    printf("────-ˋˏ ༻❁༺ ˎˊ-────\n");
    printf("ᯓ★   15 Puzzle   ★ᯓ\n");
    printf("────-ˋˏ ༻❁༺ ˎˊ-────\n");

    for (int i = 0; i < TAM; i++) {
        for (int j = 0; j < TAM; j++) {
            if (tabuleiro[i][j] == INICIO) {
                printf(" __ "); // Espaço vazio destacado
            } else {
                printf("%3d ", tabuleiro[i][j]);
            }
        }
        printf("\n\n");
    }
     printf("──────────────────────\n");
}

/**
 * @brief Verifica se uma dada posição (linha, coluna) está dentro dos limites do tabuleiro.
 * @param l Linha a ser verificada.
 * @param c Coluna a ser verificada.
 * @return true se a posição for válida, false caso contrário.
 */
bool posicaoValida(int l, int c) {
    return l >= 0 && l < TAM && c >= 0 && c < TAM;
}

/**
 * @brief Move a peça com o número especificado para o espaço vazio, se o movimento for válido.
 * @param numero O número da peça a ser movida.
 * @return true se o movimento foi realizado, false caso contrário.
 */
bool mover(int numero) {
    int i, j, linhaNumero, colunaNumero;
    bool achou = false;

    // Encontra a posição do número que o jogador quer mover
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

    if (!achou) return false; // Número não está no tabuleiro

    // Verifica se a peça está adjacente ao espaço vazio
    if ((linhaNumero == linhaVazia && (colunaNumero == colunaVazia + 1 || colunaNumero == colunaVazia - 1)) ||
        (colunaNumero == colunaVazia && (linhaNumero == linhaVazia + 1 || linhaNumero == linhaVazia - 1))) {

        // Troca a peça com o espaço vazio
        tabuleiro[linhaVazia][colunaVazia] = numero;
        tabuleiro[linhaNumero][colunaNumero] = INICIO;

        // Atualiza a nova posição do espaço vazio
        linhaVazia = linhaNumero;
        colunaVazia = colunaNumero;
        return true;
    }

    return false; // Movimento inválido
}

/**
 * @brief Embaralha o tabuleiro realizando um grande número de movimentos aleatórios.
 * Isso garante que o quebra-cabeça resultante seja sempre solucionável.
 */
void embaralhar() {
    int movimentos = 150; // Aumenta a aleatoriedade
    srand(time(NULL));

    for (int k = 0; k < movimentos; k++) {
        int direcao = rand() % 4; // 0: Cima, 1: Baixo, 2: Esquerda, 3: Direita
        int novaLinha = linhaVazia;
        int novaColuna = colunaVazia;

        if (direcao == 0) novaLinha--;      // Mover para cima
        else if (direcao == 1) novaLinha++; // Mover para baixo
        else if (direcao == 2) novaColuna--;// Mover para a esquerda
        else novaColuna++;                  // Mover para a direita

        // Se a nova posição for válida, troca o espaço vazio com a peça vizinha
        if (posicaoValida(novaLinha, novaColuna)) {
            tabuleiro[linhaVazia][colunaVazia] = tabuleiro[novaLinha][novaColuna];
            tabuleiro[novaLinha][novaColuna] = INICIO;
            linhaVazia = novaLinha;
            colunaVazia = novaColuna;
        }
    }
}

/**
 * @brief Verifica se o tabuleiro está na configuração de vitória (números em ordem crescente).
 * @return true se o jogador venceu, false caso contrário.
 */
bool verificarVitoria() {
    int valorEsperado = 1;
    for (int i = 0; i < TAM; i++) {
        for (int j = 0; j < TAM; j++) {
            // Verifica se não é a última célula, que deve ser o espaço vazio
            if (i == TAM - 1 && j == TAM - 1) {
                return tabuleiro[i][j] == INICIO;
            }
            if (tabuleiro[i][j] != valorEsperado) {
                return false; // Fora de ordem
            }
            valorEsperado++;
        }
    }
    return true; // Se chegou aqui, está tudo em ordem
}

/**
 * @brief (Bônus) Conta o número de inversões para verificar se o quebra-cabeça tem solução.
 * Uma inversão é um par de peças (a, b) tal que 'a' aparece antes de 'b' mas a > b.
 * @return O número de inversões.
 */
int contarInversoes() {
    int arr[TAM * TAM];
    int k = 0;
    for (int i = 0; i < TAM; i++) {
        for (int j = 0; j < TAM; j++) {
            arr[k++] = tabuleiro[i][j];
        }
    }

    int inv_count = 0;
    for (int i = 0; i < TAM * TAM - 1; i++) {
        for (int j = i + 1; j < TAM * TAM; j++) {
            if (arr[i] != INICIO && arr[j] != INICIO && arr[i] > arr[j]) {
                inv_count++;
            }
        }
    }
    return inv_count;
}

/**
 * @brief (Bônus) Verifica se a combinação atual do tabuleiro é solucionável.
 * A regra de solubilidade depende do tamanho do grid (par/ímpar), do número de inversões e da
 * posição do espaço vazio.
 * @return true se for solucionável, false caso contrário.
 */
bool verificarCombinacaoDoEmbaralhamento() {
    int invCount = contarInversoes();
    // Para um grid de tamanho PAR (como 4x4):
    // Se a linha do espaço vazio (contando de baixo para cima, 1-indexado) for PAR, o número de inversões deve ser ÍMPAR.
    // Se a linha do espaço vazio for ÍMPAR, o número de inversões deve ser PAR.
    int linhaVaziaAPartirDeBaixo = TAM - linhaVazia;
    if (linhaVaziaAPartirDeBaixo % 2 == 0) { // Linha par
        return (invCount % 2 != 0); // Inversões ímpares
    } else { // Linha ímpar
        return (invCount % 2 == 0); // Inversões pares
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
            printf("\n🎉 PARABÉNS! VOCÊ VENCEU! 🎉\n\n");
            // system("pause") para Windows
            #ifdef _WIN32
                system("pause");
            #else
                printf("Pressione Enter para continuar...");
                getchar(); // Pausa para o jogador ver a mensagem
                getchar();
            #endif
            break; // Sai do loop do jogo e volta para o menu
        }

        printf("Digite o número que deseja mover (0 para voltar ao menu): ");
        if (scanf("%d", &numero) != 1) { // Lida com entrada não-numérica
             while (getchar() != '\n'); // Limpa o buffer de entrada
             numero = -1; // Força a mensagem de número inválido
        }


        if (numero == 0) {
            printf("Voltando ao menu...\n");
            usleep(1000 * 1000); // Espera 1 segundo
            break;
        }

        if (numero < 1 || numero > 15) {
            printf("Número inválido! Digite um valor entre 1 e 15.\n");
            usleep(1500 * 1000);
            continue;
        }

        if (!mover(numero)) {
            printf("Movimento inválido! A peça não está ao lado do espaço vazio.\n");
             usleep(1500 * 1000);
        }
    }
}

/**
 * @brief Exibe o menu principal e gerencia a navegação do usuário.
 */
void menu() {
    int escolha = 0;
    while (true) {
        // Limpa a tela
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif

        printf("──────────୨ৎ─────────\n");
        printf("ᯓ★「 ✦ Menu ✦ 」★ᯓ\n");
        printf("──────────୨ৎ─────────\n");
        printf("✷ 1. Jogar\n");
        printf("✷ 2. Sair\n");
        printf("──────────────────────\n");
        printf("Escolha uma opção: ");

        if (scanf("%d", &escolha) != 1) {
            while (getchar() != '\n'); // Limpa o buffer de entrada se não for número
            escolha = -1; // Força o default
        }


        switch (escolha) {
            case 1:
                jogar();
                break;
            case 2:
                printf("Saindo....( ˶°ㅁ°) !! Até depois\n");
                usleep(1500 * 1000);
                return; // Sai da função menu e termina o programa
            default:
                printf("Escolha Inválida! As opções disponíveis são 1 ou 2!\n");
                usleep(1500 * 1000); // Espera 1.5 segundos
                break;
        }
    }
}

/**
 * @brief Função principal que inicia o programa.
 */
int main() {
    menu();
    return 0;
}
