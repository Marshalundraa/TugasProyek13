#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
using namespace std;

// =========================================================
// 1. STRUCT KTP
// =========================================================
struct KTP {
    string nik, nama, alamat, jk;
};

// =========================================================
// 2. AVL TREE (PRODUK)
// =========================================================
struct Produk {
    string kode, nama;
    int harga, stok, height;
    Produk *left, *right;
};

Produk* rootProduk = NULL;

int height(Produk* n) { return n ? n->height : 0; }
int max(int a, int b) { return (a > b) ? a : b; }

Produk* rotasiKanan(Produk* y) {
    Produk* x = y->left;
    Produk* T2 = x->right;
    x->right = y;
    y->left = T2;
    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;
    return x;
}

Produk* rotasiKiri(Produk* x) {
    Produk* y = x->right;
    Produk* T2 = y->left;
    y->left = x;
    x->right = T2;
    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;
    return y;
}

Produk* insertAVL(Produk* node, string k, string n, int h, int s) {
    if (!node) return new Produk{k, n, h, s, 1, NULL, NULL};

    if (k < node->kode)
        node->left = insertAVL(node->left, k, n, h, s);
    else if (k > node->kode)
        node->right = insertAVL(node->right, k, n, h, s);
    else {
        node->stok += s;
        return node;
    }

    node->height = 1 + max(height(node->left), height(node->right));
    int balance = height(node->left) - height(node->right);

    if (balance > 1 && k < node->left->kode)
        return rotasiKanan(node);
    if (balance < -1 && k > node->right->kode)
        return rotasiKiri(node);
    if (balance > 1 && k > node->left->kode) {
        node->left = rotasiKiri(node->left);
        return rotasiKanan(node);
    }
    if (balance < -1 && k < node->right->kode) {
        node->right = rotasiKanan(node->right);
        return rotasiKiri(node);
    }
    return node;
}

Produk* cariProduk(Produk* r, string k) {
    if (!r || r->kode == k) return r;
    return (k < r->kode) ? cariProduk(r->left, k) : cariProduk(r->right, k);
}

void inorderProduk(Produk* r) {
    if (!r) return;
    inorderProduk(r->left);
    cout << left << setw(10) << r->kode
         << setw(20) << r->nama
         << setw(10) << r->harga
         << setw(6) << r->stok << endl;
    inorderProduk(r->right);
}

// === KHUSUS PEMBELI (TAMPIL SEMUA PRODUK) ===
void tampilProdukPembeli(Produk* r) {
    if (!r) return;
    tampilProdukPembeli(r->left);
    cout << left << setw(20) << r->nama
         << setw(10) << r->harga
         << setw(6) << r->stok << endl;
    tampilProdukPembeli(r->right);
}

// =========================================================
// 3. STACK (RIWAYAT TRANSAKSI)
// =========================================================
struct Stack {
    string info;
    Stack* next;
};

Stack* topStack = NULL;

void push(string s) {
    topStack = new Stack{s, topStack};
}

void tampilRiwayat() {
    Stack* c = topStack;
    cout << "\n=== RIWAYAT TRANSAKSI ===\n";
    while (c) {
        cout << "- " << c->info << endl;
        c = c->next;
    }
}

// =========================================================
// 4. QUEUE (ANTRIAN PEMBELI)
// =========================================================
struct Antrian {
    KTP data;
    Antrian* next;
};

Antrian *frontQ = NULL, *rearQ = NULL;

void enqueue(KTP p) {
    Antrian* b = new Antrian{p, NULL};
    if (!rearQ) frontQ = rearQ = b;
    else {
        rearQ->next = b;
        rearQ = b;
    }
}

KTP dequeue() {
    Antrian* t = frontQ;
    KTP d = t->data;
    frontQ = frontQ->next;
    if (!frontQ) rearQ = NULL;
    delete t;
    return d;
}

// =========================================================
// 5. DOUBLE LINKED LIST (KERANJANG)
// =========================================================
struct Keranjang {
    string nama;
    int qty, subtotal;
    Keranjang *next, *prev;
};

Keranjang *headK = NULL, *tailK = NULL;

void tambahKeranjang(string n, int q, int s) {
    Keranjang* b = new Keranjang{n, q, s, NULL, NULL};
    if (!headK) headK = tailK = b;
    else {
        tailK->next = b;
        b->prev = tailK;
        tailK = b;
    }
}

void hapusKeranjang() {
    while (headK) {
        Keranjang* t = headK;
        headK = headK->next;
        delete t;
    }
    tailK = NULL;
}

// =========================================================
// 6. LOGIN PEMBELI
// =========================================================
KTP loginPembeli() {
    KTP p;
    cout << "\n======= LOGIN PEMBELI (KTP) =======\n";
    cout << "NIK           : "; cin >> p.nik;
    cin.ignore();
    cout << "Nama          : "; getline(cin, p.nama);
    cout << "Alamat        : "; getline(cin, p.alamat);
    cout << "Jenis Kelamin : "; getline(cin, p.jk);
    return p;
}

// =========================================================
// 7. CETAK NOTA TXT
// =========================================================
void cetakNota(KTP p) {
    ofstream f("Nota_" + p.nik + ".txt");
    int total = 0;

    f << "========= STRUK BELANJA PC =========\n";
    f << "Nama   : " << p.nama << endl;
    f << "NIK    : " << p.nik << endl;
    f << "Alamat : " << p.alamat << endl;
    f << "-----------------------------------\n";

    Keranjang* c = headK;
    while (c) {
        f << c->nama << " x" << c->qty
          << " = Rp " << c->subtotal << endl;
        total += c->subtotal;
        c = c->next;
    }

    f << "-----------------------------------\n";
    f << "TOTAL BAYAR : Rp " << total << endl;
    f.close();
}

// =========================================================
// 8. MENU ADMIN
// =========================================================
void menuAdmin() {
    int pil;
    do {
        cout << "\n========== MENU ADMIN ==========\n";
        cout << "1. Tambah / Update Produk\n";
        cout << "2. Lihat Daftar Produk\n";
        cout << "3. Cari Produk\n";
        cout << "4. Lihat Riwayat Transaksi\n";
        cout << "5. Logout\n";
        cout << "Pilih: ";
        cin >> pil;

        if (pil == 1) {
            string k, n;
            int h, s;
            cout << "Kode Produk : "; cin >> k;
            cin.ignore();
            cout << "Nama Produk : "; getline(cin, n);
            cout << "Harga       : "; cin >> h;
            cout << "Stok        : "; cin >> s;
            rootProduk = insertAVL(rootProduk, k, n, h, s);
            cout << "Produk berhasil disimpan\n";
        }
        else if (pil == 2) {
            cout << "\nKode      Nama Produk         Harga     Stok\n";
            cout << "--------------------------------------------\n";
            inorderProduk(rootProduk);
        }
        else if (pil == 3) {
            string k;
            cout << "Kode Produk: "; cin >> k;
            Produk* p = cariProduk(rootProduk, k);
            if (p)
                cout << "Ditemukan: " << p->nama << endl;
            else
                cout << "Produk tidak ditemukan\n";
        }
        else if (pil == 4) {
            tampilRiwayat();
        }
    } while (pil != 5);
}

// =========================================================
// 9. MAIN PROGRAM
// =========================================================
int main() {
    int pilih;
    while (true) {
        cout << "\n====== SISTEM PENJUALAN PC ======\n";
        cout << "1. Admin\n2. Pembeli\n3. Keluar\nPilih: ";
        cin >> pilih;

        if (pilih == 1) {
            string u, p;
            cout << "Username: "; cin >> u;
            cout << "Password: "; cin >> p;
            if (u == "admin" && p == "123")
                menuAdmin();
            else
                cout << "Login gagal\n";
        }
        else if (pilih == 2) {
            KTP p = loginPembeli();
            enqueue(p);
            KTP aktif = dequeue();

            int pb;
            do {
                cout << "\n====== MENU PEMBELI ======\n";
                cout << "1. Lihat Semua Produk\n";
                cout << "2. Beli Produk\n";
                cout << "3. Checkout\n";
                cout << "Pilih: ";
                cin >> pb;

                if (pb == 1) {
                    cout << "\nNama Produk          Harga     Stok\n";
                    cout << "-----------------------------------\n";
                    tampilProdukPembeli(rootProduk);
                }
                else if (pb == 2) {
                    string k;
                    int j;
                    cout << "Kode Produk: "; cin >> k;
                    Produk* pr = cariProduk(rootProduk, k);
                    if (pr) {
                        cout << "Jumlah: "; cin >> j;
                        if (pr->stok >= j) {
                            pr->stok -= j;
                            tambahKeranjang(pr->nama, j, pr->harga * j);
                            cout << "Masuk keranjang\n";
                        } else {
                            cout << "Stok tidak cukup\n";
                        }
                    } else {
                        cout << "Produk tidak ditemukan\n";
                    }
                }
                else if (pb == 3) {
                    cetakNota(aktif);
                    push("Checkout oleh " + aktif.nama);
                    hapusKeranjang();
                    cout << "Checkout berhasil\n";
                }
            } while (pb != 3);
        }
        else break;
    }
    return 0;
}

