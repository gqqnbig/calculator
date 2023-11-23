#include <iostream>

using std::cout;
using std::cin;
using std::endl;


void remove_spaces(char* str) {
	char* d = str;
	do {
		while (*d == ' ') {
			++d;
		}
		*str++ = *d++;
	} while (*str != '\0');
}

double times(double a, double b)
{
	return a * b;
}


double divides(double a, double b)
{
	return a / b;
}

double adds(double a, double b)
{
	return a + b;
}

double substracts(double a, double b)
{
	return a - b;
}

double calculate(char* str, double value);

double calculate(double value, char* str);




double atof_s(const char* p)
{
	if (*p < '0' || *p>'9')
	{
		char* err = new char[30];
		strcpy_s(err, 30, p);
		strcat_s(err, 30, " is not a number.");
		throw err;
	}

	double x = atof(p);
	if (x == 0 && *p != '0')
	{
		char* err = new char[30];
		strcpy_s(err, 30, p);
		strcat_s(err, 30, " is not a number.");
		throw err;
	}
	return x;
}


// If not found, return the length of str.
size_t strcspnr(const char* str, const char* control)
{
	size_t length = strlen(control);

	const char* maxP = str + strlen(str);
	for (int i = 0; i < length; i++)
	{
		const char* p = strrchr(str, *(control + i));
		if (p != nullptr && p < maxP)
			maxP = p;
	}

	return maxP - str;
}

// char* input, double v1, char* middle are input
// double* x, char*& inputBefore are output
// Return: is value used
bool findLeft(char* input, double v1, char* middle, double* x, char*& inputBefore)
{
	inputBefore = input;

	char* pBefore;
	size_t p = strcspnr(inputBefore, "+-*/");
	if (p == strlen(inputBefore))
		pBefore = inputBefore;
	else
		pBefore = inputBefore + p + 1;


	if (*pBefore == '\0')
	{
		*x = v1;
		return true;
	}
	else
	{
		*x = atof_s(pBefore);
		*pBefore = '\0';
		return false;
	}
}

// Return: is value used
bool findRight(double v2, char* middle, double* y, char*& inputAfter)
{

	inputAfter = middle + 1;
	char* pAfter = inputAfter;
	if (*pAfter == '\0')
	{
		*y = v2;
		return true;
	}

	size_t	p = strcspn(inputAfter, "+-*/");
	if (p == strlen(inputAfter))
	{
		*y = atof(pAfter);
		*inputAfter = '\0';
	}
	else
	{
		inputAfter = inputAfter + p;
		char b = *(pAfter + p);
		*(pAfter + p) = '\0';
		*y = atof_s(pAfter);
		*(pAfter + p) = b;
	}
	return false;
}

// input is input.
// other parameters are output.
bool findFirstCalculation(double v1, char* input, double v2, double (*&f)(double, double), double* x, double* y, char*& inputBefore, char*& inputAfter)
{
	size_t p = strcspn(input, "*/");

	if (p == strlen(input))
	{
		p = strcspn(input, "+-");

		if (p == strlen(input))
			throw "Not implemented";

		if (*(input + p) == '+')
			f = adds;
		else //if (*(input + p) == '-')
			f = substracts;

	}
	else
	{
		if (*(input + p) == '/')
			f = divides;
		else //if (*(input + p) == '*')
			f = times;
	}
	char* middle = input + p;
	*middle = '\0';

	bool b1 = findLeft(input, v1, middle, x, inputBefore);
	bool b2 = findRight(v2, middle, y, inputAfter);
	return b1 || b2;
}


void main()
{
	const int length = 80;
	char input[length];

	cout << "Type your expression" << endl;
	cin.getline(input, length);
	remove_spaces(input);

	cout << input;


	try
	{
		double res = calculate(0, input);

		cout << endl << "Result is " << res;
	}
	catch (const char* ex)
	{
		cout << endl << "Error: " << ex;
		delete ex;
	}
}


double calculate(double value, char* str)
{
	if (*str == '\0')
		return value;

	double (*f)(double, double) = nullptr;
	double* x = new double;
	double* y = new double;
	char* inputBefore = nullptr;
	char* inputAfter = nullptr;

	bool isValueUsed = findFirstCalculation(value, str, 0, f, x, y, inputBefore, inputAfter);
	str = nullptr;


	cout << endl << "previous term is " << *x;
	cout << endl << "next term is " << *y;

	double z = f(*x, *y);

	size_t lastIndex = strlen(inputBefore) - 1;
	if (lastIndex < 0)
		return calculate(z, inputAfter);

	double res;
	if (*(inputBefore + lastIndex) == '*' || *(inputBefore + lastIndex) == '/')
		res = calculate(calculate(inputBefore, z), inputAfter);
	else
		res = calculate(inputBefore, calculate(z, inputAfter));

	//TODO: test: 1*2 + 3*4 + 5*6
	if (isValueUsed == false)
		return calculate(value, inputBefore) 

		delete x;
	delete y;
}


double calculate(char* str, double value)
{
	if (*str == '\0')
		return value;

	double (*f)(double, double) = nullptr;
	double* x = new double;
	double* y = new double;
	char* inputBefore = nullptr;
	char* inputAfter = nullptr;

	findFirstCalculation(0, str, value, f, x, y, inputBefore, inputAfter);
	str = nullptr;


	cout << endl << "previous term is " << *x;
	cout << endl << "next term is " << *y;

	double z = f(*x, *y);

	size_t lastIndex = strlen(inputBefore) - 1;
	if (lastIndex < 0)
		return calculate(z, inputAfter);

	if (*(inputBefore + lastIndex) == '*' || *(inputBefore + lastIndex) == '/')
		return calculate(calculate(inputBefore, z), inputAfter);
	else
		return calculate(inputBefore, calculate(z, inputAfter));


	delete x;
	delete y;
}