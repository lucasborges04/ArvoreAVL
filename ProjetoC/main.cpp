#include <iostream>
#include <stdio.h>
#include <stdlib.h>

typedef struct no{
	int valor;
	struct no *esq;
	struct no *dir;
	int altura;
}No;

/* Fun��o que cria um novo n� x -> valor a ser inserido no n�
retorna: o endere�o do n� criado*/

No* novoNo (int x){
	No *novo = (No*) malloc(sizeof(No));
	
	if (novo){
		novo->valor = x;
		novo->esq = NULL;
		novo->dir = NULL;
		novo->altura = 0;
	}
	else
	    printf("\n Erro ao alocar n� em novoNo!\n");
	return novo;
}

// Descobrir a altura das sub-�rvores
/* Retorna o maior dentre dois valores a, b -> altura de dois n�s da �rvore */
short maior(short a, short b){
	return (a > b)? a : b;
};

// Fun��o para descobrir a altura de um n�
// Retorna a altura de um n� ou -1 caso ele seja null
short alturaDoNo(No *no){
	if(no == NULL)
	    return -1;
	else
	    return no->altura;
};

// Calcula e retorna o fator de balanceamento de um n�
short fatorDeBalanceamento (No *no){
	if (no)
	    return (alturaDoNo(no->esq) - alturaDoNo(no->dir));
	else
	    return 0;
};

// Fun��o para Rota��o Esquerda
No* rotacaoEsq(No *raiz){
	No *y, *filho;
	
	y = raiz->dir;
	filho = y->esq;
	
	y->esq = raiz;
	raiz->dir = filho;
	
	raiz->altura = maior(alturaDoNo(raiz->esq), alturaDoNo(raiz->dir)) +1;
	y->altura = maior(alturaDoNo(y->esq), alturaDoNo(y->dir)) +1;
	
	return y;
};

// Fun��o para Rota��o Direita
No* rotacaoDir(No *raiz){
	No *y, *filho;
	
	y = raiz->esq;
	filho = y->dir;
	
	y->dir = raiz;
	raiz->esq = filho;
	
	raiz->altura = maior(alturaDoNo(raiz->esq), alturaDoNo(raiz->dir)) +1;
	y->altura = maior(alturaDoNo(y->esq), alturaDoNo(y->dir)) +1;
	
	return y;
};

// Rota��o Direita Esquerda ou Dupla � Direita
No* rotacaoDirEsq(No *raiz){
	raiz->dir = rotacaoDir(raiz->dir); //'(raiz->dir)' quer dizer que a rota��o direita � na sub �rvore direita apenas. A raiz fica parada.
	return rotacaoEsq(raiz); //(Ele chama a fun��o 'rotacaoEsq', criada anteriormente)'rotacaoEsq(raiz)' Essa rota��o ser� na �rvore completa. A raiz ir� rotacionar tamb�m (nesse caso, para a esquerda)
};

// Rota��o Esquerda Direita ou Dupla � Esquerda
No* rotacaoEsqDir(No *raiz){
	raiz->esq = rotacaoEsq(raiz->esq); //'(raiz->esq)' quer dizer que a rota��o esquerda � na sub �rvore esquerda apenas. A raiz fica parada.
	return rotacaoDir(raiz); //(Ele chama a fun��o 'rotacaoDir', criada anteriormente) 'rotacaoDir(raiz)' Essa rota��o ser� na �rvore completa. A raiz ir� rotacionar tamb�m (nesse caso, para a direita)
};

/*
	Fun��o para realizar o balanceamento da �rvore ap�s uma inser��o ou remo��o
	Recebe o n� que est� desbalanceado e retorna a nova raiz ap�s o balanceamento
*/
No* balancear(No *raiz){
	short fb = fatorDeBalanceamento(raiz);
	
	// Rota��o � esquerda
	if(fb < -1 && fatorDeBalanceamento(raiz->dir) <= 0)
		raiz = rotacaoEsq(raiz);
	
	// Rota��o � direita
	else if(fb > 1 && fatorDeBalanceamento(raiz->esq) >= 0)
		raiz = rotacaoDir(raiz);
	
	// Rota��o Esquerda Direita
	else if(fb > 1 && fatorDeBalanceamento(raiz->esq) < 0)
		raiz = rotacaoEsqDir(raiz);
	
	// Rota��o Direita Esquerda
	else if(fb < -1 && fatorDeBalanceamento(raiz->dir) > 0)
		raiz = rotacaoDirEsq(raiz);
	
	return raiz;
};

/*
	Insere um novo n� na �rvore
	raiz -> raiz da �rvore
	x -> valor a ser inserido
	Retorno: Endere�o do n� novo ou nova raiz ap�s o balanceamento
*/

No* inserir(No *raiz, int x){
	if(raiz == NULL) // �rvore vazia
		return novoNo(x);
	else{ // Verificar se a inser��o ser� � esquerda ou � direita
		if(x < raiz->valor)
			raiz->esq = inserir(raiz->esq, x); // A raiz ir� apontar para esq e inserir X na sub-�rvore esquerda
		else if(x > raiz->valor)
			raiz->dir = inserir(raiz->dir, x); // A raiz ir� apontar para dir e inserir X na sub-�rvore direita
		else
			printf("\nInser�ao nao realizada!\nO elemento %d ja existe!\n", x);
	};
	
	//Recalcula a altura de todos os n�s entre a raiz e o novo n� inserido
	raiz->altura = maior(alturaDoNo(raiz->esq), alturaDoNo(raiz->dir)) +1; //Escolhe a sub-�rvore com a maior altura e soma 1 para encontrar a altura da �rvore
	
	//Verifica a necessidade de rebalancear a �rvore
	raiz = balancear(raiz);
	
	return raiz;
}

// Fun��o para remover um n� na �rvore
No* remover(No *raiz, int chave){
	if(raiz == NULL){
		printf("Valor nao encontrado!\n");
		return NULL;
	}
	else{ // Procura o n� que ser� removido
		if(raiz->valor == chave){
			// remove n�s folhas
			if(raiz->esq == NULL && raiz->dir == NULL){
				free(raiz);
				printf("\tElemento removido: %d !\n", chave);
				return NULL;
			}
			else{
				// Remove n�s que possuem 2 filhos
				if(raiz->esq != NULL && raiz->dir != NULL){
					No *aux = raiz->esq;
					while(aux->dir != NULL)
						aux = aux->dir;
					raiz->valor = aux->valor;
					aux->valor = chave;
					printf("\tElemento trocado: %d !\n", chave);
					raiz->esq = remover(raiz->esq, chave);
					return raiz;
				}
				else{
					// remove n�s que possuem apenas 1 filho
					No *aux;
					if(raiz->esq != NULL)
						aux = raiz->esq;
					else
						aux = raiz->dir;
					free(raiz);
					printf("\tElemento removido: %d !\n", chave);
					return aux;
				}
			}
		}
		else{
			if(chave < raiz->valor)
				raiz->esq = remover(raiz->esq, chave);
			else
				raiz->dir = remover(raiz->dir, chave);
		}
		
		//Recalcula a altura de todos os n�s entre a raiz e o n� removido
		raiz->altura = maior(alturaDoNo(raiz->esq), alturaDoNo(raiz->dir)) +1; //Escolhe a sub-�rvore com a maior altura e soma 1 para encontrar a altura da �rvore
	
		//Verifica a necessidade de rebalancear a �rvore
		raiz = balancear(raiz);
	
		return raiz;
	}
}

No* buscar(No *raiz, int numx){
	while(raiz){
		if(numx < raiz->valor)
			raiz = raiz->esq;
		else if(numx > raiz->valor)
			raiz = raiz->dir;
		else
			return raiz;		
	}
	return NULL;
}

No* editar(No *raiz, int valorAntigo, int novoValor){
	No *noParaEditar = buscar(raiz, valorAntigo);
	if(noParaEditar == NULL){
		printf("Valor %d nao encontrado para edicao!\n", valorAntigo);
		return raiz;
	}
	if(buscar(raiz, novoValor) != NULL){
		printf("Erro: O valor ja existe na arvore!");
		return raiz;
	}
	raiz = remover(raiz, valorAntigo);
	raiz = inserir(raiz, novoValor);
	printf("\tValor %d editado para %d com sucesso!\n", valorAntigo, novoValor);
	return raiz;
}

// Arvore esta na posicao horizontal
void imprimirFormGrafos(No *raiz, int nivel){
	int i;
	if(raiz){
	imprimirFormGrafos(raiz->dir, nivel + 1);
	printf("\n\n");
		
	for(i = 0; i < nivel; i++)
		printf("\t");
			
	printf("%d", raiz->valor);
	imprimirFormGrafos(raiz->esq, nivel + 1);
	}
}

void imprimir_pre_ordem(No *raiz){
	if(raiz){ //Verifica se raiz � diferente de NULL
		printf("%d ", raiz->valor);
		imprimir_pre_ordem(raiz->esq);
		imprimir_pre_ordem(raiz->dir);
	}
}

void imprimir_ordem_simetrica(No *raiz){
	if(raiz){ //Verifica se raiz � diferente de NULL
		imprimir_ordem_simetrica(raiz->esq);
		printf("%d ", raiz->valor);
		imprimir_ordem_simetrica(raiz->dir);
	}
}

void imprimir_pos_ordem(No *raiz){
	if(raiz){ //Verifica se raiz � diferente de NULL
		imprimir_pos_ordem(raiz->esq);
		imprimir_pos_ordem(raiz->dir);
		printf("%d ", raiz->valor);
	}
}

int main(){
	
	int opcao, valor;
	No *busca, *raiz = NULL;
	
	do{
		printf("\n\n\tARVORE AVL");
		printf("\n\n\t0 - Sair\n\t1 - Inserir\n\t2 - Remover\n\t3 - Buscar\n\t4 - Editar\n\t5 - Imprimir Arvore no Formato de Grafos \n\t6 - Imprimir em Pre-Ordem\n\t7 - Imprimir em Ordem Simetrica\n\t8 - Imprimir em Pos-Ordem\n\n");
		scanf("%d", &opcao);
		
		switch(opcao){
			case 0:
				printf("Saindo...");
				break;
			case 1:
				printf("\tDigite o valor a ser inserido: ");
				scanf("%d", &valor);
				raiz = inserir(raiz, valor);
				break;
			case 2:
				printf("\tDigite o valor a ser removido: ");
				scanf("%d", &valor);
				raiz = remover(raiz, valor);
				break;
			case 3:
				printf("\n\tDigite o valor a ser procurado: ");
				scanf("%d", &valor);
				busca = buscar(raiz, valor);
				if(busca)
					printf("\n\tValor encontrado: %d\n", busca->valor);
				else
					printf("\n\tValor nao encontrado!\n");
				break;
			case 4:
				printf("\n\tDigite o valor que deseja editar: ");
				scanf("%d", &valor);
				printf("\n\tDigite o novo valor: ");
				int novoValor;
				scanf("%d", &novoValor);
				raiz = editar(raiz, valor, novoValor);
				break;
			case 5:
				printf("\n\tImpressao no formato de grafos na posicao horizontal (raiz eh o que se encontra mais a esquerda.):\n");
				imprimirFormGrafos(raiz, 1);
				break;
			case 6:
				printf("\nImpressao em pre-ordem: \n");
				imprimir_pre_ordem(raiz);
				break;
			case 7:
				printf("\nImpressao em ordem simetrica: \n");
				imprimir_ordem_simetrica(raiz);
				break;
			case 8:
				printf("\nImpressao em pos-ordem: \n");
				imprimir_pos_ordem(raiz);
				break;
			default:
				printf("\nOpcao invalida!!!\n");
		}
	}
	while(opcao != 0);
	
	return 0;
}
