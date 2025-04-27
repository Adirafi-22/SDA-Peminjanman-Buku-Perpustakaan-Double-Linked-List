#include "header.h"

void createEmpty(List *L) {
    (*L).FIRST = NULL;
    (*L).LAST = NULL;
}

addressList alokasi(char judul[], int stok) {
    addressList P = (addressList)malloc(sizeof(ElmtList));
    if (P != NULL) {
        strcpy(P->judul, judul);
        P->stok = stok;
        createEmptyQueue(&(P->antrianPeminjam));
        P->next = NULL;
        P->prev = NULL;
    }
    return P;
}

void insertFirst(List *L, addressList P) {
    if ((*L).FIRST == NULL) {
        // Jika list kosong
        (*L).FIRST = P;
        (*L).LAST = P;
    } else {
        // Jika list tidak kosong
        P->next = (*L).FIRST;
        (*L).FIRST->prev = P;
        (*L).FIRST = P;
    }
}

void insertLast(List *L, addressList P) {
    if ((*L).FIRST == NULL) {
        // Jika list kosong
        insertFirst(L, P);
    } else {
        // Jika list tidak kosong
        (*L).LAST->next = P;
        P->prev = (*L).LAST;
        (*L).LAST = P;
    }
}

void insertAfter(addressList prec, addressList P) {
    if (prec != NULL) {
        if (prec->next == NULL) {
            // Jika prec adalah elemen terakhir
            P->next = NULL;
            P->prev = prec;
            prec->next = P;
        } else {
            // Jika prec bukan elemen terakhir
            P->next = prec->next;
            P->prev = prec;
            prec->next->prev = P;
            prec->next = P;
        }
    }
}

void deleteFirst(List *L, addressList *P) {
    if ((*L).FIRST != NULL) {
        *P = (*L).FIRST;
        
        if ((*L).FIRST == (*L).LAST) {
            // Jika hanya ada satu elemen
            (*L).FIRST = NULL;
            (*L).LAST = NULL;
        } else {
            // Jika ada lebih dari satu elemen
            (*L).FIRST = (*L).FIRST->next;
            (*L).FIRST->prev = NULL;
            (*P)->next = NULL;
        }
    } else {
        *P = NULL;
    }
}

void deleteLast(List *L, addressList *P) {
    if ((*L).FIRST != NULL) {
        *P = (*L).LAST;
        
        if ((*L).FIRST == (*L).LAST) {
            // Jika hanya ada satu elemen
            (*L).FIRST = NULL;
            (*L).LAST = NULL;
        } else {
            // Jika ada lebih dari satu elemen
            (*L).LAST = (*L).LAST->prev;
            (*L).LAST->next = NULL;
            (*P)->prev = NULL;
        }
    } else {
        *P = NULL;
    }
}

void deleteAfter(addressList prec, addressList *P) {
    if (prec != NULL && prec->next != NULL) {
        *P = prec->next;
        
        if (prec->next == (*P)->next) {
            // Jika elemen yang dihapus adalah elemen terakhir
            prec->next = NULL;
        } else {
            // Jika elemen yang dihapus bukan elemen terakhir
            prec->next = (*P)->next;
            (*P)->next->prev = prec;
            (*P)->next = NULL;
        }
        (*P)->prev = NULL;
    } else {
        *P = NULL;
    }
}

addressList findElm(List L, char judul[]) {
    addressList P = L.FIRST;
    while (P != NULL) {
        if (strcmp(P->judul, judul) == 0) {
            return P;
        }
        P = P->next;
    }
    return NULL;
}

void printInfo(List L) {
    addressList P = L.FIRST;
    if (P == NULL) {
        printf("List kosong\n");
    } else {
        printf("\n=== DAFTAR BUKU ===\n");
        printf("%-30s | %s\n", "Judul Buku", "Stok");
        printf("-----------------------------------\n");
        while (P != NULL) {
            printf("%-30s | %d\n", P->judul, P->stok);
            P = P->next;
        }
        printf("\n");
    }
}

void printInfoReverse(List L) {
    addressList P = L.LAST;
    if (P == NULL) {
        printf("List kosong\n");
    } else {
        printf("\n=== DAFTAR BUKU (TERBALIK) ===\n");
        printf("%-30s | %s\n", "Judul Buku", "Stok");
        printf("-----------------------------------\n");
        while (P != NULL) {
            printf("%-30s | %d\n", P->judul, P->stok);
            P = P->prev;
        }
        printf("\n");
    }
}
