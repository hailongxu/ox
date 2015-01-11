// money_change.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"



#include <vector>

struct unit
{
	unit(): count(0), base(0) {}
	unit(int b,int c) : count(c), base(b) {}
	int count;
	int base;
};
void print(char c,int n)
{
	for (int i=0;i<n;++i)
		printf ("%c",c);
}
int how(int sum,std::vector<unit> money,int d=0)
{
	if (sum==0)
	{
		printf (" YES");
		return 1;
	}
	else if (money.empty())
		return 0;

	unit u = money.back();
	money.pop_back();
	int sum2 = 0;
	for (int i=0;i<=u.count;++i)
	{
		if (sum-u.base*i<0)
		{
			//printf ("\n??");
			break;
		}
		printf ("\n");
		print (' ',d*2);
		printf ("%dx%d",u.base,i);
		sum2 += how(sum-u.base*i,money,d+1);
	}
	return sum2;
}

int _tmain(int argc, _TCHAR* argv[])
{
	std::vector<unit> money;
	money.push_back(unit(1,10));
	money.push_back(unit(2,12));
	money.push_back(unit(5,5));
	money.push_back(unit(10,1));
	int sum = how(20,money);
	return 0;
}


