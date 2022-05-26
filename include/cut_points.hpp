#ifndef CUT_POINTS
#define CUT_POINTS

#include "graph.hpp"
#include "oriented_graph.hpp"
#include "weighted_graph.hpp"
#include "weighted_oriented_graph.hpp"
#include "iterators.hpp"
#include <vector>
#include <map>
#include <unordered_set>

namespace graph {

template<class T>

void Dfs_Cut_Points(int* timer, std::unordered_set<size_t>* used, std::map<size_t, int>* entry_time, std::map<size_t, int>* min_of_time, std::vector<size_t>* points, T graph, size_t v, size_t p = static_cast<size_t>(-1)) {

  (*used).insert(v);
  (*entry_time)[v] = (*timer)++;
  (*min_of_time)[v] = (*timer)++;

  int children = 0;
  for (size_t to : graph.IncomingEdges(v)) {
    if (to == p)  
      continue;
    if ((*used).find(to) != (*used).end())
      (*min_of_time)[v] = std::min((*min_of_time)[v], (*entry_time)[to]);
    else {
      Dfs_Cut_Points(timer, used, entry_time, min_of_time, points, graph, to, v);
      (*min_of_time)[v] = std::min ((*min_of_time)[v], (*min_of_time)[to]);
      if ((*min_of_time)[to] >= (*entry_time)[v] && p != static_cast<size_t>(-1))
        (*points).push_back(v);
      ++children;
    }
  }
  if (p == static_cast<size_t>(-1) && children > 1)
    (*points).push_back(v);
}
 
template<class T>
void CutPoints(T graph, std::vector<size_t>* points) {
  int timer = 0;
  std::unordered_set<size_t> used;
  std::map<size_t, int> entry_time;
  std::map<size_t, int> min_of_time;
    
  for (size_t v : graph.Vertices()) {
    if (used.find(v) == (used).end()) {
      Dfs_Cut_Points(&timer, &used, &entry_time, &min_of_time, points, graph, v);
    }
  }
}
} // namespase graph
#endif // CUT_POINTS