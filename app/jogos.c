#include "jogos.h"
#include "clientes.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* Base dos Jogos */

Estjogos *embaralharJogos(Estjogos *dados, int tam){
    srand(time(NULL));

    int rand_id = 0;

    for(int i = tam - 1; i > 0; i--){
        rand_id = rand() % (i + 1);
        Estjogos temp = dados[i];
        dados[i] = dados[rand_id];
        dados[rand_id] = temp;

    }

    return dados;    
}

void ler_dados_jogos(FILE *file){
    rewind(file);

    Estjogos *jogo = (Estjogos *) malloc(sizeof(Estjogos));
    while (fread(jogo, sizeof(Estjogos), 1, file) == 1){
        printf("-----------------------------------\n");
        printf("ID: %d\n", jogo->id);
        printf("Nome: %s\n", jogo->nome);
        printf("Estilo: %s\n", jogo->tipo);
        printf("Data de Lancamento: %s", jogo->data_lancamento);
        printf("\nÚltimo Emprestimo: %s\n", jogo->data_ultimo_emprestimo);
        if (jogo->disponivel == 1){
            printf("Estado: Disponível\n");
        } else {
            printf("Estado: Alugado | Cliente: %d\n ", jogo->id_cliente_aluguel);
        }
        printf("-----------------------------------\n");      
    }
    
}


int quantidade_jogos(FILE *file){
    int quantidade;

    fseek(file, 0, SEEK_END);
    long tamanho = ftell(file);
    quantidade = tamanho / sizeof(Estjogos);

    rewind(file);
    return quantidade;
}

FILE *bd_jogos(FILE *file){
    int numero_jogo = 0, tamanho_cat;
    char linha[100], *StringToken;
    FILE *wordlist = (fopen("wordlist.txt", "r"));
    Estjogos *jogo = NULL;

    file = fopen("bd_catalogo.dat", "wb+");

    while (fgets(linha, 100, wordlist)){
    jogo = realloc(jogo, (numero_jogo + 1) * sizeof(Estjogos));
        jogo[numero_jogo].id = numero_jogo + 1;
        StringToken = strtok(linha, ",\n");
        if (StringToken) strcpy(jogo[numero_jogo].nome, StringToken);
        StringToken = strtok(NULL, ",\n");
        if (StringToken) strcpy(jogo[numero_jogo].tipo, StringToken);
        StringToken = strtok(NULL, ",\n");
        if (StringToken) strcpy(jogo[numero_jogo].data_lancamento, StringToken);
        jogo[numero_jogo].disponivel = 1;
        numero_jogo++;
    }

    fclose(wordlist);

    printf("Tamanho do catálogo:\n>> ");
    scanf("%d", &tamanho_cat);

    jogo = embaralharJogos(jogo, tamanho_cat);

    fwrite(jogo, sizeof(Estjogos), tamanho_cat, file);
    printf("Catalogo foi gerado\n");
    free(jogo);
    fclose(file);
    return file;
}

void imprimir_catalogo(){
    int option;
    FILE *file = fopen("bd_catalogo.dat", "rb");

    printf("\tCatálogo de Jogos\n");
    printf("==============================================\n");

    if ((file == NULL)){
        printf("Nenhum catálogo encontrado. Verifique a base de dados.\n");
        return;
    }
    
    ler_dados_jogos(file);
    fclose(file);

    printf("0 - Voltar ao menu\n1 - Buscar por um jogo\n2 - Ordenar base (ShellSort)\n");
    scanf("%d", &option);

    switch (option){
    case 0:
        printf("Voltando ao menu...\n");
        break;

    case 1:
        int id;
        printf("Digite o ID do jogo: ");
        scanf("%d", &id);
        buscar_jogo(file, id);
        break;
    
    case 2:
        FILE *f = fopen("bd_catalogo.dat", "rb+");
        int tam = quantidade_jogos(f);
        rewind(f);
        shellSort(f, tam);
        break;

    default:
        break;
    }
}

void novo_jogo(FILE *file){
    Estjogos *jogo;

    if ((file == NULL)){
        printf("Nenhum catálogo encontrado. Verifique a base de dados.\n");
        return;
    }

    jogo = (Estjogos *)malloc(sizeof(Estjogos));
    int id = quantidade_jogos(file);
    jogo->id = id + 1;

    printf("Nome: ");
    scanf(" %[^\n]", jogo->nome);

    printf("Estilo: ");
    scanf(" %[^\n]", jogo->tipo);

    printf("Data de Lançamento(DD-MM-AAAA): ");
    scanf(" %[^\n]", jogo->data_lancamento);

    jogo->disponivel = 1; //* 1 - Disponível para aluguel | 0 - Já foi alugado

    fwrite(jogo, sizeof(Estjogos), 1, file);

    printf("Novo Jogo adicionado ao catálogo\n");
    free(jogo);
}


void alugar_jogo(int id_jogo, FILE *file){
    int id_cliente;
    char data_aluguel[12];
    long pos, pos_cliente;
    FILE *bd_clientes = fopen("bd_cli.dat", "rb+");
    Estjogos jogo;
    EstCliente cliente;

    rewind(file);
    rewind(bd_clientes);

    while (fread(&jogo, sizeof(Estjogos), 1, file) == 1) {
        if (jogo.id == id_jogo) {
            pos = ftell(file) - sizeof(Estjogos);
            break;
        }
    }

    if(jogo.disponivel != 1){
        printf("O jogo já foi alugado por outro cliente\n");
        return;
    }

    printf("Informe a data do empréstimo (DD-MM-AAAA): ");
    scanf(" %[^\n]", data_aluguel);
    printf("Informe o ID do cliente que está alugando: ");
    scanf("%d", &id_cliente);
    

    jogo.disponivel = 0; 
    strcpy(jogo.data_ultimo_emprestimo, data_aluguel);
    jogo.id_cliente_aluguel = id_cliente;
    char nome_jogo[30];
    strcpy(nome_jogo, jogo.nome);

    if (bd_clientes != NULL){
            while (fread(&cliente, sizeof(EstCliente), 1, bd_clientes) == 1) {
            if (id_cliente == cliente.id) {
                pos_cliente = ftell(bd_clientes) - sizeof(EstCliente);
                strcpy(cliente.jogo_alugado,nome_jogo);
                fseek(bd_clientes, pos_cliente, SEEK_SET);
                fwrite(&cliente, sizeof(EstCliente), 1, bd_clientes);
                printf("\n-----------------------------------\n");
                printf("ID: %d\n", cliente.id);
                printf("Nome: %s\n", cliente.nome_completo);
                printf("Email: %s\n", cliente.email);
                printf("Jogo Alugado: %s\n", cliente.jogo_alugado);
                printf("-----------------------------------\n");
                fclose(bd_clientes);
                break;

            }
        } 
    } else {
        printf("Houve um erro ao abrir o arquivo");
    }

    
    fseek(file, pos, SEEK_SET);
    fwrite(&jogo, sizeof(Estjogos), 1, file);
    
    printf("Jogo alugado com sucesso!\n");
    return;
}

void devolver_jogo(FILE *file){
    int id_jogo, id_cliente = -1;
    
    long pos, pos_cliente;
    FILE *jogos = fopen("bd_catalogo.dat", "rb+");
    EstCliente cliente;
    Estjogos jogo;

    rewind(file);
    rewind(jogos);

    printf("ID do jogo a ser devolvido: ");
    scanf("%d", &id_jogo);

    while (fread(&jogo, sizeof(Estjogos), 1, jogos) == 1){
        if (id_jogo == jogo.id){
            pos = ftell(jogos) - sizeof(Estjogos);
            id_cliente = jogo.id_cliente_aluguel;
            jogo.disponivel = jogo.disponivel - 1;
            jogo.id_cliente_aluguel--;
            memset(jogo.data_ultimo_emprestimo, 0, sizeof(jogo.data_ultimo_emprestimo));

            fseek(jogos, pos, SEEK_SET);
            fwrite(&jogo, sizeof(Estjogos), 1, jogos);
            fflush(jogos);
            break;
        }
    }

    while (fread(&cliente, sizeof(EstCliente), 1, file) == 1){
        if (cliente.id == id_cliente){
            pos_cliente = ftell(file) - sizeof(EstCliente);
            memset(cliente.jogo_alugado, 0, sizeof(cliente.jogo_alugado));
            fseek(file, pos_cliente, SEEK_SET);
            fwrite(&cliente, sizeof(EstCliente), 1, file);
            fflush(file);
            break;
        }
    }
    
    return;
}
void buscar_jogo(FILE *file, int id){
    Estjogos *procurado;

    int esquerda = 0, direita = ftell(file) / sizeof(Estjogos) - 1, meio = 0, option = 0;
    rewind(file);


    while(esquerda <= direita){

        meio = (esquerda + direita) / 2;
        fseek(file, meio * sizeof(Estjogos), SEEK_SET);
        procurado = (Estjogos *) malloc(sizeof(Estjogos));
        fread(procurado, sizeof(Estjogos), 1, file);

        if(procurado->id == id){
            printf("\n-----------------------------------\n");
            printf("ID: %d\n", procurado->id);
            printf("Nome: %s\n", procurado->nome);
            printf("Estilo: %s\n", procurado->tipo);
            printf("Data de Lancamento: %s", procurado->data_lancamento);
            printf("\nÚltimo Emprestimo: %s\n", procurado->data_ultimo_emprestimo);
            printf("Estado: %s\n", procurado->disponivel ? "Disponível" : "Alugado");
            printf("\n-----------------------------------\n");

            printf("Alugar Jogo?(1 - sim | 0 - nao)\n>> ");
            scanf("%d", &option);
            if (option == 1){
                alugar_jogo(procurado->id, file);
            } else if(option == 0){
                printf("voltando ao menu...");
                return;
            }
            free(procurado);
            return;
        }

        if(procurado->id > id){
            direita = meio - 1;
        }

        if (procurado->id < id){
            esquerda = meio + 1;
        }
    
    }
    free(procurado);
}

/* ALGORITMOS DE ORDENAÇÃO */

void shellSort(FILE *file, int tam){
    int espaco = 0;
    Estjogos temp, espacoJogo;

    for (espaco = tam / 2; espaco > 0; espaco /= 2){

        for (int i = espaco; i < tam; i++){
            fseek(file, i * sizeof(Estjogos), SEEK_SET);
            fread(&temp, sizeof(Estjogos), 1, file);
            int j = i;

            while (j >= espaco){
                fseek(file, (j - espaco) * sizeof(Estjogos), SEEK_SET);
                fread(&espacoJogo, sizeof(Estjogos), 1, file);

                if (espacoJogo.id > temp.id){
                    fseek(file, j * sizeof(Estjogos), SEEK_SET);
                    fwrite(&espacoJogo, sizeof(Estjogos), 1, file);

                    j -= espaco;
                } else {
                    break;
                }
            }

            fseek(file, j * sizeof(Estjogos), SEEK_SET);
            fwrite(&temp, sizeof(Estjogos), 1, file);
        }
    }
    ler_dados_jogos(file);
    fclose(file);
    printf("Ordenação concluida\n");
    sleep(2);
}
