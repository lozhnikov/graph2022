/**
 * @file include/negetive_cycle.hpp
 * @author Vladislav Zyuzin
 *
 * Реализация метода поиска антицикла.
 */

#ifndef NEGETIVE_CYCLE_HPP_
#define NEGETIVE_CYCLE_HPP_

#include <iostream>
#include <vector>
#include <algorithm>
#include "iterators.hpp"
#include <cstddef>
#include "weighted_oriented_graph.hpp"

namespace graph {
    
/**
 * @brief Алгоритм поиска отрицательного цикла в графе.
 *
 * @tparam T Тип графа.
 * @tparam Weight Тип веса ребер графа.
 *
 * @param path Искомый путь (антицикл).
 * @param v Вершина, в которой мы находимся.
 * @param p предок.
 *
**/
template<class T, typename Weight>
void NegCycle(size_t size, size_t min, T* graph, Weight inf, 
              std::vector<size_t>* path) {
    std::vector<Weight> d (size, inf); // для сравнение весов ребер
    std::vector<size_t> p (size, 0); // для хранения вершин, по которым надо пройтись 
                                     //(т.е. антицикл содержит эти вершины)
    size_t x = 0; //индикатор отсутствия антицикла (нумерация вершин начинается с 1)
    
    /* Запускаем алгоритм Форда-Беллмана */
    for (size_t i = 0; i < size; i++) {
        x = 0;
        for (size_t elem : (*graph).Vertices()) {
            size_t v = elem;
            for (auto neighbourId : (*graph).Edges(v)) {
                size_t to = neighbourId;
                if ((d[to - min] - d[v - min] - (*graph).EdgeWeight(v, to)) > 10e-6) {
                    if (-inf - (*graph).EdgeWeight(v, to) > 10e-6) {     
                        d[to - min] = -inf;
                    } else {
                        d[to - min] = d[v - min] + (*graph).EdgeWeight(v, to);
                    }
                    p[to - min] = v;
                    x = to;
                }
            }
        }
    }
    
    if (x == 0) {
        (*path) = std::vector<size_t>();    
    } else {
        size_t y = x;
        for (size_t i = 0; i < size; ++i) {
            if (y >= min) {
                y = p[y - min];
            } else {
                y = min;
            }
        }
        for (size_t tmp = y; ; tmp = p[tmp - min]) {
            (*path).push_back(tmp);
            if (tmp == y && (*path).size() > 1) {
                break;
            }
        }
        
        reverse((*path).begin(), (*path).end());
    }
}
}  // namespace graph

#endif  // NEGETIVE_CYCLE_HPP_
