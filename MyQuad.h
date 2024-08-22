#ifndef MYQUAD_H
#define MYQUAD_H

#include "PPMImage.h"
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <vector>

using namespace std;

// Estrutura do nó da MyQuad
class NoMyQuad {
public:
    NoMyQuad(int x, int y, int tamanho, const Color &cor)
        : x(x), y(y), tamanho(tamanho), cor(cor), Folha(true) { // Inicializa como folha
        for (int i = 0; i < 4; ++i) {
            filhos[i] = nullptr; // Inicializa os filhos como nullptr
        }
    }

    NoMyQuad *filhos[4]; // Vetor de ponteiros para os filhos
    int x, y, tamanho; // Coordenadas e tamanho do nó
    Color cor; // Cor do nó
    bool Folha; // Indica se o nó é uma folha
};

// Classe MyQuad para compressão e descompressão
class MyQuad {
public:

    PPMImage imagem; // Imagem a ser comprimida
    MyQuad(PPMImage &img); // Construtor que inicializa a raiz como nullptr
    MyQuad(const string &comprimido); // Construtor que cria uma MyQuad a partir de uma string comprimida
    MyQuad(const MyQuad &other); // Construtor de cópia
    ~MyQuad(); // Destrutor
    MyQuad & operator=(const MyQuad &other);

    void comprimir(string argumento); 
    void descomprimir();
    void imprimir_Comprimido(ostream &cout, bool raiz = true);
    bool extra_uniforme(int y, int x, int tamanho, Color &representingColor) const;

private:
    NoMyQuad *raiz; // Raiz da MyQuad
    NoMyQuad *copiar_Arvore(const NoMyQuad *no);
    NoMyQuad *comprimir(int x, int y, int tamanho, string argumento);
    void descomprimir(NoMyQuad *no);
    void deletar_Arvore(NoMyQuad *no);
    void imprimir_Comprimido(ostream &out, NoMyQuad *no);
    NoMyQuad *ler_Comprimido(const string &comprimido, int &pos, int tamanho, int x, int y);
};

// Construtor que inicializa a raiz como nullptr
MyQuad::MyQuad(PPMImage &img) : imagem(img), raiz(nullptr) {}

// Construtor que cria uma MyQuad a partir de uma string comprimida
MyQuad::MyQuad(const string &comprimido) : raiz(nullptr) {
    int pos = 0;
    int rows, maxIntensity;
    istringstream iss(comprimido); // Cria um stream a partir da string comprimida
    iss >> rows >> maxIntensity; // Lê as dimensões e a intensidade máxima
    while (comprimido[pos] != '(') pos++; // Encontra o início da árvore (primeiro parêntese)
    // Inicializa a imagem corretamente com as dimensões e intensidade máxima
    imagem = PPMImage(rows, rows, maxIntensity); // A imagem é quadrada
    raiz = ler_Comprimido(comprimido, pos, rows, 0, 0); // Lê a árvore a partir da string comprimida
}

// Construtor de cópia
MyQuad::MyQuad(const MyQuad &other) : imagem(other.imagem), raiz(nullptr) {
    if (other.raiz) { // Se a árvore não for nula, copiar a árvore
        raiz = copiar_Arvore(other.raiz); // Copia a árvore
    }
}


// Operador de atribuição
MyQuad& MyQuad::operator=(const MyQuad &other) {
    if (this != &other) { // Verifica se não é a mesma instância
        // Primeiro, liberar a memória da árvore existente
        deletar_Arvore(raiz);

        // Copiar a imagem
        imagem = other.imagem;

        // Copiar a árvore
        if (other.raiz) {
            raiz = copiar_Arvore(other.raiz);
        } else {
            raiz = nullptr;
        }
    }
    return *this;
}


// Função auxiliar para copiar a árvore
NoMyQuad* MyQuad::copiar_Arvore(const NoMyQuad *no) {
    if (!no) return nullptr; // Se o nó for nulo, retorna nullptr

    NoMyQuad *novoNo = new NoMyQuad(no->x, no->y, no->tamanho, no->cor); // Cria um novo nó com as mesmas informações
    novoNo->Folha = no->Folha; // Copia a informação se é folha ou não

    if (!no->Folha) { // Se não for folha, copiar os filhos
        for (int i = 0; i < 4; ++i) { 
            novoNo->filhos[i] = copiar_Arvore(no->filhos[i]); // Copia recursivamente os filhos
        }
    }

    return novoNo;
}

// Destrutor que deleta a árvore
MyQuad::~MyQuad() { deletar_Arvore(raiz); }

// Função para comprimir a imagem e preencher a MyQuad
// Inicializa a compressão com a raiz   
void MyQuad::comprimir(string argumento) { raiz = comprimir(0, 0, imagem.getRows(), argumento); } // Inicializa a raiz com a compressão

NoMyQuad *MyQuad::comprimir(int x, int y, int tamanho, string argumento) {
    Color corRepresentativa;

    // Ajuste para acessar a imagem de forma invertido

    if(argumento == "extra"){

        if (extra_uniforme(imagem.getRows() - y - tamanho, x, tamanho, corRepresentativa)) {
            return new NoMyQuad(x, y, tamanho, corRepresentativa); // Cria um nó folha com a cor representativa se a regiao for uniforme
        } else {
            NoMyQuad *no = new NoMyQuad(x, y, tamanho, Color(0, 0, 0)); // Inicializa com uma cor padrão
            no->Folha = false;
            int metade = tamanho / 2;

            // Ordem correta dos quadrantes na compressão
            no->filhos[0] = comprimir(x, y, metade, argumento);          // Superior esquerdo
            no->filhos[1] = comprimir(x + metade, y, metade, argumento); // Superior direito
            no->filhos[2] = comprimir(x, y + metade, metade, argumento); // Inferior esquerdo
            no->filhos[3] = comprimir(x + metade, y + metade, metade, argumento); // Inferior direito

            return no;
        }
    }

    else{
                            // Ajuste para acessar a imagem de forma invertida                             
        if (imagem.isUniform(imagem.getRows() - y - tamanho, x, tamanho, corRepresentativa)) {
            return new NoMyQuad(x, y, tamanho, corRepresentativa); // Se a região for uniforme, retorna um nó folha
        } else {
            NoMyQuad *no = new NoMyQuad(x, y, tamanho, Color(0, 0, 0)); // Inicializa com uma cor padrão
            no->Folha = false;
            int metade = tamanho / 2;

            // Ordem correta dos quadrantes na compressão
            no->filhos[0] = comprimir(x, y, metade, argumento);            // Superior esquerdo
            no->filhos[1] = comprimir(x + metade, y, metade, argumento);   // Superior direito
            no->filhos[2] = comprimir(x, y + metade, metade, argumento);   // Inferior esquerdo
            no->filhos[3] = comprimir(x + metade, y + metade, metade, argumento); // Inferior direito

            return no;
    }
}
}

// Função para descomprimir a MyQuad e preencher a imagem
void MyQuad::descomprimir() {
    if (raiz) { // Se a raiz não for nula, descomprimir a árvore
        descomprimir(raiz); // Descomprime a árvore
    }
}


void MyQuad::descomprimir(NoMyQuad *no) {
    if (!no) return; // Se o nó for nulo, retorna

    if (no->Folha) {
        for (int i = no->x; i < no->x + no->tamanho; i++) { 
            for (int j = no->y; j < no->y + no->tamanho; j++) { // Percorre a região do nó
                // Define a cor do pixel e ajusta a imagem de forma invertida 
                imagem.set(imagem.getRows() - j - 1, imagem.getRows() - i - 1 , no->cor); 
                
            }
         }
        } else {
        descomprimir(no->filhos[3]); // Superior esquerdo
        descomprimir(no->filhos[1]); // Superior direito
        descomprimir(no->filhos[0]); // Inferior esquerdo
        descomprimir(no->filhos[2]); // Inferior direito
       
    }
   
}

// Função que deleta a árvore recursivamente
void MyQuad::deletar_Arvore(NoMyQuad *no) {
    if (no) {
        for (int i = 0; i < 4; i++) {
            deletar_Arvore(no->filhos[i]);
        }
        delete no;
    }
}

// Função para imprimir a representação comprimida da imagem
void MyQuad::imprimir_Comprimido(ostream &cout, bool raiz) {
    if (raiz) {
        cout << imagem.getColumns() << " " << imagem.getMaxCol() << "\n"; // Imprime as dimensões e a intensidade máxima
    }
    imprimir_Comprimido(cout, this->raiz); // Imprime a árvore comprimida
}

// Função recursiva que imprime a MyQuad comprimida
void MyQuad::imprimir_Comprimido(ostream &out, NoMyQuad *no) {
    if (no->Folha) {
        cout << "(" << no->cor.toStr() << ")"; // Imprime a cor do nó
    } else {
        cout << "("; 
        // Ordem correta dos quadrantes na impressão
        imprimir_Comprimido(cout, no->filhos[1]); // Superior esquerdo
        imprimir_Comprimido(cout, no->filhos[0]); // Superior direito
        imprimir_Comprimido(cout, no->filhos[2]); // Inferior esquerdo
        imprimir_Comprimido(cout, no->filhos[3]); // Inferior direito
        cout << ")";
    }
}


// Função para ler o conteúdo comprimido e reconstruir a MyQuad
NoMyQuad *MyQuad::ler_Comprimido(const string &comprimido, int &pos, int tamanho, int x, int y) {
    if (pos >= comprimido.size()) return nullptr; // Se a posição for maior que o tamanho da string, retorna nullptr

    if (comprimido[pos] == '(') { // Se for um nó
        ++pos; // Avança após o parêntese de abertura

        if (isdigit(comprimido[pos]) || comprimido[pos] == '-') { // Se for um nó folha
            int r = 0, g = 0, b = 0; // Inicializa as cores
            

            while (isdigit(comprimido[pos])) { // Enquanto for um dígito
                r = r * 10 + (comprimido[pos] - '0'); // Atualiza o valor de r
                ++pos; // Avança
            }
            ++pos;  // Avança após a vírgula

            while (isdigit(comprimido[pos])) { // Enquanto for um dígito
                g = g * 10 + (comprimido[pos] - '0'); // Atualiza o valor de g
                ++pos; // Avança
            }
            ++pos;  // Avança após a vírgula

            while (isdigit(comprimido[pos])) { // Enquanto for um dígito
                b = b * 10 + (comprimido[pos] - '0'); // Atualiza o valor de b
                ++pos; // Avança
            }

            ++pos;  // Avança após o parêntese de fechamento

            return new NoMyQuad(x, y, tamanho, Color(r, g, b)); // Retorna um nó folha com a cor lida
        } else {
            NoMyQuad *node = new NoMyQuad(x, y, tamanho, Color(0, 0, 0)); // Inicializa com uma cor padrão
            node->Folha = false; 
            int metade = tamanho / 2;

            // Ordem correta dos quadrantes na leitura da quadtree
            node->filhos[3] = ler_Comprimido(comprimido, pos, metade, x, y);               // Superior esquerdo
            node->filhos[1] = ler_Comprimido(comprimido, pos, metade, x + metade, y);      // Superior direito
            node->filhos[0] = ler_Comprimido(comprimido, pos, metade, x + metade, y + metade); // Inferior esquerdo
            node->filhos[2] = ler_Comprimido(comprimido, pos, metade, x, y + metade);      // Inferior direito

            ++pos;  // Avança o parêntese de fechamento
            return node;
        }
    }
    return nullptr;
}

bool MyQuad::extra_uniforme(int y, int x, int tamanho, Color &representingColor) const {
    // Inicializar as variáveis para cálculo da média e dos limites
    double avgR = 0, avgG = 0, avgB = 0; // Médias
    int minR = 255, minG = 255, minB = 255; // Mínimos
    int maxR = 0, maxG = 0, maxB = 0; // Máximos

    int total_pixels = tamanho * tamanho; // Total de pixels

    // Primeira passagem: calcular somas e determinar mínimo e máximo
    for (int i = y; i < y + tamanho; i++) { 
        for (int j = x; j < x + tamanho; j++) {
            const Color &pixel = imagem.get(i, j); // Obtém o pixel

            // Atualizar as somas
            avgR += pixel.r; // Soma dos valores de R
            avgG += pixel.g; // Soma dos valores de G
            avgB += pixel.b; // Soma dos valores de B

            // Atualizar os valores mínimo e máximo
            minR = min(minR, pixel.r); // Mínimo de R
            minG = min(minG, pixel.g); // Mínimo de G
            minB = min(minB, pixel.b); // Mínimo de B

            maxR = max(maxR, pixel.r); // Máximo de R
            maxG = max(maxG, pixel.g); // Máximo de G
            maxB = max(maxB, pixel.b); // Máximo de B
        }
    }

    // Calcular a média
    avgR /= total_pixels; // Média de R
    avgG /= total_pixels; // Média de G
    avgB /= total_pixels; // Média de B

    // Atualizar a cor representativa
    representingColor.r = static_cast<int>(avgR); // Atualiza a cor representativa de R
    representingColor.g = static_cast<int>(avgG); // Atualiza a cor representativa de G
    representingColor.b = static_cast<int>(avgB); // Atualiza a cor representativa de B

    // Critério de uniformidade
    if ((maxR - minR) > 10 || (maxG - minG) > 10 || (maxB - minB) > 10) { // Se a diferença entre os valores for maior que 10
        return false;
    }

    // Segunda passagem: contar pixels próximos à média
    int countCloseToAvg = 0; // Contador de pixels próximos à média
    for (int i = y; i < y + tamanho; i++) {
        for (int j = x; j < x + tamanho; j++) {
            const Color &pixel = imagem.get(i, j);
            int diffR = abs(pixel.r - representingColor.r); // Diferença de R
            int diffG = abs(pixel.g - representingColor.g); // Diferença de G
            int diffB = abs(pixel.b - representingColor.b); // Diferença de B

            if (diffR <= 10 && diffG <= 10 && diffB <= 10) { // Se a diferença for menor ou igual a 10
                countCloseToAvg++; // Incrementa o contador
            }
        }
    }

    // Verifica se 97% ou mais dos pixels estão próximos da média
    return (countCloseToAvg >= total_pixels * 0.97); 
}


#endif
