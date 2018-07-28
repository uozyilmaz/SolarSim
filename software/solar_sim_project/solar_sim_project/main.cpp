#include <iostream>
#include <fstream>
#include <iomanip>
#include <math.h>

using namespace std;
int main(int argc, char* argv[]) {
    cout << endl;cout << endl;cout << endl;
    cout << "SolarSim Ver1.0\n";
    cout << "------------------------------------------\n";
    if (argc==1){
        cout << "Parametes needed\n";
        cout << "For Arduino file:\n toarduino input.txt exit.txt <step value>\n\n";
        cout << "Convert data from arduino:\n topc input.txt exit.txt <step value>\n\n";
        cout << "To solve arduino file:\n analiz log.txt analys.txt\n\n";
        cout << "(step value indicate that part amount between to value)\n";
        return 1;
    }
    
    int durum=0;
    float boll;
    int bol;
    string acilan,yazilan;
    acilan=argv[2];
    yazilan=argv[3];
    if (std::string(argv[1]) == "toarduino") {
        durum=1;}
    if (std::string(argv[1]) == "topc") {
        durum=2;}
    if (std::string(argv[1]) == "analiz") {
        durum=3;}
    float okunan=0.00,sayac=0,sayac2=0,onceki=0.00,parca=0.00,hesaplanan=0.00;
    int sol,sag;
    int sayi;
    if (durum==3){
        ofstream dosyayaz (yazilan);
        ifstream fin;
        fin.open(acilan, ios::in);
        if (fin.fail())
        {
            cout << "File not found\n";
            return 1;
        }
       char my_character ;
       sayac2=0;
       {
        while (!fin.eof() )
        {
 fin.get(my_character);
            if (fin.eof()){cout << sayac2 << " data created and writen analysis file\n"; fin.close();dosyayaz.close();return 0;}
            sayi=my_character;
            std::string s = std::to_string(sayi);
            if (s.size()==1)
            {s = "0" + s;}
            dosyayaz << s;
            sayac++;
            if (sayac==1){dosyayaz << ".";}
            if (sayac==3){dosyayaz << ".";}
            if (sayac==2){dosyayaz << char(9);}
            if (sayac==4){sayac2++;sayac=0;dosyayaz << endl;}
        }
    }
    }
    boll=strtof(argv[4],NULL);
    bol=boll;
    ofstream dosyayaz (yazilan);
    ifstream dosyaoku (acilan);
    if (dosyaoku.is_open())
    {
        while ( dosyaoku>>okunan )
        {
            sayac++;
            if (sayac!=1){
                parca=(okunan-onceki)/bol;
                for (int i=1;i<bol;i++){
                    hesaplanan=roundf((onceki+(parca*i))*100)/100;
                    sol=hesaplanan;
                    sag=(hesaplanan-sol)*100;
                    if (durum==1){dosyayaz << char(sol) << char(sag);}
                    if (durum==2){dosyayaz <<hesaplanan<<endl;}
                    sayac2++;                   }
                onceki=okunan;
                hesaplanan=roundf((okunan)*100)/100;
                sol=hesaplanan;
                sag=(hesaplanan-sol)*100;
                if (durum==1){dosyayaz << char(sol) << char(sag);}
                if (durum==2){dosyayaz <<hesaplanan<<endl;}
                sayac2++;
                            }
            else {onceki=okunan;
                hesaplanan=roundf((okunan)*100)/100;
                sol=hesaplanan;
                sag=(hesaplanan-sol)*100;
                if (durum==1){dosyayaz << char(sol) << char(sag);}
                if (durum==2){dosyayaz <<hesaplanan<<endl;}
                sayac2++;}
        }
        dosyaoku.close();
        dosyayaz.close();
        cout << "Number of records read: " << sayac << endl;
        cout << "Step count: " << bol << endl;
        cout << "Number of records writen: " << sayac2 << endl;
    }
    else {cout << "File not found <" << acilan << ">\n";return 1;}
    if (durum==1){cout << "Arduino data file written.\n";}
    if (durum==2){cout << "Computer text file written.\n";}
    return 0;
}
