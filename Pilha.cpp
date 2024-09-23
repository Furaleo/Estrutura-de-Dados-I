#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Boleto {
    char descricao[100];
    float valor;
    struct Boleto* prox;
} Boleto;

typedef struct Pilha {
    Boleto* topo;
    int tamanho;
} Pilha;

Pilha* criarPilha() {
    Pilha* p = (Pilha*)malloc(sizeof(Pilha));
    p->topo = NULL;
    p->tamanho = 0;
    return p;
}

void empilhar(Pilha* p, char* descricao, float valor) {						
    Boleto* novo = (Boleto*)malloc(sizeof(Boleto));
    strcpy(novo->descricao, descricao);
    novo->valor = valor;
    novo->prox = p->topo;
    p->topo = novo;
    p->tamanho++;
}

void desempilhar(Pilha* p) {
    if (p->topo != NULL) {
        Boleto* temp = p->topo;
        p->topo = p->topo->prox;
        free(temp);
        p->tamanho--;
    } else {
        printf("A pilha de boletos esta vazia, nao e possivel desempilhar.\n");
    }
}

int estaVazia(Pilha* p) {
    return p->topo == NULL;
}

Boleto* consultarTopo(Pilha* p) {
    return p->topo;
}

void consultarPilha(Pilha* p) {
    Boleto* atual = p->topo;
    int posicao = 1;

    if (estaVazia(p)) {
        printf("A pilha de boletos esta vazia.\n");
    } else {
        printf("Boletos na pilha:\n");
        while (atual != NULL) {
            printf("Posicao %d: %s - R$ %.2f\n", posicao, atual->descricao, atual->valor);
            atual = atual->prox;
            posicao++;
        }
    }
}

void destruirPilha(Pilha* p) {
    while (!estaVazia(p)) {
        desempilhar(p);
    }
    free(p); //ver se esta sendo util
}

int main() {
    Pilha* minhaPilha = criarPilha();
    int opcao;
    char descricao[100];
    float valor;

    do {
        printf("\nMenu:\n");
        printf("1. Empilhar boleto\n");
        printf("2. Desempilhar boleto\n");
        printf("3. Consultar topo\n");
        printf("4. Consultar pilha\n");
        printf("5. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        getchar();

        switch(opcao) {
            case 1:
                printf("Digite a descricao do boleto: ");
                fgets(descricao, sizeof(descricao), stdin);
                descricao[strcspn(descricao, "\n")] = '\0';
                printf("Digite o valor do boleto: R$ ");
                scanf("%f", &valor);
                empilhar(minhaPilha, descricao, valor);
                break;
            case 2:
                desempilhar(minhaPilha);
                break;
            case 3:
                if (estaVazia(minhaPilha)) {
                    printf("A pilha de boletos esta vazia.\n");
                } else {
                    Boleto* topo = consultarTopo(minhaPilha);
                    printf("Boleto no topo: %s - R$ %.2f\n", topo->descricao, topo->valor);
                }
                break;
            case 4:
                consultarPilha(minhaPilha);
                break;
            case 5:
                destruirPilha(minhaPilha);
                printf("Tenha medo, o leao sempre esta observando....\nSaindo...\n");
                break;
            default:
                printf("Opcao invalida. Tente novamente.\n");
        }
    } while (opcao != 5);

    return 0;
}

