#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
using namespace std;

/* =====================================================
   STRUCT KTP (LINKED LIST)
===================================================== */
struct KTP {
    string nik, nama, alamat, jk;
};

struct Produk {
    string kode, nama;
    int harga, stok;
    Produk* next;
};

struct Keranjang {
    string nama;
    int qty, subtotal;
    Keranjang* next;
};

struct Stack {
    string info;
    Stack* next;
};

/* =====================================================
   GLOBAL POINTER
===================================================== */
Produk* headProduk = NULL;
Keranjang* headKeranjang = NULL;
Stack* topStack = NULL;

/* =====================================================
   STACK - RIWAYAT TRANSAKSI (OUTPUT ADMIN)
===================================================== */
void push(string x) {
    topStack = new Stack{x, topStack};
}

void tampilRiwayat() {
    cout << "=====================================\n";
    cout << "        RIWAYAT TRANSAKSI ADMIN       \n";
    cout << "=====================================\n";
    if (!topStack) {
        cout << "Belum ada transaksi\n";
        return;
    }
    Stack* t = topStack;
    int no = 1;
    while (t) {
        cout << no++ << ". " << t->info << endl;
        t = t->next;
    }
}

/* =====================================================
   PRODUK - LINKED LIST DINAMIS
===================================================== */
void tambahProduk() {
    Produk* b = new Produk;
    cout << "Kode Produk : "; cin >> b->kode;
    cin.ignore();
    cout << "Nama Produk : "; getline(cin, b->nama);
    cout << "Harga       : "; cin >> b->harga;
    cout << "Stok        : "; cin >> b->stok;
    b->next = NULL;

    if (!headProduk) headProduk = b;
    else {
        Produk* t = headProduk;
        while (t->next) t = t->next;
        t->next = b;
    }
    cout << "Produk berhasil ditambahkan\n";
}

void tampilProduk() {
    cout << "=====================================\n";
    cout << "            DAFTAR PRODUK             \n";
    cout << "=====================================\n";
    if (!headProduk) {
        cout << "Produk kosong\n";
        return;
    }
    Produk* t = headProduk;
    cout << left << setw(10) << "Kode"
         << setw(20) << "Nama"
         << setw(10) << "Harga"
         << setw(6) << "Stok" << endl;
    cout << "-------------------------------------\n";
    while (t) {
        cout << setw(10) << t->kode
             << setw(20) << t->nama
             << setw(10) << t->harga
             << setw(6) << t->stok << endl;
        t = t->next;
    }
}

Produk* cariProduk(string kode) {
    Produk* t = headProduk;
    while (t) {
        if (t->kode == kode) return t;
        t = t->next;
    }
    return NULL;
}

/* =====================================================
   KERANJANG BELANJA
===================================================== */
void tambahKeranjang(string nama, int qty, int harga) {
    Keranjang* b = new Keranjang{nama, qty, qty * harga, NULL};
    if (!headKeranjang) headKeranjang = b;
    else {
        Keranjang* t = headKeranjang;
        while (t->next) t = t->next;
        t->next = b;
    }
}

void tampilKeranjang() {
    cout << "=====================================\n";
    cout << "          KERANJANG BELANJA           \n";
    cout << "=====================================\n";
    if (!headKeranjang) {
        cout << "Keranjang kosong\n";
        return;
    }
    Keranjang* t = headKeranjang;
    int total = 0, no = 1;
    while (t) {
        cout << no++ << ". " << t->nama
             << " x" << t->qty
             << " = Rp " << t->subtotal << endl;
        total += t->subtotal;
        t = t->next;
    }
    cout << "-------------------------------------\n";
    cout << "TOTAL : Rp " << total << endl;
}

void hapusKeranjang() {
    while (headKeranjang) {
        Keranjang* h = headKeranjang;
        headKeranjang = headKeranjang->next;
        delete h;
    }
}

/* =====================================================
   LOGIN PEMBELI
===================================================== */
KTP loginPembeli() {
    KTP p;
    cout << "=====================================\n";
    cout << "        INPUT DATA PEMBELI            \n";
    cout << "=====================================\n";
    cout << "NIK           : "; cin >> p.nik;
    cin.ignore();
    cout << "Nama          : "; getline(cin, p.nama);
    cout << "Alamat        : "; getline(cin, p.alamat);
    cout << "Jenis Kelamin : "; getline(cin, p.jk);
    return p;
}

/* =====================================================
   STRUK BELANJA (TXT PER PEMBELI)
===================================================== */
void cetakNota(KTP p) {
    ofstream f("Nota_" + p.nik + ".txt");
    Keranjang* t = headKeranjang;
    int total = 0;

    f << "=====================================\n";
    f << "          STRUK BELANJA PC            \n";
    f << "=====================================\n";
    f << "Nama   : " << p.nama << endl;
    f << "NIK    : " << p.nik << endl;
    f << "Alamat : " << p.alamat << endl;
    f << "-------------------------------------\n";

    while (t) {
        f << t->nama << " x" << t->qty
          << " = Rp " << t->subtotal << endl;
        total += t->subtotal;
        t = t->next;
    }

    f << "-------------------------------------\n";
    f << "TOTAL BAYAR : Rp " << total << endl;
    f << "=====================================\n";
    f.close();
}

/* =====================================================
   RIWAYAT TRANSAKSI FORMAL (TXT APPEND)
===================================================== */
void simpanTransaksiTxt(KTP p) {
    ofstream f("Transaksi.txt", ios::app);
    Keranjang* t = headKeranjang;
    int total = 0;

    f << "=====================================\n";
    f << "RIWAYAT TRANSAKSI PENJUALAN PC\n";
    f << "=====================================\n";
    f << "Nama   : " << p.nama << endl;
    f << "NIK    : " << p.nik << endl;
    f << "Alamat : " << p.alamat << endl;
    f << "-------------------------------------\n";

    while (t) {
        f << t->nama << " x" << t->qty
          << " = Rp " << t->subtotal << endl;
        total += t->subtotal;
        t = t->next;
    }

    f << "-------------------------------------\n";
    f << "TOTAL BAYAR : Rp " << total << endl;
    f << "=====================================\n\n";
    f.close();
}

/* =====================================================
   MENU ADMIN
===================================================== */
void menuAdmin() {
    int pil;
    do {
        cout << "=====================================\n";
        cout << "              MENU ADMIN              \n";
        cout << "=====================================\n";
        cout << "1. Tambah Produk\n";
        cout << "2. Lihat Produk\n";
        cout << "3. Lihat Riwayat Transaksi\n";
        cout << "0. Logout\n";
        cout << "=====================================\n";
        cout << "Pilih : ";
        cin >> pil;

        if (pil == 1) tambahProduk();
        else if (pil == 2) tampilProduk();
        else if (pil == 3) tampilRiwayat();

    } while (pil != 0);
}

/* =====================================================
   MAIN PROGRAM
===================================================== */
int main() {
    int pilih;
    while (true) {
        cout << "=====================================\n";
        cout << "      SISTEM PENJUALAN PC STORE       \n";
        cout << "=====================================\n";
        cout << "1. Admin\n";
        cout << "2. Pembeli\n";
        cout << "0. Keluar\n";
        cout << "=====================================\n";
        cout << "Pilih Menu : ";
        cin >> pilih;

        if (pilih == 1) {
            string u, p;
            cout << "Username : "; cin >> u;
            cout << "Password : "; cin >> p;
            if (u == "admin" && p == "123")
                menuAdmin();
            else
                cout << "Login gagal\n";
        }
        else if (pilih == 2) {
            KTP pembeli = loginPembeli();
            int m;
            do {
                cout << "=====================================\n";
                cout << "            MENU PEMBELI              \n";
                cout << "=====================================\n";
                cout << "1. Lihat Produk\n";
                cout << "2. Beli Produk\n";
                cout << "3. Lihat Keranjang\n";
                cout << "4. Checkout\n";
                cout << "0. Kembali\n";
                cout << "=====================================\n";
                cout << "Pilih : ";
                cin >> m;

                if (m == 1) tampilProduk();
                else if (m == 2) {
                    string kode;
                    int qty;
                    cout << "Kode Produk : "; cin >> kode;
                    Produk* pr = cariProduk(kode);
                    if (pr) {
                        cout << "Jumlah : "; cin >> qty;
                        if (qty <= pr->stok) {
                            pr->stok -= qty;
                            tambahKeranjang(pr->nama, qty, pr->harga);
                            cout << "Berhasil ditambahkan ke keranjang\n";
                        } else cout << "Stok tidak cukup\n";
                    } else cout << "Produk tidak ditemukan\n";
                }
                else if (m == 3) tampilKeranjang();
                else if (m == 4) {
                    tampilKeranjang();           // OUTPUT
                    cetakNota(pembeli);          // Nota_NIK.txt
                    simpanTransaksiTxt(pembeli); // Transaksi.txt
                    push("Checkout oleh " + pembeli.nama);
                    hapusKeranjang();
                    cout << "Checkout berhasil & disimpan ke file\n";
                }

            } while (m != 0);
        }
        else break;
    }
    return 0;
}

