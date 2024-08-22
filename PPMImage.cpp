//INF213: ATENCAO: NAO MODIFIQUE ESTE ARQUIVO!!


#include "PPMImage.h"

#include <cassert>
#include <cstdlib>

//o operador >> pode ser utilizado para ler uma imagem PPM da entrada padrao
istream& operator >>(istream  &in, PPMImage &img) {
	string st;
	in >> st;
	assert(st=="P3");
	in >> img.columns >> img.rows >> img.mxIntensity;
	img.init(img.rows,img.columns,img.mxIntensity);
	for(int i=0;i<img.rows;i++)
		for(int j=0;j<img.columns;j++) { //supomos que as imagens estao de cabeca para baixo (o pixel 0,0 e' o do canto inferior esquerdo, nao o do superior esquerdo)
			in >> img.image[img.rows-1-i][j].r >> img.image[img.rows-1-i][j].g >> img.image[img.rows-1-i][j].b;
		}
	return in;
}

//Grava a imagem (no formato PPM) na saida padrao
ostream& operator<<(ostream  &out, const PPMImage &img) {
	out << "P3\n";
	out << img.getColumns() << " " << img.getRows() << "\n";
	out << img.getMaxCol() << "\n";

	for(int i=0;i<img.getRows();i++) {
		for(int j=0;j<img.getColumns();j++) { //supomos que as imagens estao de cabeca para baixo (o pixel 0,0 e' o do canto inferior esquerdo, nao o do superior esquerdo)
			out << img.get(img.getRows()-1-i,j).r << " " << img.get(img.getRows()-1-i,j).g << " " << img.get(img.getRows()-1-i,j).b << "  ";
		}
		out << "\n";
	}
	return out;
}




#include <cmath>
#include <map>

void PPMImage::init(int rows_, int columns_,int max_intensity_){
		rows = rows_;
		columns = columns_;
		mxIntensity = max_intensity_;
		image = vector<vector<Color> >(rows,vector<Color>(columns));
}

bool PPMImage::isUniform(int yMin,int xMin, int size, Color &representingColor) const {
	//Observacao: esta funcao deveria ter mais comentarios!!
	representingColor = get(yMin,xMin);
	if(size==1) return true; //regiao com 1 pixel certamente e' uniforme

	double avg[3] = {0};
	int mi[3] = {255};
	int ma[3] = {0};

	map<array<int,3> ,int> s;
	for(int i=yMin;i<yMin+size;i++) //mi e max vao representar 
		for(int j=xMin;j<xMin+size;j++) {
			avg[0] += get(i,j).r;
			avg[1] += get(i,j).g;
			avg[2] += get(i,j).b;

			s[{get(i,j).r/10,get(i,j).g/10,get(i,j).b/10}]++; //mapeia as cores para um numero menor de cores distintas
			
			mi[0] = min(mi[0],get(i,j).r);
			mi[1] = min(mi[1],get(i,j).g);
			mi[2] = min(mi[2],get(i,j).b);
			ma[0] = max(ma[0],get(i,j).r);
			ma[1] = max(ma[1],get(i,j).g);
			ma[2] = max(ma[2],get(i,j).b);
		}

	for(int i=0;i<3;i++) {
		avg[i] /= (size*size);
		//cerr << avg[i] << endl;
	}
	representingColor.r = avg[0];
	representingColor.g = avg[1];
	representingColor.b = avg[2];

	int countCloseToAvg = 0;
	for(int i=yMin;i<yMin+size;i++)
		for(int j=xMin;j<xMin+size;j++) {
			const Color &pixel = get(i,j);
			int mxDist = max( max(pixel.r-representingColor.r, pixel.g-representingColor.g),pixel.b-representingColor.b );
			if(mxDist<255*0.04) countCloseToAvg++;
		}
	return (countCloseToAvg >= size*size*0.97); //pelo menos 97% dos pixels estao "perto da media"
}

