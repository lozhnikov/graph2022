/**
 * @file tests/cut_points_test.cpp
 * @author Daniil Morozov
 */

#include <cut_points.hpp>
#include <httplib.h>
#include <nlohmann/json.hpp>
#include "test_core.hpp"
#include "test.hpp"

static void SimpleTest(httplib::Client* cli);
static void RandomTest(httplib::Client* cli);

template<typename T>
static void RandomIntegerHelperTest(httplib::Client* cli, std::string type);

template<typename T>
static void RandomFloatingPointHelperTest(httplib::Client* cli,
                                          std::string type);

void TestCutPoints(httplib::Client* cli) {
  TestSuite suite("TestCutPoints");
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
    "type": "graph",
    "size": 3,
    "vertices": [ 1, 2, 3 ],
    "numEdges": 2,
    "edges": [[1,2],[2,3]]
  }
)"_json;

    /* Делаем POST запрос по адресу нашего метода на сервере.
    Метод dump() используется для преобразования JSON обратно в строку.
    (Можно было сразу строку передать). При передаче JSON данных
    необходимо поставить тип MIME "application/json".
    */
  httplib::Result res = cli->Post("/CutPoints", input.dump(),
                                  "application/json");
    /* Используем метод parse() для преобразования строки ответа сервера
    (res->body) в объект JSON. */
  nlohmann::json output = nlohmann::json::parse(res->body);
    REQUIRE_EQUAL(1, output["id"]);
    REQUIRE_EQUAL("graph", output["type"]);
    REQUIRE_EQUAL(3, output["size"]);
    REQUIRE_EQUAL(static_cast<size_t>(1), output["data"].size());
    REQUIRE_EQUAL(static_cast<size_t>(2), output["data"][0]);
}
/**
 * @brief Простейший случайный тест.
 *
 * @param cli Указатель на HTTP клиент.
 *
 * Функция используется для сокращения кода, необходимого для поддержки
 * различных типов данных.
 */
static void RandomTest(httplib::Client* cli) {
  // Число попыток.
  const int numTries = 100;
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
    std::uniform_int_distribution<size_t> vert(1, size-1);
    nlohmann::json input;
    input["id"] = it;
    input["type"] = "graph";
    input["size"] = size;
    size_t numEdges = 2*size-2;
    input["numEdges"] = numEdges;

    for (size_t i = 0; i < size; i++) {
      // Записываем элемент в JSON.
      input["vertices"][i] = i+1;
    }
    // Генирируем ребра случайный образом.
    size_t j = 2;
    for (size_t i = 0; i < numEdges-1; i+=2) {
      input["edges"][i][0] = vert(gen);
      input["edges"][i][1] = vert(gen);
      input["edges"][i+1][0] = 1;
      input["edges"][i+1][1] = j;
      j++;
    }
    // Добавляем точку сочленения
    input["edges"][numEdges-1][0] = size;
    input["edges"][numEdges-1][1] = size-1;
    /* Отправляем данные на сервер POST запросом. */
     httplib::Result res = cli->Post("/CutPoints", input.dump(),
        "application/json");
    /* Используем метод parse() для преобразования строки ответа сервера
    (res->body) в объект JSON. */
    nlohmann::json output = nlohmann::json::parse(res->body);

    /* Проверка результатов метода. */

    bool find = false;
    size_t i = 0;
    while ((i < output["data"].size())&&(!find)) {
      find = (output["data"].at(i) == size-1);
      i++;
    }
    REQUIRE_EQUAL(size, output["size"]);
    REQUIRE_EQUAL(it, output["id"]);
    REQUIRE_EQUAL("graph", output["type"]);
    REQUIRE_EQUAL(true, output["data"].size() <= size);
    // Добавленная точка должна быть в ответе.
    REQUIRE(find);
  }
}
