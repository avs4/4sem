#include <iostream>
#include <cstring>

using namespace std;

/*class Symbol
{
private:
	char *A;
	int n;
public:
	Symbol(int nn=80)
	{
		n=nn;
		A = new char[n];
		for (int i=0; i<n; i++)
			A[i]=0;
	};
	~Symbol()
	{
		delete[] A;
	};
};*/

class Complex//:public Symbol
{
private:
	double a,b;
	char c;
public:
	Complex(double aa,double bb,char cc)
	{
		a=aa;
		b=bb;
		c=cc;
	};
	Complex operator+=(Complex X)
	{
		a+=X.a;
		b+=X.b;
		return *this;
	};
	void Show(char s[])
	{
		cout<<s<<a<<" "<<b<<" "<<c<<"\n";
	};
	void Show()
	{
		cout<<a<<" "<<b<<" "<<c<<"\n";
	};
};

int main()
{
	Complex A(1,2,' '),B(3,4,' ');
	A.Show("A = ");
	B.Show("B = ");
	A+=B;
	A.Show();
	system("pause");
};
