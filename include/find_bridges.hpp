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
#include "graph.hpp"
#include "weighted_graph.hpp"
#include "iterators.hpp"


using std::vector;

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
 * @param p предок вершины p.
 * @param bridges вектор, в котором хранится ответ.
 */
template<class T>

void DeepFirstSearch(int* timer, std::map<size_t, bool>* used,
         std::map<size_t, int>* timerIn, std::map<size_t, int>* minTime,
         vector<vector<size_t>>* bridges, T graph, size_t v, size_t p = 0) {
    (*timerIn)[v] = (*timer)++;
    (*minTime)[v] = (*timer)++;
    (*used)[v] = true;


    for (auto neighbourId : graph.Edges(v)) {
        size_t to = neighbourId;
        if (to == p)  continue;
        if ((*used)[to]) {
            (*minTime)[v] = std::min((*minTime)[v], (*timerIn)[to]);
        } else {
            DeepFirstSearch(timer, used, timerIn, minTime, bridges, graph,
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
void FindBridges(T graph, vector<vector<size_t>>* bridges ) {
    int timer = 0;
    std::map<size_t, bool> used;
    std::map<size_t, int> timerIn;
    std::map<size_t, int> minTime;

    for (auto elem = graph.Vertices().begin();
         elem != graph.Vertices().end(); ++elem) {
        used[*elem] = false;
    }

    for (auto elem = graph.Vertices().begin();
         elem != graph.Vertices().end(); ++elem) {
        if (!used[*elem]) {
            DeepFirstSearch(&timer, &used, &timerIn, &minTime, bridges, graph,
                            *elem);
        }
    }
}


}  // namespace graph
#endif  // INCLUDE_FIND_BRIDGES_HPP_
