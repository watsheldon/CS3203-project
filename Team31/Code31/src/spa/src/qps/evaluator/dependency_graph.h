//
// Created by vanessa on 3/2/22.
//

#ifndef INC_21S2_CP_SPA_TEAM_31_DEPENDENCYGRAPH_H
#define INC_21S2_CP_SPA_TEAM_31_DEPENDENCYGRAPH_H

#include <list>
#include <map>

class dependency_graph {


public:

    void addConnection(int v1, int v2);

    std::vector<std::vector<int> > getConnectedNodes();


    explicit dependency_graph(int numNodes);


private:

    int numVert;

    std::list<int>* nodeList;

    std::vector<int> runDfs(int i, bool visited[], std::vector<int> group);

};


#endif //INC_21S2_CP_SPA_TEAM_31_DEPENDENCYGRAPH_H
