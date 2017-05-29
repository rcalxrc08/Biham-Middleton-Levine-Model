#include<algorithm>
#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include "class2.h"

using namespace std;

const char rc='2';
const char bc='1';
const char cv='0';


//costruttore:abbiamo optato per una versione ibrida che include sia una struttura densa(il classico vettore matrice)
//sia una struttura spars(un vettore per le blu ordinato per colonne,uno per le rosse ordinato per righe e
//altri due vettori su cui viene salvata la distribuzione cumulativa delle macchine blu e rosse)

traffico::traffico(unsigned int r,unsigned int c,const char *filename_X){
   ifstream file_X;
   string line;
   rig=r;
   cols=c;
   v=new char[rig*cols];
   unsigned int contatore=0;
   file_X.open(filename_X);
   if (file_X.is_open()){
      unsigned int i=0;
      unsigned int length=0;
      unsigned int n=0;
      string line1;
      getline(file_X, line1);
      length=line1.length();                  //leggo la prima riga spezzo e la metto nel vettore delle iter
      while(i<length){
         n=atoi(line1.substr(0,line1.find(',')).c_str());
         vec.push_back(n);
         i+=line1.substr(0,line1.find(',')).length()+1;
         line1=line1.substr(line1.find(',')+1);
           }
      sort(vec.begin(),vec.end());
     
      while(!file_X.eof()){
         getline(file_X, line);
            i=0;
            while(i<line.size()){
               v[contatore]=line[i];  
             switch(line[i]){
	        case '0':
		   break;
		case '1':{
		   blucar.push_back(((contatore-(contatore/cols)*cols)*rig)+(contatore/cols));
		   break;}
	        case '2':{
		   redcar.push_back(contatore);
		   break;}
		default:
		   cout<<"dati non validi";
                   exit(1);}
               i+=2;
               ++contatore;
                       }
                     }
      file_X.close();
              }

   sort(blucar.begin(),blucar.end());                       //cosi il vettore blu è ordinato per colonne   
   unsigned int n=1;
   unsigned int i=0;
   unsigned int cont=0;
   numsurig.push_back(0);
   while(i<redcar.size()){                                //riempio il vettore della distribuzione(cumulata) dei due vettori
      while(i<redcar.size() && redcar[i]<n*cols){
         ++cont;
         ++i;
                      }
      numsurig.push_back(cont);
      ++n;
      }
   while(numsurig.size()<rig+1){              
      numsurig.push_back(cont);
     }

   n=1;i=0;cont=0;
   numcol.push_back(0);
   while(i<blucar.size()){
      while(i<blucar.size()&&blucar[i]<n*rig ){
         ++cont;
         ++i;
         }
      numcol.push_back(cont);
      ++n;
      }
   while(numcol.size()<cols+1){
      numcol.push_back(cont);
   }
}
//distruttore
traffico::~traffico(){
delete [] v;
}

//stampa la matrice a schermo(optional per il progetto);
void traffico::stampa(){
   for(unsigned int i=0;i<rig;++i){
      for(unsigned int j=0;j<cols;++j){
         cout<<v[j+i*cols];cout<<' ';}
      cout<<endl;
           }
}

void traffico::ordinared(unsigned int ri){                           //riordina il vettore delle rossi(solo le macchine che giaciono sulla riga su cui giace ri vengono riordinate)
   for(unsigned int contpercol=numsurig[ri/cols+1]-1;contpercol>numsurig[ri/cols];--contpercol){
      swap(redcar[contpercol],redcar[contpercol-1]);
       }
  }

//gestisce scambi macchine blu;

bool traffico::red(){            
   bool Ftot=false;
   int prig=-1;                                                      //bool che mi indica se è avvenuto o no almeno un movimento
   for(unsigned int j=0;j<rig;++j){                                    //ciclo sul numero di righe
      if(numsurig[j]<redcar.size() && redcar[numsurig[j]]%cols==0){      //tengo traccia della macchina sulla prima colonna(se c'è)
         prig=redcar[numsurig[j]];
         }
      for(unsigned int i=numsurig[j];i<numsurig[j+1];++i){            //ciclo sul vettore rosso(solo sulla parte corrispondente alla riga che sta venendo analizzata)
         if((redcar[i]+1)%cols!=0){                                   //movimento se non è sull'ultima colonna la macchina rossa
            if(v[redcar[i]+1]==cv){ 
               if(!Ftot){Ftot=true;}
               v[redcar[i]]=cv;
               v[redcar[i]+1]=rc;
               ++redcar[i];
                }
         }
         else{                                                      //movimento se la macchina rossa è sull'ultima colonna
            if(v[redcar[i]-cols+1]==cv&&redcar[i]+1!=prig+cols){
               if(!Ftot){Ftot=true;}
               v[redcar[i]]=cv;
               v[redcar[i]-cols+1]=rc;
               redcar[i]-=cols-1;
               ordinared(redcar[i]);
              }
         }
        }
        
     }

   return Ftot; 
}

void traffico::ordinablu(unsigned int bi){                            //riordina il vettore delle blu(solo le macchine che giacciono sulla colonna su cui giace bi)
   for(unsigned int contperrig=numcol[bi/rig+1]-1;contperrig>numcol[bi/rig];--contperrig){
       swap(blucar[contperrig],blucar[contperrig-1]);
        }
   }

//gestisce scambi macchine rosse
bool traffico::blu(){                                                //lo spostamento delle blu è uguale a quello delle rosse  
   bool Ttot=false; 
   int prig=-1; 
   unsigned int trasf=0;                                                //NOTA:tras è la variabile che mi permette di trasformare le coordinate di una macchina blu(per colonne)
   for(unsigned int j=0;j<cols;++j){                                 //nelle coordinate delle macchine rosse(per righe)
      if(numcol[j]<blucar.size()&&blucar[numcol[j]]%rig==0){
         prig=blucar[numcol[j]];}
   for(unsigned int i=numcol[j];i<numcol[j+1];++i){
      trasf=(blucar[i]-(blucar[i]/rig)*rig)*cols+(blucar[i]/rig);
      if((blucar[i]+1)%rig!=0){
         if(v[trasf+cols]==cv){
            if(!Ttot){Ttot=true;}
            v[trasf]=cv;
            v[trasf+cols]=bc;
            ++blucar[i];
               }
             }
      else{
         if(v[trasf-cols*(rig-1)]==cv&&blucar[i]+1!=prig+rig){
            if(!Ttot){Ttot=true;}
            v[trasf]=cv;
            v[trasf-cols*(rig-1)]=bc;
            blucar[i]-=rig-1;
            ordinablu(blucar[i]);
               }
             }
        }
         
    }

   return Ttot;
}


//in ingresso differenza tra il numero di iterazioni prec e quelle successive e l'indice del vettore iterazione corrente.

void traffico::scambi(unsigned int o,unsigned int n){
   bool B=false;bool R=false;unsigned int cont=0;
   if(o==0||vec[o-1]%2==0){                                  //se non sono all'inizio del gioco e se le iterazioni che ho fatto al passo prima sono pari allora
      while(cont<n){                                //inizio con le blu finche non finisco il numeor di iterazioni da fare       
         B=blu();
         ++cont;
	 if(cont==n)break;                           
         R=red();
         ++cont;
         if(B==false && R==false){
            cout<<"la matrice si e bloccata allo scambio "<<cont<<endl;
            break;
            }
         }
      }
   else{                                             //altrimenti si inizia con le rosse e si cicla fino a quando le iterazioni non finiscono
      while(cont<n){
         R=red();
         ++cont;
         if(cont==n)break;
         B=blu();
         ++cont;
         if(B==false && R==false){
            cout<<"Matrix stopped at step "<<cont<<endl;
            break;
            }
         }
      }
   }


//stampa su file

void traffico::stampafile(unsigned int n){
stringstream c;
string s;
c<<vec[n];
c<<".csv";
s=c.str();
cout<<c.str();
ofstream outfile(s.c_str());
for(unsigned int h=0;h<rig*cols;++h){
   outfile<<v[h];
   if((h+1)%cols!=0){outfile<<',';}
   else if (h!=rig*cols-1){outfile<<endl;}
   }
outfile.close();
}

void traffico::fai(){
int pre=0;
    for(unsigned int c=0;c<vec.size();++c){
        scambi(c,vec[c]-pre);
        stampafile(c);cout<<endl;
        pre=vec[c];
  }
}

//la classe dimension gestisce il computo delle righe,delle colonne e 
//la gestione di eventuali errori di lunghezza errata delle righe
dimension::dimension(const char *filename_X){  
ifstream file_X;
file_X.open(filename_X);
    if(file_X.is_open()){
        string line;
        rig=0;
        cols=0;
        getline(file_X,line);
        getline(file_X, line);
        cols=(line.size()+1)/2;                     //calcolo numero di colonne
        unsigned int lunghezza=line.size();
        while(!file_X.eof()){                       //controllo lunghezza righe
            if(lunghezza!=line.size()){
                cout<<"righe di diversa lunghezza o vuote";
                exit(1);
                }
            ++rig;
            getline(file_X, line);
          
           }

       }
file_X.close();
}

//metodi che mi ritornano il numero di righe e colonne 

inline unsigned int dimension:: colonne(){
   return cols;
   }

inline unsigned int dimension:: righe(){
   return rig;
   }


int main(){
	std::cout<<"Starting BML"<<std::endl;
   dimension b("problem.csv");
   traffico a(b.righe(),b.colonne(),"problem.csv");
   a.fai();
   std::cout<<"Ending BML"<<std::endl;
   //a.stampa();
   return 0;
}
