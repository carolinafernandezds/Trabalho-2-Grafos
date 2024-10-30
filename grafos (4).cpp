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

int n, numero = 0, opcao, verifica = 0, final; 
double infinito = numeric_limits<double>::infinity();
vector <int> grau, caminho, pai, niveis, vertices_componente, componente_auxiliar1, componente_auxiliar2, distancia_auxiliar;
vector <vector <double> > matriz_peso;
vector <double> distancia_dijkstra;
int ** matriz;
vector <vector <int> > lista, componentes;
vector <vector <vector <double> > > lista_peso; 

//Função para verificar utilizição de memória.
void memoria_utilizada() {
    struct rusage r_usage;
    getrusage(RUSAGE_SELF, &r_usage);
    cout << "Memória utilizada: " << r_usage.ru_maxrss << " KB" << endl;
}

//Função para inicializar a estrutura de matriz de adjacência (utiliza array de array, cada um com tamanho n indicado por ponteiro).
void constroi_matriz (string nome_arquivo){
    ifstream arquivoEntrada(nome_arquivo);
    string linha;
    getline(arquivoEntrada, linha);
    n = stoi(linha); //Determina o total de vértices

    grau.resize(n, 0); //Preenche o vector grau, inicialmente, com n zeros.

        int rows = n, cols = n;
        matriz = new int*[rows];
        for (int i = 0; i < rows; i += 1){  //Cria a estrutura de matriz, utilizando ponteiros.
            matriz[i] = new int[cols];
        }   
        for (int i1 = 0; i1 < n; i1 += 1){     //Preenche a matriz com zeros.
            for (int i2 = 0; i2 < n; i2 += 1){
                matriz[i1][i2] = 0;
            }
        }

        while (getline (arquivoEntrada, linha)){ //Lê as arestas do grafo, a partir da linha 2.
            numero += 1;
            istringstream ss(linha);
            int n1, n2;
            ss >> n1 >> n2; 
            grau[n1 - 1] += 1;  //Para os vértices da aresta, aumenta seu grau em 1.
            grau[n2 - 1] += 1;
            matriz[n1 - 1][n2 - 1] = 1; //Preenche com 1 vértices que compõe a aresta.
            matriz[n2 - 1][n1 - 1] = 1;
        }
    arquivoEntrada.close();    
    memoria_utilizada();    //Registra o pico de memória utilizada.
}

//Função para inicializar a estrutura de lista de adjacência (utiliza vector de vector de inteiros, o vector de i-ésima posição representa os vizinhos do vértice i + 1); recebe como parâmetro o nome do arquivo .txt a ser lido.
void constroi_lista(string nome_arquivo) {
    ifstream arquivoEntrada(nome_arquivo);
    string linha;
    getline(arquivoEntrada, linha);
    n = stoi(linha);  // Lê o número de vértices (n) do arquivo.

    // Inicializa o vetor grau com zeros.
    grau.resize(n, 0);

    // Inicializa a lista de adjacência (um vetor de vetores).
    vector<vector<int> > vertices(n);  // Cada vértice terá uma lista de vizinhos.

    while (getline(arquivoEntrada, linha)) {
        numero += 1;
        istringstream ss(linha);
        int n1, n2;

        // Lê os dois vértices da aresta.
        ss >> n1 >> n2;

        // Incrementa o grau de cada vértice.
        grau[n1 - 1] += 1;
        grau[n2 - 1] += 1;

        // Adiciona a aresta na lista de adjacência (grafo não direcionado).
        vertices[n1 - 1].push_back(n2);  // Adiciona n2 como vizinho de n1.
        vertices[n2 - 1].push_back(n1);  // Adiciona n1 como vizinho de n2.
        sort(vertices[n1 - 1].begin(), vertices[n1 - 1].end());    //Ordena os vizinhos dos vértices.
        sort(vertices[n2 - 1].begin(), vertices[n2 - 1].end());

    }
    lista = vertices;

    arquivoEntrada.close();
}

//Função que inicializa o grafo; recebe como parâmetros 0 (escolhe como representação a matriz) ou 1 (escolhe como representação a lista) e o nome do arquivo .txt a ser lido.
void constroi_grafo(int opcao1, string nome_arquivo){
    opcao = opcao1;

    // Representa o grafo conforme a escolha do usuário.
    if (opcao == 0){                
        constroi_matriz(nome_arquivo); 
    }
    else{
        constroi_lista(nome_arquivo);
    }
}

//Função que executa o BFS no vértice indicado, recebendo-o como parâmetro; armazena os pais de cada vértice i no vector pai e os níveis no vector niveis, nas posições i - 1. 
void BFS(int vertice){
    vector <int> marcados (n,0);    // Para marcar vértices visitados.
    int nivel;                     
    queue <int> fila;   //Fila para a BFS.
    pai.resize(n, 0);
    niveis.resize(n, -1);
    vertices_componente.resize(0);      

    marcados[vertice - 1] = 1; //Marca vértice inicial.
    fila.push(vertice);
    fila.push(0);
    niveis[vertice - 1] = 0;

    while (fila.size() != 0){
        vertice = fila.front();
        fila.pop(); //Pega o mais antigo da fila.
        vertices_componente.push_back(vertice); //Adiciona vértices para a componente conexa.
        nivel = fila.front();
        fila.pop();

        if (opcao == 0){    //Matriz
            for (int i = 0; i < n; i ++){
                if ((matriz[vertice - 1][i] == 1) && marcados[i] == 0){
                    marcados[i] = 1;
                    fila.push(i + 1);   //Adiciona na fila.
                    pai[i] = vertice;   //Atualiza pai.
                    niveis[i] = nivel + 1;  //Atualiza nível.
                    fila.push(nivel + 1);
                }
            }
        }
        else{   //Lista
            for (int i = 0; i < lista[vertice - 1].size(); i ++){
                if (marcados[lista[vertice - 1][i] - 1] == 0){
                    marcados[lista[vertice - 1][i] - 1] = 1;
                    fila.push(lista[vertice - 1][i]);   //Adiciona na fila.
                    pai[lista[vertice - 1][i] - 1] = vertice;   //Atualiza pai.
                    niveis[lista[vertice - 1][i] - 1] = nivel + 1;  //Atualiza nível.
                    fila.push(nivel + 1);
                }
            }
        }
    }
    componente_auxiliar1 = marcados;
}

//Função que executa o DFS no vértice indicado, recebendo-o como parâmetro; armazena os pais de cada vértice i no vector pai e os níveis no vector niveis, nas posições i - 1.
void DFS(int vertice) {
    vector<int> marcados(n, 0); // Para marcar vértices visitados.
    stack<int> pilha; // Pilha para a DFS.
    pai.resize(n, 0);
    niveis.resize(n, 0);

    // Marque o vértice inicial como visitado e empilhe.
    marcados[vertice - 1] = 1; 
    pilha.push(vertice);

    while (!pilha.empty()) {
        
        vertice = pilha.top();  // Obtenha o vértice do topo da pilha.
        pilha.pop();
        int nivel = niveis[vertice - 1]; // O nível é armazenado no vetor niveis.

        for (int i = 0; i < (opcao == 0 ? n : lista[vertice - 1].size()); i++) {    
            int adj = (opcao == 0) ? i : lista[vertice - 1][i] - 1;
            if (opcao == 0 && matriz[vertice - 1][adj] == 1) {  //Matriz
             
                if (marcados[adj] == 0) {       
                    marcados[adj] = 1; 
                    niveis[adj] = nivel + 1; // Atualiza o nível.
                    pai[adj] = vertice; // Atualiza o pai.
                    pilha.push(adj + 1); // Empilha o adjacente.
                }
            } else if (opcao == 1) {      //Lista
                
                if (marcados[adj] == 0) {
                    marcados[adj] = 1; 
                    niveis[adj] = nivel + 1; // Atualiza o nível.
                    pai[adj] = vertice; // Atualiza o pai.
                    pilha.push(lista[vertice - 1][i]); // Empilha o adjacente.
                }
            }
        }
    }
}

//Função para gerar um arquivo .txt contendo o pai e o nível de um vértice i na árvore de largura gerada; recebe como parâmetro o nome do arquivo a ser criado.
void constroi_arvore(string arquivo_saida){
    ofstream arquivo_Saida(arquivo_saida);

    arquivo_Saida << "Árvore gerada: " << endl;

    for (int i = 0; i < n; i++){
        arquivo_Saida << "Vértice: " << i + 1 <<  " Pai: " << pai[i] << " Nível: " << niveis[i] << endl; //Percorre os vectors pai e niveis.
    }
    arquivo_Saida.close();
}

//Função auxiliar para encontrar componentes conexas;
void componentes_conexas_auxiliar(){
    for (int i = 0; i < n; i++){
        if (componente_auxiliar1[i] == 1){ 
            componente_auxiliar2[i] = 1;
        }
    }
}

//Função para determinar as componentes conexas de um grafo (já inicializado), armazenando-as no vector componentes.
void componentes_conexas(){
    componentes.resize(0);

    BFS(1);  //Executa BFS no vértice 1 como ponto de partida.
    componentes.push_back(vertices_componente); //Registra a primeira componente.

    componente_auxiliar2 = componente_auxiliar1;

    for (int j = 0; j < n; j ++){   //Verifica se ainda há vértice não marcado.
        if (componente_auxiliar2[j] == 0){
            BFS(j + 1);     //Caso sim, executa BFS nesse vértice.
            componentes.push_back(vertices_componente); //Registra nova componente.
            componentes_conexas_auxiliar();
        }
    }
    cout << componentes.size(); //Total de componentes.
}

//Função que determina a distância entre dois vértices, inicial e final, recebendo-os como parâmetros; 
void distancia(int inicial, int final){
    BFS(inicial);   //Executa BFS no vértice inicial, para achar componente conexa.
    int pai_atual = final, distancia_atual = 0;

    while (pai_atual != 0){     //Executa enquanto não achar um pai = 0 (neste caso, ou não pertence a componente, ou é raiz, portanto, vértice inicial).
        final = pai_atual;
        pai_atual = pai[final - 1];
        distancia_atual += 1;   //Atualiza o valor da distância.
    }
    if (niveis[final - 1] == 0){    //Se nível = 0, é raiz da árvore;
        cout << "Distância: " << distancia_atual << endl;
    }
    else{                           //Caso contrário, não faz parte da árvore, portanto não tem conexão com o vértice inicial.
        cout << "Não há ligação entre os vértices." << endl;
    }
}

//Função que retorna uma aproximação do diâmetro para grafos com grande número de vértices;
void diametro_aproximado(){
    int maior = 0, idx;
    componentes_conexas();

    for (int i = 0; i < componentes.size(); i++){   //Acha a maior das componentes conexas.
        if (componentes[i].size() > maior){
            maior = componentes[i].size();
            idx = i;
        }
    }

    maior = 0;
    BFS(componentes[idx][0]);   //Executa BFS no primeiro vértice encontrado dessa componente.

    for (int i = 0; i < n; i++){    //Encontra o vértice com maior distância do incial.
        if (niveis[i] > maior){
            maior = niveis[i];
            idx = i + 1;
        }
    }

    maior = 0;
    BFS(idx);   //Executa BFS no vértice encontrado.

    for (int i = 0; i < n; i++){    //Encontra o vértice com maior distância.
        if (niveis[i] > maior){
            maior = niveis[i];
        }
    }
    cout << "Diâmetro aproximado: " << maior << endl;
}

//Função que retorna o diâmetro exato do grafo; ineficiente para um grande número de vértices.
void diametro(){
    int maior = 0, maior_atual;
    for (int i = 1; i < n + 1 ; i ++){
        BFS(i);
        maior_atual = *max_element(niveis.begin(), niveis.end());
        if (maior_atual > maior){
            maior = maior_atual;
        }
    }   
    cout << "Diâmetro: " << maior;
}

//Função que gera um arquivo .txt contendo o menor, maior e médio grau de um grafo, bem como a mediana de todos os graus, armazenados no vector grau; o arquivo também contém todas as componentes conexas do grafo, listando-as em ordem decrescente de tamanho; recebe como parâmetro o nome do arquivo a ser criado.
void saida(string arquivo_saida){
    vector <int> copia = grau;
    bool acabou = false;
    sort(copia.begin(), copia.end());   //Ordena o vector grau de forma crescente.
    int menor = copia[0];   //Menor grau.
    int maior = copia[n - 1];   //Maior grau.
    int mediana, maior_comp = 0, idx, atual = 1, grau_medio;

    ofstream arquivo_Saida(arquivo_saida);

    if (n%2 == 0){  //Verifica valor da mediana
        mediana = (copia[(n/2) - 1] + copia[n/2])/2; //Número de vértices ímpar. 
    }
    else{
        mediana = (copia[(n-1)/2]); //Número de vértices par. 
    }

    for (int i = 0; i < n; i++){    
        grau_medio += grau[i];
    }

    grau_medio = (grau_medio/n); //Calcula valor do grau médio.

    componentes_conexas();

    arquivo_Saida << "Número de vértices: " << n << endl;       //Gera arquivo .txt
    arquivo_Saida << "Número de arestas: " << numero << endl;
    arquivo_Saida << "Menor grau: " << menor << endl;
    arquivo_Saida << "Maior grau: " << maior << endl;
    arquivo_Saida << "Grau médio: " << grau_medio << endl;
    arquivo_Saida << "Mediana do grau: " << mediana << endl;

    while(acabou == false){                            //Ordena as componentes e lista cada vértice
        maior_comp = 0;
        for (int i = 0; i < componentes.size(); i++){
            if (componentes.size() == 1){
                acabou = true;
            }
            if (componentes[i].size() > maior_comp){
                maior_comp = componentes[i].size();
                idx = i;
            }
        }

        arquivo_Saida << "" << endl;
        arquivo_Saida << "Componente " << atual << endl;
        arquivo_Saida << "Tamanho:  " << componentes[idx].size() << endl;
        arquivo_Saida << "" << endl;
        
        for (int i = 0; i < componentes[idx].size(); i++){
            arquivo_Saida << componentes[idx][i] << endl;
        }

        componentes.erase(componentes.begin() + idx);
        atual += 1;
    }
    arquivo_Saida.close();
}

//Função para inicializar a estrutura da matriz de adjacência (utiliza vector de vector de double, visto que, para os vértices relacionados, é preenchida pelo peso da aresta; para os não relacionados, é preenchida por infinito).
void constroi_matriz_peso(string nome_arquivo){
    vector <vector <double> > exemplo2;
    vector <double> exemplo1;
    ifstream arquivoEntrada(nome_arquivo);
    string linha;
    getline(arquivoEntrada, linha);
    n = stoi(linha); //Determina o total de vértices

    grau.resize(n, 0); //Preenche o vector grau, inicialmente, com n zeros.

    exemplo1.resize(n, infinito); //Preenche a matriz com infinito.
    for (int i = 0; i < n; i++){
        exemplo2.push_back(exemplo1);
    }
    matriz_peso = exemplo2;

        while (getline (arquivoEntrada, linha)){ //Lê as arestas do grafo, a partir da linha 2.
            numero += 1;
            istringstream ss(linha);
            int n1, n2;
            double n3;
            ss >> n1 >> n2 >> n3; 
            if (n3 < 0){
                verifica = 1;
            }
            grau[n1 - 1] += 1;  //Para os vértices da aresta, aumenta seu grau em 1.
            grau[n2 - 1] += 1;
            matriz_peso[n1 - 1][n2 - 1] = n3; //Preenche com o peso da aresta.
            matriz_peso[n2 - 1][n1 - 1] = n3;
        }
    arquivoEntrada.close();    
    memoria_utilizada();    //Registra o pico de memória utilizada.
}

//Função para inicializar a estrutura da lista de adjacência (utliza vector de vector de vector de double, visto que o vetor maior armazena n vetores; cada um armazena os vizinhos na forma de vetores, onde cada vetor armazena o vértice relacionado e o peso da aresta).
void constroi_lista_peso(string nome_arquivo){
    ifstream arquivoEntrada(nome_arquivo);
    string linha;
    getline(arquivoEntrada, linha);
    n = stoi(linha);  // Lê o número de vértices (n) do arquivo.

    // Inicializa o vetor grau com zeros.
    grau.resize(n, 0);

    // Inicializa a lista de adjacência (um vetor de vetores).
    vector <vector <vector <double> > > vertices(n);  // Cada vértice terá uma lista de vizinhos.
    vector <double> exemplo1, exemplo2; 

    while (getline(arquivoEntrada, linha)) {
        exemplo1.resize(0);
        exemplo2.resize(0);
        numero += 1;
        istringstream ss(linha);
        int n1, n2;
        double n3;

        // Lê os dois vértices da aresta.
        ss >> n1 >> n2 >> n3;

        if (n3 < 0){
            verifica = 1;
        }

        // Incrementa o grau de cada vértice.
        grau[n1 - 1] += 1;
        grau[n2 - 1] += 1;

        exemplo1.push_back(n1);
        exemplo1.push_back(n3);
        exemplo2.push_back(n2);
        exemplo2.push_back(n3);

        // Adiciona a aresta na lista de adjacência.
        vertices[n1 - 1].push_back(exemplo2);  // Adiciona n2 como vizinho de n1.
        vertices[n2 - 1].push_back(exemplo1);  // Adiciona n1 como vizinho de n2.
    }
    lista_peso = vertices;

    arquivoEntrada.close();
    memoria_utilizada(); //Registra o pico de memória utilizada.
}

//Função que inicializa o grafo com peso; recebe como parâmetros 0 (escolhe como representação a matriz) ou 1 (escolhe como representação a lista) e o nome do arquivo .txt a ser lido.
void constroi_grafo_peso(int opcao1, string nome_arquivo){
    opcao = opcao1;

    // Representa o grafo conforme a escolha do usuário.
    if (opcao == 0){                
        constroi_matriz_peso(nome_arquivo); 
    }
    else{
        constroi_lista_peso(nome_arquivo);
    }
}

//Função que fornece as distâncias de um vértice inicial s para todos os demais vértices do grafo, utilizando na implementação do algoritmo de Dijkstra um vetor auxiliar.
void distancia_peso(int s) {
    int minimo, idx;
    vector<double> distancia_copia(n);

    // Inicialização dos vetores
    distancia_dijkstra.resize(n, infinito);
    pai.resize(n, -1);
    niveis.resize(n, 0);

    distancia_dijkstra[s - 1] = 0;
    pai[s - 1] = 0;
    distancia_copia = distancia_dijkstra;

    for (int total = 0; total < n; total++) {
        // Encontrar o menor valor em distancia_copia.
        minimo = infinito;
        idx = -1;
        for (int i = 0; i < n; i++) {
            if (distancia_copia[i] < minimo) {
                minimo = distancia_copia[i];
                idx = i;
            }
        }

        if (idx == -1) break;  // Se não há mais nós para processar, sai do loop.
        distancia_copia[idx] = infinito;  // Marca como visitado.

        if (opcao == 0) {
            // Caso da matriz de adjacência
            for (int i = 0; i < n; i++) {
                if (matriz_peso[idx][i] != infinito) {
                    if (distancia_dijkstra[i] > distancia_dijkstra[idx] + matriz_peso[idx][i]) { 
                        distancia_dijkstra[i] = distancia_dijkstra[idx] + matriz_peso[idx][i]; //Atualiza com menor distância.
                        distancia_copia[i] = distancia_dijkstra[idx] + matriz_peso[idx][i];
                        pai[i] = idx + 1; //Constrói árvore de busca.
                        niveis[i] = niveis[idx] + 1;
                    }
                }
            }
        } 
        else {
            // Caso da lista de adjacência
            for (int i = 0; i < lista_peso[idx].size(); i++){
                if (distancia_dijkstra[lista_peso[idx][i][0] - 1] > distancia_dijkstra[idx] + lista_peso[idx][i][1]){
                    distancia_dijkstra[lista_peso[idx][i][0] - 1] = distancia_dijkstra[idx] + lista_peso[idx][i][1];  //Atualiza com menor distância.
                    distancia_copia[lista_peso[idx][i][0] - 1] = distancia_dijkstra[idx] + lista_peso[idx][i][1];
                    pai[lista_peso[idx][i][0] - 1] = idx + 1; //Constrói árvore de busca.
                    niveis[lista_peso[idx][i][0] - 1] = niveis[idx] + 1;
                }
            }
                
        }
    }
}

//Função que fornece as distâncias de um vértice inicial s para todos os demais vértices do grafo, utilizando na implementação do algoritmo de Dijkstra um heap mínimo (MinHeap).
void distancia_peso_heap(int s){
    int minimo, idx;
    bool t = true;
    vector <int> marcados (n, 0);
    priority_queue< pair <double, double>, vector <pair <double, double> >, greater <pair <double, double> > > minHeap;


    //Inicialização dos vetores.
    distancia_dijkstra.resize(n, infinito);
    pai.resize(n, -1);
    niveis.resize(n, 0);

    distancia_dijkstra[s-1] = 0;
    pai[s-1] = 0;
    minHeap.push(make_pair(distancia_dijkstra[s-1], s - 1));

    for (int total = 0; total < n; total++){
        // Encontrar a menor chave/valor
        while (t != false){
            idx = minHeap.top().second;
            minHeap.pop();
            if (marcados[idx] != 1){
                t = false;
            }
        }
        t = true;
        if (opcao == 0){
            // Caso da matriz de adjacência
            for (int i = 0; i < n; i++){
                if (matriz_peso[idx][i] != infinito){
                    if (distancia_dijkstra[i] > distancia_dijkstra[idx] + matriz_peso[idx][i]){
                        distancia_dijkstra[i] = distancia_dijkstra[idx] + matriz_peso[idx][i]; //Atualiza com menor distância.
                        minHeap.push(make_pair(distancia_dijkstra[i], i));
                        pai[i] = idx + 1; //Constrói árvore de busca.
                        niveis[i] = niveis[idx] + 1;
                    }
                }
            }
        }

        else{
             // Caso da lista de adjacência
            for (int i = 0; i < lista_peso[idx].size(); i++){
                if (distancia_dijkstra[lista_peso[idx][i][0] - 1] > distancia_dijkstra[idx] + lista_peso[idx][i][1]){
                    distancia_dijkstra[lista_peso[idx][i][0] - 1] = distancia_dijkstra[idx] + lista_peso[idx][i][1]; //Atualiza com menor distância
                    minHeap.push(make_pair(distancia_dijkstra[lista_peso[idx][i][0] - 1], lista_peso[idx][i][0] - 1));
                    pai[lista_peso[idx][i][0] - 1] = idx + 1; //Constrói árvore de busca.
                    niveis[lista_peso[idx][i][0] - 1] = niveis[idx] + 1;
                }
            }
        }
    }

}

//Função que retorna o caminho mínimo entre o vértice inicial s e o vértice final f, após a execução das funções distancia_peso(s) ou distancia_peso_heap(s).
void caminho_minimo(int s, int f){
    //Inicialização de vetores.
    caminho.resize(0);

    //Recursão até achar raiz (vértice s) ou não ter caminho.
    while (niveis[f-1] != 0 && niveis[f-1] != -1){
        caminho.push_back(f);
        f = pai[f - 1];
    }

    caminho.push_back(f); //Inclui raiz.

    if (niveis[f-1] == -1){
        cout << "Não há caminho mínimo.";
    }

    else{
        cout << "Caminho: ";
        for (int i = caminho.size() - 1; i > -1; i--){
            cout << caminho[i] << " ";
        }
    }
}