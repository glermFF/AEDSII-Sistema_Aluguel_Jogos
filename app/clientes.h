#ifndef CLIENTES_H
#define CLIENTES_H

#include <stdlib.h>
#include <stdio.h>

typedef struct Cliente{
    int id;
    char nome_completo[100];
    char data_nacimento[15];
    char email[50];
    char jogo_alugado[30]; 
    int congelado;
}EstCliente;

void ler_dados_clientes(FILE *file);
void ler_dados_jogos(FILE *file);
void unir_particoes(int partitions);

int quantidade_clientes(FILE *file);
void cadastrar_cliente(FILE *file);
void imprimir_clientes();
FILE *db_clientes(FILE *file);

/* Algoritmos*/

// Busca Sequencial
void buscar_cliente(FILE *file);

EstCliente *embaralhar(EstCliente *dados, int tam); 

// Seleção Natural
int selecao_natural(FILE *file);

// Oredenação

#endif