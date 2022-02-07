//
// Created by vanessa on 3/2/22.
//

#ifndef INC_21S2_CP_SPA_TEAM_31_DEPENDENCYGRAPH_H
#define INC_21S2_CP_SPA_TEAM_31_DEPENDENCYGRAPH_H

#include <list>
#include <map>

namespace spa {

class dependency_graph {

  public:

    explicit dependency_graph(int num_nodes);
    void AddConnection(int v1, int v2);

    std::vector<std::vector<int> > GetConnectedNodes();

  private:

    int num_vert;
    std::list<int> *node_list;

    std::vector<int> RunDfs(int i, bool visited[], std::vector<int> group);

};

}

#endif //INC_21S2_CP_SPA_TEAM_31_DEPENDENCYGRAPH_H
