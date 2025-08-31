#include <filesystem>
#include <iostream>
#include <string>
#include <vector>
#include <fmt/core.h>
#include "tinyfiledialogs.h"
#include "filefound.h"
#include "txt.h"
using namespace std;
namespace fs = std::filesystem;
 
int main()
{
	const char * ds = tinyfd_openFileDialog(
		"Dosya Seç", /* title */
		"", /* default path */
		0, /* number of filter patterns */
		NULL, /* filter patterns */
		NULL, /* single filter description */
		0); /* allow multiple selects */
	fs::path dosya(ds);
	string uzantı= dosya.extension().string();
	if (uzantı == ".txt")
	{
		string islem;
		string f_name;
		cout<<"Hangi dosya yapmak istiyosun.(Noktasız yaz):";
		cin>>islem;
		cout<<"\nOluşturulan dosyanın ismi tam ne olsun (Uzantıyı yazma) : "<<endl;
		cin>>f_name;
		string tam_yol=f_name+"."+islem;
		if (islem=="csv")
		{
			int ayrım;
			cout<<"\nDosyanızı kaç bosluk sonrası kesiyim."<<endl;
			cin>>ayrım;
			txt_t_csv(dosya.string(),tam_yol,ayrım);
		}
		else if (islem=="json")
		{
			txt_t_json(dosya.string(),f_name);
		}
		else if (islem =="xml")
		{
			cout<<"Şimdi oluşturulacak xml öncesi bilgilendirme!!\nTxt formatınız şu şekilde olmalıdır:\npersons(\n\tperson(\n\t\tname bilal))\nFormat buşekilde olmaz ise taglarınızı tam almaz."<<endl;
			txt_t_xml(dosya,f_name);
		}
		else
		{
			cout<<"Böyle bir dosya türü yok."<<endl;
		}
	}
	
	return 0;
}