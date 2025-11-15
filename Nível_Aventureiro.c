//Partida de War em C
#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>
#include <time.h>

// Constantes Globais
#define MAX_TERRITORIOS 5
#define MAX_STRING 50

// Estrutura para representar um território
struct territorio {
    char nome[MAX_STRING];
    char cor[MAX_STRING];
    int tropas;
};

//Estrutura para informações do ataque
struct ataque {
    int atacante;
    int defensor;
    int tropas_atacantes;
    int tropas_defensoras;
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
    }
}

// Função para lançar um dado
int lancar_dado() {
    return (rand() % 6) + 1;
}

// Função para executar o ataque
void executar_ataque(struct territorio *territorios, int atacante, int defensor) {
    // Converter para índice (entrada é 1-5, índice é 0-4)
    int idx_atacante = atacante - 1;
    int idx_defensor = defensor - 1;
    
    // Validações
    if (idx_atacante < 0 || idx_atacante >= MAX_TERRITORIOS) {
        printf("Territórios inválidos!\n");
        return;
    }
    
    if (idx_atacante == idx_defensor) {
        printf("Você não pode atacar a si mesmo!\n");
        return;
    }
    
    if (territorios[idx_atacante].tropas == 0) {
        printf("O território atacante não possui tropas!\n");
        return;
    }

    if (territorios[idx_defensor].tropas == 0) {
        printf("O território defensor não possui tropas!\n");
        return;
    }
    
    // Lançar dados
    int dado_atacante = lancar_dado();
    int dado_defensor = lancar_dado();
    
    printf("\n=========================================\n");
    printf("           RESULTADO DO ATAQUE\n");
    printf("=========================================\n");
    printf("Atacante: %s (Exército %s) - Dado: %d\n", 
           territorios[idx_atacante].nome, territorios[idx_atacante].cor, dado_atacante);
    printf("Defensor: %s (Exército %s) - Dado: %d\n", 
           territorios[idx_defensor].nome, territorios[idx_defensor].cor, dado_defensor);
    
    // Comparar dados e aplicar a regra
    if (dado_atacante > dado_defensor) {
        printf("\nVITÓRIA! O Atacante VENCEU!\n");
        printf("O defensor perde 1 tropa. (%d -> %d)\n", 
               territorios[idx_defensor].tropas, territorios[idx_defensor].tropas - 1);
        territorios[idx_defensor].tropas--;
    } 
    else if (dado_defensor > dado_atacante) {
        printf("\nDERROTA... O Defensor VENCEU!\n");
        printf("O atacante perde 1 tropa. (%d -> %d)\n", 
               territorios[idx_atacante].tropas, territorios[idx_atacante].tropas - 1);
        territorios[idx_atacante].tropas--;
    } 
    else {
        printf("\nEMPATE!\n");
        printf("Nenhuma tropa é perdida.\n");
    }
    printf("=========================================\n");
}

// Função Main
int main() {
    setlocale(LC_ALL, "pt_BR.UTF-8"); // Configurar locale para português
    srand(time(NULL)); // Inicializar seed do gerador de números aleatórios
    
    struct territorio *territorios;
    struct ataque *info_ataque;
    int i;
    char continuar;

    territorios = malloc(MAX_TERRITORIOS * sizeof(struct territorio));
    info_ataque = malloc(sizeof(struct ataque));
    
    //Verificação de alocação
    if (territorios == NULL || info_ataque == NULL) {
        printf("Erro ao alocar memória!\n");
        return 1;
    }   

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

    //Inicio da fase de ataque
    printf("=========================================\n");
    printf("       Iniciando a fase de ataque\n");
    printf("=========================================\n");
    
    do {
        printf("\nSelecione o território atacante (1-%d): ", MAX_TERRITORIOS);
        scanf("%d", &info_ataque->atacante);
        limparBuffer();
        
        printf("Selecione o território defensor (1-%d): ", MAX_TERRITORIOS);
        scanf("%d", &info_ataque->defensor); 
        limparBuffer();
        
        // Executar o ataque
        executar_ataque(territorios, info_ataque->atacante, info_ataque->defensor);
        
        // Exibir estado atual dos territórios
        printf("\n----- Estado atual dos territórios -----\n");
        for (i = 0; i < MAX_TERRITORIOS; i++) {
            printf("T%d - %s (%s): %d tropas\n", i + 1, territorios[i].nome, territorios[i].cor, territorios[i].tropas);
        }
        
        printf("\nDeseja fazer outro ataque? (S/N): ");
        scanf("%c", &continuar);
        limparBuffer();
        
    } while (continuar == 'S' || continuar == 's');
    
    free(info_ataque);
    free(territorios);
    
    return 0;
}