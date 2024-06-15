#include "pch.h"
#include <iostream>
#include <vector>
#include "Random.h"

using namespace std;

void menu(string userName, string url)
{
	vector<char> rd{};
	string pwd{};
	cout << "Please select: \n1. Generate new password for a website/app\n2. Login to a website/app\n3. Exit\n";
	int choice{};
	cin >> choice;
	static int nc{}, cap{}, num{}, spec{};
	char yn1{}, yn2{}, yn3{};
	switch (choice)
	{
	case 1:
		cout << "Please specify password requirements.\n1. No. of characters: ";
		cin >> nc;
		cout << "At least one capital letter required (Y or N): ";
		cin >> yn1;
		cout << "At least one number required (Y or N): ";
		cin >> yn2;
		cout << "At least one special character (Y or N): ";
		cin >> yn3;
		if (yn1 == 'Y' || yn1 == 'y')
		{
			nc--;
			cap = Random::get(65, 90);
			rd.push_back(static_cast<char>(cap));
		}
		if (yn2 == 'Y' || yn2 == 'y')
		{
			nc--;
			num = Random::get(48, 57);
			rd.push_back(static_cast<char>(num));
		}
		if (yn3 == 'Y' || yn3 == 'y')
		{
			nc--;
			spec = Random::get(32, 47);
			rd.push_back(static_cast<char>(spec));
		}
		for (int i = 0; i < nc; ++i)
		{
			rd.push_back(static_cast<char>(Random::get(32, 126)));
		}
		for (int i = 0; i < rd.size(); ++i)
		{
			pwd = pwd + rd[i];
		}
		cout << "Password: " << pwd << '\n';
		return;
	case 2:
		return;
	case 3:
		return;
	default:
		return;

	}


}