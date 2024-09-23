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

// Fun��o para criar uma nova pessoa
Pessoa* novaPessoa(int senha) {
    Pessoa* nova = (Pessoa*)malloc(sizeof(Pessoa));
    nova->senha = senha;
    nova->proximo = NULL;
    return nova;
}

// Fun��o para inicializar a fila
void inicializarFila(Fila *fila) {
    fila->inicio = NULL;
    fila->fim = NULL;
    fila->contador = 0;
}

// Fun��o para verificar se a fila est� vazia
int filaVazia(Fila *fila) {
    return fila->inicio == NULL;
}

// Fun��o para adicionar uma pessoa na fila
void enfileirar(Fila *fila) {
    static int senha = 1;  // Senha autom�tica que inicia em 1
    Pessoa* nova = novaPessoa(senha++);

    if (fila->fim == NULL) {
        fila->inicio = nova;
        fila->fim = nova;
    } else {
        fila->fim->proximo = nova;
        fila->fim = nova;
    }

    fila->contador++;
    printf("Pessoa com senha %d adicionada � fila.\n", nova->senha);
}

// Fun��o para atender uma pessoa (remover a primeira da fila)
void desenfileirar(Fila *fila) {
    if (filaVazia(fila)) {
        printf("A fila est� vazia. N�o h� ningu�m para atender.\n");
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

// Fun��o para consultar as pessoas na fila
void consultarFila(Fila *fila) {
    if (filaVazia(fila)) {
        printf("A fila est� vazia.\n");
        return;
    }

    Pessoa *temp = fila->inicio;
    printf("Fila atual:\n");
    while (temp != NULL) {
        printf("Senha: %d\n", temp->senha);
        temp = temp->proximo;
    }
}

// Fun��o para atender a fila: primeiro os priorit�rios, depois os normais
void atenderProximaPessoa(Fila *filaPrioritaria, Fila *filaNormal) {
    if (!filaVazia(filaPrioritaria)) {
        desenfileirar(filaPrioritaria);  // Atende a fila priorit�ria primeiro
    } else if (!filaVazia(filaNormal)) {
        desenfileirar(filaNormal);  // Se a fila priorit�ria estiver vazia, atende a fila normal
    } else {
        printf("Nenhuma pessoa para atender.\n");
    }
}

// Fun��o para liberar a mem�ria alocada para a fila
void liberarFila(Fila *fila) {
    while (!filaVazia(fila)) {
        desenfileirar(fila);  // Vai desenfileirando at� liberar todas as pessoas
    }
}

// Fun��o principal
int main() {
    Fila filaNormal, filaPrioritaria;
    inicializarFila(&filaNormal);
    inicializarFila(&filaPrioritaria);

    int opcao, prioridade;

    do {
        printf("\n1 - Adicionar pessoa na fila\n");
        printf("2 - Atender pr�xima pessoa\n");
        printf("3 - Consultar fila\n");
        printf("4 - Verificar se a fila est� vazia\n");
        printf("0 - Sair\n");
        printf("Escolha uma op��o: ");
        
        // Valida��o de entrada do menu
        if (scanf("%d", &opcao) != 1) {
            printf("Entrada inv�lida! Por favor, insira um n�mero.\n");
            while (getchar() != '\n');  // Limpa o buffer de entrada
            continue;
        }

        switch (opcao) {
            case 1:
                printf("A pessoa tem prioridade? (1 - Sim, 0 - N�o): ");
                if (scanf("%d", &prioridade) != 1 || (prioridade != 0 && prioridade != 1)) {
                    printf("Entrada inv�lida! Por favor, insira 0 ou 1.\n");
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
                printf("\nConsultando fila priorit�ria:\n");
                consultarFila(&filaPrioritaria);
                printf("\nConsultando fila normal:\n");
                consultarFila(&filaNormal);
                break;
            case 4:
                if (filaVazia(&filaPrioritaria) && filaVazia(&filaNormal)) {
                    printf("Ambas as filas est�o vazias.\n");
                } else {
                    printf("H� pessoas em alguma fila.\n");
                }
                break;
            case 0:
                printf("Saindo...\n");
                break;
            default:
                printf("Op��o inv�lida!\n");
        }
    } while (opcao != 0);

    // Libera��o de mem�ria das filas antes de sair
    liberarFila(&filaPrioritaria);
    liberarFila(&filaNormal);

    return 0;
}

