#include <iostream>
#include <stdio.h>
#include <stdlib.h>

typedef struct no{
	int valor;
	struct no *esq;
	struct no *dir;
	int altura;
}No;

/* Função que cria um novo nó x -> valor a ser inserido no nó
retorna: o endereço do nó criado*/

No* novoNo (int x){
	No *novo = (No*) malloc(sizeof(No));
	
	if (novo){
		novo->valor = x;
		novo->esq = NULL;
		novo->dir = NULL;
		novo->altura = 0;
	}
	else
	    printf("\n Erro ao alocar nó em novoNo!\n");
	return novo;
}

// Descobrir a altura das sub-árvores
/* Retorna o maior dentre dois valores a, b -> altura de dois nós da árvore */
short maior(short a, short b){
	return (a > b)? a : b;
};

// Função para descobrir a altura de um nó
// Retorna a altura de um nó ou -1 caso ele seja null
short alturaDoNo(No *no){
	if(no == NULL)
	    return -1;
	else
	    return no->altura;
};

// Calcula e retorna o fator de balanceamento de um nó
short fatorDeBalanceamento (No *no){
	if (no)
	    return (alturaDoNo(no->esq) - alturaDoNo(no->dir));
	else
	    return 0;
};

// Função para Rotação Esquerda
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

// Função para Rotação Direita
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

// Rotação Direita Esquerda ou Dupla à Direita
No* rotacaoDirEsq(No *raiz){
	raiz->dir = rotacaoDir(raiz->dir); //'(raiz->dir)' quer dizer que a rotação direita é na sub árvore direita apenas. A raiz fica parada.
	return rotacaoEsq(raiz); //(Ele chama a função 'rotacaoEsq', criada anteriormente)'rotacaoEsq(raiz)' Essa rotação será na árvore completa. A raiz irá rotacionar também (nesse caso, para a esquerda)
};

// Rotação Esquerda Direita ou Dupla à Esquerda
No* rotacaoEsqDir(No *raiz){
	raiz->esq = rotacaoEsq(raiz->esq); //'(raiz->esq)' quer dizer que a rotação esquerda é na sub árvore esquerda apenas. A raiz fica parada.
	return rotacaoDir(raiz); //(Ele chama a função 'rotacaoDir', criada anteriormente) 'rotacaoDir(raiz)' Essa rotação será na árvore completa. A raiz irá rotacionar também (nesse caso, para a direita)
};

/*
	Função para realizar o balanceamento da árvore após uma inserção ou remoção
	Recebe o nó que está desbalanceado e retorna a nova raiz após o balanceamento
*/
No* balancear(No *raiz){
	short fb = fatorDeBalanceamento(raiz);
	
	// Rotação à esquerda
	if(fb < -1 && fatorDeBalanceamento(raiz->dir) <= 0)
		raiz = rotacaoEsq(raiz);
	
	// Rotação à direita
	else if(fb > 1 && fatorDeBalanceamento(raiz->esq) >= 0)
		raiz = rotacaoDir(raiz);
	
	// Rotação Esquerda Direita
	else if(fb > 1 && fatorDeBalanceamento(raiz->esq) < 0)
		raiz = rotacaoEsqDir(raiz);
	
	// Rotação Direita Esquerda
	else if(fb < -1 && fatorDeBalanceamento(raiz->dir) > 0)
		raiz = rotacaoDirEsq(raiz);
	
	return raiz;
};

/*
	Insere um novo nó na árvore
	raiz -> raiz da árvore
	x -> valor a ser inserido
	Retorno: Endereço do nó novo ou nova raiz após o balanceamento
*/

No* inserir(No *raiz, int x){
	if(raiz == NULL) // árvore vazia
		return novoNo(x);
	else{ // Verificar se a inserção será à esquerda ou à direita
		if(x < raiz->valor)
			raiz->esq = inserir(raiz->esq, x); // A raiz irá apontar para esq e inserir X na sub-árvore esquerda
		else if(x > raiz->valor)
			raiz->dir = inserir(raiz->dir, x); // A raiz irá apontar para dir e inserir X na sub-árvore direita
		else
			printf("\nInserçao nao realizada!\nO elemento %d ja existe!\n", x);
	};
	
	//Recalcula a altura de todos os nós entre a raiz e o novo nó inserido
	raiz->altura = maior(alturaDoNo(raiz->esq), alturaDoNo(raiz->dir)) +1; //Escolhe a sub-árvore com a maior altura e soma 1 para encontrar a altura da árvore
	
	//Verifica a necessidade de rebalancear a árvore
	raiz = balancear(raiz);
	
	return raiz;
}

// Função para remover um nó na árvore
No* remover(No *raiz, int chave){
	if(raiz == NULL){
		printf("Valor nao encontrado!\n");
		return NULL;
	}
	else{ // Procura o nó que será removido
		if(raiz->valor == chave){
			// remove nós folhas
			if(raiz->esq == NULL && raiz->dir == NULL){
				free(raiz);
				printf("\tElemento removido: %d !\n", chave);
				return NULL;
			}
			else{
				// Remove nós que possuem 2 filhos
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
					// remove nós que possuem apenas 1 filho
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
		
		//Recalcula a altura de todos os nós entre a raiz e o nó removido
		raiz->altura = maior(alturaDoNo(raiz->esq), alturaDoNo(raiz->dir)) +1; //Escolhe a sub-árvore com a maior altura e soma 1 para encontrar a altura da árvore
	
		//Verifica a necessidade de rebalancear a árvore
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
	if(raiz){ //Verifica se raiz é diferente de NULL
		printf("%d ", raiz->valor);
		imprimir_pre_ordem(raiz->esq);
		imprimir_pre_ordem(raiz->dir);
	}
}

void imprimir_ordem_simetrica(No *raiz){
	if(raiz){ //Verifica se raiz é diferente de NULL
		imprimir_ordem_simetrica(raiz->esq);
		printf("%d ", raiz->valor);
		imprimir_ordem_simetrica(raiz->dir);
	}
}

void imprimir_pos_ordem(No *raiz){
	if(raiz){ //Verifica se raiz é diferente de NULL
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
