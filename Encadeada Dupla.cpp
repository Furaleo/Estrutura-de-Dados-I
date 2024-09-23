#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

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

// Estrutura do no da lista encadeada dupla
typedef struct Node {
    Element element;   // Elemento que contem o dado
    struct Node *next; // Ponteiro para o proximo no
    struct Node *prev; // Ponteiro para o no anterior
} Node;

// Estrutura que representa a lista encadeada dupla
typedef struct {
    Node *head;  // Ponteiro para o primeiro no
    Node *tail;  // Ponteiro para o ultimo no
    size_t size; // Tamanho da lista
} DoublyLinkedList;

// Funcao para criar uma nova lista
DoublyLinkedList* create_list() {
    DoublyLinkedList *list = (DoublyLinkedList*)malloc(sizeof(DoublyLinkedList));
    if (!list) {
        fprintf(stderr, "Falha na alocacao de memoria\n");
        exit(EXIT_FAILURE);
    }
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    return list;
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
    new_node->prev = NULL;
    return new_node;
}

// Funcao para inserir um no de forma ordenada na lista por valor
void insert_node(DoublyLinkedList *list, DataType type, void *data, size_t data_size) {
    Node *new_node = create_node(type, data, data_size);
    Node *current = list->head;

    // Caso a lista esteja vazia
    if (current == NULL) {
        list->head = new_node;
        list->tail = new_node;
        list->size++;
        return;
    }

    // Insercao ordenada
    while (current != NULL && ((Investment*)current->element.data)->value < ((Investment*)data)->value) {
        current = current->next;
    }

    // Insercao no inicio da lista
    if (current == list->head) {
        new_node->next = list->head;
        list->head->prev = new_node;
        list->head = new_node;
    }
    // Insercao no final da lista
    else if (current == NULL) {
        new_node->prev = list->tail;
        list->tail->next = new_node;
        list->tail = new_node;
    }
    // Insercao no meio da lista
    else {
        new_node->prev = current->prev;
        new_node->next = current;
        current->prev->next = new_node;
        current->prev = new_node;
    }
    list->size++;
}

// Funcao para remover um no da lista
void remove_node(DoublyLinkedList *list, const char *name) {
    Node *current = list->head;

    while (current != NULL) {
        Investment *investment = (Investment*)current->element.data;
        if (strcmp(investment->name, name) == 0) break;
        current = current->next;
    }

    if (current == NULL) {
        printf("Investimento nao encontrado.\n");
        return; // Elemento nao encontrado
    }

    if (current == list->head) {
        list->head = current->next;
        if (list->head != NULL) {
            list->head->prev = NULL;
        } else {
            list->tail = NULL; // A lista ficou vazia
        }
    } else if (current == list->tail) {
        list->tail = current->prev;
        if (list->tail != NULL) {
            list->tail->next = NULL;
        } else {
            list->head = NULL; // A lista ficou vazia
        }
    } else {
        current->prev->next = current->next;
        current->next->prev = current->prev;
    }

    free(current->element.data); // Libera o dado alocado dinamicamente
    free(current); // Libera o no
    list->size--;
    printf("Investimento removido com sucesso.\n");
}

// Funcao para procurar um no na lista
Node* search_node(DoublyLinkedList *list, const char *name) {
    Node *current = list->head;
    while (current != NULL) {
        Investment *investment = (Investment*)current->element.data;
        if (strcmp(investment->name, name) == 0) return current;
        current = current->next;
    }
    return NULL; // Elemento nao encontrado
}

// Funcao para formatar e imprimir o valor monetario
void print_money(double amount) {
    if (amount == (int)amount) {
        printf("R$ %.0f", amount);
    } else {
        // Formatar o valor para remover os zeros finais
        char buffer[50];
        snprintf(buffer, sizeof(buffer), "%.2f", amount);
        char *end = buffer + strlen(buffer) - 1;
        while (end > buffer && *end == '0') {
            end--;
        }
        if (*end == '.') {
            end--;
        }
        *(end + 1) = '\0';
        printf("R$ %s", buffer);
    }
}

// Funcao para imprimir a lista do inicio ao fim
void print_list(DoublyLinkedList *list, int print_option) {
    Node *current = list->head;
    if (current == NULL) {
        printf("A lista esta vazia.\n");
    } else {
        while (current != NULL) {
            Investment *investment = (Investment*)current->element.data;
            if (print_option == 1) { // Imprimir apenas nome
                printf("Investimento: %s\n", investment->name);
            } else if (print_option == 2) { // Imprimir apenas valor
                print_money(investment->value);
                printf("\n");
            } else if (print_option == 3) { // Imprimir nome, valor e taxa de retorno
                printf("Investimento: %s | Valor: ", investment->name);
                print_money(investment->value);
                printf(" | Taxa de Retorno: %.2f%%\n", investment->return_rate);
            }
            current = current->next;
        }
        printf("----------------------\n");
    }
}

// Funcao para imprimir a lista do fim ao inicio
void print_reverse(DoublyLinkedList *list, int print_option) {
    Node *current = list->tail;
    if (current == NULL) {
        printf("A lista esta vazia.\n");
    } else {
        while (current != NULL) {
            Investment *investment = (Investment*)current->element.data;
            if (print_option == 1) { // Imprimir apenas nome
                printf("Investimento: %s\n", investment->name);
            } else if (print_option == 2) { // Imprimir apenas valor
                print_money(investment->value);
                printf("\n");
            } else if (print_option == 3) { // Imprimir nome, valor e taxa de retorno
                printf("Investimento: %s | Valor: ", investment->name);
                print_money(investment->value);
                printf(" | Taxa de Retorno: %.2f%%\n", investment->return_rate);
            }
            current = current->prev;
        }
        printf("----------------------\n");
    }
}

// Funcao para liberar toda a memoria da lista
void free_list(DoublyLinkedList *list) {
    Node *current = list->head;
    Node *next;

    while (current != NULL) {
        next = current->next;
        free(current->element.data); // Libera o dado alocado dinamicamente
        free(current); // Libera o no
        current = next;
    }
    free(list); // Libera a estrutura da lista
}

// Funcao para comparar nos por nome (ordem alfabetica)
int compare_by_name(const void *a, const void *b) {
    Node *node_a = *(Node**)a;
    Node *node_b = *(Node**)b;
    Investment *inv_a = (Investment*)node_a->element.data;
    Investment *inv_b = (Investment*)node_b->element.data;
    return strcmp(inv_a->name, inv_b->name);
}

// Funcao para calcular o rendimento de um investimento ao longo do tempo
void calculate_return(Investment *investment, int years) {
    double final_value = investment->value * pow(1 + investment->return_rate / 100, years);
    printf("Valor inicial: ");
    print_money(investment->value);
    printf(" | Valor apos %d anos: ", years);
    print_money(final_value);
    printf("\n");
}

int main() {
    DoublyLinkedList *list = create_list();

    // Adicionando investimentos de exemplo
    Investment inv1 = {"Acao X", 1000.00, 5.0};
    Investment inv2 = {"Acao Y", 500.00, 7.0};
    Investment inv3 = {"Acao Z", 1500.00, 4.5};

    insert_node(list, INVESTMENT, &inv1, sizeof(Investment));
    insert_node(list, INVESTMENT, &inv2, sizeof(Investment));
    insert_node(list, INVESTMENT, &inv3, sizeof(Investment));

    // Imprimir a lista (exemplo)
    printf("Lista de Investimentos - Impressao do inicio ao fim:\n");
    print_list(list, 3);  // 1 = Nome, 2 = Valor, 3 = Nome, Valor e Taxa de Retorno

    printf("Lista de Investimentos - Impressao do fim ao inicio:\n");
    print_reverse(list, 3);  // 1 = Nome, 2 = Valor, 3 = Nome, Valor e Taxa de Retorno

    // Calcular e imprimir rendimento de um investimento
    Node *node = search_node(list, "Acao X");
    if (node) {
        calculate_return((Investment*)node->element.data, 1); // Calcula o rendimento apos 1 ano
    }

    // Liberar memoria
    free_list(list);

    return 0;
}

