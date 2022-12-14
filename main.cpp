#include<cstdio>
#include <vector>
#include <list>
#include <stack>
#include <algorithm>
#include <queue>
#include <ctime>

/*
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
    printf("------------------\n");
    printf("\n\nVariance is %d\n\n", max - 1);
    printf("------------------\n");
}

void check_components_sort(std::vector<std::list<int>> sccLists, std::vector<int> scc) {
    std::vector<std::vector<int>> x(sccLists.size()+1, std::vector<int>());
    for(int i = 0; i < scc.size(); i++) {
        if(scc[i] != -1) {
            x[scc[i]].push_back(i);
        }

    }

    for(int i = 0; i < x.size(); i++) {
        if(x[i].empty()) {
            x.erase(x.begin() + i);
            i--;
            continue;
        }
        std::sort(x[i].begin(), x[i].end());
    }
    for(int i = 0; i < sccLists.size(); i++) {

        if(sccLists[i].empty()) {
            sccLists.erase(sccLists.begin() + i);
            i--;
            continue;
        }
        sccLists[i].sort();
        std::sort(x[i].begin(), x[i].end());
    }

//    printf("###############################");
//    for(auto row: x) {
//        for(auto el: row) {
//            printf("%d ", el);
//        }
//        printf("\n");
//    }

    printf("###############################\n");

    for(int i = 0; i < sccLists.size(); i++) {
        bool found = false;
        for(int j = 0; j < x.size(); j++){
            if(std::equal(sccLists[i].begin(), sccLists[i].end(), x[j].begin())) {
                found = true;
                break;
            }
        }
        if(!found)
            printf("Ne valja %d\n", i);
    }
    printf("###############################\n");

}
*/
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
int counter2 = 0;
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
        std::list<int> cache;
        while (!sccIt->empty()) {
            int crossing = *it;
            for (const auto &node: graph[crossing]) {
                if (scc[crossing] != scc[node]) {
                    weakComponents[sccIt - sccLists.begin()] = true;
                    weakCrossings[crossing] = true;

                    auto p = it;
                    it--;
                    sccIt->erase(p);

                    cache.push_back(crossing);
                    break;
                }
            }
            it++;
            if (it == sccIt->end())
                break;
        }

        for(const auto el: cache)
            scc[el] = -1;

        sccIt++;
    }
}

void findSCC2(int crossing, std::vector<int> &lowlinks, std::stack<int> &components, std::vector<std::list<int>> &graph, std::vector<bool> &inStack, std::vector<int> &index, std::vector<std::list<int>> &sccLists, std::vector<bool> &weakCrossings) {
    counter2++;
    lowlinks[crossing] = counter2;
    index[crossing] = counter2;

    components.push(crossing);
    inStack[crossing] = true;

    for(const auto &node: graph[crossing]) {
        if(!weakCrossings[node]) {
            if(index[node] == 0) {
                findSCC2(node, lowlinks, components, graph, inStack, index, sccLists, weakCrossings);
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
            inStack[node] = false;
        } while (node != crossing);

        sccLists.push_back(component);
    }

}

void filter(std::vector<std::list<int>> &sccLists, const std::vector<std::list<int>> &graph, std::vector<int> &scc, std::vector<bool> &weakComponents, std::vector<bool> &weakCrossings) {
    for(int i = 0; i < graph.size(); i++){
        for(const auto &node: graph[i]) {
            if(scc[i] != scc[node]) {
                weakCrossings[i] = true;
                weakComponents[scc[i]] = true;
            }
        }
    }

    int i = 0;
//    for(auto &component: sccLists) {
//        std::list<int> cache;
//        for(const auto &cross: component) {
//            for(const auto &node: graph[cross]) {
//                if(scc[cross] != scc[node]) {
//                    weakCrossings[cross] = true;
//                    weakComponents[i] = true;
//                    cache.push_back(cross);
//                }
//            }
//        }
//        for(auto cross: cache) {
////            component.remove(cross);
//            scc[cross] = -1;
//        }
//        i++;
//    }
}


void getCost(const int crossings, const std::list<int> &component, const std::vector<std::list<int>> &graph, const std::vector<int> &scc, std::vector<bool> &weakCrossings, int &minCost, int &potentialCrossings) {
    std::queue<int> toVisit;
    std::vector<std::vector<int>> costsOfCrossings(crossings, std::vector<int>(2, 0));

    for(const auto crossing: component) {
        if(weakCrossings[crossing])
            continue;
        std::vector<bool> visited(crossings, false);
        std::list<int> cache;
        toVisit.push(crossing);
        std::vector<int> costs(crossings, 10e8);

        costs[crossing] = 0;

        while (!toVisit.empty()) {
            int cross = toVisit.front();
            toVisit.pop();

            if(!visited[cross]) {
                visited[cross]  = true;
                for(const auto node: graph[cross]) {
                    if(node == crossing || scc[node] != scc[crossing])
                        continue;
                    if(weakCrossings[node])
                        continue;

                    if(costs[node] > costs[cross] + 1) {
                        costs[node] = costs[cross] + 1;
                        cache.emplace_back(node);
                    }
                        toVisit.push(node);
                }
            }
        }

        for(auto node: cache) {
            costsOfCrossings[crossing][0] += costs[node];
            costsOfCrossings[node][1] +=  costs[node];
        }
    }

    minCost = costsOfCrossings[*component.begin()][0] * 2 + costsOfCrossings[*component.begin()][1];
    for(const auto &crossing: component) {
        if(!weakCrossings[crossing] && minCost > costsOfCrossings[crossing][0] * 2 + costsOfCrossings[crossing][1])
            minCost = costsOfCrossings[crossing][0] * 2 + costsOfCrossings[crossing][1];
    }
    int count = 0;
    for(const auto &crossing: component) {
        if(!weakCrossings[crossing] && minCost == costsOfCrossings[crossing][0] * 2 + costsOfCrossings[crossing][1])
            count++;
    }
    potentialCrossings = count;

//    printf("%d\n", minCost);
}





void solution() {
    int crossings = 0, streets = 0;

    scanf("%d %d", &crossings, &streets);
    std::vector<std::list<int>> graph(crossings);

    for(int i = 0; i < streets; i++) {
        int x, y;
        scanf("%d %d",&x, &y);
        graph[x].push_back(y);
    }

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

    std::vector<bool> weakCrossings(crossings, false);
    std::vector<bool> weakComponents(sccLists.size(), false);

//    print_scc(sccLists, weakCrossings);

    filter(sccLists, graph, scc, weakComponents, weakCrossings);
//    print_scc(sccLists, weakComponents);

    std::stack<int> components2;
    std::vector<bool> inStack2(crossings, false);
    std::vector<int> lowlinks2(crossings, -1);
    std::vector<int> index2(crossings, 0);

    for(int i = 0; i < sccLists.size(); i++) {

        if(!sccLists[i].empty() && weakComponents[i]) {
            //repeat tarjan
            bool del = true;
            std::vector<std::list<int>> sccLists2;

            for(const auto &el: sccLists[i]) {
                if(!weakCrossings[el]){
                    if (index2[el] == 0)
                        findSCC2(el, lowlinks2, components2, graph, inStack2, index2, sccLists2, weakCrossings);
                    else
                        del = false;
                }
            }

            if(sccLists2.empty() && del) {
                sccLists.erase(sccLists.begin() + i);
                i--;
            }
            else if(sccLists2.size() == 1 )
                sccLists[i] = std::move(sccLists2[0]);
            else if(!sccLists2.empty()){
                sccLists[i] = std::move(sccLists2[0]);
                for(int j = 1; j < sccLists2.size(); j++) {
                    sccLists.push_back(sccLists2[j]);
                    for(const auto crossing: sccLists2[j]) {
                        scc[crossing] = sccLists.size();
                    }
                }
            }
        }
    }

//    print_scc(sccLists, weakCrossings);
//    check_components_sort(sccLists, scc);

    int maxVariance = -1;
    bool firstVariance = true;
    std::vector<int> maxComponent(sccLists.size(), 0);
    int it = 0;
    for(const auto &component: sccLists){
        int count = 0;
        for(const auto node: component)
            if(!weakCrossings[node])
                count++;
        if(firstVariance) {
            maxVariance = count;
            firstVariance = false;
        }
        else if(maxVariance < count) {
            maxVariance = count;
        }
        maxComponent[it] = count;
        it++;
    }

    std::list<std::pair<int, int>> costs;
    int minCost;
    bool first = true;
    int itt = 0;
    for(const auto &component : sccLists) {
        if(maxComponent[itt] == maxVariance) {
            int cost, potentialCrossings;
            getCost(crossings, component, graph, scc, weakCrossings,cost, potentialCrossings);
            if(first) {
                minCost = cost;
                first = false;
            }
            if(minCost > cost)
                minCost = cost;

            costs.emplace_back(cost, potentialCrossings);
        }
        itt++;
    }


//    int minCost = costs.begin()->first;
//    for(const auto &cp: costs) {
//        if(cp.first < minCost)
//            minCost = cp.first;
//    }

    int potentialCrossings = 0;
    for(const auto &cp: costs) {
        if(cp.first == minCost)
            potentialCrossings += cp.second;
    }

//    printf("\n\nPotential crossings %d\n", potentialCrossings);
//    printf("Max variability is %d\n", maxVariance - 1);
//    printf("Min cost is %d\n\n", minCost);


    int x = 0, y = 0, z = 0;
//    printf("%d %d %d", x, y, z);
    printf("%d %d %d", potentialCrossings, maxVariance -1, minCost);
}

int main() {
    clock_t start, end;
    start = clock();

    solution();

    end = clock();

//    printf("\n\nExecution time: %f", (end - start) / 1000.);

    return 0;
}
