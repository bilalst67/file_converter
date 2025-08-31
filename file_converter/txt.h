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

string indent(int level){
    return string(level * 4,' ');
}

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
        if(!first) yaz<<",\n";

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

void txt_t_xml(const string& ds,const string& f_name) {
    ofstream yaz(f_name + ".xml");
    ifstream oku(ds);

    if (!oku.is_open() || !yaz.is_open()) {
        cout << "Dosyalarınız açılmıyor." << endl;
        return;
    }

    vector<string> stack; // aktif parent stack
    string satir;
    while (getline(oku, satir)) {
        if (satir.empty()) continue;

        // sondaki ')' karakterlerini say ve sil
        int close_count = 0;
        while (!satir.empty() && satir.back() == ')') {
            satir.pop_back();
            close_count++;
        }

        // gerekiyorsa kapatma etiketlerini yaz
        for (int i = 0; i < close_count; i++) {
            if (!stack.empty()) {
                string parent = stack.back();
                stack.pop_back();
                yaz << indent(stack.size()) << fmt::format("</{}>\n", parent);
            }
        }

        if (satir.empty()) continue;

        istringstream iss(satir);
        string first;
        iss >> first;

        // Eğer tag açılışı ise (örnek: persons()
        if (!first.empty() && first.back() == '(') {
            first.pop_back(); // '(' kaldır
            stack.push_back(first);
            yaz << indent(stack.size() - 1) << fmt::format("<{}>\n", first);
        }
        // Eğer key-value satırı ise
        else if (!first.empty()) {
            string key = first, value;
            getline(iss, value);
            if (!value.empty() && value[0] == ' ')
                value = value.substr(1);
            yaz << indent(stack.size()) << fmt::format("<{}>{}</{}>\n", key, value, key);
        }
    }
}

void txt_t_yml(const string& dsy,const string& f_name){
}
#endif // txt