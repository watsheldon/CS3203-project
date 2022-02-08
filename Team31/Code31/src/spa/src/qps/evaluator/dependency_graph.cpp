//
// Created by vanessa on 3/2/22.
//

#include "dependency_graph.h"
#include <list>
#include <vector>

using namespace spa;
/**
dependency_graph::dependency_graph(int num_nodes) {

    this->num_vert = num_nodes;
    node_list = new std::list<int>[num_nodes];

}

void dependency_graph::AddConnection(int v1, int v2) {

    node_list[v1].push_back(v2);
    node_list[v2].push_back(v1);

}

std::vector<int> dependency_graph::RunDfs(int i, bool visited[], std::vector<int> group) {

    visited[i] = true;
    group.push_back(i);

    std::list<int>::iterator j;
    for (j = node_list[i].begin(); j != node_list[i].end(); j++) {
        if (!visited[*j]) {
            RunDfs(*j, visited, group);
        }
    }

    return group;

}

std::vector<std::vector<int> > dependency_graph::GetConnectedNodes() {

    int numNodes = this->num_vert;

    bool *visited = new bool[numNodes];
    std::vector<std::vector<int> > allGroups;

    for (int i = 0; i < numNodes; i++) {
        visited[i] = false;
    }

    for (int i = 0; i < numNodes; i++) {
        if (!visited[i]) {
            std::vector<int> group;
            group = RunDfs(i, visited, group);
            allGroups.push_back(group);
        }
    }

    delete[] visited;
    return allGroups;
}
*/