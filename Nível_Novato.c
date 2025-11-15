//Partida de War em C
#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>

// Constantes Globais
#define MAX_TERRITORIOS 5
#define MAX_STRING 50

// Estrutura para representar um território
struct territorio {
    char nome[MAX_STRING];
    char cor[MAX_STRING];
    int tropas;
};

// Função para limpar o buffer de entrada
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Converte a string do território para maiúsculas (compatível com acentos)
void str_toupper(char *s) {
    for (int i = 0; s[i]; i++) {
        unsigned char c = (unsigned char) s[i];
        if (c >= 'a' && c <= 'z') {
            s[i] = c - 32;
        }
        // Acentos minúsculos para maiúsculos
        else if (c == 'á') s[i] = 'Á';
        else if (c == 'é') s[i] = 'É';
        else if (c == 'í') s[i] = 'Í';
        else if (c == 'ó') s[i] = 'Ó';
        else if (c == 'ú') s[i] = 'Ú';
        else if (c == 'ã') s[i] = 'Ã';
        else if (c == 'õ') s[i] = 'Õ';
        else if (c == 'ç') s[i] = 'Ç';
    }
}

// Função Main
int main() {
    setlocale(LC_ALL, "pt_BR.UTF-8"); // Configurar locale para português
    struct territorio territorios[MAX_TERRITORIOS];
    int i;

    do{
        // Entrada de dados dos territórios
        printf("=========================================\n");
        printf("        Bem-vindo ao WAR BY PEDRO!\n");
        printf("=========================================\n");
        printf("Vamos iniciar cadastrando 5 territórios:\n");
        for (i = 0; i < MAX_TERRITORIOS; i++) {
            printf("\n----- CADASTRO DO TERRITÓRIO %d -----\n", i + 1);
            printf("Digite o nome do território: ");
            fgets(territorios[i].nome, MAX_STRING, stdin);
            territorios[i].nome[strcspn(territorios[i].nome, "\n")] = 0; // Remover nova linha
            str_toupper(territorios[i].nome); // Converter nome para MAIÚSCULAS

            printf("Digite a cor do exército: ");
            fgets(territorios[i].cor, MAX_STRING, stdin);
            territorios[i].cor[strcspn(territorios[i].cor, "\n")] = 0; // Remover nova linha

            printf("Digite o número de tropas no território: ");
            scanf("%d", &territorios[i].tropas);
            limparBuffer(); // Limpar buffer após scanf
        }

        // Exibir os territórios cadastrados
        printf("\nTerritórios cadastrados:\n");
        for (i = 0; i < MAX_TERRITORIOS; i++) {
            printf("\n----- Território %d ----- \n- %s \n- Exército %s \n- Tropas: %d\n", i + 1, territorios[i].nome, territorios[i].cor, territorios[i].tropas);
        }
    }

    while(0);   
    return 0;
}
//Partida de War em C
#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>

// Constantes Globais
#define MAX_TERRITORIOS 5
#define MAX_STRING 50

// Estrutura para representar um território
struct territorio {
    char nome[MAX_STRING];
    char cor[MAX_STRING];
    int tropas;
};

// Função para limpar o buffer de entrada
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Converte a string do território para maiúsculas (compatível com acentos)
void str_toupper(char *s) {
    for (int i = 0; s[i]; i++) {
        unsigned char c = (unsigned char) s[i];
        if (c >= 'a' && c <= 'z') {
            s[i] = c - 32;
        }
        // Acentos minúsculos para maiúsculos
        else if (c == 'á') s[i] = 'Á';
        else if (c == 'é') s[i] = 'É';
        else if (c == 'í') s[i] = 'Í';
        else if (c == 'ó') s[i] = 'Ó';
        else if (c == 'ú') s[i] = 'Ú';
        else if (c == 'ã') s[i] = 'Ã';
        else if (c == 'õ') s[i] = 'Õ';
        else if (c == 'ç') s[i] = 'Ç';
    }
}

// Função Main
int main() {
    setlocale(LC_ALL, "pt_BR.UTF-8"); // Configurar locale para português
    struct territorio territorios[MAX_TERRITORIOS];
    int i;

    do{
        // Entrada de dados dos territórios
        printf("=========================================\n");
        printf("        Bem-vindo ao WAR BY PEDRO!\n");
        printf("=========================================\n");
        printf("Vamos iniciar cadastrando 5 territórios:\n");
        for (i = 0; i < MAX_TERRITORIOS; i++) {
            printf("\n----- CADASTRO DO TERRITÓRIO %d -----\n", i + 1);
            printf("Digite o nome do território: ");
            fgets(territorios[i].nome, MAX_STRING, stdin);
            territorios[i].nome[strcspn(territorios[i].nome, "\n")] = 0; // Remover nova linha
            str_toupper(territorios[i].nome); // Converter nome para MAIÚSCULAS

            printf("Digite a cor do exército: ");
            fgets(territorios[i].cor, MAX_STRING, stdin);
            territorios[i].cor[strcspn(territorios[i].cor, "\n")] = 0; // Remover nova linha

            printf("Digite o número de tropas no território: ");
            scanf("%d", &territorios[i].tropas);
            limparBuffer(); // Limpar buffer após scanf
        }

        // Exibir os territórios cadastrados
        printf("\nTerritórios cadastrados:\n");
        for (i = 0; i < MAX_TERRITORIOS; i++) {
            printf("\n----- Território %d ----- \n- %s \n- Exército %s \n- Tropas: %d\n", i + 1, territorios[i].nome, territorios[i].cor, territorios[i].tropas);
        }
    }

    while(0);   
    return 0;
}
