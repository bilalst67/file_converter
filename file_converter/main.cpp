#include <filesystem>
#include <iostream>
#include <string>
#include "filefound.h"
#include "txt.h"
namespace fs = std::filesystem;
 
int main()
{
	fs::path dosya("lale.txt");
	string yol= uzantı("lale.txt");
	if (yol == ".txt")
	{
		string islem;
		string f_name;
		cout<<"Hangi dosya yapmak istiyosun."<<endl;
		cin>>islem;
		cout<<"\nOluşturulan dosyanın ismi tam ne olsun."<<endl;
		cin>>f_name;
		string tam_yol=f_name+"."+islem;
		if (islem=="csv")
		{
			int ayrım;
			cout<<"\nDosyanızı kaç bosluk sonrası kesiyim."<<endl;
			cin>>ayrım;
			txt_t_csv(dosya,tam_yol,ayrım);
		}
		else if (islem=="json")
		{
			txt_t_json(dosya);
		}
		
		
	}
	
	return 0;
}