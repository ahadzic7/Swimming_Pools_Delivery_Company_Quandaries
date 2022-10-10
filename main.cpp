#include<cstdio>
#include <ctime>
#include <vector>
#include <list>

void readData(int &crossings, int &streets, std::vector<std::list<int>> &graph) {
    scanf("%d %d", &crossings, &streets);

    std::vector<std::list<int>> g(crossings);
    graph = std::move(g);

    for(int i = 0; i < streets; i++) {
        int x, y;
        scanf("%d %d",&x, &y);
        graph.emplace_back(x, y);
    }
}

void x() {
    int crossings, streets;
    std::vector<std::list<int>> graph(0);

    readData(crossings, streets, graph);
}

int main() {
    clock_t start, end;

    start = clock();

    x();

    end = clock();

    printf("Execution time: %f", (end - start) / 1000.);

    return 0;
}
