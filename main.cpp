#include<cstdio>
#include <ctime>
#include <vector>
#include <list>
#include <stack>
#include <algorithm>

void print_graph(const std::vector<std::list<int>> &graph) {
    for(int i = 0; i < graph.size(); i++) {
        printf("%d: ", i);
        for(auto el: graph[i])
            printf("%d ", el);
        printf("\n");
    }
}

void print_scc(std::vector<std::list<int>> scc) {
    for(auto & list : scc) {
        for(auto el: list)
            printf("%d ", el);
        printf("\n");
    }
}

void readData(int &crossings, int &streets, std::vector<std::list<int>> &graph) {
    scanf("%d %d", &crossings, &streets);

    std::vector<std::list<int>> g(crossings);
    graph = std::move(g);

    for(int i = 0; i < streets; i++) {
        int x, y;
        scanf("%d %d",&x, &y);
        graph[x].push_back(y);
    }

//    print_graph(graph);
}

//----------------------------------------------------GLOBAL VARIABLES--------------------------------------------------
int counter = 0;
//----------------------------------------------------GLOBAL VARIABLES--------------------------------------------------


void findSCC(int crossing, std::vector<int> &lowlinks, std::stack<int> &components, std::vector<std::list<int>> &graph, std::vector<bool> &inStack, std::vector<int> &index, std::vector<std::list<int>> &sccLists, std::vector<int> &scc) {
    counter++;
    lowlinks[crossing] = counter;
    index[crossing] = counter;

    components.push(crossing);
    inStack[crossing] = true;

    for(const auto &node: graph[crossing]) {
        if(index[node] == 0) {
            findSCC(node, lowlinks, components, graph, inStack, index, sccLists, scc);
            if(lowlinks[node] < lowlinks[crossing])
                lowlinks[crossing] = lowlinks[node];

        }

        else if(inStack[node]) {
            if(index[node] < lowlinks[crossing])
                lowlinks[crossing] = index[node];
        }
    }

    if(lowlinks[crossing] == index[crossing]) {
        std::list<int> component;
        int node;

        do {
            node = components.top();
            components.pop();
            component.push_back(node);
            scc[node] = sccLists.size();
            inStack[node] = false;
        } while (node != crossing);

        sccLists.push_back(component);
    }

}

void filterWeakCrossings(std::vector<std::list<int>> &sccLists, const std::vector<std::list<int>> &graph, std::vector<int> &scc) {
    for(auto &component: sccLists) {
        auto it = component.begin();
        while(it != component.end()) {
            int crossing = *it;
            if(!graph[crossing].empty()) {
                for(const auto &node: graph[crossing]) {
                    if(scc[crossing] != scc[node]) {
                        auto p = it;
                        it--;
                        component.erase(p);
                        break;
                    }
                }
            }
            it++;
            if(component.empty())
                break;
        }
    }
}

void solution() {
    int crossings, streets;
    std::vector<std::list<int>> graph(0);

    readData(crossings, streets, graph);

    std::stack<int> components;
    std::vector<bool> inStack(crossings, false);
    std::vector<int> lowlinks(crossings, -1);
    std::vector<int> index(crossings, 0);
    std::vector<std::list<int>> sccLists;
    std::vector<int> scc(crossings);


    for(int i = 0; i < crossings; i++) {
        if(index[i] == 0)
        findSCC(i, lowlinks, components, graph, inStack, index, sccLists, scc);
    }

//    print_scc(sccLists);

    printf("------------------\n");
    filterWeakCrossings(sccLists, graph, scc);

//    print_scc(sccLists);

}

int main() {
    clock_t start, end;

    start = clock();

    solution();

    end = clock();

    printf("Execution time: %f", (end - start) / 1000.);

    return 0;
}
