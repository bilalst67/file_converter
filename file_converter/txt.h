#ifndef txt
#define txt
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <filesystem>
#include <algorithm>
#include <fmt/core.h>
using namespace std;
namespace fs=std::filesystem;

void txt_t_csv(const string& ds,const string& f_name,int ayrım){
    fs::path csv(f_name);
    std::ifstream dosya(ds);
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

void txt_t_json(const string& dsy,const string& f_name){
    fs::path ds(f_name+".json");
    std::ifstream oku(dsy);
    std::ofstream yaz(ds,std::ios::trunc);
    if (!oku.is_open() || !yaz.is_open()) {
        cout << "Dosyalarınız açılmıyor." << endl;
        return;
    }
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

        
        yaz<<fmt::format("  \"{}\" : ",key);
        
        if (isnumb){
            yaz<<value<<endl;
        }
        else if(isbool){
            std::transform(value.begin(), value.end(), value.begin(), ::tolower);
            yaz<<value<<endl;
        }
        else{
            yaz<<fmt::format("\"{}\"",value)<<endl;
        }
    }
yaz<<"}";
}

void txt_t_xml(const string& ds,const string& f_name,const string& top_tag){
    
    cout<<"Bu oluşturulcak xml dosyası nested olmicak.\n"<<endl;
    
    ofstream yaz(f_name+".xml");
    ifstream oku(ds);
    if (!oku.is_open() || !yaz.is_open()) {
        cout << "Dosyalarınız açılmıyor." << endl;
        return;
    }
    string satir;
    yaz<<fmt::format("<{}>",top_tag);
    while (getline(oku,satir))
    {
        if (satir.empty())continue;
        istringstream kk(satir);
        string key,value;
        kk>>key;
        getline(kk, value);
        if (!value.empty() && value[0] == ' ') value = value.substr(1);
        yaz<<fmt::format("<{}>{}</{}>",key,value,key);
    }
    yaz<<fmt::format("</{}>",top_tag);
    yaz.close();
    oku.close();
    cout<<"Dosyanız xml e çevrilmiştir yanlış çevirilmiş olabilir.";
}
#endif // txt