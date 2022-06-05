/**
 * @file include/negetive_cycle.hpp
 * @author Vladislav Zyuzin
 *
 * Реализация метода поиска антицикла.
**/
#ifndef NEGETIVE_CYCLE_HPP_
#define NEGETIVE_CYCLE_HPP_
#include <iostream>
#include <unordered_map>
#include <unordered_set>
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
template<class T>
void NegCycle(size_t size, const T& graph, std::vector<size_t>* path) {
    // заводим массив расстояний от вершины v до других вершин
    std::unordered_map<size_t, typename T::WeightType> d;
    // для каждой вершины антицикла будем хранить его предка
    std::unordered_map<size_t, size_t> p;
    // индикатор отсутствия антицикла
    // (нумерация вершин начинается как мининимум с 1)
    size_t x = 0, j = 0;
    
    /* Запускаем алгоритм Форда-Беллмана */
    for (size_t i = 0; i < size; i++) {
        x = 0;
        for (size_t elem : graph.Vertices()) {
            size_t v = elem;
            if (j == 0) {
                d[v] = 0;
            }
            for (auto neighbourId : graph.Edges(v)) {
                size_t to = neighbourId;
                auto it1 = d.find(to);
                std::unordered_map<size_t, size_t>::iterator it;
                if (it1 == d.end()) {
                    d[to] = d[v] + graph.EdgeWeight(v, to);
                    it = p.find(to);
                    if (it == p.end()) {
                        p.insert({to, v});
                    } else {
                        p[to] = v;
                    }
                } else {
                    if (d[to] > d[v] + graph.EdgeWeight(v, to)) {
                        d[to] = d[v] + graph.EdgeWeight(v, to);
                        it = p.find(to);
                        if (it == p.end()) {
                            p.insert({to, v});
                        } else {
                            p[to] = v;
                        }
                        x = to;
                    }
                }
            }
            j++;
        }
    }
    
    if (x == 0) {
        (*path) = std::vector<size_t>();
    } else {
        size_t y = x;
        for (size_t i = 0; i < p.size(); ++i) {
            y = p[y];
        }
        for (size_t tmp = y; ; tmp = p[tmp]) {
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
