#include <cmath>
#include <algorithm>
#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <stdexcept>
#include <omp.h>
#include <sys/time.h>
#include <mpi.h>

#include "Utils.h"

using namespace std;

const char rc='2';
const char bc='1';
const char cv='0';


//costruttore da file;
gioco::gioco(unsigned int r,unsigned int c,const char *filename_X,int rank, int size)
{
	//std::cout<<"inizio costruzione per "<<rank<<endl;
	ifstream file_X;
	std::string line;
	unsigned int kk=r+1;
	unsigned int tre=floor(kk/size);
	cols=c;
	for(unsigned KK=0;KK<100;KK++)
	{
		vec[KK]=-1;
	}
	if(size==1)
	{
		rig=r+2;
	}
	else
	{
		if(rank==size-1)
		{
			rig=tre+1+kk%size;
		}
		else
		{
			rig=tre+1;
		}
	}
	v=new char[rig*cols];
	unsigned int contatore=0;
	unsigned int pp1=0;
	if (rank==0)
	{   
		file_X.open(filename_X);
		unsigned int i=0;
		unsigned int length=0;
		unsigned int n=0;
		std::string line1;
		getline(file_X, line1);
		length=line1.length();

		while(i<length)
		{
			n=atoi(line1.substr(0,line1.find(',')).c_str());
			vec[pp1]=n;pp1++;
			i+=line1.substr(0,line1.find(',')).length()+1;
			line1=line1.substr(line1.find(',')+1);
		}
		sort(vec,vec+pp1-1);
		std::string line2;
		unsigned int cc=0;
		while(cc<r+3)
		{
			getline(file_X, line);
			if(line.size()>0)
			{
				line2=line;
			}
			cc++;
		}//l'ultima riga è la mia prima.
		i=0;
		while(i<line2.size())
		{
			v[contatore]=line2[i];
			i+=2;
			++contatore;
		}   
		file_X.close();
	}
	MPI_Barrier(MPI_COMM_WORLD);
	unsigned int G=pp1;
	MPI_Bcast(&G,1, MPI_INT,0, MPI_COMM_WORLD);
	MPI_Bcast(&vec,G, MPI_INT,0, MPI_COMM_WORLD);
	file_X.open(filename_X);
	getline(file_X, line); //prima linea da buttare
	unsigned int ii=0;
	unsigned int UB;
	while(ii+1<rank*tre)
	{
		getline(file_X, line);ii++;
	}
	if (rank==size-1)
	{
		UB=r+1;
	}
	else
	{
		if(rank==0)
		{
			UB=(rank+1)*tre;
		}
		else
		{
			if(rank<size-1)
			{
				UB=(rank+1)*tre;
			}
			else
			{
				UB=rig;
			}
		}
	}
	while(ii<UB)
	{
		getline(file_X, line);
		unsigned int i1=0;
		if(line.size()>0)
		{
			while(i1<line.size())
			{
				v[contatore]=line[i1];
				i1+=2;
				++contatore;
			}
		}
		ii++;
	}

	file_X.close();
	//std::cout<<"ho finito la costruzione per "<<rank<<endl;
}

gioco::~gioco()
{
	delete [] v;
}

//stampa la matrice a schermo;
void gioco::stampa()
{
	for(unsigned int i=1;i<rig-1;i++)
	{
		for(unsigned int j=0;j<cols;j++)
		{
			std::cout<<v[j+i*cols];std::cout<<' ';
		}
		std::cout<<endl;   
	}
}

//gestisce scambi macchine blu;
bool gioco::red()
{
	bool F=false;
	for(unsigned int j=0;j<rig;++j)
	{
		bool ty=(v[(j+1)*cols-1]==rc && v[j*cols]==cv);
		for(unsigned int k=0;k<cols-1;++k)
		{
			if(v[k+j*cols]==rc && v[k+j*cols+1]==cv) 
			{
				if(!F)
				{
					F=true;
				}
				v[k+j*cols]=cv;
				v[k+j*cols+1]=rc;
				++k;
			}
		}
		if(ty==true)
		{
			if(!F)
			{
				F=true;
			}
			v[j*cols]=rc;
			v[(j+1)*cols-1]=cv;
		}       
	}
	return F;
}

//gestisce scambi macchine rosse
bool gioco::blu(int rank,int size)
{
	MPI_Status status,status1,status2;
	bool T=false;
	int ok[cols];
	int ok1[cols];
	for(unsigned int H=0;H<cols;H++)
	{
		ok[H]=-1;ok1[H]=-1;
	} 
	unsigned jj=0;
	unsigned jj1=0;

	for(unsigned int j=0;j<cols;++j)
	{
		for(unsigned int k=0;k<rig-1;++k)
		{
			if(v[k*cols+j]==bc && v[(k+1)*cols+j]==cv)
			{
				if(!T)
				{
					T=true;
				}

				v[k*cols+j]=cv;
				v[(k+1)*cols+j]=bc;
				if(k==rig-2)
				{
					ok[jj]=j;jj++;
				}//ok verrà inviato al successivo
				if((k==0)&&(size!=1))
				{
					ok1[jj1]=j;jj1++;
				}//ok1 al precedente
				++k;

			}
		}
	}
	//INIZIO MESSAGGISTICA
	//Vector
	if(size!=1)
	{
		jj++;jj1++;
	}
	int buf[cols];
	int buf1[cols];
	for(unsigned int H=0;H<cols;H++)
	{
		buf[H]=-1;buf1[H]=-1;
	}
	MPI_Request send1_request,send2_request;

	unsigned int lgn1=cols;
	if((rank>0)&&(size>1)&&(rank<size-1))
	{
		MPI_Isend(&ok,jj, MPI_INT,rank+1,4,MPI_Comm MPI_COMM_WORLD,&send1_request);
		MPI_Isend(&ok1,jj1, MPI_INT,rank-1,1,MPI_Comm MPI_COMM_WORLD,&send2_request);
		MPI_Recv(&buf1,lgn1, MPI_INT,rank+1,MPI_ANY_TAG,MPI_Comm MPI_COMM_WORLD,&status);
		MPI_Recv(&buf,lgn1, MPI_INT,rank-1,MPI_ANY_TAG,MPI_Comm MPI_COMM_WORLD,&status);
	}
	if(size>1&&rank==0)
	{
		MPI_Recv(&buf,lgn1, MPI_INT,size-1,3,MPI_Comm MPI_COMM_WORLD,&status);
		MPI_Recv(&buf1,lgn1, MPI_INT,rank+1,1,MPI_Comm MPI_COMM_WORLD,&status);
		MPI_Isend(&ok,jj, MPI_INT,rank+1,4,MPI_COMM_WORLD,&send1_request);
		MPI_Isend(&ok1,jj1, MPI_INT,size-1,2,MPI_COMM_WORLD,&send2_request);
	}
	if(size>1&&rank==size-1)
	{
		MPI_Isend(&ok,jj, MPI_INT,0,3,MPI_COMM_WORLD,&send1_request);
		MPI_Isend(&ok1,jj1, MPI_INT,rank-1,1,MPI_COMM_WORLD,&send2_request);
		MPI_Recv(&buf,lgn1, MPI_INT,rank-1,4,MPI_COMM_WORLD,&status);
		MPI_Recv(&buf1,lgn1, MPI_INT,0,2,MPI_COMM_WORLD,&status);
	}
	if(size>1)
	{
		MPI_Wait(&send1_request,&status1);
		MPI_Wait(&send2_request,&status2);
	}

	if(size==1)
	{
		for(unsigned int p=0;p<jj;p++)
		{
			unsigned int aa=ok[p];
			v[aa]='1';
		}
	}
	else
	{
		unsigned int a1=0;
		unsigned int a2=0;
		while(buf1[a1]>=0)
		{
			unsigned int p=buf1[a1];v[p+(rig-1)*cols]='0';a1++;
		}
		while(buf[a2]>=0)
		{
			unsigned int p=buf[a2];v[p]='1';a2++;
		}
	}


	//copio prima e ultima riga
	if(size==1)
	{
		for(unsigned int h=0;h<cols;h++)
		{
			v[(rig-1)*cols+h]=v[h];
		}
	}
	return T;
}


//in ingresso differenza tra il numero di iterazioni prec e quelle successive e l'indice del vettore iterazione corrente.
void gioco::scambi(unsigned int o,unsigned int n,int rank,int size)
{
	bool B=false;bool R=false;bool B1=false;bool R1=false;unsigned int cont=0;
	if(o==1||vec[o]%2==0)
	{
		while(cont<n)
		{
			B1=blu(rank,size);++cont;
			if(cont==n)
				break;
			MPI_Barrier(MPI_Comm MPI_COMM_WORLD);
			R1=red();++cont;
			MPI_Allreduce(&R1, &R, 1, MPI::BOOL,MPI_LOR,MPI_COMM_WORLD);
			MPI_Allreduce(&B1, &B, 1, MPI::BOOL,MPI_LOR,MPI_COMM_WORLD);

			if(B==false && R==false)
			{
				std::cout<<"Matrix stopped at step "<<cont<<endl;
				break;
			}
		}
	}

	else
	{
		while(cont<n)
		{
			R=red();++cont;
			if(cont==n)
			{
				break;
			}
			B=blu(rank,size);++cont;
			MPI_Allreduce(&R1, &R, 1, MPI::BOOL,MPI_LOR,MPI_COMM_WORLD);
			MPI_Allreduce(&B1, &B, 1, MPI::BOOL,MPI_LOR,MPI_COMM_WORLD);

			if(B==false && R==false)
			{
				std::cout<<"Matrix stopped at step "<<cont<<endl;
				break;
			}


		}
	}

}


//stampa su file


void gioco::stampafile(unsigned int n,int rank,int size)
{
	stringstream c;
	int Buff1=0;
	MPI_Status stats1;
	//std::cout<<"sono "<<rank<<" e ho "<<rig<<" righe"<<endl;
	std::string s;
	if(rank>0)
	{
		MPI_Recv(&Buff1,1, MPI_INT,rank-1,MPI_ANY_TAG,MPI_COMM_WORLD,&stats1);
	}
	c<<vec[n];
	c<<".csv";
	s=c.str();
	ofstream outfile;
	if (rank==0) {//std::cout<<c.str()<<endl;
		outfile.open (s.c_str());
	}
	else 
	{
		outfile.open (s.c_str(),fstream::app);
		outfile<<endl;
	}
	for(unsigned int h=cols;h<(rig)*cols;++h)
	{
		outfile<<v[h];
		if((h+1)%cols!=0)
		{
			outfile<<',';
		}
		else if (h!=rig*cols-1)
		{
			outfile<<endl;
		}
	}
	outfile.close();
	if(rank<size-1)
	{
		MPI_Send(&Buff1,1,MPI_INT,rank+1,1,MPI_COMM_WORLD);
	}

}

void gioco::fai(int rank,int size)
{
	int pre=0;
	unsigned int aa=0;
	while(vec[aa]>=0)
	{
		aa++;
	}


	for(unsigned int c=0;c<aa;++c)
	{
		scambi(c,vec[c]-pre,rank,size);

		MPI_Barrier(MPI_COMM_WORLD);
		stampafile(c,rank,size);
		//std::cout<<"sono "<<rank<<"e ho "<<vec[c]<<" iterazioni"<<endl;
		MPI_Barrier(MPI_COMM_WORLD);
		pre=vec[c];

	}
}


//classe densita per calcolare la densita della mia matrice

density::density(const char *filename_X)
{   ifstream file_X;
double numd0=0;
double numel=0;
file_X.open(filename_X);
if (file_X.is_open())
{
	std::string line;
	rig=0;
	cols=0;
	unsigned int lunghezza=0;
	unsigned int i=0;
	getline(file_X,line);
	getline(file_X, line);
	while(true)
	{
		lunghezza=line.size();
		if(rig!=0&&lunghezza!=line.size())
		{
			throw runtime_error("righe di diversa lunghezza o vuote");
		}

		rig++;
		i=0;
		while(i<line.size())
		{
			++numel;
			if(line[i]!='0')
			{
				++numd0;
			}

			if(line[i]!='0'&&line[i]!='1'&&line[i]!='2')
			{
				throw runtime_error("dati non validi");
			}


			i+=2;
		}
		if(line.size()%2==0)
		{
			throw runtime_error("dati non validi");
		}
		getline(file_X, line);
		if(file_X.eof())break;
	}   
	cols=numel/rig;
	rig=rig-1;
	densit=numd0/numel;
}
file_X.close();
}



//metodi che mi ritornano il numero di righe,colonne e stampa densità della matrice

unsigned int density:: colonne()
{
	return cols;
}

unsigned int density:: righe()
{
	return rig;
}

void density::stampa()
{
	std::cout<<densit;
}
double density::densty()
{
	return densit;
}



int main(int argc, char* argv[])
{
	density b("problem.csv");
	MPI::Init(argc, argv);// MPI_FORK
	int rank = MPI::COMM_WORLD.Get_rank();
	int size = MPI::COMM_WORLD.Get_size();
	gioco a(b.righe(),b.colonne(),"problem.csv",rank,size);
	a.fai(rank,size);
	MPI_Finalize();//MPI_JOIN

}
