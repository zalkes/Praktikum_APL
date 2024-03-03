#include <iostream>
#include <string>
#include <windows.h>
#include <math.h>
#include <iomanip>
using namespace std;

int main() {
    double nominal;
    double konversi1, konversi2, konversi3;
    string ulang = "y";
    int pilihan;
    string username, password;
    int cobalogin = 0;
    while (cobalogin <= 3) {
        system("cls");
        cout << "Silahkan Login" << endl;
        cout << "Masukkan username: ";
        cin >> username;
        cout << "Masukkan password: ";
        cin >> password;
        if (username == "Muhammad_Afrizal_Kesuma" && password == "2309106013") {
            cout << "Login berhasil";
            Sleep(2000);
            break;
        } 
        else {
            cout << "Login gagal, silahkan coba lagi" << endl;
            cobalogin++;
            Sleep(2000);
        }
        if (cobalogin == 3) {
            system("cls");
            cout << "Anda telah mencoba login sebanyak 3 kali, program akan berhenti";
            exit(0);               
        }   
    }
    while (ulang == "y") {
        system("cls");
        cout << "Selamat datang di Program Konversi Mata Uang" << endl;
        cout <<endl;
        cout << "   PILIH MENU KONVERSI" << endl;
        cout << "--------------------------" << endl;
        cout << "|| [1] Konversi Rupiah  ||" << endl;
        cout << "|| [2] Konversi Dollar  ||" << endl;
        cout << "|| [3] Konversi Euro    ||" << endl;
        cout << "|| [4] Konversi Yen     ||" << endl;
        cout << "|| [5] Keluar Program   ||" << endl; 
        cout << "--------------------------" << endl;
        cout << "Masukkan Pilihan: ";
        cin >> pilihan;
        if (pilihan == 1) {
            system("cls");
            cout << "Masukkan nominal IDR: ";
            cin >> nominal;
            while (nominal < 0) {
                cout << "Nominal tidak boleh negatif, silahkan masukkan nominal kembali: ";
                cin >> nominal;
            }
            konversi1 = nominal / 15700;
            cout << "Hasil konversi ke Dollar : " << fixed << setprecision(2) << konversi1 << " USD" << endl;
            konversi2 = nominal / 17000;
            cout << "Hasil konversi ke Euro   : " << fixed << setprecision(2) << konversi2 << " EUR" << endl;
            konversi3 = nominal / 104;
            cout << "Hasil konversi ke Yen    : " << fixed << setprecision(2) << konversi3 << " JPY" << endl;
        }
        else if (pilihan == 2) {
            system("cls");
            cout << "Masukkan nominal USD: ";
            cin >> nominal;
            while (nominal < 0) {
                cout << "Nominal tidak boleh negatif, silahkan masukkan nominal kembali: ";
                cin >> nominal;
            }
            konversi1 = nominal * 15700;
            cout << "Hasil konversi ke Rupiah : " << fixed << setprecision(2) << konversi1 << " IDR" << endl;
            konversi2 = nominal * 0.92;
            cout << "Hasil konversi ke Euro   : " << fixed << setprecision(2) << konversi2 << " EUR" << endl;
            konversi3 = nominal * 150;
            cout << "Hasil konversi ke Yen    : " << fixed << setprecision(2) << konversi3 << " JPY" << endl;
        }
        else if (pilihan == 3) {
            system("cls");
            cout << "Masukkan nominal EUR: ";
            cin >> nominal;
            while (nominal < 0) {
                cout << "Nominal tidak boleh negatif, silahkan masukkan nominal kembali: ";
                cin >> nominal;
            }
            konversi1 = nominal * 17000;
            cout << "Hasil konversi ke Rupiah : " << fixed << setprecision(2) << konversi1 << " IDR" << endl;
            konversi2 = nominal * 1.09;
            cout << "Hasil konversi ke Dollar : " << fixed << setprecision(2) << konversi2 << " USD" << endl;
            konversi3 = nominal * 162;
            cout << "Hasil konversi ke Yen    : " << fixed << setprecision(2) << konversi3 << " JPY" << endl;
        }
        else if (pilihan == 4) {
            system("cls");
            cout << "Masukkan nominal JPY   : ";
            cin >> nominal;
            while (nominal < 0) {
                cout << "Nominal tidak boleh negatif, silahkan masukkan nominal kembali: ";
                cin >> nominal;
            }
            konversi1 = nominal * 104;
            cout << "Hasil konversi ke Rupiah : " << fixed << setprecision(2) << konversi1 << " IDR" << endl;
            konversi2 = nominal / 150;
            cout << "Hasil konversi ke Dollar : " << fixed << setprecision(2) << konversi2 << " USD" << endl;
            konversi3 = nominal / 162;
            cout << "Hasil konversi ke Euro   : " << fixed << setprecision(2) << konversi3 << " EUR" << endl;
        } 
        else if (pilihan == 5) {
            system("cls");
            cout << "Terimakasih telah menggunakan program ini" << endl;
            exit(0);
        }
        else {
            cout << "Pilihan tidak tersedia" << endl;
            cin.clear();
            cin.ignore(INT_MAX, '\n');
            
        }
        cout << endl;
        cout << "Apakah anda ingin kembali melakukan pengkonversian? (y/n): ";
        cin >> ulang;
    }
    system("cls");
    cout << "Terimakasih telah menggunakan program ini" << endl;
    return 0;

}