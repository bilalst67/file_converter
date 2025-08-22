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
    std::string k_sayi;
    while (std::getline(dosya,satir))
    {
        if (satir==" ")
        {
            sayim++;
        }
        if (satir==" " && sayim==(ayrım+1))//Satır atlancak yer
        {
            sayim = 0;
            continue;
        }
        out<<satir;
    }
    
    
}
void txt_t_json(std::string yol){

}

#endif // txt