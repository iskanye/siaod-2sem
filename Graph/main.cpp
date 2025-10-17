#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <algorithm>
#include <stdlib.h>

using namespace std;

struct Edge {
    int destination;
    int weight;

    Edge(int dest, int w) : destination(dest), weight(w) {}
};

class Graph {
    int vertices;
    vector<vector<Edge>> edges;
public:
    // Конструктор
    Graph(int v) : vertices(v), edges(v) {}

    // Добавление ребра в граф
    void addEdge(int source, int destination, int weight) {
        edges[source].push_back(Edge(destination, weight));
    }

    // Вывод графа
    void printGraph() {
        cout << "Структура графа:" << endl;
        for (int i = 0; i < vertices; i++) {
            cout << "Вершина " << i << " соединена с: ";
            for (auto& edge : edges[i]) {
                cout << "(" << edge.destination << ", вес: " << edge.weight << ") ";
            }
            cout << endl;
        }
    }

    // Получение количества вершин
    int getVerticesCount() {
        return vertices;
    }

    // Алгоритм Дейкстры для нахождения кратчайшего пути
    vector<int> dijkstra(int startVertex, int endVertex);
};

vector<int> Graph::dijkstra(int startVertex, int endVertex) {
    vector<int> weights(vertices, INT_MAX), previous(vertices, -1);
    vector<bool> visited(vertices, false);
    // Приоритетная очередь для выбора вершины с минимальным расстоянием
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    weights[startVertex] = 0;
    pq.push({0, startVertex});
    while (!pq.empty()) {
        // Извлекаем вершину с минимальным расстоянием
        int currentVertex = pq.top().second;
        pq.pop();
        // Если вершина уже посещена, пропускаем
        if (visited[currentVertex])
            continue;
        visited[currentVertex] = true;
        // Если достигли конечной вершины, выходим
        if (currentVertex == endVertex)
            break;
        // Обновляем расстояния до соседних вершин
        for (auto& edge : edges[currentVertex]) {
            int neighbor = edge.destination;
            int newDistance = weights[currentVertex] + edge.weight;

            if (newDistance < weights[neighbor]) {
                weights[neighbor] = newDistance;
                previous[neighbor] = currentVertex;
                pq.push({newDistance, neighbor});
            }
        }
    }
    // Восстанавливаем путь от конечной вершины к начальной
    vector<int> path;
    if (weights[endVertex] == INT_MAX) {
        // Путь не существует
        return path;
    }
    for (int vertex = endVertex; vertex != -1; vertex = previous[vertex]) {
        path.push_back(vertex);
    }
    return path;
}

void demonstration() {
    Graph graph(6);

    // Добавляем ребра
    graph.addEdge(0, 1, 8);
    graph.addEdge(0, 2, 4);
    graph.addEdge(1, 3, 6);
    graph.addEdge(1, 4, 3);
    graph.addEdge(2, 1, 3);
    graph.addEdge(2, 3, 2);
    graph.addEdge(2, 5, 10);
    graph.addEdge(3, 5, 1);
    graph.addEdge(3, 4, 3);
    graph.addEdge(4, 5, 4);
    graph.printGraph();

    vector<pair<int, int>> testPairs = {{0, 5}, {0, 4}, {1, 5}, {2, 3}};
    for (auto& pair : testPairs) {
        vector<int> testPath = graph.dijkstra(pair.first, pair.second);

        if (testPath.empty()) {
            cout << "Путь от вершины " << pair.first << " к вершине " << pair.second << " не существует!" << endl;
        } else {
            cout << "Путь от вершины " << pair.first << " к вершине " << pair.second << ": ";
            for (size_t i = 0; i < testPath.size(); i++) {
                cout << testPath[i];
                if (i != testPath.size() - 1) cout << " <- ";
            }
            cout << endl;
        }
    }
}

// Функция для интерактивной работы с графом
void interactiveMode() {
    int vertices;
    cout << "Введите количество вершин в графе: ";
    cin >> vertices;

    Graph graph(vertices);

    int edges;
    cout << "Введите количество ребер: ";
    cin >> edges;

    cout << "Введите ребра в формате: исходная_вершина конечная_вершина вес" << endl;
    for (int i = 0; i < edges; i++) {
        int src, dest, weight;
        cin >> src >> dest >> weight;
        graph.addEdge(src, dest, weight);
    }

    while (true) {
        cout << "1. Показать структуру графа" << endl;
        cout << "2. Найти кратчайший путь" << endl;
        cout << "3. Выход" << endl;
        cout << "Выберите опцию: ";

        int choice;
        cin >> choice;

        switch (choice) {
            case 1:
                graph.printGraph();
                break;
            case 2: {
                int start, end;
                cout << "Введите начальную вершину: ";
                cin >> start;
                cout << "Введите конечную вершину: ";
                cin >> end;

                if (start < 0 || start >= vertices || end < 0 || end >= vertices) {
                    cout << "Неверные номера вершин!" << endl;
                    break;
                }

                vector<int> path = graph.dijkstra(start, end);

                if (path.empty()) {
                    cout << "Путь не существует!" << endl;
                } else {
                    cout << "Кратчайший путь: ";
                    for (size_t i = 0; i < path.size(); i++) {
                        cout << path[i];
                        if (i != path.size() - 1) cout << " -> ";
                    }
                    cout << endl;
                }
                break;
            }
            case 3:
                return;
            default:
                cout << "Неверный выбор!" << endl;
        }
    }
}

int main() {
    setlocale(LC_ALL, "Russian");

    int mode;
    cout << "1. Демонстрация на готовом графе" << endl;
    cout << "2. Интерактивный режим" << endl;
    cout << "Ваш выбор: ";
    cin >> mode;
    system("cls");

    if (mode == 1) {
        demonstration();
    } else if (mode == 2) {
        interactiveMode();
    }

    return 0;
}
