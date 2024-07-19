#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <fstream>
#define _USE_MATH_DEFINES

using namespace std;


class HunterSystem
{
public:
	void RightPartsCalc(double a, double t, double e, long double* x_and_y, long double* fx_and_fy);
	double NoiseGen(double q, double h);
	void SystemCalc();
	~HunterSystem();
	void InputData();
private:
	double h, t{}, T, q, a;
	int count{};
	long double* x_and_y = new long double[2];
	long double* temp1 = new long double[2];
	long double* temp2 = new long double[2];
	long double* temp3 = new long double[2];
	long double* temp4 = new long double[2];
	long double* tmp = new long double[2];
};

void HunterSystem::InputData()
{
	setlocale(LC_ALL, "Russian");
	t = 0;
	cout << "Input a: ";	//коэффициент вымирания хищников
	cin >> a;
	cout << "Input h: ";	//шаг
	cin >> h;
	cout << "Input T: ";	//промежуток времени
	cin >> T;
	cout << "Input q: ";	//интенсивность шума
	cin >> q;
	cout << "Input x: ";	//хищник
	cin >> x_and_y[0];
	cout << "Input y: ";	//жертва
	cin >> x_and_y[1];
	cout << "Данные успешно обновлены" << endl;

	double h_buf = h;

	while (h_buf < 1)
	{
		h_buf *= 10;
		count++;
	}
}

HunterSystem::~HunterSystem()
{
	delete[]x_and_y;
	delete[]temp1;
	delete[]temp2;
	delete[]temp3;
	delete[]temp4;
	delete[]tmp;
}

void HunterSystem::RightPartsCalc(double a, double t, double e, long double* x_and_y, long double* fx_and_fy)
{
	fx_and_fy[0] = -a * (1 + e) * x_and_y[0] + x_and_y[0] * x_and_y[1];
	fx_and_fy[1] = x_and_y[1] - x_and_y[0] * x_and_y[1];
	return;
}

double HunterSystem::NoiseGen(double q, double h)
{
	srand(clock());
	double y1 = (double)rand() / RAND_MAX;	//если генерируются равномерно распределенные
	double y2 = (double)rand() / RAND_MAX;
	double e = q / (sqrt(h / 2)) * sqrt((-2) * log(y1)) * cos(2 * y2 * 3.1415);	//y2 точно внутри косинуса?; зачем с шагом h/2?
	return e;
}
void HunterSystem::SystemCalc()
{
	ofstream file;
	file.open("data.txt");
	double e1 = NoiseGen(q, h);	//шум в момент времени 0
	file << 't' << ' ' << 'x' << ' ' << 'y' << ' ' << ' ' << 'e1' << endl;
	file << t << ' ' << x_and_y[0] << ' ' << x_and_y[1] << ' ' << 'e1' << endl; //нулевая строка в файл
	while (t < T)
	{
		RightPartsCalc(a, t, e1, x_and_y, temp1); //расчет правых частей системы
		for (int i = 0; i < 2; i++)
			tmp[i] = x_and_y[i] + (h * temp1[i]) / 2;
		double e2 = NoiseGen(q, h); //генерация шума
		RightPartsCalc(a, t, e2, tmp, temp2);
		for (int i = 0; i < 2; i++)
			tmp[i] = x_and_y[i] + (h * temp2[i]) / 2;
		RightPartsCalc(a, t, e2, tmp, temp3);
		for (int i = 0; i < 2; i++)
			tmp[i] = x_and_y[i] + h * temp3[i];
		double e1 = NoiseGen(q, h);
		RightPartsCalc(a, t, e1, tmp, temp4);
		for (int i = 0; i < 2; i++)
			x_and_y[i] += h * (temp1[i] + 2 * temp2[i] + 2 * temp3[i] + temp4[i]) / 6;
		t = round((t += h) * pow(10, count))/ pow(10, count);
		cout << t << " " << x_and_y[0] << " " << x_and_y[1] << ' ' << e1 << endl;
		file << t << ' ' << x_and_y[0] << ' ' << x_and_y[1] << ' ' << e1 << endl; //запись в файл
	}
	file.close();
}