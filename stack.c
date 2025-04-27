#include "header.h"

void createEmptyStack(Stack *S) {
    (*S).TOP = NULL;
    (*S).BOTTOM = NULL;
}

addressStack alokasiStack(Aktivitas aktivitas) {
    addressStack P = (addressStack)malloc(sizeof(ElmtStack));
    if (P != NULL) {
        P->info = aktivitas;
        P->next = NULL;
        P->prev = NULL;
    }
    return P;
}

int isEmptyStack(Stack S) {
    return (S.TOP == NULL && S.BOTTOM == NULL);
}

void push(Stack *S, addressStack P) {
    if (isEmptyStack(*S)) {
        // Jika stack kosong
        (*S).TOP = P;
        (*S).BOTTOM = P;
    } else {
        // Jika stack tidak kosong
        P->next = (*S).TOP;
        (*S).TOP->prev = P;
        (*S).TOP = P;
    }
}

void pop(Stack *S, addressStack *P) {
    if (!isEmptyStack(*S)) {
        *P = (*S).TOP;
        
        if ((*S).TOP == (*S).BOTTOM) {
            // Jika hanya ada satu elemen
            (*S).TOP = NULL;
            (*S).BOTTOM = NULL;
        } else {
            // Jika ada lebih dari satu elemen
            (*S).TOP = (*S).TOP->next;
            (*S).TOP->prev = NULL;
        }
        
        (*P)->next = NULL;
        (*P)->prev = NULL;
    } else {
        *P = NULL;
    }
}

void printInfoStack(Stack S) {
    addressStack P = S.TOP;
    
    if (isEmptyStack(S)) {
        printf("Stack kosong\n");
    } else {
        printf("\n=== HISTORY AKTIVITAS ===\n");
        int i = 1;
        
        while (P != NULL) {
            char tipeAktivitasStr[20];
            char tipePriorityStr[20];
            
            switch (P->info.tipeAktivitas) {
                case 1:
                    strcpy(tipeAktivitasStr, "Pinjam");
                    break;
                case 2:
                    strcpy(tipeAktivitasStr, "Kembalikan");
                    break;
                case 3:
                    strcpy(tipeAktivitasStr, "Batalkan");
                    break;
                default:
                    strcpy(tipeAktivitasStr, "Unknown");
            }
            
            switch (P->info.tipePriority) {
                case DOSEN:
                    strcpy(tipePriorityStr, "Dosen");
                    break;
                case MAHASISWA:
                    strcpy(tipePriorityStr, "Mahasiswa");
                    break;
                case MASYARAKAT_UMUM:
                    strcpy(tipePriorityStr, "Masyarakat Umum");
                    break;
                default:
                    strcpy(tipePriorityStr, "Unknown");
            }
            
            printf("%d. %s oleh '%s' (%s) - Buku: '%s'\n", 
                   i, tipeAktivitasStr, P->info.namaPeminjam, 
                   tipePriorityStr, P->info.judulBuku);
            
            P = P->next;
            i++;
        }
        printf("\n");
    }
}

void printInfoStackReverse(Stack S) {
    addressStack P = S.BOTTOM;
    
    if (isEmptyStack(S)) {
        printf("Stack kosong\n");
    } else {
        printf("\n=== HISTORY AKTIVITAS (TERBALIK) ===\n");
        int i = 1;
        
        while (P != NULL) {
            char tipeAktivitasStr[20];
            char tipePriorityStr[20];
            
            switch (P->info.tipeAktivitas) {
                case 1:
                    strcpy(tipeAktivitasStr, "Pinjam");
                    break;
                case 2:
                    strcpy(tipeAktivitasStr, "Kembalikan");
                    break;
                case 3:
                    strcpy(tipeAktivitasStr, "Batalkan");
                    break;
                default:
                    strcpy(tipeAktivitasStr, "Unknown");
            }
            
            switch (P->info.tipePriority) {
                case DOSEN:
                    strcpy(tipePriorityStr, "Dosen");
                    break;
                case MAHASISWA:
                    strcpy(tipePriorityStr, "Mahasiswa");
                    break;
                case MASYARAKAT_UMUM:
                    strcpy(tipePriorityStr, "Masyarakat Umum");
                    break;
                default:
                    strcpy(tipePriorityStr, "Unknown");
            }
            
            printf("%d. %s oleh '%s' (%s) - Buku: '%s'\n", 
                   i, tipeAktivitasStr, P->info.namaPeminjam, 
                   tipePriorityStr, P->info.judulBuku);
            
            P = P->prev;
            i++;
        }
        printf("\n");
    }
}
