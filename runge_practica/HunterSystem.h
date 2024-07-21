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
	long double* RightPartsCalc(double a, double t, double ksi, long double* x_and_y, long double* fx_and_fy);
	double NoiseGen(double sigma, double h);
	void SystemCalc();
	~HunterSystem();
	void InputData();
private:
	double h, t{}, T, sigma, a;
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
	cout << "Input sigma: ";//интенсивность шума
	cin >> sigma;
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

long double* HunterSystem::RightPartsCalc(double a, double t, double ksi, long double* x_and_y, long double* fx_and_fy)
{
	fx_and_fy[0] = -a * (1.0 + ksi) * x_and_y[0] + x_and_y[0] * x_and_y[1];
	fx_and_fy[1] = x_and_y[1] - x_and_y[0] * x_and_y[1];
	return fx_and_fy;
}

double HunterSystem::NoiseGen(double sigma, double h)
{
	srand(clock());
	double y1 = (double)rand() / RAND_MAX;	// генерания [0:1]
	double y2 = (double)rand() / RAND_MAX;
	double ksi = sigma / (sqrt(h / 2.0)) * sqrt((-2.0) * log(y1)) * cos(2.0 * y2 * 3.1415);
	return ksi;
}
void HunterSystem::SystemCalc()
{
	ofstream file;
	file.open("data.txt");
	double ksi1 = NoiseGen(sigma, h);	//шум в момент времени 0
	file << 't' << ' ' << 'x' << ' ' << 'y' << ' ' << ' ' << "ksi1" << endl;
	file << t << ' ' << x_and_y[0] << ' ' << x_and_y[1] << ' ' << ksi1 << endl; //нулевая строка в файл
	while (t < T)
	{
		RightPartsCalc(a, t, ksi1, x_and_y, temp1); //расчет правых частей системы
		for (int i = 0; i < 2; i++)
			tmp[i] = x_and_y[i] + (h * temp1[i]) / 2;
		double ksi2 = NoiseGen(sigma, h); //генерация шума
		RightPartsCalc(a, t, ksi2, tmp, temp2);
		for (int i = 0; i < 2; i++)
			tmp[i] = x_and_y[i] + (h * temp2[i]) / 2;
		RightPartsCalc(a, t, ksi2, tmp, temp3);
		for (int i = 0; i < 2; i++)
			tmp[i] = x_and_y[i] + h * temp3[i];
		double ksi1 = NoiseGen(sigma, h);
		RightPartsCalc(a, t, ksi1, tmp, temp4);
		for (int i = 0; i < 2; i++)
			x_and_y[i] += h * (temp1[i] + 2 * temp2[i] + 2 * temp3[i] + temp4[i]) / 6;
		t = round((t += h) * pow(10, count))/ pow(10, count);
		cout << t << " " << x_and_y[0] << " " << x_and_y[1] << ' ' << ksi1 << endl;
		file << t << ' ' << x_and_y[0] << ' ' << x_and_y[1] << ' ' << ksi1 << endl; //запись в файл
	}
	file.close();
}
