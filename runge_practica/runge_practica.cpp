#include <iostream>
#include <iostream>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <fstream>

using namespace std;

void RightParts(double a, double t, double e, long double* x_and_y, long double* fx_and_fy)	//считает правые части уравнений
{
	fx_and_fy[0] = -a * (1 + e) * x_and_y[0] + x_and_y[0] * x_and_y[1];
	fx_and_fy[1] = x_and_y[1] - x_and_y[0] * x_and_y[1];
	return;
}

double Noise(double q, double h)
{
	srand(time(NULL));
	double y1 = (double)rand() / RAND_MAX;	//если генерируются равномерно распределенные
	double y2 = (double)rand() / RAND_MAX;
	double e = q / (sqrt(h / 2)) * sqrt((-2) * log(y1)) * cos(2 * M_PI * y2);	//y2 точно внутри косинуса?; зачем с шагом h/2?
	return e;
}

void main()
{
	double h, t = 0, T, q, a;
	long double* x_and_y = new long double[2];
	long double* temp1 = new long double[2];
	long double* temp2 = new long double[2];
	long double* temp3 = new long double[2];
	long double* temp4 = new long double[2];
	long double* tmp = new long double[2];
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

	cout << t << " " << x_and_y[0] << " " << x_and_y[1] << '\n';
	_getch();
	ofstream file;
	file.open("data.txt");
	while (t < T)
	{
		double e1 = Noise(q, h);	//шум в момент времени 0
		RightParts(a, t, e1, x_and_y, temp1);
		for (int i = 0; i < 2; i++)
		{
			tmp[i] = x_and_y[i] + (h * temp1[i]) / 2;
		}
		double e2 = Noise(q, h);
		RightParts(a, t, e2, tmp, temp2);
		for (int i = 0; i < 2; i++)
		{
			tmp[i] = x_and_y[i] + (h * temp2[i]) / 2;	//зачем то округлил игрик, хотя входные данные были как надо
		}

		//шум не генерю потому что тот же момент времени?
		RightParts(a, t, e2, tmp, temp3);
		for (int i = 0; i < 2; i++)
		{
			tmp[i] = x_and_y[i] + h * temp3[i];
		}

		double e3 = Noise(q, h);
		RightParts(a, t, e3, tmp, temp4);

		for (int i = 0; i < 2; i++)
		{
			x_and_y[i] += h * (temp1[i] + 2 * temp2[i] + 2 * temp3[i] + temp4[i]) / 6;
		}
		t += h;
		cout << t << " " << x_and_y[0] << " " << x_and_y[1] << '\n';	//как выводить шум от времени? одно из значений/по формуле x и y/по другой формуле....
		file << t << ' ' << x_and_y[0] << ' ' << x_and_y[1] << endl;
	}
	file.close();
	system("pause");
}
