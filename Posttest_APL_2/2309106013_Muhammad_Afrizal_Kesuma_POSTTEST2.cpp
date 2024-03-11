#include <iostream>
#include <windows.h>

using namespace std;

int main(){
    string musik[50] = {"Viva La Vida", "Fix You", "Adventure of a Lifetime", "Talking to the Moon", "Perfect"};
    string artist[50] = {"Coldplay", "Coldplay", "Coldplay", "Bruno Mars", "Ed Sheeran"};
    int banyak_data = 5;
    int urutan;
    int cobalogin = 0;
    string username,password;
    string ulang, pilihan;

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
        Sleep(2000);
        system("cls");
        if (username == "Muhammad Afrizal Kesuma" && password == "2309106013"){
            cout << "Login Berhasil" << endl;
            Sleep(1000);
            ulang = "y";
            break;
        }
        else {
            cout << "Login Gagal, Silahkan Coba Lagi" << endl;
            cobalogin++;
            Sleep(1000);
        }
    }

    if (cobalogin == 3) {
        system("cls");
        cout << "Anda telah mencoba login sebanyak 3 kali, program akan terhenti";
        exit(0);               
    }

    while (ulang == "y"){
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
        if (pilihan == "1"){
            system("cls");
            if (banyak_data == 0){
                cout << "Playlist Anda Kosong" << endl;
                Sleep(2000);
                continue;
            }
            cout << "================================================" << endl;
            cout << "                 PLAYLIST MUSIK\n" << endl;
            for (int i = 0; i < banyak_data; i++) {
                cout << i+1 << ". " << musik[i] << " - " << artist[i] << endl;
            }
            cout << "================================================" << endl;
            cout << "\nTekan Enter untuk kembali ke menu";
            cin.ignore();
            cin.get();
        }
        else if (pilihan == "2") {
            system("cls");
            cout << "|| TAMBAHKAN MUSIK ||" << endl;
            cout << "=====================\n" << endl;
            cout << "Masukkan judul musik : ";
            cin.ignore();
            getline(cin, musik[banyak_data]);
            cout << "Masukkan nama artist : ";
            getline(cin, artist[banyak_data]);
            banyak_data++;
            cout << "\nBerhasil Menambahkan Musik ke Playlist" << endl;
            Sleep(2000);
        } 
        else if (pilihan == "3") {
            system("cls");
            cout << "|| UBAH DATA PLAYLIST ||" << endl;
            cout << "========================\n" << endl;
            if (banyak_data == 0){
                cout << "Playlist Anda Kosong" << endl;
                Sleep(2000);
                continue;
            }
            cout << "================================================" << endl;
            cout << "                 PLAYLIST MUSIK\n" << endl;
            for (int i = 0; i < banyak_data; i++) {
                cout << i+1 << ". " << musik[i] << " - " << artist[i] << endl;
            }
            cout << "================================================" << endl;
            cout << "\nMasukkan No. Urut Musik yang Ingin Diubah: ";
            cin >> urutan;
            if (urutan > banyak_data || urutan < 1){
                cout << "\nData Tidak Ditemukan" << endl;
                cin.clear();
                cin.ignore(INT_MAX, '\n');
                Sleep(2000);
                continue;
            }
            cout << "Masukkan perubahan judul musik : ";
            cin.ignore();
            getline(cin, musik[urutan-1]);
            cout << "Masukkan perubahan nama artist : ";
            getline(cin, artist[urutan-1]);
            cout << "\nBerhasil Mengubah Data Playlist" << endl;
            Sleep(2000);
        } 
        else if (pilihan == "4") {
            system("cls");
            cout << "|| HAPUS MUSIK DARI PLAYLIST ||" << endl;
            cout << "===============================\n" << endl;
            if (banyak_data == 0){
                cout << "Playlist Anda Kosong" << endl;
                Sleep(2000);
                continue;
            }
            cout << "================================================" << endl;
            cout << "                 PLAYLIST MUSIK\n" << endl;
            for (int i = 0; i < banyak_data; i++) {
                cout << i+1 << ". " << musik[i] << " - " << artist[i] << endl;
            }
            cout << "================================================" << endl;
            cout << "\nMasukkan No. Urut Musik yang Ingin Dihapus : ";
            cin >> urutan;
            if (urutan > banyak_data || urutan < 1){
                cout << "\nData Tidak Ditemukan" << endl;
                cin.clear();
                cin.ignore(INT_MAX, '\n');
                Sleep(2000);
                continue;
            }
            for (int i = urutan-1; i < banyak_data-1; i++) {
                musik[i] = musik[i+1];
                artist[i] = artist[i+1];
            }
            banyak_data--;
            cout << "\nBerhasil Menghapus Musik dari Playlist" << endl;
            Sleep(2000);
        }
        else if (pilihan == "5"){
            ulang = "n";}
        else {
            cout << "Pilihan Tidak Valid" << endl;
            Sleep(2000);
        }
        
    }
    system("cls");
    cout << "Anda Telah Keluar dari Program" << endl;
    return 0;
}