//
// Created by vanessa on 3/2/22.
//

#include "dependency_graph.h"
#include <list>
#include <vector>

using namespace spa;

dependency_graph::dependency_graph(int numNodes) {

    this->numVert = numNodes;
    nodeList = new std::list<int>[numNodes];

}

void dependency_graph::addConnection(int v1, int v2) {

    nodeList[v1].push_back(v2);
    nodeList[v2].push_back(v1);

}

std::vector<int> dependency_graph::runDfs(int i, bool visited[], std::vector<int> group) {

    visited[i] = true;
    group.push_back(i);

    std::list<int>::iterator j;
    for (j = nodeList[i].begin(); j != nodeList[i].end(); j++) {
        if (!visited[*j]) {
            runDfs(*j, visited, group);
        }
    }

    return group;

}

std::vector<std::vector<int> > dependency_graph::getConnectedNodes() {

    int numNodes = this->numVert;

    bool *visited = new bool[numNodes];
    std::vector<std::vector<int> > allGroups;

    for (int i = 0; i < numNodes; i++) {
        visited[i] = false;
    }

    for (int i = 0; i < numNodes; i++) {
        if (!visited[i]) {
            std::vector<int> group;
            group = runDfs(i, visited, group);
            allGroups.push_back(group);
        }
    }

    delete[] visited;
    return allGroups;
}
