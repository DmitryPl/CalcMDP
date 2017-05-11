// Rules
// G ::= P \0
// N ::= P | I | [ + - ] [ 0 - 9 ] +
// E ::= T { [ + - ] E } *
// T ::= D { [ * / ] T } *
// P ::= [ ( E ) | E ] *
// I ::= [ sin ( | cos ( | ln(  | sqrt( | e |  pi ] E )
// D ::= N ^ { ( E ) | N }

#include <cstdio>
#include <cstdlib>
#include <conio.h>
#include <cstring>
#include <string>
#include <cmath>

using std::string;

const int MAX_PATH_SIZE = 128;
const double PI = 3.1415926535897932385;
const double E = 2.7182818284590452354;

class Calculator
{
private:
	size_t place;
	char* str;
	bool flag_error;
	double answer;
	char* output;
	string error;
	FILE* file;

	bool Files(char* output);
	bool Read();
	double GetFunc(size_t n);
	bool GetG();
	double GetN();
	double GetT();
	double GetE();
	double GetP();
	double GetD();
	double GetI();

public:
	bool Dialog(char* output);

	explicit Calculator() : answer(NAN), place(0)
	{
		flag_error = false; error = "Default"; str = ""; file = NULL;
	}
};

void Files_Main(char* buffer);

void main(int argc, char* argv[])
{
	printf("Running from %s\n", argv[0]);
	char output[MAX_PATH_SIZE] = "";
	Files_Main(output);

	Calculator* Calc = new Calculator();
	if (Calc->Dialog(output))
	{
		printf("\nEND\n");
		getch();
	}
	else
	{
		printf("\nERROR CALC\n");
		getch();
	}
}

void Files_Main(char* buffer)
{
	printf("\nEnter \"yes\" to use standart file test.txt\n or enter way to directory like \"D:\\test.txt\"\n");
	scanf("%s", buffer);
	if (!strcmp("yes", buffer))
	{
		strcpy(buffer, "test.txt");
	}
	else
	{
		printf("\n Enter way to file\n");
		scanf("%s", buffer);
	}
}

bool Calculator::Dialog(char* output)
{
	try
	{
		if (!Files(output))
		{
			error = "Error - Dialog - Files\n";
			throw error;
		}
		else
		{
			if (!Read())
			{
				error = "Error - Dialog - Read\n";
				throw error;
			}
			else
			{
				if (answer != NAN)
				{
					printf("%s = ", str);
					printf("%g\n", answer);
					return true;
				}
				else
				{
					error = "Error - answer - file\n";
					throw error;
				}
			}
		}
	}
	catch (string error)
	{
		printf("%s\n", error.c_str());
		return false;
	}
}

bool Calculator::Files(char* output)
{
	file = fopen(output, "r");
	if (file != NULL)
	{
		return true;
	}
	else
	{
		printf("Bad file!\n");
		return false;
	}
}

bool Calculator::Read()
{
	fseek(file, 0, SEEK_END);
	int end = ftell(file);
	fseek(file, 0, SEEK_SET);
	str = (char*)malloc((end + 1)*sizeof(char));
	if (fgets(str, end + 1, file))
	{
		if (GetG())
		{
			return true;
		}
		else
		{
			printf("Error - Read - GetG\n");
			return false;
		}
	}
	else
	{
		printf("Error - Read - fgets\n");
		return false;
	}
}

bool Calculator::GetG()
{
	answer = GetP();
	if (flag_error != true)
	{
		while (true)
		{
			if (str[place] == '\0')
			{
				return true;
			}
			else
			{
				printf("Error - GetG - Eof\n");
				return false;
			}
		}
	}
	else
	{
		printf("Error - GetG - GetP\n");
		return false;
	}
}

double Calculator::GetT()
{
	double val = GetD();
	if (flag_error != true)
	{
		if (str[place] == '*' || str[place] == '/')
		{
			double op = str[place];
			place++;
			double val2 = GetT();
			if (flag_error != true)
			{
				if (op == '*')
					val *= val2;
				else
					if (val2 != 0)
					{
						val /= val2;
					}
					else
					{
						printf("Error - GetT - 0 val2\n");
						return NAN;
					}
			}
			else
			{
				printf("Error - GetT - GetT\n");
				return NAN;
			}
		}
		return val;
	}
	else
	{
		printf("Error - GetT - GetD\n");
		return NAN;
	}
}

double Calculator::GetE()
{
	double val = GetT();
	if (flag_error != true)
	{
		if (str[place] == '+' || str[place] == '-')
		{
			double op = str[place];
			place++;
			double val2 = GetE();
			if (flag_error != true)
			{
				if (op == '+')
					val += val2;
				else
					val -= val2;
			}
			else
			{
				printf("Error - GetE - GetE\n");
				return NAN;
			}
		}
		return val;
	}
	else
	{
		printf("Error - GetE - GetT\n");
		return NAN;
	}
}

double Calculator::GetP()
{
	try
	{
		if (str[place] == '(')
		{
			place++;
			double val = GetE();
			if (flag_error != true)
			{
				if (str[place] == ')')
				{
					place++;
					while (str[place] == ' ')
					{
						place++;
					}
					return val;
				}
				else
				{
					error = "Error - GetP - ()\n";
					throw error;
				}
			}
			else
			{
				error = "Error - GetP - GetE\n";
				throw error;
			}
		}
		else
		{
			double val = GetE();
			if (flag_error != true)
			{
				return val;
			}
			else
			{
				error = "Error - GetP - GetE\n";
				throw error;
			}
		}
	}
	catch (string error)
	{
		printf("%s", error.c_str());
		flag_error = true;
		return NAN;
	}
}

double Calculator::GetN()
{
	double val = NAN;
	double point = 1;
	bool flag_sign = false;
	bool flag_read = false;
	bool flag = false;
	while (str[place] == ' ')
	{
		place++;
	}
	if (str[place] == '(')
	{
		val = GetP();
		if (flag_error == true)
		{
			printf("Error - GetN - GetP\n");
			return NAN;
		}
		else
			return val;
	}
	else if ('a' <= str[place] && str[place] <= 'z')
	{
		val = GetI();
		if (flag_error == true)
		{
			printf("Error - GetN - GetI\n");
			return NAN;
		}
		else
		{
			while (str[place] == ' ')
			{
				place++;
			}
			return val;
		}
	}
	else
	{
		if (str[place] == '-')
		{
			flag_sign = true;
			place++;
		}
		if (str[place] == '+')
		{
			place++;
		}
		while (!flag)
		{
			if ('0' <= str[place] && str[place] <= '9')
			{
				if (point == 1)
				{
					if (flag_read == false)
					{
						val = 0;
						flag_read = true;
					}
					val = val * 10 + str[place] - '0';
					place++;
				}
				else
				{
					val = val + (str[place] - '0') * point;
					point /= 10;
					place++;
				}
			}
			else if (str[place] == '.' && point == 1)
			{
				point /= 10;
				place++;
			}
			else if (str[place] == ' ' && flag_read == true)
			{
				place++;
				flag = true;
			}
			else
			{
				flag = true;
			}
		}
		if ((point == 1 || point < 0.2) && flag_read == true)
		{
			if (flag_sign == true)
			{
				return -1 * val;
			}
			else
				return val;
		}
		else
		{
			printf("Error - GetN - wrong num\n");
			flag_error = true;
			return NAN;
		}
	}
}

double Calculator::GetD()
{
	double val = GetN();
	if (flag_error != true)
	{
		if (str[place] == '^')
		{
			place++;
			while (str[place] == ' ')
			{
				place++;
			}
			double val2;
			if (str[place] == '(')
			{
				place++;
				val2 = GetE();
				if (str[place] == ')')
				{
					place++;
					if (flag_error != true)
					{
						return pow(val, val2);
					}
					else
					{
						printf("Error - GetD - GetE\n");
						return NAN;
					}
				}
				else
				{
					printf("Error - GetD - ()\n");
					return NAN;
				}
			}
			else
			{
				val2 = GetN();
				if (flag_error != true)
				{
					return pow(val, val2);
				}
				else
				{
					printf("Error - GetD - GetN\n");
					return NAN;
				}
			}
		}
		else
			return val;
	}
	else
	{
		printf("Error - GetE - GetT\n");
		return NAN;
	}
}

double Calculator::GetI()
{
	try
	{
		double val = NAN;
		if (str[place] == 'e')
		{
			place++;
			while (str[place] == ' ')
			{
				place++;
			}
			return E;
		}
		if (str[place] == 'p' && str[place + 1] == 'i')
		{
			place += 2;
			while (str[place] == ' ')
			{
				place++;
			}
			return PI;
		}
		if (str[place] == 'l' && str[place + 1] == 'n')
		{
			val = GetFunc(2);
			if (flag_error != true)
			{
				if (val < 0)
				{
					return log(val);
				}
				else
				{
					error = "Error - GetI - < 0\n";
				}
			}
			else
			{
				error = "Error - GetI - ln\n";
				throw error;
			}
		}
		else if (str[place] == 's' && str[place + 1] == 'i' && str[place + 2] == 'n')
		{
			val = GetFunc(3);
			if (flag_error != true)
			{
				return sin(val);
			}
			else
			{
				error = "Error - GetI - Sin\n";
				throw error;
			}
		}
		else if (str[place] == 'c' && str[place + 1] == 'o' && str[place + 2] == 's')
		{
			val = GetFunc(3);
			if (flag_error != true)
			{
				return cos(val);
			}
			else
			{
				error = "Error - GetI - Cos\n";
				throw error;
			}
		}
		else if (str[place] == 's' && str[place + 1] == 'q' && str[place + 2] == 'r' && str[place + 3] == 't')
		{
			val = GetFunc(4);
			if (flag_error != true)
			{
				if (val >= 0)
				{
					return sqrt(val);
				}
				else
				{
					error = "Error - GetI - < 0\n";
					throw error;
				}
			}
			else
			{
				error = "Error - GetI - Sqrt\n";
				throw error;
			}
		}
		else
		{
			error = "Error - GetI - Func\n";
			throw error;
		}
	}
	catch (string error)
	{
		printf("%s", error.c_str());
		flag_error = true;
		return NAN;
	}
}

double Calculator::GetFunc(size_t n)
{
	try
	{
		place += n;
		while (str[place] == ' ')
		{
			place++;
		}
		if (str[place] == '(')
		{
			place++;
			double val = GetE();
			if (flag_error == true)
			{
				error = "Error - GetFunc - GetE\n";
				throw error;
			}
			else
			{
				if (str[place] == ')')
				{
					place++;
					while (str[place] == ' ')
					{
						place++;
					}
					return val;
				}
				else
				{
					error = "Error - GetFunc - )\n";
					throw error;
				}
			}
		}
		else
		{
			error = "Error - GetFunc - ( \n";
			throw error;
		}
	}
	catch (string error)
	{
		printf("%s", error.c_str());
		flag_error = true;
		return NAN;
	}
}
