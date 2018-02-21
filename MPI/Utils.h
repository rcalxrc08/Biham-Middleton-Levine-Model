#ifndef BHL_MPI_UTILS_H
#define BHL_MPI_UTILS_H

using namespace std;
class gioco
{
private:
	char *v;
	unsigned int rig;
	unsigned int cols;
	int vec[100];
public:
	gioco(unsigned int r,unsigned int c,const char * filename_X,int rank,int size);
	~gioco();
	void stampa();
	inline bool blu(int rank,int size);
	inline bool red();
	void scambi(unsigned int o,unsigned int n,int rank,int size);
	void stampafile(unsigned int n,int rank,int size);
	void fai(int rank,int size);
};


class density
{
private:
	unsigned int rig;
	unsigned int cols;
	double densit;

public:
	density(const char *filename_X);
	void stampa();
	unsigned int colonne();
	unsigned int righe();
	double densty();
};



class sparse
{
private:
	std::vector<unsigned int>blucar;
	std::vector<unsigned int>redcar;
	std::vector<unsigned int>vec;
	std::vector<unsigned int>numcol;
	std::vector<unsigned int>numsurig;
	unsigned int rig;
	unsigned int cols;
	unsigned int bsize;
	unsigned int rsize;
public:
	sparse(unsigned int r,unsigned int c,const char * filename_X);
	~sparse();

	void stampa();
	void blu();
	inline void ordinablu(unsigned int bi);
	void red();
	inline void ordinared(unsigned int ri);
	void scambi(unsigned int o,unsigned int n);
	void stampafile(unsigned int n);
	void fai();
};

#endif //BHL_MPI_UTILS_H