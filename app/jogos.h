#ifndef JOGOS_H
#define JOGOS_H

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

int quantidade_jogos(FILE *file);
void imprimir_catalogo();
void novo_jogo(FILE *file);
FILE *bd_jogos(FILE *file);

void devolver_jogo(FILE *file); 
void alugar_jogo(int id_jogo, FILE *file); 

/* Algoritmos */

// Busca Binária
void buscar_jogo(FILE *file, int id); 

// Ordenação 
void shellSort(FILE *file, int tam);

#endif