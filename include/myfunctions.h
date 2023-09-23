#ifndef CODA_H
#define CODA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>
#include <assert.h>
#include <limits.h>

typedef struct mappa {
    int altezza;
    int lunghezza;
    char direzione;
} mappa;

typedef struct coordinate {
    int i; // coordinata altezza
    int j; // coordinata lunghezza
} coordinate;

typedef struct list {
  int x;
  int y;
  struct list *next;
  struct list *prev;
} coda;

typedef coda* pCoda;


pCoda new_node(int, int);
void l_append(pCoda*, int, int);
void push_front(int, int, pCoda*);
void pop_back(pCoda*);
void libera(pCoda*);

int conta_coda(pCoda*);
int taglia_coda(pCoda*, int);
int mangia_coda(pCoda*, coordinate, int);
void switch_positions(pCoda*, coordinate);
void print_coda(char*, mappa, pCoda*);

void stampa (char*, mappa);
bool win (char*, mappa);
bool moneta(char*, mappa, coordinate);
bool imprevisto(char*, mappa, coordinate);
bool muro(char*, mappa, coordinate);
bool trapano(char*, mappa, coordinate);

coordinate personaggio(char*, mappa, coordinate);
void nuova_posizione(char*, mappa, coordinate);
bool collisione(coordinate, mappa);
coordinate leggi_comandi(char*, coordinate, mappa, bool);

void Refresh(char*, mappa, coordinate);
void refresh_coda(char*, mappa, pCoda*);
void modalita_interattiva(char*, int, int);

bool controllo_SOPRA(char*, mappa, int, int, char);
bool controllo_SOTTO(char*, mappa, int, int, char);
bool controllo_DESTRA(char*, mappa, int, int, char);
bool controllo_SINISTRA(char*, mappa, int, int, char);

void distruggi(char*, mappa, int, int, char);
bool crossroads(char*, mappa, int, int);
bool bivio(char*, mappa, int, int); 

bool spezza_destra(char*, mappa, int, int, char);
bool spezza_sinistra(char*, mappa, int, int, char);
bool spezza_sopra(char*, mappa, int, int, char);
bool spezza_sotto(char*, mappa, int, int, char);

void ripristina(char*, mappa, char);
void controllo_bivio_I_H(char*, mappa, int, int, char);
bool fuori_mappa(int, int, mappa);
bool AI(char*, mappa, int, int);

int stampa_mosse_punteggio(char*, mappa, coordinate, pCoda*, pCoda*, int); 
coordinate trova_uscita(char*, mappa, coordinate);
void genera_H(char*, mappa, int, int, char);
void distruggi_tutto(char*, mappa);
pCoda lista_simboli(char*, mappa, char);
void modalità_AI(char*, int, int); 
char* malloc2darray(int, int);
void scrivi_maze(char*, int, int);
void free_maze(char*);

void menu(void);
  
#endif // CODA_H
