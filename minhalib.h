// Arquivo minhalib.h
#if !defined __minhalib_h
#define __minhalib_h

/*
 * Trabalho de Organizacao e Arquitetura de Computadores
 * Simulador de Mapeamento de Memoria Cache
 * Alunos: Rian Ruela Ribeiro e Geovani Santana Michels
 * Professor: Rodrigo Porfirio da Silva Sacchi
 */

#include <stdio.h>
#include <math.h>
/* aqui guarda tudo que é da memoria principal */
typedef struct {
    int tamanho_kb;         /* tamanho que o usuario digita em KB */
    int palavras_por_bloco; /* quantas palavras cabem em cada bloco */
    int num_blocos;         /* calculado: tamanho total dividido pelas palavras do bloco */
    int bits_endereco;      /* total de bits do endereco, tambem calculado */
} Mem_pri;
/*
 * aqui ficam os campos de bits do endereco
 * depende do tipo de mapeamento quais campos sao usados:
 *   Direto:              tag | r | w
 *   Tot. associativo:    tag | d
 *   Assoc. por conjunto: tag | s | w
 */
typedef struct {
    int w;    /* bits de offset, diz qual palavra dentro do bloco */
    int r;    /* bits de linha, so usa no mapeamento direto */
    int d;    /* deslocamento, so usa no totalmente associativo */
    int s;    /* bits de conjunto, so usa no associativo por conjunto */
    int tag;  /* o resto dos bits vira tag, serve pra identificar o bloco */
} Campos_bits;

/* aqui guarda tudo que é da cache */
typedef struct {
    int tipo;                /* 1=direto  2=tot. associativo  3=assoc. por conjunto */
    int tam_bytes;           /* tamanho total da cache em bytes */
    int tam_linha;           /* tamanho de cada linha em bytes */
    int num_linhas;          /* calculado: divide o tamanho da cache pelo tamanho da linha */
    int num_conjuntos;       /* so no tipo 3, calculado dividindo linhas por linhas_por_conjunto */
    int linhas_por_conjunto; /* so no tipo 3, o usuario informa */
    Campos_bits bits;        /* os campos do endereco ficam aqui dentro */
} Cache;
/* prototipos das funcoes — implementadas no minhalib.c */
int  eh_potencia_de_2    (int n);
int  ler_inteiro_positivo(const char *msg);
int  ler_potencia_de_2   (const char *msg);
void calcular_campos     (Mem_pri *mp, Cache *cache);
void ler_mp              (Mem_pri *mp);
void ler_cache           (Cache *cache);
int  ler_de_arquivo      (Mem_pri *mp, Cache *cache);
void exibir_resultados   (Mem_pri *mp, Cache *cache);
int  exibir_menu         ();

#endif