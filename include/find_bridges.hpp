/**
 * @file find_bridges.hpp
 * @author Eugene Semennikov
 *
 * Реализация метода поиска мостов.
 */
#ifndef INCLUDE_FIND_BRIDGES_HPP_
#define INCLUDE_FIND_BRIDGES_HPP_
#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include "graph.hpp"

namespace graph {
/**
 * @brief Поиск в глубину в графе.
 *
 * @tparam T тип графа.
 *
 * @param timer переменная, служащая для отслеживания времени входа в вершину.
 * @param used список использованных вершин при работе метода.
 * @param timerIn список времен входа в вершину.
 * @param minTime список минимальный времен входа в вершину.
 * @param v вершина, из которой идет поиск.
 * @param p предок вершины v.
 * @param bridges вектор, в котором хранится ответ.
 */
template<class T>

void DeepSearch(int* timer, std::unordered_set<size_t>* used,
         std::map<size_t, int>* timerIn, std::map<size_t, int>* minTime,
         std::vector<std::pair<size_t, size_t>>* bridges, T graph, size_t v, size_t p = 0) {
    (*timerIn)[v] = (*timer)++;
    (*minTime)[v] = (*timer)++;
    (*used).insert(v);


    for (auto neighbourId : graph.Edges(v)) {
        size_t to = neighbourId;
        if (to == p)  continue;
        if ((*used).find(to) != (*used).end()) {
            (*minTime)[v] = std::min((*minTime)[v], (*timerIn)[to]);
        } else {
            DeepSearch(timer, used, timerIn, minTime, bridges, graph,
                            to, v);
            (*minTime)[v] = std::min((*minTime)[v], (*minTime)[to]);
            if ((*minTime)[to] > (*timerIn)[v]) {
                (*bridges).push_back({v, to});
            }
        }
    }
}
/**
 * @brief Функция поиска мостов.
 *
 * @tparam T тип графа.
 *
 * @param bridges вектор, в котором хранится ответ.
 */
template<class T>
void FindBridges(T graph, std::vector<std::pair<size_t, size_t>>* bridges ) {
    int timer = 0;
    std::unordered_set<size_t> used;
    std::map<size_t, int> timerIn;
    std::map<size_t, int> minTime;

    for (size_t elem : graph.Vertices()) {
        if (used.find(elem) == used.end()) {
            DeepSearch(&timer, &used, &timerIn, &minTime, bridges, graph,
                            elem);
        }
    }
}


}  // namespace graph
#endif  // INCLUDE_FIND_BRIDGES_HPP_
