/*
 * Trabalho de Organizacao e Arquitetura de Computadores
 * Simulador de Mapeamento de Memoria Cache
 * Alunos: Rian Ruela Ribeiro e Geovani Santana Michels
 * Professor: Rodrigo Porfirio da Silva Sacchi
 */
// Arquivo minhalib.c
#if !defined __minhalib_h
#include "minhalib.h"
#endif

/*
 * verifica se um numero inteiro e potencia de 2
 * utiliza operacao bitwise: um numero n e potencia de 2 se n > 0
 * e se (n & (n-1)) == 0, pois potencias de 2 tem apenas um bit setado
 * retorna 1 se for potencia de 2, ou 0 caso contrario
 */
int eh_potencia_de_2(int n) {
    if (n <= 0) return 0;
    return (n & (n - 1)) == 0;
}

/*
 * exibe a mensagem msg e le um numero inteiro do teclado
 * repete a leitura enquanto o valor digitado for menor ou igual a zero
 * retorna o primeiro valor valido (maior que zero) digitado pelo usuario
 */
int ler_inteiro_positivo(const char *msg) {
    int valor;
    do {
    printf("%s", msg);
    scanf("%d", &valor);
    if (valor <= 0)
    printf("Erro: valor deve ser maior que zero.\n");
    } while (valor <= 0);
    return valor;
}

/*
 * exibe a mensagem msg e le um numero inteiro do teclado
 * repete a leitura enquanto o valor digitado nao for uma potencia de 2 valida
 * retorna o primeiro valor que satisfaca a condicao de potencia de 2
 */
int ler_potencia_de_2(const char *msg) {
    int valor;
    do {
    printf("%s", msg);
     scanf("%d", &valor);
    if (!eh_potencia_de_2(valor))
     printf("Erro: valor deve ser potencia de 2 (ex: 2, 4, 8, 16...)\n");
    } while (!eh_potencia_de_2(valor));
    return valor;
}
/*
essa funcao faz todos os calculos depois que o usuario digitou os dados
calcula blocos, bits de endereco, linhas da cache e os campos w r d s tag
tem que chamar ela antes de mostrar os resultados
 */
void calcular_campos(Mem_pri *mp, Cache *cache) {
    /* primeiro calcula as coisas da MP */
    int tam_mp_bytes  = mp->tamanho_kb * 1024;
    mp->num_blocos    = tam_mp_bytes / mp->palavras_por_bloco;
    mp->bits_endereco = (int)(log2(mp->num_blocos)+ log2(mp->palavras_por_bloco));
    /* calcula as linhas da cache e o campo w que é igual pra todos os tipos */
    cache->num_linhas = cache->tam_bytes / cache->tam_linha;
    cache->bits.w = (int)log2(mp->palavras_por_bloco);
    /* zera os campos que nao vao ser usados dependendo do tipo */
    cache->bits.r = 0;
    cache->bits.d = 0;
    cache->bits.s = 0;
    switch (cache->tipo) {
        case 1: /* direto: divide o endereco em tag | r | w */
            cache->bits.r   = (int)log2(cache->num_linhas);
            cache->bits.tag = mp->bits_endereco
                            - cache->bits.r
                            - cache->bits.w;
            break;
        case 2: /* totalmente associativo: so tem tag | d, sem campo de linha */
        cache->bits.d   = cache->bits.w;
        cache->bits.tag = mp->bits_endereco - cache->bits.d;
        break;
        case 3: /* associativo por conjunto: divide em tag | s | w */
        cache->num_conjuntos = cache->num_linhas/ cache->linhas_por_conjunto;
        cache->bits.s   = (int)log2(cache->num_conjuntos);
        cache->bits.tag = mp->bits_endereco- cache->bits.s- cache->bits.w;
            break;
    }
}

/*
 * solicita ao usuario os dados da memoria principal via teclado
 * le o tamanho da MP em KB (qualquer inteiro positivo) e a quantidade
 * de palavras por bloco (deve ser potencia de 2), ambos com validacao
 * armazena os valores lidos na estrutura apontada por mp
 */
void ler_mp(Mem_pri *mp) {
    mp->tamanho_kb         = ler_inteiro_positivo("Tamanho da MP (KB): ");
    mp->palavras_por_bloco = ler_potencia_de_2("Palavras por bloco: ");
}
/*
pede os dados da cache pro usuario
valida o tipo, o tamanho e garante que a linha nao seja maior que a cache se for tipo 3 pede tambem quantas linhas tem por conjunto
 */
void ler_cache(Cache *cache) {
    do {
    printf("  Tipo (1=Direto  2=Tot.Assoc  3=Assoc.Conjunto): ");
    scanf("%d", &cache->tipo);
    if (cache->tipo < 1 || cache->tipo > 3)
    printf("  Erro: tipo deve ser 1, 2 ou 3.\n");
    } while (cache->tipo < 1 || cache->tipo > 3);

    cache->tam_bytes = ler_potencia_de_2("  Tamanho da cache (bytes): ");

    /* nao faz sentido a linha ser maior ou igual ao tamanho da cache inteira */
    do {
    cache->tam_linha = ler_potencia_de_2("  Tamanho da linha (bytes): ");
    if (cache->tam_linha >= cache->tam_bytes)
    printf("  Erro: linha (%d bytes) deve ser menor que a cache (%d bytes).\n",
    cache->tam_linha, cache->tam_bytes);
    } while (cache->tam_linha >= cache->tam_bytes);

    if (cache->tipo == 3)
    cache->linhas_por_conjunto = ler_potencia_de_2("  Linhas por conjunto: ");
    else
    cache->linhas_por_conjunto = 0;
}

/*
 * mostra todos os resultados calculados na tela
o que aparece muda dependendo do tipo de mapeamento escolhido
 */
void exibir_resultados(Mem_pri *mp, Cache *cache) {
    const char *tipos[] = {"", "Direto", "Totalmente Associativo","Associativo por Conjunto"};

    printf("\nResultados \n");
    printf("-- Memoria Principal --\n");
    printf("Tamanho: %d KB (%d bytes)\n",
    mp->tamanho_kb, mp->tamanho_kb * 1024);
    printf("Palavras/bloco: %d\n", mp->palavras_por_bloco);
    printf("Num. blocos: %d\n", mp->num_blocos);
    printf("Bits endereco: %d\n", mp->bits_endereco);

    printf("\n-- Cache --\n");
    printf("Tipo: %s\n", tipos[cache->tipo]);
    printf("Tamanho: %d bytes\n", cache->tam_bytes);
    printf("Tam. linha: %d bytes\n", cache->tam_linha);
    printf("Num. linhas: %d\n", cache->num_linhas);
    /* so mostra conjuntos se for associativo por conjunto */
    if (cache->tipo == 3) {
        printf("Num. conjuntos: %d (calculado)\n", cache->num_conjuntos);
        printf("Linhas/conj.: %d\n", cache->linhas_por_conjunto);
    }
    printf("\n Campos do endereco \n");
    printf("w (palavra): %d bit(s)\n", cache->bits.w);

    /* cada tipo mostra um campo diferente alem do w e da tag */
    switch (cache->tipo) {
        case 1:
            printf("r (linha): %d bit(s)\n", cache->bits.r);
            break;
        case 2:
            printf("d (desloc.): %d bit(s)\n", cache->bits.d);
            break;
        case 3:
            printf("s(conjunto): %d bit(s)\n", cache->bits.s);
            break;
    }
    printf("tag: %d bit(s)\n", cache->bits.tag);
}

/*
 * le os dados da memoria principal e da cache a partir de um arquivo texto
 * pede ao usuario o nome do arquivo, tenta abri-lo e le os valores na ordem:
 *   tamanho_mp_kb, palavras_por_bloco, tipo_cache, tam_cache, tam_linha
 *   e, se tipo for 3, tambem lê linhas_por_conjunto
 * retorna 1 se a leitura foi bem-sucedida, ou 0 em caso de erro (arquivo
 * nao encontrado ou dados insuficientes/invalidos)
 */
int ler_de_arquivo(Mem_pri *mp, Cache *cache) {
    char nome[256];
    FILE *arq;

    printf("Nome do arquivo: ");
    scanf("%255s", nome);
/*na hora de digitar 2, digite os nome do arquivo + .exe  exe: teste1.exe <-*/
    char caminho[512];
    snprintf(caminho, sizeof(caminho), "C:\\Users\\Notebook\\Documents\\Trabalho_AOC\\%s", nome); /* esse aqui precisei de uma ajuda inteligentissima para conseguir*/
    arq = fopen(caminho, "r");                                                                    /*precisava que ele desse a lida no arquivo teste no mesmo diretorio, se nao teria que copiar o url do diretorio e colar*/
    if (arq == NULL) {
        printf("Erro: nao foi possivel abrir o arquivo '%s'.\n", caminho);
        return 0;
    }

    if (fscanf(arq, "%d %d %d %d %d",
               &mp->tamanho_kb,
               &mp->palavras_por_bloco,
               &cache->tipo,
               &cache->tam_bytes,
               &cache->tam_linha) != 5) {
        printf("Erro: arquivo com formato invalido.\n");
        fclose(arq);
        return 0;
    }

    cache->linhas_por_conjunto = 0;
    if (cache->tipo == 3) {
        if (fscanf(arq, "%d", &cache->linhas_por_conjunto) != 1) {
            printf("Erro: mapeamento tipo 3 requer linhas_por_conjunto no arquivo.\n");
            fclose(arq);
            return 0;
        }
    }

    fclose(arq);

    /* validacoes basicas dos valores lidos */
    if (mp->tamanho_kb <= 0 || !eh_potencia_de_2(mp->palavras_por_bloco)
        || cache->tipo < 1 || cache->tipo > 3
        || !eh_potencia_de_2(cache->tam_bytes)
        || !eh_potencia_de_2(cache->tam_linha)
        || cache->tam_linha >= cache->tam_bytes) {
        printf("Erro: valores invalidos no arquivo.\n");
        return 0;
    }
    if (cache->tipo == 3 && !eh_potencia_de_2(cache->linhas_por_conjunto)) {
        printf("Erro: linhas_por_conjunto deve ser potencia de 2.\n");
        return 0;
    }

    printf("Arquivo lido com sucesso.\n");
    return 1;
}

/*
 * exibe o menu principal do programa com as opcoes disponiveis
 * le e retorna a opcao escolhida pelo usuario (inteiro)
 * nao faz validacao: o controle do valor retornado e responsabilidade do main
 */
int exibir_menu() {
    int op;
    printf("Menu\n");
    printf("1. Inserir dados e calcular\n");
    printf("2. Ler dados de arquivo e calcular\n");
    printf("3. Sair\n");
    printf("Opcao: ");
    scanf("%d", &op);
    return op;
}