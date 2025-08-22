#ifndef file_find
#define file_find
#include <iostream>
#include <string>
#include <filesystem>
using namespace std;
namespace fs= std::filesystem;
string uzantı(const string& dosya) {
    fs::path yol(dosya);
    return yol.extension().string();
}

#endif // file_find