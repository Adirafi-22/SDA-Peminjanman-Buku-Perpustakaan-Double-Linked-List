#include "header.h"

// Fungsi untuk membersihkan layar konsol
void clearScreen() {
    #ifdef _WIN32
        system("cls");  // Untuk Windows
    #else
        system("clear"); // Untuk Linux/Unix/macOS
    #endif
}

// Implementasi fungsi-fungsi operasi perpustakaan yang belum ada
void tambahBuku(List *L, char judul[], int stok) {
    addressList P = alokasi(judul, stok);
    if (P != NULL) {
        insertLast(L, P);
        printf("Buku '%s' berhasil ditambahkan dengan stok %d.\n", judul, stok);
    } else {
        printf("Gagal menambahkan buku.\n");
    }
}

void tambahStok(List *L, char judul[], int tambahan) {
    addressList P = findElm(*L, judul);
    if (P != NULL) {
        P->stok += tambahan;
        printf("Stok buku '%s' berhasil ditambah menjadi %d.\n", judul, P->stok);
    } else {
        printf("Buku '%s' tidak ditemukan.\n", judul);
    }
}

void pinjamBuku(List *L, Stack *S, char judul[], char nama[], int priority) {
    addressList buku = findElm(*L, judul);
    if (buku == NULL) {
        printf("Buku '%s' tidak ditemukan.\n", judul);
        return;
    }

    // Tambahkan ke antrian peminjam
    addressQueue peminjam = alokasiQueue(nama, priority);
    if (peminjam != NULL) {
        enqueue(&(buku->antrianPeminjam), peminjam);
        printf("Anda telah masuk antrian untuk meminjam buku '%s'.\n", judul);
        
        // Catat aktivitas di stack
        Aktivitas aktivitas;
        strcpy(aktivitas.namaPeminjam, nama);
        strcpy(aktivitas.judulBuku, judul);
        aktivitas.tipePriority = priority;
        aktivitas.tipeAktivitas = 1; // 1: Pinjam
        
        addressStack P = alokasiStack(aktivitas);
        if (P != NULL) {
            push(S, P);
        }
    } else {
        printf("Gagal menambahkan ke antrian peminjam.\n");
    }
}

void kembalikanBuku(List *L, Stack *S, char judul[], char nama[]) {
    addressList buku = findElm(*L, judul);
    if (buku == NULL) {
        printf("Buku '%s' tidak ditemukan.\n", judul);
        return;
    }

    // Tambahkan stok buku
    buku->stok++;
    printf("Buku '%s' berhasil dikembalikan oleh '%s'. Stok sekarang: %d\n", judul, nama, buku->stok);
    
    // Catat aktivitas di stack
    Aktivitas aktivitas;
    strcpy(aktivitas.namaPeminjam, nama);
    strcpy(aktivitas.judulBuku, judul);
    aktivitas.tipePriority = 0; // Tidak relevan untuk pengembalian
    aktivitas.tipeAktivitas = 2; // 2: Kembalikan
    
    addressStack P = alokasiStack(aktivitas);
    if (P != NULL) {
        push(S, P);
    }
}

void batalkanAktivitas(List *L, Stack *S) {
    addressStack P;
    pop(S, &P);
    
    if (P == NULL) {
        printf("Tidak ada aktivitas untuk dibatalkan.\n");
        return;
    }
    
    printf("Membatalkan aktivitas terakhir...\n");
    
    // Handle pembatalan berdasarkan jenis aktivitas
    switch (P->info.tipeAktivitas) {
        case 1: // Batalkan peminjaman
            {
                addressList buku = findElm(*L, P->info.judulBuku);
                if (buku != NULL) {
                    // Hapus peminjam dari antrian (harusnya yang terakhir ditambahkan)
                    addressQueue peminjam;
                    if (!isEmptyQueue(buku->antrianPeminjam)) {
                        // Ini penyederhanaan - idealnya cari peminjam yang sesuai
                        dequeue(&(buku->antrianPeminjam), &peminjam);
                        if (peminjam != NULL) {
                            free(peminjam);
                        }
                    }
                }
                printf("Peminjaman buku '%s' oleh '%s' dibatalkan.\n", 
                       P->info.judulBuku, P->info.namaPeminjam);
            }
            break;
            
        case 2: // Batalkan pengembalian
            {
                addressList buku = findElm(*L, P->info.judulBuku);
                if (buku != NULL && buku->stok > 0) {
                    buku->stok--;
                    printf("Pengembalian buku '%s' oleh '%s' dibatalkan.\n", 
                           P->info.judulBuku, P->info.namaPeminjam);
                }
            }
            break;
            
        default:
            printf("Aktivitas tidak dikenal.\n");
    }
    
    free(P);
}

void prosesPeminjaman(List *L, Stack *S, char judul[]) {
    addressList buku = findElm(*L, judul);
    if (buku == NULL) {
        printf("Buku '%s' tidak ditemukan.\n", judul);
        return;
    }

    if (buku->stok <= 0) {
        printf("Stok buku '%s' habis. Tidak bisa memproses peminjaman.\n", judul);
        return;
    }

    if (isEmptyQueue(buku->antrianPeminjam)) {
        printf("Tidak ada antrian peminjam untuk buku '%s'.\n", judul);
        return;
    }

    // Ambil peminjam dengan prioritas tertinggi
    addressQueue peminjam;
    dequeue(&(buku->antrianPeminjam), &peminjam);
    
    if (peminjam != NULL) {
        // Kurangi stok buku
        buku->stok--;
        
        printf("Peminjaman buku '%s' oleh '%s' berhasil diproses. Stok tersisa: %d\n", 
               judul, peminjam->nama, buku->stok);
        
        // Catat aktivitas di stack (jika perlu)
        Aktivitas aktivitas;
        strcpy(aktivitas.namaPeminjam, peminjam->nama);
        strcpy(aktivitas.judulBuku, judul);
        aktivitas.tipePriority = peminjam->priority;
        aktivitas.tipeAktivitas = 1; // 1: Pinjam (proses)
        
        addressStack P = alokasiStack(aktivitas);
        if (P != NULL) {
            push(S, P);
        }
        
        free(peminjam);
    }
}

int main() {
    List daftarBuku;
    Stack historyAktivitas;
    int pilihan, stok, priority;
    char judul[50], nama[50];
    
    createEmpty(&daftarBuku);
    createEmptyStack(&historyAktivitas);
    
    do {
        printf("\n=============================================\n");
        printf("    SISTEM PEMINJAMAN BUKU PERPUSTAKAAN\n");
        printf("=============================================\n");
        printf("1. Tambah Buku\n");
        printf("2. Tambah Stok\n");
        printf("3. Pinjam Buku\n");
        printf("4. Proses Peminjaman\n");
        printf("5. Kembalikan Buku\n");
        printf("6. Batalkan Aktivitas Terakhir\n");
        printf("7. Lihat Data Buku\n");
        printf("8. Lihat Data Buku (Reverse)\n");
        printf("9. Lihat History Aktivitas\n");
        printf("10. Lihat History Aktivitas (Reverse)\n");
        printf("11. Lihat Antrian Peminjam\n");
        printf("0. Keluar\n");
        printf("Pilihan Anda: ");
        scanf("%d", &pilihan);
        getchar(); // clear buffer
        
        switch (pilihan) {
            case 1: // Tambah Buku
                clearScreen();
                printf("\n--- TAMBAH BUKU ---\n");
                printf("Judul buku: ");
                fgets(judul, 50, stdin);
                judul[strcspn(judul, "\n")] = 0; // hapus newline
                
                printf("Stok: ");
                scanf("%d", &stok);
                getchar(); // clear buffer
                
                tambahBuku(&daftarBuku, judul, stok);
                printf("Tekan Enter untuk melanjutkan...");
                getchar();
                break;
                
            case 2: // Tambah Stok
                clearScreen();
                printf("\n--- TAMBAH STOK ---\n");
                printf("Judul buku: ");
                fgets(judul, 50, stdin);
                judul[strcspn(judul, "\n")] = 0; // hapus newline
                
                printf("Tambahan stok: ");
                scanf("%d", &stok);
                getchar(); // clear buffer
                
                tambahStok(&daftarBuku, judul, stok);
                printf("Tekan Enter untuk melanjutkan...");
                getchar();
                break;
                
            case 3: // Pinjam Buku
                clearScreen();
                printf("\n--- PINJAM BUKU ---\n");
                printf("Judul buku: ");
                fgets(judul, 50, stdin);
                judul[strcspn(judul, "\n")] = 0; // hapus newline
                
                printf("Nama peminjam: ");
                fgets(nama, 50, stdin);
                nama[strcspn(nama, "\n")] = 0; // hapus newline
                
                printf("Prioritas (1: Masyarakat Umum, 2: Mahasiswa, 3: Dosen): ");
                scanf("%d", &priority);
                getchar(); // clear buffer
                
                pinjamBuku(&daftarBuku, &historyAktivitas, judul, nama, priority);
                printf("Tekan Enter untuk melanjutkan...");
                getchar();
                break;
                
            case 4: // Proses Peminjaman
                clearScreen();
                printf("\n--- PROSES PEMINJAMAN ---\n");
                printf("Judul buku yang akan diproses: ");
                fgets(judul, 50, stdin);
                judul[strcspn(judul, "\n")] = 0; // hapus newline
                
                prosesPeminjaman(&daftarBuku, &historyAktivitas, judul);
                printf("Tekan Enter untuk melanjutkan...");
                getchar();
                break;
                
            case 5: // Kembalikan Buku
                clearScreen();
                printf("\n--- KEMBALIKAN BUKU ---\n");
                printf("Judul buku: ");
                fgets(judul, 50, stdin);
                judul[strcspn(judul, "\n")] = 0; // hapus newline
                
                printf("Nama peminjam: ");
                fgets(nama, 50, stdin);
                nama[strcspn(nama, "\n")] = 0; // hapus newline
                
                kembalikanBuku(&daftarBuku, &historyAktivitas, judul, nama);
                printf("Tekan Enter untuk melanjutkan...");
                getchar();
                break;
                
            case 6: // Batalkan Aktivitas Terakhir
                clearScreen();
                printf("\n--- BATALKAN AKTIVITAS TERAKHIR ---\n");
                batalkanAktivitas(&daftarBuku, &historyAktivitas);
                printf("Tekan Enter untuk melanjutkan...");
                getchar();
                break;
                
            case 7: // Lihat Data Buku
                clearScreen();
                printInfo(daftarBuku);
                printf("Tekan Enter untuk melanjutkan...");
                getchar();
                break;
                
            case 8: // Lihat Data Buku (Reverse)
                clearScreen();
                printInfoReverse(daftarBuku);
                printf("Tekan Enter untuk melanjutkan...");
                getchar();
                break;
                
            case 9: // Lihat History Aktivitas
                clearScreen();
                printInfoStack(historyAktivitas);
                printf("Tekan Enter untuk melanjutkan...");
                getchar();
                break;
                
            case 10: // Lihat History Aktivitas (Reverse)
                clearScreen();
                printInfoStackReverse(historyAktivitas);
                printf("Tekan Enter untuk melanjutkan...");
                getchar();
                break;
                
            case 11: // Lihat Antrian Peminjam
                clearScreen();
                printf("\n--- LIHAT ANTRIAN PEMINJAM ---\n");
                printf("Judul buku: ");
                fgets(judul, 50, stdin);
                judul[strcspn(judul, "\n")] = 0; // hapus newline
                
                addressList P = findElm(daftarBuku, judul);
                if (P != NULL) {
                    printf("Antrian peminjam untuk buku '%s':\n", judul);
                    printInfoQueue(P->antrianPeminjam);
                    
                    printf("\nAntrian peminjam (reverse):\n");
                    printInfoQueueReverse(P->antrianPeminjam);
                } else {
                    printf("Buku '%s' tidak ditemukan.\n", judul);
                }
                printf("Tekan Enter untuk melanjutkan...");
                getchar();
                break;
                
            case 0: // Keluar
                clearScreen();
                printf("Terima kasih telah menggunakan program ini.\n");
                break;
                
            default:
                printf("Pilihan tidak valid.\n");
                printf("Tekan Enter untuk melanjutkan...");
                getchar();
        }
        
        clearScreen();
    } while (pilihan != 0);
    
    return 0;
}
