#include<cstdio>
#include <ctime>
#include <vector>
#include <list>
#include <stack>

void print_graph(const std::vector<std::list<int>> &graph) {
    for(int i = 0; i < graph.size(); i++) {
        printf("%d: ", i);
        for(auto el: graph[i])
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

//-----------------------------------------------------------GLOBAL VARIABLES---------------------------------

std::vector<std::list<int>> sccLists;
//-----------------------------------------------------------GLOBAL VARIABLES---------------------------------


void findSCC(int crossing, std::vector<int> &lowlinks, std::stack<int> &components, std::vector<std::list<int>> &graph, std::vector<bool> &inStack) {
    lowlinks[crossing] = crossing;
    components.push(crossing);
    inStack[crossing] = true;

    for(const auto &node: graph[crossing]) {
        if(lowlinks[node] == -1) {
            findSCC(node, lowlinks, components, graph, inStack);
            if(lowlinks[node] < lowlinks[crossing])
                lowlinks[crossing] = lowlinks[node];
        }

        else if(inStack[node]) {
            if(node < lowlinks[crossing])
                lowlinks[crossing] = node;
        }
    }

    if(lowlinks[crossing] == crossing) {
        std::list<int> component;
        int node;
        do {
            node = components.top();
            components.pop();
            component.push_back(node);
            inStack[node] = false;
        } while (node != crossing);
        sccLists.push_back(component);
    }

}


void solution() {
    int crossings, streets;
    std::vector<std::list<int>> graph(0);

    readData(crossings, streets, graph);
    std::stack<int> components;
    std::vector<bool> inStack(crossings, false);
    std::vector<int> lowlinks(crossings, -1);


    for(int i = 0; i < crossings; i++) {
        findSCC(i, lowlinks, components, graph, inStack);
    }

    for(auto & sccList : sccLists) {
        for(auto el: sccList)
            printf("%d ", el);
        printf("\n");
    }





}

int main() {
    clock_t start, end;

    start = clock();

    solution();

    end = clock();

    printf("Execution time: %f", (end - start) / 1000.);

    return 0;
}
