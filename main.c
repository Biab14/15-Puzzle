#include <stdio.h>
#include <stdlib.h>    
#include <time.h> 
#include <stdbool.h>
#include <Windows.h>

#define TAM 4
#define MAX 50
#define INICIO 0

int tabuleiro[TAM][TAM];

int i,j;
int linhaVazia = TAM - 1;
int colunaVazia = TAM - 1;
int escolha;

//Metodo para separar as celulas com números
void inicializar() {
     int valor = 1;
     for (int i = 0; i < TAM; i++) {
         for (int j = 0; j < TAM; j++) {
             if (valor <= 15) {
                 tabuleiro[i][j] = valor++;
             } else {
                 tabuleiro[i][j] = INICIO; // último espaço é o "vazio"
             }
         }
     }
 }




//Esse metodo printa o tabuleiro do nosso joguinho
void imprimirTabuleiro(){
     int i,j;

    system("cls"); 
    printf("────-ˋˏ ༻❁༺ ˎˊ-────\n");
    printf("ᯓ★   15 Puzzle   ★ᯓ\n");
    printf("────-ˋˏ ༻❁༺ ˎˊ-────\n");

    for (int i = 0; i < TAM; i++){
        for (int j = 0; j < TAM; j++){
            if (tabuleiro[i][j] == INICIO)
               printf("___");
            else
                printf("%3d", tabuleiro[i][j]);
            }
            printf("\n");
        }

     }

bool posicaoValida(int l, int c) {
     return l >= 0 && l < TAM && c >= 0 && c < TAM;
}


//move a célula que o jogador escolheu
bool mover(int numero) {
     int i, j;

     for (i = 0; i < TAM; i++) {
         for (j = 0; j < TAM; j++) {
             if (tabuleiro[i][j] == numero) {
                 if ((i == linhaVazia && (j == colunaVazia + 1 || j == colunaVazia - 1)) ||
                     (j == colunaVazia && (i == linhaVazia + 1 || i == linhaVazia - 1))) {

                     tabuleiro[linhaVazia][colunaVazia] = numero;
                     tabuleiro[i][j] = INICIO;
                     linhaVazia = i;
                     colunaVazia = j;
                     return true;
                 }
             }
         }
     }

     return false;
 }  

void jogar() {
    inicializar();
    int numero;

    while (1) {
        imprimirTabuleiro();
        printf("Digite o número que deseja mover (0 para voltar ao menu): ");
        scanf("%d", &numero);

        if (numero == 0) {
            printf("Voltando ao menu...\n");
            Sleep(1000);
            break;
        }

        if (numero < 1 || numero > 15) {
            printf("Número inválido! Digite entre 1 e 15.\n");
            system("pause");
            continue;
        }

        if (!mover(numero)) {
            printf("Movimento inválido! O número não está ao lado do espaço vazio.\n");
            system("pause");
        }
    }
}

//Esse metodo printa o menu na tela :D
void menu(){
        printf("──────────୨ৎ─────────\n");
        printf("ᯓ★「 ✦ Menu ✦ 」★ᯓ\n");
        printf("──────────୨ৎ─────────\n");
        printf("✷1.Jogar\n");
        printf("✷2.Sair\n");
        printf("Escolha a opção:\n");
        scanf("%d", &escolha);
         if(escolha == 1 ){
           jogar();
          }else if(escolha == 2){
          printf("Saindo....( ˶°ㅁ°) !! Até depois\n");
          Sleep(1500);
          menu();
          }while (escolha > 1  || 2 < escolha ){
               printf("Escolha Inválida! As opções disponíveis são 1 ou 2!\n");
               Sleep(1500);
               menu();   
               
          }

        }


//pra começar somente com o menu
int main(){
   menu();
   return 0;
}