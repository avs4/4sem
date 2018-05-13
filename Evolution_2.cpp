#include "stdio.h"
#include "stdlib.h"
#include "time.h"
#include "windows.h"
#include <iostream>

using namespace std;

//__0__Tables

int N_species=4;
int species[4][3]={{0,0,0},{3,3,20},{2,1,60},{1,2,40}};
int direction[4][2]={{0,1},{0,-1},{1,0},{-1,0}};

//__1__Animal

struct Animal
{
	int type;
	int period;
	int force;
	int limit;
	int energy;
};

//__2__Field

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
		for (x=0; x<=xmax+1; x++) //Creating new field
			new_fld[x]=new Animal[ymax+2];
		init_field(new_fld,xmax,ymax,0);
		srand(time(NULL));
		for (x=1; x<=xmax; x++) //Moving those animals that can be moved
			for (y=1; y<=ymax; y++)
				if (fld[x][y].type>0) //If there is an animal on the cell
				{
					if (T%fld[x][y].period==0) //If it's time for animal to move
					{
						for (i=0; i<=3; i++)
							possibility[i]=0;
						N_possible=0;
						for (i=0; i<=3; i++) //Checking what neighboring cells are not walls (type of walls is -1)
							if (fld[x+direction[i][0]][y+direction[i][1]].type>-1)
							{
								possibility[i]=1;
								N_possible++;
							}
							else
								possibility[i]=0;
						if (N_possible>0) //If there is an ability to go somewhere
						{
							variant=rand()%N_possible; //Choosing a random number among the POSSIBLE variants
							j=-1;
							for (i=0; i<=3; i++) //Among ALL
								if (possibility[i]==1)
								{
									j++; //Among the POSSIBLE
									if (j==variant) //If the selected random variant is found among the POSSIBLE
									{
										variant=i; //Redesignating the variant to the number among ALL variants
										break;
									}
								}
							xx=x+direction[variant][0];
							yy=y+direction[variant][1];
						}
						else //If there is NOwhere to go
						{
							xx=x;
							yy=y;
						}
					}
					else //If it's NOT time for animal to move
					{
						xx=x;
						yy=y;
					}
					if (fld[x][y].force*fld[x][y].energy>new_fld[xx][yy].force*new_fld[xx][yy].energy) //If the current animal is stronger than the one who stands in the new field
						init_type(new_fld[xx][yy],fld[x][y].type); //On the new cell of the new field the current animal appears
					init_energy(new_fld[xx][yy],new_fld[xx][yy].energy+fld[x][y].energy); //Energy changes in any case
				}
		for (x=1; x<=xmax; x++) //Reproducing those animals that want and can reproduce
			for (y=1; y<=ymax; y++)
				if (new_fld[x][y].type>0) //If there is an animal on the cell
				{
					if (new_fld[x][y].energy>new_fld[x][y].limit) //If it wants to reproduce
					{
						for (i=0; i<=3; i++)
							possibility[i]=0;
						N_possible=0;
						for (i=0; i<=3; i++) //Checking where the offspring can be left
							if (new_fld[x+direction[i][0]][y+direction[i][1]].type==0)
							{
								possibility[i]=1;
								N_possible++;
							}
							else
								possibility[i]=0;
						if (N_possible>0) //If there is ability to leave an offspring somewhere
						{
							variant=rand()%N_possible; //Choosing a random number among the POSSIBLE variants
							j=-1;
							for (i=0; i<=3; i++)
								if (possibility[i]==1)
								{
									j++;
									if (j==variant) //If the selected random variant is found among the POSSIBLE
									{
										variant=i; //Redesignating the variant to the number among ALL variants
										break;
									}
								}
							xx=x+direction[variant][0]; //Coordinates of the offspring
							yy=y+direction[variant][1];
							init_energy(new_fld[xx][yy],int(new_fld[x][y].energy/2)+new_fld[xx][yy].energy); //Energy of the offspring
							if ((int(new_fld[x][y].energy/2)+new_fld[xx][yy].energy)<=new_fld[xx][yy].limit) //Type of the offspring
								init_type(new_fld[xx][yy],new_fld[x][y].type);
							else
								init_type(new_fld[xx][yy],0);
							init_energy(new_fld[x][y],int(new_fld[x][y].energy/2)+new_fld[x][y].energy%2); //Energy of the parent
						}
						else //If every neighboring cell is engaged
							init_type(new_fld[x][y],0); //Animal turns into an empty cell
					}
				}
		for (x=1; x<=xmax; x++) //Overwriting the original array
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