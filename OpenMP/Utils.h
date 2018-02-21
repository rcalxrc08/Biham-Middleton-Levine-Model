#ifndef BHL_OPENMP_UTILS_H
#define BHL_OPENMP_UTILS_H

#include <vector>
class traffico
{
private:
	char *v;
	std::vector<unsigned int>vec;
	std::vector<unsigned int>blucar;
	std::vector<unsigned int>redcar;
	std::vector<unsigned int>numcol;
	std::vector<unsigned int>numsurig;
	unsigned int rig;
	unsigned int cols;

	bool blu();
	bool red();
	void ordinared(unsigned int ri);
	void ordinablu(unsigned int ri);
	inline void scambi(unsigned int o,unsigned int n);
	void stampafile(unsigned int n);
public:
	traffico(unsigned int r,unsigned int c,const char * filename_X);
	inline void fai();
	~traffico();
	void stampa();
};

class dimension
{
private:
	unsigned int rig;
	unsigned int cols;
	double densit;

public:
	dimension(const char *filename_X);
	unsigned int colonne();
	unsigned int righe();
};


#endif //BHL_OPENMP_UTILS_H