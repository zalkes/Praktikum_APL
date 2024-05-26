#include <iostream>
#include <windows.h>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <random>
#include <algorithm>
#include <ctime>
#include <locale>

using namespace std;

// Definisi warna konsol
enum ConsoleColor {
    BLACK = 0,
    BLUE = 1,
    GREEN = 2,
    CYAN = 3,
    RED = 4,
    MAGENTA = 5,
    BROWN = 6,
    LIGHTGRAY = 7,
    DARKGRAY = 8,
    LIGHTBLUE = 9,
    LIGHTGREEN = 10,
    LIGHTCYAN = 11,
    LIGHTRED = 12,
    LIGHTMAGENTA = 13,
    YELLOW = 14,
    WHITE = 15
};

// Fungsi Untuk Mengatur Warna Konsol
void setConsoleColor(int textColor, int bgColor = BLACK) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (bgColor << 4) | textColor);
}

// Struct Untuk Data User
struct data_user{
    string nama;
    string no_telp;
    string email;   
    string user;
    string pass;    
};

// Struct Untuk Data Tiket
struct tiket{
    int ekonomi;
    int bisnis;
    int first;
};

// Penggunaan Nested Struct Untuk Data Pesawat
struct data_pesawat{
    string maskapai;
    string kode;
    string tujuan;
    string tanggal;
    string jam;
    tiket harga;
    int jumlah_kursi;
};

// Struct Untuk Data Pemesanan
struct data_pemesanan{
    string nama_pemesan;
    string maskapai_pesan;
    string kode_pesan;
    string tujuan_pesan;
    string tanggal_pesan;
    string jam_pesan;
    string kelas_pesan;
    string kode_unik;
    string waktu_pemesanan;
    int jumlah_tiket;
    int total_harga;
};

// Deklarasi Variabel Global
data_pemesanan pemesanan[1000];
data_user user[1000];
data_pesawat pesawat[1000];

int banyak_pesawat = 0;
int banyak_user = 0;
int total_harga;
int jumlah_tiket;
int index_user;
bool admin = false;
string pilihan_kelas;
int index_pesanan = 0;
int banyak_pesanan = 0;

// Untuk Mengubah Format Hari ke Bahasa Indonesia
string nama_hari(int day) {
    switch (day) {
        case 0: return "Minggu";
        case 1: return "Senin";
        case 2: return "Selasa";
        case 3: return "Rabu";
        case 4: return "Kamis";
        case 5: return "Jumat";
        case 6: return "Sabtu";
        default: return "";
    }
}

// Mendapatkan waktu saat ini
time_t t = time(nullptr);
tm* now = localtime(&t); 
// Menyimpan hari, bulan, dan tahun saat ini
int hari_sekarang = now->tm_mday;
int bulan_sekarang = (now->tm_mon + 1);
int tahun_sekarang = (now->tm_year + 1900);

void main_menu();
void registrasi();
bool login(string username, string password);
bool login_rekursif(int cobalogin);
void tambah_jadwal();
void tampil_jadwal();
void ubah_jadwal();
void hapus_jadwal();
void menu_admin();
void pesan_tiket();
void menu_user();
void cetak_tiket(data_pemesanan &pesanan);

// Error Handling Untuk Inputan String Kosong
void check_string_kosong(string &inputan, string jenis) {
    while (inputan.empty() || inputan == " ") {
        system("cls");
        setConsoleColor(RED);
        cout << "Inputan tidak boleh kosong" << endl;
        setConsoleColor(LIGHTGRAY);
        cout << "Masukkan " << jenis << ": ";
        getline(cin, inputan);
    }
}

// Error Handling Untuk Inputan Bukan Angka
void check_angka(int &inputan, string jenis) {
    while (cin.fail() || inputan <= 0) {
        system("cls");
        setConsoleColor(RED);
        cout << "Inputan harus berupa angka atau lebih dari 0" << endl;
        setConsoleColor(LIGHTGRAY);
        cout << "Masukkan " << jenis << ": ";
        cin.clear();
        cin.ignore(INT_MAX, '\n');
        cin >> inputan;
    }
}

// Error Handling Untuk Mengecek Nama
bool check_nama(string nama){
    for (int i = 0; i < nama.length(); i++){
        if (isdigit(nama[i])){
            system("cls");
            setConsoleColor(RED);
            cout << "Nama Tidak Boleh Mengandung Angka" << endl;
            setConsoleColor(LIGHTGRAY);
            return false;
        }
        if (nama.length() < 3){
            system("cls");
            setConsoleColor(RED);
            cout << "Nama Minimal 3 Karakter" << endl;
            setConsoleColor(LIGHTGRAY);
            return false;
        }
    }
    return true;
}

// Error Handling Untuk Mengecek Nomor Telepon
bool check_telp(string angka){
    for (int i = 0; i < angka.length(); i++){
        if (!isdigit(angka[i])){
            system("cls");
            setConsoleColor(RED);
            cout << "No. Telp Harus Berupa Angka" << endl;
            setConsoleColor(LIGHTGRAY);
            return false;
        }
        if (angka.length() < 11){
            system("cls");
            setConsoleColor(RED);
            cout << "No. Telp Minimal 11 Digit" << endl;
            setConsoleColor(LIGHTGRAY);
            return false;
        }
    }
    return true;
}

// Error Handling Untuk Mengecek Username Apakah Sudah Digunakan
bool cek_username(string username){
    for (int i = 0; i < banyak_user; i++) {
        if (username == user[i].user){
            system("cls");
            setConsoleColor(RED);
            cout << "Username Sudah Digunakan" << endl;
            setConsoleColor(LIGHTGRAY);
            return false;
        }
    }
    return true;
}

// Error Handling Untuk Mengecek Email
bool cek_email(string email){
    for (int i = 0; i < banyak_user; i++) {
        if (email == user[i].email){
            system("cls");
            setConsoleColor(RED);
            cout << "Email Sudah Digunakan" << endl;
            setConsoleColor(LIGHTGRAY);
            return false;
        }
        if (email.find("@") == string::npos || email.find(".") == string::npos){
            system("cls");
            setConsoleColor(RED);
            cout << "Email Tidak Valid" << endl;
            setConsoleColor(LIGHTGRAY);
            return false;
        }
    }
    return true;
}

// Fungsi Untuk Mengimport Data Pesanan dari Database
void import_pesananDatabase(){
    ifstream file;
    string data, nama_pemesan, maskapai_pesanan, kode_pesan, tujuan_pesan, tanggal_pesan, jam_pesan, kelas_pesan, jumlah_tiket, total_harga, kode_unik, waktu_pemesanan;
    int int_jumlah_tiket, int_total_harga;
    int indeks = 0;

    file.open("pesanan_database.csv", ios::in);
    if (file.is_open()) {
        while (getline(file, data)) {
            stringstream ss(data);
            getline(ss, nama_pemesan, ',');
            getline(ss, maskapai_pesanan, ',');
            getline(ss, kode_pesan, ',');
            getline(ss, tujuan_pesan, ',');
            getline(ss, tanggal_pesan, ',');
            getline(ss, jam_pesan, ',');
            getline(ss, kelas_pesan, ',');
            getline(ss, jumlah_tiket, ',');
            getline(ss, total_harga, ',');
            getline(ss, kode_unik , ',');
            getline(ss, waktu_pemesanan, ',');

            stringstream jumlah_tiket_string(jumlah_tiket);
            stringstream total_harga_string(total_harga);

            jumlah_tiket_string >> int_jumlah_tiket;
            total_harga_string >> int_total_harga;

            pemesanan[indeks].nama_pemesan = nama_pemesan;
            pemesanan[indeks].maskapai_pesan = maskapai_pesanan;
            pemesanan[indeks].kode_pesan = kode_pesan;
            pemesanan[indeks].tujuan_pesan = tujuan_pesan;
            pemesanan[indeks].tanggal_pesan = tanggal_pesan;
            pemesanan[indeks].jam_pesan = jam_pesan;
            pemesanan[indeks].kelas_pesan = kelas_pesan;
            pemesanan[indeks].jumlah_tiket = int_jumlah_tiket;
            pemesanan[indeks].total_harga = int_total_harga;
            pemesanan[indeks].kode_unik = kode_unik;
            pemesanan[indeks].waktu_pemesanan = waktu_pemesanan;

            indeks++;
        }
        file.close();
    } 

    banyak_pesanan = indeks;
}

// Fungsi Untuk Mengimport Data Pesawat dari Database
void import_pesawatDatabase(){
    ifstream file;
    string data, maskapai, kode, tujuan, tanggal, jam, harga_ekonomi, harga_bisnis, harga_first, jumlah_kursi;
    int int_harga_ekonomi, int_harga_bisnis, int_harga_first, int_jumlah_kursi;
    int indeks = 0;

    file.open("pesawat_database.csv", ios::in);
    if (file.is_open()) {
        while (getline(file, data)) {
            stringstream ss(data);
            getline(ss, maskapai, ',');
            getline(ss, kode, ',');
            getline(ss, tujuan, ',');
            getline(ss, tanggal, ',');
            getline(ss, jam, ',');
            getline(ss, harga_ekonomi, ',');
            getline(ss, harga_bisnis, ',');
            getline(ss, harga_first, ',');
            getline(ss, jumlah_kursi, ',');

            stringstream harga_ekonomi_string(harga_ekonomi);
            stringstream harga_bisnis_string(harga_bisnis);
            stringstream harga_first_string(harga_first);
            stringstream jumlah_kursi_string(jumlah_kursi);
            
            harga_ekonomi_string >> int_harga_ekonomi;
            harga_bisnis_string >> int_harga_bisnis;
            harga_first_string >> int_harga_first;
            jumlah_kursi_string >> int_jumlah_kursi;

            pesawat[indeks].maskapai = maskapai;
            pesawat[indeks].kode = kode;
            pesawat[indeks].tujuan = tujuan;
            pesawat[indeks].tanggal = tanggal;
            pesawat[indeks].jam = jam;
            pesawat[indeks].harga.ekonomi = int_harga_ekonomi;
            pesawat[indeks].harga.bisnis = int_harga_bisnis;
            pesawat[indeks].harga.first = int_harga_first;
            pesawat[indeks].jumlah_kursi = int_jumlah_kursi;

            indeks++;
        }
        file.close();
    } 

    banyak_pesawat = indeks;
}

// Fungsi Untuk Menyimpan Data Pesawat ke Database
void savedata_pesawat(){
    ofstream file;
    file.open("pesawat_database.csv", ios::out | ios::trunc);

    for (int i = 0; i < banyak_pesawat; i++){
        file << pesawat[i].maskapai << ",";
        file << pesawat[i].kode << ",";
        file << pesawat[i].tujuan << ",";
        file << pesawat[i].tanggal << ",";
        file << pesawat[i].jam << ",";
        file << pesawat[i].harga.ekonomi << ",";
        file << pesawat[i].harga.bisnis << ",";
        file << pesawat[i].harga.first << ",";
        file << pesawat[i].jumlah_kursi << endl;
    }
    file.close();
}

// Fungsi Untuk Menyimpan Data Pemesanan ke Database
void savedata_pemesanan(){
    ofstream file;
    file.open("pesanan_database.csv", ios::out | ios::trunc);

    for (int i = 0; i < banyak_pesanan; i++){
        file << pemesanan[i].nama_pemesan << ",";
        file << pemesanan[i].maskapai_pesan << ",";
        file << pemesanan[i].kode_pesan << ",";
        file << pemesanan[i].tujuan_pesan << ",";
        file << pemesanan[i].tanggal_pesan << ",";
        file << pemesanan[i].jam_pesan << ",";
        file << pemesanan[i].kelas_pesan << ",";
        file << pemesanan[i].jumlah_tiket << ",";
        file << pemesanan[i].total_harga << ",";
        file << pemesanan[i].kode_unik << ",";
        file << pemesanan[i].waktu_pemesanan << endl;
    }
    file.close();
}

//Fungsi Untuk Membuat Output String Menjadi Kapital
string uppercase(string x) {
    transform(x.begin(), x.end(), x.begin(), ::toupper);
    return x;
}

//Selection Sort Nama Maskapai Secara Ascending
void sorting_maskapai_ascending(data_pesawat nama[]){
    int i, j, min_idx;
    for (i = 0; i < banyak_pesawat - 1; i++) {
        min_idx = i;
        for (j = i + 1; j < banyak_pesawat; j++) {
            if (uppercase(nama[j].maskapai) < uppercase(nama[min_idx].maskapai)) {
                min_idx = j;
            }
        }
        // Swap dilakukan di luar loop j
        if (min_idx != i) {
            data_pesawat swap = nama[min_idx];
            nama[min_idx] = nama[i];
            nama[i] = swap;
        }
    }
    cout << endl;
}

//Selection Sort Nama Maskapai Secara Descending
void sorting_maskapai_descending(data_pesawat nama[]){
    int i, j, min_idx;
    for (i = 0; i < banyak_pesawat - 1; i++) {
        min_idx = i;
        for (j = i + 1; j < banyak_pesawat; j++) {
            if (uppercase(nama[j].maskapai) > uppercase(nama[min_idx].maskapai)) {
                min_idx = j;
            }
        }
        // Swap dilakukan di luar loop j
        if (min_idx != i) {
            data_pesawat swap = nama[min_idx];
            nama[min_idx] = nama[i];
            nama[i] = swap;
        }
    }
    cout << endl;
}

//Bubble Sort Untuk Harga Tiket Ekonomi Termurah
void sorting_harga_bisnis_termurah(data_pesawat nama[]){
    for (int i = 0; i < banyak_pesawat - 1; i++) {
        for (int j = 0; j < banyak_pesawat - i - 1; j++) {
            if (nama[j].harga.bisnis > nama[j + 1].harga.bisnis) {
                swap(nama[j], nama[j + 1]);
            }
        }
    }
}

// Bubble Sort Untuk Harga Tiket Ekonomi Termurah
void sorting_harga_ekonomi_termurah(data_pesawat nama[]){
    for (int i = 0; i < banyak_pesawat - 1; i++) {
        for (int j = 0; j < banyak_pesawat - i - 1; j++) {
            if (nama[j].harga.ekonomi > nama[j + 1].harga.ekonomi) {
                swap(nama[j], nama[j + 1]);
            }
        }
    }
}

// Bubble Sort Untuk Harga Tiket First Termurah
void sorting_harga_first_termurah(data_pesawat nama[]){
    for (int i = 0; i < banyak_pesawat - 1; i++) {
        for (int j = 0; j < banyak_pesawat - i - 1; j++) {
            if (nama[j].harga.first > nama[j + 1].harga.first) {
                swap(nama[j], nama[j + 1]);
            }
        }
    }
}

// Fungsi Untuk Menampilkan Menu Sorting Jadwal Penerbangan
void sorting_jadwal(){
    while(true){
        system("cls");
        tampil_jadwal();
        setConsoleColor(LIGHTBLUE);
        cout << "\n==============================================================" << endl;
        cout << "=====================    Menu Sorting    =====================" << endl;
        cout << "==============================================================" << endl;
        setConsoleColor(LIGHTGRAY);
        setConsoleColor(LIGHTCYAN);
        cout << "[1] Maskapai Pesawat (Ascending)" << endl;
        cout << "[2] Maskapai Pesawat (Descending)" << endl;
        cout << "[3] Harga Kursi Ekonomi Termurah" << endl;
        cout << "[4] Harga Kursi Bisnis Termurah" << endl;
        cout << "[5] Harga Kursi First Termurah" << endl;
        setConsoleColor(LIGHTGRAY);
        setConsoleColor(LIGHTBLUE);
        cout << "==============================================================" << endl;
        cout << "       Tampilkan Jadwal sesuai Urutan yang di Inginkan        " << endl;
        cout << "==============================================================" << endl;
        setConsoleColor(LIGHTGRAY);
        cout << "\nMasukkan Pilihan : ";
        string pilihan_sorting;
        cin >> pilihan_sorting;
        if(pilihan_sorting == "1"){
            sorting_maskapai_ascending(pesawat);
            tampil_jadwal();
            break;
        }
        else if(pilihan_sorting == "2"){
            sorting_maskapai_descending(pesawat);
            tampil_jadwal();
            break;
        }
        else if(pilihan_sorting == "3"){
            sorting_harga_ekonomi_termurah(pesawat);
            tampil_jadwal();
            break;
        }
        else if(pilihan_sorting == "4"){
            sorting_harga_bisnis_termurah(pesawat);
            tampil_jadwal();
            break;
        }
        else if(pilihan_sorting == "5"){
            sorting_harga_first_termurah(pesawat);
            tampil_jadwal();
            break;
        }
        else{
            system("cls");
            setConsoleColor(RED);
            cout << "==============================================================" << endl;
            cout << "==================    Pilihan Tidak Ada    ===================" << endl;
            cout << "==============================================================" << endl;
            setConsoleColor(LIGHTGRAY);
            Sleep(1000);
        }
    }
}

// Fungsi Untuk Melakukan Pencarian Jadwal Penerbangan
void searching_jadwal(){
    system("cls");
    tampil_jadwal();
    int cari;
    string tes_kode;
    bool found = false;
    setConsoleColor(LIGHTBLUE);
    cout << "==============================================================" << endl;
    cout << "        Searching Jadwal Penerbangan Berdasarkan Kode         " << endl;
    cout << "==============================================================" << endl;
    setConsoleColor(LIGHTGRAY);
    cout << "\nMasukkan Kode Penerbangan : ";
    string cari_kode;
    cin >> cari_kode;
    for (int i = 0; i < banyak_pesawat; i++) {
        if (pesawat[i].kode == cari_kode) {
            found = true;
            system("cls");
            setConsoleColor(LIGHTBLUE);
            cout << "==============================================================" << endl;
            setConsoleColor(LIGHTMAGENTA);
            cout << "                   "<< uppercase(pesawat[i].maskapai) << " (" << pesawat[i].kode << ")" << endl;
            setConsoleColor(LIGHTBLUE);
            cout << "==============================================================" << endl;
            setConsoleColor(LIGHTGRAY);
            cout << "Tujuan                : " << uppercase(pesawat[i].tujuan) << endl;
            cout << "Tanggal Keberangkatan : " << pesawat[i].tanggal << endl;
            cout << "Jam Keberangkatan     : " << pesawat[i].jam << endl;
            cout << "Kursi Tersedia        : " << pesawat[i].jumlah_kursi << " Kursi" << endl;
            cout << "Harga Tiket Ekonomi   : Rp." << pesawat[i].harga.ekonomi << endl;
            cout << "Harga Tiket Bisnis    : Rp." << pesawat[i].harga.bisnis << endl;
            cout << "Harga Tiket First     : Rp." << pesawat[i].harga.first << endl;
            setConsoleColor(LIGHTBLUE);
            cout << "==============================================================" << endl;
            setConsoleColor(LIGHTGRAY);
        }
    }
    if (!found) {
        system("cls");
        setConsoleColor(RED);
        cout << "==============================================================" << endl;
        cout << "=========    Jadwal Penerbangan Tidak Ditemukan    ===========" << endl;
        cout << "==============================================================" << endl;
        setConsoleColor(LIGHTGRAY);
        Sleep(1000);
    }  
}

// Fungsi Untuk Membuat Kode Pesawat Secara Acak
string kode_generator(){
    //kode pesawat dengan random number generator
    string kodeGenerator;
    random_device rand_dev;
    mt19937 generator(rand_dev());
    uniform_int_distribution<int> distr(10000, 99999);
    kodeGenerator = to_string(distr(generator));
    for (int i = 0; i < banyak_pesawat; i++) {
        if (to_string(distr(generator)) == pesawat[i].kode){
            return kode_generator();
        }
    }
    return kodeGenerator;
}

// Fungsi Untuk Membuat Kode Tiket Secara Acak
string kode_unik_generator(){
    //uniqe code pesawat dengan random number generator
    string kodeUnikGenerator;
    random_device rand_dev;
    mt19937 generator(rand_dev());
    uniform_int_distribution<int> distr(10000000, 99999999);
    kodeUnikGenerator = to_string(distr(generator));
    for (int i = 0; i < banyak_pesawat; i++) {
        if (to_string(distr(generator)) == pemesanan[i].kode_unik){
            return kode_unik_generator();
        }
    }
    return kodeUnikGenerator;
}

// Fungsi Untuk Mengambil Waktu dan Tanggal Saat Ini
string tanggal_waktu() {
    // Mendapatkan waktu saat ini
    time_t t = time(nullptr);
    tm* now = localtime(&t); // Penggunaan Pointer untuk mengakses waktu saat ini

    // Mengubah nama hari dalam bahasa Indonesia
    string hari = nama_hari(now->tm_wday);

    // Menampilkan tanggal dan waktu dalam format Indonesia
    ostringstream oss;
    oss << hari << " " << std::put_time(now, "%d-%m-%Y %H:%M:%S");
    return oss.str();
}

// Fungsi Untuk Menyimpan Data User ke Database
void savedata_user(){
    ofstream file;
    file.open("user_database.csv", ios::out | ios::app);
    file << user[banyak_user].nama << ",";
    file << user[banyak_user].no_telp << ",";
    file << user[banyak_user].email << ",";
    file << user[banyak_user].user << ",";
    file << user[banyak_user].pass << endl;
    file.close();
}

// Fungsi Untuk Menampilkan Menu Konfirmasi
void menu_konfirmasi(string jenis_menu){
    system("cls");
    string konfirmasi;
    setConsoleColor(LIGHTBLUE);
    cout << "==============================================================" << endl;
    cout << "=====================    Pilih Menu    =======================" << endl;
    cout << "==============================================================" << endl;
    cout << "[1] Lanjut" << endl;
    cout << "[2] Kembali" << endl;
    cout << "==============================================================" << endl;
    setConsoleColor(LIGHTGRAY);
    cout << "Masukkan Pilihan : ";
    cin >> konfirmasi;
    if (konfirmasi == "1"){
        system("cls");
    }
    else if (konfirmasi == "2"){
        system("cls");
        if (jenis_menu == "admin"){
            menu_admin();
        }
        else if (jenis_menu == "user"){
            menu_user();
        }
        else if (jenis_menu == "main"){
            main_menu();
        }
    }
    else{
        system("cls");
        setConsoleColor(RED);
        cout << "==============================================================" << endl;
        cout << "==================    Pilihan Tidak Ada    ===================" << endl;
        cout << "==============================================================" << endl;
        setConsoleColor(LIGHTGRAY);
        Sleep(1000);
        menu_konfirmasi(jenis_menu);
    }
}

// Fungsi Untuk Registrasi Akun
void registrasi(){
    menu_konfirmasi("main");
    system("cls");
    setConsoleColor(LIGHTBLUE);
    cout << "==============================================================" << endl;
    cout << "====================    Registrasi Akun    ===================" << endl;
    cout << "==============================================================" << endl;
    setConsoleColor(LIGHTGRAY);
    cout << "Masukkan Nama : ";
    cin.ignore();
    getline(cin, user[banyak_user].nama);
    check_string_kosong(user[banyak_user].nama, "Nama");
    while(check_nama(user[banyak_user].nama) == false){
        cout << "Masukkan Nama : ";
        getline(cin, user[banyak_user].nama);
        check_string_kosong(user[banyak_user].nama, "Nama");
    }

    cout << "Masukkan No.Telp : ";
    cin >> user[banyak_user].no_telp;
    check_telp(user[banyak_user].no_telp);
    while (check_telp(user[banyak_user].no_telp) == false){
        cout << "Masukkan No.Telp : ";
        cin >> user[banyak_user].no_telp;
        check_telp(user[banyak_user].no_telp);
    }

    cout << "Masukkan Email : ";
    cin >> user[banyak_user].email;
    cek_email(user[banyak_user].email);
    while (cek_email(user[banyak_user].email) == false){
        cout << "Masukkan Email : ";
        cin >> user[banyak_user].email;
        cek_email(user[banyak_user].email);
    }

    cout << "Masukkan Username : ";
    cin >> user[banyak_user].user;
    check_string_kosong(user[banyak_user].user, "Username");
    while (cek_username(user[banyak_user].user) == false || user[banyak_user].user == "admin"){
        cout << "Masukkan Username : ";
        cin >> user[banyak_user].user;
        cek_username(user[banyak_user].user);
        check_string_kosong(user[banyak_user].user, "Username");
    }

    cout << "Masukkan Password : ";
    cin >> user[banyak_user].pass;
    check_string_kosong(user[banyak_user].pass, "Password");

    while(true){
        system("cls");
        setConsoleColor(LIGHTBLUE);
        cout << "==============================================================" << endl;
        setConsoleColor(LIGHTGRAY);
        cout << "Nama         : " << user[banyak_user].nama << endl;
        cout << "No. Telp     : " << user[banyak_user].no_telp << endl;
        cout << "Email        : " << user[banyak_user].email << endl;
        setConsoleColor(LIGHTBLUE);
        cout << "==============================================================" << endl;
        setConsoleColor(LIGHTGRAY);
        cout << "\nApakah Data Di atas Sudah Benar? (y/n): ";
        string confirm;
        cin >> confirm;
        if (confirm == "y" || confirm == "Y"){
            break;

        } else if (confirm == "n" || confirm == "N"){
            system("cls");
            setConsoleColor(RED);
            cout << "==============================================================" << endl;
            cout << "================    Registrasi Dibatalkan    =================" << endl;
            cout << "==============================================================" << endl;
            setConsoleColor(LIGHTGRAY);
            Sleep(1000);
            return;

        } else {
            system("cls");
            setConsoleColor(RED);
            cout << "==============================================================" << endl;
            cout << "==================    Pilihan Tidak Ada    ===================" << endl;
            cout << "==============================================================" << endl;
            setConsoleColor(LIGHTGRAY);
            Sleep(1000);
        }
    }
    savedata_user();
    banyak_user++;
    system("cls");
    setConsoleColor(LIGHTGREEN);
    cout << "==============================================================" << endl;
    cout << "================    Registrasi Berhasil    ===================" << endl;
    cout << "==============================================================" << endl;
    setConsoleColor(LIGHTGRAY);
    Sleep(1000);
}

// Fungsi Untuk Mengimport Data User dari Database
void import_userDatabase() {
    ifstream file;
    string data, nama, telp, mail, username, passwd;
    int indeks = 0;

    file.open("user_database.csv", ios::in);
    if (file.is_open()) {
        while (getline(file, data)) {
            stringstream ss(data);
            getline(ss, nama, ',');
            getline(ss, telp, ',');
            getline(ss, mail, ',');
            getline(ss, username, ',');
            getline(ss, passwd, '\n');

            user[indeks].nama = nama;
            user[indeks].no_telp = telp;
            user[indeks].email = mail;
            user[indeks].user = username;
            user[indeks].pass = passwd;

            indeks++;
        }
        file.close();
    } 

    banyak_user = indeks;
}

// Fungsi Login
bool login(string username, string password) {
    string nama_pemesan;
    if (username == "admin" && password == "admin") {
        admin = true;
        system("cls");
        cout << "Anda Telah Login Sebagai Admin" << endl;
        Sleep(1000);
        return true;
    } else {
        admin = false;
        for (int i = 0; i < banyak_user; i++) {
            if (username == user[i].user && password == user[i].pass) {
                nama_pemesan = user[i].nama;
                index_user = i;
                system("cls");
                cout << "Selamat Datang, " << nama_pemesan << endl;
                Sleep(1000);
                return true;
            }
        }
        system("cls");
        setConsoleColor(RED);
        cout << "==============================================================" << endl;
        cout << "============    Username atau Password Salah    ==============" << endl;
        cout << "==============================================================" << endl;
        setConsoleColor(LIGHTGRAY);
        Sleep(1000);
        return false;
    }
}

// Fungsi Login Rekursif
bool login_rekursif(int cobalogin = 0){
    system("cls");
    if(cobalogin == 3){
        setConsoleColor(RED);
        cout << "===============================================================" << endl;
        cout << "Anda sudah mencoba login sebanyak 3 kali, program akan terhenti" << endl;
        cout << "===============================================================" << endl;
        setConsoleColor(LIGHTGRAY);
        Sleep(1000);
        exit(0);
    }
    string username, password;
    menu_konfirmasi("main");
    setConsoleColor(LIGHTBLUE);
    cout << "==============================================================" << endl;
    cout << "====================    Silahkan Login    ====================" << endl;
    cout << "==============================================================" << endl;
    setConsoleColor(LIGHTGRAY);
    cout << "Masukkan Username : ";
    cin >> username;
    cout << "Masukkan Password : ";
    cin >> password;
    if(login(username, password)){
        return true;
    }
    else{
        return login_rekursif(cobalogin + 1);
    }
}

// Fungsi Untuk Menambah Jadwal Penerbangan
void tambah_jadwal(){
    menu_konfirmasi("admin");
    int hari, bulan, tahun, jam, menit;
    system("cls");
    cout << "Masukkan Maskapai : ";
    cin.ignore();
    getline(cin, pesawat[banyak_pesawat].maskapai);
    check_string_kosong(pesawat[banyak_pesawat].maskapai, "Maskapai");

    cout << "Masukkan Tujuan : ";
    getline(cin, pesawat[banyak_pesawat].tujuan);
    check_string_kosong(pesawat[banyak_pesawat].tujuan, "Tujuan");

    while(true){
        cout << "Masukkan Tanggal Keberangkatan (Format: dd/mm/yyyy)";
        cout << "\nMasukkan Hari : ";
        cin >> hari;
        check_angka(hari, "Hari");
        while (hari > 31){
            system("cls");
            setConsoleColor(RED);
            cout << "Hari tidak boleh lebih dari 31" << endl;
            setConsoleColor(LIGHTGRAY);
            cout << "Masukkan Hari : ";
            cin >> hari;
            check_angka(hari, "Hari");
        }
        cout << "Masukkan Bulan : ";
        cin >> bulan;
        check_angka(bulan, "Bulan");
        while (bulan > 12){
            system("cls");
            setConsoleColor(RED);
            cout << "Bulan tidak boleh lebih dari 12" << endl;
            setConsoleColor(LIGHTGRAY);
            cout << "Masukkan Bulan : ";
            cin >> bulan;
            check_angka(bulan, "Bulan");
        }
        cout << "Masukkan Tahun : ";
        cin >> tahun;
        check_angka(tahun, "Tahun");
        while (tahun < 2024){
            system("cls");
            setConsoleColor(RED);
            cout << "Tahun tidak boleh kurang dari 2024" << endl;
            setConsoleColor(LIGHTGRAY);
            cout << "Masukkan Tahun : ";
            cin >> tahun;
            check_angka(tahun, "Tahun");
        }
        if (tahun < tahun_sekarang || (tahun == tahun_sekarang && bulan < bulan_sekarang) || 
           (tahun == tahun_sekarang && bulan == bulan_sekarang && hari < hari_sekarang)){
            system("cls");
            setConsoleColor(RED);
            cout << "========================================================" << endl;
            cout << "   Tanggal tidak boleh kurang dari tanggal sekarang" << endl;
            cout << "--------------------------------------------------------" << endl;
            cout << "             Tanggal Sekarang : " << hari_sekarang << "/" << bulan_sekarang << "/" << tahun_sekarang << endl;
            cout << "========================================================" << endl;
            setConsoleColor(LIGHTGRAY);
        }
        else {
            break;
        }
    }
    if (hari < 10 && bulan < 10){
        pesawat[banyak_pesawat].tanggal = "0" + to_string(hari) + "/0" + to_string(bulan) + "/" + to_string(tahun);
    } else if (hari < 10){
        pesawat[banyak_pesawat].tanggal = "0" + to_string(hari) + "/" + to_string(bulan) + "/" + to_string(tahun);
    } else if (bulan < 10){
        pesawat[banyak_pesawat].tanggal = to_string(hari) + "/0" + to_string(bulan) + "/" + to_string(tahun);
    } else {
        pesawat[banyak_pesawat].tanggal = to_string(hari) + "/" + to_string(bulan) + "/" + to_string(tahun);
    }
    cout << "Masukkan Jam Keberangkatan (Format: Jam:Menit) ";
    cout << "\nMasukkan Jam : ";
    cin >> jam;
    check_angka(jam, "Jam");
    while (jam > 24){
        system("cls");
        setConsoleColor(RED);
        cout << "Jam tidak boleh lebih dari 24" << endl;
        setConsoleColor(LIGHTGRAY);
        cout << "Masukkan Jam : ";
        cin >> jam;
        check_angka(jam, "Jam");
    }
    cout << "Masukkan Menit : ";
    cin >> menit;
    while (cin.fail() || menit >= 60 || menit < 0){
        system("cls");
        setConsoleColor(RED);
        cout << "Menit tidak boleh lebih dari atau sama dengan 60 maupun negatif" << endl;
        setConsoleColor(LIGHTGRAY);
        cout << "Masukkan Menit : ";
        cin.clear();
        cin.ignore(INT_MAX, '\n');
        cin >> menit;
    }
    if (jam < 10 && menit < 10){
        pesawat[banyak_pesawat].jam = "0" + to_string(jam) + ":0" + to_string(menit);
    } else if (jam < 10){
        pesawat[banyak_pesawat].jam = "0" + to_string(jam) + ":" + to_string(menit);
    } else if (menit < 10){
        pesawat[banyak_pesawat].jam = to_string(jam) + ":0" + to_string(menit);
    } else {
        pesawat[banyak_pesawat].jam = to_string(jam) + ":" + to_string(menit);
    }
    cout << "Masukkan Harga Tiket Ekonomi : ";
    cin >> pesawat[banyak_pesawat].harga.ekonomi;
    check_angka(pesawat[banyak_pesawat].harga.ekonomi, "Harga Tiket Ekonomi");

    cout << "Masukkan Harga Tiket Bisnis : ";
    cin >> pesawat[banyak_pesawat].harga.bisnis;
    check_angka(pesawat[banyak_pesawat].harga.bisnis, "Harga Tiket Bisnis");

    cout << "Masukkan Harga Tiket First : ";
    cin >> pesawat[banyak_pesawat].harga.first;
    check_angka(pesawat[banyak_pesawat].harga.first, "Harga Tiket First");

    cout << "Masukkan Jumlah Kursi : ";
    cin >> pesawat[banyak_pesawat].jumlah_kursi;
    check_angka(pesawat[banyak_pesawat].jumlah_kursi, "Jumlah Kursi");
    while (pesawat[banyak_pesawat].jumlah_kursi < 50){
        system("cls");
        setConsoleColor(RED);
        cout << "Jumlah Kursi tidak boleh kurang dari 50" << endl;
        setConsoleColor(LIGHTGRAY);
        cout << "Masukkan Jumlah Kursi : ";
        cin >> pesawat[banyak_pesawat].jumlah_kursi;
        check_angka(pesawat[banyak_pesawat].jumlah_kursi, "Jumlah Kursi");
    }

    pesawat[banyak_pesawat].kode = kode_generator();
    
    while(true){
        system("cls");
        setConsoleColor(LIGHTBLUE);
        cout << "==============================================================" << endl;
        setConsoleColor(LIGHTGRAY);
        cout << "Nama Maskapai         : " << pesawat[banyak_pesawat].maskapai << endl;
        cout << "Kode Pesawat          : " << pesawat[banyak_pesawat].kode << endl;
        cout << "Tujuan                : " << pesawat[banyak_pesawat].tujuan << endl;
        cout << "Tanggal Keberangkatan : " << pesawat[banyak_pesawat].tanggal << endl;
        cout << "Jam Keberangkatan     : " << pesawat[banyak_pesawat].jam << endl;
        cout << "Harga Tiket Ekonomi   : " << "Rp." << pesawat[banyak_pesawat].harga.ekonomi << endl;
        cout << "Harga Tiket Bisnis    : " << "Rp." << pesawat[banyak_pesawat].harga.bisnis << endl;
        cout << "Harga Tiket First     : " << "Rp." << pesawat[banyak_pesawat].harga.first << endl;
        cout << "Jumlah Kursi          : " << pesawat[banyak_pesawat].jumlah_kursi << endl;
        setConsoleColor(LIGHTBLUE);
        cout << "==============================================================" << endl;
        setConsoleColor(LIGHTGRAY);
        cout << "\nApakah Data Di atas Sudah Benar? (y/n): ";
        string confirm;
        cin >> confirm;
        if (confirm == "y" || confirm == "Y"){
            break;

        } else if (confirm == "n" || confirm == "N"){
            system("cls");
            setConsoleColor(RED);
            cout << "==============================================================" << endl;
            cout << "=============    Penambahan Jadwal Dibatalkan    =============" << endl;
            cout << "==============================================================" << endl;
            setConsoleColor(LIGHTGRAY);
            Sleep(1000);
            menu_admin();

        } else {
            system("cls");
            setConsoleColor(RED);
            cout << "==============================================================" << endl;
            cout << "==================    Pilihan Tidak Ada    ===================" << endl;
            cout << "==============================================================" << endl;
            setConsoleColor(LIGHTGRAY);
            Sleep(1000);
        }
    }

    banyak_pesawat++;
    savedata_pesawat();
    system("cls");
    setConsoleColor(LIGHTGREEN);
    cout << "==============================================================" << endl;
    cout << "================    Penambahan Jadwal Berhasil   =============" << endl;
    cout << "==============================================================" << endl;
    setConsoleColor(LIGHTGRAY);
    cout << "\nTekan Enter untuk kembali ke menu admin";
    cin.ignore();
    cin.get();
    menu_admin();
}

// Fungsi Untuk Menampilkan Jadwal Penerbangan
void tampil_jadwal() {
    system("cls");
    if (banyak_pesawat == 0) {
        setConsoleColor(RED);
        cout << "==============================================================" << endl;
        cout << "==================    Jadwal Kosong    =======================" << endl;
        cout << "==============================================================" << endl;
        setConsoleColor(LIGHTGRAY);
        Sleep(1000);
    }
    else{
    setConsoleColor(LIGHTBLUE);
    cout << "=======================================================================================================================================================================" << endl;
    cout << "=========================================================================   Jadwal Penerbangan    =====================================================================" << endl;
    cout << "================================================================  BANDARA SULTAN AJI MUHAMMAD BALIKPAPAN  =============================================================" << endl;
    cout << "=======================================================================================================================================================================" << endl;
    setConsoleColor(LIGHTCYAN);
    cout << left << setw(5) << "No"
         << setw(20) << "Maskapai"
         << setw(10) << "Kode"
         << setw(15) << "Tujuan"
         << setw(15) << "Tanggal"
         << setw(10) << "Jam"
         << setw(25) << "Harga Tiket Ekonomi"
         << setw(25) << "Harga Tiket Bisnis"
         << setw(25) << "Harga Tiket First" 
         << setw(15) << "Kursi Tersedia" << endl;
    setConsoleColor(LIGHTBLUE);
    cout << setfill('=') << setw(167) << "=" << setfill(' ') << endl;
    setConsoleColor(LIGHTGRAY);
    for (int i = 0; i < banyak_pesawat; i++) {
        cout << left << setw(5) << i + 1
             << setw(20) << pesawat[i].maskapai
             << setw(10) << pesawat[i].kode
             << setw(15) << pesawat[i].tujuan
             << setw(15) << pesawat[i].tanggal
             << setw(16) << pesawat[i].jam
             << setw(24) << pesawat[i].harga.ekonomi
             << setw(25) << pesawat[i].harga.bisnis
             << setw(26) << pesawat[i].harga.first
             << setw(15) << pesawat[i].jumlah_kursi << endl;
    }
    setConsoleColor(LIGHTBLUE);
    cout << setfill('=') << setw(167) << "=" << setfill(' ') << endl;
    setConsoleColor(LIGHTGRAY);
    }
}

// Fungsi Untuk Menampilkan Pilihan Menu Sorting/Searching Pada Saat Menampilkan Jadwal Penerbangan
void menu_tampil_jadwal(){
    if (banyak_pesawat == 0) {
        system("cls");
        setConsoleColor(RED);
        cout << "==============================================================" << endl;
        cout << "==================    Jadwal Kosong    =======================" << endl;
        cout << "==============================================================" << endl;
        setConsoleColor(LIGHTGRAY);
        Sleep(1000);
    }
    else {
        while(true){
            tampil_jadwal();
            setConsoleColor(LIGHTBLUE);
            cout << "\n==============================================================" << endl;
            cout << "======================    Pilih Menu    ======================" << endl;
            cout << "==============================================================" << endl;
            setConsoleColor(LIGHTGRAY);
            setConsoleColor(LIGHTCYAN);
            cout << "[1] Sorting Jadwal Penerbangan" << endl;
            cout << "[2] Searching Jadwal Penerbangan" << endl;
            cout << "[3] Kembali" << endl;
            setConsoleColor(LIGHTGRAY);
            setConsoleColor(LIGHTBLUE);
            cout << "==============================================================" << endl;
            setConsoleColor(LIGHTGRAY);
            cout << "Masukkan Pilihan : ";
            string pilihan_jadwal;
            cin >> pilihan_jadwal;
            if(pilihan_jadwal == "1"){
                sorting_jadwal();
                cout << "\nTekan Enter untuk kembali";
                cin.ignore();
                cin.get();
            }
            else if (pilihan_jadwal == "2") {
                searching_jadwal();
                cout << "\nTekan Enter untuk kembali";
                cin.ignore();
                cin.get();
            }
            else if (pilihan_jadwal == "3") {
                break;
            }
            else{
                system("cls");
                setConsoleColor(RED);
                cout << "===============================================================" << endl;
                cout << "================    Pilihan Tidak Ditemukan    ================" << endl;
                cout << "===============================================================" << endl;
                setConsoleColor(LIGHTGRAY);
                Sleep(1000);
            }
        }
    }
}

// Fungsi Untuk Mengubah Data Jadwal Penerbangan
void ubah_jadwal(){
    system("cls");
    if (banyak_pesawat == 0) {
        setConsoleColor(RED);
        cout << "==============================================================" << endl;
        cout << "==================    Jadwal Kosong    =======================" << endl;
        cout << "==============================================================" << endl;
        setConsoleColor(LIGHTGRAY);
        Sleep(1000);
        menu_admin();
    }
    else{
        menu_konfirmasi("admin");
        tampil_jadwal();
        cout << "==============================================================" << endl;
        cout << "Masukkan Kode Pesawat yang ingin diubah datanya : ";
        string kode;
        cin >> kode;
        bool found = false;
        for(int i = 0; i < banyak_pesawat; i++){
            if(kode == pesawat[i].kode){
                found = true;
                while(true){
                    system("cls");
                    setConsoleColor(LIGHTBLUE);
                    cout << "==============================================================================" << endl;
                    setConsoleColor(LIGHTGRAY);
                    cout << "Melakukan Perubahan Data pada Pesawat " << pesawat[i].maskapai << " (" << pesawat[i].kode << ")" << endl;
                    cout << "------------------------------------------------------------------------------" << endl;
                    cout << "[1] Ubah Tanggal" << endl;
                    cout << "[2] Ubah Jam" << endl;
                    cout << "[3] Ubah Harga Tiket Ekonomi" << endl;
                    cout << "[4] Ubah Harga Tiket Bisnis" << endl;
                    cout << "[5] Ubah Harga Tiket First" << endl;
                    setConsoleColor(LIGHTBLUE);
                    cout << "==============================================================================" << endl;
                    setConsoleColor(LIGHTGRAY);
                    cout << "Masukkan Pilihan : ";
                    string pilihan_ubah;
                    cin >> pilihan_ubah;
                    if(pilihan_ubah == "1"){
                        int hari, bulan, tahun;
                        system("cls");
                        while (true){
                            cout << "Masukkan Tanggal Keberangkatan Baru (Format: dd/mm/yyyy) ";
                            cout << "\nMasukkan Hari : ";
                            cin >> hari;
                            check_angka(hari, "Hari");
                            while (hari > 31){
                                system("cls");
                                setConsoleColor(RED);
                                cout << "Hari tidak boleh lebih dari 31" << endl;
                                setConsoleColor(LIGHTGRAY);
                                cout << "Masukkan Hari : ";
                                cin >> hari;
                                check_angka(hari, "Hari");
                            }
                            cout << "Masukkan Bulan : ";
                            cin >> bulan;
                            check_angka(bulan, "Bulan");
                            while (bulan > 12){
                                system("cls");
                                setConsoleColor(RED);
                                cout << "Bulan tidak boleh lebih dari 12" << endl;
                                setConsoleColor(LIGHTGRAY);
                                cout << "Masukkan Bulan : ";
                                cin >> bulan;
                                check_angka(bulan, "Bulan");
                            }
                            cout << "Masukkan Tahun : ";
                            cin >> tahun;
                            check_angka(tahun, "Tahun");
                            while (tahun < 2024){
                                system("cls");
                                setConsoleColor(RED);
                                cout << "Tahun tidak boleh kurang dari 2024" << endl;
                                setConsoleColor(LIGHTGRAY);
                                cout << "Masukkan Tahun : ";
                                cin >> tahun;
                                check_angka(tahun, "Tahun");
                            }
                            if (tahun < tahun_sekarang || (tahun == tahun_sekarang && bulan < bulan_sekarang) || 
                                (tahun == tahun_sekarang && bulan == bulan_sekarang && hari < hari_sekarang)){
                                system("cls");
                                setConsoleColor(RED);
                                cout << "========================================================" << endl;
                                cout << "   Tanggal tidak boleh kurang dari tanggal sekarang" << endl;
                                cout << "--------------------------------------------------------" << endl;
                                cout << "             Tanggal Sekarang : " << hari_sekarang << "/" << bulan_sekarang << "/" << tahun_sekarang << endl;
                                cout << "========================================================" << endl;
                                setConsoleColor(LIGHTGRAY);
                            }
                            else {
                                break;
                            }
                        }
                        if (hari < 10 && bulan < 10){
                            pesawat[i].tanggal = "0" + to_string(hari) + "/0" + to_string(bulan) + "/" + to_string(tahun);
                        } else if (hari < 10){
                            pesawat[i].tanggal = "0" + to_string(hari) + "/" + to_string(bulan) + "/" + to_string(tahun);
                        } else if (bulan < 10){
                            pesawat[i].tanggal = to_string(hari) + "/0" + to_string(bulan) + "/" + to_string(tahun);
                        } else {
                            pesawat[i].tanggal = to_string(hari) + "/" + to_string(bulan) + "/" + to_string(tahun);
                        }    
                    }
                    else if(pilihan_ubah == "2"){
                        int jam, menit;
                        system("cls");
                        cout << "Masukkan Jam Keberangkatan Baru (Format: Jam:Menit) ";
                        cout << "\nMasukkan Jam : ";
                        cin >> jam;
                        check_angka(jam, "Jam");
                        while (jam > 24){
                            system("cls");
                            setConsoleColor(RED);
                            cout << "Jam tidak boleh lebih dari 24" << endl;
                            setConsoleColor(LIGHTGRAY);
                            cout << "Masukkan Jam : ";
                            cin >> jam;
                            check_angka(jam, "Jam");
                        }
                        cout << "Masukkan Menit : ";
                        cin >> menit;
                        while (cin.fail() || menit >= 60 || menit < 0){
                            system("cls");
                            setConsoleColor(RED);
                            cout << "Menit tidak boleh lebih dari atau sama dengan 60 maupun negatif" << endl;
                            setConsoleColor(LIGHTGRAY);
                            cout << "Masukkan Menit : ";
                            cin.clear();
                            cin.ignore(INT_MAX, '\n');
                            cin >> menit;
                        }
                        if (jam < 10 && menit < 10){
                            pesawat[i].jam = "0" + to_string(jam) + ":0" + to_string(menit);
                        } else if (jam < 10){
                            pesawat[i].jam = "0" + to_string(jam) + ":" + to_string(menit);
                        } else if (menit == 0){
                            pesawat[i].jam = to_string(jam) + ":00";    
                        } else if (menit < 10 && menit > 0){
                            pesawat[i].jam = to_string(jam) + ":0" + to_string(menit);
                        } else {
                            pesawat[i].jam = to_string(jam) + ":" + to_string(menit);
                        }
                    }
                    else if(pilihan_ubah == "3"){
                        system("cls");
                        cout << "Masukkan Harga Tiket Ekonomi Baru : ";
                        cin >> pesawat[i].harga.ekonomi;
                        check_angka(pesawat[i].harga.ekonomi, "Harga Tiket Ekonomi");
                    }
                    else if(pilihan_ubah == "4"){
                        system("cls");
                        cout << "Masukkan Harga Tiket Bisnis Baru : ";
                        cin >> pesawat[i].harga.bisnis;
                        check_angka(pesawat[i].harga.bisnis, "Harga Tiket Bisnis");
                    }
                    else if(pilihan_ubah == "5"){
                        system("cls");
                        cout << "Masukkan Harga Tiket First Baru : ";
                        cin >> pesawat[i].harga.first;
                        check_angka(pesawat[i].harga.first, "Harga Tiket First");
                    }
                    else {
                        system("cls");
                        setConsoleColor(RED);
                        cout << "==============================================================" << endl;
                        cout << "===================    Pilihan Tidak ada    ==================" << endl;
                        cout << "==============================================================" << endl;
                        setConsoleColor(LIGHTGRAY);
                        Sleep(1000);
                    }
                    if (pilihan_ubah == "1" || pilihan_ubah == "2" || pilihan_ubah == "3" || pilihan_ubah == "4" || pilihan_ubah == "5"){
                        string ulang;
                        ulang = "n";
                        while(true){
                            system("cls");
                            cout << "Apakah anda ingin melakukan perubahan data lagi? (y/n) : ";
                            cin >> ulang;
                            if(ulang == "n" || ulang == "N"){
                                break;
                            }
                            else if(ulang == "y" || ulang == "Y"){
                                break;
                            }
                            else{
                                system("cls");
                                setConsoleColor(RED);
                                cout << "==============================================================" << endl;
                                cout << "===================    Pilihan Tidak ada    ==================" << endl;
                                cout << "==============================================================" << endl;
                                setConsoleColor(LIGHTGRAY);
                                Sleep(1000);
                            }
                        }
                        if (ulang == "n" || ulang == "N"){
                            break;
                        }
                    }
                }

                while(true){
                    system("cls");
                    setConsoleColor(LIGHTBLUE);
                    cout << "==============================================================================" << endl;
                    setConsoleColor(LIGHTGRAY);
                    cout << "Melakukan Perubahan Data pada Pesawat " << pesawat[i].maskapai << " (" << pesawat[i].kode << ")" << endl;
                    cout << "------------------------------------------------------------------------------" << endl;
                    cout << "Nama Maskapai         : " << pesawat[i].maskapai << endl;
                    cout << "Kode Pesawat          : " << pesawat[i].kode << endl;
                    cout << "Tujuan                : " << pesawat[i].tujuan << endl;
                    cout << "Tanggal Keberangkatan : " << pesawat[i].tanggal << endl;
                    cout << "Jam Keberangkatan     : " << pesawat[i].jam << endl;
                    cout << "Harga Tiket Ekonomi   : " << "Rp." << pesawat[i].harga.ekonomi << endl;
                    cout << "Harga Tiket Bisnis    : " << "Rp." << pesawat[i].harga.bisnis << endl;
                    cout << "Harga Tiket First     : " << "Rp." << pesawat[i].harga.first << endl;
                    cout << "Jumlah Kursi          : " << pesawat[i].jumlah_kursi << endl;
                    setConsoleColor(LIGHTBLUE);
                    cout << "==============================================================================" << endl;
                    setConsoleColor(LIGHTGRAY);
                    cout << "\nApakah Benar Anda Ingin Mengubah Data Jadwal? (y/n): ";
                    string confirm;
                    cin >> confirm;

                    if (confirm == "y" || confirm == "Y"){
                        break;

                    } else if (confirm == "n" || confirm == "N"){
                        system("cls");
                        import_pesawatDatabase();
                        setConsoleColor(RED);
                        cout << "==============================================================" << endl;
                        cout << "===========    Perubahan Data Jadwal Dibatalkan    ===========" << endl;
                        cout << "==============================================================" << endl;
                        setConsoleColor(LIGHTGRAY);
                        Sleep(1000);
                        menu_admin();

                    } else {
                        system("cls");
                        setConsoleColor(RED);
                        cout << "==============================================================" << endl;
                        cout << "===================    Pilihan Tidak ada    ==================" << endl;
                        cout << "==============================================================" << endl;
                        setConsoleColor(LIGHTGRAY);
                        Sleep(1000);
                    }
                }

                savedata_pesawat();
                system("cls");
                setConsoleColor(LIGHTGREEN);
                cout << "==============================================================" << endl;
                cout << "===========     Perubahan Data Jadwal Berhasil     ===========" << endl;
                cout << "==============================================================" << endl;
                setConsoleColor(LIGHTGRAY);
                cout << "\nTekan Enter untuk kembali ke menu admin";
                cin.ignore();
                cin.get();
                menu_admin();
                return;
            }
        }
        if (!found) {
            system("cls");
            setConsoleColor(RED);
            cout << "==============================================================" << endl;
            cout << "=============    Kode Pesawat Tidak Ditemukan    ============" << endl;
            cout << "==============================================================" << endl;
            setConsoleColor(LIGHTGRAY);
            cout << "\nTekan Enter untuk kembali";
            cin.ignore();
            cin.get();
            ubah_jadwal();
        }
    }
}

// Fungsi Untuk Menghapus Data Jadwal Penerbangan
void hapus_jadwal(){
    system("cls");
    if (banyak_pesawat == 0) {
        setConsoleColor(RED);
        cout << "==============================================================" << endl;
        cout << "==================    Jadwal Kosong    =======================" << endl;
        cout << "==============================================================" << endl;
        setConsoleColor(LIGHTGRAY);
        Sleep(1000);
        menu_admin();
    }
    else {
        menu_konfirmasi("admin");
        tampil_jadwal();
        cout << "\nMasukkan Kode Pesawat yang ingin dihapus : ";
        string kode;
        cin >> kode;
        bool found = false;  // Variabel untuk melacak apakah kode pesawat ditemukan
        for(int i = 0; i < banyak_pesawat; i++){
            if(kode == pesawat[i].kode){
                found = true;  // Kode pesawat ditemukan
                while (true) {
                    system("cls");
                    setConsoleColor(LIGHTBLUE);
                    cout << "==============================================================" << endl;
                    setConsoleColor(LIGHTGRAY);
                    cout << "Nama Maskapai         : " << pesawat[i].maskapai << endl;
                    cout << "Kode Pesawat          : " << pesawat[i].kode << endl;
                    cout << "Tujuan                : " << pesawat[i].tujuan << endl;
                    cout << "Tanggal Keberangkatan : " << pesawat[i].tanggal << endl;
                    cout << "Jam Keberangkatan     : " << pesawat[i].jam << endl;
                    cout << "Harga Tiket Ekonomi   : " << "Rp." << pesawat[i].harga.ekonomi << endl;
                    cout << "Harga Tiket Bisnis    : " << "Rp." << pesawat[i].harga.bisnis << endl;
                    cout << "Harga Tiket First     : " << "Rp." << pesawat[i].harga.first << endl;
                    cout << "Jumlah Kursi          : " << pesawat[i].jumlah_kursi << endl;
                    setConsoleColor(LIGHTBLUE);
                    cout << "==============================================================" << endl;
                    setConsoleColor(LIGHTGRAY);
                    cout << "\nApakah Anda Yakin Ingin Menghapus Data Penerbangan Ini? (y/n) : ";
                    string confirm;
                    cin >> confirm;
                    if (confirm == "y" || confirm == "Y") {
                        for(int j = i; j < banyak_pesawat - 1; j++){
                            pesawat[j] = pesawat[j + 1];
                        }
                        banyak_pesawat--;
                        savedata_pesawat();
                        system("cls");
                        setConsoleColor(LIGHTGREEN);
                        cout << "===============================================================" << endl;
                        cout << "=============    Data Pesawat berhasil dihapus    =============" << endl;
                        cout << "===============================================================" << endl;
                        setConsoleColor(LIGHTGRAY);
                        cout << "\n                   Tekan Enter untuk kembali";
                        cin.ignore();
                        cin.get();
                        menu_admin();
                        return;  // Keluar dari fungsi setelah penghapusan dilakukan
                    }
                    else if (confirm == "n" || confirm == "N") {
                        system("cls");
                        setConsoleColor(RED);
                        cout << "==============================================================" << endl;
                        cout << "=============    Penghapusan Data Dibatalkan    ==============" << endl;
                        cout << "==============================================================" << endl;
                        setConsoleColor(LIGHTGRAY);
                        Sleep(1000);
                        menu_admin();
                        return;
                    }
                    else {
                        system("cls");
                        setConsoleColor(RED);
                        cout << "==============================================================" << endl;
                        cout << "===================    Pilihan Tidak Ada    ==================" << endl;
                        cout << "==============================================================" << endl;
                        setConsoleColor(LIGHTGRAY);
                        Sleep(1000);
                    }
                }
            }
        }
        // Jika kode pesawat tidak ditemukan
        if (!found) {
            system("cls");
            setConsoleColor(RED);
            cout << "===============================================================" << endl;
            cout << "=============    Kode Pesawat Tidak Ditemukan    ==============" << endl;
            cout << "===============================================================" << endl;
            setConsoleColor(LIGHTGRAY);
            cout << "\n                   Tekan Enter untuk kembali";
            cin.ignore();
            cin.get();
            hapus_jadwal();
        }
    }
}

// Fungsi Untuk Menampilkan Menu Admin
void menu_admin(){
    import_pesawatDatabase();
    import_pesananDatabase();
    system("cls");
    setConsoleColor(LIGHTBLUE);
    cout << "==============================================================" << endl;
    cout << "======================    Menu Admin    ======================" << endl;
    cout << "==============================================================" << endl;
    setConsoleColor(LIGHTGRAY);
    setConsoleColor(LIGHTCYAN);
    cout << "1. Tampilkan Data Pesawat" << endl;
    cout << "2. Tambah Data Pesawat" << endl;
    cout << "3. Hapus Data Pesawat" << endl;
    cout << "4. Ubah Data Pesawat" << endl;
    cout << "5. Logout" << endl;
    setConsoleColor(LIGHTGRAY);
    setConsoleColor(LIGHTBLUE);
    cout << "==============================================================" << endl;
    setConsoleColor(LIGHTGRAY);
    cout << "Masukkan Pilihan : ";
    string pilihan_admin;
    cin >> pilihan_admin;
    if(pilihan_admin == "1") {
        menu_tampil_jadwal();
        menu_admin();
    }
    else if (pilihan_admin == "2") {
        tambah_jadwal();
    }
    else if (pilihan_admin == "3") {
        hapus_jadwal();
    }
    else if (pilihan_admin == "4") {
        ubah_jadwal();
    }
    else if (pilihan_admin == "5") {
        main_menu();
    }
    else {
        system("cls");
        setConsoleColor(RED);
        cout << "===============================================================" << endl;
        cout << "================    Pilihan Tidak Ditemukan    ================" << endl;
        cout << "===============================================================" << endl;
        setConsoleColor(LIGHTGRAY);
        cout << "\n                   Tekan Enter untuk kembali";
        cin.ignore();
        cin.get();
        menu_admin();
    }
}

// Fungsi Untuk Memesan Tiket Penerbangan
void pesan_tiket(){
    system("cls");
    if (banyak_pesawat == 0){
        setConsoleColor(RED);
        cout << "==============================================================" << endl;
        cout << "==================    Jadwal Kosong    =======================" << endl;
        cout << "==============================================================" << endl;
        setConsoleColor(LIGHTGRAY);
        Sleep(1000);
        menu_user();
    }
    else {
        menu_konfirmasi("user");
        tampil_jadwal();
        cout << "Pesan Tiket" << endl;
        cout << "Masukkan Kode Pesawat : ";
        string kode, kelas;
        cin >> kode;
        bool found = false;
        for(int i = 0; i < banyak_pesawat; i++){
            if(kode == pesawat[i].kode){
                found = true;
                while(true){
                    if (pesawat[i].jumlah_kursi == 0){
                        system("cls");
                        setConsoleColor(RED);
                        cout << "==============================================================" << endl;
                        cout << "===================    Kursi Sudah Penuh    ==================" << endl;
                        cout << "==============================================================" << endl;
                        setConsoleColor(LIGHTGRAY);
                        cout << "\n                   Tekan Enter untuk kembali";
                        cin.ignore();
                        cin.get();
                        menu_user();
                    }
                    system("cls");
                    setConsoleColor(LIGHTBLUE);
                    cout << "==============================================================" << endl;
                    cout << "==================    Pilih Kelas Tiket    ===================" << endl;
                    cout << "==============================================================" << endl;
                    setConsoleColor(LIGHTGRAY);
                    setConsoleColor(LIGHTCYAN);
                    cout << "1. Ekonomi" << endl;
                    cout << "2. Bisnis" << endl;
                    cout << "3. First" << endl;
                    setConsoleColor(LIGHTGRAY);
                    setConsoleColor(LIGHTBLUE);
                    cout << "==============================================================" << endl;
                    setConsoleColor(LIGHTGRAY);
                    cout << "Masukkan Pilihan : ";
                    string pilihan_kelas;
                    cin >> pilihan_kelas;
                    if(pilihan_kelas == "1" || pilihan_kelas == "2" || pilihan_kelas == "3"){
                        cout << "Masukkan Jumlah Tiket : ";
                        int jumlah_tiket;
                        cin >> jumlah_tiket;
                        check_angka(jumlah_tiket, "Jumlah Tiket");
                        while (jumlah_tiket > pesawat[i].jumlah_kursi || jumlah_tiket < 1){
                            system("cls");
                            setConsoleColor(RED);
                            if (jumlah_tiket < 1){
                                cout << "==============================================================" << endl;
                                cout << "===================  Jumlah Tiket Tidak Valid   ==============" << endl;
                                cout << "==============================================================" << endl;
                            } else {
                                cout << "==============================================================" << endl;
                                cout << "=========  Jumlah Tiket Melebihi Jumlah Kursi Tersedia  ======" << endl;
                                cout << "--------------------------------------------------------------" << endl;
                                cout << "                 Jumlah Kursi Tersedia : " << pesawat[i].jumlah_kursi << endl;
                                cout << "==============================================================" << endl;
                            }
                            setConsoleColor(LIGHTGRAY);
                            cout << "\nTekan Enter untuk kembali";
                            cin.ignore();
                            cin.get();
                            system("cls");
                            cout << "Masukkan Jumlah Tiket : ";
                            cin >> jumlah_tiket;
                            check_angka(jumlah_tiket, "Jumlah Tiket");    
                        }
                        if(pilihan_kelas == "1"){
                            total_harga = jumlah_tiket * pesawat[i].harga.ekonomi;
                            kelas = "Ekonomi";
                        } else if(pilihan_kelas == "2"){
                            total_harga = jumlah_tiket * pesawat[i].harga.bisnis;
                            kelas = "Bisnis";
                        } else {
                            total_harga = jumlah_tiket * pesawat[i].harga.first;
                            kelas = "First";
                        }

                        while(true){
                            system("cls");
                            setConsoleColor(LIGHTBLUE);
                            cout << "==============================================================" << endl;
                            setConsoleColor(LIGHTGRAY);
                            cout << "Nama Maskapai         : " << pesawat[i].maskapai << endl;
                            cout << "Kode Pesawat          : " << pesawat[i].kode << endl;
                            cout << "Tujuan                : " << pesawat[i].tujuan << endl;
                            cout << "Tanggal Keberangkatan : " << pesawat[i].tanggal << endl;
                            cout << "Jam Keberangkatan     : " << pesawat[i].jam << endl;
                            cout << "Kelas                 : " << kelas << endl;
                            cout << "Jumlah Tiket          : " << jumlah_tiket << endl;
                            cout << "Total Harga           : " << "Rp." << total_harga << endl;
                            setConsoleColor(LIGHTBLUE);
                            cout << "==============================================================" << endl;
                            setConsoleColor(LIGHTGRAY);
                            cout << "\nApakah Benar Anda Ingin Melakukan Pemesanan Tiket? (y/n): ";
                            string confirm;
                            cin >> confirm;
                            if (confirm == "y" || confirm == "Y"){
                                break;
                    
                            } else if (confirm == "n" || confirm == "N"){
                                system("cls");
                                setConsoleColor(RED);
                                cout << "==============================================================" << endl;
                                cout << "==============    Pemesanan Tiket Dibatalkan    ==============" << endl;
                                cout << "==============================================================" << endl;
                                setConsoleColor(LIGHTGRAY);
                                Sleep(1000);
                                menu_user();
                    
                            } else {
                                system("cls");
                                setConsoleColor(RED);
                                cout << "==============================================================" << endl;
                                cout << "===================    Pilihan Tidak ada    ==================" << endl;
                                cout << "==============================================================" << endl;
                                setConsoleColor(LIGHTGRAY);
                                Sleep(1000);
                            }
                        }

                        // Simpan informasi pemesanan
                        pemesanan[banyak_pesanan].nama_pemesan = user[index_user].nama;
                        pemesanan[banyak_pesanan].maskapai_pesan = pesawat[i].maskapai;
                        pemesanan[banyak_pesanan].kode_pesan = pesawat[i].kode;
                        pemesanan[banyak_pesanan].tujuan_pesan = pesawat[i].tujuan;
                        pemesanan[banyak_pesanan].tanggal_pesan = pesawat[i].tanggal;
                        pemesanan[banyak_pesanan].jam_pesan = pesawat[i].jam;
                        pemesanan[banyak_pesanan].kelas_pesan = kelas;
                        pemesanan[banyak_pesanan].jumlah_tiket = jumlah_tiket;
                        pemesanan[banyak_pesanan].total_harga = total_harga;
                        pemesanan[banyak_pesanan].kode_unik = kode_unik_generator();
                        pemesanan[banyak_pesanan].waktu_pemesanan = tanggal_waktu();
                        pesawat[i].jumlah_kursi -= jumlah_tiket;

                        banyak_pesanan++;
                        savedata_pesawat();
                        savedata_pemesanan();
                        system("cls");
                        setConsoleColor(LIGHTGREEN);
                        cout << "===============================================================" << endl;
                        cout << "================    Pemesanan Tiket Berhasil    ===============" << endl;
                        cout << "===============================================================" << endl;
                        setConsoleColor(LIGHTGRAY);
                        cout << "\nTekan Enter untuk mencetak tiket";
                        cin.ignore();
                        cin.get();
                        // Cetak tiket
                        system("cls");
                        cetak_tiket(pemesanan[banyak_pesanan-1]);
                        setConsoleColor(RED);
                        cout << "        SILAHKAN LAKUKAN PEMBAYARAN DI KANTOR BANDARA" << endl;
                        setConsoleColor(LIGHTGRAY);
                        cout << "\n\n\nTekan Enter untuk kembali ke menu user";
                        cin.ignore();
                        cin.get();
                        menu_user();
                        break;
                    } else {
                        system("cls");
                        setConsoleColor(RED);
                        cout << "===============================================================" << endl;
                        cout << "================    Pilihan Tidak Ditemukan    ================" << endl;
                        cout << "===============================================================" << endl;
                        setConsoleColor(LIGHTGRAY);
                        cout << "\n                   Tekan Enter untuk kembali";
                        cin.ignore();
                        cin.get();
                    }
                }
            }
        }
        if(!found) {
            system("cls");
            setConsoleColor(RED);
            cout << "===============================================================" << endl;
            cout << "==============    Kode Pesawat Tidak Ditemukan    =============" << endl;
            cout << "===============================================================" << endl;
            setConsoleColor(LIGHTGRAY);
            cout << "\n                   Tekan Enter untuk kembali";
            cin.ignore();
            cin.get();
            pesan_tiket();
        }
    }
}

// Fungsi Untuk Mencetak Tiket Penerbangan
void cetak_tiket(data_pemesanan &pesanan) {
    setConsoleColor(LIGHTMAGENTA);
    cout << "====================    Tiket Pesawat    =====================" << endl;
    cout << "     BANDARA INTERNASIONAL SULTAN AJI MUHAMMAD BALIKPAPAN " << endl;
    cout << "--------------------------------------------------------------" << endl;
    cout << "                    Kode Tiket : " << pesanan.kode_unik << endl; 
    cout << "       Tanggal/Waktu Pemesanan : " << pesanan.waktu_pemesanan << endl;
    cout << "==============================================================" << endl;
    setConsoleColor(LIGHTGRAY);
    setConsoleColor(LIGHTCYAN);
    cout << "Nama Pemesan          : " << pesanan.nama_pemesan << endl;
    cout << "Maskapai              : " << pesanan.maskapai_pesan << endl;
    cout << "Kode Pesawat          : " << pesanan.kode_pesan << endl;
    cout << "Tujuan                : " << pesanan.tujuan_pesan << endl;
    cout << "Tanggal Keberangkatan : " << pesanan.tanggal_pesan << endl;
    cout << "Jam Keberangkatan     : " << pesanan.jam_pesan << endl;
    cout << "Kelas                 : " << pesanan.kelas_pesan << endl;
    cout << "Jumlah Tiket          : " << pesanan.jumlah_tiket << endl;
    cout << "Total Harga           : " << "Rp."<< pesanan.total_harga << endl;
    setConsoleColor(LIGHTGRAY);
    setConsoleColor(LIGHTMAGENTA);
    cout << "==============================================================" << endl;
    setConsoleColor(LIGHTGRAY);
    cout << endl << endl;
}

// Fungsi Untuk Menampilkan Menu User
void menu_user(){
    import_pesawatDatabase();
    import_pesananDatabase();
    system("cls");
    setConsoleColor(LIGHTBLUE);
    cout << "==============================================================" << endl;
    cout << "=======================    Menu User    ======================" << endl;
    cout << "==============================================================" << endl;
    setConsoleColor(LIGHTGRAY);
    setConsoleColor(LIGHTCYAN);
    cout << "1. Lihat Jadwal Penerbangan" << endl;
    cout << "2. Pesan Tiket Pesawat" << endl;
    cout << "3. Lihat Riwayat Pemesanan" << endl;
    cout << "4. Logout" << endl;
    setConsoleColor(LIGHTGRAY);
    setConsoleColor(LIGHTBLUE);
    cout << "==============================================================" << endl;
    setConsoleColor(LIGHTGRAY);
    cout << "Masukkan Pilihan : ";
    string pilihan_user;
    cin >> pilihan_user;
    if (pilihan_user == "1"){
        menu_tampil_jadwal();
        menu_user();
    }
    else if(pilihan_user == "2"){
        pesan_tiket();
        cin.ignore();
        cin.get();
        menu_user();
    }
    else if(pilihan_user == "3"){ //Menampilkan riwayat pemesanan
        system("cls");
        setConsoleColor(LIGHTBLUE);
        cout << "==============================================================" << endl;
        cout << "===============    Riwayat Pemesanan Anda    =================" << endl;
        cout << "==============================================================" << endl << endl;
        setConsoleColor(LIGHTGRAY);
        if (banyak_pesanan == 0) {
            setConsoleColor(RED);
            cout << "\n          Anda belum pernah melakukan pemesanan" << endl;
            setConsoleColor(LIGHTBLUE);
            cout << "==============================================================" << endl;
            setConsoleColor(LIGHTGRAY);
            cout << "\nTekan Enter untuk kembali";
            cin.ignore();
            cin.get();
            menu_user();
        }
        else{
        bool riwayat = false;
        for (int i = 0; i < banyak_pesanan; i++) {
            if (pemesanan[i].nama_pemesan == user[index_user].nama) {
                cetak_tiket(pemesanan[i]);
                cout << endl;
                riwayat = true;
            }
        }
        if (riwayat == false) {
            setConsoleColor(RED);
            cout << "\n          Anda belum pernah melakukan pemesanan" << endl;
            }
        setConsoleColor(LIGHTGRAY);
        setConsoleColor(LIGHTBLUE);
        cout << "==============================================================" << endl;
        setConsoleColor(LIGHTGRAY);
        cout << "\nTekan Enter untuk kembali";
        cin.ignore();
        cin.get();
        menu_user();
        }
    }
    else if(pilihan_user == "4"){
        main_menu();
    }
    else {
        system("cls");
        setConsoleColor(RED);
        cout << "===============================================================" << endl;
        cout << "================    Pilihan Tidak Ditemukan    ================" << endl;
        cout << "===============================================================" << endl;
        setConsoleColor(LIGHTGRAY);
        cout << "\n                   Tekan Enter untuk kembali";
        cin.ignore();
        cin.get();
        menu_user();
    }
}

// Header Untuk Menu Utama
void header() {
    system("cls");
    setConsoleColor(LIGHTGREEN);
    cout << "==============================================================" << endl;
    cout << "====================   SELAMAT DATANG DI   ===================" << endl;
    cout << "=========   BANDARA INTERNASIONAL SULTAN AJI MUHAMMAD   ======" << endl;
    cout << "==============================================================" << endl;
    setConsoleColor(LIGHTGRAY); 
}

// Fungsi Untuk Menampilkan Menu Utama
void main_menu() {
    system("cls");
    header();
    setConsoleColor(LIGHTCYAN);
    cout << "          [1] Registrasi" << endl;
    cout << "          [2] Login" << endl;
    cout << "          [3] Exit" << endl;
    setConsoleColor(LIGHTGREEN);
    cout << "==============================================================" << endl;
    setConsoleColor(LIGHTGRAY); 
    cout << "Masukkan Pilihan : ";
    string pilihan_utama;
    cin >> pilihan_utama;
    if(pilihan_utama == "1"){
        registrasi();
    }
    else if(pilihan_utama == "2"){
        login_rekursif();
        if(admin == true){
            menu_admin();
        }
        else {
            menu_user();
        }
    }
    else if(pilihan_utama == "3"){
        system("cls");
        setConsoleColor(LIGHTGREEN);
        cout << "===============================================================" << endl;
        cout << "       Terima Kasih Karena Telah Menggunakan Program Ini       " << endl;
        cout << "===============================================================" << endl;
        setConsoleColor(LIGHTGRAY);
        exit(0);
    }
    else {
        system("cls");
        setConsoleColor(RED);
        cout << "===============================================================" << endl;
        cout << "================    Pilihan Tidak Ditemukan    ================" << endl;
        cout << "===============================================================" << endl;
        setConsoleColor(LIGHTGRAY);
        cout << "\n                   Tekan Enter untuk kembali";
        cin.ignore();
        cin.get();
        main_menu();
    }
}

int main(){
    import_pesawatDatabase();
    import_pesananDatabase();
    import_userDatabase();
    
    while(true){ 
        main_menu();
    }
    
    return 0;
}