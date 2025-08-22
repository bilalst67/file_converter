#ifndef txt
#define txt
#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>
namespace fs=std::filesystem;
void txt_t_csv(std::string yol,std::string f_name,int ayrım){
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
void txt_t_json(std::string yol,std::string f_name){
    fs::path ds(f_name);
    std::ifstream oku(yol);
    std::ofstream yaz(ds,std::ios::app);
    if (!oku.is_open())
    {
        if (!yaz.is_open())
        {
            std::cout<<"Dosyalarınız açılmıyor."<<endl;
            return;
        }
    }
    std::string satir;
    while (std::getline(oku,satir))
    {
        if (satir==",")
        {
            satir=",\n";
        }
        yaz<<satir;
    }
    
    
}

#endif // txt