#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio2.h>

struct bits{
	unsigned char b8:1;
	unsigned char b7:1;
	unsigned char b6:1;
	unsigned char b5:1;
	unsigned char b4:1;
	unsigned char b3:1;
	unsigned char b2:1;
	unsigned char b1:1;
};

struct tree{
	int simbolo, freq;
	struct tree *dir, *esq;
};
typedef struct tree Tree;

struct tabela{
	char palavra[100];
	int freq;
	int simbolo;
	char codigo[9];
	struct tabela *prox;
};
typedef struct tabela Tabela;

struct TabelaArq{
	char palavra[100];
	int freq;
	int simbolo;
	char codigo[9];
};

struct no{
	Tree *no;
	struct no *prox;
};
typedef struct no No;

Tree *criaTree(int freq, int simbolo)
{
	Tree *nc = (Tree*)malloc(sizeof(Tree));
	nc -> dir = nc -> esq = NULL;
	nc -> freq = freq;
	nc -> simbolo = simbolo;
	
	return nc;
}

No *criaNo(Tree *tree)
{
	No *nc = (No*)malloc(sizeof(No));
	nc -> prox = NULL;
	nc -> no = tree;
	
	return nc;
}

Tabela *criaTab(TabelaArq reg)
{
	Tabela *nc = (Tabela*)malloc(sizeof(Tabela));
	
	strcpy(nc -> palavra,reg.palavra);
	nc -> freq = reg.freq;
	nc -> simbolo = reg.simbolo;
	strcpy(nc->codigo,reg.codigo);
	nc -> prox = NULL;
	
	return nc;
}

void inserirOrdenadoListaHuf(No **lista, Tree *arv)
{
	No *aux, *ant, *nc;
	
	if(*lista == NULL)
		*lista = criaNo(arv);
	else
	{
		 
		ant = aux = *lista;
		while(aux != NULL && (aux -> no -> freq) < arv -> freq)
		{
			ant = aux;
			aux = aux -> prox;
		}
		
		if(ant == aux)
		{
			nc = criaNo(arv);
			nc -> prox = aux;
			*lista = nc;
		
		}
		else 
		{
			nc = criaNo(arv);
			nc -> prox = aux;
			ant -> prox = nc;
			
		}	
	}
}

void criarListaHuff(No **raiz,Tabela *tab)
{
	Tree *nc;
	while(tab != NULL)
	{
		nc = criaTree(tab -> freq, tab -> simbolo);
		inserirOrdenadoListaHuf(&(*raiz),nc);
		tab = tab -> prox;
	}
} 

void insererHuffman(No *lista, Tree **Raiz)
{
	No *prim, *segun, *nc;
	Tree *arvHuf;
	while(lista -> prox != NULL)
	{
		prim = lista;
		segun = lista -> prox;
		lista = lista -> prox -> prox;
		
		arvHuf = criaTree(prim -> no -> freq + segun -> no -> freq,-1);
		arvHuf -> esq = prim -> no;
		arvHuf -> dir = segun -> no;
		
		free(prim);
		free(segun);
		
		inserirOrdenadoListaHuf(&lista,arvHuf);
	}
	
	*Raiz = lista -> no;
	free(lista);
}

void carregarTabela(Tabela **tab)
{
	FILE *ptr = fopen("arqTabela.dat","rb");
	Tabela *aux,*nc;
	TabelaArq reg;
	
	fread(&reg,sizeof(reg),1,ptr);
	while(!feof(ptr))
	{
		nc = criaTab(reg);
		if(*tab == NULL)
		{
			*tab = nc;
		}
		else
		{
			aux = *tab;
			while(aux -> prox != NULL)
				aux = aux -> prox;
			aux -> prox = nc;
		}
		
		fread(&reg,sizeof(reg),1,ptr);
	}
	
	fclose(ptr);
}

void carregarBits(char str[])
{
	FILE *ptr = fopen("arqBits.dat","rb");
	bits b;
	int i=0;
	
	fread(&b,sizeof(b),1,ptr);
	while(!feof(ptr))
	{
		str[i++]='0'+b.b1;
		str[i++]='0'+b.b2;
		str[i++]='0'+b.b3;
		str[i++]='0'+b.b4;
		str[i++]='0'+b.b5;
		str[i++]='0'+b.b6;
		str[i++]='0'+b.b7;
		str[i++]='0'+b.b8;
		fread(&b,sizeof(b),1,ptr);
	}
	str[i]='\0';
	fclose(ptr);
}

void varrerArvore(Tree *arv,char str[],Tabela *tab)
{
	Tree *raiz = arv;
	Tabela *auxT;
	int i = 0;
	printf("\n");
	while(i < strlen(str))
	{
		auxT = tab;
		while(i<strlen(str) && arv -> simbolo == -1)
		{
			if(str[i] == '0')
				arv = arv -> esq;
			else
				arv = arv -> dir;
			i++;
		}
		
		while(auxT != NULL && auxT -> simbolo != arv -> simbolo)
			auxT = auxT -> prox;
			
		printf("%s",auxT -> palavra);
		
		arv = raiz;
	}
	
}

void exibirArvoreAux(Tree *arv, int nivel)
{
	if (arv != NULL)
	{
		exibirArvoreAux(arv->dir, nivel + 1);
		printf("\n");
		for (int i = 0; i < nivel; i++)
		{
			printf("\t");
		}
		printf("(%d,%d)", arv->freq, arv->simbolo);
		exibirArvoreAux(arv->esq, nivel + 1);
	}
}

void print_tabela(Tabela* T)
{
    int i=2;
    gotoxy(1,1);printf("Simbolo");
    gotoxy(13,1);printf("Palavra");
    gotoxy(25,1);printf("Freq");
    gotoxy(37,1);printf("Codigo");

    while(T!=NULL)
    {
        gotoxy(1,i);printf("%d",T->simbolo);
        gotoxy(13,i);printf("%s",T->palavra);
        gotoxy(25,i);printf("%d",T->freq);
        gotoxy(37,i);printf("%s",T->codigo);
        i++;
        T=T->prox;
    }
}

int main (void)
{
	Tree *arvHuf = NULL;
	No *raiz = NULL;
	Tabela *tab = NULL;	
	char str[10000];
	
	carregarTabela(&tab);
	carregarBits(str);

	criarListaHuff(&raiz,tab);
	insererHuffman(raiz,&arvHuf);
	print_tabela(tab);
	exibirArvoreAux(arvHuf,1);
	printf("\n%s\n",str);
	varrerArvore(arvHuf,str,tab);
	
}



