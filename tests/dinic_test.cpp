/**
 * @file tests/find_bridges_test.cpp
 * @author Denis Maslenikov
 *
 * Реализация набора тестов для метода поиска мостов.
 */
#include <dinic.hpp>
#include <httplib.h>
#include <nlohmann/json.hpp>
#include "test_core.hpp"
#include "test.hpp"

static void SimpleTest(httplib::Client *cli);
static void RandomTest(httplib::Client *cli);

void TestDinic(httplib::Client *cli) {
  TestSuite suite("TestDinic");

  RUN_TEST_REMOTE(suite, cli, SimpleTest);
  RUN_TEST_REMOTE(suite, cli, RandomTest);
}

static void SimpleTest(httplib::Client *cli) {
  {
  /*первый тест*/
    nlohmann::json input = R"(
  {
    "id": 0,
    "type": "weighted_oriented_graph",
    "size": 4,
    "vertices": [1, 2, 3, 4],
    "numEdges": 3,
    "start": 1,
    "theEnd": 4,
    "edges": [[1, 2, 1], [2, 3, 1], [1, 3, 1]]
  }
)"_json;


    /* Делаем POST запрос по адресу нашего метода на сервере.*/
    httplib::Result res = cli->Post("/Dinic", input.dump(),
        "application/json");

    /* Используем метод parse() для преобразования строки ответа сервера
    (res->body) в объект JSON. */
    nlohmann::json output = nlohmann::json::parse(res->body);

    /* Проверка результатов. */
    REQUIRE_EQUAL(4, output["size"]);
    REQUIRE_EQUAL(0, output["id"]);
    REQUIRE_EQUAL(3, output["numEdges"]);
    REQUIRE_EQUAL("weighted_oriented_graph", output["type"]);
    for (size_t i = 0; i < output.at("numEdges"); i++) {// Перебор ребер
        //output.at("edges").at(i).at(0);// Откуда
        //output.at("edges").at(i).at(1);// Куда
        REQUIRE_EQUAL(0,output.at("edges").at(i).at(2));// Вес
    }

    /*первый тест*/
    nlohmann::json input1 = R"(
  {
    "id": 1,
    "type": "weighted_oriented_graph",
    "size": 4,
    "vertices": [1, 2, 3, 4],
    "numEdges": 6,
    "start": 1,
    "theEnd": 4,
    "edges": [[1,2,5],[1,3,2],[2,3,5],[3,1,3],[3,4,8],[1,4,1]]
  }
)"_json;

    httplib::Result res1 = cli->Post("/Dinic", input1.dump(),
        "application/json");
    nlohmann::json output1 = nlohmann::json::parse(res1->body);

    /* Проверка результатов. */
    REQUIRE_EQUAL(4, output1["size"]);
    REQUIRE_EQUAL(1, output1["id"]);
    REQUIRE_EQUAL(6, output1["numEdges"]);
    REQUIRE_EQUAL("weighted_oriented_graph", output1["type"]);
    for (size_t i = 0; i < output1.at("numEdges"); i++) {// Перебор ребер
        if((output1.at("edges").at(i).at(0) == 1)&&(output1.at("edges").at(i).at(1) == 2)){
            REQUIRE_EQUAL(5,output1.at("edges").at(i).at(2));
        }
        if((output1.at("edges").at(i).at(0) == 1)&&(output1.at("edges").at(i).at(1) == 3)){
            REQUIRE_EQUAL(2,output1.at("edges").at(i).at(2));
        }
        if((output1.at("edges").at(i).at(0) == 2)&&(output1.at("edges").at(i).at(1) == 3)){
            REQUIRE_EQUAL(5,output1.at("edges").at(i).at(2));
        }
        if((output1.at("edges").at(i).at(0) == 3)&&(output1.at("edges").at(i).at(1) == 1)){
            REQUIRE_EQUAL(0,output1.at("edges").at(i).at(2));
        }
        if((output1.at("edges").at(i).at(0) == 3)&&(output1.at("edges").at(i).at(1) == 4)){
            REQUIRE_EQUAL(7,output1.at("edges").at(i).at(2));
        }
        if((output1.at("edges").at(i).at(0) == 1)&&(output1.at("edges").at(i).at(1) == 4)){
            REQUIRE_EQUAL(1,output1.at("edges").at(i).at(2));
        }
    }

    /*третий тест*/
    nlohmann::json input2 = R"(
  {
    "id": 2,
    "type": "weighted_oriented_graph",
    "size": 8,
    "vertices": [1, 2, 3, 4, 5, 6, 7, 8],
    "numEdges": 10,
    "start": 1,
    "theEnd": 4,
    "edges": [[1,2,1],[2,3,1],[3,2,1],[3,4,1],[1,5,1],[5,6,1],[6,3,1],[2,7,1],[7,8,1],[8,4,1]]
  }
)"_json;

    httplib::Result res2 = cli->Post("/Dinic", input2.dump(),
        "application/json");
    nlohmann::json output2 = nlohmann::json::parse(res2->body);

    /* Проверка результатов. */
    REQUIRE_EQUAL(8, output2["size"]);
    REQUIRE_EQUAL(2, output2["id"]);
    REQUIRE_EQUAL(10, output2["numEdges"]);
    REQUIRE_EQUAL("weighted_oriented_graph", output2["type"]);
    for (size_t i = 0; i < output2.at("numEdges"); i++) {// Перебор ребер
        if(!(((output2.at("edges").at(i).at(0) == 2) && (output2.at("edges").at(i).at(1) == 3)) ||
                ((output2.at("edges").at(i).at(0) == 3) && (output2.at("edges").at(i).at(1) == 2)))) {
            REQUIRE_EQUAL(1,output2.at("edges").at(i).at(2));
        }
    }
  }
}

/*Случайый тест*/
static void RandomTest(httplib::Client *cli) {
  const int numTries = 10;
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<size_t> arraySize(15, 30);
  const size_t size = arraySize(gen);
  std::uniform_int_distribution<size_t> vert1(1, 10);
  std::uniform_int_distribution<size_t> vert2(11, size);
  std::uniform_int_distribution<size_t> randommin(1, 10);
  std::uniform_int_distribution<size_t> weight(1, 30);

  for (int id = 0; id < numTries; id++) {
    nlohmann::json input;

    input["id"] = id;
    input["type"] = "weighted_oriented_graph";
    input["size"] = size;
    input["numEdges"] = 2*size+1;


    for (size_t i = 0; i < size; i++) {
      input["vertices"][i] = i + 1;
    }
    input["start"] = 1;
    input["theEnd"] = size;
    size_t n = weight(gen);
    input["edges"][0][0] = 10;
    input["edges"][0][1] = 11;
    input["edges"][0][2] = n;
    for (size_t i = 1; i < 11; i++) {
        input["edges"][i][0] = vert1(gen);
        input["edges"][i][1] = vert1(gen);
        input["edges"][i][2] = weight(gen);
    }
    for (size_t i = 11; i < 2 * size + 1; i++) {
        input["edges"][i][0] = vert2(gen);
        input["edges"][i][1] = vert2(gen);
        input["edges"][i][2] = weight(gen);
    }

     httplib::Result res = cli->Post("/Dinic", input.dump(),
        "application/json");

    nlohmann::json output = nlohmann::json::parse(res->body);

    size_t s = 0, t = 0;
    for (size_t i = 0; i < output.at("numEdges"); i++) {
      if(output.at("edges").at(i).at(0) == 1){
        s += static_cast<size_t>(output.at("edges").at(i).at(2));
      }
      if(output.at("edges").at(i).at(1) == size){
        t += static_cast<size_t>(output.at("edges").at(i).at(2));
      }
    }

    /* Проверка результатов. */
    REQUIRE_EQUAL("weighted_oriented_graph", output["type"]);
    REQUIRE_EQUAL(size, output["size"]);
    REQUIRE_EQUAL(id, output["id"]);
    REQUIRE_EQUAL(s, t);
    for (size_t i = 0; i < 2 * size + 1; i++)
	    if (output["edges"][i][0] == 10 &&
			    output["edges"][i][0] == 11)
		    REQUIRE_EQUAL(s, output["edges"][i][2]);
    REQUIRE_EQUAL((s <= n), true);
  }
}
