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

struct tabelaArq{
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

struct pilha{
	Tree *nodo;
    struct pilha *prox;
};
typedef struct pilha Pilha;

struct pilhai{
	int topo;
	struct pilhai *prox;
};
typedef struct pilhai PilhaInt;


void push(Pilha **p, Tree *nodo) {
    Pilha *novo = (Pilha*) malloc(sizeof(Pilha));
    novo->nodo = nodo;
    novo->prox = *p;
    *p = novo;
}


Tree* pop(Pilha **p) {
    
    Pilha *temp = *p;
    Tree *nodo = temp->nodo;
    *p = (*p)->prox;
    free(temp);
    return nodo;
}

int isEmpty(Pilha *p) 
{
    return p == NULL;
}

void pushInt(PilhaInt **p,int n){
	PilhaInt *novo = (PilhaInt*)malloc(sizeof(PilhaInt));
	novo->topo = n;
	novo->prox = *p;
	*p = novo;
}

int popInt(PilhaInt **p){
	PilhaInt *aux = *p;
	*p = (*p)->prox;
	int x = aux->topo;
	free(aux);
	return x;
}

int isEmptyInt(PilhaInt *p){
	return p == NULL;
}

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

Tabela *criaTab(char str[], int simbolo)
{
	Tabela *nc = (Tabela*)malloc(sizeof(Tabela));
	
	strcpy(nc -> palavra,str);
	nc -> freq = 1;
	nc -> simbolo = simbolo;
	strcpy(nc->codigo,"");
	nc -> prox = NULL;
	
	return nc;
}

int buscaTabela(FILE *ptr,char str[])
{
	Tabela reg;
	rewind(ptr);
	fread(&reg,sizeof(reg),1,ptr);
	while(!feof(ptr) && strcmp(reg.palavra,str) != 0)
	{
		fread(&reg,sizeof(reg),1,ptr);
	}
	
	if(!feof(ptr))
		return ftell(ptr)-sizeof(reg);
	return -1;
}


void gerarTabela(Tabela **tab)
{
	Tabela *aux, *nc;
	int i=0,j=0,busc, simbolo = 0;
	char str2[20];
	char str[]="amar e sonhar sonhar e viver viver e curtir curtir e amar Cada um tera uma escolha cada um fara a escolha cada um escolhe a sua escolha Levaremos um tempo para crescer levaremos um tempo para amadurecer levaremos um tempo para entender levaremos um tempo para envelhecer levaremos um tempo para morrer";
	while(str[i] != '\0')
	{
		 j=0;
        if(str[i]==' '){
            str2[j++]=' ';
            i++;
        }
        else
            while(str[i] != ' ' && str[i]!= '\0')
                str2[j++]= str[i++];
                
		str2[j]='\0';
		
		
		if(*tab == NULL)
		{
			*tab = criaTab(str2,simbolo++);
		}
		else
		{
			aux = *tab;
			while(aux -> prox != NULL && stricmp(str2,aux -> palavra) != 0)
				aux = aux -> prox;
			
			if(stricmp(str2,aux -> palavra) == 0)
			{
				aux -> freq++;
			}
			else
			{
				aux -> prox = criaTab(str2, simbolo++);
			}
		}	
	}
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


//void inserirCodigo(Tree *raiz,Tabela **tab, char cod[9],int i)
//{
//	tabela *aux = *tab;
//	if(raiz != NULL)
//	{
//		if(raiz -> simbolo != -1)
//		{
//			cod[i]='\0';
//			while(aux !=NULL && aux -> simbolo != raiz->simbolo)
//            	aux=aux->prox;
//            	
//            strcpy(aux->codigo,cod);
//		}
//		cod[i]='0';
//		inserirCodigo(raiz -> esq, tab, cod,i+1);
//		cod[i]='1';
//		inserirCodigo(raiz -> dir, tab, cod,i+1);
//	}
//}
void inserirCodigo(Tree *raiz,Tabela **tab)
{
	char cod[50];
	int j=0, n = 0;
	Tabela *auxT = *tab;
	Tree *aux = raiz;
	Pilha *p = NULL;
	PilhaInt *p1 = NULL;
	
	push(&p,raiz);
	pushInt(&p1,n);
	
	while(!isEmpty(p))
	{
		if(raiz != NULL)
		{
			raiz = pop(&p);
			n = popInt(&p1)	;
			cod[n] = '\0';
			while(raiz != NULL)
			{
				push(&p,raiz);
				pushInt(&p1,n);
				if(raiz->simbolo != -1){
					auxT = *tab;
					while(auxT!=NULL && auxT->simbolo != raiz->simbolo)
						auxT = auxT->prox;
					strcpy(auxT->codigo,cod);
				}
				n++;
				cod[n-1] = '0';
				cod[n] = '\0';
				raiz = raiz -> esq;
			}
		}
		
		raiz = pop(&p);
		n = popInt(&p1);
		n++;
		cod[n-1] = '1';
		cod[n] = '\0';
		raiz = raiz -> dir;
		
		if(raiz != NULL)
		{
			push(&p,raiz);
			pushInt(&p1,n);
		}
		
	}
}

void gerarArquivoTab(Tabela *tab){
	FILE *ptr = fopen("arqTabela.dat","wb+");
	Tabela *aux = tab;
	tabelaArq reg;
	
	while(aux != NULL)
	{
		strcpy(reg.codigo, aux -> codigo);
		strcpy(reg.palavra, aux -> palavra);
		reg.freq = aux -> freq;
		reg.simbolo = aux -> simbolo;
		fwrite(&reg,sizeof(reg),1,ptr);
		
		aux = aux -> prox;
	}		
	fclose(ptr);
}

void inserirBits(Tabela *tab, char str[])
{
	FILE *ptr = fopen("arqBits.dat","wb+");
	char cod[10000],str2[100];
	bits bit;
	int i=0,j,x=0;
	Tabela *aux;
	strcpy(cod,"");
	
	while(i<strlen(str)){
	
		aux =tab;
		j=0;
		while(i<strlen(str) && str[i]!= ' ')
			str2[j++]= str[i++];
		str2[j]='\0';
		
		while(aux != NULL && stricmp(aux->palavra,str2)!= 0)
			aux = aux -> prox;
		
		strcat(cod,aux-> codigo);
		if(str[i] == ' ')
		{
			x=strlen(cod);
			cod[x]= '0';
			cod[x+1]= '\0';
			i++;
		}
	}
	
	i=0;
	while(i<strlen(cod))
	{
		j=0;
		while(i<strlen(cod) && j < 8)
		{
			str2[j++]=cod[i++];
		}
	
		while(j < 8)
		{
			str2[j++]='0';
		}
		bit.b1 = str2[0]-'0';
		bit.b2 = str2[1]-'0';
		bit.b3 = str2[2]-'0';
		bit.b4 = str2[3]-'0';
		bit.b5 = str2[4]-'0';
		bit.b6 = str2[5]-'0';
		bit.b7 = str2[6]-'0';
		bit.b8 = str2[7]-'0';
	
		fwrite(&bit,sizeof(bit),1,ptr);
		
		bit.b1 = 0;
		bit.b2 = 0;
		bit.b3 = 0;
		bit.b4 = 0;
		bit.b5 = 0;
		bit.b6 = 0;
		bit.b7 = 0;
		bit.b8 = 0;
	}

 
	fclose(ptr);
}

void exibir(Tabela *tab){
	
	printf("Nome \t\t\t\tFreq	Simbolo\n");
	while(tab!=NULL)
	{
		printf("%s \t\t\t\t%d	%d \n",tab-> palavra, tab -> freq, tab -> simbolo);
		tab = tab -> prox;
	}
}

void exibeh(Tree* N)
{
    static int n= -1;
    if(N==NULL) return;
    n++;
    exibeh(N->dir);
    for(int i=0;i<5*n;i++) printf(" ");
    printf("(%d,%d)\n",N->simbolo,N->freq);
    exibeh(N->esq);
    n--;
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

int main(void)
{
	Tabela *tab = NULL;
	No *raiz = NULL;
	Tree *arvHuf = NULL;
	char cod[9], str[1000];
	
	strcpy(str,"amar e sonhar sonhar e viver escolha cada um escolhe um tempo para amadurecer levaremos");
	
	gerarTabela(&tab);
	criarListaHuff(&raiz,tab);
	insererHuffman(raiz,&arvHuf);
	inserirCodigo(arvHuf,&tab);
	inserirBits(tab,str);
	gerarArquivoTab(tab);
	print_tabela(tab);
	exibirArvoreAux(arvHuf,1);
	
	return 0;
}
