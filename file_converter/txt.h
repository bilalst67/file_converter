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

string trim(const string& str) {
    size_t first = str.find_first_not_of(" \t\n\r");
    if (string::npos == first) return str;
    size_t last = str.find_last_not_of(" \t\n\r");
    return str.substr(first, (last - first + 1));
}

vector<string> split(const string& s, char delimiter) {
    vector<string> tokens;
    string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

string indent(int level) {
    return string(level * 4, ' '); // 4 boşluklu girinti
}

bool is_number(const string& s) {
    if(s.empty()) return false;
    for (char const &c : s) {
        if (std::isdigit(c) == 0 && c != '.') return false;
    }
    return true;
}

bool is_bool(const string& s) {
    return s == "true" || s == "false";
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

void txt_t_json(const string& dsy, const string& f_name) {
    fs::path ds(f_name + ".json");
    std::ifstream oku(dsy);
    std::ofstream yaz(ds, std::ios::trunc);

    if (!oku.is_open() || !yaz.is_open()) {
        cout << "Dosyalarınız açılamıyor." << endl;
        return;
    }

    yaz << "{\n";

    vector<string> prev_path;
    bool is_first_line_overall = true;

    string satir;
    while (getline(oku, satir)) {
        if (satir.empty()) continue;

        // 1. SATIRI AYRIŞTIR: Yolu (path), son anahtarı (lastKey) ve değeri (value) ayır.
        vector<string> parts;
        for (const auto& p : split(satir, '.')) {
            string t = trim(p);
            if (!t.empty()) parts.push_back(t);
        }
        if (parts.size() < 2) continue;

        vector<string> path(parts.begin(), parts.end() - 2);
        string lastKey = parts[parts.size() - 2];
        string value = parts.back();

        // 2. ORTAK YOLU BUL
        int common_depth = 0;
        while (common_depth < prev_path.size() && common_depth < path.size() && prev_path[common_depth] == path[common_depth]) {
            common_depth++;
        }

        // 3. KAPANAN YAPILARI YAZDIR
        for (int i = prev_path.size(); i > common_depth; --i) {
            yaz << "\n" << indent(i) << "]\n" << indent(i-1) << "}";
        }

        // 4. YENİ YOLU AÇ
        for (int i = common_depth; i < path.size(); ++i) {
             if (!is_first_line_overall) {
                yaz << ",\n";
            }
            if (i == 0) {
                yaz << indent(i) << "\"" << path[i] << "\": [";
            } else {
                yaz << indent(i) << "{\"" << path[i] << "\": [";
            }
            is_first_line_overall = true; // Her yeni dizi açıldığında, içindeki ilk eleman virgül almaz.
        }

        // 5. ANAHTAR-DEĞER ÇİFTİNİ YAZDIR
        if (!is_first_line_overall) {
            yaz << "\n" << indent(path.size() + 1) << ",";
        }
        
        vector<string> tokens;
        for(const auto& val_part : split(value, '-')) {
            string t = trim(val_part);
            if (!t.empty()) tokens.push_back(t);
        }

        bool first_token = true;
        for (const auto& token : tokens) {
            if (!first_token) {
                 yaz << ",\n";
            }
            yaz << "\n" << indent(path.size()) << "{\"" << lastKey << "\": ";
            if (is_number(token) || is_bool(token)) {
                yaz << token << "}";
            } else {
                yaz << "\"" << token << "\"}";
            }
            first_token = false;
        }

        prev_path = path;
        is_first_line_overall = false;
    }

    // 6. DOSYA SONUNDA TÜM AÇIK YAPILARI KAPAT
    for (int i = prev_path.size(); i > 0; --i) {
        yaz << "\n" << indent(i) << "]\n" << indent(i-1) << "}";
    }
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