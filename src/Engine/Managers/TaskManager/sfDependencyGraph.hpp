/******************************************************************************/
/*!
\par     Sulfur
\file    sfDependencyGraph.hpp
\author  Maxim Kolesnik
\par     DP email: maxim.kolesnik@digipen.edu
\date    9/6/2016

\brief   Task dependency graph

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#pragma once

#include <unordered_map>
#include <queue>

namespace Sulfur
{
  class DependencyGraph
  {
  public:
    DependencyGraph(void);
    ~DependencyGraph(void);

    void AddNode(const std::string &taskName);
    //Task that is first to update, does not depend on anything
    void SetStartingTask(const std::string &taskName);
    void SetDependency(const std::string &taskName, const std::string &dependsOn);

    //Finishes the graph adding dummy end node and start node
    //Every node that is not passed as dependency will be considered as final task
    //and linked to the end node
    void CompleteGraph(void);

    bool TryPullTask(std::string &taskName);
    bool AreAllTasksDone(void) const;

    //Resets dependency counts.
    void Restart(void);

    void NotifyTaskCompletion(const std::string &taskName);
  private:
    const std::string c_startingNode = "DepGraphStart";
    const std::string c_endNode = "DepGraphEndNode";

    struct DepNode;

    struct NodeHash
    {
      size_t operator()(const DepNode &node) const;
      size_t operator()(const DepNode *node) const;
    };

    struct DepNode
    {
      DepNode(void) {};
      DepNode(const std::string &name);
      
      bool operator==(const DepNode &node) const;
      bool operator==(const std::string &name) const;

      //Dependency count. Indicates how many task are needed to be finsihed
      //before this node can be executed
      INT32 m_depCount; 
      std::string m_name;
    };

    void _ResetCounts(void);
    void _EnqueueFirstTasks(void);

    //List is going to contain a pointer to a node from NodeMap
    // key is a dependency, list of items is list of tasks that are executed after
    // key task
    typedef std::unordered_map<DepNode, std::list<DepNode*>, NodeHash> AdjacencyList;
    //NodeMap is used to contain all nodes for shared storage
    typedef std::unordered_map<DepNode, DepNode, NodeHash> NodeMap; 

    AdjacencyList m_adjacencyList;
    NodeMap m_nodeMap;

    std::queue<DepNode*> m_readyNodes;
  };
}
