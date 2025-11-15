// Jogo completo do War em C com missões e fases de ataque
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <time.h>

// Constantes Globais
#define MAX_JOGADORES 5
#define MAX_TERRITORIOS 5
#define MAX_STRING 50

// Estrutura para representar um jogador
struct jogador {
    char nome[MAX_STRING];
    char cor[MAX_STRING];
    char territorio[MAX_STRING]; // nome do território inicial escolhido
    int missoes_completas;
    char missao[MAX_STRING];
    int missao_target; // objetivo numérico
    int missao_concluida; // 0 ou 1
    int ataques_vencidos; // contador de vitórias em ataques
};

// Estrutura para representar um território
struct territorio {
    char nome[MAX_STRING];
    int owner; // índice do jogador dono (0..MAX_JOGADORES-1) ou -1 se neutro
    int tropas;
};

// Protótipos de funções (manter no topo)
void limparBuffer(void);
void exibir_introducao(void);
void mostrar_menu(void);
void cadastrar_jogadores(struct jogador *jogadores, struct territorio *territorios);
void mostrar_mapa(struct jogador *jogadores, struct territorio *territorios);
void liberar_memoria(struct jogador *jogadores, struct territorio *territorios);
int lancar_dado(void);
void executar_ataque(struct territorio *territorios, struct jogador *jogadores, int atacante_idx, int defensor_idx);
void atribuir_missoes(struct jogador *jogadores);
void checar_missao(struct jogador *jogadores, struct territorio *territorios, int jogador_idx);
void mostrar_jogadores(struct jogador *jogadores);

int main(void) {
    setlocale(LC_ALL, "pt_BR.UTF-8"); // Configurar locale para português
    srand((unsigned int)time(NULL)); // Inicializar seed do gerador de números aleatórios

    struct jogador *jogadores = malloc(MAX_JOGADORES * sizeof(struct jogador));
    struct territorio *territorios = malloc(MAX_TERRITORIOS * sizeof(struct territorio));

    if (jogadores == NULL || territorios == NULL) {
        printf("Erro ao alocar memória!\n");
        return 1;
    }

    // Inicializar territórios neutros
    for (int i = 0; i < MAX_TERRITORIOS; i++) {
        territorios[i].nome[0] = '\0';
        territorios[i].owner = -1;
        territorios[i].tropas = 0;
    }

    exibir_introducao();
    cadastrar_jogadores(jogadores, territorios); // jogadores escolhem seus territórios aqui
    atribuir_missoes(jogadores);

    int opcao = 0;
    do {
        printf("\n=========================================\n");
        printf("           WAR BY PEDRO - MENU\n");
        printf("=========================================\n");
        printf("\n1 - Mostrar mapa\n2 - Atacar\n3 - Mostrar jogadores e missões\n4 - Sair\n\nEscolha: ");
        if (scanf("%d", &opcao) != 1) { limparBuffer(); opcao = 0; }
        limparBuffer();

        if (opcao == 1) {
            mostrar_mapa(jogadores, territorios);
        } else if (opcao == 2) {
            int atacante, defensor;
            printf("Digite o número do jogador atacante (1-%d): ", MAX_JOGADORES);
            if (scanf("%d", &atacante) != 1) { limparBuffer(); continue; }
            limparBuffer();
            printf("Digite o número do jogador defensor (1-%d): ", MAX_JOGADORES);
            if (scanf("%d", &defensor) != 1) { limparBuffer(); continue; }
            limparBuffer();

            if (atacante < 1 || atacante > MAX_JOGADORES || defensor < 1 || defensor > MAX_JOGADORES) {
                printf("Jogadores inválidos!\n");
            } else if (atacante == defensor) {
                printf("Um jogador não pode atacar a si mesmo!\n");
            } else {
                executar_ataque(territorios, jogadores, atacante - 1, defensor - 1);
                // checar missão do atacante após o ataque
                checar_missao(jogadores, territorios, atacante - 1);
            }

        } else if (opcao == 3) {
            mostrar_jogadores(jogadores);
        }

    } while (opcao != 4);

    liberar_memoria(jogadores, territorios);

    return 0;
}

// Funções auxiliares e de jogo

// Função para liberar memória alocada
void liberar_memoria(struct jogador *jogadores, struct territorio *territorios) {
    free(jogadores);
    free(territorios);
}

// Função para limpar o buffer de entrada
void limparBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Função para exibir a introdução do jogo
void exibir_introducao(void) {
    printf("=========================================\n");
    printf("        Bem-vindo ao WAR BY PEDRO!\n");
    printf("=========================================\n");
    printf("Neste jogo, você irá comandar exércitos,\n");
    printf("conquistar territórios e completar missões.\n");
    printf("Prepare-se para a batalha!\n");
}

// Cadastrar jogadores e atribuir territórios iniciais escolhidos por cada jogador
void cadastrar_jogadores(struct jogador *jogadores, struct territorio *territorios) {
    for (int i = 0; i < MAX_JOGADORES; i++) {
        printf("\n----- CADASTRO DO JOGADOR %d -----\n", i + 1);
        printf("Digite o nome do jogador: ");
        fgets(jogadores[i].nome, MAX_STRING, stdin);
        jogadores[i].nome[strcspn(jogadores[i].nome, "\n")] = 0; // Remover nova linha

        printf("Digite o nome do seu território inicial: ");
        fgets(jogadores[i].territorio, MAX_STRING, stdin);
        jogadores[i].territorio[strcspn(jogadores[i].territorio, "\n")] = 0; // Remover nova linha

        printf("Digite a cor do exército: ");
        fgets(jogadores[i].cor, MAX_STRING, stdin);
        jogadores[i].cor[strcspn(jogadores[i].cor, "\n")] = 0; // Remover nova linha

        jogadores[i].missoes_completas = 0;
        jogadores[i].missao_concluida = 0;
        jogadores[i].missao[0] = '\0';
        jogadores[i].missao_target = 0;
        jogadores[i].ataques_vencidos = 0;

        // Atribuir território criado ao vetor de territórios
        strncpy(territorios[i].nome, jogadores[i].territorio, MAX_STRING - 1);
        territorios[i].nome[MAX_STRING - 1] = '\0';
        territorios[i].owner = i;
        // distribuir tropas iniciais básicas
        territorios[i].tropas = 3 + (rand() % 4);
    }
}

// Atribuir missões: vencer N ataques
void atribuir_missoes(struct jogador *jogadores) {
    for (int i = 0; i < MAX_JOGADORES; i++) {
        int alvo = 2 + (rand() % 3);
        jogadores[i].missao_target = alvo;
        snprintf(jogadores[i].missao, MAX_STRING, "Vencer %d Ataques", alvo);
        jogadores[i].ataques_vencidos = 0;
        jogadores[i].missao_concluida = 0;
    }
}

// Mostrar mapa com territórios, donos e tropas
void mostrar_mapa(struct jogador *jogadores, struct territorio *territorios) {
    printf("=========================================\n");
    printf("              MAPA DO MUNDO\n");
    printf("=========================================\n");
    for (int i = 0; i < MAX_TERRITORIOS; i++) {
        const char *ownerName = "(neutro)";
        if (territorios[i].owner >= 0 && territorios[i].owner < MAX_JOGADORES) {
            ownerName = jogadores[territorios[i].owner].nome;
        }
        printf("\nT%d. %s - Dono: %s - Tropas: %d\n", i + 1, territorios[i].nome[0] ? territorios[i].nome : "(sem nome)", ownerName, territorios[i].tropas);
    }
    printf("=========================================\n");
}

// Mostrar jogadores e suas missões
void mostrar_jogadores(struct jogador *jogadores) {
    printf("\n----- Jogadores -----\n");
    for (int i = 0; i < MAX_JOGADORES; i++) {
        printf("\n%d) Nome: %s | Cor: %s | Território inicial: %s\n", i + 1, jogadores[i].nome[0] ? jogadores[i].nome : "(sem nome)", jogadores[i].cor[0] ? jogadores[i].cor : "(sem cor)", jogadores[i].territorio[0] ? jogadores[i].territorio : "(sem território)");
        printf("   Missão: %s | Vitórias em ataques: %d | Concluída: %s\n", jogadores[i].missao, jogadores[i].ataques_vencidos, jogadores[i].missao_concluida ? "SIM" : "NÃO");
    }
}

// Lançar dado
int lancar_dado(void) {
    return (rand() % 6) + 1;
}

// Executar ataque entre jogadores: atacante_idx ataca defensor_idx
void executar_ataque(struct territorio *territorios, struct jogador *jogadores, int atacante_idx, int defensor_idx) {
    // Escolher um território do atacante e do defensor para simplificar: usar território inicial (índice igual ao jogador)
    int idx_atacante = -1, idx_defensor = -1;
    // procurar um território de propriedade do atacante
    for (int i = 0; i < MAX_TERRITORIOS; i++) {
        if (territorios[i].owner == atacante_idx) { idx_atacante = i; break; }
    }
    for (int i = 0; i < MAX_TERRITORIOS; i++) {
        if (territorios[i].owner == defensor_idx) { idx_defensor = i; break; }
    }

    if (idx_atacante == -1) { printf("O atacante não possui tropas para atacar.\n"); return; }
    if (idx_defensor == -1) { printf("O defensor não possui territórios para serem atacados.\n"); return; }

    if (territorios[idx_atacante].tropas <= 0) { printf("O território atacante não possui tropas suficientes.\n"); return; }

    int dado_a = lancar_dado();
    int dado_d = lancar_dado();
    printf("\n=========================================\n");
    printf("           RESULTADO DO ATAQUE\n");
    printf("=========================================\n");
    printf("Atacante: %s (Território: %s) - Dado: %d\n", jogadores[atacante_idx].nome, territorios[idx_atacante].nome, dado_a);
    printf("Defensor: %s (Território: %s) - Dado: %d\n", jogadores[defensor_idx].nome, territorios[idx_defensor].nome, dado_d);

    if (dado_a > dado_d) {
        printf("\nVITÓRIA! O Atacante VENCEU!\n");
        jogadores[atacante_idx].ataques_vencidos++;
        printf("Vitórias de ataque de %s: %d\n", jogadores[atacante_idx].nome, jogadores[atacante_idx].ataques_vencidos);
        if (territorios[idx_defensor].tropas > 0) territorios[idx_defensor].tropas--;
        printf("O defensor perde 1 tropa. Agora: %d\n", territorios[idx_defensor].tropas);
        // Se defensor ficar sem tropas, transfere território
        if (territorios[idx_defensor].tropas == 0) {
            territorios[idx_defensor].owner = atacante_idx;
            territorios[idx_defensor].tropas = 1; // mover 1 tropa conquistadora
            printf("Território %s conquistado por %s!\n", territorios[idx_defensor].nome, jogadores[atacante_idx].nome);
            // Verificar se o defensor ainda possui territórios
            int restantes = 0;
            for (int j = 0; j < MAX_TERRITORIOS; j++) {
                if (territorios[j].owner == defensor_idx) restantes++;
            }
            if (restantes == 0) {
                // Jogador eliminado: informar e, se desejar, limpar dados básicos
                printf("Jogador %s foi eliminado! Todos os seus territórios foram conquistados.\n", jogadores[defensor_idx].nome);
                // Opcional: resetar nome/território do jogador eliminado para indicar eliminação
                jogadores[defensor_idx].nome[0] = '\0';
            }
        }
    } else if (dado_d > dado_a) {
        printf("\nDERROTA... O Defensor VENCEU!\n");
        if (territorios[idx_atacante].tropas > 0) territorios[idx_atacante].tropas--;
        printf("O atacante perde 1 tropa. Agora: %d\n", territorios[idx_atacante].tropas);
    } else {
        printf("\nEMPATE! Nenhuma tropa é perdida.\n");
    }
    printf("=========================================\n");
}

// Checar missão: marcar como concluída quando vencer ataques suficientes
void checar_missao(struct jogador *jogadores, struct territorio *territorios, int jogador_idx) {
    if (jogadores[jogador_idx].missao_concluida) return;
    if (jogadores[jogador_idx].ataques_vencidos >= jogadores[jogador_idx].missao_target) {
        jogadores[jogador_idx].missao_concluida = 1;
        jogadores[jogador_idx].missoes_completas++;
        printf("\nParabéns %s! Você concluiu a missão: %s\n", jogadores[jogador_idx].nome, jogadores[jogador_idx].missao);
        printf("\nDeseja finalizar a partida? (s/n): ");
        char resposta;
        scanf(" %c", &resposta);
        limparBuffer();
        if (resposta == 's' || resposta == 'S') {
            printf("\nObrigado por jogar! Até a próxima!\n");
            exit(0);
        }
    }
}