/******************************************************************************/
/*!
\par     Sulfur
\file    sfDependencyGraph.cpp
\author  Maxim Kolesnik
\par     DP email: maxim.kolesnik@digipen.edu
\date    9/6/2016

\brief   Job dependency graph

All content � 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#include "sfDependencyGraph.hpp"

namespace Sulfur
{
  DependencyGraph::DependencyGraph(void)
  {
    m_nodeMap[c_startingNode] = DepNode(c_startingNode);
    m_nodeMap[c_endNode] = DepNode(c_endNode);
  }

  void DependencyGraph::AddIndependentNode(const std::string &taskName)
  {
    AddDependentNode(taskName, c_startingNode);
  }

  void DependencyGraph::AddDependentNode(const std::string &taskName, 
    const std::string &dependencyName)
  {
    if (m_nodeMap.find(taskName) == m_nodeMap.end())
      m_nodeMap[taskName] = DepNode(taskName);
    if (m_nodeMap.find(dependencyName) == m_nodeMap.end())
      m_nodeMap[dependencyName] = DepNode(dependencyName);

    auto item = m_adjacencyList[dependencyName];
    item.push_back(&m_nodeMap[taskName]);
  }

  bool DependencyGraph::TryPullTask(std::string &taskName)
  {
    if (m_readyNodes.empty())
      return false;

    taskName = m_readyNodes.front()->m_name;
    m_readyNodes.pop();

    return true;
  }

  void DependencyGraph::NotifyTaskCompletion(const std::string &taskName)
  {
    const std::list<DepNode*> &depList = m_adjacencyList[taskName];

    for (auto &it : depList)
    {
      --(it->m_depCount);

      //Do not add dummy end node
      if (it->m_name == c_endNode)
        continue;

      if (it->m_depCount == 0)
        m_readyNodes.push(it);
    }
  }

  void DependencyGraph::CompleteGraph(void)
  {
    //Find nodes without links and link them to end node
    for (auto &it : m_adjacencyList)
    {
      if (it.second.empty())
        it.second.push_back(&m_nodeMap[c_endNode]);
    }
  }

  bool DependencyGraph::AreAllTasksDone(void) const
  {
    return m_nodeMap.at(c_endNode).m_depCount == 0;
  }

  void DependencyGraph::_ResetCounts(void)
  {
    //First set every node count to 0
    for (auto &it : m_nodeMap)
      it.second.m_depCount = 0;

    //Increment dependency count
    for (auto &it : m_adjacencyList)
    {
      if (it.first == c_startingNode)
        continue;

      for (auto &listIt : it.second)
        ++(listIt->m_depCount);
    }
  }

  void DependencyGraph::_EnqueueFirstTasks(void)
  {
    const std::list<DepNode*> &depList = m_adjacencyList[c_startingNode];

    for (auto &it : depList)
      m_readyNodes.push(it);
  }

  size_t DependencyGraph::NodeHash::operator()(const DepNode &node) const
  {
    return std::hash<std::string>{}(node.m_name);
  }

  size_t DependencyGraph::NodeHash::operator()(const DepNode *node) const
  {
    return std::hash<std::string>{}(node->m_name);
  }

  DependencyGraph::DepNode::DepNode(const std::string &name) : m_name(name),
    m_depCount(0)
  {

  }

  bool DependencyGraph::DepNode::operator==(const DepNode &node) const
  {
    return this->m_name == node.m_name;
  }

  bool DependencyGraph::DepNode::operator==(const std::string &name) const
  {
    return this->m_name == name;
  }
}