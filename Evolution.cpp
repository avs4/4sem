#include "stdio.h"
#include "stdlib.h"
#include "time.h"
#include "windows.h"
#include <iostream>

using namespace std;

//__0__Таблицы

int N_species=4;
int species[4][3]={{0,0,0},{3,3,20},{2,1,60},{1,2,40}};
int direction[4][2]={{0,1},{0,-1},{1,0},{-1,0}};

//__1__Животное

struct Animal
{
	int type;
	int period;
	int force;
	int limit;
	int energy;
};

//__2__Поле

class Field
{
	Animal **fld;
	int xmax, ymax, T;
	void init_type(Animal &A,int t)
	{
		if ((t==0)||(t==1)||(t==2)||(t==3))
		{
			A.type=t;
			A.period=species[t][0];
			A.force=species[t][1];
			A.limit=species[t][2];
		}
		else
		{
			A.type=t;
			A.period=t;
			A.force=t;
			A.limit=t;
		}
	}
	void init_energy(Animal &A,int e)
	{
		A.energy=e;
	}
	void init_field(Animal **F,int xm, int ym, int N)
	{
		int i,x,y;
		for (x=0; x<=xm+1; x++)
		{
			for (y=0; y<=ym+1; y++)
			{
				if ((x==0)||(x==xm+1)||(y==0)||(y==ym+1))
				{
					init_type(F[x][y],-1);
					init_energy(F[x][y],-1);
				}
				else
				{
					init_type(F[x][y],0);
					init_energy(F[x][y],0);
				}
			}
		}
		if (N>0)
		{
			srand(time(NULL));
			for (i=0; i<N; i++)
			{
				x=rand()%xm+1;
				y=rand()%ym+1;
				init_type(F[x][y],rand()%(N_species-1)+1);
				init_energy(F[x][y],F[x][y].limit/2);
			}
		}
	}
public:
	Field()
	{
		Field(20,20,100);
	}
	Field(int xm, int ym, int N)
	{
		int x;
		fld=new Animal*[xm+2];
		for (x=0; x<=xm+1; x++)
			fld[x]=new Animal[ym+2];
		init_field(fld,xm,ym,N);
		xmax=xm;
		ymax=ym;
		T=0;
	}
	~Field()
	{
		int x;
		for (x=0; x<=xmax+1; x++)
		{
			delete[] fld[x];
		}
		delete[] fld;
	}
	int type(int x,int y) {return fld[x][y].type;}
	void type(int x,int y,int t) {init_type(fld[x][y],t);}
	int period(int x,int y) {return fld[x][y].period;}
	int force(int x,int y) {return fld[x][y].force;}
	int limit(int x,int y) {return fld[x][y].limit;}
	int energy(int x,int y) {return fld[x][y].energy;}
	void energy(int x,int y,int e) {init_energy(fld[x][y],e);}
	void Change()
	{
		int i,j,x,y,xx,yy,N_possible,variant;
		int possibility[4];
		Animal **new_fld;

		new_fld=new Animal*[xmax+2];
		for (x=0; x<=xmax+1; x++) //Создадим новое поле
			new_fld[x]=new Animal[ymax+2];
		init_field(new_fld,xmax,ymax,0);
		srand(time(NULL));
		for (x=1; x<=xmax; x++) //Переместим тех животных, которых можно переместить
			for (y=1; y<=ymax; y++)
				if (fld[x][y].type>0) //Если на клетке стоит животное
				{
					if (T%fld[x][y].period==0) //Если ему пора двигаться
					{
						for (i=0; i<=3; i++)
							possibility[i]=0;
						N_possible=0;
						for (i=0; i<=3; i++) //Проверим, где отсутствуют стенки
							if (fld[x+direction[i][0]][y+direction[i][1]].type>-1)
							{
								possibility[i]=1;
								N_possible++;
							}
							else
								possibility[i]=0;
						if (N_possible>0) //Если есть куда пойти
						{
							variant=rand()%N_possible; //Случайный номер среди возможных вариантов
							j=-1;
							for (i=0; i<=3; i++)
								if (possibility[i]==1)
								{
									j++;
									if (j==variant) //Если найден выбранный случайный вариант среди возможных
									{
										variant=i; //Переобозначаем на номер среди всех вариантов
										break;
									}
								}
							xx=x+direction[variant][0];
							yy=y+direction[variant][1];
						}
						else //Если идти некуда
						{
							xx=x;
							yy=y;
						}
					}
					else //Если ему не пора двигаться
					{
						xx=x;
						yy=y;
					}
					if (fld[x][y].force*fld[x][y].energy>new_fld[xx][yy].force*new_fld[xx][yy].energy) //Если текущее животное сильнее того, кто стоит на новом поле
						init_type(new_fld[xx][yy],fld[x][y].type); //На новой клетке нового поля оказывается текущее
					init_energy(new_fld[xx][yy],new_fld[xx][yy].energy+fld[x][y].energy);
				}
		for (x=1; x<=xmax; x++) //Размножим тех животных, которые хотят и могут размножаться
			for (y=1; y<=ymax; y++)
				if (new_fld[x][y].type>0) //Если на клетке стоит животное
				{
					if (new_fld[x][y].energy>new_fld[x][y].limit) //Если оно хочет размножаться
					{
						for (i=0; i<=3; i++)
							possibility[i]=0;
						N_possible=0;
						for (i=0; i<=3; i++) //Проверим, где можно оставить потомка
							if (new_fld[x+direction[i][0]][y+direction[i][1]].type==0)
							{
								possibility[i]=1;
								N_possible++;
							}
							else
								possibility[i]=0;
						if (N_possible>0) //Если есть где оставить потомка
						{
							variant=rand()%N_possible; //Случайный номер среди возможных вариантов
							j=-1;
							for (i=0; i<=3; i++)
								if (possibility[i]==1)
								{
									j++;
									if (j==variant) //Если найден выбранный случайный вариант среди возможных
									{
										variant=i; //Переобозначаем на номер среди всех вариантов
										break;
									}
								}
							xx=x+direction[variant][0];
							yy=y+direction[variant][1];
							init_type(new_fld[xx][yy],new_fld[x][y].type);
							init_energy(new_fld[xx][yy],int(new_fld[x][y].energy/2));
							init_energy(new_fld[x][y],int(new_fld[x][y].energy/2)+new_fld[x][y].energy%2);
						}
						else //Если всё занято
							init_type(new_fld[x][y],0);
					}
				}
		for (x=1; x<=xmax; x++) //Перезапишем исходный массив
			for (y=1; y<=ymax; y++)
			{
				init_type(fld[x][y],new_fld[x][y].type);
				init_energy(fld[x][y],new_fld[x][y].energy);
			}
		for (x=0; x<=xmax+1; x++)
		{
			delete[] new_fld[x];
		}
		delete[] new_fld;

		T=T+1;
	}
	void Out()
	{
		int x,y;
		for (x=0; x<=xmax+1; x++)
		{
			for (y=0; y<=ymax+1; y++)
				if (fld[x][y].type>0)
					cout<<fld[x][y].type;
				else if (fld[x][y].type<0)
					cout<<'#';
				else
					cout<<' ';
			cout<<'\n';
		}
	}
	void Statistics(char variant)
	{
		int x,y;
		int count=0,count_1=0,count_2=0,count_3=0;
		int energy=0,energy_1=0,energy_2=0,energy_3=0;
		for (x=1; x<=xmax; x++)
			for (y=1; y<=ymax; y++)
				if (fld[x][y].type>0)
				{
					count++;
					energy=energy+fld[x][y].energy;
					if (fld[x][y].type==1)
					{
						count_1++;
						energy_1=energy_1+fld[x][y].energy;
					}
					else if (fld[x][y].type==2)
					{
						count_2++;
						energy_2=energy_2+fld[x][y].energy;
					}
					else
					{
						count_3++;
						energy_3=energy_3+fld[x][y].energy;
					}
				}
		if (variant=='o')
		{
			cout<<"\ncount="<<count<<"\tenergy="<<energy;
			cout<<"\ncount_1="<<count_1<<"\tenergy_1="<<energy_1;
			cout<<"\ncount_2="<<count_2<<"\tenergy_2="<<energy_2;
			cout<<"\ncount_3="<<count_3<<"\tenergy_3="<<energy_3;
		}
		else if (variant=='f')
		{
			FILE *f;
			if (T==0)
			{
				fopen_s(&f,"Statistics.xls","w");
				fprintf(f,"T\tcount\tenergy\tcount_1\tenergy_1\tcount_2\tenergy_2\tcount_3\tenergy_3");
			}
			else
				fopen_s(&f,"Statistics.xls","a");
			fprintf(f,"\n%d",T);
			fprintf(f,"\t%d",count);
			fprintf(f,"\t%d",energy);
			fprintf(f,"\t%d",count_1);
			fprintf(f,"\t%d",energy_1);
			fprintf(f,"\t%d",count_2);
			fprintf(f,"\t%d",energy_2);
			fprintf(f,"\t%d",count_3);
			fprintf(f,"\t%d",energy_3);
			fclose(f);
		}
	}
	void Deviation(char variant)
	{
		//FIXME
	}
};


void main()
{
	int i,M,N;
	Field F(20,20,150);
	cout<<"How many cycles?\nN=";
	cin>>N;
	system("cls");
	F.Out();
	F.Statistics('f');
	Sleep(1000);
	for (i=0; i<N; i++)
	{
		F.Change();
		system("cls");
		F.Out();
		F.Statistics('f');
		Sleep(1000);
	}

	system("pause");
}