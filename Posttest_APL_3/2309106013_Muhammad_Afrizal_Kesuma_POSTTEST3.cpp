#include <iostream>
#include <windows.h>

using namespace std;

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

// Fungsi Rekursif Untuk Menampilkan Playlist
void tampilkan_playlist(string musik[], string artist[], int &banyak_data, int index = 0) {
    if (index >= banyak_data) {
        return;
    }
    if (index == 0) {
        cout << "================================================" << endl;
        cout << "                 PLAYLIST MUSIK\n" << endl;
    }
    cout << index + 1 << ". " << musik[index] << " - " << artist[index] << endl;
    tampilkan_playlist(musik, artist, banyak_data, index + 1);
    if (index == banyak_data - 1) {
        cout << "================================================" << endl;
    }
}

// Fungsi Untuk Menambahkan Musik
string tambah_musik(string musik[], string artist[], int &banyak_data) {
    cout << "|| TAMBAHKAN MUSIK ||" << endl;
    cout << "=====================\n" << endl;
    cout << "Masukkan judul musik : ";
    cin.ignore();
    getline(cin, musik[banyak_data]);
    cout << "Masukkan nama artist : ";
    getline(cin, artist[banyak_data]);
    return musik[banyak_data], artist[banyak_data];
}

// Prosedur Untuk Mengubah Data Playlist
void ubahdata_playlist(string musik[], string artist[], int &banyak_data) {
    system("cls");
    int urutan;
    cout << "|| UBAH DATA PLAYLIST ||" << endl;
    cout << "========================\n" << endl;
    tampilkan_playlist(musik, artist, banyak_data);
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
    getline(cin, musik[urutan-1]);
    cout << "Masukkan perubahan nama artist : ";
    getline(cin, artist[urutan-1]);
    cout << "\nBerhasil Mengubah Data Playlist" << endl;
    Sleep(2000);
    }
}

// Prosedur Untuk Menghapus Musik
void hapus_musik(string musik[], string artist[], int &banyak_data) {
    system("cls");
    int urutan;
    cout << "|| HAPUS MUSIK DARI PLAYLIST ||" << endl;
    cout << "===============================\n" << endl;
    tampilkan_playlist(musik, artist, banyak_data);
    cout << "\nMasukkan No. Urut Musik yang Ingin Dihapus : ";
    cin >> urutan;
    if (urutan > banyak_data || urutan < 1){
        cout << "\nData Tidak Ditemukan" << endl;
        cin.clear();
        cin.ignore(INT_MAX, '\n');
        Sleep(2000);
    }
    else {
        cout << "\nBerhasil Menghapus (" << musik[urutan-1] << " - " << artist[urutan-1]<< ") dari Playlist" << endl;
        for (int i = urutan-1; i < banyak_data-1; i++) {
            musik[i] = musik[i+1];
            artist[i] = artist[i+1];
        }
        banyak_data--;
        Sleep(2000);
    }
}

int main(){
    string musik[50] = {"Viva La Vida", "Fix You", "Adventure of a Lifetime", "Talking to the Moon", "Perfect"};
    string artist[50] = {"Coldplay", "Coldplay", "Coldplay", "Bruno Mars", "Ed Sheeran"};
    int banyak_data = 5;
    int urutan;
    int cobalogin = 0;
    string username, password, pilihan;

    while (cobalogin < 3) {
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
            break;
        }
        else {
            cobalogin++;
        }
    }
        
    if (cobalogin == 3) {
        system("cls");
        cout << "Anda telah mencoba login sebanyak 3 kali, program akan terhenti";
        exit(0);               
    }

    while (true) {
        system("cls");
        cout << "            PLAYLIST MENU" << endl;
        cout << "=====================================" << endl;
        cout << "|| [1] Tampilkan Playlist Musik    ||" << endl;
        cout << "|| [2] Tambahkan Musik ke Playlist ||" << endl;
        cout << "|| [3] Ubah Data Playlist          ||" << endl;
        cout << "|| [4] Hapus Musik dari Playlist   ||" << endl;
        cout << "|| [5] Keluar Program              ||" << endl;
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
            tampilkan_playlist(musik, artist, banyak_data);
            cout << "\nTekan Enter untuk kembali ke menu";
            cin.ignore();
            cin.get();
        }
        
        else if (pilihan == "2") {
            tambah_musik(musik, artist, banyak_data);
            cout << "\nBerhasil Menambahkan (" << musik[banyak_data] << " - " << artist[banyak_data] << ") ke Playlist" << endl;
            banyak_data++;
            Sleep(2000);
        } 

        else if (pilihan == "3") {
            if (banyak_data == 0){
                cout << "Playlist Anda Kosong" << endl;
                Sleep(2000);
                continue;
            }
            ubahdata_playlist(musik, artist, banyak_data);
        } 

        else if (pilihan == "4") {
            if (banyak_data == 0){
                cout << "Playlist Anda Kosong" << endl;
                Sleep(2000);
                continue;
            }
            hapus_musik(musik, artist, banyak_data);
        }

        else if (pilihan == "5"){
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