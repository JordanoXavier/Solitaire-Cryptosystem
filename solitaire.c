#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define TAM_DECK 54
#define SPADE   "\u2660"
#define CLUB    "\u2663"
#define HEART   "\u2665"
#define DIAMOND "\u2666"

struct carta{
    int valor;
    int naipe;
    int indice;
    struct carta *next;
};

struct carta *criaBaralho(struct carta *deck){
    struct carta *aux;

    //inicializa o final do baralho com a carta 54 (coringa B)
    deck = (struct carta*)malloc(sizeof(struct carta));
    deck->indice = 54;
    deck->valor = 15; //15 eh o valor do coringa B
    deck->naipe = 4; //4 eh o naipe do coringa
    deck->next = NULL;

    //insere a cartas 53 (coringa A)
    aux = (struct carta*)malloc(sizeof(struct carta));
    aux->indice = 53;
    aux->valor = 14;
    aux->naipe = 4;
    aux->next = deck;
    deck = aux;

    //insere as cartas 52 (K de espadas) a 1 (A de paus) no inicio da lista
    for(int i=TAM_DECK-2; i>0; i--){
        aux = (struct carta*)malloc(sizeof(struct carta));

        aux->indice = i;
        aux->valor = (i%13==0)?13:i%13; //se o resto da divisao por 13 for 0, o valor da carta eh 13, senao eh o resto da divisao
        aux->naipe = (i-1)/13; //se o indice da carta for 1-13, o naipe eh 0, se for 14-26, eh 1, se for 27-39, eh 2, se for 40-52, eh 3, se for 53 ou 54, eh 4

        aux->next = deck; //insere a carta no inicio da lista 
        deck = aux; //atualiza o ponteiro deck para apontar para a nova carta
    }
    return deck;
}

void imprimeBaralho(struct carta *deck){
    struct carta *aux = deck;
    while(aux != NULL){
        printf("[%d:",aux->indice);//imprime o indice da carta
        switch(aux->valor){ //imprime o valor da carta
            case 1:
                printf("A");
                break;
            case 11:
                printf("J");
                break;
            case 12:
                printf("Q");
                break;
            case 13:
                printf("K");
                break;
            case 14: /* coringa A */
                printf("CA");
                break;
            case 15: /* coringa B */
                printf("CB");
                break;
            default:
                printf("%d",aux->valor);
        }
        printf("%s]",(aux->naipe==0)?CLUB:((aux->naipe==1)?DIAMOND:((aux->naipe==2)?HEART:(aux->naipe==3)?SPADE:""))); //imprime o naipe da carta (operador ternário if else if else if else)
        aux = aux->next;
    }
    printf("\n");
}

char *normalizeMsg(char *line){
	int i, j;
	for (i = 0; line[i] != '\0'; ++i) {
      while (!(line[i] >= 'a' && line[i] <= 'z') && !(line[i] >= 'A' && line[i] <= 'Z') && line[i] != '\0') { //enquanto o caractere nao for letra
         for (j = i; line[j] != '\0'; ++j) {
            line[j] = line[j + 1]; // desloca os caracteres para a esquerda
         }
         line[j] = '\0'; // adiciona o caractere nulo no final da string
      }
   }
   for(i=0; line[i] != '\0'; i++)
   	line[i]=toupper(line[i]); //converte para maiuscula
   return line;
}

struct carta *findPreviousCard(struct carta *card, struct carta *deck){
    struct carta *aux = deck;
    while(aux->next != card)
        aux = aux->next;

    return aux; 
}


struct carta *findJokerA(struct carta *deck){
    struct carta *aux = deck;
    while(aux->valor != 14) //enquanto o valor da carta for diferente de 14 (coringa A)
        aux = aux->next; //procura a carta coringa A

    return aux; //retorna o ponteiro para a carta coringa A
}

struct carta *findJokerB(struct carta *deck){
    struct carta *aux = deck;
    while(aux->valor != 15) //enquanto o valor da carta for diferente de 15 (coringa B)
        aux = aux->next; //procura a carta coringa B

    return aux; //retorna o ponteiro para a carta coringa B
}

struct carta *moveCardDown(struct carta *card, int moves, struct carta *deck){
    int aux;
    for(int i=0; i<moves; i++){
        if(card->next != NULL){ //se a carta nao for a ultima do baralho, apenas troque a carta com a proxima
            //fazer a carta anterior a card apontar para card->next
            if(card == deck){ //se a carta for a primeira do baralho
                struct carta *aux = card->next;
                deck->next = deck->next->next;
                aux->next = deck;
                deck = aux;
            }
            else{
                struct carta *prev = findPreviousCard(card, deck);
                prev->next = card->next;
                //fazer card apontar para card->next->next
                struct carta *aux = card->next;
                card->next = card->next->next;
                //fazer aux apontar para card
                aux->next = card;
            }

        }
        else{ //se for a última carta, mova-o para a segunda posição
            //fazer a penúltima carta apontar para NULL
            struct carta *aux = findPreviousCard(card, deck);
            aux->next = NULL;

            //fazer a carta em movimento apontar para a segunda carta
            card->next = deck->next;

            //fazer a primeira carta apontar para a carta em movimento
            deck->next = card;
        }
    }

    return deck;
}


struct carta *findFirstJoker(struct carta *deck){
    struct carta *aux = deck;
    while(aux->valor != 14 && aux->valor != 15)
        aux = aux->next;

    return aux;
}

struct carta *findLastJoker(struct carta *deck){
    struct carta *aux = deck;
    while(aux->next != NULL)//acha a ultima carta
        aux = aux->next;

    while(aux->valor != 14 && aux->valor != 15)//acha o coringa mais proximo da ultima carta
        aux = findPreviousCard(aux, deck);

    return aux;
}

struct carta *tripleCut(struct carta *deck){
    struct carta *firstJoker = findFirstJoker(deck);
    struct carta *lastJoker = findLastJoker(deck);
    struct carta *aux;
    aux = deck;

    if(firstJoker->next == deck && lastJoker->next == NULL){ //se o primeiro coringa for a primeira carta do baralho e o ultimo coringa for a ultima carta do baralho
        return deck;
    }
    else if(firstJoker->next == deck){ //se o primeiro coringa for a primeira carta do baralho
        while(aux->next != NULL)
            aux = aux->next; //achar a última carta do baralho
        aux->next = deck; //faz a ultima carta apontar para a primeira carta
        deck = lastJoker->next; //faz a primeira carta apontar para a carta depois do ultimo coringa
        lastJoker->next = NULL; //faz a ultima carta apontar para NULL
    }
    else if(lastJoker->next == NULL){
        aux = findPreviousCard(firstJoker, deck); //acha a carta anterior ao primeiro coringa
        aux->next = NULL; //faz a carta anterior ao primeiro coringa apontar para NULL
        lastJoker->next = deck; //faz a ultima carta apontar para a primeira carta
        deck = firstJoker; //faz a primeira carta apontar para o primeiro coringa
    }
    else{ //se os coringas nao estiverem na primeira ou ultima posicao
        while(aux->next != NULL)
            aux = aux->next; //achar a última carta do baralho
        aux->next = firstJoker; //faz a ultima carta apontar para o primeiro coringa

        aux = findPreviousCard(firstJoker, deck); //acha a carta anterior ao primeiro coringa
        aux->next = NULL; //faz a carta anterior ao primeiro coringa apontar para NULL

        aux = lastJoker->next; //aux aponta para a carta depois do ultimo coringa
        lastJoker->next = deck; //faz a ultima carta apontar para a primeira carta
        deck = aux; //faz a primeira carta apontar para a carta depois do ultimo coringa
    }

    return deck;
}


struct carta *countCut(struct carta *deck){
    struct carta *last = deck;
    while(last->next != NULL) //acha a ultima carta
        last = last->next;
    struct carta *prevLast = findPreviousCard(last, deck); //acha a carta anterior a ultima carta

    if(last->valor == 14 || last->valor == 15) //se a ultima carta for um coringa, nao faca nada
        return deck;
    else{
        int count = last->indice; //count recebe o valor da ultima carta
        struct carta *aux = deck;
        for(int i=0; i<count; i++) //aux aponta para a carta que esta count posicoes a frente da primeira carta
            aux = aux->next;
        
        struct carta *prevAux = findPreviousCard(aux, deck);
        prevAux->next = last; //faz a carta anterior a aux apontar para a última carta

        prevLast->next = deck; //faz a carta anterior a ultima carta apontar para a primeira carta
        deck = aux; //faz a primeira carta apontar para a carta que esta count posicoes a frente da primeira carta

        return deck;        
    }
}


int findKey(struct carta *deck){
    struct carta *aux = deck;
    int key = aux->indice;
    if(key == 54)
        key = 53; //ambos os coringas tem indice 54, mas o valor deles eh 53

    for(int i=0; i<key; i++) //aux aponta para a carta que esta key posicoes a frente da primeira carta
        aux = aux->next;

    if(aux->naipe ==4){ //se for coringa
        return 0;
    }
    else{
        int value = aux->indice;
        key = (value<27)?value:value-26; //se o valor for maior que 26, subtrai 26

        return key;
    }
}


struct carta *nextKeystream(struct carta *deck, int *value){
    int key=0;
	do{
		/* passo 1 */
		deck = moveCardDown(findJokerA(deck), 1, deck); //move a carta coringa A uma posicao para baixo
        //imprimeBaralho(deck);
		/* passo 2 */
		deck = moveCardDown(findJokerB(deck), 2, deck); //move a carta coringa B duas posicoes para baixo
		//imprimeBaralho(deck);
		/* passo 3 */
		deck = tripleCut(deck);
		//imprimeBaralho(deck);
		/* passo 4 */
		deck = countCut(deck);
		//imprimeBaralho(deck);
		/* passo 5 */
		key = findKey(deck);
	}while(key == 0); /* se a carta encontrada for um coringa, repetir os passos */

    *value = key;

	return deck;
}

char *encryptSolitaire(char *msg, struct carta *deck){
	int i, nextStreamValue, nextCharCipher;
	char *cifra;
	cifra = (char *)malloc(sizeof(char) * strlen(msg));
	for(i=0; msg[i] != '\0'; i++){
		deck = nextKeystream(deck, &nextStreamValue); // gera o próximo keystream
		nextCharCipher = (nextStreamValue + (msg[i]-'A'+1))%27; // encripta (soma com o keystream)
		cifra[i] = nextCharCipher+'A'-1; // converte para char
	}
	return cifra;
}

char *decryptSolitaire(char *cifra, struct carta *deck){
	int i, nextStreamValue, nextCharDecipher;
	char *msg;
	msg = (char *)malloc(sizeof(char) * strlen(cifra));
	for(i=0; cifra[i] != '\0'; i++){
		deck = nextKeystream(deck, &nextStreamValue);
		nextCharDecipher = (cifra[i]-'A') - nextStreamValue ;
		if(nextCharDecipher < 0)
			nextCharDecipher += 27;
		msg[i] = nextCharDecipher+'A';
	}
	return msg;
}

int main(){
	char msg[256];
	char *cifra;
    struct carta *deck = NULL;

	printf("Creating standard deck.\n");
	deck = criaBaralho(deck);
	printf("Deck configured.\n");
	imprimeBaralho(deck);

	printf("Enter the message to be encrypted by Solitaire:\n");
	fgets(msg, 256, stdin);
	strcpy(msg, normalizeMsg(msg));

	cifra = encryptSolitaire(msg, deck);
	fprintf(stdout,"Encrypting\nmessage: [%s] cipher[%s]\n",msg, cifra);

	deck = criaBaralho(deck); /* retorna o baralho a configuração inicial */
	strcpy(msg, decryptSolitaire(cifra, deck));
	fprintf(stdout,"Decrypting\ncipher: [%s] message[%s]\n",cifra, msg);
	printf("\nEnd of test.\n");

	exit(0);
}

