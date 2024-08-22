#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <vector>
#include <cassert>
#include "MyQuad.h"
#include "PPMImage.h"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Uso: " << argv[0] << " <comprimir|descomprimir>\n";
        return 1;
    }

    if (string(argv[1]) == "comprimir") {
        PPMImage img;
        cin >> img;
        MyQuad qt(img); // Use o construtor para compressão
        qt.comprimir(string(argv[1]));
        qt.imprimir_Comprimido(cout);

    }
    else if (string(argv[1]) == "descomprimir") {
        // Ler a string comprimida diretamente do input
        string comprimido;
        string linha;
        while(getline(cin, linha)){
            comprimido += linha;
        } // Lê a linha completa do input comprimido
        // Cria a MyQuad a partir da string comprimida
        MyQuad qt(comprimido); 

        // Descomprime a imagem
        qt.descomprimir(); 

        // Imprime a imagem descomprimida
        cout << qt.imagem; 
        
    }
    else if((string(argv[1]) == "extra")){
        PPMImage img;
        cin >> img;
        MyQuad qt(img); // Use o construtor para compressão
        qt.comprimir(string(argv[1]));
        qt.imprimir_Comprimido(cout);
    }
    
    
     else {
        cerr << "Argumento inválido!\n";
        return 1;
    }

    return 0;
}
