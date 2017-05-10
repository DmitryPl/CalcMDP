//Rules
//G ::= P \0
//N ::= [0-9]+
//E ::= T{[+-]E}*
//T ::= N{[*/]T}*
//P ::= ( E ) | N

#include <cstdio>
#include <cstdlib>
#include <conio.h>
#include <cstring>
#include <string>

using std::string;

const int MAX_PATH_SIZE = 64;

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

	bool GetG();
	double GetN();
	double GetT();
	double GetE();
	double GetP();
	//bool GetI();

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
					printf("Answer : %g\n", answer);
					printf("%s\n", str);
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
	double val = GetN();
	if (flag_error != true)
	{
		while (true)
		{
			while (str[place] == '*' || str[place] == '/')
			{
				double op = str[place];
				place++;
				double val2 = GetT();
				if (flag_error != true)
				{
					if (op == '*')
						val *= val2;
					else
						val /= val2;
				}
				else
				{
					printf("Error - GetT - GetT\n");
					return NAN;
				}
			}
			return val;
		}
	}
	else
	{
		printf("Error - GetT - GetN\n");
		return NAN;
	}
}

double Calculator::GetE()
{
	double val = GetT();
	if (flag_error != true)
	{
		while (true)
		{
			while (str[place] == '+' || str[place] == '-')
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
		while (true)
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
	bool flag_read = false;
	bool flag = false;
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
		else if (str[place] == ' ' && flag_read == false ) 
		{
			place++;
		}
		else if (str[place] == '.' && point == 1)
		{
			point /= 10;
			place++;
		}
		else if (str[place] == ' ' && flag_read == true )
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
		return val;
	}
	else
	{
		printf("Error - GetN - wrong num\n");
		flag_error = true;
		return NAN;
	}
}