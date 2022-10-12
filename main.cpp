#include<cstdio>
#include <ctime>
#include <vector>
#include <list>
#include <stack>
#include <algorithm>
#include <queue>

void print_graph(const std::vector<std::list<int>> &graph) {
    for(int i = 0; i < graph.size(); i++) {
        printf("%d: ", i);
        for(auto el: graph[i])
            printf("%d ", el);
        printf("\n");
    }
}

void print_components_graph(const std::vector<std::list<std::pair<int, int>>> &graph) {
    for(int i = 0; i < graph.size(); i++) {
        printf("%d: ", i);
        for(auto el: graph[i])
            printf("(%d, %d) ", el.first, el.second);
        printf("\n");
    }
}

void print_scc(std::vector<std::list<int>> scc, std::vector<bool> weakCrossings) {
    int max = scc[0].size();
    for(int i = 0; i < scc.size(); i++) {
        printf("%d: weak Crosing %d -> ", i,  weakCrossings[i] ? 1 : 0);
        if(max < scc[i].size())
            max = scc[i].size();
        scc[i].sort();
        for(auto el: scc[i])
            printf("%d ", el);
        printf("\n");
    }
    printf("\n\n Variance is %d\n ", max - 1);
}

void print_cost_variance(std::vector<std::vector<int>> costVariance) {
    for(int i = 0; i < costVariance.size(); i++) {
        if(costVariance[i][1] != -1)
            printf("%d: c = %d v = %d", i, costVariance[i][0], costVariance[i][1]);
        printf("\n");
    }
}

void print_weakCrossings() {}

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


void filterWeakCrossings(std::vector<std::list<int>> &sccLists, const std::vector<std::list<int>> &graph, std::vector<int> &scc, std::vector<bool> &weakComponents, std::vector<bool> &weakCrossings) {
    auto sccIt = sccLists.begin();
    while (sccIt != sccLists.end()) {
        auto it = sccIt->begin();

        while (!sccIt->empty()) {
            int crossing = *it;
            for (const auto &node: graph[crossing]) {
                if (scc[crossing] != scc[node]) {
                    weakComponents[sccIt - sccLists.begin()] = true;
                    weakCrossings[crossing] = true;
                    auto p = it;
                    it--;
                    sccIt->erase(p);
                    break;
                }
            }
            it++;
            if (it == sccIt->end())
                break;
        }
        sccIt++;
    }
}

void findSCC2(int crossing, std::vector<int> &lowlinks, std::stack<int> &components, std::vector<std::list<int>> &graph, std::vector<bool> &inStack, std::vector<int> &index, std::vector<std::list<int>> &sccLists, std::vector<int> &scc, std::vector<bool> &weakCrossings) {
    counter++;
    lowlinks[crossing] = counter;
    index[crossing] = counter;

    components.push(crossing);
    inStack[crossing] = true;

    for(const auto &node: graph[crossing]) {
        if(!weakCrossings[node]) {
            if(index[node] == 0) {
                findSCC2(node, lowlinks, components, graph, inStack, index, sccLists, scc, weakCrossings);
                if(lowlinks[node] < lowlinks[crossing])
                    lowlinks[crossing] = lowlinks[node];

            }

            else if(inStack[node]) {
                if(index[node] < lowlinks[crossing])
                    lowlinks[crossing] = index[node];
            }
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




void getCostVariance(const int crossings, const std::list<int> &component, const std::vector<std::list<int>> &graph, const std::vector<bool> &weakCrossings, std::vector<std::vector<int>> &costVariance) {
    if(component.size() == 1) {
        int crossing = *component.begin();
        if(!weakCrossings[crossing]) {
            costVariance[crossing][0] = 0;
            costVariance[crossing][1] = 0;
        }
        return;
    }

    std::queue<int> toVisit;
    std::vector<std::list<std::pair<int, int>>> componentGraph(crossings);

//    for(const auto crossing: component) {
//        int targets = 0;
//        std::vector<bool> visited(crossings, false);
//
//        toVisit.push(crossing);
//
//
//        int iteration = 0;
//
//        while (!toVisit.empty()) {
//            int cross = toVisit.front();
//
//
//
//            toVisit.pop();
//
//            iteration++;
//
//
//            if(weakCrossings[cross]) {
//                visited[cross] = true;
//                continue;
//            }
//
//            if(!visited[cross]) {
//                visited[cross]  = true;
//                for(const auto node: graph[cross]) {
//                    if(!visited[node] && !weakCrossings[node] && crossing != node) {
//                        targets++;
//                        componentGraph[crossing].emplace_back(node, iteration);
//                        toVisit.push(node);
//                    }
//                }
//
//            }
//        }
//        int x = 0;
//    }

//    print_components_graph(componentGraph);

    std::vector<std::vector<int>> costs(crossings, std::vector<int>(2, 0));



//    for(const auto crossing: component) {
//        for(const auto &strongLink: componentGraph[crossing]) {
//            costs[crossing][0] += strongLink.second;
//            costs[strongLink.first][1] += strongLink.second;
//        }
//    }

    int i = 0;
//    for(auto row: costs) {
//        printf("%d: ", i);
//        for(auto el: row)
//            printf("%d ", el);
//        printf("\n");
//        i++;
//    }


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


    std::vector<std::vector<int>> costVariance(crossings, std::vector<int>(2, -1));
    std::vector<bool> weakCrossings(crossings, false);
    std::vector<bool> weakComponents(sccLists.size(), false);

    print_scc(sccLists, weakCrossings);
    printf("------------------\n");


    filterWeakCrossings(sccLists, graph, scc, weakComponents, weakCrossings);

    print_scc(sccLists, weakComponents);
    printf("------------------\n");

    for(int i = 0; i < sccLists.size(); i++) {
        if(!sccLists[i].empty() && weakComponents[i]) {
            //repeat tarjan
            std::stack<int> components2;
            std::vector<bool> inStack2(crossings, false);
            std::vector<int> lowlinks2(crossings, -1);
            std::vector<int> index2(crossings, 0);
            std::vector<std::list<int>> sccLists2;
            std::vector<int> scc2(crossings);

            for(const auto &el: sccLists[i]) {
                if(!index2[el])
                    findSCC2(el, lowlinks2, components2, graph, inStack2, index2, sccLists2, scc2, weakCrossings);
            }


            if(sccLists2.empty())
                sccLists.erase(sccLists.begin() + i);
            else if(sccLists2.size() == 1)
                sccLists[i] = std::move(sccLists2[0]);
            else {
                sccLists[i] = std::move(sccLists2[0]);
                for(int j = 1; j < sccLists2.size(); j++)
                    sccLists.push_back(sccLists2[j]);
            }

        }
    }

    print_scc(sccLists, weakCrossings);
}

int main() {
    clock_t start, end;

    start = clock();

    solution();

    end = clock();

    printf("Execution time: %f", (end - start) / 1000.);

    return 0;
}
