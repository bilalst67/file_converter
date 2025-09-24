#ifndef txt
#define txt
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <filesystem>
#include <algorithm>
#include <cctype>
#include <fmt/core.h>
using namespace std;
namespace fs=std::filesystem;

string indent(int level){
    return string(level * 4,' ');
}
static vector<string> split(const string& s, char delim) {
    vector<string> out;
    string tmp;
    stringstream ss(s);
    while (getline(ss, tmp, delim)) {
        out.push_back(tmp);
    }
    return out;
}

static string trim(const string& s) {
    size_t a = s.find_first_not_of(" \t\r\n");
    if (a == string::npos) return "";
    size_t b = s.find_last_not_of(" \t\r\n");
    return s.substr(a, b - a + 1);
}

static bool is_number(const string& s) {
    if (s.empty()) return false;
    return all_of(s.begin(), s.end(), [](unsigned char c){ return isdigit(c); });
}

static bool is_bool(const string& s) {
    string t = s;
    transform(t.begin(), t.end(), t.begin(), ::tolower);
    return t == "true" || "True" || "false" || "False";
}

void txt_t_csv(const string& ds,const string& f_name,int ayrım){
    fs::path csv(f_name+".csv");
    std::ifstream oku(ds);
    std::ofstream yaz(csv,std::ios::app);
    if (!oku.is_open() || !yaz.is_open()) {
        cout << "Dosyalarınız açılmıyor." << endl;
        return;
    }
    int sayim=0;
    std::string satir;
    while (std::getline(oku,satir))
    {
        if (satir.empty())continue;
        if (satir==" ")
        {
            sayim++;
        }
        if (sayim==(ayrım+1) && satir==" ")//Satır atlancak yer
        {
            sayim = 0;
            continue;
        }
        yaz<<satir<<",\n";
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
        
        yaz<<fmt::format("  \"{}\" : ",key);
        
        if (is_number(value)){
            yaz<<value<<endl;
        }
        else if(is_bool(value)){
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

    vector<string> prevstack; // geçmiş parent stack
    string satir;
    yaz<<"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    while (getline(oku, satir)) {
        if (satir.empty()) continue;
        
        vector<string>partsraw=split(satir,'.');
        vector<string>parts;
        for (auto &p : partsraw)
        {
            string t=trim(p);
            if(!t.empty())parts.push_back(t);
        }
        string value;
        vector<string>keys;
        if (parts.size()==1)
        {
            keys.push_back(parts[0]);
            value="";
        }
        else{
            keys.assign(parts.begin(), parts.end() - 1);
            value=parts.back();
        }

        //Hangi keyler aynı.
        int common=0;
        while (common< (int)prevstack.size() && common<(int)keys.size() && keys[common]==prevstack[common]){++common;}
        for (int i = prevstack.size() - 1; i >= common+1; --i)
            {
                yaz << fmt::format("{}</{}>\n", indent(i-1), prevstack[i-1]);
            }
        
        for (int i = common; i < (int)keys.size()-1; i++){yaz<<fmt::format("{}<{}>\n",indent(i),keys[i]);}

        if (!keys.empty()){
            int lastIndex = (int)keys.size() - 1;
            string lastKey = keys.back();

            // Eğer value içindeki '-' ile liste benzetiliyorsa split et
            // Örn: "python-renpy" veya "-python-c++-..." veya "python" tek eleman
            vector<string> items = split(value, '-');
            // temizle empty tokenleri
            vector<string> tokens;
            for (auto &it : items) {
                string tt = trim(it);
                if (!tt.empty()) tokens.push_back(tt);
            }

            if (tokens.size() > 1) {                
                for (auto &it : tokens) {
                    yaz << fmt::format("{}<{}>{}</{}>\n",indent(lastIndex),lastKey,it,lastKey);
                }
            }else {
                // tek değer veya boş
                if (value.empty()) {
                    // sadece map başlat (value yok)
                    yaz << indent(lastIndex) << lastKey << ":\n";
                } else {
                    // tek değer: tip kontrolü
                    if (is_number(value) || is_bool(value)) {
                        yaz << fmt::format("{}<{}>{}</{}>\n",indent(lastIndex),lastKey,value,lastKey);
                    } else {
                        yaz << fmt::format("{}<{}>'{}'</{}>\n",indent(lastIndex),lastKey,value,lastKey);
                    }
                }
            }
        } else {
            // keys boşsa => doğrudan top-level value? (nadir durum)
            // parts boyutu 1 idi; yaz key:
            yaz << indent(0) << value << "\n";
        }
        
        
        // prevKeys'i güncelle
        prevstack = keys;
    }
    for (int i = prevstack.size() - 1; i >= 1; --i)
        {
            yaz << fmt::format("{}</{}>\n", indent(i-1), prevstack[i-1]);
        }
}
// txt format: path.parts.value  (son parça value)
// Örnek: persons.person.name.yusuf
void txt_t_yml(const string& dsy, const string& f_name) {
    ifstream oku(dsy);
    ofstream yaz(f_name + ".yml", ios::trunc);
    if (!oku.is_open() || !yaz.is_open()) {
        cerr << "Dosyalar açılmıyor\n";
        return;
    }

    vector<string> prevKeys; // önceki satırın key path'i (value hariç)
    string line;
    yaz << "---\n";

    while (getline(oku, line)) {
        line = trim(line);
        if (line.empty()) continue;

        // parçala ve trimle
        vector<string> partsRaw = split(line, '.');
        vector<string> parts;
        for (auto &p : partsRaw) {
            string t = trim(p);
            if (!t.empty()) parts.push_back(t);
        }
        if (parts.empty()) continue;

        // Son parça value; öncekiler keys
        string value;
        vector<string> keys;
        if (parts.size() == 1) {
            // Tek parça => treat as key with empty value (edge case)
            keys.push_back(parts[0]);
            value = "";
        } else {
            value = parts.back();
            keys.assign(parts.begin(), parts.end() - 1);
        }

        // ortak prefix uzunluğunu bul
        int common = 0;
        while (common < (int)prevKeys.size() && common < (int)keys.size() && prevKeys[common] == keys[common]) {
            ++common;
        }

        // Yeni (ortak olmayan) keys'i yaz; *son* key'i (eğer varsa) ayrı işleyeceğiz
        // Yani sadece keys[common .. keys.size()-2] aralığını yaz
        for (int i = common; i < (int)keys.size() - 1; ++i) {
            yaz << indent(i) << keys[i] << ":\n";
        }

        // Şimdi son anahtar + value işlemi
        if (!keys.empty()) {
            int lastIndex = (int)keys.size() - 1;
            string lastKey = keys.back();

            // Eğer value içindeki '-' ile liste benzetiliyorsa split et
            // Örn: "python-renpy" veya "-python-c++-..." veya "python" tek eleman
            vector<string> items = split(value, '-');
            // temizle empty tokenleri
            vector<string> tokens;
            for (auto &it : items) {
                string tt = trim(it);
                if (!tt.empty()) tokens.push_back(tt);
            }

            if (tokens.size() > 1) {
                // liste olarak yaz (önce key:)
                yaz << indent(lastIndex) << lastKey << ":\n";
                for (auto &it : tokens) {
                    yaz << indent(lastIndex + 1) << "- " << it << "\n";
                }
            } else {
                // tek değer veya boş
                if (value.empty()) {
                    // sadece map başlat (value yok)
                    yaz << indent(lastIndex) << lastKey << ":\n";
                } else {
                    // tek değer: tip kontrolü
                    if (is_number(value) || is_bool(value)) {
                        yaz << indent(lastIndex) << lastKey << ": " << value << "\n";
                    } else {
                        yaz << indent(lastIndex) << lastKey << ": " << value << "\n";
                    }
                }
            }
        } else {
            // keys boşsa => doğrudan top-level value? (nadir durum)
            // parts boyutu 1 idi; yaz key:
            yaz << indent(0) << value << "\n";
        }

        // prevKeys'i güncelle
        prevKeys = keys;
    }

    yaz << "...\n";
}

#endif // txt