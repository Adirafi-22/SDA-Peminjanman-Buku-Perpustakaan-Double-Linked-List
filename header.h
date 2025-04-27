#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definisi konstanta prioritas
#define MASYARAKAT_UMUM 1
#define MAHASISWA 2
#define DOSEN 3
void clearScreen();

// Struktur data untuk aktivitas (digunakan di stack)
typedef struct {
    char namaPeminjam[50];
    char judulBuku[50];
    int tipePriority;
    int tipeAktivitas; // 1: Pinjam, 2: Kembalikan, 3: Batalkan
} Aktivitas;

// Struktur data untuk pengguna/peminjam (digunakan di queue)
typedef struct ElmtQueue *addressQueue;
typedef struct ElmtQueue {
    char nama[50];
    int priority;
    addressQueue next;
    addressQueue prev; // Tambahan untuk double linked list
} ElmtQueue;

// Struktur data untuk antrian dengan prioritas
typedef struct {
    addressQueue HEAD;
    addressQueue TAIL;
} Queue;

// Struktur data untuk stack
typedef struct ElmtStack *addressStack;
typedef struct ElmtStack {
    Aktivitas info;
    addressStack next;
    addressStack prev; // Tambahan untuk double linked list
} ElmtStack;

typedef struct {
    addressStack TOP;
    addressStack BOTTOM; // Tambahan untuk double linked list
} Stack;

// Struktur data untuk buku (node dalam linked list)
typedef struct ElmtList *addressList;
typedef struct ElmtList {
    char judul[50];
    int stok;
    Queue antrianPeminjam;
    addressList next;
    addressList prev; // Tambahan untuk double linked list
} ElmtList;

// Struktur data untuk linked list
typedef struct {
    addressList FIRST;
    addressList LAST; // Tambahan untuk double linked list
} List;

// Deklarasi fungsi untuk List
void createEmpty(List *L);
addressList alokasi(char judul[], int stok);
void insertFirst(List *L, addressList P);
void insertLast(List *L, addressList P); // Tambahan untuk double linked list
void insertAfter(addressList prec, addressList P); // Tambahan untuk double linked list
void deleteFirst(List *L, addressList *P);
void deleteLast(List *L, addressList *P); // Tambahan untuk double linked list
void deleteAfter(addressList prec, addressList *P); // Tambahan untuk double linked list
addressList findElm(List L, char judul[]);
void printInfo(List L);
void printInfoReverse(List L); // Tambahan untuk double linked list

// Deklarasi fungsi untuk Queue
void createEmptyQueue(Queue *Q);
addressQueue alokasiQueue(char nama[], int priority);
int isEmptyQueue(Queue Q);
void enqueue(Queue *Q, addressQueue P);
void dequeue(Queue *Q, addressQueue *P);
void printInfoQueue(Queue Q);
void printInfoQueueReverse(Queue Q); // Tambahan untuk double linked list

// Deklarasi fungsi untuk Stack
void createEmptyStack(Stack *S);
addressStack alokasiStack(Aktivitas aktivitas);
int isEmptyStack(Stack S);
void push(Stack *S, addressStack P);
void pop(Stack *S, addressStack *P);
void printInfoStack(Stack S);
void printInfoStackReverse(Stack S); // Tambahan untuk double linked list

// Deklarasi fungsi untuk operasi perpustakaan
void tambahBuku(List *L, char judul[], int stok);
void tambahStok(List *L, char judul[], int tambahan);
void pinjamBuku(List *L, Stack *S, char judul[], char nama[], int priority);
void kembalikanBuku(List *L, Stack *S, char judul[], char nama[]);
void batalkanAktivitas(List *L, Stack *S);
void prosesPeminjaman(List *L, Stack *S, char judul[]);

#endif
