#include "grafos.h"

int main(){

    //Item 1
    constroi_grafo_peso(1, "grafo_W_1.txt"); //Alternar entre os grafos.
    distancia_peso_heap(10); //Foi escolhida a implementação com heap, por ser consideravelmente mais rápida.
    cout << "Distância: " << distancia_dijkstra[19] << " "; //Vértice 20.
    caminho_minimo(10, 20);
    cout << endl << "Distância: " << distancia_dijkstra[29] << " "; //Vértice 30.
    caminho_minimo(10, 30);
    cout << endl << "Distância: " << distancia_dijkstra[39] << " "; //Vértice 40.
    caminho_minimo(10, 40);
    cout << endl << "Distância: " << distancia_dijkstra[49] << " "; //Vértice 50.
    caminho_minimo(10, 50);
    cout << endl << "Distância: " << distancia_dijkstra[59] << " "; //Vértice 60.
    caminho_minimo(10, 60);

    //Item 2
   constroi_grafo_peso(1, "grafo_W_1.txt"); //Alternar entre matriz/lista e os grafos.
   for (int i = 0; i < 50; i++){ //Alterar número de testes.
   random_device rd;
   mt19937 gen(rd());
   uniform_int_distribution <> dis(1, n); 
   int escolha = dis(gen);          //Escolhe vértice aleatório
   clock_t start = clock();
   distancia_peso(escolha);         //Calcula as distâncias (alternar entre distancia_peso(escolha) e distancia_peso_heap(escolha)).
   clock_t end = clock();
   cout <<  double(end - start) / CLOCKS_PER_SEC << endl;  // Retorna o tempo em segundos
   }

    //Item 3
    constroi_grafo_peso(1, "rede_colaboracao.txt");
    distancia_peso_heap(2722); //Calcula as distâncias a partir de Edsger W. Dijkstra.
    cout << "Distância: " << distancia_dijkstra[11364] << " "; // Alan M. Turing.
    caminho_minimo(2722, 11365);
    cout << endl << "Distância: " << distancia_dijkstra[471364] << " "; //J. B. Kruskal.
    caminho_minimo(2722, 471365);
    cout << endl << "Distância: " << distancia_dijkstra[5708] << " "; //Jon M. Kleinberg.
    caminho_minimo(2722, 5709);
    cout << endl << "Distância: " << distancia_dijkstra[11385] << " "; //Éva Tardos.
    caminho_minimo(2722, 11386);
    cout << endl << "Distância: " << distancia_dijkstra[343929] << " "; //Daniel R. Figueiredo.
    caminho_minimo(2722, 343930);

    return 0;

    //OBSERVAÇÃO: Cada item foi executado individualmente, de modo a observar cada reultado.
}