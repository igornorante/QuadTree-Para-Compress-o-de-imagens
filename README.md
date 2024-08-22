///////Eu li as regras///////////
Nome: Igor Cristian Acácio Josafá
Matricula: 105489

------------------------------------------------------------------
Fontes de consulta:
- https://demonstrations.wolfram.com/QuadtreeSubdivision/
- http://www.decom.ufop.br/guilherme/BCC203/geral/ed2-introducao-estruturas-dados-espaciais-victor.pdf
- https://www.youtube.com/watch?v=jxbDYxm-pXg
- https://www.youtube.com/watch?v=Rf3wl1UA9Uk
- https://www.geeksforgeeks.org/quad-tree/
- https://42bits.medium.com/analisando-algoritmos-quadtree-2fe86394f624
- http://www.inf.ufsc.br/~aldo.vw/visao/1998/harley/quadtree.htm

------------------------------------------------------------------

Funções e funcionalidades: 

O código MyQuad.h implementa uma estrutura de dados chamada Quadtree para compressão de imagens, juntamente com funções para manipular essa estrutura. 

-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CLASSE NOMYQUAD:

Construtor NoMyQuad(int x, int y, int tamanho, const Color &cor): Inicializa um novo nó da Quadtree com as coordenadas (x, y), tamanho, cor e define como uma folha (não possui filhos).

-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CLASSE MYQUAD:

Construtor MyQuad(PPMImage &img): Inicializa uma nova Quadtree a partir de uma imagem PPM.

Construtor MyQuad(const string &comprimido):  Constrói uma Quadtree a partir de uma string que representa a imagem comprimida.  Extrai as dimensões e a intensidade máxima da imagem da string e reconstrói a árvore a partir dos dados da string.

Destrutor ~MyQuad(): Libera a memória alocada para a árvore.

-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
PONTOS EXTRAS - FUNÇÃO EXTRA_UNIFORME: 

 - extra_uniforme(int y, int x, int tamanho, Color &representingColor) const: 

A função extra_uniforme (presente no arquivo "MyQuad.txt") é um critério adicional para determinar se uma região da imagem é uniforme, além da função isUniform fornecida. 
Ela calcula a média RGB da região e verifica se uma alta porcentagem dos pixels está próxima dessa média, considerando uma tolerância.
Cálculo da Média e Limites: A função itera sobre a região da imagem, calculando a média dos valores R, G e B dos pixels (avgR, avgG, avgB) e também os valores mínimos e máximos de cada componente de cor (minR, minG, minB, maxR, maxG, maxB).
Primeiro Critério de Uniformidade:  Se a diferença entre os valores máximos e mínimos de cada componente de cor for maior que 10, a região é considerada não uniforme, retornando false.
Segundo Critério de Uniformidade: A função conta quantos pixels estão próximos da média calculada, considerando uma tolerância de 10 para cada componente de cor. 
Se pelo menos 97% dos pixels estiverem dentro dessa tolerância, a região é considerada uniforme, retornando true. Caso contrário, retorna false.

Diferença entre extra_uniforme e isuniforme:
Ambas as funções são utilizadas para determinar se uma região da imagem é uniforme.
A principal diferença reside na maneira como cada função decide se uma região é uniforme e na tolerância utilizada.
isuniforme:  Calcula a média da região e verifica se pelo menos 97% dos pixels estão a uma distância máxima de 4% da intensidade máxima de cor da média.
extra_uniforme: Utiliza dois critérios: 
1) Se a diferença entre as cores máximas e mínimas na região for maior que 10, a região não é uniforme. 
2)  Verifica se pelo menos 97% dos pixels estão a uma distância máxima de 10 unidades de cada componente RGB da média.



-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

FUNÇÃO COMPRIMIR

comprimir():  É uma função recursiva para comprimir uma subregião da imagem. Verifica se a região é uniforme (todos os pixels com a mesma cor). 
Se for, cria um nó folha com a cor representativa. Caso contrário, cria um nó não folha e divide a região em 4 quadrantes, chamando recursivamente a função para cada quadrante.
Esta função atua como um ponto de partida para o processo de compressão.
Ela recebe implicitamente o objeto MyQuad ao qual pertence e opera na imagem armazenada em seu membro imagem.
Primeiramente, ela chama a função comprimir(int x, int y, int tamanho) recursivamente, passando as coordenadas iniciais (0, 0) e o tamanho da imagem como argumentos.
Isso inicia a divisão recursiva da imagem em sub-regiões.

comprimir(int x, int y, int tamanho): Esta função recursiva é a principal do algoritmo de compressão.
Ela recebe as coordenadas do canto superior esquerdo (x, y) e o tamanho da sub-região a ser comprimida.
Verificação de Uniformidade: Primeiramente, verifica se a sub-região da imagem é uniforme, ou seja, se todos os pixels dentro dela possuem a mesma cor.
Essa verificação é feita utilizando a função isUniform da classe PPMImage disponibilizada pelo professor.
Criação do Nó da Quadtree:
Nó Folha: Se a região for uniforme, a função cria um novo nó folha NoMyQuad representando essa região.
O nó armazena as coordenadas, o tamanho e a cor representativa da região.
Nó Não Folha (Divisão): Se a região não for uniforme, a função cria um nó não folha NoMyQuad.
Em seguida, divide a região em quatro quadrantes iguais. Para cada quadrante, a função chama recursivamente comprimir, passando as coordenadas e o tamanho apropriados. 
Esse processo continua até que todas as sub-regiões sejam uniformes ou atinjam o tamanho mínimo de pixel.

-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

FUNÇÃO DESCOMPRIMIR:

- descomprimir(): 

Assim como comprimir(), esta função serve como um ponto de partida para a descompressão.
Ela verifica se a raiz da Quadtree (raiz) não é nula. Se não for, ela chama a função recursiva descomprimir(NoMyQuad *no), passando a raiz como argumento.

- descomprimir(NoMyQuad *no): 

Esta função percorre a Quadtree e reconstrói a imagem descomprimida.
Processamento do Nó Folha: Se encontrar um nó folha, significa que ele representa uma região uniforme na imagem original. 
A função preenche a região correspondente na imagem imagem com a cor armazenada no nó folha.
Chamadas Recursivas: Se encontrar um nó não folha, a função chama recursivamente descomprimir para cada um dos seus quatro filhos, na ordem: Inferior Direito, Inferior Esquerdo, Superior Direito e Superior Esquerdo.
Essa ordem específica é crucial para reconstruir a imagem corretamente.

-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

FUNÇÃO IMPRIMIR_COMPRIMIDO:

- imprimir_Comprimido(ostream &out, bool raiz): 

Esta função é responsável por imprimir a representação textual da Quadtree comprimida.
O argumento out é um fluxo de saída (como cout ou um arquivo) para onde a representação será impressa. 
O argumento booleano raiz indica se a função está sendo chamada para a raiz da árvore.
Se chamada para a raiz (raiz é true), a função primeiro imprime as dimensões da imagem (largura e altura) e a intensidade máxima de cor.
Em seguida, chama recursivamente a função imprimir_Comprimido(ostream &out, NoMyQuad *no), passando o nó raiz como argumento.
Observação: A string de saída final conterá a representação da Quadtree, onde cada nó é delimitado por parênteses. 
Nós folha conterão a cor RGB, e nós não folha conterão a representação recursiva dos seus filhos.

- imprimir_Comprimido(ostream &out, NoMyQuad *no): 

Esta função recursiva percorre a Quadtree e imprime a representação textual de cada nó.
Nó Folha: Para um nó folha, a função imprime a cor armazenada no formato "(R,G,B)".
Nó Não Folha: Para um nó não folha, a função primeiro imprime um parêntese de abertura "(".
Em seguida, chama recursivamente imprimir_Comprimido para cada um dos seus quatro filhos, na ordem: Superior Direito, Superior Esquerdo, Inferior Esquerdo e Inferior Direito. 
Finalmente, imprime um parêntese de fechamento ")".

-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

FUNÇÃO LER_COMPRIMIDO: 


- ler_Comprimido(const string &comprimido, int &pos, int tamanho, int x, int y): 

A função ler_comprimido é responsável por reconstruir a estrutura da QuadTree a partir de uma string que representa a imagem comprimida.
O processo de reconstrução é realizado recursivamente, interpretando a string comprimida e criando os nós da árvore de acordo com a informação encontrada.
A função recebe a string comprimida, a posição atual na string (pos), o tamanho do bloco atual, e as coordenadas x e y do bloco na imagem.
A função verifica o caractere na posição atual da string. Se for um abre parênteses "(", indica o início de um novo nó.
Se o próximo caractere após o "(" for um dígito ou um sinal de menos "-", significa que o nó é uma folha. A função então lê os valores RGB da cor, 
cria um novo nó NoMyQuad com esses valores e retorna esse nó.
Se o próximo caractere não for um dígito ou "-", significa que o nó é interno e possui filhos. 
A função cria um novo nó NoMyQuad, define-o como não sendo folha (Folha = false), 
e recursivamente chama ler_comprimido para cada um dos seus 4 filhos, ajustando as coordenadas e tamanho do bloco para cada filho.
Retorno: A função retorna o nó NoMyQuad criado, que pode ser um nó folha (representando um bloco uniforme da imagem) ou um nó interno com seus respectivos filhos.

-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

FUNÇÕES AUXILIARES: 

- deletar_Arvore(NoMyQuad *no): Função recursiva para liberar a memória alocada para os nós da árvore.

- isUniform pertence à classe PPMImage é utilizada para determinar se uma região da imagem é uniforme, ou seja, se todos os pixels têm a mesma cor.

-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

Em resumo, o código define uma estrutura de Quadtree para representar imagens de forma hierárquica. As funções fornecidas permitem a construção da Quadtree a partir de uma imagem, a compressão e descompressão da imagem, a impressão da árvore e a liberação da memória utilizada

-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
COMPLEXIDADES E JUSTIFICATIVAS

1) Função comprimir 

Complexidade: O(1) melhor caso e O(N²) (onde N  é a resolução da imagem) no pior caso

Justificativa:

Melhor caso: 
Se a imagem for completamente uniforme, ou seja, todos os pixels tiverem a mesma cor, a Quadtree terá apenas um nó (a raiz). 
Nesse caso, a função comprimir, terá complexidade O(1), pois precisarão apenas verificar a raiz da árvore.

Pior caso: 
O pior caso ocorre quando a imagem exige a divisão máxima da Quadtree, resultando em uma árvore com altura máxima. Considerando N a resolução da imagem.  
Nesse cenário, a função, comprimir, visitaa todos os nós da árvore. Como o número de nós em uma árvore completa é  O(N²), a complexidade nesse caso é O(N²).


2) Função descomprimir

Complexidade: O(1) melhor caso e O(N²) (onde N  é a resolução da imagem) no pior caso

Justificativa:

Melhor caso: 
Se a imagem for completamente uniforme, ou seja, todos os pixels tiverem a mesma cor, a Quadtree terá apenas um nó (a raiz). 
Nesse caso, a função descomprimir, terá complexidade O(1), pois precisarão apenas verificar a raiz da árvore.

Pior caso: 
O pior caso ocorre quando a imagem exige a divisão máxima da Quadtree, resultando em uma árvore com altura máxima. Considerando N a resolução da imagem.  
Nesse cenário, a função, descomprimir, visitaa todos os nós da árvore. Como o número de nós em uma árvore completa é  O(N²), a complexidade nesse caso é O(N²).


3)Outras complexidades:

A função isUniform desempenha um papel crucial na complexidade. Logo, a eficiência dessa função influencia diretamente o tempo necessário para construir a Quadtree.
As funções imprimir_Comprimido e ler_Comprimido, responsáveis por gerar e interpretar a representação textual da árvore, 
também possuem complexidade O(N²) no pior caso, pois precisam percorrer todos os nós da árvore.

-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Experimentos e comentários:


| Tamanho | Descricao                            | Tempo para comprimir (s) | Tempo para descomprimir (s)  | Tempo para comprimir (EXTRA) (s)  | 
|  32X32  | Pixels.ppm                           |                    0,021 |                        0,008 |                             0,015 |                                                       
| 512x512 | Brasao.ppm                           |                    1,500 |                        1,581 |                             0,258 |                                    
|1024X1024| Capivara.ppm                         |                    9,228 |                       30,427 |                             1,386 |                                    
|1000X1000| Jordan.ppm                           |                   16,537 |                       28,000 |                             1,569 |

Comentários: 
Os tempos de execução para comprimir e descomprimir imagens variam de acordo com o tamanho e a complexidade da imagem.
Imagens com regiões uniformes são comprimidas mais rapidamente, pois a Quadtree pode ser construída com menos divisões.
Imagens com muitas variações de cor e detalhes exigem mais divisões e, portanto, mais tempo para comprimir e descomprimir.
A função extra_uniforme pode acelerar o processo de compressão em imagens com regiões uniformes, reduzindo o número de divisões necessárias.

-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


