//INF213: ATENCAO: NAO MODIFIQUE ESTE ARQUIVO!!

#ifndef PPMImage_H
#define PPMImage_H

#include <vector>
#include <string>
#include <iostream>

using namespace std;

struct Color {
	int r,g,b;
	Color(int r_, int g_,int b_) : r(r_),g(g_),b(b_) {}
	Color() {}
	bool operator==(const Color &c) const { return r==c.r && g==c.g && b==c.b;}
	bool operator!=(const Color &c) const { return !(*this==c); }
	string toStr() const { //gera uma string representando a cor
		return to_string(r)+","+to_string(g)+","+to_string(b); //o metodo to_string requer o uso de C++11
	}
};

class PPMImage {
public:
	PPMImage() {}
	PPMImage(int rows_, int columns_,int max_intensity)  {
		init(rows_,columns_,max_intensity);
	}  
	 
	void set(int y,int x, const Color &c) { //define a cor do pixel (y,x) como sendo c
		image[y][x] = c;
	}
	const Color &get(int y,int x) const { //retorna a cor associada a linha y e coluna x da imagem
		return image[y][x];
	}
	int getRows() const {return rows;}
	int getColumns() const {return columns;}
	int getMaxCol() const {return mxIntensity;}

	//a "subimagem" que esta na regiao definida pelos limitantes (yMin,xMin) e (yMin+size-1, xMin+size-1) e' 
	//"uniforme" ? 
	//Retorna true <-> se for uniforme
	//Se a regiao for uniforme, representingColor e' uma cor que "representa" essa regiao 
	//Essa funcao e' razoavel, mas certamente ha muitas funcoes que podem decidir melhor como comprimir uma quadtree
	//Por exemplo, a "media das cores" conforme esta sendo calculada nao faz muito sentido.
	//Obs: voce DEVE utilizar a funcao isUniform no trabalho pratico!!!! (exceto para a parte extra do trabalho)
	//Caso contrario, a saida do seu programa podera nao corresponder as saidas esperadas pelo Submitty
	bool isUniform(int yMin,int xMin, int size, Color &representingColor) const;

	friend istream& operator>>(istream  &in, PPMImage &img);
private:
	//inicializa o objeto, alocando memoria e inicializando o numero de linhas, colunas e intensidade maxima de cor
	void init(int rows_, int columns_,int max_intensity_);
	int rows, columns;
	vector<vector<Color> > image; //vectors sao similares a nossa classe MyVec (estao presentes na STL do C++)
	int mxIntensity;

};


//o operador >> pode ser utilizado para ler uma imagem PPM da entrada padrao
istream& operator >>(istream  &in, PPMImage &img);

//Grava a imagem (no formato PPM) na saida padrao
ostream& operator<<(ostream  &out, const PPMImage &img);

#endif