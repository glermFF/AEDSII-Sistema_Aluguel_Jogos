#include "clientes.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

EstCliente *embaralhar(EstCliente *dados, int tam){
    srand(time(NULL));

    int rand_id = 0;

    for(int i = tam - 1; i > 0; i--){
        rand_id = rand() % (i + 1);
        EstCliente temp = dados[i];
        dados[i] = dados[rand_id];
        dados[rand_id] = temp;

    }

    return dados;
}

void ler_dados_clientes(FILE *file){

    rewind(file);
    
    EstCliente *clientes = (EstCliente *) malloc(sizeof(EstCliente));
    while (fread(clientes, sizeof(EstCliente), 1, file) == 1){
        printf("-----------------------------------\n");
        printf("ID: %d\n", clientes->id);
        printf("Cliente: %s | %s\n", clientes->nome_completo, clientes->data_nacimento);
        printf("Email: %s\n", clientes->email);
        printf("Jogo alugado: %s\n", clientes->jogo_alugado);
        printf("-----------------------------------\n");
    }    
}

/* Base dos Clientes */
int quantidade_clientes(FILE *file){
    EstCliente cliente;

    fseek(file, -sizeof(EstCliente), SEEK_END);

    if (fread(&cliente, sizeof(EstCliente), 1, file) == 1){
        return cliente.id;
    }

    return 0;
}

void cadastrar_cliente(FILE *file){
    EstCliente *cliente;

    if ((file == NULL)){
        printf("Nenhuma lista foi encontrada. Verifique a base de dados.\n");
        return;
    }

    printf("\tCadastre-se\n");
    printf("==============================================\n");

    cliente = (EstCliente *)malloc(sizeof(EstCliente));

    int new_id = quantidade_clientes(file);
    cliente->id = new_id + 1;

    printf("Nome: ");
    scanf(" %[^\n]", cliente->nome_completo);

    printf("Data de Nascimento(DD-MM-AAAA): ");
    scanf(" %[^\n]", cliente->data_nacimento);

    printf("Email: ");
    scanf(" %[^\n]", cliente->email);

    fwrite(cliente, sizeof(EstCliente), 1, file);

    printf("Cliente cadastrado com sucesso\n");
    free(cliente);
}

void imprimir_clientes(){
    int choice;
    FILE *file = fopen("bd_cli.dat", "rb");

    printf("\tClientes Registrados\n");
    printf("=========================================\n");

    if ((file == NULL)){
        printf("Nenhuma lista foi encontrada. Verifique a base de dados.\n");
        return;
    }

    ler_dados_clientes(file);
    fclose(file);
    printf("0 - Sair\n1 - Procurar por um cliente\n2 - Gerar partições\n>> ");
    scanf("%d", &choice);

    switch (choice){
    case 0:
        printf("Voltando ao menu...\n");
        break;
    
    case 1:
        buscar_cliente(file);
        break;

    case 2:
        FILE *f = fopen("bd_cli.dat", "rb+");
        int num_part = selecao_natural(f);
        unir_particoes(num_part);
        break;

    default:
        break;
    }

}

FILE *db_clientes(FILE *file){
    int numero_cliente = 0, tamanho_lista;
    char linha[200], *StringToken;
    FILE *wordlist = (fopen("wordlist_clientes.txt", "r"));
    EstCliente *cliente = NULL;

    file = fopen("bd_cli.dat", "wb+");

    while (fgets(linha, 100, wordlist)){
    cliente = realloc(cliente, (numero_cliente + 1) * sizeof(EstCliente));
        cliente[numero_cliente].id = numero_cliente + 1;
        StringToken = strtok(linha, ",\n");
        if (StringToken) strcpy(cliente[numero_cliente].nome_completo, StringToken);
        StringToken = strtok(NULL, ",\n");
        if (StringToken) strcpy(cliente[numero_cliente].data_nacimento, StringToken);
        StringToken = strtok(NULL, ",\n");
        if (StringToken) strcpy(cliente[numero_cliente].email, StringToken);
        numero_cliente++;
    }

    fclose(wordlist);

    printf("Tamanho da lista:\n>> ");
    scanf("%d", &tamanho_lista);

    cliente = embaralhar(cliente, tamanho_lista);
    
    fwrite(cliente, sizeof(EstCliente), tamanho_lista, file);
    printf("Lista foi gerada com sucesso\n");
    free(cliente);
    fclose(file);
    return file;   
}


/* ALGORITMOS DE BUSCA */

void buscar_cliente(FILE *file){
    int id_cliente, found = 0;
    EstCliente cliente;

    printf("ID do cliente\n>> ");
    scanf("%d", &id_cliente);

    rewind(file);

    while (fread(&cliente, sizeof(cliente), 1, file) == 1){
        if (id_cliente == cliente.id){
            printf("\n-----------------------------------\n");
            printf("ID: %d\n", cliente.id);
            printf("Nome: %s\n", cliente.nome_completo);
            printf("Email: %s\n", cliente.email);
            printf("Jogo Alugado: %s\n", cliente.jogo_alugado);
            printf("-----------------------------------\n");
            found = 1;
            int option2 = 0;
            printf("1 - Devolver jogo | 2 - Voltar para o menu\n");
            scanf("%d", &option2);
            if (option2 == 1){
                devolver_jogo(file);
            } else if (option2 == 2){
                return;
            }
            break;
        } 
        
    }
    
    if(found == 0){
        printf("Cliente não encontrado. Verifique se ID está correto\n");
        return;
    }

    return;
}

/* ALOGORITMO DE ORDENAÇÃO (PARTIÇÕES) */

/* PARTICIONAMENTO */
void swap(EstCliente *a, EstCliente *b) {
    EstCliente temp = *a;
    *a = *b;
    *b = temp;
}

void reheap_down(EstCliente heap[], int n, int i) {
    int menor = i;
    int esq = 2 * i + 1;
    int dir = 2 * i + 2;

    if (esq < n && heap[esq].id < heap[menor].id)
        menor = esq;

    if (dir < n && heap[dir].id < heap[menor].id)
        menor = dir;

    if (menor != i) {
        swap(&heap[i], &heap[menor]);
        reheap_down(heap, n, menor);
    }
}

void build_min_heap(EstCliente heap[], int n) {
    for (int i = (n / 2) - 1; i >= 0; i--) {
        reheap_down(heap, n, i);
    }
}

int selecao_natural(FILE *file) {
    int registros = 0, lidos = 0, particoes = 0, limite = 5;
    int quantidade = quantidade_clientes(file);
    char partitionName[20];
    EstCliente *heap;

    rewind(file);

    while (lidos <= quantidade) { // Continua enquanto houver registros a ler
        heap = (EstCliente *) malloc(limite * sizeof(EstCliente));
        int ativos = 0;

        // Carregar o heap inicial
        while (ativos < limite && fread(&heap[ativos], sizeof(EstCliente), 1, file) == 1) {
            heap[ativos].congelado = 0;
            ativos++;
            lidos++;
        }

        // Construir o heap mínimo
        build_min_heap(heap, ativos);

        // Criar a partição
        sprintf(partitionName, "partition%d.dat", particoes);
        FILE *p = fopen(partitionName, "wb");
        if (!p) {
            printf("Erro ao criar a partição %d\n", particoes - 1);
            free(heap);
            return -1;
        }

        while (ativos != 0) {

            EstCliente menor = heap[0];
            fwrite(&menor, sizeof(EstCliente), 1, p); 
            registros++;

            EstCliente novo;
            if (fread(&novo, sizeof(EstCliente), 1, file) == 1) {
                lidos++;

                if (novo.id >= menor.id) {
                    novo.congelado = 0;
                    
                } else {
                    novo.congelado = 1; 
                    ativos--;
                }
                heap[0] = novo; 
            } else {
                
                heap[0] = heap[ativos - 1];
                ativos--; 
            }
            
            reheap_down(heap, ativos, 0);
        }

        fclose(p);
        free(heap);
        particoes++;
    }

    printf("As partições %d foram geradas!\n", particoes);
    return particoes;
}

void unir_particoes(int partitions){
    char parName[20];
    EstCliente dados;
    FILE *file;

    file = fopen("bd_cli.dat", "wb+");

    for(int i = 0; i < partitions; i++){
        sprintf(parName, "partition%d.dat", i);
        FILE *partition = fopen(parName, "rb");

        while (fread(&dados, sizeof(EstCliente), 1, partition) == 1){
            fwrite(&dados, sizeof(EstCliente), 1, file);
        }
        fclose(partition);
        remove(parName);
    }
    fclose(file);
}