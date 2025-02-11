#include "entidades.c"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void telaInicial();

int main(){
    FILE *base_clientes, *base_catalogo;
    int option, option2;

    do{
        system("clear");
        telaInicial();
        printf(">> ");
        scanf("%d", &option);

        switch (option){
        case 1:
            system("clear");
            imprimir_catalogo(base_catalogo);
            sleep(2);
            break;
        case 2:
            system("clear");
            imprimir_clientes(base_clientes);
            sleep(3);
            break;
        case 3:
            system("clear");
            printf("1 - Gerar Catalogo de Jogos\n2 - Gerar Lista de Clientes\n >> ");
            scanf("%d", &option2);
            if (option2 == 1){
                base_catalogo = bd_jogos(base_catalogo);
            }
            if (option2 == 2){
                base_clientes = db_clientes(base_clientes);
            }
            sleep(2);
            break;
        case 4:
            novo_jogo(base_catalogo);
            break;
        case 5:
            system("clear");
            cadastrar_cliente(base_clientes);
            sleep(2);
            break;
        case 6:
            printf("Saindo...\n");
            break;
        
        default:
            printf("Não há esta opção em nosso menu\n");
        }
    } while(option != 6);
    exit(1);
}


void telaInicial(){
    printf("==============================================\n");
    printf("\tSistema de Aluguel de Jogos\n");
    printf("==============================================\n\n");
    printf("1 - Catálogo\n");
    printf("2 - Nossos Clientes\n\n");
    printf("3 - Gerar Bases de Dados\n\n");
    printf("4 - Novo Jogo\n");
    printf("5 - Cadastrar Cliente\n\n");
    printf("6 - Sair...\n\n");
}