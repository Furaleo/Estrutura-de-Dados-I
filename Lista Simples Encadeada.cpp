#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Enum para definir os tipos de dados que a lista pode armazenar
typedef enum {
    INVESTMENT,
} DataType;

// Estrutura para armazenar as informacoes do investimento
typedef struct {
    char name[50];
    double value;
    double return_rate;
} Investment;

// Estrutura para armazenar o dado do no
typedef struct {
    DataType type;   // Tipo de dado
    void *data;      // Ponteiro generico para o dado
} Element;

// Estrutura do no da lista encadeada
typedef struct Node {
    Element element; // Elemento que contem o dado
    struct Node *next; // Ponteiro para o proximo no
} Node;

// Funcao para criar uma nova lista
Node* create_list() {
    return NULL; // Lista vazia e representada por NULL
}

// Funcao para criar um novo elemento (no)
Node* create_node(DataType type, void *data, size_t data_size) {
    Node *new_node = (Node*)malloc(sizeof(Node));
    if (!new_node) {
        fprintf(stderr, "Falha na alocacao de memoria\n");
        exit(EXIT_FAILURE);
    }

    new_node->element.type = type;
    new_node->element.data = malloc(data_size);
    if (!new_node->element.data) {
        fprintf(stderr, "Falha na alocacao de memoria\n");
        exit(EXIT_FAILURE);
    }
    memcpy(new_node->element.data, data, data_size); // Copia os dados para a memoria alocada

    new_node->next = NULL;
    return new_node;
}

// Funcao para inserir um no de forma ordenada na lista por valor
void insert_node(Node **head, DataType type, void *data, size_t data_size) {
    Node *new_node = create_node(type, data, data_size);
    Node *current = *head;
    Node *prev = NULL;

    while (current != NULL && ((Investment*)current->element.data)->value < ((Investment*)data)->value) {
        prev = current;
        current = current->next;
    }

    new_node->next = current;
    
    if (prev == NULL) {
        *head = new_node;
    } else {
        prev->next = new_node;
    }
}

// Funcao para remover um no da lista
void remove_node(Node **head, char *name) {
    Node *current = *head;
    Node *prev = NULL;

    while (current != NULL) {
        Investment *investment = (Investment*)current->element.data;
        if (strcmp(investment->name, name) == 0) break;
        prev = current;
        current = current->next;
    }

    if (current == NULL) {
        printf("Investimento nao encontrado.\n");
        return; // Elemento nao encontrado
    }

    if (prev == NULL) {
        *head = current->next;
    } else {
        prev->next = current->next;
    }

    free(current->element.data); // Libera o dado alocado dinamicamente
    free(current); // Libera o no
    printf("Investimento removido com sucesso.\n");
}

// Funcao para procurar um no na lista
Node* search_node(Node *head, char *name) {
    Node *current = head;
    while (current != NULL) {
        Investment *investment = (Investment*)current->element.data;
        if (strcmp(investment->name, name) == 0) return current;
        current = current->next;
    }
    return NULL; // Elemento nao encontrado
}

// Funcao para imprimir a lista
void print_list(Node *head) {
    Node *current = head;
    if (current == NULL) {
        printf("A lista esta vazia.\n");
    } else {
        while (current != NULL) {
            Investment *investment = (Investment*)current->element.data;
            printf("Investimento: %s | Valor: %.2f | Taxa de Retorno: %.2f%%\n", investment->name, investment->value, investment->return_rate);
            current = current->next;
        }
        printf("----------------------\n");
    }
}

// Funcao para liberar toda a memoria da lista
void free_list(Node *head) {
    Node *current = head;
    Node *next;

    while (current != NULL) {
        next = current->next;
        free(current->element.data); // Libera o dado alocado dinamicamente
        free(current); // Libera o no
        current = next;
    }
}

// Funcao para comparar nos por nome (ordem alfabetica)
int compare_by_name(const void *a, const void *b) {
    Node *node_a = *(Node**)a;
    Node *node_b = *(Node**)b;
    Investment *inv_a = (Investment*)node_a->element.data;
    Investment *inv_b = (Investment*)node_b->element.data;
    return strcmp(inv_a->name, inv_b->name);
}

// Funcao para comparar nos por taxa de retorno
int compare_by_return_rate(const void *a, const void *b) {
    Node *node_a = *(Node**)a;
    Node *node_b = *(Node**)b;
    Investment *inv_a = (Investment*)node_a->element.data;
    Investment *inv_b = (Investment*)node_b->element.data;
    if (inv_a->return_rate < inv_b->return_rate) return -1;
    if (inv_a->return_rate > inv_b->return_rate) return 1;
    return 0;
}

// Funcao para comparar nos por valor
int compare_by_value(const void *a, const void *b) {
    Node *node_a = *(Node**)a;
    Node *node_b = *(Node**)b;
    Investment *inv_a = (Investment*)node_a->element.data;
    Investment *inv_b = (Investment*)node_b->element.data;
    if (inv_a->value < inv_b->value) return -1;
    if (inv_a->value > inv_b->value) return 1;
    return 0;
}

// Funcao para ordenar e imprimir a lista
void print_sorted_list(Node *head, int (*comparator)(const void *, const void *)) {
    // Contar o numero de nos
    int count = 0;
    Node *current = head;
    while (current != NULL) {
        count++;
        current = current->next;
    }

    if (count == 0) {
        printf("A lista esta vazia.\n");
        return;
    }

    // Criar um array de ponteiros para nos
    Node **node_array = (Node**)malloc(count * sizeof(Node*));
    current = head;
    for (int i = 0; i < count; i++) {
        node_array[i] = current;
        current = current->next;
    }

    // Ordenar o array
    qsort(node_array, count, sizeof(Node*), comparator);

    // Imprimir os nos ordenados
    for (int i = 0; i < count; i++) {
        Investment *investment = (Investment*)node_array[i]->element.data;
        printf("Investment: %s | Value: %.2f | Return Rate: %.2f%%\n", investment->name, investment->value, investment->return_rate);
    }

    printf("----------------------\n");
    free(node_array); // Liberar a memoria alocada para o array
}

// Funcao para mostrar o menu e receber a escolha do usuario
int menu() {
    int choice;
    printf("\nEscolha uma opcao:\n");
    printf("1. Inserir Investimento\n");
    printf("2. Remover Investimento\n");
    printf("3. Buscar Investimento\n");
    printf("4. Imprimir Lista\n");
    printf("5. Imprimir Lista Ordenada por Nome\n");
    printf("6. Imprimir Lista Ordenada por Taxa de Retorno\n");
    printf("7. Imprimir Lista Ordenada por Valor\n");
    printf("8. Sair\n");
    printf("Opcao: ");
    scanf("%d", &choice);
    return choice;
}

// Funcao principal para demonstrar o uso
int main() {
    Node *list = create_list();
    int choice;

    do {
        choice = menu();
        switch (choice) {
            case 1: {
                Investment inv;
                printf("Nome do investimento: ");
                scanf(" %[^\n]", inv.name);
                printf("Valor do investimento: ");
                scanf("%lf", &inv.value);
                printf("Taxa de retorno: ");
                scanf("%lf", &inv.return_rate);
                insert_node(&list, INVESTMENT, &inv, sizeof(Investment));
                printf("Investimento inserido com sucesso.\n");
                break;
            }
            case 2: {
                char name[50];
                printf("Nome do investimento a remover: ");
                scanf(" %[^\n]", name);
                remove_node(&list, name);
                break;
            }
            case 3: {
                char name[50];
                printf("Nome do investimento a buscar: ");
                scanf(" %[^\n]", name);
                Node *found = search_node(list, name);
                if (found) {
                    Investment *found_inv = (Investment*)found->element.data;
                    printf("Investimento encontrado: %s com valor de %.2f e taxa de retorno de %.2f%%\n", found_inv->name, found_inv->value, found_inv->return_rate);
                } else {
                    printf("Investimento nao encontrado.\n");
                }
                break;
            }
            case 4: {
                printf("Lista de Investimentos:\n");
                print_list(list);
                break;
            }
            case 5: {
                printf("Lista de Investimentos Ordenada por Nome:\n");
                print_sorted_list(list, compare_by_name);
                break;
            }
            case 6: {
                printf("Lista de Investimentos Ordenada por Taxa de Retorno:\n");
                print_sorted_list(list, compare_by_return_rate);
                break;
            }
            case 7: {
                printf("Lista de Investimentos Ordenada por Valor:\n");
                print_sorted_list(list, compare_by_value);
                break;
            }
            case 8: {
                printf("Saindo...\n");
                break;
            }
            default:
                printf("Opcao invalida. Tente novamente.\n");
        }
    } while (choice != 8);

    // Liberacao da memoria da lista
    free_list(list);

    return 0;
}



