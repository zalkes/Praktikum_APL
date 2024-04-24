#include <iostream>
#include <windows.h>
#include <iomanip>

using namespace std;

//Pendeklarasian struct musik
struct musik {
    string judul;
    string artist;
};

// Penggunaan nested struct
struct infomusik {
    struct musik lagu;
    int durasi;
    string genre;
    int tahunrilis;
};

// Penggunaan Array of Struct
struct infomusik playlist[50] = {
    {{"Viva La Vida", "Coldplay"}, 242, "Alternative Rock", 2008},
    {{"Fix You", "Coldplay"}, 294, "Alternative Rock", 2005},
    {{"Adventure of a Lifetime", "Coldplay"}, 259, "Alternative Rock", 2015},
    {{"Talking to the Moon", "Bruno Mars"}, 217, "Pop", 2010},
    {{"Perfect", "Ed Sheeran"}, 263, "Pop", 2017}
};

// Fungsi Untuk Login
bool login(string username, string password) {
    if (username == "Muhammad Afrizal Kesuma" && password == "2309106013") {
        cout << "Login Berhasil" << endl;
        Sleep(1000);
        return true;
    } else {
        cout << "Login Gagal, Silahkan Coba Lagi" << endl;
        Sleep(1000);
        return false;
    }
}

// Fungsi Rekursif Untuk Login
bool login_rekursif(int cobalogin = 0) {
    string username, password;
    if (cobalogin == 3) {
        system("cls");
        cout << "Anda telah mencoba login sebanyak 3 kali, program akan terhenti";
        exit(0);               
    }
    system("cls");
    cout << "      Selamat datang di Program Playlist Musik\n" << endl;
    cout <<"===================================================" << endl;
    cout << "                  Silahkan Login" << endl;
    cout <<"---------------------------------------------------" << endl;
    cout << "Masukkan Username : ";
    getline(cin, username);
    cout << "Masukkan Password : ";
    cin >> password;
    cout <<"===================================================" << endl;
    cout << "\nChecking..." << endl;
    cin.ignore();
    Sleep(1000);
    system("cls");
    if (login(username, password)) {
        return true;
    }
    else {
        return login_rekursif(cobalogin + 1);
    }
}

// Prosedur Untuk Menampilkan Playlist dengan Parameter Dereference playlist dan Address-of banyak_data
void tampilkan_playlist(struct infomusik *playlist, int &banyak_data) {
    cout << "                                            PLAYLIST MUSIK" << endl;
    cout << "=========================================================================================================" << endl;
    // Mencetak header tabel
    cout << left << setw(5) << "No" << setw(30) << "Judul" << setw(20) 
    << "Artist" << setw(15) << "Durasi(s)" << setw(20) 
    << "Genre" << setw(20) << "Tahun Rilis" << endl;
    cout << setfill('-') << setw(105) << "" << endl;
    cout << setfill(' '); 
    // Mencetak data dalam bentuk tabel
    for (int i = 0; i < banyak_data; ++i) {
        if (playlist[i].lagu.judul.empty() || playlist[i].lagu.artist.empty() || playlist[i].genre.empty()) {
            continue;
        }
        cout << left << setw(5) << i+1 << setw(30) << playlist[i].lagu.judul 
        << setw(23) << playlist[i].lagu.artist << setw(12) << playlist[i].durasi 
        << setw(24) << playlist[i].genre << setw(10) << playlist[i].tahunrilis << endl;
    }
    cout << "=========================================================================================================" << endl;
}

// Error Handling Untuk Inputan String Kosong
void check_string_kosong(string &inputan, string jenis) {
    while (inputan.empty() || inputan == " ") {
        cout << "Inputan tidak boleh kosong" << endl;
        cout << "Masukkan " << jenis << ": ";
        getline(cin, inputan);
    }
}

// Error Handling Untuk Inputan Bukan Angka
void check_angka(int &inputan, string jenis) {
    while (cin.fail() || inputan < 0) {
        cout << "Inputan harus berupa angka" << endl;
        cout << "Masukkan " << jenis << ": ";
        cin.clear();
        cin.ignore(INT_MAX, '\n');
        cin >> inputan;
    }
}

// Fungsi Untuk Menambahkan Musik Dengan Parameter Dereference playlist dan Address-of banyak_data
string tambah_musik(struct infomusik *playlist, int &banyak_data) { 
    cout << "|| TAMBAHKAN MUSIK ||" << endl;
    cout << "=====================\n" << endl;
    cout << "Masukkan judul musik : ";
    cin.ignore();
    getline(cin, playlist[banyak_data].lagu.judul);
    check_string_kosong(playlist[banyak_data].lagu.judul, "judul musik");
    // Mengecek apakah judul musik sudah terdapat di array
    for (int i = 0; i < banyak_data; i++) {
        while (playlist[i].lagu.judul == playlist[banyak_data].lagu.judul) {
            cout << "Judul musik sudah terdapat di playlist" << endl;
            cout << "Masukkan judul musik : ";
            getline(cin, playlist[banyak_data].lagu.judul);
        }
    }
    cout << "Masukkan nama artist : ";
    getline(cin, playlist[banyak_data].lagu.artist);
    check_string_kosong(playlist[banyak_data].lagu.artist, "nama artist");
    cout << "Masukkan durasi musik (dalam detik) : ";
    cin >> playlist[banyak_data].durasi;
    check_angka(playlist[banyak_data].durasi, "durasi musik");
    cout << "Masukkan genre musik : ";
    cin.ignore();
    getline(cin, playlist[banyak_data].genre);
    check_string_kosong(playlist[banyak_data].genre, "genre musik");
    cout << "Masukkan tahun rilis musik : ";
    cin >> playlist[banyak_data].tahunrilis;
    check_angka(playlist[banyak_data].tahunrilis, "tahun rilis musik");
    return playlist[banyak_data].lagu.judul, playlist[banyak_data].lagu.artist;
}

// Prosedur Untuk Mengubah Data Playlist Dengan Parameter Dereference playlist dan Address-of banyak_data
void ubahdata_playlist(struct infomusik *playlist, int banyak_data) {
    system("cls");
    int urutan;
    cout << "|| UBAH DATA PLAYLIST ||" << endl;
    cout << "========================\n" << endl;
    tampilkan_playlist(playlist, banyak_data);
    cout << "\nMasukkan No. Urut Musik yang Ingin Diubah: ";
    cin >> urutan;
    if (urutan > banyak_data || urutan < 1){
        cout << "\nData Tidak Ditemukan" << endl;
        cin.clear();
        cin.ignore(INT_MAX, '\n');
        Sleep(2000);
    }
    else {
        cout << "Masukkan perubahan judul musik : ";
        cin.ignore();
        getline(cin, playlist[urutan-1].lagu.judul);
        check_string_kosong(playlist[urutan-1].lagu.judul, "judul musik");
        cout << "Masukkan perubahan nama artist : ";
        getline(cin, playlist[urutan-1].lagu.artist);
        check_string_kosong(playlist[urutan-1].lagu.artist, "nama artist");
        cout << "Masukkan perubahan durasi musik (dalam detik) : ";
        cin >> playlist[urutan-1].durasi;
        check_angka(playlist[urutan-1].durasi, "durasi musik");
        cout << "Masukkan perubahan genre musik : ";
        cin.ignore();
        getline(cin, playlist[urutan-1].genre);
        check_string_kosong(playlist[urutan-1].genre, "genre musik");
        cout << "Masukkan perubahan tahun rilis musik : ";
        cin >> playlist[urutan-1].tahunrilis;
        check_angka(playlist[urutan-1].tahunrilis, "tahun rilis musik");
        cout << "\nBerhasil Mengubah Data Playlist" << endl;
        Sleep(2000);
    }
}

// Prosedur Untuk Menghapus Musik Dengan Parameter Dereference playlist dan Address-of banyak_data
void hapus_musik(struct infomusik *playlist, int &banyak_data) {
    system("cls");
    int urutan;
    cout << "|| HAPUS MUSIK DARI PLAYLIST ||" << endl;
    cout << "===============================\n" << endl;
    tampilkan_playlist(playlist, banyak_data);
    cout << "\nMasukkan No. Urut Musik yang Ingin Dihapus : ";
    cin >> urutan;
    if (urutan > banyak_data || urutan < 1){
        cout << "\nData Tidak Ditemukan" << endl;
        cin.clear();
        cin.ignore(INT_MAX, '\n');
        Sleep(2000);
    }
    else {
        cout << "\nBerhasil Menghapus (" << playlist[urutan-1].lagu.judul << " - " << playlist[urutan-1].lagu.artist << ") dari Playlist" << endl;
        for (int i = urutan-1; i < banyak_data-1; i++) {
            playlist[i] = playlist[i+1];
        }
        banyak_data--;
        Sleep(2000);
    }
}

//Fungsi Bubble Sort Untuk Mengurutkan Judul Musik Secara Ascending
void bubblesort(struct infomusik *playlist, int &banyak_data) {
    for (int i = 0; i < banyak_data - 1; i++) {
        for (int j = 0; j < banyak_data - i - 1; j++) {
            if (!playlist[j].lagu.judul.empty() && !playlist[j + 1].lagu.judul.empty()) {
                if (playlist[j].lagu.judul > playlist[j + 1].lagu.judul) {
                    swap(playlist[j], playlist[j + 1]);
                }
            }
        }
    }
}

//Fungsi Selection Sort Untuk Mengurutkan Genre Secara Ascending
void selectionsort(struct infomusik *playlist, int &banyak_data) {
    for (int i = 0; i < banyak_data - 1; i++) {
        int min_idx = i;
        for (int j = i + 1; j < banyak_data; j++) {
            if (!playlist[j].genre.empty() && !playlist[min_idx].genre.empty()) {
                if (playlist[j].genre < playlist[min_idx].genre) {
                    min_idx = j;
                }
            }
        }
        if (min_idx != i) {
            swap(playlist[i], playlist[min_idx]);
        }
    }
}

//Fungsi Insertion Sort Untuk Mengurutkan Tahun Rilis Secara Descending
void insertionsort (struct infomusik *playlist, int &banyak_data) {
    for (int i = 1; i < banyak_data; i++) {
        struct infomusik key = playlist[i];
        int j = i - 1;
        while (j >= 0 && playlist[j].tahunrilis < key.tahunrilis) {
            playlist[j+1] = playlist[j];
            j = j - 1;
        }
        playlist[j+1] = key;
    }
}

//Fungsi Binary Search Untuk Mencari Musik dengan Data Terurut secara Ascending
void binarysearch (struct infomusik *playlist, int banyak_data) {
    string cari;
    cout << "|| SEARCH MUSIK BERDASARKAN JUDUL ||" << endl;
    cout << "====================================\n" << endl;
    cout << "Masukkan judul musik yang ingin dicari : ";
    cin.ignore();
    getline(cin, cari);
    check_string_kosong(cari, "judul musik");
    int kiri = 0, kanan = banyak_data - 1;
    while (kiri <= kanan) {
        int tengah = kiri + (kanan - kiri) / 2;
        if (playlist[tengah].lagu.judul == cari) {
            cout << "\nMusik Ditemukan pada urutan ke-" << tengah + 1 << " dari playlist" << endl;
            cout << "\nJudul : " << playlist[tengah].lagu.judul << endl;
            cout << "Artist : " << playlist[tengah].lagu.artist << endl;
            cout << "Durasi : " << playlist[tengah].durasi << " detik" << endl;
            cout << "Genre : " << playlist[tengah].genre << endl;
            cout << "Tahun Rilis : " << playlist[tengah].tahunrilis << endl;
            return;
        }
        else if (playlist[tengah].lagu.judul < cari) {
            kiri = tengah + 1;
        }
        else {
            kanan = tengah - 1;
        }
    }
    cout << "\nData Tidak Ditemukan" << endl;
}

//Fungsi Sequential Search Untuk Mencari Musik berdasarkan Tahun Rilis dengan Data Terurut secara Descending
void sequentialsearch (struct infomusik *playlist, int banyak_data) {
    int cari;
    bool found = false;
    cout << "|| SEARCH MUSIK BERDASARKAN TAHUN RILIS ||" << endl;
    cout << "==========================================\n" << endl;
    cout << "Masukkan tahun rilis musik yang ingin dicari : ";
    cin >> cari;
    check_angka(cari, "tahun rilis musik");
    for (int i = 0; i < banyak_data; i++) {
        if (playlist[i].tahunrilis == cari) {
            found = true;
            cout << "\nMusik Ditemukan pada urutan ke-" << i + 1 << " dari playlist" << endl;
            cout << "\nJudul : " << playlist[i].lagu.judul << endl;
            cout << "Artist : " << playlist[i].lagu.artist << endl;
            cout << "Durasi : " << playlist[i].durasi << " detik" << endl;
            cout << "Genre : " << playlist[i].genre << endl;
            cout << "Tahun Rilis : " << playlist[i].tahunrilis << endl;
        }
    }
    if (!found) {
        cout << "\nData Tidak Ditemukan" << endl;
    }
}

void sortmenu(){
    while(true){
        system("cls");
        cout << "                     SORT MENU" << endl;
        cout << "======================================================" << endl;
        cout << "|| [1] Bubble Sort (Judul Musik Ascending)          ||" << endl;
        cout << "|| [2] Selection Sort (Genre Musik Ascending)       ||" << endl;
        cout << "|| [3] Insertion Sort (Tahun Rilis Musik Descending)||" << endl;
        cout << "|| [4] Kembali ke Menu Utama                        ||" << endl;
        cout << "======================================================" << endl;
        cout << "Pilih Menu : ";
        int pilihan_sort, banyak_data;
        cin >> pilihan_sort;
        if (pilihan_sort == 1){
            bubblesort(playlist, banyak_data);
            cout << "\nBerhasil Mengurutkan Judul Musik Secara Ascending" << endl;
            Sleep(2000);
            cout << endl;
            tampilkan_playlist(playlist, banyak_data);
            cout << "\nTekan Enter untuk kembali ke menu";
            cin.ignore();
            cin.get();
        }
        else if (pilihan_sort == 2){
            selectionsort(playlist, banyak_data);
            cout << "\nBerhasil Mengurutkan Genre Musik Secara Ascending" << endl;
            Sleep(2000);
            cout << endl;
            tampilkan_playlist(playlist, banyak_data);
            cout << "\nTekan Enter untuk kembali ke menu";
            cin.ignore();
            cin.get();
        }
        else if (pilihan_sort == 3){
            insertionsort(playlist, banyak_data);
            cout << "\nBerhasil Mengurutkan Tahun Rilis Musik Secara Descending" << endl;
            Sleep(2000);
            cout << endl;
            tampilkan_playlist(playlist, banyak_data);
            cout << "\nTekan Enter untuk kembali ke menu";
            cin.ignore();
            cin.get();
        }
        else if (pilihan_sort == 4){
            break;
        }
        else {
            cout << "Pilihan Tidak Valid" << endl;
            Sleep(2000);
        }
    }
}

void searchmenu() {
    while(true){
        system("cls");
        cout << "                     SEARCH MENU" << endl;
        cout << "======================================================" << endl;
        cout << "|| [1] Binary Search (Judul Musik Ascending)        ||" << endl;
        cout << "|| [2] Sequential Search (Tahun Rilis Descending)   ||" << endl;
        cout << "|| [3] Kembali ke Menu Utama                        ||" << endl;
        cout << "======================================================" << endl;
        cout << "Pilih Menu : ";
        int pilihan_search, banyak_data;
        cin >> pilihan_search;
        if (pilihan_search == 1){
            system("cls");
            bubblesort(playlist, banyak_data);
            tampilkan_playlist(playlist, banyak_data);
            cout << endl;
            binarysearch(playlist, banyak_data);
            cout << "\nTekan Enter untuk kembali ke menu";
            cin.ignore();
            cin.get();
        }
        else if (pilihan_search == 2){
            system("cls");
            insertionsort(playlist, banyak_data);
            tampilkan_playlist(playlist, banyak_data);
            cout << endl;
            sequentialsearch(playlist, banyak_data);
            cout << "\nTekan Enter untuk kembali ke menu";
            cin.ignore();
            cin.get();
        }
        else if (pilihan_search == 3){
            break;
        }
        else {
            cout << "Pilihan Tidak Valid" << endl;
            Sleep(2000);
        }
    }
}

int main(){
    int banyak_data = 5;
    int urutan;
    string pilihan;
    login_rekursif();    
    while (true) {
        system("cls");
        cout << "            PLAYLIST MENU" << endl;
        cout << "=====================================" << endl;
        cout << "|| [1] Tampilkan Playlist Musik    ||" << endl;
        cout << "|| [2] Tambahkan Musik ke Playlist ||" << endl;
        cout << "|| [3] Ubah Data Playlist          ||" << endl;
        cout << "|| [4] Hapus Musik dari Playlist   ||" << endl;
        cout << "|| [5] Sorting Musik               ||" << endl;
        cout << "|| [6] Search Musik                ||" << endl;
        cout << "|| [7] Keluar Program              ||" << endl;
        cout << "=====================================" << endl;
        cout << "Pilih Menu : ";
        cin >> pilihan;
        system("cls");
        if (pilihan == "1") {
            if (banyak_data == 0){
                cout << "Playlist Anda Kosong" << endl;
                Sleep(2000);
                continue;
            }
            tampilkan_playlist(playlist, banyak_data);
            cout << "\nTekan Enter untuk kembali ke menu";
            cin.ignore();
            cin.get();
        }
        
        else if (pilihan == "2") {
            tambah_musik(playlist, banyak_data);
            cout << "\nBerhasil Menambahkan (" << playlist[banyak_data].lagu.judul << " - " << playlist[banyak_data].lagu.artist << ") ke Playlist" << endl;
            banyak_data++;
            Sleep(2000);
        } 

        else if (pilihan == "3") {
            if (banyak_data == 0){
                cout << "Playlist Anda Kosong" << endl;
                Sleep(2000);
                continue;
            }
            ubahdata_playlist(playlist, banyak_data);
        } 
        else if (pilihan == "4") {
            if (banyak_data == 0){
                cout << "Playlist Anda Kosong" << endl;
                Sleep(2000);
                continue;
            }
            hapus_musik(playlist, banyak_data);
        }
        else if (pilihan == "5"){
            if (banyak_data == 0){
                cout << "Playlist Anda Kosong" << endl;
                Sleep(2000);
                continue;
            }
            sortmenu();
        }
        else if (pilihan == "6"){
            if (banyak_data == 0){
                cout << "Playlist Anda Kosong" << endl;
                Sleep(2000);
                continue;
            }
            searchmenu();
        }
        else if (pilihan == "7") {
            break;
        }
        else {
            cout << "Pilihan Tidak Valid" << endl;
            Sleep(2000);
        }
    }
    system("cls");
    cout << "Anda Telah Keluar dari Program" << endl;
    return 0;
}