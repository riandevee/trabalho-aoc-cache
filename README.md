# 💾 Simulador de Mapeamento de Memória Cache

Projeto desenvolvido para a disciplina de **Organização e Arquitetura de Computadores**, com o objetivo de simular os cálculos de particionamento de endereços para diferentes modos de mapeamento de memória cache.

## 💻 Sobre o Projeto

O sistema, construído em **Linguagem C**, permite calcular a quantidade de bits destinados a cada campo do endereço da memória principal (Tag, Linha, Conjunto, Palavra/Offset) com base nas configurações da Memória Principal e da Cache.

O simulador suporta os três principais tipos de mapeamento:
1. **Mapeamento Direto**
2. **Mapeamento Totalmente Associativo**
3. **Mapeamento Associativo por Conjunto**

## 📂 Estrutura de Arquivos

A arquitetura do código foi modularizada para melhor organização:

* **`meuprog.c`**: Arquivo principal (`main`). Gerencia o menu interativo e o fluxo de execução.
* **`minhalib.h`**: Cabeçalho contendo a definição das estruturas de dados (`Mem_pri`, `Cache`, `Campos_bits`) e os protótipos das funções.
* **`minhalib.c`**: Implementação da lógica do simulador, validações (como checagem de potências de 2), cálculos matemáticos de bits e exibição dos resultados.
* **`teste1.txt`, `teste2.txt`, `teste3.txt`**: Arquivos de texto contendo cenários de entrada pré-definidos para testes automatizados rápidos de cada tipo de mapeamento.

## ⚙️ Como Compilar e Executar

Certifique-se de ter o compilador **GCC** instalado no seu ambiente. 

Para compilar o projeto, abra o terminal na pasta dos arquivos e digite:
```bash
gcc meuprog.c minhalib.c -o simulador_cache -lm
