#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TMAX 40
FILE *abreL(char *nome);
void fecha(FILE *arquivo);
typedef struct galho
{
    char palavra[TMAX];
    int freq;
    struct galho *esquerda;
    struct galho *direita;
} ponta;
/*Recebe um endereço de no, uma string e um ponteiro para inteiro, retorna a arvore preenchida com palavras*/
ponta *inserePArv(ponta *no, char pal[], int *maior);
void lista(ponta *no);
typedef char item;
typedef struct cel
{
    ponta *flecha;
    struct cel *prox;
} celula;
/*Recebe o endereço para o inicio da lista e um no da arvore, inserindo no fim essa palavra*/
celula *insereNoFimRec(celula *inicio, ponta *atual);
/*Recebe o endereço para o inicio da arvore e a lista, insere as palavras no vetor de listas em ordem, segundo a frequencia*/
void inordemFS(ponta *raiz, celula **listaPalavras);
int buscaArv(ponta *no, char pal[]);
void imprimeListaRec(celula *inicio);
int altura(ponta *raiz);
int contaNos(ponta *raiz);
int main(int argc, char *argv[])
{
    int numero, alturaA, quantNos;
    ponta *raiz, *inserePArv();
    char palavra[TMAX], letra;
    int maior;
    int i, quant, textando;
    char nomeArq[80];
    celula **listaPalavras;
    FILE *arquivo;
    textando = 1;
    raiz = NULL;
    quant = 0;

    printf("Qual o nome do arquivo? ");
    scanf("%s", nomeArq);
    arquivo = abreL(nomeArq);
    /*Coisas da FILA*/

    maior = 1;
    if (arquivo && textando == 1)
    {
        printf("Processando o arquivo ... \n");
        while (!feof(arquivo) && !ferror(arquivo))
        {
            i = 0;
            while (1)
            {
                letra = fgetc(arquivo);
                if (letra == EOF)
                    break;
                if (letra == ' ' || letra == '\n' || letra == '\t' || letra == '\r' || (letra >= 58 && letra <= 64) || (letra >= 91 && letra <= 93) || (letra >= 123 && letra <= 125) || (letra >= 32 && letra <= 47) || letra == 95)
                    break;
                if (letra == 94 || letra == 96 || letra == 126)
                    continue;
                else
                {
                    palavra[i] = letra;
                    i++;
                }
            }
            if (i > 0)
            {
                palavra[i++] = '\0';
                raiz = inserePArv(raiz, palavra, &maior);
                quant++;
            }
        }
        fecha(arquivo);
        textando = 0;
    }
    else
    {
        printf("\nNao abriu para leitura o arquivo!\n");
        printf("\nFim do programa!\n");
        exit(1);
    }
    listaPalavras = malloc(maior * sizeof(celula));
    for (i = 0; i < maior; i++)
    {
        listaPalavras[i] = NULL;
    }

    inordemFS(raiz, listaPalavras);
    alturaA = altura(raiz);
    quantNos = contaNos(raiz);
    printf("A maior frequencia observada foi de: %d\n", maior);
    printf("A quantidade de palavras lidas foi de %d\n", quant);
    printf("A altura da arvore eh de: %d \n", alturaA);
    printf("A quantidade de nos eh de: %d \n", quantNos);
    do
    {
        printf("\nForneça o valor de frequencia que desejas:\n");
        scanf("%d", &numero);
        if (numero != 0)
        {
            if (numero <= maior)
            {
                if (listaPalavras[numero - 1] != NULL)
                    imprimeListaRec(listaPalavras[numero - 1]);
                else
                    printf("Nao existe nenhuma palavra com essa numero de ocorrencias no texto!\n");
            }
            else
                printf("Nao existe nenhuma palavra com essa numero de ocorrencias no texto!\n");
        }
    } while (numero != 0);

    return 0;
}
int altura(ponta *raiz)
{
    int he, hd;
    if (raiz == NULL)
        return -1;
    he = altura(raiz->esquerda);
    hd = altura(raiz->direita);
    if (he > hd)
        return he + 1;
    return hd + 1;
}
int contaNos(ponta *raiz)
{
    if (raiz == NULL)
        return 0;
    return 1 + contaNos(raiz->esquerda) + contaNos(raiz->direita);
}
FILE *abreL(char *nome)
{
    return fopen(nome, "r");
}
void fecha(FILE *arquivo)
{
    fclose(arquivo);
}
ponta *inserePArv(ponta *no, char pal[], int *maior)
{
    /*Implementação do maior, observar sempre a maior frequencia*/
    int i;
    char first;
    if (no == NULL)
    {
        no = (ponta *)malloc(sizeof(*no));
        if (no == NULL)
        {
            printf("Sem memoria\n");
            exit(0);
        };
        sprintf(no->palavra, pal);
        no->freq = 1;
        no->direita = NULL;
        no->esquerda = NULL;
    }
    else
    {
        first = pal[0];
        if ((first >= 65) && (first <= 90))
            pal[0] = (first + 32);
        first = no->palavra[0];
        if ((first >= 65) && (first <= 90))
            no->palavra[0] = (first + 32);
        i = strcmp(no->palavra, pal);
        if (i == 0)
        {
            no->freq++;
            if (no->freq > *maior)
                *maior = no->freq;
        }
        else
        {
            if (i > 0)
                no->esquerda = inserePArv(no->esquerda, pal, maior);
            else
                no->direita = inserePArv(no->direita, pal, maior);
        }
    }
    return (no);
}
void lista(ponta *no)
{
    if (no != NULL)
    {
        lista(no->esquerda);
        printf("%s -> %d ocorrencias\n", no->palavra, no->freq);
        lista(no->direita);
    }
}
/*Lista inserindo o ponteiro para o endereço fornecido da árvore*/
celula *insereNoFimRec(celula *inicio, ponta *atual)
{
    celula *novo;
    if (inicio == NULL)
    {
        novo = malloc(sizeof(celula));
        novo->flecha = atual;
        novo->prox = NULL;
        return novo;
    }
    inicio->prox = insereNoFimRec(inicio->prox, atual);
    return inicio;
}
void inordemFS(ponta *raiz, celula **listaPalavras)
{
    if (raiz != NULL)
    {
        /*Aqui não tenho certeza se funciona*/
        inordemFS(raiz->esquerda, listaPalavras);
        listaPalavras[raiz->freq - 1] = insereNoFimRec(listaPalavras[raiz->freq - 1], raiz);
        inordemFS(raiz->direita, listaPalavras);
    }
}
int buscaArv(ponta *no, char pal[])
{
    int i, retorna = 0;
    if (no != NULL)
    {
        if ((i = strcmp(no->palavra, pal)) == 0)
            retorna = no->freq;
        else if (i > 0)
            retorna = buscaArv(no->esquerda, pal);
        else
            retorna = buscaArv(no->direita, pal);
    }
    return (retorna);
}
void imprimeListaRec(celula *inicio)
{
    if (inicio != NULL)
    {
        printf("%s\n", inicio->flecha->palavra);
        imprimeListaRec(inicio->prox);
    }
    else
        printf("\n");
}
