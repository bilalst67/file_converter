#include <filesystem>
#include <iostream>
#include <string>
#include <vector>
#include <fmt/core.h>
/*#include "tinyfiledialogs.h"
#include "filefound.h"*/
#include "txt.h"
using namespace std;
namespace fs = std::filesystem;
 
int main(){
	const char * ds = "/home/bilal/Belgeler/GitHub/txt.txt";
	fs::path dosya(ds);
	string uzantı= dosya.extension().string();
	if (uzantı == ".txt")
	{
		string islem;
		string f_name;
		cout<<"Hangi dosya yapmak istiyosun.(Noktasız yaz) : ";
		cin>>islem;
		cout<<"\nOluşturulan dosyanın ismi tam ne olsun (Uzantıyı yazma) : ";
		cin>>f_name;
		if (islem=="csv")
		{
			int ayrim;
			cout<<"\nDosyanızı kaç bosluk sonrası kesiyim."<<endl;
			cin>>ayrim;
			txt_t_csv(dosya.string(),f_name,ayrim);
		}
		else if (islem=="json")
		{
			txt_t_json(dosya.string(),f_name);
		}
		else if (islem =="xml")
		{
			cout<<"Şimdi oluşturulacak xml öncesi bilgilendirme!!\nTxt formatınız şu şekilde olmalıdır:\npersons.person.name.yusuf\npersons.person.skill.python\npersons.person.language.Türkçe\ncompanys.company.name.paperworks\ncompanys.company.founder.name.Yusuf\ncompanys.company.founder.skill.python-renpy"<<endl;
			txt_t_xml(dosya,f_name);
		}
		else if(islem=="yml")
		{
			cout<<"Şimdi oluşturulacak yml öncesi bilgilendirme!!\nTxt formatınız şu şekilde olmalıdır:\npersons.person.name.yusuf\npersons.person.skill.python\npersons.person.language.Türkçe\ncompanys.company.name.paperworks\ncompanys.company.founder.name.Yusuf\ncompanys.company.founder.skill.python-renpy"<<endl;
			txt_t_yml(dosya.string(),f_name);
		}
		else
		{
			cout<<"Böyle bir dosya türü şuanlık işleyemiyoruz."<<endl;
		}
	}
	
	return 0;
}