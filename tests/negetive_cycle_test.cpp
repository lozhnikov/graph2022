/* @file tests/negetive_cycle_test.cpp
 * @author Vladislav Zyuzin
 *
 * Реализация набора тестов для метода поиска антицикла.
*/

#include <httplib.h>
#include <nlohmann/json.hpp>
#include "test_core.hpp"
#include "test.hpp"
#include <negetive_cycle.hpp>


static void SimpleTestNegCycle(httplib::Client* cli);
static void RandomTestNegCycle(httplib::Client* cli);

void TestNegCycle(httplib::Client* cli) {
    
    TestSuite suite("TestNegCycle");
    RUN_TEST_REMOTE(suite, cli, SimpleTestNegCycle);
    RUN_TEST_REMOTE(suite, cli, RandomTestNegCycle);
    
}

/**
 * @brief Простейший статический тест.
 *
 * @param cli Указатель на HTTP клиент.
**/

static void SimpleTestNegCycle(httplib::Client* cli) {
    {
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
        
        /*Первый тест*/
        // граф без ребер
        nlohmann::json input1 = R"(
        {
        "id": 3,
        "type": "weighted_oriented_graph",
        "TypeWeights": "int",
        "size": 7,
        "vertices": [ 4, 3, 2, 1, 6, 5, 7 ],
        "numEdges": 0,
        "edges": []
        }
    )"_json;
        
        /* Делаем POST запрос по адресу нашего метода на сервере.
        Метод dump() используется для преобразования JSON обратно в строку.
        (Можно было сразу строку передать). При передаче JSON данных
        необходимо поставить тип MIME "application/json".
        */
        httplib::Result res1 = cli->Post("/NegCycle", input1.dump(),
        "application/json");
        /* Используем метод parse() для преобразования строки ответа сервера
        (res->body) в объект JSON. */
        nlohmann::json output1 = nlohmann::json::parse(res1->body);
        
        /* Проверка результатов. */
        REQUIRE_EQUAL(7, output1["size"]);
        REQUIRE_EQUAL(3, output1["id"]);
        REQUIRE_EQUAL("weighted_oriented_graph", output1["type"]);
        
        /* Второй тест */
        // граф без вершин
        
        nlohmann::json input2 = R"(
        {
        "id": 7,
        "type": "weighted_oriented_graph",
        "TypeWeights": "int",
        "size": 0,
        "vertices": [],
        "numEdges": 0,
        "edges": []
        }
    )"_json;
        
        httplib::Result res2 = cli->Post("/NegCycle", input2.dump(),
        "application/json");
        nlohmann::json output2 = nlohmann::json::parse(res2->body);
        
        REQUIRE_EQUAL(0, output2["size"]);
        REQUIRE_EQUAL(7, output2["id"]);
        REQUIRE_EQUAL("weighted_oriented_graph", output2["type"]);
        REQUIRE_EQUAL(output2["data"].size(), static_cast<size_t>(0));
        
        /*Третий тест*/
        // есть антицикл 1->2->3->1 
        nlohmann::json input3 = R"(
        {
        "id": 1,
        "type": "weighted_oriented_graph",
        "TypeWeights": "int",
        "size": 3,
        "vertices": [ 1, 2, 3 ],
        "numEdges": 3,
        "edges": [[1,2,-1],[2,3,-1],[3,1,-1]]
        }
    )"_json;
        
        httplib::Result res3 = cli->Post("/NegCycle", input3.dump(),
        "application/json");
        nlohmann::json output3 = nlohmann::json::parse(res3->body);
        
        bool find = false;
        
        if ((output3["data"].at(0) == 1) && (output3["data"].at(1) == 2)
            && (output3["data"].at(2) == 3) && (output3["data"].at(3) == 1))  {
            find = true;
        }        
        if ((output3["data"].at(0) == 2) && (output3["data"].at(1) == 3)
            && (output3["data"].at(2) == 1) && (output3["data"].at(3) == 2)) {
            find = true;
        }        
        if ((output3["data"].at(0) == 3) && (output3["data"].at(1) == 1)
            && (output3["data"].at(2) == 2) && (output3["data"].at(3) == 3)) {
            find = true;
        }
        
        REQUIRE_EQUAL(3, output3["size"]);
        REQUIRE_EQUAL(1, output3["id"]);
        REQUIRE_EQUAL("weighted_oriented_graph", output3["type"]);
        REQUIRE_EQUAL(output3["data"].size(), static_cast<size_t>(4));
        REQUIRE_EQUAL(true, find);
        
        /*Четвертый тест*/
        //есть антицикл 1->2->3->4->7->1
        nlohmann::json input4 = R"(
        {
        "id": 1,
        "type": "weighted_oriented_graph",
        "TypeWeights": "int",
        "size": 7,
        "vertices": [ 4, 3, 2, 1, 6, 5, 7 ],
        "numEdges": 8,
        "edges": [[1,2,-5],[2,3,-1],[3,4,-1],[1,5,1],[7,1,-4],[7,6,1],[6,3,2],[4,7,-2]]
        }
    )"_json;
        
        httplib::Result res4 = cli->Post("/NegCycle", input4.dump(),
        "application/json");
        nlohmann::json output4 = nlohmann::json::parse(res4->body);
        
        bool find1 = false;
        
        if ((output4["data"].at(0) == 1) && (output4["data"].at(1) == 2)
            && (output4["data"].at(2) == 3) && (output4["data"].at(3) == 4) &&
           (output4["data"].at(4) == 7))  {
            find1 = true;
        }        
        if ((output4["data"].at(0) == 2) && (output4["data"].at(1) == 3)
            && (output4["data"].at(2) == 4) && (output4["data"].at(3) == 7)
            && (output4["data"].at(4) == 1)) {
            find1 = true;
        }        
        if ((output4["data"].at(0) == 3) && (output4["data"].at(1) == 4)
            && (output4["data"].at(2) == 7) && (output4["data"].at(3) == 1)
           && (output4["data"].at(4) == 2)) {
            find1 = true;
        }
        if ((output4["data"].at(0) == 4) && (output4["data"].at(1) == 7)
            && (output4["data"].at(2) == 1) && (output4["data"].at(3) == 2)
           && (output4["data"].at(4) == 3)) {
            find1 = true;
        }
        if ((output4["data"].at(0) == 7) && (output4["data"].at(1) == 1)
            && (output4["data"].at(2) == 2) && (output4["data"].at(3) == 3)
           && (output4["data"].at(4) == 4)) {
            find1 = true;
        }
        
        /* Проверка результатов. */
        REQUIRE_EQUAL(7, output4["size"]);
        REQUIRE_EQUAL(1, output4["id"]);
        REQUIRE_EQUAL("weighted_oriented_graph", output4["type"]);
        REQUIRE_EQUAL(output4["data"].size(), static_cast<size_t>(6));
        REQUIRE_EQUAL(true, find1);
        
        /*Пятый тест*/
        //есть антицикл 1->2->3->4->7->1, причем одно ребро имеет положительный вес
        nlohmann::json input5 = R"(
        {
        "id": 1,
        "type": "weighted_oriented_graph",
        "TypeWeights": "int",
        "size": 7,
        "vertices": [ 4, 1, 5, 6, 3, 2, 7 ],
        "numEdges": 8,
        "edges": [[1,2,-5],[2,3,-1],[3,4,-1],[1,5,1],[7,1,2],[7,6,1],[6,3,2],[4,7,-2]]
        }
    )"_json;
        
        httplib::Result res5 = cli->Post("/NegCycle", input5.dump(),
        "application/json");
        nlohmann::json output5 = nlohmann::json::parse(res5->body);
        
        bool find2 = false;
        
        if ((output5["data"].at(0) == 1) && (output5["data"].at(1) == 2)
            && (output5["data"].at(2) == 3) && (output5["data"].at(3) == 4) &&
           (output4["data"].at(4) == 7))  {
            find2 = true;
        }        
        if ((output5["data"].at(0) == 2) && (output5["data"].at(1) == 3)
            && (output5["data"].at(2) == 4) && (output5["data"].at(3) == 7)
            && (output5["data"].at(4) == 1)) {
            find2 = true;
        }        
        if ((output5["data"].at(0) == 3) && (output5["data"].at(1) == 4)
            && (output5["data"].at(2) == 7) && (output5["data"].at(3) == 1)
           && (output5["data"].at(4) == 2)) {
            find2 = true;
        }
        if ((output5["data"].at(0) == 4) && (output5["data"].at(1) == 7)
            && (output5["data"].at(2) == 1) && (output5["data"].at(3) == 2)
           && (output5["data"].at(4) == 3)) {
            find2 = true;
        }
        if ((output5["data"].at(0) == 7) && (output5["data"].at(1) == 1)
            && (output5["data"].at(2) == 2) && (output5["data"].at(3) == 3)
           && (output5["data"].at(4) == 4)) {
            find2 = true;
        }
        
        /* Проверка результатов. */
        REQUIRE_EQUAL(7, output5["size"]);
        REQUIRE_EQUAL(1, output5["id"]);
        REQUIRE_EQUAL("weighted_oriented_graph", output5["type"]);
        REQUIRE_EQUAL(output5["data"].size(), static_cast<size_t>(6));
        REQUIRE_EQUAL(true, find2);
        
        /*Шестой тест*/
        //нет антицикла
        
        nlohmann::json input6 = R"(
        {
        "id": 1,
        "type": "weighted_oriented_graph",
        "TypeWeights": "int",
        "size": 7,
        "vertices": [ 1, 6, 5, 4, 3, 2, 7 ],
        "numEdges": 8,
        "edges": [[1,2,5],[2,3,1],[3,4,-1],[1,5,1],[7,1,4],[7,6,1],[6,3,2],[4,7,2]]
        }
    )"_json;
        
        httplib::Result res6 = cli->Post("/NegCycle", input6.dump(),
        "application/json");
        nlohmann::json output6 = nlohmann::json::parse(res6->body);
        
        /* Проверка результатов. */
        REQUIRE_EQUAL(7, output6["size"]);
        REQUIRE_EQUAL(1, output6["id"]);
        REQUIRE_EQUAL("weighted_oriented_graph", output6["type"]);
        REQUIRE_EQUAL(output6["data"].size(), static_cast<size_t>(0));
    }
}

/**
 * @brief Случайный тест для целых чисел.
 *
 *
 * @param cli Указатель на HTTP клиент.
**/

static void RandomTestNegCycle(httplib::Client* cli) {
    
    const int numTries = 100; //число попыток
    std::random_device rd; // Используется для инициализации генератора случайных чисел.
    std::mt19937 gen(rd()); // Генератор случайных чисел.
    std::uniform_int_distribution<size_t> arraySize(5, 100); // Распределение для количества элементов массива.
    
    for (int it = 0; it < numTries; it++) {
        // Получаем случайный размер массива, используя функцию распределения.
        size_t size = arraySize(gen);
        // Случайный номер вершины
        std::uniform_int_distribution<size_t> vert(1, size - 2);
        nlohmann::json input;
        
        input["id"] = it;
        input["type"] = "weighted_oriented_graph";
        input["TypeWeights"] = "int";
        input["size"] = size;
        size_t numEdges = size + 1;
        input["numEdges"] = numEdges;
        
        for (size_t i = 0; i < size; i++) {
            // Записываем элемент в JSON.
            input["vertices"][i] = i + 1;
        }
        
        // Генирируем ребра случайный образом.        
        for (size_t i = 0; i < numEdges - 3; i++) {
            input["edges"][i][0] = vert(gen);
            input["edges"][i][1] = vert(gen);
            input["edges"][i][2] = 1 + rand() % 10; //вес ребра не превышал 10
        }
        
        // Добавляем антицикл в граф.        
        input["edges"][numEdges-3][0] =  1;
        input["edges"][numEdges-3][1] = 2;
        input["edges"][numEdges-3][2] = -1;
        input["edges"][numEdges-2][0] =  2;
        input["edges"][numEdges-2][1] = 3;
        input["edges"][numEdges-2][2] = -1; 
        input["edges"][numEdges-1][0] =  3;
        input["edges"][numEdges-1][1] = 1;
        input["edges"][numEdges-1][2] = -1; 
        
        // Отправляем данные на сервер POST запросом.        
        httplib::Result res = cli->Post("/NegCycle", input.dump(), "application/json");
        // Используем метод parse() для преобразования строки ответа сервера
        //(res->body) в объект JSON.
        nlohmann::json output = nlohmann::json::parse(res->body);
        
        // Проверка результатов метода.        
        bool find = false;
        
        if ((output["data"].at(0) == 1) && (output["data"].at(1) == 2)
            && (output["data"].at(2) == 3) && (output["data"].at(3) == 1))  {
            find = true;
        }        
        if ((output["data"].at(0) == 2) && (output["data"].at(1) == 3)
            && (output["data"].at(2) == 1) && (output["data"].at(3) == 2)) {
            find = true;
        }        
        if ((output["data"].at(0) == 3) && (output["data"].at(1) == 1)
            && (output["data"].at(2) == 2) && (output["data"].at(3) == 3)) {
            find = true;
        }
        
        REQUIRE_EQUAL(size, output["size"]);
        REQUIRE_EQUAL(it, output["id"]);
        REQUIRE_EQUAL("weighted_oriented_graph", output["type"]);
        // всего три вершины антицикла в данном графе
        REQUIRE_EQUAL(true, output["data"].size() == 4);
        // Добавленный антицикл должен быть в ответе.
        REQUIRE_EQUAL(true, find);
    }
}
