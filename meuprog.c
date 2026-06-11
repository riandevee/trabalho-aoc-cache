 /*
 * Trabalho de Organizacao e Arquitetura de Computadores
 * Simulador de Mapeamento de Memoria Cache
 * Alunos: Rian Ruela Ribeiro e Geovani Santana Michels
 * Professor: Rodrigo Porfirio da Silva Sacchi
 */
// Arquivo meuprog.c
#if !defined __minhalib_h
#include "minhalib.h"
#endif
/*
funcao principal, fica rodando o menu ate o usuario escolher sair
opcao 1: entrada manual pelo teclado
opcao 2: leitura a partir de arquivo texto
opcao 3: encerra o programa
 */
int main() {
    Mem_pri mp;
    Cache   cache;
    int op;
    do {
        op = exibir_menu();
        if (op == 1) {
            printf("\n Dados da MP \n");
            ler_mp(&mp);
            printf("\n Dados da Cache \n");
            ler_cache(&cache);
            calcular_campos(&mp, &cache);
            exibir_resultados(&mp, &cache);
        } else if (op == 2) {
            printf("\n Leitura por arquivo \n");
            if (ler_de_arquivo(&mp, &cache)) {
                calcular_campos(&mp, &cache);
                exibir_resultados(&mp, &cache);
            }
        }

    } while (op != 3);
    printf("Encerrando...\n");
    return 0;
}
