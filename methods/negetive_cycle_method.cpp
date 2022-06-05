/**
 * @file methods/negetive_cycle_method.cpp
 * @author Vladislav Zyuzin
 *
 * Файл содержит функцию, которая вызывает алгоритм поиска всех пар вершин таких, что между ними существует путь сколько угодно малой длины.
 * Функция принимает и возвращает данные в JSON формате.
 */

#include <string>
#include <nlohmann/json.hpp>
#include "negetive_cycle.hpp"

namespace graph {

template<class T>
static int NegCycleMethodHelper(const nlohmann::json& input, 
                                nlohmann::json* output, std::string type);
int NegCycleMethod(const nlohmann::json& input, nlohmann::json* output) {
    /* 
    С классом nlohmann::json можно работать как со словарём.
    Метод at() в отличие оператора [] не меняет объект, поэтому
    этот метод можно использовать с константными объектами. 
    */
    std::string type = input.at("type");
    std::string typeweight = input.at("TypeWeights");
    
    /* Пока реализована только поддержка для взвешенных ориентированных графов.*/    
    if (type == "weighted_oriented_graph") {
        if (typeweight == "int") {
            return NegCycleMethodHelper<WeightedOrientedGraph<int>>(input,
                                                                    output, type);
        } else if (typeweight == "float") {
            return NegCycleMethodHelper<WeightedOrientedGraph<float>>(input,
                                                                      output, type);
        } else if (typeweight == "double") {
            return NegCycleMethodHelper<WeightedOrientedGraph<double>>(input,
                                                                       output, type);
        } else if (typeweight == "long double") {
            return NegCycleMethodHelper<WeightedOrientedGraph<long double>>(input,
                                                                            output, type);
        }
    }
    return -1;
}

/**
 * @brief Метод поиска цикла отрицательного веса.
 *
 * @tparam T Тип графа.
 * @tparam Weight Тип веса ребер графа.
 *
 * @param input Входные данные в формате JSON.
 * @param output Выходные данные в формате JSON.
 * @param type Строковое представление типа данных мортируемых элементов.
 * @return Функция возвращает 0 в случае успеха и отрицательное число
 * если входные данные заданы некорректно.
 *
 * Функция запускает алгоритм сортировки вставками, используя входные данные
 * в JSON формате. Результат также выдаётся в JSON формате. Функция
 * используется для сокращения кода, необходимого для поддержки различных типов данных.
**/

template<class T>
static int NegCycleMethodHelper(const nlohmann::json& input,
                                nlohmann::json* output, std::string type) {
    (*output)["id"] = input.at("id");
    size_t size = input.at("size");
    size_t numEdges = input.at("numEdges");
    
    if (numEdges == 0) {
        (*output)["size"] = size;
        (*output)["type"] = type;
        return -1;
    }
    
    T graph;
    // path - вектор, в котором хранится ответ
    std::vector<size_t> path;
    
    for (size_t i = 0; i < size; i++) {
        /* Для словарей используется индекс в виде строки,
        а для массивов просто целое число типа size_t. */
        size_t el = input.at("vertices").at(i);
        if (el != 0) {
            graph.AddVertex(el);
        }
    }
    
    // Добавляем ребра
    for (size_t j = 0; j < numEdges; j++) {
        graph.AddEdge(input.at("edges").at(j).at(0), 
                      input.at("edges").at(j).at(1),
                      input.at("edges").at(j).at(2));
    }
    
    /* Здесь вызывается сам алгоритм поиска антицикла. */
    NegCycle(size, graph, &path);
    
    /* Сохраняем в ответе результат работы алгоритма. */
    (*output)["size"] = size;
    (*output)["type"] = type;
    
    if (path.size() == 0) {
        (*output)["data"] = nullptr;
    } else {
        for (size_t i = 0; i < path.size(); i++) {
            (*output)["data"][i] = path[i];
        }
    }
    
    return 0;
}
}  // namespace graph
