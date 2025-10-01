#include <stdio.h>
#include <stdlib.h>

// variaveris globais e a matriz
int profundida_total_container, linhas_matriz, colunas_matriz;
int **matriz;

// prototipos das funcoes
void importe_matriz(char *nome_arquivo);
void combinacao_container(int alvo, int *vetor, int tamanho);
int importe_parametros(int argc, char *argv[]);
int busca_em_profundidade(int **matriz, int linhas, int colunas, int i, int j);
void encontrar_ilhas(int **matriz, int linhas, int colunas); 

//execucao do programa
int main(int argc, char *argv[]) {
    importe_parametros(argc, argv);
    
    encontrar_ilhas(matriz, linhas_matriz, colunas_matriz);

    for (int i = 0; i < linhas_matriz; i++) {
        free(matriz[i]);
    }
    free(matriz);
 return 0;
}

//funcao que importa os parametros
int importe_parametros(int argc, char *argv[]) {
    profundida_total_container = atoi(argv[1]);
    linhas_matriz = atoi(argv[2]);
    colunas_matriz = atoi(argv[3]);
    importe_matriz(argv[4]);
 return 0;
}

//funcao que importa a matriz do arquivo
void importe_matriz(char *nome_arquivo) {
    FILE *arquivo = fopen(nome_arquivo, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        exit(1);
    }
    // aloca memoria para a matriz
    matriz = (int **)malloc(linhas_matriz * sizeof(int *));
    for (int i = 0; i < linhas_matriz; i++) {
        matriz[i] = (int *)malloc(colunas_matriz * sizeof(int));
    }
    // le a matriz do arquivo e armazena na variavel matriz
    for (int i = 0; i < linhas_matriz; i++) {
        for (int j = 0; j < colunas_matriz; j++) {
            fscanf(arquivo, "%d", &matriz[i][j]);
        }
    }
    fclose(arquivo);
}

// funcao de busca em profundidade para encontrar ilhas
int busca_em_profundidade(int **matriz, int linhas, int colunas, int i, int j){
    if (i < 0 || i >= linhas || j < 0 || j >= colunas || matriz[i][j] == 0) {
        return 0;
    }
    int soma = matriz[i][j];
    matriz[i][j] = 0; // marca elemento como visitado
    // explora os quatro vizinhos
    soma += busca_em_profundidade(matriz, linhas, colunas, i+1, j);
    soma += busca_em_profundidade(matriz, linhas, colunas, i-1, j);
    soma += busca_em_profundidade(matriz, linhas, colunas, i, j+1);
    soma += busca_em_profundidade(matriz, linhas, colunas, i, j-1);
    return soma;
}

// funcao que encontra as ilhas e calcula suas profundidades
void encontrar_ilhas(int **matriz, int linhas, int colunas) {
    int *soma = (int *)malloc(linhas * colunas * sizeof(int));
    int *profundidades = (int *)malloc(linhas * colunas * sizeof(int));
    int num_ilhas = 0;
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            if (matriz[i][j] != 0) {
                soma[num_ilhas] = busca_em_profundidade(matriz, linhas, colunas, i, j);
                num_ilhas++;
            }
        }
    }
    printf("%d\n", num_ilhas);

    // calcula e armazena as profundidades
    for (int k = 0; k < num_ilhas; k++) {
        double r = (double)soma[k] / 6;
        if (r > 0 && r < 1) {
            profundidades[k] = 1;
        } else {
            profundidades[k] = (int)r;
        }
    }

    // imprime o vetor de profundidades
    for (int k = 0; k < num_ilhas; k++) {
        printf("%d ", profundidades[k]);
    }
    printf("\n");

    combinacao_container(profundida_total_container, profundidades, num_ilhas);

    free(soma);
    free(profundidades);
}

// funcao que busca a combinacao de profundidades que somam o valor alvo
void busca(int idx, int soma, int posicao, int profundidade_container, int *vetor, int tamanho, int *combinacao, int *encontrado) {
    if (*encontrado) return;
    if (soma == profundidade_container) {
        for (int i = 0; i < posicao; i++) {
            printf("%d ", combinacao[i]);
        }
        *encontrado = 1;
        return;
    }
    if (soma > profundidade_container || idx == tamanho) return;

    // inclui o elemento atual
    combinacao[posicao] = vetor[idx];
    busca(idx + 1, soma + vetor[idx], posicao + 1, profundidade_container, vetor, tamanho, combinacao, encontrado);

    // não inclui o elemento atual
    busca(idx + 1, soma, posicao, profundidade_container, vetor, tamanho, combinacao, encontrado);
}

// funcao container que inicializa a busca pela combinacao
void combinacao_container(int alvo, int *vetor, int tamanho) {
    int *combinacao = (int *)malloc(tamanho * sizeof(int));
    int encontrado = 0;

    busca(0, 0, 0, alvo, vetor, tamanho, combinacao, &encontrado);

    if (!encontrado) {
        printf("Nao ha resposta valida!");
    }
    free(combinacao);
}
