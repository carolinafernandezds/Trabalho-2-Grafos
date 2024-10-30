#ifndef GRAFOS_H
#define GRAFOS_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <queue>
#include <stack>
#include <cmath>
#include <algorithm>
#include <sys/resource.h>
#include <unordered_map>
#include <ctime>
#include <cstdlib>
#include <random>
using namespace std;

extern int n, numero = 0, opcao, verifica = 0, final; 
extern double infinito = numeric_limits<double>::infinity();
extern vector <int> grau, caminho, pai, niveis, vertices_componente, componente_auxiliar1, componente_auxiliar2, distancia_auxiliar;
extern vector <vector <double> > matriz_peso;
extern vector <double> distancia_dijkstra;
extern int ** matriz;
extern vector <vector <int> > lista, componentes;
extern vector <vector <vector <double> > > lista_peso; 

// Função para verificar a utilização de memória
void memoria_utilizada();

// Função para inicializar a matriz de adjacência
void constroi_matriz(string nome_arquivo);

// Função para inicializar a lista de adjacência
void constroi_lista(string nome_arquivo);

// Função para inicializar o grafo (0 para matriz e 1 para lista)
void constroi_grafo(int opcao1, string nome_arquivo);

// Função para executar o BFS a partir de um vértice
void BFS(int vertice);

// Função para executar o DFS a partir de um vértice
void DFS(int vertice);

// Função para gerar um arquivo de saída contendo a árvore gerada (pai e nível de cada vértice)
void constroi_arvore(string arquivo_saida);

// Função auxiliar para encontrar componentes conexas
void componentes_conexas_auxiliar();

// Função para determinar as componentes conexas do grafo
void componentes_conexas();

// Função para determinar a distância entre dois vértices
void distancia(int inicial, int final);

// Função para calcular uma aproximação do diâmetro do grafo
void diametro_aproximado();

//Função que retorna o diâmetro exato do grafo; ineficiente para um grande número de vértices.
void diametro();

// Função para gerar um arquivo de saída com informações sobre o grau do grafo e componentes conexas
void saida(string arquivo_saida);

//Função para inicializar a estrutura da matriz de adjacência (utiliza vector de vector de double, visto que, para os vértices relacionados, é preenchida pelo peso da aresta; para os não relacionados, é preenchida por infinito).
void constroi_matriz_peso(string nome_arquivo);

//Função para inicializar a estrutura da lista de adjacência (utliza vector de vector de vector de double, visto que o vetor maior armazena n vetores; cada um armazena os vizinhos na forma de vetores, onde cada vetor armazena o vértice relacionado e o peso da aresta).
void constroi_lista_peso(string nome_arquivo);

//Função que inicializa o grafo com peso; recebe como parâmetros 0 (escolhe como representação a matriz) ou 1 (escolhe como representação a lista) e o nome do arquivo .txt a ser lido.
void constroi_grafo_peso(int opcao1, string nome_arquivo);

//Função que fornece as distâncias de um vértice inicial s para todos os demais vértices do grafo, utilizando na implementação do algoritmo de Dijkstra um vetor auxiliar.
void distancia_peso(int s);

//Função que fornece as distâncias de um vértice inicial s para todos os demais vértices do grafo, utilizando na implementação do algoritmo de Dijkstra um heap mínimo (MinHeap).
void distancia_peso_heap(int s);

//Função que retorna o caminho mínimo entre o vértice inicial s e o vértice final f, após a execução das funções distancia_peso(s) ou distancia_peso_heap(s).
void caminho_minimo(int s, int f);

#endif 
