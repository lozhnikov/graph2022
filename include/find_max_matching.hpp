/**
 * @file find_max_matching.hpp
 * @author Talpa Grigory
 *
 * Реализация метода поиска наибольшего паросочетания.
 */
#ifndef INCLUDE_FIND_MAX_MATCHING_HPP_
#define INCLUDE_FIND_MAX_MATCHING_HPP_
#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include <utility>
#include <unordered_set>
#include <cstring>
#include "graph.hpp"
#define M 500  // max number of vertices
size_t V, qh = 0, qt = 0;
int match[M], father[M];
size_t q[M], base[M];
bool inq[M], inb[M];

namespace graph {

/**
* @brief Поиск наименьшего предка.
*
* @param father массив хранящий предков нечетных вершин.
* @param match массив хранящий паросочетания.
*/  
size_t LCA(size_t root, size_t u, size_t v) {
        static bool inp[M];
        memset(inp, 0, sizeof(inp));
        while (1) {
            inp[u = base[u]] = true;
            if (u == root)
                break;
            u = static_cast<size_t>(father[match[u]]);
        }
        while (1) {
            if (inp[v = base[v]])
                return v;
            else
                v = static_cast<size_t>(father[match[v]]);
        }
    }
/**
* @brief Проход от вершины до базы.
*
*
* @param base массив базу цветка вершин.
* @param father массив хранящий предков нечетных вершин.
* @param match массив хранящий паросочетания.
* @param lca наименьший общий предок
*/ 
    void MarkBlossom(size_t lca, size_t u) {
        while (base[u] != lca) {
            size_t v = static_cast<size_t>(match[u]);
            inb[base[u]] = inb[base[v]] = true;
            u = static_cast<size_t>(father[v]);
            if (base[u] != lca)
                father[u] = static_cast<int>(v);
        }
    }
/**
 * @brief Сжатие цветка.
 *
 *
 * @param base массив базу цветка вершин.
 * @param father массив хранящий предков нечетных вершин.
 * @param lca наименьший общий предок
 */    
    void BlossomContraction(size_t s, size_t u, size_t v) {
        size_t lca = LCA(s, u, v);
        memset(inb, 0, sizeof(inb));
        MarkBlossom(lca, u);
        MarkBlossom(lca, v);
        if (base[u] != lca)
            father[u] = static_cast<int>(v);
        if (base[v] != lca)
            father[v] = static_cast<int>(u);
        for (size_t i = 0; i < V; i++)
            if (inb[base[i]]) {
                base[i] = lca;
                if (!inq[i])
                    inq[q[++qt] = i] = true;
            }
    }
template<class T>
/**
 * @brief Поиск увеличивающей цепи.
 *
 * @tparam T тип графа.
 *
 * @param base массив базу цветка вершин.
 * @param father массив хранящий предков нечетных вершин.
 * @param match массив хранящий паросочетания.
 */

int FindAugmentingPath(size_t s, T graph) {
    memset(inq, 0, sizeof(inq));
    memset(father, -1, sizeof(father));
    for (size_t i = 0; i < V; i++)
        base[i] = i;
    inq[q[qh = qt = 0] = s] = true;
    while (qh <= qt) {
        size_t u = q[qh++];
        if (graph.HasVertex(u)) {
        for (auto elem : graph.Edges(u)) {
            size_t v = elem;
            if (base[u] != base[v] && static_cast<size_t>(match[u]) != v) {
                if ((v == s) || (match[v] != -1 && father[match[v]] != -1)) {
                        BlossomContraction(s, u, v);
                    } else if (father[v] == -1) {
                        father[v] = static_cast<int>(u);
                        if (match[v] == -1)
                            return static_cast<int>(v);
                        else if (!inq[match[v]])
                            inq[q[++qt] = static_cast<size_t>(match[v])] = true;
                    }
            }
            }
            }
        }
        return -1;
    }
/**
 * @brief Увелечение пути вдоль цепи.
 *
 * @tparam T тип графа.
 *
 * @param father массив хранящий предков нечетных вершин.
 * @param match массив хранящий паросочетания.
 */
size_t AugmentPath(int t) {
    int u = t;
    size_t  v, w;
    while (u != -1) {
        v = static_cast<size_t>(father[u]);
        w = static_cast<size_t>(match[v]);
        match[v] = u;
        match[u] = static_cast<int>(v);
        u = static_cast<int>(w);
    }
    return t != -1;
}
    template<class T>
/**
 * @brief Алгоритм Эдондса.
 *
 * @tparam T тип графа.
 *
 * @param match массив хранящий паросочетания.
 * @param match_counts количество ребер в наибольшем парасочетании.
 */
size_t EdmondsBlossomAlgorithm(T graph) {
    size_t match_counts = 0;
    memset(match, -1, sizeof(match));

    for (size_t u = 0; u < V; u++) {
        if (match[u] == -1)
            match_counts += AugmentPath(FindAugmentingPath(u, graph));
    }
    return match_counts;
}
template<class T>
    /**
 * @brief Функция поиска наибольшего паросочетания.
 *
 * @tparam T тип графа.
 *
 * @param result вектор, в котором хранится ответ.
 */
void FindMaxMatching(T graph, std::vector<std::pair<size_t, size_t>>* result) {
    V = graph.NumVertices();
    size_t res = EdmondsBlossomAlgorithm(graph);
    if (!res)
        return;
    for (int i = 0; i < static_cast<int>(V); i++) {
            if (i < match[i]) {
(*result).push_back({static_cast<size_t>(i), static_cast<size_t>(match[i])});
            }
    }
}

}  // namespace graph
#endif  // INCLUDE_FIND_MAX_MATCHING_HPP_

