#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

/*
 *
 * Como eu entendi o processo:
 *
 * - João: @ByllMe start recording
 * - ByllMe: Recording started
 * - João: @ByllMe total 200,00 people 'all'
 * - ByllMe: Saved
 * - João: @ByllMe I paid 100,00
 * - ByllMe: Saved
 * - Carlos: @ByllMe I paid 20,00
 * - ByllMe: Saved
 * - João: @ByllMe Gonçalo Pereira paid 80,00
 * - ByllMe: Saved
 * - João: @ByllMe end recording
 * - ByllMe: Recording ended
 * - João: @ByllMe tell balance - - - - - - - - - - - (or something like that)
 * - ByllMe: João Cacho Sá - recieve 50,00 from Daniel Silva
 * 			 Carlos Novo - pay 30,00 to Gonçalo Pereira
 * 			 Gonçalo Pereira - recieve 30,00 from Carlos Novo
 * 			 Daniel Silva - pay 50,00 to João Cacho Sá
 * - ByllMe: Bye people
 *
 */

typedef struct{

	char **nomeapagar;
    char *nome;
    int bal;
    int pago;
    int dinheirocerto; // Boolean

} velemento;

typedef struct
{
	/** numero de elementos do vetor */
	int tamanho;

    /** capacidade do vetor */
    int capacidade;

    /** array de elementos armazenados */
	velemento* elementos;

} vetor;



vetor* vetor_novo()
{
	vetor* vec;

	/* aloca memoria para a estrutura vetor */
	vec = (vetor*)malloc(sizeof(vetor));
	if(vec == NULL)
		return NULL;

	vec->tamanho = 0;
	vec->capacidade = 0;
	vec->elementos = NULL;

	return vec;
}

int vetor_insere(vetor* vec, const char* nomeprov, int pagamento){

	if(vec == NULL)
		return -1;

	/* aumenta elementos do vetor se capacidade nao for suficiente */
	if(vec->tamanho == vec->capacidade)
	{
		if(vec->capacidade == 0)
			vec->capacidade = 1;
		else
			vec->capacidade *= 2;

		vec->elementos = (velemento*)realloc(vec->elementos, vec->capacidade * sizeof(velemento));
		if(vec->elementos == NULL)
			return -1;
	}

	/* aloca espaco para a nova string na posicao pos */
	vec->elementos[vec->tamanho].nome = (char*)calloc(strlen(nomeprov)+1, sizeof(char));
	if(vec->elementos[vec->tamanho].nome == NULL) return -1;

	/* copia valor */
	strcpy(vec->elementos[vec->tamanho].nome, nomeprov);
	vec->elementos[vec->tamanho].pago = pagamento;

	vec->tamanho++;

	return vec->tamanho;
}

int vetor_ordena(vetor* vec)
{
	int i, j;
	int tmp;

	if(vec == NULL)
		return -1;

	/* ordenacao por insercao */
    for (i = 1; i < vec->tamanho; i++)
    {
		tmp = vec->elementos[i].pago;
		for (j = i; j > 0 && tmp < vec->elementos[j-1].pago; j--) {
			vec->elementos[j] = vec->elementos[j-1];
		}
		vec->elementos[j].pago = tmp;
    }

	return 0;
}

void vetor_apaga(vetor* vec)
{
	int i;

	if(vec == NULL)
		return;

	/* liberta memoria de cada string */
	for (i = 0; i < vec->tamanho; i++)	{
		free(vec->elementos[i].nome);
		free(vec->elementos[i].nomeapagar);
	}

	/* liberta memoria dos apontares para as strings */
	free(vec->elementos);

	/* liberta memoria da estrutura vetor */
	free(vec);
}

int main( ) {

	vetor *vet;
	char name [50];
	int num, pago, pess;
	int i, j, media;

	vet = vetor_novo();

	printf ("Total Payment - Number of People\n");
	scanf ("%d %d", &pago, &pess);

	printf("\n");

	for (i = 0; i < pess; i++) {
		printf("Name and Expense\n");
		scanf("%s %d", name, &num);
		vetor_insere (vet, name, num);
	}


	for (i = 0; i < pess; i++) media += vet->elementos[i].pago;
	media = media / pess;

	for (i = 0; i < pess; i++) {
		if (vet->elementos[i].pago == media) vet->elementos[i].dinheirocerto = 1;
		else vet->elementos[i].bal = media - vet->elementos[i].pago;
	}

	for (i = 0; i < pess; i++) {
		if (vet->elementos[i].bal < 0 && vet->elementos[i].dinheirocerto != 1 && vet->elementos[i+1].bal < 0) {
			for (j = i+1; j < pess; j++) {
				if (abs(vet->elementos[i].bal) < vet->elementos[j].bal && vet->elementos[i].bal < 0 && vet->elementos[j].bal > 0) {
					vet->elementos[j].bal += vet->elementos[i].bal;
					printf("%s paga %d a %s", vet->elementos[i].nome, abs(vet->elementos[i].bal), vet->elementos[j].nome);
				}
			}
		}
		else if (vet->elementos[i].bal < 0 && vet->elementos[i].dinheirocerto != 1 && vet->elementos[i+1].bal > 0) {
			for (j = i+1; j < pess; j++) {
				if (vet->elementos[i].bal < 0 && vet->elementos[j].bal > 0) {
					vet->elementos[j].bal += vet->elementos[i].bal;
				}
			}
		}
	}

	vetor_apaga(vet);

	return 0;
}