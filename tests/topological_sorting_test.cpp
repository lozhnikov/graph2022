/**
 * @file tests/topological_sorting_test.cpp
 * @author Eugene Yakimov
 *
 * Реализация набора тестов для алгоритма топологической сортировки.
 */

#include <topological_sorting.hpp>
#include <httplib.h>
#include <nlohmann/json.hpp>
#include "test_core.hpp"
#include "test.hpp"

static void SimpleTest(httplib::Client* cli);
static void RandomTest(httplib::Client* cli);


void TestTopologicalSorting(httplib::Client* cli) {
  TestSuite suite("TestTopologicalSorting");

  RUN_TEST_REMOTE(suite, cli, SimpleTest);
  RUN_TEST_REMOTE(suite, cli, RandomTest);
}

/**
 * @brief Простейший статический тест.
 *
 * @param cli Указатель на HTTP клиент.
 */
static void SimpleTest(httplib::Client* cli) {
    /*
     Библиотека nlohmann json позволяет преобразовать
     строку в объект nlohmann::json не только при помощи
     функции nlohmann::json::parse(), но и при помощи
     специального литерала _json. Если его поставить после строки
     в кавычках, то она конвертируется в json объект.
     
     R"(
     )" Так записывается строка, содержащая символы перевода строки
     в C++. Всё, что между скобками это символы строки. Перводы строк
     можно ставить просто как перевод строки в текстовом редактора
     (а не через \n).     
    */
  nlohmann::json input = R"(
  {
    "id": 1,
    "type": "oriented_graph",
    "size": 5,
    "vertices": [ 5, 1, 2, 4, 3 ],
    "numEdges": 4,
    "edges": [ [1,2], [2,3], [3,4], [4,5] ]
  }
)"_json;

/* Делаем POST запрос по адресу нашего метода на сервере.
    Метод dump() используется для преобразования JSON обратно в строку.
    (Можно было сразу строку передать). При передаче JSON данных
    необходимо поставить тип MIME "application/json".
    */
  httplib::Result res = cli->Post("/TopologicalSorting", input.dump(),
                                  "application/json");

    /* Используем метод parse() для преобразования строки ответа сервера
    (res->body) в объект JSON. */
  nlohmann::json output = nlohmann::json::parse(res->body);

    /* Проверка результатов сортировки. */
    REQUIRE_EQUAL(1, output["id"]);
    REQUIRE_EQUAL("oriented_graph", output["type"]);
    REQUIRE_EQUAL(5, output["size"]);

    std::vector<size_t>  result = output["simpledata"];

    bool success = (result == std::vector<size_t>{1, 2, 3, 4, 5}) ||
        (result == std::vector<size_t>{2, 1, 3, 4, 5}) ||
        (result == std::vector<size_t>{2, 3, 4, 5, 1}) ||
        (result == std::vector<size_t>{3, 2, 1, 4, 5}) ||
        (result == std::vector<size_t>{3, 4, 5, 2, 1}) ||
        (result == std::vector<size_t>{4, 3, 2, 1, 5}) ||
        (result == std::vector<size_t>{4, 5, 3, 2, 1}) ||
        (result == std::vector<size_t>{5, 4, 3, 2, 1});

    REQUIRE(success);
}


/**
 * @brief Простейший случайный тест.
 *
 *
 * @param cli Указатель на HTTP клиент.
 *
 * Функция используется для сокращения кода, необходимого для поддержки
 * различных типов данных.
 */


static void RandomTest(httplib::Client* cli) {
  // Число попыток.
  const int numTries = 10;

  // Используется для инициализации генератора случайных чисел.
  std::random_device rd;

  // Генератор случайных чисел.
  std::mt19937 gen(rd());

  // Распределение для количества элементов массива.
  std::uniform_int_distribution<size_t> arraySize(10, 50);

  for (int it = 0; it < numTries; it++) {
    // Получаем случайный размер массива, используя функцию распределения.
    size_t size = arraySize(gen);

    // Случайный номер вершины
    std::uniform_int_distribution<size_t> vert(1, size);

    nlohmann::json input;

    input["id"] = it;
    input["type"] = "oriented_graph";
    input["size"] = size;
    size_t numEdges = size / 2;
    input["numEdges"] = numEdges;

    // добавляем вершины случайным образом
    for (size_t i = 0; i < size; i++) {
      size_t k = vert(gen);
      for (size_t j = 0; j < i; j++) {
          if ((k == input["vertices"][j]) || (k == input["vertices"][0])) {
              k = vert(gen);
              j = 0;
          }
      }
      input["vertices"][i] = k;
    }

  // добавляем ребра случайный образом так, чтобы граф был ацикличным
  for (size_t i = 0; i < numEdges; i++) {
      input["edges"][i][0] = vert(gen);
      size_t k = vert(gen);
      for (size_t j = 0; j <= i; j++) {
        if ((k == input["edges"][j][0]) || (k == input["edges"][0][0])) {
            k = vert(gen);
            j = 0;
        }
      }
      input["edges"][i][1] = k;
  }

  // Отправляем данные на сервер POST запросом.
  httplib::Result res = cli->Post("/TopologicalSorting", input.dump(),
                                  "application/json");

  /* Используем метод parse() для преобразования строки ответа сервера
    (res->body) в объект JSON. */
  nlohmann::json output = nlohmann::json::parse(res->body);

  for (size_t i = 0; i < numEdges; i++) {
    for (size_t k = 0; k < 2; k++) {
      for (size_t j = 0; j < output["randomdata"].size(); j++) {
        if (input["edges"][i][k] == output["randomdata"][j][0]) {
            input["edges"][i][k] = output["randomdata"][j][1];
            break;
        }
      }
    }
  }

    // Проверка результатов сортировки.
    REQUIRE_EQUAL(size, output["size"]);
    REQUIRE_EQUAL(it, output["id"]);
    REQUIRE_EQUAL("oriented_graph", output["type"]);
    for (size_t i = 0; i < numEdges; i++) {
        REQUIRE(input["edges"][i][0] < input["edges"][i][1]);
    }
  }
}
