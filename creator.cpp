#include <iostream>
#include <fstream>
#include <math.h>
#include <time.h>
#include <string.h>
#include <cstdlib>
using namespace std;
void creation(int m,int n,float density);
void write(int* matrix,int m,int n,float density);

void creation(int m,int n,float density) 
{
	int dim=m*n;
	int* matrix;matrix=new int[dim];
	for (int i=0;i<dim;++i)
		matrix[i]=0;
	int nofCars=ceil(dim*density);
	bool flag;
	int index=0;
	srand(time(NULL));
	for (int i=0;i<nofCars;++i) 
	{
		flag=true;
		while (flag) 
		{
			index=rand()%dim-1;
			if (matrix[index]==0) 
			{
				matrix[index]=rand()%2+1;
				flag=false;
			}
		}
	}
	write(matrix,m,n,density);
}

void write(int* matrix,int m,int n,float density) 
{
	int index;
	//ofstream fout("testfile"+to_string(m)+"x"+to_string(n)+"_"+to_string(density)+".csv");
	ofstream fout("problem.csv");
	for (int i=0;i<m;++i) 
	{
		for (int j=0;j<n;++j) 
		{
			index=(i+1)*(j+1)-1;
			fout<<matrix[index];
			if (j!=n-1)
				fout<<",";
		}
		if (i!=m-1)
			fout<<endl;
	}
	fout.close();
}

int main() 
{
	int m,n;
	float density;
	cout<<"N of row: ";
	cin>>m;cout<<m<<endl;
	cout<<"N of column: ";
	cin>>n;cout<<n<<endl;
	cout<<"Density: ";
	cin>>density;cout<<density<<endl;
	creation(m,n,density);
	return 0;
}
