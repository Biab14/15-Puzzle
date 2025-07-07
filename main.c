#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h> // Para usleep()

#define TAM 4
#define INICIO 0

int tabuleiro[TAM][TAM];
int linhaVazia;
int colunaVazia;

void inicializar();
void imprimirTabuleiro();
bool posicaoValida(int l, int c);
bool mover(int numero);
void embaralhar();
bool verificarVitoria();
int contarInversoes();
bool verificarCombinacaoDoEmbaralhamento();
void jogar();
void menu();

void inicializar() {
    int valor = 1;
    for (int i = 0; i < TAM; i++) {
        for (int j = 0; j < TAM; j++) {
            tabuleiro[i][j] = valor++;
        }
    }
    linhaVazia = TAM - 1;
    colunaVazia = TAM - 1;
    tabuleiro[linhaVazia][colunaVazia] = INICIO;
}

void imprimirTabuleiro() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif

    printf("\033[1;33m────-ˋˏ ༻❁༺ ˎˊ-────\n");
    printf("ᯓ★   15 Puzzle   ★ᯓ\n");
    printf("────-ˋˏ ༻❁༺ ˎˊ-────\033[0m\n");

    for (int i = 0; i < TAM; i++) {
        for (int j = 0; j < TAM; j++) {
            if (tabuleiro[i][j] == INICIO) {
                printf("\033[1;37m __ \033[0m");
            } else {
                printf("\033[1;36m%3d \033[0m", tabuleiro[i][j]);
            }
        }
        printf("\n\n");
    }
    printf("\033[1;33m────────────────────\033[0m\n");
}

bool posicaoValida(int l, int c) {
    return l >= 0 && l < TAM && c >= 0 && c < TAM;
}

bool mover(int numero) {
    int linhaNumero, colunaNumero;
    bool achou = false;

    for (int i = 0; i < TAM && !achou; i++) {
        for (int j = 0; j < TAM; j++) {
            if (tabuleiro[i][j] == numero) {
                linhaNumero = i;
                colunaNumero = j;
                achou = true;
                break;
            }
        }
    }

    if (!achou) return false;

    if ((linhaNumero == linhaVazia && abs(colunaNumero - colunaVazia) == 1) ||
        (colunaNumero == colunaVazia && abs(linhaNumero - linhaVazia) == 1)) {

        tabuleiro[linhaVazia][colunaVazia] = numero;
        tabuleiro[linhaNumero][colunaNumero] = INICIO;
        linhaVazia = linhaNumero;
        colunaVazia = colunaNumero;
        return true;
    }

    return false;
}

void embaralhar() {
    int movimentos = 150;
    srand(time(NULL));

    for (int k = 0; k < movimentos; k++) {
        int direcao = rand() % 4;
        int novaLinha = linhaVazia;
        int novaColuna = colunaVazia;

        if (direcao == 0) novaLinha--;
        else if (direcao == 1) novaLinha++;
        else if (direcao == 2) novaColuna--;
        else novaColuna++;

        if (posicaoValida(novaLinha, novaColuna)) {
            tabuleiro[linhaVazia][colunaVazia] = tabuleiro[novaLinha][novaColuna];
            tabuleiro[novaLinha][novaColuna] = INICIO;
            linhaVazia = novaLinha;
            colunaVazia = novaColuna;
        }
    }
}

bool verificarVitoria() {
    int valorEsperado = 1;
    for (int i = 0; i < TAM; i++) {
        for (int j = 0; j < TAM; j++) {
            if (i == TAM - 1 && j == TAM - 1)
                return tabuleiro[i][j] == INICIO;
            if (tabuleiro[i][j] != valorEsperado++)
                return false;
        }
    }
    return true;
}

int contarInversoes() {
    int arr[TAM * TAM], k = 0;
    for (int i = 0; i < TAM; i++)
        for (int j = 0; j < TAM; j++)
            arr[k++] = tabuleiro[i][j];

    int inv_count = 0;
    for (int i = 0; i < TAM * TAM - 1; i++)
        for (int j = i + 1; j < TAM * TAM; j++)
            if (arr[i] && arr[j] && arr[i] > arr[j])
                inv_count++;
    return inv_count;
}

bool verificarCombinacaoDoEmbaralhamento() {
    int invCount = contarInversoes();
    int linhaVaziaAPartirDeBaixo = TAM - linhaVazia;
    return (linhaVaziaAPartirDeBaixo % 2 == 0) ? (invCount % 2 != 0) : (invCount % 2 == 0);
}

void jogar() {
    inicializar();
    embaralhar();

    int numero;

    while (true) {
        imprimirTabuleiro();

        if (verificarVitoria()) {
            printf("\n\033[1;32m🎉 PARABÉNS! VOCÊ VENCEU! 🎉\033[0m\n\n");
            #ifdef _WIN32
                system("pause");
            #else
                printf("Pressione Enter para continuar...");
                getchar(); getchar();
            #endif
            break;
        }

        printf("Digite o número que deseja mover (0 para voltar ao menu): ");
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
            printf("\033[1;31mNúmero inválido! Digite um valor entre 1 e 15.\033[0m\n");
            usleep(1500000);
            continue;
        }

        if (!mover(numero)) {
            printf("\033[1;31mMovimento inválido! A peça não está ao lado do espaço vazio.\033[0m\n");
            usleep(1500000);
        }
    }
}

void menu() {
    int escolha = 0;
    while (true) {
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif

        printf("\033[1;33m──────────୨ৎ─────────\n");
        printf("ᯓ★「 ✦ Menu ✦ 」★ᯓ\n");
        printf("──────────୨ৎ─────────\033[0m\n");
        printf("✷ 1. Jogar\n");
        printf("✷ 2. Sair\n");
        printf("──────────────────────\n");
        printf("Escolha uma opção: ");

        if (scanf("%d", &escolha) != 1) {
            while (getchar() != '\n');
            escolha = -1;
        }

        switch (escolha) {
            case 1:
                jogar();
                break;
            case 2:
                printf("\033[1;35mSaindo....( ˶°ㅁ°) !! Até depois\033[0m\n");
                usleep(1500000);
                return;
            default:
                printf("\033[1;31mEscolha Inválida! As opções disponíveis são 1 ou 2!\033[0m\n");
                usleep(1500000);
                break;
        }
    }
}

int main() {
    menu();
    return 0;
}
