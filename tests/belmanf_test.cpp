/**
 * @file belmanf_test.cpp
 * @author Daniil Kalambetov
 *
 * Тесты для алгоритма Беллмана-Форда поиска кратчайшего пути.
 */

#include <belmanf.hpp>
#include <httplib.h>
#include <nlohmann/json.hpp>
#include "test_core.hpp"
#include "test.hpp"

using graph::WeightedOrientedGraph;
static void SimpleTest(httplib::Client* cli);
static void RandomTest(httplib::Client* cli);

static void RandomIntegerHelperTest(httplib::Client* cli, std::string type);

void TestBelmanf(httplib::Client* cli) {
  TestSuite suite("TestBelmanf");

  RUN_TEST_REMOTE(suite, cli, SimpleTest);
  RUN_TEST_REMOTE(suite, cli, RandomTest);
}

/**
* @brief Простейший статический тест.
*
* @param cli Указатель на HTTP клиент.
*/
static void SimpleTest(httplib::Client* cli) {
{
    nlohmann::json input = R"(
      {
        "id": 1,
        "type": "int",
        "size": [ 5, 5, 3 ],
        "data": [ 1, 0, 1, 2, 1, 2, 3, 3, 2, 4, 3, 4, 1, 4, 2 ]
      }
    )"_json;

    /* Делаем POST запрос по адресу нашего метода на сервере.
    Метод dump() используется для преобразования JSON обратно в строку.
    (Можно было сразу строку передать). При передаче JSON данных
    необходимо поставить тип MIME "application/json".
    */
    httplib::Result res = cli->Post("/Belmanf", input.dump(),
                                    "application/json");

    /* Используем метод parse() для преобразования строки ответа сервера
    (res->body) в объект JSON. */
    nlohmann::json output = nlohmann::json::parse(res->body);

    /* Проверка результатов сортировки. */
    REQUIRE_EQUAL(5, output["size"]);   // edges
    REQUIRE_EQUAL(1, output["id"]);
    REQUIRE_EQUAL("int", output["type"]);
    std::unordered_map<size_t , std::string> req;
    req[0] = "no";
    req[1] = "no";
    req[2] = "3";
    req[3] = "0";
    req[4] = "4";
    for (size_t i=0; i < 5; i++)
      REQUIRE_EQUAL(req[output["data"][i][0]], output["data"][i][1]);
    nlohmann::json input1 = R"(
      {
        "id": 1,
        "type": "int",
        "size": [ 0, 6, 3 ],
        "data": []
      }
    )"_json;
      httplib::Result res1 = cli->Post("/Belmanf", input1.dump(),
                                       "application/json");
      nlohmann::json output1 = nlohmann::json::parse(res1->body);

    REQUIRE_EQUAL(6, output1["size"]);   // edges
    REQUIRE_EQUAL(1, output1["id"]);
    REQUIRE_EQUAL("int", output1["type"]);
    REQUIRE_EQUAL(output1["data"], "no edges");
    }
  }

    /**
    * @brief Простейший случайный тест.
    *
    * @param cli Указатель на HTTP клиент.
    */
  static void RandomTest(httplib::Client* cli) {
      RandomIntegerHelperTest(cli, "int");
  }
  static void RandomIntegerHelperTest(httplib::Client* cli, std::string type) {
    // Число попыток.
    const int numTries = 0;
    // Используется для инициализации генератора случайных чисел.
    std::random_device rd;
    // Генератор случайных чисел.
    std::mt19937 gen(rd());
    // Распределение для количества элементов массива.
    std::uniform_int_distribution<size_t> arraySize(10, 100);
    // Распределение для элементов массива.
    std::uniform_int_distribution<int> elem(-100, 100);
    std::uniform_int_distribution<size_t> yesno(0, 1);

    for (int it = 0; it < numTries; it++) {
        // Получаем случайный размер массива, используя функцию распределения.
        size_t n = arraySize(gen);
        std::uniform_int_distribution<size_t> ver(0, n-1);
        size_t v = ver(gen);
        nlohmann::json input;
        std::vector<size_t> matrix(n*n);
        for (size_t i = 0; i < n; i++)
          for (size_t j = i+1; j < n; j++) {
            if ((yesno(gen)) || (j == i+1)) {
              matrix[i*n+j] = 1;
            } else {
              matrix[i*n+j] = 0;
            }
          }

        size_t size = 0;
        std::vector<int> cost(n*n);
        for (size_t i = 0; i < n; i++)
          for (size_t j = i+1; j < n; j++) {
            if (matrix[i*n+j] == 1) {
              cost[i*n+j] = elem(gen);
              input["data"][size*3] = cost[i*n+j];
              input["data"][size*3+1] = i;
              input["data"][size*3+2] = j;
              size++;
            } else {
              cost[i*n+j] = 0;
            }
          }
          // Записываем элемент в JSON.
        input["id"] = it;
        input["type"] = type;
        input["size"][0] = size;
        input["size"][1] = n;
        input["size"][2] = v;

      // Отправляем данные на сервер POST запросом.
      httplib::Result res = cli->Post("/Belmanf", input.dump(),
                                      "application/json");

      // Используем метод parse() для преобразования строки ответа сервера
      // (res->body) в объект JSON.
      nlohmann::json output = nlohmann::json::parse(res->body);
      // Проверка результатов сортировки.
      REQUIRE_EQUAL(n, output["size"]);
      REQUIRE_EQUAL(it, output["id"]);
      REQUIRE_EQUAL(type, output["type"]);
      graph::WeightedOrientedGraph<int> inputGraph;
      std::unordered_map<size_t, std::string> outputGraph;
      for (size_t i = 0; i < size; i++) {
        inputGraph.AddEdge(static_cast<size_t>(input["data"][i*3+1]),
                           static_cast<size_t>(input["data"][i*3+2]),
                           input["data"][i*3]);
        inputGraph.AddVertex(static_cast<size_t>(input["data"][i*3+1]));
        inputGraph.AddVertex(static_cast<size_t>(input["data"][i*3+2]));
      }

      for (size_t j = 0; j < n; j++)
        outputGraph[output["data"][j][0]] = output["data"][j][1];

      for (size_t neighbour1 : inputGraph.Edges(v)) {
        int flag1 = (inputGraph.EdgeWeight(v, neighbour1) >=
                    std::stoi(outputGraph[neighbour1]));

        REQUIRE_EQUAL(flag1, 1);
        for (size_t neighbour2 : inputGraph.Edges(neighbour1)) {
          if (outputGraph[neighbour2] != "no") {
            int flag2 = (inputGraph.EdgeWeight(v, neighbour1) +
                        inputGraph.EdgeWeight(neighbour1, neighbour2) >=
                        std::stoi(outputGraph[neighbour2]));
            REQUIRE_EQUAL(flag2, 1);
          }
        }
      }
  }
}
