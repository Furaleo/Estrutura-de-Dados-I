#include <stdio.h>
#include <stdlib.h>

// Estrutura para representar uma pessoa na fila
typedef struct Pessoa {
    int senha;
    struct Pessoa *proximo;
} Pessoa;

// Estrutura para representar a fila
typedef struct Fila {
    Pessoa *inicio;
    Pessoa *fim;
    int contador;
} Fila;

// Função para criar uma nova pessoa
Pessoa* novaPessoa(int senha) {
    Pessoa* nova = (Pessoa*)malloc(sizeof(Pessoa));
    nova->senha = senha;
    nova->proximo = NULL;
    return nova;
}

// Função para inicializar a fila
void inicializarFila(Fila *fila) {
    fila->inicio = NULL;
    fila->fim = NULL;
    fila->contador = 0;
}

// Função para verificar se a fila está vazia
int filaVazia(Fila *fila) {
    return fila->inicio == NULL;
}

// Função para adicionar uma pessoa na fila
void enfileirar(Fila *fila) {
    static int senha = 1;  // Senha automática que inicia em 1
    Pessoa* nova = novaPessoa(senha++);

    if (fila->fim == NULL) {
        fila->inicio = nova;
        fila->fim = nova;
    } else {
        fila->fim->proximo = nova;
        fila->fim = nova;
    }

    fila->contador++;
    printf("Pessoa com senha %d adicionada à fila.\n", nova->senha);
}

// Função para atender uma pessoa (remover a primeira da fila)
void desenfileirar(Fila *fila) {
    if (filaVazia(fila)) {
        printf("A fila está vazia. Não há ninguém para atender.\n");
        return;
    }

    Pessoa *temp = fila->inicio;
    printf("Atendendo a pessoa com senha %d\n", temp->senha);
    fila->inicio = fila->inicio->proximo;

    if (fila->inicio == NULL) {
        fila->fim = NULL;
    }

    free(temp);
    fila->contador--;
}

// Função para consultar as pessoas na fila
void consultarFila(Fila *fila) {
    if (filaVazia(fila)) {
        printf("A fila está vazia.\n");
        return;
    }

    Pessoa *temp = fila->inicio;
    printf("Fila atual:\n");
    while (temp != NULL) {
        printf("Senha: %d\n", temp->senha);
        temp = temp->proximo;
    }
}

// Função para atender a fila: primeiro os prioritários, depois os normais
void atenderProximaPessoa(Fila *filaPrioritaria, Fila *filaNormal) {
    if (!filaVazia(filaPrioritaria)) {
        desenfileirar(filaPrioritaria);  // Atende a fila prioritária primeiro
    } else if (!filaVazia(filaNormal)) {
        desenfileirar(filaNormal);  // Se a fila prioritária estiver vazia, atende a fila normal
    } else {
        printf("Nenhuma pessoa para atender.\n");
    }
}

// Função para liberar a memória alocada para a fila
void liberarFila(Fila *fila) {
    while (!filaVazia(fila)) {
        desenfileirar(fila);  // Vai desenfileirando até liberar todas as pessoas
    }
}

// Função principal
int main() {
    Fila filaNormal, filaPrioritaria;
    inicializarFila(&filaNormal);
    inicializarFila(&filaPrioritaria);

    int opcao, prioridade;

    do {
        printf("\n1 - Adicionar pessoa na fila\n");
        printf("2 - Atender próxima pessoa\n");
        printf("3 - Consultar fila\n");
        printf("4 - Verificar se a fila está vazia\n");
        printf("0 - Sair\n");
        printf("Escolha uma opção: ");
        
        // Validação de entrada do menu
        if (scanf("%d", &opcao) != 1) {
            printf("Entrada inválida! Por favor, insira um número.\n");
            while (getchar() != '\n');  // Limpa o buffer de entrada
            continue;
        }

        switch (opcao) {
            case 1:
                printf("A pessoa tem prioridade? (1 - Sim, 0 - Não): ");
                if (scanf("%d", &prioridade) != 1 || (prioridade != 0 && prioridade != 1)) {
                    printf("Entrada inválida! Por favor, insira 0 ou 1.\n");
                    while (getchar() != '\n');  // Limpa o buffer de entrada
                    continue;
                }
                if (prioridade) {
                    enfileirar(&filaPrioritaria);
                } else {
                    enfileirar(&filaNormal);
                }
                break;
            case 2:
                atenderProximaPessoa(&filaPrioritaria, &filaNormal);
                break;
            case 3:
                printf("\nConsultando fila prioritária:\n");
                consultarFila(&filaPrioritaria);
                printf("\nConsultando fila normal:\n");
                consultarFila(&filaNormal);
                break;
            case 4:
                if (filaVazia(&filaPrioritaria) && filaVazia(&filaNormal)) {
                    printf("Ambas as filas estão vazias.\n");
                } else {
                    printf("Há pessoas em alguma fila.\n");
                }
                break;
            case 0:
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida!\n");
        }
    } while (opcao != 0);

    // Liberação de memória das filas antes de sair
    liberarFila(&filaPrioritaria);
    liberarFila(&filaNormal);

    return 0;
}

