#include "HunterSystem.h"

void main()
{
	setlocale(LC_ALL, "Russian");
	HunterSystem HS;
	cout << "Список команд: " << '\n' << "Нажмите 1 для введения/изменения расчетных данных;" << '\n'
		<< "Нажмите 2 для расчета математической модели (с записью в файл результатов);" << '\n'
		<< "Нажмите 3 (не трогать)" << '\n'
		<< "Нажмите 4 для выхода из программы;" << '\n';
	while (true)
	{
		cout << "-> ";
		int x{};
		cin >> x;
		switch (x)
		{
		case 1:
			HS.InputData();
			break;
		case 2:
			HS.SystemCalc();
			break;
		case 3:
			break;
		case 4:
			exit(1);
		default:
			break;
		}
	}
}
