/**
 * @file topological_sorting.hpp
 * @author Eugene Yakimov
 *
 * Реализация метода топологической сортировки.
 */

#ifndef INCLUDE_TOPOLOGICAL_SORTING_HPP_
#define INCLUDE_TOPOLOGICAL_SORTING_HPP_

#include <algorithm>
#include <iostream>
#include <map>
#include "graph.hpp"
#include <vector>
#include "oriented_graph.hpp"
#include "iterators.hpp"
#include <unordered_set>


namespace graph {

/**
 * @brief Обход графа в глубину.
 *
 * @tparam T тип графа.
 *
 * @param used список вершин, в которых уже побывали.
 * @param v текущая вершина в обходе..
 * @param vertice вектор-ответ.
 */
    
template<class T>

void Dfs(std::unordered_set<size_t> *used, std::vector<size_t>* vertice,
                             const T& graph, size_t v) {
    used->insert(v); // помечаем вершину посещенной
    for (size_t to : graph.IncomingEdges(v)) { // пробегаем все вершины, достижимые за 1 шаг из v
      if ((*used).find(to) == (*used).end()) {
      Dfs(used, vertice, graph, to); // если они не были посещены, то посещаем их
      }
    }
    (*vertice).push_back (v); // добавляем вершину в конец списка
}

/**
 * @brief Сама топологическая сортировка.
 *
 * @tparam T тип графа.
 *
 * @param points вектор-ответ.
 */

template<class T>
    
void TopologicalSorting(const T& graph, std::vector<size_t>* vertice) {
    std::unordered_set<size_t> used;
    
    for (size_t v : graph.Vertices()) {
        if (used.find(v) == (used).end()) {
          Dfs(&used, vertice, graph, v); // если в вершине ещё не были
        }  
    }
    reverse ((*vertice).begin(), (*vertice).end()); // обращаем массив
}

}  // namespace graph
#endif // TOPOLOGICAL_SORTING