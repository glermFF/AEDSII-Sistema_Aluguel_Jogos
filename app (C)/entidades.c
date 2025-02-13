#include "entidades.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Base dos Clientes

int quantidade_clientes(FILE *file){
    EstCliente cliente;

    fseek(file, -sizeof(EstCliente), SEEK_END);

    if (fread(&cliente, sizeof(EstCliente), 1, file) == 1){
        return cliente.id;
    }

    return 0;
}

void cadastrar_cliente(FILE *file){
    EstCliente *cliente = (EstCliente *)malloc(sizeof(EstCliente));

    printf("\tCadastre-se\n");
    printf("==============================================\n");

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

void imprimir_clientes(FILE *file){
    printf("\tClientes Registrados\n");
    printf("=========================================\n");

    if ((file == NULL)){
        printf("Nenhuma lista foi encontrada. Verifique a base de dados.\n");
        return;
    }
    file = fopen("bd_cli.dat", "rb");
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

    int choice;
    printf("Procurar por um cliente?(Sim - 1 | Não - 0)\n>> ");
    scanf("%d", &choice);

    if (choice == 1){
        buscar_cliente(file);
    } else if (choice == 0){
        printf("Voltando ao menu...\n");
        return;
    }

    printf("Voltando ao menu...");
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

    fwrite(cliente, sizeof(EstCliente), tamanho_lista, file);
    printf("Lista foi gerada com sucesso\n");
    free(cliente);
    fclose(file);
    return file;   
}

// Base dos Jogos

int quantidade_jogos(FILE *file){
    Estjogos cliente;

    rewind(file);
    fseek(file, -sizeof(Estjogos), SEEK_END);

    if (fread(&cliente, sizeof(Estjogos), 1, file) == 1){
        return cliente.id;
    }

    return 0;
}

void novo_jogo(FILE *file){
    Estjogos *jogo = (Estjogos *)malloc(sizeof(Estjogos));

    int id = quantidade_jogos(file);
    jogo->id = id + 1;

    printf("Nome: ");
    scanf(" %[^\n]", jogo->nome);

    printf("Estilo: ");
    scanf(" %[^\n]", jogo->tipo);

    printf("Data de Lançamento(DD-MM-AAAA): ");
    scanf(" %[^\n]", jogo->data_lancamento);

    jogo->disponivel = 1; //? 1 - Disponível para aluguel | 0 - Já foi alugado

    fwrite(jogo, sizeof(Estjogos), 1, file);

    printf("Novo Jogo adicionado ao catálogo\n");
    free(jogo);
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

    fwrite(jogo, sizeof(Estjogos), tamanho_cat, file);
    printf("Catalogo foi gerado\n");
    free(jogo);
    return file;
}

void imprimir_catalogo(FILE *file){
    printf("\tCatálogo de Jogos\n");
    printf("==============================================\n");

    if ((file == NULL)){
        printf("Nenhum catálogo encontrado. Verifique a base de dados.\n");
        return;
    }
    rewind(file);
    int option;
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

    printf("1 - Buscar por um jogo | 2 - Voltar ao menu\n");
    scanf("%d", &option);

    if (option == 1 ){
        int id;
        printf("Digite o ID do jogo: ");
        scanf("%d", &id);
        buscar_jogo(file, id);
       
    } else if (option == 2) {return;}

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