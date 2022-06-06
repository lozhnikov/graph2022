/**
 * @file methods/methods.hpp
 * @author Eugene Yakimov
 *
 * Объявления функий для серверной части алгоритмов. Эти функции должны
 * считать JSON, который прислал клиент, выполнить алгоритм и отправить клиенту
 * JSON с результатом работы алгоритма.
 */

#ifndef METHODS_METHODS_HPP_
#define METHODS_METHODS_HPP_

namespace graph {
/* Сюда нужно вставить объявление серверной части алгоритма. */
/**
 * @brief Метод поиска мостов.
 *
 * @param input Входные данные в формате JSON.
 * @param output Выходные данные в формате JSON.
 * @return Функция возвращает 0 в случае успеха и отрицательное число
 * если входные данные заданы некорректно.
 *
 * Функция запускает алгоритм сортировки вставками, используя входные данные
 * в JSON формате. Результат также выдаётся в JSON формате.
*/

/**

 * @brief Метод топологической сортировки.

 * @brief Метод поиска точек сочленения.
 * @brief Метод поиска наибольшего паросочетания.
 *
 * @param input Входные данные в формате JSON.
 * @param output Выходные данные в формате JSON.
 * @return Функция возвращает 0 в случае успеха и отрицательное число
 * если входные данные заданы некорректно.
 *
 * Функция запускает алгоритм топологической сортировки, используя входные данные
 * в JSON формате. Результат также выдаётся в JSON формате.
 */
int TopologicalSortingMethod(const nlohmann::json& input,
                             nlohmann::json* output);
int CutPointsMethod(const nlohmann::json& input, nlohmann::json* output);
int FindBridgesMethod(const nlohmann::json& input, nlohmann::json* output);
int FindMaxMatchingMethod(const nlohmann::json& input, nlohmann::json* output);


/**
 * @brief Алгоритм Крускала.
 *
 * @param input Входные данные в формате JSON.
 * @param output Выходные данные в формате JSON.
 * @return Функция возвращает 0 в случае успеха и отрицательное число
 * если входные данные заданы некорректно.
 *
 * Функция запускает алгоритм сортировки вставками, используя входные данные
 * в JSON формате. Результат также выдаётся в JSON формате.
 */

int MaximalMethod(const nlohmann::json& input, nlohmann::json* output);

/* Конец вставки. */
}  // namespace graph

#endif  // METHODS_METHODS_HPP_
