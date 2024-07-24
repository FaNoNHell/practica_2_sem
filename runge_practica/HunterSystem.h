#include <iostream> //библиотека ввода вывода в консоль
#include <stdlib.h>
#include <time.h> //библиотека для функций связанных с временем
#include <math.h> //библиотека для мат. функций
#include <fstream> //библиотека для файлового потока
#define _USE_MATH_DEFINES //дескриптор для математических констант M_PI например

//использование поля имён std
using namespace std;

//объявлление класса
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
	int count{}; //переменная для округления (технич. проблема)
	long double* x_and_y = new long double[2]; //x y
	//промежуточные значения для рунге кутты
	long double* temp1 = new long double[2]; 
	long double* temp2 = new long double[2];
	long double* temp3 = new long double[2];
	long double* temp4 = new long double[2];
	long double* tmp = new long double[2];
};

//функция отвечающая за ввод данных для расчета системы
void HunterSystem::InputData()
{
	setlocale(LC_ALL, "Russian"); //локализация консоли
	t = 0; //обнуление параметра t для повторных использований
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
	cout << "Update complete." << endl;

	//Расчет количества знаков после запятой для округления  (решение технич. проблемы с ошибочным количеством итераций цикла)
	double h_buf = h;
	while (h_buf < 1)
	{
		h_buf *= 10;
		count++;
	}
}

//дескриптор очищения памяти
HunterSystem::~HunterSystem()
{
	delete[]x_and_y;
	delete[]temp1;
	delete[]temp2;
	delete[]temp3;
	delete[]temp4;
	delete[]tmp;
}

//функция отвечающая за расчёт правых частей уравнений
long double* HunterSystem::RightPartsCalc(double a, double t, double ksi, long double* x_and_y, long double* fx_and_fy)
{
	fx_and_fy[0] = -a * (1.0 + ksi) * x_and_y[0] + x_and_y[0] * x_and_y[1];
	fx_and_fy[1] = x_and_y[1] - x_and_y[0] * x_and_y[1];
	return fx_and_fy;
}

//генерация шума
double HunterSystem::NoiseGen(double sigma, double h)
{
	srand(clock()); //для корректной работы генерации случайных чисел необходимо менять сид, в нашем случае в  качестве сида используется кол-во тактов процессора с момента запуска
	double y1 = (double)rand() / RAND_MAX;	// генерация [0:1]
	double y2 = (double)rand() / RAND_MAX;
	double ksi = sigma / (sqrt(h / 2.0)) * sqrt((-2.0) * log(y1)) * cos(2.0 * y2 * 3.1415); //расчет самого шума.
	return ksi;
}
void HunterSystem::SystemCalc()
{
	ofstream file; //создание файлового потока
	file.open("data.txt"); //открытие файлового потока
	double ksi1 = NoiseGen(sigma, h);	//шум в момент времени 0
	file << "Input data: \na: " << a << '\n' << "h: " << h << '\n' << "T: " << T << '\n' << "sigma: " << sigma << '\n' << "x: " << x_and_y[0] << '\n' << "y: " << x_and_y[1] << endl << endl; //вывод в файл исходных данных системы
	file << 't' << ' ' << 'x' << ' ' << 'y' << ' ' << ' ' << "ksi1" << endl; //вывод обозначений
	file << t << ' ' << x_and_y[0] << ' ' << x_and_y[1] << ' ' << ksi1 << endl; //нулевая строка в файл
	//цикл расчета системы, работает пока t не дойдет до параметра T вводимого ранее
	while (t < T)
	{
		RightPartsCalc(a, t, ksi1, x_and_y, temp1); //расчет правых частей системы
		for (int i = 0; i < 2; i++) //ряд циклов для расчета промежуточных значений рунге кутты по x и y
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
		t = round((t += h) * pow(10, count))/ pow(10, count); //увеличение параметра t на шаг дискретизации h 
		//(делается с округлением в связи с возникающей технической ошибкой по количеству итераций цикла  из-за особенностей хранения вещественных чисел)
		cout << t << ' ' << x_and_y[0] << ' ' << x_and_y[1] << ' ' << ksi1 << endl; //вывод в консоль
		file << t << ' ' << x_and_y[0] << ' ' << x_and_y[1] << ' ' << ksi1 << endl; //запись в файл
	}
	file.close(); //закрытие файлового потока
}
