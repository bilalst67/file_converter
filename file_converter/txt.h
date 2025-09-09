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

int find_vc(vector<string>d_name,string key){
    for (int i = 0; i < d_name.size(); i++)
    {
        if (d_name[i]==key)
        {
            return i;
        }
    }
    return-1;
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
    return t == "true" || t == "false";
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