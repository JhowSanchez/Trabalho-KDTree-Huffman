#include <bits/stdc++.h>
#include <conio2.h>

using namespace std;

struct tree{
	int ponto[2];
	struct tree *dir, *esq;
};
typedef struct tree Tree;

struct lista
{
	int pos, ponto[2];
	struct lista *prox;
};
typedef struct lista Lista;

struct fila{
	Tree *no;
	struct fila *prox;
};
typedef struct fila Fila;

struct filaI{
	int nivel;
	struct filaI *prox;
};
typedef struct filaI FilaInt;

Tree *pop(Fila **fi)
{
	Tree *r;
	Fila *aux;
	
	aux = *fi;
	*fi = (*fi) -> prox;
	r = aux -> no;
	free(aux);
	return r;
}

void push(Fila **fi, Tree *no)
{
	if(*fi != NULL)
	{
		push(&(*fi) -> prox, no);
	}
	else
	{
		*fi = (Fila *) malloc(sizeof(Fila));
		(*fi) -> prox = NULL;
		(*fi) -> no = no;
	}
}

int popInt(FilaInt **fi)
{
	int nivel;
	FilaInt *aux;
	
	aux = *fi;
	*fi = (*fi) -> prox;
	nivel = aux -> nivel;
	free(aux);
	return nivel;
}

void pushInt(FilaInt **fi, int nivel)
{
	if(*fi != NULL)
	{
		pushInt(&(*fi) -> prox, nivel);
	}
	else
	{
		*fi = (FilaInt *) malloc(sizeof(FilaInt));
		(*fi) -> prox = NULL;
		(*fi) -> nivel = nivel;
	}
}

Tree *criaTree(int pontoA, int pontoB)
{
	Tree *nc = (Tree*)malloc(sizeof(Tree));
	nc -> ponto[0] = pontoA;
	nc -> ponto[1] = pontoB;
	nc -> dir = nc -> esq = NULL;
	
	return nc;
}

void criaLista(Lista **L,int pos,int valorA, int valorB)
{
	Lista *aux;
	Lista *nc = (Lista*)malloc(sizeof(Lista));
	
	nc -> ponto[0] = valorA;
	nc -> ponto[1] = valorB;
	nc -> pos = pos;
	nc -> prox = NULL;
	
	if(*L == NULL)
	{
		*L =nc;
	}
	else
	{
		aux = *L;
		while(aux -> prox != NULL)
		{
			aux = aux -> prox;
		}
		aux -> prox = nc;
	}
}

void gerarRandom(Lista **L)
{
	int i = 0;
	srand(time(NULL));
	while (i < 30)
	{
		criaLista(&(*L),i,rand()%100,rand()%100);
		i++;
	}
}

void ordLista(Lista *L,int ini, int fim, int pos)
{
	Lista *aux, *menor;
	int valor;

	while(L != NULL && L -> pos != ini)
	{
		L = L -> prox;
	}

	if(L != NULL)
	{
		while(L -> prox != NULL && L -> pos != fim )
		{
			aux = L -> prox;
			while(aux != NULL && aux -> pos != fim + 1)
			{
				if(L -> ponto[pos] > aux -> ponto[pos])
				{
					valor = L -> ponto[0];
					L -> ponto[0] = aux -> ponto[0];
					aux -> ponto[0] = valor;
					valor = L -> ponto[1];
					L -> ponto[1] = aux -> ponto[1];
					aux -> ponto[1] = valor;
				}
				aux = aux -> prox;
			}
			L = L -> prox;
		}	
	}
}

void gerarKDTREE(Lista **L,Tree **raiz, int inicio, int fim, int nivel)
{
	Lista *aux;
	int meio, d;
	
	if(inicio <= fim)
	{
		d = nivel % 2;
		meio = (inicio + fim) / 2;
	
		ordLista(*L,inicio,fim,d);
		
		aux = *L;
		while(aux != NULL && aux-> pos != meio)
			aux = aux -> prox;
			
		*raiz = criaTree(aux -> ponto[0],aux -> ponto[1]);
		
		gerarKDTREE(L,&(*raiz)-> esq, inicio,meio - 1,nivel+1);
		gerarKDTREE(L,&(*raiz)-> dir, meio + 1,fim,nivel+1);
	}
	
}

int tamList(Lista *L)
{
	int i=0;
	while(L  != NULL)
	{
		i++;
		L = L -> prox;
	}
	
	return i;
}

char isRaio(int PontoA[], int PontoB[], int Raio)
{
	return sqrt ( pow(PontoA[0]-PontoB[0],2) + pow(PontoA[1] - PontoB[1],2) ) <= Raio;	
}

void verifRaio(Tree *raiz, int pontoRef[],int raio)
{
	int nivel=0;
	Fila *F = NULL;
	FilaInt *FI = NULL;
	
	pushInt(&FI,nivel);
	push(&F,raiz);
	printf("\nPontos (%d , %d) Raio %d\n",pontoRef[0],pontoRef[1],raio);
	printf("Resultado Busca: \n");
	while(F != NULL)
	{
		raiz = pop(&F);
		nivel = popInt(&FI);
		
		if(isRaio(raiz -> ponto, pontoRef,raio))
		{
			printf("Pontos (%d , %d)\n",raiz -> ponto[0],raiz -> ponto[1]);
		}
		
		if(raiz -> esq != NULL && pontoRef[nivel] - raio <= raiz -> ponto[nivel] )
		{
			pushInt(&FI, (nivel+1) % 2);
			push(&F,raiz -> esq);
		}
		if(raiz -> dir != NULL && pontoRef[nivel] + raio >= raiz -> ponto[nivel])
		{
			pushInt(&FI, (nivel+1) % 2);
			push(&F,raiz -> dir);
		}
	}
}

void gerarRaios(Tree *raiz){
	
	int i = 0,vet[2];
	srand(time(NULL));
	while(i < 10)
	{
		vet[0]=rand()%50;
		vet[1]=rand()%50;
		verifRaio(raiz,vet,rand()%10);
		
		i++;
	}
	
}

void exibirArvoreAux(Tree *arv, int n)
{
	if(arv != NULL)
	{
		exibirArvoreAux(arv -> dir, n+1);
		for(int i=0;i<n;i++)
			printf("\t");
	
		printf("(%d , %d)",arv-> ponto[0], arv -> ponto[1]);
		printf("\n");
		exibirArvoreAux(arv -> esq, n+1);
	}
}

void exibirArvore(Tree *arv)
{
	printf("\n\n\t\t\t#### Exibir KD Tree ####\n\n");
	exibirArvoreAux(arv, 0);
}

int main (void)
{
	Lista *List = NULL;
	Tree *raiz = NULL;
	
	gerarRandom(&List); 
	gerarKDTREE(&List,&raiz,0,tamList(List)-1,0);
	exibirArvore(raiz);
	printf("\n");
	gerarRaios(raiz);
	
	return 0;
}
