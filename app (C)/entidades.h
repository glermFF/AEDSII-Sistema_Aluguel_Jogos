#ifndef ENTIDADES_H

#include <stdlib.h>
#include <stdio.h>

typedef struct Jogo{
    int id;
    char nome[30];
    char tipo[30];
    char data_lancamento[12];
    char data_ultimo_emprestimo[12];
    int disponivel;
    int id_cliente_aluguel;
}Estjogos;

typedef struct Cliente{
    int id;
    char nome_completo[100];
    char data_nacimento[15];
    char email[50];
    char jogo_alugado[30];
}EstCliente;

/* Clientes */

int quantidade_clientes(FILE *file);
void cadastrar_cliente(FILE *file);
void imprimir_clientes(FILE *file);
FILE *db_clientes(FILE *file);

/* Jogos */
int quantidade_jogos(FILE *file);
void novo_jogo(FILE *file);
FILE *bd_jogos(FILE *file);
void imprimir_catalogo(FILE *file);

void devolver_jogo(FILE *file);
void alugar_jogo(int id_jogo, FILE *file);


/* Algoritmos*/
void buscar_jogo(FILE *file, int id); //* Usado na forma de busca biária
void buscar_cliente(FILE *file); //* Usado na forma de busca sequencial

#endif