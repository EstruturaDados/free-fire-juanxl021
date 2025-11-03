#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_ITENS 10

// ============================================================================
// 🧳 Estruturas de Dados
// ============================================================================

typedef struct {
    char nome[50];
    char tipo[30];
    int quantidade;
    int prioridade; // 1 a 5
} Item;

typedef enum {
    ORDENAR_POR_NOME = 1,
    ORDENAR_POR_TIPO,
    ORDENAR_POR_PRIORIDADE
} CriterioOrdenacao;

// ============================================================================
// 🧹 Funções Utilitárias
// ============================================================================

void limparTela() {
    for (int i = 0; i < 30; i++) printf("\n");
}

void pausar() {
    printf("\nPressione ENTER para continuar...");
    getchar();
}

// ============================================================================
// 📜 Funções de Exibição
// ============================================================================

void exibirMenu(bool ordenadaPorNome) {
    printf("=============================================\n");
    printf("🏝️  CÓDIGO DA ILHA – EDIÇÃO FREE FIRE\n");
    printf("=============================================\n");
    printf("1. Adicionar item\n");
    printf("2. Remover item\n");
    printf("3. Listar itens\n");
    printf("4. Ordenar itens\n");
    printf("5. Buscar item (binária por nome)\n");
    printf("0. Sair\n");
    printf("---------------------------------------------\n");
    printf("🔠 Status da ordenação: %s\n", ordenadaPorNome ? "Por nome (OK)" : "Não ordenada");
    printf("---------------------------------------------\n");
    printf("Escolha: ");
}

// ============================================================================
// ➕ Inserir e ➖ Remover
// ============================================================================

void inserirItem(Item mochila[], int *numItens, bool *ordenadaPorNome) {
    if (*numItens >= MAX_ITENS) {
        printf("⚠️  Mochila cheia! Não é possível adicionar mais itens.\n");
        return;
    }

    Item novo;
    printf("Digite o nome do item: ");
    getchar();
    fgets(novo.nome, 50, stdin);
    novo.nome[strcspn(novo.nome, "\n")] = 0;

    printf("Digite o tipo: ");
    fgets(novo.tipo, 30, stdin);
    novo.tipo[strcspn(novo.tipo, "\n")] = 0;

    printf("Digite a quantidade: ");
    scanf("%d", &novo.quantidade);

    printf("Digite a prioridade (1 a 5): ");
    scanf("%d", &novo.prioridade);

    mochila[*numItens] = novo;
    (*numItens)++;
    *ordenadaPorNome = false;

    printf("✅ Item adicionado com sucesso!\n");
}

void removerItem(Item mochila[], int *numItens) {
    if (*numItens == 0) {
        printf("⚠️  Mochila vazia!\n");
        return;
    }

    char nome[50];
    printf("Digite o nome do item a remover: ");
    getchar();
    fgets(nome, 50, stdin);
    nome[strcspn(nome, "\n")] = 0;

    int i, encontrado = -1;
    for (i = 0; i < *numItens; i++) {
        if (strcmp(mochila[i].nome, nome) == 0) {
            encontrado = i;
            break;
        }
    }

    if (encontrado == -1) {
        printf("❌ Item não encontrado!\n");
        return;
    }

    for (i = encontrado; i < *numItens - 1; i++)
        mochila[i] = mochila[i + 1];

    (*numItens)--;
    printf("🗑️  Item removido com sucesso!\n");
}

// ============================================================================
// 📋 Listagem
// ============================================================================

void listarItens(Item mochila[], int numItens) {
    if (numItens == 0) {
        printf("⚠️  Mochila vazia!\n");
        return;
    }

    printf("\n================ MOCHILA ==================\n");
    printf("%-20s %-15s %-10s %-10s\n", "NOME", "TIPO", "QTD", "PRIORIDADE");
    printf("--------------------------------------------\n");
    for (int i = 0; i < numItens; i++) {
        printf("%-20s %-15s %-10d %-10d\n",
               mochila[i].nome, mochila[i].tipo, mochila[i].quantidade, mochila[i].prioridade);
    }
    printf("============================================\n");
}

// ============================================================================
// 🔢 Ordenação (Insertion Sort)
// ============================================================================

void insertionSort(Item mochila[], int numItens, CriterioOrdenacao criterio, int *comparacoes, bool *ordenadaPorNome) {
    *comparacoes = 0;

    for (int i = 1; i < numItens; i++) {
        Item chave = mochila[i];
        int j = i - 1;

        while (j >= 0) {
            (*comparacoes)++;
            bool cond = false;

            switch (criterio) {
                case ORDENAR_POR_NOME:
                    cond = strcmp(mochila[j].nome, chave.nome) > 0;
                    break;
                case ORDENAR_POR_TIPO:
                    cond = strcmp(mochila[j].tipo, chave.tipo) > 0;
                    break;
                case ORDENAR_POR_PRIORIDADE:
                    cond = mochila[j].prioridade < chave.prioridade; // ordem decrescente
                    break;
            }

            if (!cond) break;
            mochila[j + 1] = mochila[j];
            j--;
        }
        mochila[j + 1] = chave;
    }

    *ordenadaPorNome = (criterio == ORDENAR_POR_NOME);
    printf("✅ Ordenação concluída com %d comparações.\n", *comparacoes);
}

// ============================================================================
// 🔍 Busca Binária
// ============================================================================

void buscaBinariaPorNome(Item mochila[], int numItens, bool ordenadaPorNome) {
    if (!ordenadaPorNome) {
        printf("⚠️  A mochila deve estar ordenada por nome!\n");
        return;
    }

    char nome[50];
    printf("Digite o nome do item que deseja buscar: ");
    getchar();
    fgets(nome, 50, stdin);
    nome[strcspn(nome, "\n")] = 0;

    int inicio = 0, fim = numItens - 1;
    while (inicio <= fim) {
        int meio = (inicio + fim) / 2;
        int cmp = strcmp(mochila[meio].nome, nome);

        if (cmp == 0) {
            printf("🔎 Item encontrado:\n");
            printf("Nome: %s | Tipo: %s | Quantidade: %d | Prioridade: %d\n",
                   mochila[meio].nome, mochila[meio].tipo, mochila[meio].quantidade, mochila[meio].prioridade);
            return;
        } else if (cmp < 0) {
            inicio = meio + 1;
        } else {
            fim = meio - 1;
        }
    }

    printf("❌ Item não encontrado!\n");
}

// ============================================================================
// 🕹️ Função Principal
// ============================================================================

int main() {
    Item mochila[MAX_ITENS];
    int numItens = 0;
    bool ordenadaPorNome = false;
    int opcao, comparacoes = 0;

    do {
        exibirMenu(ordenadaPorNome);
        scanf("%d", &opcao);

        limparTela();

        switch (opcao) {
            case 1:
                inserirItem(mochila, &numItens, &ordenadaPorNome);
                break;
            case 2:
                removerItem(mochila, &numItens);
                break;
            case 3:
                listarItens(mochila, numItens);
                break;
            case 4: {
                printf("\nEscolha o critério:\n");
                printf("1 - Nome\n2 - Tipo\n3 - Prioridade\n> ");
                int c;
                scanf("%d", &c);
                insertionSort(mochila, numItens, c, &comparacoes, &ordenadaPorNome);
                break;
            }
            case 5:
                buscaBinariaPorNome(mochila, numItens, ordenadaPorNome);
                break;
            case 0:
                printf("👋 Saindo do jogo...\n");
                break;
            default:
                printf("⚠️  Opção inválida!\n");
        }

        pausar();
        limparTela();
    } while (opcao != 0);

    return 0;
}
