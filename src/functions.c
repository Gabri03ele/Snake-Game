/**
 * @file Armani_Scantamburlo.c
 * @author Gabriele Srmani 896872 Nicolo' Scantamburlo 895101
 * @brief Snake con modalita' interattiva e AI.
 * @date 22/06/2023
*/

#include <stdio.h>

#include <stdlib.h>

#include <stdbool.h>

#include <string.h>

#include "myfunctions.h"

/** La mappa standard **/
char maze_global [7][10] = { {'#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},

                             {'O', ' ', '#', ' ', ' ', ' ', '#', ' ', ' ', '#'},

                             {'#', ' ', '#', ' ', '!', ' ', '#', '!', '!', '#'},

                             {'#', ' ', '#', ' ', '!', ' ', '#', ' ', ' ', '#'},

                             {'#', ' ', ' ', ' ', '#', ' ', '!', ' ', ' ', '#'},

                             {'#', ' ', '$', ' ', '#', '$', '$', '$', ' ', '_'},

                             {'#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},

                           };


/**
 * Funzione usata per creare un nuovo nodo
 * @param xx e' usata per la coordinata altezza
 * @param yy e' usata per la coordinata larghezza
 * @return il nodo appena creato 
*/
pCoda new_node(int xx, int yy) {

    pCoda l = (pCoda) malloc (sizeof(coda));

    if (!l) exit(1);

    l->x = xx;

    l->y = yy;

    l->next = NULL;

    l->prev = NULL;



    return l;

}

/**
 * Funzione usata per inserire un nuovo nodo alla fine della lista
 * @param head e' il primo nodo della lista
 * @param xx e' usata per la coordinata altezza
 * @param yy e' usata per la coordinata larghezza
*/
void l_append(pCoda* head, int xx, int yy) { //push_back

  if (*head==NULL) {

     *head = new_node(xx,yy);

  } else {

    coda* l = *head;

    while ( l->next!=NULL ) {

      l = l->next;

    }

    l->next = new_node(xx,yy);

    

    l->prev = l;

  }

}


/**
 * Funzione usata per inserire un nuovo nodo all'inizio della lista
 * @param head e' il primo nodo della lista
 * @param xx e' usata per la coordinata altezza
 * @param yy e' usata per la coordinata larghezza
*/
void push_front(int xx, int yy, pCoda* head) {

    if(*head == NULL){

        l_append(head, xx, yy);

    } else {

        pCoda newnode = (coda*) malloc(sizeof(coda));

        if (!newnode) exit(1);

        pCoda tmp = *head;

        

        newnode->x = xx;

        newnode->y = yy;

        newnode->prev = NULL;

        newnode->next = tmp;

        

        tmp->prev = newnode;

        *head = newnode;

        

        newnode = NULL;

        free(newnode);

    }

}


/**
 * Funzione usata per rimuovere un nuovo nodo alla fine della lista
 * @param head e' il primo nodo della lista
*/
void pop_back(pCoda *head) {

    if(*head == NULL) return;



    pCoda tmp = *head;

    if(tmp->next == NULL){

        *head = NULL;

        return;

    }

    pCoda penultimo = tmp;



    while(tmp->next != NULL){

        penultimo = tmp;

        tmp = tmp->next;

    }

    penultimo->next = NULL;

    

    free(tmp);

}


/**
 * Funzione usata per liberare la memoria
 * @param head e' il primo nodo della lista
*/
void libera(pCoda *head){

    if(*head == NULL) return;

    while(*head) {

        pop_back(head);

    }

}


/**
 * Funzione usata per contare il numero di code
 * @param head e' il primo nodo della lista
*/
int conta_coda(pCoda *head){

    int contatore = 0;



    if(*head == NULL) return 0;



    pCoda tmp = *head;

    if(tmp->next == NULL) return 1;



    while(tmp){

        contatore++;

        tmp = tmp->next;

    }

  return contatore;

}

/**
 * Funzione usata per dimezzare la coda una volta preso un imprevisto
 * @param head e' il primo nodo della lista
 * @param ncoda il numero di code dello snake
*/
int taglia_coda(pCoda *head, int ncoda) {

    if(*head == 0 || ncoda == 0) return 0;



    int div = (ncoda + 1) / 2;

    

    pCoda tmp = *head;

    while(tmp && div > 0){

        pop_back(head);

        div--;

        

    }

    return div;

}


/**
 * Funzione usata per rimuovere le code quando lo snake ci va contr
 * @param head e' il primo nodo della lista
 * @param ncoda numero di code dello snake
 * @param coo coordinate dello snake principale
 * @return il numero di code rimanenti
*/
int mangia_coda(pCoda *head, coordinate coo, int ncoda) {

    if(*head == NULL) return 0;

    pCoda tmp = *head;

    int counter = 0;

    bool found = false;

      while(tmp && !found){

        counter++;

        if(tmp->x == coo.i && tmp->y == coo.j) {

            found = true;

        }

        tmp = tmp->next;

    }

    int differenza = ncoda - counter;

    pCoda tmp1 = *head;

    

    if(differenza == 0 && (tmp1->x == coo.i && tmp1->y == coo.j)) {  //caso della singola coda

         *head = NULL;

         ncoda -= 1;

         free(tmp);

        

         return ncoda;

    }



    while(differenza > 0 && found) {

        ncoda--;

        differenza--;

        pop_back(head);

    }

    return ncoda;

}

/**
 * Funzione usata per rimuovere le code quando lo snake ci va contr
 * @param head e' il primo nodo della lista
 * @param coo coordinate dello snake principale
*/
void switch_positions(pCoda *head, coordinate coo) {

    // devo prendere le coordinaate vecchie della P, e sostituirle con l'ultimo elemento della coda

   if(*head == NULL) return;



    pCoda tmp1 = *head;



    if(tmp1->next == NULL){ // nel caso ci sia solo una cella faccio lo swap coordinate con la P

        tmp1->x = coo.i;

        tmp1->y = coo.j;

       

    } else { // invece nel caso ci sono più celle devo anche ordinarle

    pop_back(head);

    push_front(coo.i,coo.j,head);

    }

}


/**
 * Funzione usata per stampare a video le code dello snake
 * @param head e' il primo nodo della lista
 * @param maze la mappa usata
 * @param variabili altezza e larghezza della mappa
*/
void print_coda(char* maze, mappa variabili, pCoda *head) {

    if(*head == NULL) return;



    pCoda tmp = *head;

    while (tmp) {

    maze[tmp->x * variabili.lunghezza + tmp->y] = 'o';

    tmp = tmp->next;

    }

}


/**
 * Funzione usata per stampare a video il labirinto
*/
void stampa (char* maze, mappa variabili) {

    printf("\n");

    

    for(int i = 0; i < variabili.altezza; i++){

        printf("\t\t ");

        for(int j = 0; j < variabili.lunghezza; j++){

            printf("%c", maze[i * variabili.lunghezza + j]);

        }

        printf("\n");

    }

}

/**
 * Funzione che controlla se l'uscita è ancora presente
 * @param maze la mappa che usiamo
 * @param variabili le variabili per l'altezza e lunghezza della mappa
 * @return true se siamo sopra all'uscita, false in caso contrario
*/

bool win (char* maze, mappa variabili) {

    bool check = false;



    for(int i = 0; i < variabili.altezza; i++){

        for(int j = 0; j < variabili.lunghezza; j++){

            if(maze[i * variabili.lunghezza + j] == '_') check = true; // controllo in tutta la mappa se c'e' ancora la O

        }

    }

    return check;

}


/**
 * Funzione che controlla se siamo sopra a una moneta
 * @param maze la mappa che usiamo
 * @param variabile le variabili per l'altezza e lunghezza della mappa
 * @param coo coordinate dello snake principale
 * @return true se siamo sopra alla moneta, false in caso contrario
*/
bool moneta(char* maze, mappa variabile, coordinate coo){ // controllo se sono salito sopra una moneta

    bool check = false;

    

    if(maze[coo.i * variabile.lunghezza + coo.j] == '$') check = true;

   

    return check;

}

/**
 * Funzione che controlla se siamo sopra a una imprevisto
 * @param maze la mappa che usiamo
 * @param variabile le variabili per l'altezza e lunghezza della mappa
 * @param coo coordinate dello snake principale
 * @return true se siamo sopra alla imprevisto, false in caso contrario
*/

bool imprevisto(char* maze, mappa variabile, coordinate coo){ // controllo se sono salito sopra un imprevisto

    bool check = false;

    if(maze[coo.i * variabile.lunghezza + coo.j] == '!') check = true;

    

    return check;

}

/**
 * Funzione che controlla se siamo sopra a una muro
 * @param maze la mappa che usiamo
 * @param variabile le variabili per l'altezza e lunghezza della mappa
 * @param coo coordinate dello snake principale
 * @return true se siamo sopra alla muro, false in caso contrario
*/

bool muro(char* maze, mappa variabile, coordinate coo){ // controllo se sto andando contro un muro

    bool check = false;

    if(maze[coo.i * variabile.lunghezza + coo.j] == '#') check = true;

    

    return check;

}

/**
 * Funzione che controlla se siamo sopra a una trapano
 * @param maze la mappa che usiamo
 * @param variabile le variabili per l'altezza e lunghezza della mappa
 * @param coo coordinate dello snake principale
 * @return true se siamo sopra alla trapano, false in caso contrario
*/

bool trapano(char* maze, mappa variabile, coordinate coo){

    bool check = false;

    if(maze[coo.i * variabile.lunghezza + coo.j] == 'T') check = true;

    

    return check;

}

/**
 * Funzione che cerca la posizione in cui si trova snake
 * @param maze la mappa che usiamo
 * @param variabili le variabili per l'altezza e lunghezza della mappa
 * @param coo coordinate dello snake principale
 * @return restituisce le coordinate in cui si trova lo snake
*/

coordinate personaggio(char* maze, mappa variabili, coordinate coo){

    coo.i = 0;

    coo.j = 0;



 

    for(int i = 0; i < variabili.altezza; i++) {

        for(int j = 0; j < variabili.lunghezza; j++) {

            if(maze[i * variabili.lunghezza + j] == 'O') {

                coo.i = i;

                coo.j = j;

            }

        }

    }

    return coo;

}


/** sposto lo snake alle posizione nuove **/
void nuova_posizione(char* maze, mappa variabili, coordinate coo){

    maze[coo.i * variabili.lunghezza + coo.j] = 'O';

}


/**
 * Funzione che controlla se andiamo fuori dal campo di gioco
 * @param maze la mappa che usiamo
 * @param coo coordinate dello snake principale
 * @return true se stiamo provando ad uscire dalla mappa, altrimenti false
*/
bool collisione(coordinate coo, mappa maze) {

     if (coo.j < 0 || coo.j >= maze.lunghezza || coo.i < 0 || coo.i >= maze.altezza) {

         return false;

     }

    return true;

 }


/**
 * Funzione che legge i comandi e sposta il nostro snake
 * @param maze la mappa che usiamo
 * @param variabili le variabili per l'altezza e lunghezza della mappa
 * @param coo le coordinate attuali dello snake
 * @param trapani se "trapani" e' attivo possiamo rompare
 * @return le coordinate aggiornate se non stiamo andando contro un muro senno' restituisce le coordinate non aggiornate
*/
coordinate leggi_comandi(char* maze, coordinate coo, mappa variabili, bool trapani){

    coordinate nuovo;

    nuovo.i = coo.i;

    nuovo.j = coo.j;



    

    switch (variabili.direzione) {

        case 'w': nuovo.i -= 1;  break;

        case 's': nuovo.i += 1;  break;

        case 'a': nuovo.j -= 1;  break;

        case 'd': nuovo.j += 1;  break;

        case 'W': nuovo.i -= 1;  break;

        case 'S': nuovo.i += 1;  break;

        case 'A': nuovo.j -= 1;  break;

        case 'D': nuovo.j += 1;  break;

        default: printf(" \n Mossa  non valida \n");

    }





    if((!muro(maze,variabili,nuovo) || trapani) && (collisione(nuovo,variabili)) ) { // se non vai contro un muro  o il trapano e' attivo allora restituisco le coordinate nuove

        return nuovo;

    } else {

        return coo; // senno' restituisco le variabili vecchie

    }

}


/**
 * Funzione che cancella la posizione precedente dello snake 
 * @param maze la mappa che usiamo
 * @param variabili le variabili per l'altezza e lunghezza della mappa
 * @param coo coordinate dello snake principale
*/
void Refresh(char* maze, mappa variabili, coordinate coo){ // cancello la P

   maze[coo.i * variabili.lunghezza + coo.j] = ' ';

}


/**
 * Funzione che cancella le posizioni precedenti della coda
 * @param maze la mappa che usiamo
 * @param variabili le variabili per l'altezza e lunghezza della mappa
 * @param head del primo nodo della lista
*/
void refresh_coda(char* maze, mappa variabili, pCoda *head) {

   if(*head == NULL) {

       return;

   } else {

       pCoda tmp = *head;

       while(tmp){

           maze[tmp->x * variabili.lunghezza + tmp->y] = ' ';

           tmp = tmp->next;

       }

    }

}


/**
 * Funzione che, grazie ad un loop, mi continua a giocare finche' non arriviamo all'uscita o si fa un punteggio negativo  
 * @param maze la mappa che usiamo
 * @param altezza altezza della mappa
 * @param lunghezza larghezza della mappa
*/
void modalita_interattiva(char* maze, int altezza, int lunghezza) {

    mappa variabili;

    coordinate coo = {0,0};

   

    pCoda head = NULL;

    int ncoda = 0;



    int punteggio = 1000;

    int trapani = 0;

    

    variabili.altezza = altezza;

    variabili.lunghezza = lunghezza;



    stampa(maze, variabili);

    coo =  personaggio(maze,variabili,coo); // inizializzo le variabili i e j nella cella in cui si trova O



    while(win(maze,variabili) && punteggio >= 0) {

        bool pc = false;

       

        coordinate coo_coda = coo; // cordinate non aggiornate

       

        Refresh(maze,variabili,coo);

        ncoda = conta_coda(&head);

       

        printf(" Inserisci una mossa [w, a, s, d]: ");

        scanf(" %c", &variabili.direzione);

        

        coo = leggi_comandi(maze,coo, variabili, trapani);

 

        //   ECCEZIONE NEL CASO ANDIAMO CONTRO UN MURO O FUORI DAI LIMITI RIFACCIAMO LA MOSSA

        while(coo.i == coo_coda.i && coo.j == coo_coda.j){

            

            printf("\n Inserisci una nuova mossa [w,a,s,d]: ");

            scanf(" %c", &variabili.direzione);

            coo_coda = coo;

            coo = leggi_comandi(maze,coo,variabili,trapani);

        }

        ncoda = mangia_coda(&head,coo, ncoda);



        if(trapano(maze,variabili,coo)) trapani += 3;

        

        if(trapani > 0) {

            if(muro(maze,variabili,coo)) --trapani; // se vado contro un muro, allora uso un trapano

            printf("\t\tHai a disposizione %d trapani\n", trapani);

        }



        if(moneta(maze,variabili,coo)){

            punteggio += 10;

            push_front(coo_coda.i, coo_coda.j, &head);

            pc = true;

            ncoda++;

        }

        

        if(imprevisto(maze,variabili,coo)){

            punteggio = punteggio / 2;

            ncoda = taglia_coda(&head,ncoda);

         }

        

        nuova_posizione(maze,variabili, coo);



        if(!pc) switch_positions(&head,coo_coda);

       

        print_coda(maze,variabili,&head);

     

        stampa(maze,variabili);

        printf("\t \t \t Il tuo punteggio e' di: %d \n \n", punteggio);



       refresh_coda(maze,variabili,&head);

       punteggio--;

        

    }

     libera(&head);

     free(head);

    

    if(punteggio >= 0){

         printf("\a");



         printf("\n \n");

         printf("------------------------------------------------------------------\n");

         printf("\n \t \t \t \t     Hai vinto! \n \n \t \t \t Hai fatto un punteggio di: %d \n", punteggio);

         printf("------------------------------------------------------------------\n");

         printf("\n");

         

    } else {

         printf("\n \n");

         printf("------------------------------------------------------------------\n");

         printf("\n \t \t \t \t    Hai perso! :( \n \n \t \t \t Hai fatto un punteggio di: %d \n", punteggio);

         printf("------------------------------------------------------------------\n");

         printf("\n");

    }

}



/*

 ------------------------------------------------------------------------------------------------------------------------------------------------

 -------------------------------------------------MODALITA' AI-----------------------------------------------------------------------------------

 ------------------------------------------------------------------------------------------------------------------------------------------------

 */



bool controllo_SOPRA(char* maze, mappa variabili, int i, int j, char dir);

bool controllo_SOTTO(char* maze, mappa variabili, int i, int j, char dir);

bool controllo_DESTRA(char* maze, mappa variabili, int i, int j, char dir);

bool controllo_SINISTRA(char* maze, mappa variabili, int i, int j, char dir);



// serve a distruggere il controllo direzione che da come  risultato false!

void distruggi(char* maze, mappa variabili, int i, int j, char a);

/**
 * Funzione che mi controlla se lo snake ha trovaro un bivio
 * @param maze e' la mappa usata
 * @param variabili le variabili usate per l'altezza e lunghezza della mappa
 * @param i altezza mappa
 * @param j larghezza mappa
 * @return true se ho trovato un bivio, altrimenti false
*/
bool crossroads(char* maze, mappa variabili, int i, int j) {

    bool check = false;



    //posso andare sia destra che sotto

    if((maze[(i - 1) * variabili.lunghezza + (j)] == '#' || maze[(i + 1) * variabili.lunghezza + (j)] == '#') && maze[(i) * variabili.lunghezza + j + 1] == ' ' && maze[(i) * variabili.lunghezza + j - 1] == ' ' && !check)

    {

        check = true;

     }

    

    //posso andare copra che a destra

    if((maze[(i-1)*variabili.lunghezza+(j+1)] == '#' || maze[(i+1)*variabili.lunghezza+(j+1)] == '#') && maze[(i-1)*variabili.lunghezza+j] == ' ' && maze[(i)*variabili.lunghezza+(j+1)] == ' ' && !check)

    {

        check = true;

     }

    

    //posso andare sia sopra che sotto;

    if((maze[i * variabili.lunghezza + (j + 1)] == '#' || maze[i * variabili.lunghezza + (j - 1)] == '#') && maze[(i + 1) * variabili.lunghezza+j] == ' ' && maze[(i - 1)*variabili.lunghezza + j] == ' ' && !check)

    {

        check = true;

     }

    

    //posso andare sia a sinistra che sotto

    if(maze[(i - 1)*variabili.lunghezza+(j - 1)] == '#'  && ( maze[(i)*variabili.lunghezza+(j-1)] == ' ' && maze[(i+1)*variabili.lunghezza+(j)] == ' ' && !check))

    {

        check = true;

     }

     return check;

}

/**
 * Funzione che controlla usando il backtracking la possibili vie del nostro percorso
 * @param maze la mappa che usiamo
 * @param variabili le variabili per l'altezza e lunghezza della mappa
 * @param i altezza mappa
 * @param j lunghezza mappa
 * @return true se siamo sopra alla moneta, false in caso contrario
*/

bool bivio(char* maze, mappa variabili, int i, int j) {

    bool direzione_su = false;

    bool direzione_giu = false;

    bool direzione_destra = false;

    bool direzione_sinistra = false;



    bool bivioo = false;



    // posso andare sia a destra che sinistra

    if((maze[(i-1)*variabili.lunghezza+(j)] == '#' || maze[(i + 1)*variabili.lunghezza+(j)] == '#') && maze[(i)*variabili.lunghezza+j+1] == ' ' && maze[(i)*variabili.lunghezza+j-1] == ' ' && !bivioo)

    {

        direzione_destra = controllo_DESTRA(maze,variabili,i,j+1,'D');

        if(!direzione_destra) distruggi(maze,variabili,i,j+1,'D');



        direzione_sinistra = controllo_SINISTRA(maze,variabili,i,j-1,'L');

        if(!direzione_sinistra) distruggi(maze,variabili,i,j-1,'L');



        bivioo = direzione_destra || direzione_sinistra;

    }

    

    //posso andare sia sopra che a destra

    if((maze[(i-1)*variabili.lunghezza+(j+1)] == '#' || maze[(i+1)*variabili.lunghezza+(j+1)] == '#') && maze[(i-1)*variabili.lunghezza+j] == ' ' && maze[(i)*variabili.lunghezza+(j+1)] == ' ' && !bivioo)

     {

         direzione_su = controllo_SOPRA(maze,variabili,i-1,j,'S');

         if(!direzione_su) distruggi(maze,variabili,i-1,j,'S');

         

         direzione_destra = controllo_DESTRA(maze,variabili,i,j+1,'D');

         if(!direzione_destra) distruggi(maze,variabili,i,j+1,'D');

        

         bivioo = direzione_su || direzione_destra;

    }



    // posso andare sia a sopra che sotto

    if((maze[i*variabili.lunghezza+(j+1)] == '#' || maze[i*variabili.lunghezza+(j-1)] == '#') && maze[(i+1)*variabili.lunghezza+j] == ' ' && maze[(i-1)*variabili.lunghezza+j] == ' ' && !bivioo)

     {

         direzione_su = controllo_SOPRA(maze,variabili,i-1,j,'S');

         if(!direzione_su) distruggi(maze,variabili,i-1,j,'S');

         

         direzione_giu = controllo_SOTTO(maze,variabili,i+1,j,'U');

         if(!direzione_giu) distruggi(maze,variabili,i+1,j,'U');



         bivioo = direzione_su || direzione_giu;

    }



    //posso andare sia sinistra che sotto

    if((( maze[(i + 1)*variabili.lunghezza+(j - 1)] == '#') && ( maze[(i)*variabili.lunghezza+(j-1)] == ' ' && maze[(i+1)*variabili.lunghezza+(j)] == ' ') && !bivioo))

    {

        direzione_sinistra = controllo_SINISTRA(maze,variabili,i,j-1,'L');

        if(!direzione_sinistra) distruggi(maze,variabili,i,j-1,'L');



        direzione_giu = controllo_SOTTO(maze,variabili,i+1,j,'U');

        if(!direzione_giu) distruggi(maze,variabili,i+1,j,'U');



        bivioo = direzione_sinistra || direzione_giu;

    }

    return bivioo;

}



/*

    queste funzioni controllano dove il nostro personaggio potrebbe andare,

    e controllano le possibile vie che il nostro percorso può andare.

    lo scopo è quello di evitare i vicoli ciechi.

    infatti ritorna false se non trova la O, la H, o non trova altri bivii.



    problema, quando entrambe le direzioni ritornano false

*/

/**
 * Funzione che controlla se sopra c'e' l'uscita o un'altro bivio
 * @param maze la mappa che usiamo
 * @param variabili le variabili per l'altezza e lunghezza della mappa
 * @param i altezza mappa
 * @param j lunghezza mappa
 * @param dir la funzione ricorsivamente segue la direzione precedentemente fatta
 * @return true se sopra troviamo l'uscita oppure un altro bivio, falso in caso contrario
*/

bool controllo_SOPRA(char* maze, mappa variabili, int i, int j, char dir){

    stampa(maze,variabili);



    if(maze[i * variabili.lunghezza + j] == '#')  return false;



    maze[i * variabili.lunghezza + j] = 'S';



    if(maze[(i-1)*variabili.lunghezza+j] == '_') return true;

    if(maze[(i+1)*variabili.lunghezza+j] == '_') return true;

    if(maze[i*variabili.lunghezza+j+1] == '_') return true;

    if(maze[i*variabili.lunghezza+j-1] == '_') return true;



    if(maze[(i-1)*variabili.lunghezza+j] == 'H') return true;

    if(maze[(i+1)*variabili.lunghezza+j] == 'H') return true;

    if(maze[i*variabili.lunghezza+j+1] == 'H') return true;

    if(maze[i*variabili.lunghezza+j-1] == 'H') return true;



    if((j < 0 || j > variabili.lunghezza || i < 0 || i > variabili.altezza)) return false;



    

    if(crossroads(maze,variabili,i,j)) {

        maze[(i)*variabili.lunghezza+j] = 'S';

        return bivio(maze,variabili,i,j);

    }

    

    if(dir == 'D')

    {

        if(((maze[(i)*variabili.lunghezza+j+1] == ' ' || maze[(i)*variabili.lunghezza+j+1] == '$' || maze[(i)*variabili.lunghezza+j+1] == '!' || maze[(i)*variabili.lunghezza+j+1] == 'T' )) && (maze[(i)*variabili.lunghezza+j+1] != 
'S' || maze[(i)*variabili.lunghezza+j+1] != '#' || maze[(i)*variabili.lunghezza+j+1] != 'I'))

        {

            return controllo_SOPRA(maze,variabili,i,j+1,'D');

        }

    }

    

    if(dir == 'L')

    {

        if(((maze[(i)*variabili.lunghezza+j-1] == ' ' || maze[(i)*variabili.lunghezza+j-1] == '$' || maze[(i)*variabili.lunghezza+j-1] == '!' || maze[(i)*variabili.lunghezza+j-1] == 'T' )) && (maze[(i)*variabili.lunghezza+j-1] != 
'S' || maze[(i)*variabili.lunghezza+j-1] != '#' || maze[(i)*variabili.lunghezza+j-1] != 'I'))

        {

            return controllo_SOPRA(maze,variabili,i,j-1,'L');

        }

    }

    

    if(dir == 'U')

    {

        if(((maze[(i+1)*variabili.lunghezza+j] == ' ' || maze[(i+1)*variabili.lunghezza+j] == '$' || maze[(i+1)*variabili.lunghezza+j] == '!' || maze[(i+1)*variabili.lunghezza+j] == 'T' )) && (maze[(i+1)*variabili.lunghezza+j] != 
'S' || maze[(i+1)*variabili.lunghezza+j] != '#' || maze[(i+1)*variabili.lunghezza+j] != 'I'))

        {

            return controllo_SOPRA(maze,variabili,i+1,j,'U');

        }

    }

    

    if(dir == 'S')

    {

        if(((maze[(i-1)*variabili.lunghezza+j] == ' ' || maze[(i-1)*variabili.lunghezza+j] == '$' || maze[(i-1)*variabili.lunghezza+j] == '!' || maze[(i-1)*variabili.lunghezza+j] == 'T' )) && (maze[(i-1)*variabili.lunghezza+j] != 
'S' || maze[(i-1)*variabili.lunghezza+j] != '#' || maze[(i-1)*variabili.lunghezza+j] != 'I'))

        {

            return controllo_SOPRA(maze,variabili,i-1,j,'S');

        }

    }



    if(((maze[(i)*variabili.lunghezza+j-1] == ' ' || maze[(i)*variabili.lunghezza+j-1] == '$' || maze[(i)*variabili.lunghezza+j-1] == '!' || maze[(i)*variabili.lunghezza+j-1] == 'T' )) && (maze[(i)*variabili.lunghezza+j-1] != 'S' || 
maze[(i)*variabili.lunghezza+j-1] != '#' || maze[(i)*variabili.lunghezza+j-1] != 'I'))

    {

        return controllo_SOPRA(maze,variabili,i,j-1,'L');

    }



    if(((maze[(i+1)*variabili.lunghezza+j] == ' ' || maze[(i+1)*variabili.lunghezza+j] == '$' || maze[(i+1)*variabili.lunghezza+j] == '!' || maze[(i+1)*variabili.lunghezza+j] == 'T' )) && (maze[(i+1)*variabili.lunghezza+j] != 'S' || 
maze[(i+1)*variabili.lunghezza+j] != '#' || maze[(i+1)*variabili.lunghezza+j] != 'I'))

    {

         return controllo_SOPRA(maze,variabili,i+1,j,'U');

    }



    if(((maze[(i-1)*variabili.lunghezza+j] == ' ' || maze[(i-1)*variabili.lunghezza+j] == '$' || maze[(i-1)*variabili.lunghezza+j] == '!' || maze[(i-1)*variabili.lunghezza+j] == 'T' )) && (maze[(i-1)*variabili.lunghezza+j] != 'S' || 
maze[(i-1)*variabili.lunghezza+j] != '#' || maze[(i-1)*variabili.lunghezza+j] != 'I'))

    {

         return controllo_SOPRA(maze,variabili,i-1,j,'S');

    }



    if(((maze[(i)*variabili.lunghezza+j+1] == ' ' || maze[(i)*variabili.lunghezza+j+1] == '$' || maze[(i)*variabili.lunghezza+j+1] == '!' || maze[(i)*variabili.lunghezza+j+1] == 'T' )) && (maze[(i)*variabili.lunghezza+j+1] != 'S' || 
maze[(i)*variabili.lunghezza+j+1] != '#' || maze[(i)*variabili.lunghezza+j+1] != 'I'))

    {

         return controllo_SOPRA(maze,variabili,i,j+1,'D');

    }



  return false;

}

/**
 * Funzione che controlla se sotto c'e' l'uscita o un'altro bivio
 * @param maze la mappa che usiamo
 * @param variabili le variabili per l'altezza e lunghezza della mappa
 * @param i altezza mappa
 * @param j lunghezza mappa
 * @param dir la funzione ricorsivamente segue la direzione precedentemente fatta
 * @return true se sotto troviamo l'uscita oppure un altro bivio, falso in caso contrario
*/

bool controllo_SOTTO(char* maze, mappa variabili, int i, int j, char dir){

    if(maze[i*variabili.lunghezza+j] == 'I') return false;

    if(maze[i*variabili.lunghezza+j] == '#') return false;

    

    maze[i*variabili.lunghezza+j] = 'U';

    

    if(maze[(i-1)*variabili.lunghezza+j] == '_') return true;

    if(maze[(i+1)*variabili.lunghezza+j] == '_') return true;

    if(maze[i*variabili.lunghezza+j+1] == '_') return true;

    if(maze[i*variabili.lunghezza+j-1] == '_') return true;
    

    if(maze[(i-1)*variabili.lunghezza+j] == 'H') return true;

    if(maze[(i+1)*variabili.lunghezza+j] == 'H') return true;

    if(maze[i*variabili.lunghezza+j+1] == 'H') return true;

    if(maze[i*variabili.lunghezza+j-1] == 'H') return true;

    

    if((j < 0 || j >= variabili.lunghezza || i < 0 || i >= variabili.altezza)) return false;

    

    if(crossroads(maze,variabili,i,j)) {

        maze[(i)*variabili.lunghezza+j] = 'U';

        return bivio(maze,variabili,i,j);

    }

    

    if(dir == 'D')

    {

        if(((maze[(i)*variabili.lunghezza+j+1] == ' ' || maze[(i)*variabili.lunghezza+j+1] == '$' || maze[(i)*variabili.lunghezza+j+1] == '!' || maze[(i)*variabili.lunghezza+j+1] == 'T' )) && (maze[(i)*variabili.lunghezza+j+1] != 
'U' || maze[(i)*variabili.lunghezza+j+1] != '#' || maze[(i)*variabili.lunghezza+j+1] != 'I'))

        {

            return controllo_SOTTO(maze,variabili,i,j+1,'D');

        }

    }

    

    if(dir == 'L')

    {

        if(((maze[(i)*variabili.lunghezza+j-1] == ' ' || maze[(i)*variabili.lunghezza+j-1] == '$' || maze[(i)*variabili.lunghezza+j-1] == '!' || maze[(i)*variabili.lunghezza+j-1] == 'T' )) && (maze[(i)*variabili.lunghezza+j-1] != 
'U' || maze[(i)*variabili.lunghezza+j-1] != '#' || maze[(i)*variabili.lunghezza+j-1] != 'I'))

        {

            return controllo_SOTTO(maze,variabili,i,j-1,'L');

        }

    }

    

    if(dir == 'U')

    {

        if(((maze[(i+1)*variabili.lunghezza+j] == ' ' || maze[(i+1)*variabili.lunghezza+j] == '$' || maze[(i+1)*variabili.lunghezza+j] == '!' || maze[(i+1)*variabili.lunghezza+j] == 'T' )) && (maze[(i+1)*variabili.lunghezza+j] != 
'U' || maze[(i+1)*variabili.lunghezza+j] != '#' || maze[(i+1)*variabili.lunghezza+j] != 'I'))

        {

            return controllo_SOTTO(maze,variabili,i+1,j,'U');

        }

    }

    if(dir == 'S')

    {

        if(((maze[(i-1)*variabili.lunghezza+j] == ' ' || maze[(i-1)*variabili.lunghezza+j] == '$' || maze[(i-1)*variabili.lunghezza+j] == '!' || maze[(i-1)*variabili.lunghezza+j] == 'T' )) && (maze[(i-1)*variabili.lunghezza+j] != 
'U' || maze[(i-1)*variabili.lunghezza+j] != '#' || maze[(i-1)*variabili.lunghezza+j] != 'I'))

        {

            return controllo_SOTTO(maze,variabili,i-1,j,'S');

        }

    }

    

    if(((maze[(i)*variabili.lunghezza+j-1] == ' ' || maze[(i)*variabili.lunghezza+j-1] == '$' || maze[(i)*variabili.lunghezza+j-1] == '!' || maze[(i)*variabili.lunghezza+j-1] == 'T' )) && (maze[(i)*variabili.lunghezza+j-1] != 'U' || 
maze[(i)*variabili.lunghezza+j-1] != '#' || maze[(i)*variabili.lunghezza+j-1] != 'I'))

    {

        return controllo_SOTTO(maze,variabili,i,j-1,'L');

    }

    

    if(((maze[(i+1)*variabili.lunghezza+j] == ' ' || maze[(i+1)*variabili.lunghezza+j] == '$' || maze[(i+1)*variabili.lunghezza+j] == '!' || maze[(i+1)*variabili.lunghezza+j] == 'T' )) && (maze[(i+1)*variabili.lunghezza+j] != 'U' || 
maze[(i+1)*variabili.lunghezza+j] != '#' || maze[(i+1)*variabili.lunghezza+j] != 'I'))

    {

        return controllo_SOTTO(maze,variabili,i+1,j,'U');

    }

    

    if(((maze[(i-1)*variabili.lunghezza+j] == ' ' || maze[(i-1)*variabili.lunghezza+j] == '$' || maze[(i-1)*variabili.lunghezza+j] == '!' || maze[(i-1)*variabili.lunghezza+j] == 'T' )) && (maze[(i-1)*variabili.lunghezza+j] != 'U' || 
maze[(i-1)*variabili.lunghezza+j] != '#' || maze[(i-1)*variabili.lunghezza+j] != 'I'))

    {

        return controllo_SOTTO(maze,variabili,i-1,j,'S');

    }

    

    if(((maze[(i)*variabili.lunghezza+j+1] == ' ' || maze[(i)*variabili.lunghezza+j+1] == '$' || maze[(i)*variabili.lunghezza+j+1] == '!' || maze[(i)*variabili.lunghezza+j+1] == 'T' )) && (maze[(i)*variabili.lunghezza+j+1] != 'U' || 
maze[(i)*variabili.lunghezza+j+1] != '#' || maze[(i)*variabili.lunghezza+j+1] != 'I'))

    {

        return controllo_SOTTO(maze,variabili,i,j+1,'D');

    }

    

    return false;

}

/**
 * Funzione che controlla se destra c'e' l'uscita o un'altro bivio
 * @param maze la mappa che usiamo
 * @param variabili le variabili per l'altezza e lunghezza della mappa
 * @param i altezza mappa
 * @param j lunghezza mappa
 * @param dir la funzione ricorsivamente segue la direzione precedentemente fatta
 * @return true se destra troviamo l'uscita oppure un altro bivio, falso in caso contrario
*/

bool controllo_DESTRA(char* maze, mappa variabili, int i, int j, char dir) {

    if(maze[i*variabili.lunghezza+j] == '#'){

        return false;

    }

    

    maze[i*variabili.lunghezza+j] = 'D';

    

    if(maze[(i-1)*variabili.lunghezza+j] == '_') return true;

    if(maze[(i+1)*variabili.lunghezza+j] == '_') return true;

    if(maze[i*variabili.lunghezza+j+1] == '_') return true;

    if(maze[i*variabili.lunghezza+j-1] == '_') return true;



     if(maze[(i-1)*variabili.lunghezza+j] == 'H') return true;

    if(maze[(i+1)*variabili.lunghezza+j] == 'H') return true;

    if(maze[i*variabili.lunghezza+j+1] == 'H') return true;

    if(maze[i*variabili.lunghezza+j-1] == 'H') return true;



    if((j < 0 || j >= variabili.lunghezza || i < 0 || i >= variabili.altezza)) return false;



    if( crossroads(maze,variabili,i,j)) {

        maze[i*variabili.lunghezza+j] = 'D';

        return bivio(maze,variabili,i,j);

        }





    if(dir == 'D')

    {

        if(((maze[(i)*variabili.lunghezza+j+1] == ' ' || maze[(i)*variabili.lunghezza+j+1] == '$' || maze[(i)*variabili.lunghezza+j+1] == '!' || maze[(i)*variabili.lunghezza+j+1] == 'T' )) && (maze[(i)*variabili.lunghezza+j+1] != 
'D' || maze[(i)*variabili.lunghezza+j+1] != '#' || maze[(i)*variabili.lunghezza+j+1] != 'I'))

        {

            return controllo_DESTRA(maze,variabili,i,j+1,'D');

        }

    }

       

    if(dir == 'L')

    {

        if(((maze[(i)*variabili.lunghezza+j-1] == ' ' || maze[(i)*variabili.lunghezza+j-1] == '$' || maze[(i)*variabili.lunghezza+j-1] == '!' || maze[(i)*variabili.lunghezza+j-1] == 'T' )) && (maze[(i)*variabili.lunghezza+j-1] != 
'D' || maze[(i)*variabili.lunghezza+j-1] != '#' || maze[(i)*variabili.lunghezza+j-1] != 'I'))

        {

            return controllo_DESTRA(maze,variabili,i,j-1,'L');

        }

    }

       

    if(dir == 'U')

    {

        if(((maze[(i+1)*variabili.lunghezza+j] == ' ' || maze[(i+1)*variabili.lunghezza+j] == '$' || maze[(i+1)*variabili.lunghezza+j] == '!' || maze[(i+1)*variabili.lunghezza+j] == 'T' )) && (maze[(i+1)*variabili.lunghezza+j] != 
'D' || maze[(i+1)*variabili.lunghezza+j] != '#' || maze[(i+1)*variabili.lunghezza+j] != 'I'))

        {

            return controllo_DESTRA(maze,variabili,i+1,j,'U');

        }

    }

       

    if(dir == 'S')

       {

        if(((maze[(i-1)*variabili.lunghezza+j] == ' ' || maze[(i-1)*variabili.lunghezza+j] == '$' || maze[(i-1)*variabili.lunghezza+j] == '!' || maze[(i-1)*variabili.lunghezza+j] == 'T' )) && (maze[(i-1)*variabili.lunghezza+j] != 
'D' || maze[(i-1)*variabili.lunghezza+j] != '#' || maze[(i-1)*variabili.lunghezza+j] != 'I'))

        {

            return controllo_DESTRA(maze,variabili,i-1,j,'S');

        }

    }



    if(((maze[(i)*variabili.lunghezza+j-1] == ' ' || maze[(i)*variabili.lunghezza+j-1] == '$' || maze[(i)*variabili.lunghezza+j-1] == '!' || maze[(i)*variabili.lunghezza+j-1] == 'T' )) && (maze[(i)*variabili.lunghezza+j-1] != 'D' || 
maze[(i)*variabili.lunghezza+j-1] != '#' || maze[(i)*variabili.lunghezza+j-1] != 'I'))

    {

        return controllo_DESTRA(maze,variabili,i,j-1,'L');

    }



    if(((maze[(i+1)*variabili.lunghezza+j] == ' ' || maze[(i+1)*variabili.lunghezza+j] == '$' || maze[(i+1)*variabili.lunghezza+j] == '!' || maze[(i+1)*variabili.lunghezza+j] == 'T' )) && (maze[(i+1)*variabili.lunghezza+j] != 'D' || 
maze[(i+1)*variabili.lunghezza+j] != '#' || maze[(i+1)*variabili.lunghezza+j] != 'I'))

    {

         return controllo_DESTRA(maze,variabili,i+1,j,'U');

    }



    if(((maze[(i-1)*variabili.lunghezza+j] == ' ' || maze[(i-1)*variabili.lunghezza+j] == '$' || maze[(i-1)*variabili.lunghezza+j] == '!' || maze[(i-1)*variabili.lunghezza+j] == 'T' )) && (maze[(i-1)*variabili.lunghezza+j] != 'D' || 
maze[(i-1)*variabili.lunghezza+j] != '#' || maze[(i-1)*variabili.lunghezza+j] != 'I'))

    {

         return controllo_DESTRA(maze,variabili,i-1,j,'S');

    }



    if(((maze[(i)*variabili.lunghezza+j+1] == ' ' || maze[(i)*variabili.lunghezza+j+1] == '$' || maze[(i)*variabili.lunghezza+j+1] == '!' || maze[(i)*variabili.lunghezza+j+1] == 'T' )) && (maze[(i)*variabili.lunghezza+j+1] != 'D' || 
maze[(i)*variabili.lunghezza+j+1] != '#' || maze[(i)*variabili.lunghezza+j+1] != 'I'))

    {

         return controllo_DESTRA(maze,variabili,i,j+1,'D');

    }

       

     return false;

}

/**
 * Funzione che controlla se sinistra c'e' l'uscita o un'altro bivio
 * @param maze la mappa che usiamo
 * @param variabili le variabili per l'altezza e lunghezza della mappa
 * @param i altezza mappa
 * @param j lunghezza mappa
 * @param dir la funzione ricorsivamente segue la direzione precedentemente fatta
 * @return true se sinistra troviamo l'uscita oppure un altro bivio, falso in caso contrario
*/


bool controllo_SINISTRA(char* maze, mappa variabili, int i, int j, char dir){

   if(maze[i*variabili.lunghezza+j] == '#') return false;



    if(maze[i*variabili.lunghezza+j] == 'I'){

        return false;

    }



    maze[i*variabili.lunghezza+j] = 'L';



  

    if(maze[(i-1)*variabili.lunghezza+j] == '_') return true;

    if(maze[(i+1)*variabili.lunghezza+j] == '_') return true;

    if(maze[i*variabili.lunghezza+j+1] == '_') return true;

    if(maze[i*variabili.lunghezza+j-1] == '_') return true;



    if(maze[(i-1)*variabili.lunghezza+j] == 'H') return true;

    if(maze[(i+1)*variabili.lunghezza+j] == 'H') return true;

    if(maze[i*variabili.lunghezza+j+1] == 'H') return true;

    if(maze[i*variabili.lunghezza+j-1] == 'H') return true;



    if(((j < 0 || j > variabili.lunghezza) || (i < 0 || i > variabili.altezza))) return false;



    if(crossroads(maze,variabili,i,j)) {

        maze[i*variabili.lunghezza+j] = 'L';

        return bivio(maze,variabili,i,j);

    }



    if(dir == 'D')

    {

        if(((maze[(i)*variabili.lunghezza+j+1] == ' ' || maze[(i)*variabili.lunghezza+j+1] == '$' || maze[(i)*variabili.lunghezza+j+1] == '!' || maze[(i)*variabili.lunghezza+j+1] == 'T' )) && (maze[(i)*variabili.lunghezza+j+1] != 
'L' || maze[(i)*variabili.lunghezza+j+1] != '#' || maze[(i)*variabili.lunghezza+j+1] != 'I'))

        {

            return controllo_SINISTRA(maze,variabili,i,j+1,'D');

        }

    }

        

    if(dir == 'L')

    {

        if(((maze[(i)*variabili.lunghezza+j-1] == ' ' || maze[(i)*variabili.lunghezza+j-1] == '$' || maze[(i)*variabili.lunghezza+j-1] == '!' || maze[(i)*variabili.lunghezza+j-1] == 'T' )) && (maze[(i)*variabili.lunghezza+j-1] != 
'L' || maze[(i)*variabili.lunghezza+j-1] != '#' || maze[(i)*variabili.lunghezza+j-1] != 'I'))

        {

            return controllo_SINISTRA(maze,variabili,i,j-1,'L');

        }

    }

        

    if(dir == 'U')

    {

        if(((maze[(i+1)*variabili.lunghezza+j] == ' ' || maze[(i+1)*variabili.lunghezza+j] == '$' || maze[(i+1)*variabili.lunghezza+j] == '!' || maze[(i+1)*variabili.lunghezza+j] == 'T' )) && (maze[(i+1)*variabili.lunghezza+j] != 
'L' || maze[(i+1)*variabili.lunghezza+j] != '#' || maze[(i+1)*variabili.lunghezza+j] != 'I'))

        {

         return controllo_SINISTRA(maze,variabili,i+1,j,'U');

        }

    }

    if(dir == 'S')

    {

        if(((maze[(i-1)*variabili.lunghezza+j] == ' ' || maze[(i-1)*variabili.lunghezza+j] == '$' || maze[(i-1)*variabili.lunghezza+j] == '!' || maze[(i-1)*variabili.lunghezza+j] == 'T' )) && (maze[(i-1)*variabili.lunghezza+j] != 
'L' || maze[(i-1)*variabili.lunghezza+j] != '#' || maze[(i-1)*variabili.lunghezza+j] != 'I'))

        {

            return controllo_SINISTRA(maze,variabili,i-1,j,'S');

        }

    }



    if(((maze[(i)*variabili.lunghezza+j-1] == ' ' || maze[(i)*variabili.lunghezza+j-1] == '$' || maze[(i)*variabili.lunghezza+j-1] == '!' || maze[(i)*variabili.lunghezza+j-1] == 'T' )) && (maze[(i)*variabili.lunghezza+j-1] != 'L' || 
maze[(i)*variabili.lunghezza+j-1] != '#' || maze[(i)*variabili.lunghezza+j-1] != 'I'))

    {

        return controllo_SINISTRA(maze,variabili,i,j-1,'L');

    }



    if(((maze[(i+1)*variabili.lunghezza+j] == ' ' || maze[(i+1)*variabili.lunghezza+j] == '$' || maze[(i+1)*variabili.lunghezza+j] == '!' || maze[(i+1)*variabili.lunghezza+j] == 'T' )) && (maze[(i+1)*variabili.lunghezza+j] != 'L' || 
maze[(i+1)*variabili.lunghezza+j] != '#' || maze[(i+1)*variabili.lunghezza+j] != 'I'))

    {

         return controllo_SINISTRA(maze,variabili,i+1,j,'U');

    }



    if(((maze[(i-1)*variabili.lunghezza+j] == ' ' || maze[(i-1)*variabili.lunghezza+j] == '$' || maze[(i-1)*variabili.lunghezza+j] == '!' || maze[(i-1)*variabili.lunghezza+j] == 'T' )) && (maze[(i-1)*variabili.lunghezza+j] != 'L' || 
maze[(i-1)*variabili.lunghezza+j] != '#' || maze[(i-1)*variabili.lunghezza+j] != 'I'))

    {

         return controllo_SINISTRA(maze,variabili,i-1,j,'S');

    }



    if(((maze[(i)*variabili.lunghezza+j+1] == ' ' || maze[(i)*variabili.lunghezza+j+1] == '$' || maze[(i)*variabili.lunghezza+j+1] == '!' || maze[(i)*variabili.lunghezza+j+1] == 'T' )) && (maze[(i)*variabili.lunghezza+j+1] != 'L' || 
maze[(i)*variabili.lunghezza+j+1] != '#' || maze[(i)*variabili.lunghezza+j+1] != 'I'))

    {

         return controllo_SINISTRA(maze,variabili,i,j+1,'D');

    }

        

     return false;

}



/*

le funzioni controllo se incontrano un bivio, (ossia può andare in due direzioni) oppure l'uscita (_) devono ritornare TRUE

così la funzione AI potrà gestire quali coordinare inviare alle funzioni controllo!!

*/
/**
 * Funzione che mi cancella un determinato simbolo fino a che non trovo l'uscita
 * @param maze e' la mappa usata
 * @param variabili le variabili usate per l'altezza e lunghezza della mappa
 * @param i altezza mappa
 * @param j larghezza mappa
 * @param a simbolo che andra' distrutto
 * @return true se ho trovato l'uscita, altrimenti false
*/
bool spezza_destra(char* maze, mappa variabili, int i, int j, char a){

    //nel caso la mentre segue la H possa andare sia a destra che sinistra, controlla quale arriva effettivamente alla fine

    if( maze[i*variabili.lunghezza+j] != '#' )  maze[i*variabili.lunghezza+j] = 'B';



    if(maze[(i-1)*variabili.lunghezza+j] == '_') return true;

    if(maze[(i+1)*variabili.lunghezza+j] == '_') return true;

    if(maze[i*variabili.lunghezza+j+1] == '_') return true;

    if(maze[i*variabili.lunghezza+j-1] == '_') return true;





    if(maze[i*variabili.lunghezza+j+1] == a){

         return spezza_destra(maze,variabili,i,j+1,a);

    }

        

    if(maze[(i-1)*variabili.lunghezza+j] == a){

        return spezza_destra(maze,variabili,i-1,j,a);

    }



    if(maze[i*variabili.lunghezza+j-1] == a){

        return spezza_destra(maze,variabili,i,j-1,a);

    }

        

    if(maze[(i+1)*variabili.lunghezza+j] == a){

       return spezza_destra(maze,variabili,i+1,j,a);

    }

        

    return false;

}

/**
 * Funzione che mi cancella un determinato simbolo fino a che non trovo l'uscita
 * @param maze e' la mappa usata
 * @param variabili le variabili usate per l'altezza e lunghezza della mappa
 * @param i altezza mappa
 * @param j larghezza mappa
 * @param a simbolo che andrà distrutto
 * @return true se ho trovato l'uscita, altrimenti false
*/
bool spezza_sinistra(char* maze, mappa variabili, int i, int j, char a){

    //nel caso la mentre segue la H possa andare sia a destra che sinistra, controlla quale arriva effettivamente alla fine

    if( maze[i*variabili.lunghezza+j] != '#' )  maze[i*variabili.lunghezza+j] = 'B';



    if(maze[(i-1)*variabili.lunghezza+j] == '_') return true;

    if(maze[(i+1)*variabili.lunghezza+j] == '_') return true;

    if(maze[i*variabili.lunghezza+j+1] == '_') return true;

    if(maze[i*variabili.lunghezza+j-1] == '_') return true;





    if(maze[i*variabili.lunghezza+j-1] == a){

         return spezza_sinistra(maze,variabili,i,j-1,a);

    }

        

    if(maze[(i+1)*variabili.lunghezza+j] == a ){

       return spezza_sinistra(maze,variabili,i-1,j,a);

    }

        

    if(maze[(i-1)*variabili.lunghezza+j] == a){

        return spezza_sinistra(maze,variabili,i+1,j,a);

    }

        

    if(maze[i*variabili.lunghezza+j+1] == a){

        return spezza_sinistra(maze,variabili,i,j+1,a);

    }

    return false;

}

/**
 * Funzione che mi cancella un determinato simbolo fino a che non trovo l'uscita
 * @param maze e' la mappa usata
 * @param variabili le variabili usate per l'altezza e lunghezza della mappa
 * @param i altezza mappa
 * @param j larghezza mappa
 * @param a simbolo che andrà distrutto
 * @return true se ho trovato l'uscita, altrimenti false
*/
bool spezza_sopra(char* maze, mappa variabili, int i, int j, char a){

    //nel caso la mentre segue la H possa andare sia sopra che sotto, controlla quale arriva effettivamente alla fine



    if(maze[i*variabili.lunghezza+j] != '#')  maze[i*variabili.lunghezza+j] = 'B';



    if(maze[(i-1)*variabili.lunghezza+j] == '_') return true;

    if(maze[(i+1)*variabili.lunghezza+j] == '_') return true;

    if(maze[i*variabili.lunghezza+j+1] == '_') return true;

    if(maze[i*variabili.lunghezza+j-1] == '_') return true;





    if(maze[(i-1)*variabili.lunghezza+j] == a){

         return spezza_sopra(maze,variabili,i-1,j,a);

    }

    if(maze[(i)*variabili.lunghezza+j+1] == a ){

       return spezza_sopra(maze,variabili,i,j+1,a);

    }

        

    if(maze[(i+1)*variabili.lunghezza+j] == a){

        return spezza_sopra(maze,variabili,i+1,j,a);

    }

        

    if(maze[i*variabili.lunghezza+j-1] == a){

        return spezza_sopra(maze,variabili,i,j-1,a);

    }

    return false;

}

/**
 * Funzione che mi cancella un determinato simbolo fino a che non trovo l'uscita
 * @param maze e' la mappa usata
 * @param variabili le variabili usate per l'altezza e lunghezza della mappa
 * @param i altezza mappa
 * @param j larghezza mappa
 * @param a simbolo che andrà distrutto
 * @return true se ho trovato l'uscita, altrimenti false
*/
bool spezza_sotto(char* maze, mappa variabili, int i, int j, char a){

    //nel caso la mentre segue la H possa andare sia sopra che sotto, controlla quale arriva effettivamente alla fine

    if( maze[i*variabili.lunghezza+j] != '#' )  maze[i*variabili.lunghezza+j] = 'B';



    if(maze[(i-1)*variabili.lunghezza+j] == '_') return true;

    if(maze[(i+1)*variabili.lunghezza+j] == '_') return true;

    if(maze[i*variabili.lunghezza+j+1] == '_') return true;

    if(maze[i*variabili.lunghezza+j-1] == '_') return true;





    if(maze[(i+1)*variabili.lunghezza+j] == a){

         return spezza_sotto(maze,variabili,i+1,j,a);

    }

        

    if(maze[(i)*variabili.lunghezza+j+1] == a){

       return spezza_sotto(maze,variabili,i,j+1,a);

    }

        

    if(maze[(i-1)*variabili.lunghezza+j] == a) {

        return spezza_sotto(maze,variabili,i-1,j,a);

    }

        

    if(maze[i*variabili.lunghezza+j-1] == a){

        return spezza_sotto(maze,variabili,i,j-1,a);

    }

    return false;

}


/**
 * Funzione che mi distrugge il percorso delle 'a' che non mi porta all'uscita
 * @param maze e' la mappa usata
 * @param variabili le variabili usate per l'altezza e lunghezza della mappa
 * @param i altezza mappa
 * @param j larghezza mappa
 * @param a simbolo che andrà distrutto
*/
void distruggi(char* maze, mappa variabili, int i, int j, char a){

    //mi distrugge il percorso delle 'a' che non mi porta dalla O!



    bool sopra = false;
    bool sotto = false;
    bool destra = false;
    bool sinistra = false;

      

    if(maze[i*variabili.lunghezza+j] == '#') return;

  

    while(maze[(i-1)*variabili.lunghezza+j] == a || maze[(i+1)*variabili.lunghezza+j] == a || maze[(i)*variabili.lunghezza+j+1] == a || maze[(i)*variabili.lunghezza+j-1] == a){

        if(sopra) {

            if(maze[(i-1)*variabili.lunghezza+j] == a && maze[(i-1)*variabili.lunghezza+j] != '#'){

                 maze[i*variabili.lunghezza+j] = ' ';

                 i -= 1;

            } else sopra = false;

        }



        if(destra){

            if(maze[i*variabili.lunghezza+j+1] == a && maze[i*variabili.lunghezza+j+1] != '#'){

                 maze[i*variabili.lunghezza+j] = ' ';

                 j += 1;

            } else destra = false;

        }



        if(sinistra){

            if(maze[i*variabili.lunghezza+j-1] == a && maze[i*variabili.lunghezza+j-1] != '#'){

                maze[i*variabili.lunghezza+j] = ' ';

                j -= 1;

            } else sinistra = false;

        }



        if(sotto){

            if(maze[(i+1)*variabili.lunghezza+j] == a && maze[(i+1)*variabili.lunghezza+j] != '#') {

                maze[i*variabili.lunghezza+j] = ' ';

                i += 1;

            } else sotto = false;

        }



        if(maze[(i+1)*variabili.lunghezza+j] == a && !sotto){

            maze[i*variabili.lunghezza+j] = ' ';

            i += 1;

            sotto = true;

        }

        

        if(maze[i*variabili.lunghezza+j+1] == a && !destra){

            maze[i*variabili.lunghezza+j] = ' ';

            j += 1;

            destra = true;

        }

        

        if(maze[i*variabili.lunghezza+j-1] == a && !sinistra){

            maze[i*variabili.lunghezza+j] = ' ';

            j -= 1;

            sinistra = true;

        }

        

        if(maze[(i-1)*variabili.lunghezza+j] == a && !sopra){

            maze[i*variabili.lunghezza+j] = ' ';

            i -= 1;

            sopra = true;

        }

    }

    maze[(i)*variabili.lunghezza+j] = ' ';

}


/**
 * Funzione che mi ripristiana il simbolo 'B' (usato per i controlli), nel suo simbolo originale
 * @param maze e' la mappa usata
 * @param variabili le variabili usate per l'altezza e lunghezza della mappa
 * @param a simbolo originale
*/
void ripristina(char* maze, mappa variabili, char a){

    for(int i = 0; i < variabili.altezza; i++){

        for(int j = 0; j < variabili.lunghezza; j++){

            if(maze[i*variabili.lunghezza+j] == 'B') maze[i*variabili.lunghezza+j] = a;

        }

    }

}


/**
 * Funzione che mi controlla se nel cercare l'uscita ho trovato un bivio
 * @param maze e' la mappa usata
 * @param variabili le variabili usate per l'altezza e lunghezza della mappa
 * @param i altezza mappa
 * @param j larghezza mappa
 * @param a simbolo che andrà distrutto
*/
void controllo_bivio_I_H(char* maze, mappa variabili, int i, int j, char a) {

// sotto e sopra
    if(maze[(i-1)*variabili.lunghezza+j] == a && maze[(i+1)*variabili.lunghezza+j] == a){

        

        if(!spezza_sotto(maze,variabili,i,j,a)) {

            maze[(i+1)*variabili.lunghezza+j] = ' ';

            maze[(i)*variabili.lunghezza+j] = 'I';

            ripristina(maze,variabili,a);

            

            return;

        }



       if(!spezza_sopra(maze,variabili,i,j, a)){

            maze[(i-1)*variabili.lunghezza+j] = ' ';

            maze[(i)*variabili.lunghezza+j] = 'I';

            ripristina(maze,variabili,a);

           

            return;

        }

    }


// sinistra e destra
    if(maze[(i)*variabili.lunghezza+j-1] == a && maze[(i)*variabili.lunghezza+j+1] == a){

        

       if(!spezza_sinistra(maze,variabili,i,j, a) ){

           maze[i*variabili.lunghezza+j-1] = ' ';

           maze[i*variabili.lunghezza+j] = 'I';

            ripristina(maze,variabili,a);

           

            return;

        }

         if(!spezza_destra(maze,variabili,i,j, a)){

           maze[i*variabili.lunghezza+j+1] = ' ';

           maze[i*variabili.lunghezza+j] = 'I';

            ripristina(maze,variabili, a);

             

            return;

        }

    }

    //sopra e sinistra
    if(maze[(i)*variabili.lunghezza+(j-1)] == a && maze[(i-1)*variabili.lunghezza+(j)] == a){
        
        if(!spezza_sopra(maze,variabili,i,j, a)){

            maze[(i-1)*variabili.lunghezza+j] = ' ';

            maze[(i)*variabili.lunghezza+j] = 'I';

            ripristina(maze,variabili,a);

           

            return;

        }

        if(!spezza_sinistra(maze,variabili,i,j, a) ){

           maze[i*variabili.lunghezza+j-1] = ' ';

           maze[i*variabili.lunghezza+j] = 'I';

           ripristina(maze,variabili,a);

        

            return;

        }
        
       
    }
    //sotto e destra
     if(maze[(i)*variabili.lunghezza+(j+1)] == a && maze[(i+1)*variabili.lunghezza+(j)] == a){
        
        if(!spezza_sotto(maze,variabili,i,j, a) ){

           maze[(i+1)*variabili.lunghezza+j] = ' ';

           maze[i*variabili.lunghezza+j] = 'I';

            ripristina(maze,variabili,a);

           
            return;

        }
        
        if(!spezza_destra(maze,variabili,i,j, a)){

            maze[(i)*variabili.lunghezza+j+1] = ' ';

            maze[(i)*variabili.lunghezza+j] = 'I';

            ripristina(maze,variabili,a);

            return;

        }
    }

}



bool fuori_mappa(int i, int j, mappa variabili){

   if(j >= 0 && j <= variabili.lunghezza && i >= 0 && i <= variabili.altezza) return true;

   else return false;

}


/**
 * Funzione ricorsiva che in modo casuale mi cerca l'uscita, quando trova un bivio attiva le funzioni ausiliarie
 * @param maze e' la mappa usata
 * @param variabili le variabili usate per l'altezza e lunghezza della mappa
 * @param i altezza mappa
 * @param j larghezza mappa
 * @return true se ho trovato l'uscita, altrimenti false
*/
bool AI(char* maze, mappa variabili, int i, int j){

        stampa(maze,variabili);

   

        if(maze[(i-1)*variabili.lunghezza+j] == '_' && (i-1 >= 0)) return true;

        if(maze[(i+1)*variabili.lunghezza+j] == '_') return true;

        if(maze[i*variabili.lunghezza+j+1] == '_') return true;

        if(maze[i*variabili.lunghezza+j-1] == '_' && (j-1 >= 0)) return true;



        controllo_bivio_I_H(maze,variabili,i,j,'H');



        if(maze[i*variabili.lunghezza+j+1] == 'H'){

            maze[i*variabili.lunghezza+j+1] = 'I';

            return AI(maze, variabili, i, j+1);

        }

        

        if(maze[(i+1)*variabili.lunghezza+j] == 'H' ){

            maze[(i+1)*variabili.lunghezza+j] = 'I';

            return AI(maze, variabili, i+1, j);

        }

        

        if(maze[(i-1)*variabili.lunghezza+j] == 'H'){

            maze[(i-1)*variabili.lunghezza+j] = 'I';

            return AI(maze, variabili, i-1, j);

        }

        

        if(maze[i*variabili.lunghezza+j-1] == 'H'){

            maze[i*variabili.lunghezza+j-1] = 'I';

            return AI(maze, variabili, i, j-1);

        }

    

        //se trovo le lettere U S D L (usate per il backtracking) le seguo, portano ad una H o all'uscita o a un'altro bivio

        if(maze[(i+1)*variabili.lunghezza+j] == 'U' || maze[(i+1)*variabili.lunghezza+j] == 'S' ||  maze[(i+1)*variabili.lunghezza+j] == 'D' || maze[(i+1)*variabili.lunghezza+j] == 'L'){

            maze[(i+1)*variabili.lunghezza+j] = 'I';

            return AI(maze, variabili, i+1, j);

        } else

        if(maze[(i-1)*variabili.lunghezza+j] == 'U' || maze[(i-1)*variabili.lunghezza+j] == 'S' || maze[(i-1)*variabili.lunghezza+j] == 'D' || maze[(i-1)*variabili.lunghezza+j] == 'L'){

            maze[(i-1)*variabili.lunghezza+j] = 'I';

            return AI(maze, variabili, i-1, j);

        } else

        if(maze[i*variabili.lunghezza+j+1] == 'U' || maze[i*variabili.lunghezza+j+1] == 'S' || maze[i*variabili.lunghezza+j+1] == 'D' || maze[i*variabili.lunghezza+j+1] == 'L'){

            maze[i*variabili.lunghezza+j+1] = 'I';

            return AI(maze, variabili, i, j+1);

        } else

        if(maze[i*variabili.lunghezza+j-1] == 'U' || maze[i*variabili.lunghezza+j-1] == 'S' || maze[i*variabili.lunghezza+j-1] == 'D' || maze[i*variabili.lunghezza+j-1] == 'L'){

            maze[i*variabili.lunghezza+j-1] = 'I';

            return AI(maze, variabili, i, j-1);

        }

/*

 appena mi trovo su O e non ci sono vicoli ciechi inizio a scrivere il percorso in questo modo

    se posso vado prima a destra.

    sennò vado giù

    sennò vado sopra

    sennò vado a sinistra

    se nessuna di queste mi sono bloccato

*/

        if(maze[i*variabili.lunghezza+j] == 'O'){

            controllo_bivio_I_H(maze,variabili,i,j,'H');



            if((maze[i*variabili.lunghezza+j+1] == ' ' || maze[i*variabili.lunghezza+j+1] == '$' || maze[i*variabili.lunghezza+j+1] == '!' || maze[i*variabili.lunghezza+j+1] == 'T')&& maze[i*variabili.lunghezza+j+1] != 'I' && 
maze[i*variabili.lunghezza+j+1] != '#' && maze[i*variabili.lunghezza+j+1] != 'O' && fuori_mappa(i,j,variabili))

            {

                maze[i*variabili.lunghezza+j+1] = 'I';

                return AI(maze,variabili,i,j+1);

            }

            else if((maze[(i+1)*variabili.lunghezza+j] == ' ' || maze[(i+1)*variabili.lunghezza+j] == '$' || maze[(i+1)*variabili.lunghezza+j] == '!' || maze[(i+1)*variabili.lunghezza+j] == 'T') && maze[(i+1)*variabili.lunghezza+j] 
!= 'I' && maze[(i+1)*variabili.lunghezza+j] != '#' && maze[(i+1)*variabili.lunghezza+j] != 'O' && fuori_mappa(i,j,variabili))

            {

                maze[(i+1)*variabili.lunghezza+j] = 'I';

                return AI(maze,variabili,i+1,j);

            }

            else if((maze[(i-1)*variabili.lunghezza+j] == ' ' || maze[(i-1)*variabili.lunghezza+j] == '$' || maze[(i-1)*variabili.lunghezza+j] != '!' || maze[(i-1)*variabili.lunghezza+j] != 'T') && maze[(i-1)*variabili.lunghezza+j] 
!= 'I' && maze[(i-1)*variabili.lunghezza+j] != '#' && maze[(i-1)*variabili.lunghezza+j] != 'O' && fuori_mappa(i,j,variabili))

            {

                maze[(i-1)*variabili.lunghezza+j] = 'I';

                return AI(maze,variabili,i-1,j);

            }

            else if ((maze[i*variabili.lunghezza+j-1] == ' ' || maze[i*variabili.lunghezza+j-1] == '$' || maze[i*variabili.lunghezza+j-1] == '!' || maze[i*variabili.lunghezza+j-1] == 'T') && maze[i*variabili.lunghezza+j-1] != 'I' && 
maze[i*variabili.lunghezza+j-1] != '#' && maze[i*variabili.lunghezza+j-1] != 'O' && fuori_mappa(i,j,variabili))

            {

                maze[i*variabili.lunghezza+j-1] = 'I';

                return AI(maze,variabili,i,j-1);

            } else return false;

        }



    

    // nel caso non ci fossero bivii allora proseguimo il nostro percorso in questo modo:



    if((maze[i*variabili.lunghezza+j+1] == ' ' || maze[i*variabili.lunghezza+j+1] == '$' || maze[i*variabili.lunghezza+j+1] == '!' || maze[i*variabili.lunghezza+j+1] == 'T') && maze[i*variabili.lunghezza+j+1] != 'I' && 
maze[i*variabili.lunghezza+j+1] != '#' && maze[i*variabili.lunghezza+j+1] != 'O' && (j+1 <= variabili.lunghezza &&  j+1 >= 0))

    {

        maze[i*variabili.lunghezza+j+1] = 'I';

        if(crossroads(maze,variabili,i,j+1)){

            bivio(maze,variabili,i,j+1);

        }

        return AI(maze,variabili,i,j+1);

        

    } else if((maze[(i+1)*variabili.lunghezza+j] == ' ' || maze[(i+1)*variabili.lunghezza+j] == '$' || maze[(i+1)*variabili.lunghezza+j] == '!' || maze[(i+1)*variabili.lunghezza+j] == 'T') && maze[(i+1)*variabili.lunghezza+j] != 'I' 
&& maze[(i+1)*variabili.lunghezza+j] != '#' && maze[(i+1)*variabili.lunghezza+j] != 'O' && ( i+1 >= 0 && i+1 <= variabili.altezza))

    {

        maze[(i+1)*variabili.lunghezza+j] = 'I';

        if(crossroads(maze,variabili,i+1,j)){

            bivio(maze,variabili,i+1,j);

        }

        return AI(maze,variabili,i+1,j);

        

    } else if((maze[(i-1)*variabili.lunghezza+j] == ' ' || maze[(i-1)*variabili.lunghezza+j] == '$' || maze[(i-1)*variabili.lunghezza+j] != '!' || maze[(i-1)*variabili.lunghezza+j] != 'T') && maze[(i-1)*variabili.lunghezza+j] != 'I' 
&& maze[(i-1)*variabili.lunghezza+j] != '#' && maze[(i-1)*variabili.lunghezza+j] != 'O'  && ( i-1 >= 0 && i-1 <= variabili.altezza))

    {

        maze[(i-1)*variabili.lunghezza+j] = 'I';

        if(crossroads(maze,variabili,i-1,j)){

            bivio(maze,variabili,i-1,j);

        }

        return AI(maze,variabili,i-1,j);

        

    } else if ((maze[i*variabili.lunghezza+j-1] == ' ' || maze[i*variabili.lunghezza+j-1] == '$' || maze[i*variabili.lunghezza+j-1] == '!' || maze[i*variabili.lunghezza+j-1] == 'T') && maze[i*variabili.lunghezza+j-1] != 'I' && 
maze[i*variabili.lunghezza+j-1] != '#' && maze[i*variabili.lunghezza+j-1] != 'O' && (j-1 >= 0 && j-1 <= variabili.lunghezza))

    {

         maze[i*variabili.lunghezza+j-1] = 'I';

         if(crossroads(maze,variabili,i,j-1)){

             bivio(maze,variabili,i,j-1);

         }

        return AI(maze,variabili,i,j-1);

        

    } else return false;



    return false;

}



/**
 * Questa funzione stampa sia le mosse e sia mi calcola il punteggio fatto.
 * @param maze e' la mappa usata
 * @param variabili le variabili usate per l'altezza e lunghezza della mappa
 * @param coo coordinate del nostro snake
 * @param monete la lista di coordinate delle monete
 * @param imprevisti la lista di coordinate delle imprevisti
 * @param punteggio il punteggio attuale
 * @return il punteggio
*/

int stampa_mosse_punteggio(char* maze, mappa variabili, coordinate coo, pCoda* monete, pCoda* imprevisti, int punteggio) {

    bool sopra = false;

    bool sotto = false;

    bool destra = false;

    bool sinistra = false;



    punteggio -= 1;



    /*

    stampo le mosse con questo metodo:

    il nostro percorso è formato da I e parte da P fino alla O

    quindi parto dalle cordinate della P e ad una ad una I controllo se è sotto (W) destra (D) sinistra (A) sopra (S)

    e ad ogni controllo cancello la I appena controllata.

    il while finisce quando non ci sono più I nelle celle limitrofe.

    */



    while(maze[(coo.i-1)*variabili.lunghezza+coo.j] == 'I' || maze[(coo.i+1)*variabili.lunghezza+coo.j] == 'I' || maze[(coo.i)*variabili.lunghezza+coo.j+1] == 'I' || maze[(coo.i)*variabili.lunghezza+coo.j-1] == 'I'  )

    {

        pCoda pc = *monete;

        pCoda pc1 = *imprevisti;



        controllo_bivio_I_H(maze,variabili,coo.i,coo.j,'I');



        while(pc1){

            if(pc1->x == coo.i && pc1->y == coo.j){

                punteggio /= 2;

            }

            pc1 = pc1->next;

        }



        while(pc){

            if(pc->x == coo.i && pc->y == coo.j){

                punteggio += 10;

            }

            pc = pc->next;

        }



        punteggio -= 1;



        if(sopra){

            if(maze[(coo.i-1)*variabili.lunghezza+coo.j] == 'I'){

                 printf(" W ");

                 maze[coo.i*variabili.lunghezza+coo.j] = ' ';

                 coo.i -= 1;

            } else sopra = false;

        }



        if(destra){

            if(maze[coo.i*variabili.lunghezza+coo.j+1] == 'I'){

                 printf(" D ");

                 maze[coo.i*variabili.lunghezza+coo.j] = ' ';

                 coo.j += 1;

            } else destra = false;

        }



        if(sinistra){

            if(maze[coo.i*variabili.lunghezza+coo.j-1] == 'I'){

                printf(" A ");

                maze[coo.i*variabili.lunghezza+coo.j] = ' ';

                coo.j -= 1;

            } else sinistra = false;

        }



        if(sotto){

            if(maze[(coo.i+1)*variabili.lunghezza+coo.j] == 'I'){

                printf(" S ");

                maze[coo.i*variabili.lunghezza+coo.j] = ' ';

                coo.i += 1;

            } else sotto = false;

        }



        if(maze[(coo.i+1)*variabili.lunghezza+coo.j] == 'I' && !sotto){

            printf(" S ");

            maze[coo.i*variabili.lunghezza+coo.j] = ' ';

            coo.i += 1;

            sotto = true;

        }

        

        if(maze[coo.i*variabili.lunghezza+coo.j+1] == 'I' && !destra){

            printf(" D ");

            maze[coo.i*variabili.lunghezza+coo.j] = ' ';

            coo.j += 1;

            destra = true;

        }

        

        if(maze[coo.i*variabili.lunghezza+coo.j-1] == 'I' && !sinistra){

            printf(" A ");

            maze[coo.i*variabili.lunghezza+coo.j] = ' ';

            coo.j -= 1;

            sinistra = true;

        }

        

        if(maze[(coo.i-1)*variabili.lunghezza+coo.j] == 'I' && !sopra){

            printf(" W ");

            maze[coo.i*variabili.lunghezza+coo.j] = ' ';

            coo.i -= 1;

            sopra = true;

        }

    }



    punteggio -= 2;



    //questi if servono per scrivere l'ultima direzione

    if(maze[(coo.i)*variabili.lunghezza+coo.j+1] == '_') printf(" D ");

    if(maze[(coo.i)*variabili.lunghezza+coo.j-1] == '_') printf(" A ");

    if(maze[(coo.i-1)*variabili.lunghezza+coo.j] == '_') printf(" W ");

    if(maze[(coo.i+1)*variabili.lunghezza+coo.j] == '_') printf(" S ");



    printf("\n");

    return punteggio;

}


/**
 * Funzione che cerca le coordinate dell'uscita
 * @param maze e' la mappa usata
 * @param variabili le variabili usate per l'altezza e lunghezza della mappa
 * @param uscita posizione in cui si trova l'uscita all'interno della mappa
 * @return le coordinate dell'uscita
*/
coordinate trova_uscita(char* maze, mappa variabili, coordinate uscita){

    uscita.i = 0;

    uscita.j = 0;



    for(int i = 0; i < variabili.altezza; i++){

        for(int j = 0; j < variabili.lunghezza; j++){

            if(maze[i*variabili.lunghezza+j] == '_'){

                uscita.i = i;

                uscita.j = j;

            }

         }

    }

 return uscita;

}


/**
 * Funzione ausiliaria che aiuta il personaggio a trovare l'uscita
 * @param maze e' la mappa usata
 * @param variabili le variabili usate per l'altezza e lunghezza della mappa
 * @param i altezza mappa
 * @param j larghezza mappa
 * @param a la direzione che la funzione seguira' ricorsivamente
*/
void genera_H(char* maze, mappa variabili, int i, int j, char a){

    /*

    le H nascono in questo modo:

                H

              H O H

                H

    muovo le H con questo criterio:



    se la prima mossa è avanti, allora continuerà andare avanti finchè non troverà un muro. da qui cambia direzione seguendo la stessa direzione fino a che si blocca in qualche modo.

    lo scopo della H e quello di aiutare a trovare facilmente una via verso la nostra O, visto che appena il nostro percorso incontra una H significa che abbiamo trovato una via d'uscita.

    */

    

    if( maze[(i)*variabili.lunghezza+j] == '#' || maze[(i)*variabili.lunghezza+j] == 'O') return;

    

    maze[i*variabili.lunghezza+j] = 'H';

     

     if(a == 'W' && maze[(i-1)*variabili.lunghezza+j] != '#' && maze[(i-1)*variabili.lunghezza+j] != 'O' &&  maze[(i-1)*variabili.lunghezza+j] != '_') return genera_H(maze,variabili,i-1,j,'W');

    

     if(a == 'D' && maze[i*variabili.lunghezza+j+1] != '#' &&   maze[i*variabili.lunghezza+j+1] != 'O'  && maze[i*variabili.lunghezza+j+1] != '_') return genera_H(maze,variabili,i,j+1,'D');

    

     if(a == 'S' && maze[(i+1)*variabili.lunghezza+j] != '#' &&  maze[(i+1)*variabili.lunghezza+j] != 'O' &&  maze[(i+1)*variabili.lunghezza+j] != '_') return genera_H(maze,variabili,i+1,j,'S');

    

     if(a == 'A' && maze[i*variabili.lunghezza+j-1] != '#'  && maze[i*variabili.lunghezza+j-1] != 'O' && maze[i*variabili.lunghezza+j-1] != '_') return

         genera_H(maze,variabili,i,j-1,'A');



    if((maze[i*variabili.lunghezza+j+1] == ' ' || maze[i*variabili.lunghezza+j+1] == '$' || maze[i*variabili.lunghezza+j+1] == '!' || maze[i*variabili.lunghezza+j+1] == 'T') && maze[i*variabili.lunghezza+j+1] != 'H' && 
maze[i*variabili.lunghezza+j+1] != '#' && maze[i*variabili.lunghezza+j+1] != 'O' && maze[i*variabili.lunghezza+j+1] != '_'){

         return genera_H(maze,variabili,i,j+1,'D');

    }

    else if((maze[(i+1)*variabili.lunghezza+j] == ' ' || maze[(i+1)*variabili.lunghezza+j] == '$' || maze[(i+1)*variabili.lunghezza+j] == '!' || maze[(i+1)*variabili.lunghezza+j] == 'T' ) && maze[(i+1)*variabili.lunghezza+j] != 'H' 
&& maze[(i+1)*variabili.lunghezza+j] != '#' && maze[(i+1)*variabili.lunghezza+j] != 'O' && maze[(i+1)*variabili.lunghezza+j] != '_'){

        return genera_H(maze,variabili,i+1,j,'S');

    }

    else if((maze[(i-1)*variabili.lunghezza+j] == ' ' || maze[(i-1)*variabili.lunghezza+j] == '$' || maze[(i-1)*variabili.lunghezza+j] == '!' || maze[(i-1)*variabili.lunghezza+j] == 'T') && maze[(i-1)*variabili.lunghezza+j] != 'H' 
&& maze[(i-1)*variabili.lunghezza+j] != '#' && maze[(i-1)*variabili.lunghezza+j] != 'O' && maze[(i-1)*variabili.lunghezza+j] != '_'){

         return genera_H(maze,variabili,i-1,j,'W');

    }

    else if ((maze[i*variabili.lunghezza+j-1] == ' ' || maze[i*variabili.lunghezza+j-1] == '$' || maze[i*variabili.lunghezza+j-1] == '!' || maze[i*variabili.lunghezza+j-1] == 'T') && maze[i*variabili.lunghezza+j-1] != 'H' && 
maze[i*variabili.lunghezza+j-1] != '#' && maze[i*variabili.lunghezza+j-1] != 'O' && maze[i*variabili.lunghezza+j-1] != '_' ){

         return genera_H(maze,variabili,i,j-1,'A');

    } else return;

}


/**
 * Funzione che mi cancella tutti i simboli nella mappa tranne per l'uscita e lo snake
 * @param maze e' la mappa usata
 * @param variabili le variabili usate per l'altezza e lunghezza della mappa
*/
void distruggi_tutto(char* maze,mappa variabili){

    for(int i = 0; i < variabili.altezza; i++){

        for(int j = 0; j < variabili.lunghezza; j++){

            if(maze[i*variabili.lunghezza+j] == 'S' || maze[i*variabili.lunghezza+j] == 'U' || maze[i*variabili.lunghezza+j] == 'D' || maze[i*variabili.lunghezza+j] == 'L' || maze[i*variabili.lunghezza+j] == 'H'){

               maze[i*variabili.lunghezza+j] = ' ';

            }

         }

    }

}


/**
 * Funzione che mi cerca le coordinate del simbolo
 * @param maze e' la mappa usata
 * @param variabili le variabili usate per l'altezza e lunghezza della mappa
 * @param simbolo il simbolo che sto cercando
 * @return la lista con le coordinate del simbolo
*/
pCoda lista_simboli(char* maze, mappa variabili, char simbolo){

    //cerca le cordinate del simbolo

    pCoda pc = NULL;

    for(int i = 0; i < variabili.altezza; i++){

        for(int j = 0; j < variabili.lunghezza;j++){

            if(maze[i*variabili.lunghezza+j] == simbolo){

                push_front(i,j,&pc);

            }

        }

    }

    return pc;

}

/**
 * Modalita' AI, se riesce a trovare un'uscita stampa le mosse e il punteggio
 * @param maze e' la mappa usata
 * @param altezza altezza della mappa
 * @param lunghezza lunghezza della mappa
*/
void modalità_AI(char* maze, int altezza, int lunghezza) {

    mappa variabili;

    coordinate coo = {0,0};

    coordinate uscita = {0,0};

    pCoda monete = NULL;

    pCoda imprevisti = NULL;

    int punteggio = 1000;

    

    variabili.altezza = altezza;

    variabili.lunghezza = lunghezza;



    uscita = trova_uscita(maze,variabili,uscita); // metto le coordinate di dove si trova la nostra O

    monete = lista_simboli(maze,variabili,'$'); // salvo in una lista le coordinate di $

    imprevisti = lista_simboli(maze,variabili,'!'); // salvo in una lista le coordinate di !



    //muovo le H

    if(uscita.i-2 > 0 && uscita.i-1 < altezza) genera_H(maze,variabili,uscita.i-1,uscita.j,'W');

    if(uscita.i+2 > 0 && uscita.i+1 < altezza) genera_H(maze,variabili,uscita.i+1,uscita.j,'S');

    if(uscita.j+2 > 0 && uscita.j+1 < lunghezza) genera_H(maze,variabili,uscita.i,uscita.j+1,'D');

    if(uscita.j-2 > 0 && uscita.j-1 < lunghezza)genera_H(maze,variabili,uscita.i,uscita.j-1,'A');



    coo = personaggio(maze,variabili,coo);



    stampa(maze,variabili);

   

    if(!AI(maze,variabili,coo.i, coo.j)){

        distruggi_tutto(maze,variabili);

        printf("Non trovo una soluzione\n");

    } else {

        distruggi_tutto(maze,variabili);

        

        stampa(maze,variabili);

        printf("Le mosse sono: ");

        

        //la funzione stampa_mosse calcola anche il punteggio

        punteggio = stampa_mosse_punteggio(maze,variabili,coo,&monete,&imprevisti,punteggio);

        if(punteggio < 0) printf("La soluzione trovata ha un punteggio inferiore a 0!\n");

        else printf("\n\t\t[punteggio: %d]\n\n", punteggio);

    }

    libera(&monete);

    libera(&imprevisti);

}

/*

--------------------------------------------------------------------------------------------------------------------------------------------

                                                    MENU

-----------------------------------------------------------------------------------------------------------------------------------------

*/

/**
 * Funzione che mi crea una mappa 
 * @param altezza altezza della mappa
 * @param lunghezza lunghezza della mappa
 * @return ritorna la mappa allocata dinamicamente
*/

char* malloc2darray(int altezza, int lunghezza){

    char* maze;

    maze = (char*)malloc((altezza*lunghezza)*sizeof(char));



    return maze;

}


/**
 * Funzione che mi consente di inserire l'altezza e la larghezza del labirinto
 * @param maze e' la mappa usata
 * @param altezza variabile usata per l'altezza
 * @param lunghezza variabile usata per la larghezza
*/
void scrivi_maze(char* maze,int altezza, int lunghezza){

    for(int i = 0; i < altezza; i++){

        char *s = (char*) malloc(lunghezza+1);

        scanf(" %[^\n]s", s);

        for(int j = 0; j < lunghezza; j++){

            maze[i*lunghezza+j] = s[j];

        }

         free(s);

    }

}

/**
 * Funzione che mi libera dalla memoria dinamica la mappa che abbiamo appena utilizzato
 * @param maze e' la mappa usata

*/

void free_maze(char* maze){

    free(maze);

}

/**
 * Funzione che chiede all'utente quale modalita' vuole usare e se vuole usare la mappa standard o inserirla.
*/

void menu(void){

    int scelta_modalità = 0;

    int scelta_mappa = 0;

    int lunghezza = 0;

    int altezza = 0;

    do {
        printf("Scegli la modalita': \n \t [1 Modalita' interattiva] [2 Modalita' AI]\n");
        
        char input[100];
        scanf("%s", input);
        
        scelta_modalità = atoi(input);

        if (scelta_modalità != 1 && scelta_modalità != 2) {
            printf("\nModalita' non valida\n\n");
            
            while (getchar() != '\n');
        }
    } while (scelta_modalità != 1 && scelta_modalità != 2);



    do{

        printf(" Vuoi usare una mappa standard o inserirla? \n \t [1 Inserisci la mappa] [2 Mappa standard]\n");

        char input[100];
        scanf("%s", input);

        scelta_mappa = atoi(input);

        

        if((scelta_mappa != 1 && scelta_mappa != 2)){

            printf("\n Scelta non valida \n\n");
            
            while (getchar() != '\n');
        }

    } while(scelta_mappa != 1 && scelta_mappa != 2);



    if(scelta_mappa == 2){

        if(scelta_modalità == 1 ) modalita_interattiva(*maze_global, 7, 10);

        else modalità_AI(*maze_global, 7, 10);

        

        return;

    } else {

        do{

            printf("\n Quanto e' larga la mappa? \n");

            scanf("%d", &lunghezza);

            printf("\n Quanto e' alta la mappa? \n");

            scanf("%d", &altezza);



            if(lunghezza < 0 || altezza < 0) printf("\n Dati non validi \n\n");

        } while(lunghezza < 0 || altezza < 0 );



        char* maze_;

        maze_ = malloc2darray(altezza, lunghezza);



        printf("\n Inserisci la mappa: \n");

        

        scrivi_maze(maze_,altezza,lunghezza);



        if(scelta_modalità == 1 ) modalita_interattiva(maze_,altezza,lunghezza);

        else modalità_AI(maze_,altezza,lunghezza);



        free_maze(maze_);

        return;

    }

}
