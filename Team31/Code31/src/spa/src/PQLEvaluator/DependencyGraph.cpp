//
// Created by vanessa on 3/2/22.
//

#include "DependencyGraph.h"
#include <list>
#include <map>
#include <vector>

DependencyGraph::DependencyGraph(int numNodes) {

    this->numNodes = numNodes;
    nodeList = new std::list<int>[numNodes];

}

void DependencyGraph::addConnection(int v1, int v2) {

    nodeList[v1].push_back(v2);
    nodeList[v2].push_back(v1);

}

std::vector<int> DependencyGraph::runDfs(int i, bool visited[], std::vector<int> group) {

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

std::vector<std::vector<int> > DependencyGraph::getConnectedNodes(int nodes) {

    bool *visited = new bool[nodes];
    std::vector<std::vector<int> > allGroups;

    for (int i = 0; i < nodes; i++) {
        visited[i] = false;
    }

    for (int i = 0; i < nodes; i++) {
        if (!visited[i]) {
            std::vector<int> group;
            group = runDfs(i, visited, group);
            allGroups.push_back(group);
        }
    }

    delete[] visited;
    return allGroups;
}
