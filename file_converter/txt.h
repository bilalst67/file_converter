#ifndef txt
#define txt
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <filesystem>
#include <algorithm>
namespace fs=std::filesystem;

void txt_t_csv(const string& yol,const string& f_name,int ayrım){
    fs::path csv(f_name);
    std::ifstream dosya(yol);
    if (!dosya.is_open())
    {
        std::cout<<"Txt dosyası açılmadı."<<std::endl;
    }
    std::ofstream out(csv,std::ios::app);
    if (!out.is_open())
    {
        std::cout<<"Txt dosyası açılmadı."<<std::endl;
        return;
    }
    int sayim=0;
    std::string satir;
    while (std::getline(dosya,satir))
    {
        if (satir=="")
        {
            continue;
        }
        
        if (satir==" ")
        {
            sayim++;
        }
        if (sayim==(ayrım+1) && satir==" ")//Satır atlancak yer
        {
            sayim = 0;
            continue;
        }
        out<<satir<<",\n";
    }   
}

void txt_t_json(const string& yol,const string& f_name){
    fs::path ds(f_name+".json");
    std::ifstream oku(yol);
    std::ofstream yaz(ds,std::ios::trunc);
    if (!oku.is_open())
    {
        if (!yaz.is_open())
        {
            std::cout<<"Dosyalarınız açılmıyor."<<endl;
            return;
        }
    }
    int sayac;
    bool first=true;
    std::string satir;
    yaz<<"{\n";
    while (getline(oku,satir))
    {
        if (satir.empty())continue;
        std::string key,value;
        istringstream iss(satir);
        iss>>key>>value;
        if(!first) yaz<<',\n';

        first=false;

        bool isnumb = !value.empty() && all_of(value.begin(),value.end(), ::isdigit);
        bool isbool = (value=="true" || value=="True" || value=="false" || value=="False");

        
        yaz<<" \""<<key<<"\": ";
        
        if (isnumb){
            yaz<<value;
        }
        else if(isbool){
            std::transform(value.begin(), value.end(), value.begin(), ::tolower);
            yaz<<value;
        }
        else{
            yaz<<" \""<<value<<"\"";
        }
    }
yaz<<"}";
}
#endif // txt