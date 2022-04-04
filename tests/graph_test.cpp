/**
 * @file graph_test.cpp
 * @author Mikhail Lozhnikov
 *
 * Тесты для класса graph::Graph.
 */

#include <unordered_set>
#include <vector>
#include <algorithm>
#include <utility>
#include <random>
#include "test_core.hpp"
#include <graph.hpp>

using std::unordered_set;
using std::vector;
using std::pair;
using std::make_pair;

using std::out_of_range;
using std::random_device;
using std::mt19937;
using std::uniform_int_distribution;

using graph::Graph;

namespace {
/**
 * @brief Класс для хеширования std::pair<size_t, size_t>.
 */
class Hash {
 public:
  size_t operator()(pair<size_t, size_t> item) const noexcept {
    const size_t prime = 479001599;

    return item.first * prime + item.second;
  }
};

}  // namespace

static void VerticesAdditionTest();
static void EdgesAdditionTest();
static void VerticesRemovalTest();
static void EdgesRemovalTest();
static void GraphIteratorTest();
static void RandomAdditionTest();
static void RandomTest();

/**
 * @brief Основная функция для тестирования класса graph::Graph.
 */
void TestGraph() {
  TestSuite suite("TestGraph");

  RUN_TEST(suite, VerticesAdditionTest);
  RUN_TEST(suite, EdgesAdditionTest);
  RUN_TEST(suite, VerticesRemovalTest);
  RUN_TEST(suite, EdgesRemovalTest);
  RUN_TEST(suite, GraphIteratorTest);
  RUN_TEST(suite, RandomAdditionTest);
  RUN_TEST(suite, RandomTest);
}

/**
 * @brief Набор тестов на добавление вершины в граф.
 */
static void VerticesAdditionTest() {
  {
    Graph graph;

    REQUIRE_EQUAL(graph.HasVertex(1), false);

    graph.AddVertex(1);

    REQUIRE(graph.HasVertex(1));
  }

  {
    Graph graph;

    REQUIRE_EQUAL(graph.HasVertex(1), false);

    graph.AddVertex(1);
    graph.AddVertex(1);

    REQUIRE(graph.HasVertex(1));
  }

  {
    Graph graph;

    REQUIRE_EQUAL(graph.HasVertex(1), false);
    REQUIRE_EQUAL(graph.HasVertex(2), false);
    REQUIRE_EQUAL(graph.HasVertex(3), false);
    REQUIRE_THROW(graph.Edges(1), out_of_range);
    REQUIRE_THROW(graph.Edges(2), out_of_range);
    REQUIRE_THROW(graph.Edges(3), out_of_range);
    REQUIRE_THROW(graph.IncomingEdges(1), out_of_range);
    REQUIRE_THROW(graph.IncomingEdges(2), out_of_range);
    REQUIRE_THROW(graph.IncomingEdges(3), out_of_range);

    graph.AddVertex(1);

    REQUIRE(graph.HasVertex(1));
    REQUIRE_EQUAL(graph.HasVertex(2), false);
    REQUIRE_EQUAL(graph.HasVertex(3), false);
    REQUIRE_EQUAL(graph.Edges(1).size(), 0ul);
    REQUIRE_THROW(graph.Edges(2), out_of_range);
    REQUIRE_THROW(graph.Edges(3), out_of_range);
    REQUIRE_EQUAL(graph.IncomingEdges(1).size(), 0ul);
    REQUIRE_THROW(graph.IncomingEdges(2), out_of_range);
    REQUIRE_THROW(graph.IncomingEdges(3), out_of_range);

    graph.AddVertex(2);

    REQUIRE(graph.HasVertex(1));
    REQUIRE(graph.HasVertex(2));
    REQUIRE_EQUAL(graph.HasVertex(3), false);
    REQUIRE_EQUAL(graph.Edges(1).size(), 0ul);
    REQUIRE_EQUAL(graph.Edges(2).size(), 0ul);
    REQUIRE_THROW(graph.Edges(3), out_of_range);
    REQUIRE_EQUAL(graph.IncomingEdges(1).size(), 0ul);
    REQUIRE_EQUAL(graph.IncomingEdges(2).size(), 0ul);
    REQUIRE_THROW(graph.IncomingEdges(3), out_of_range);

    graph.AddVertex(3);

    REQUIRE(graph.HasVertex(1));
    REQUIRE(graph.HasVertex(2));
    REQUIRE(graph.HasVertex(3));
    REQUIRE_EQUAL(graph.Edges(1).size(), 0ul);
    REQUIRE_EQUAL(graph.Edges(2).size(), 0ul);
    REQUIRE_EQUAL(graph.Edges(3).size(), 0ul);
    REQUIRE_EQUAL(graph.IncomingEdges(1).size(), 0ul);
    REQUIRE_EQUAL(graph.IncomingEdges(2).size(), 0ul);
    REQUIRE_EQUAL(graph.IncomingEdges(3).size(), 0ul);
  }
}

/**
 * @brief Набор тестов на добавление ребра в граф.
 */
static void EdgesAdditionTest() {
  {
    Graph graph;

    REQUIRE_EQUAL(graph.HasVertex(1), false);
    REQUIRE_EQUAL(graph.HasVertex(2), false);
    REQUIRE_EQUAL(graph.HasEdge(1, 2), false);
    REQUIRE_EQUAL(graph.HasEdge(2, 1), false);

    graph.AddEdge(1, 2);

    REQUIRE(graph.HasVertex(1));
    REQUIRE(graph.HasVertex(2));
    REQUIRE(graph.HasEdge(1, 2));
    REQUIRE(graph.HasEdge(2, 1));
  }

  {
    Graph graph;

    REQUIRE_EQUAL(graph.HasVertex(1), false);
    REQUIRE_EQUAL(graph.HasVertex(2), false);
    REQUIRE_EQUAL(graph.HasEdge(1, 2), false);
    REQUIRE_EQUAL(graph.HasEdge(2, 1), false);

    graph.AddEdge(2, 1);

    REQUIRE(graph.HasVertex(1));
    REQUIRE(graph.HasVertex(2));
    REQUIRE(graph.HasEdge(1, 2));
    REQUIRE(graph.HasEdge(2, 1));
  }

  {
    Graph graph;

    REQUIRE_EQUAL(graph.HasVertex(1), false);
    REQUIRE_EQUAL(graph.HasVertex(2), false);
    REQUIRE_EQUAL(graph.HasEdge(1, 2), false);
    REQUIRE_EQUAL(graph.HasEdge(2, 1), false);

    graph.AddEdge(1, 2);
    graph.AddEdge(2, 1);

    REQUIRE(graph.HasVertex(1));
    REQUIRE(graph.HasVertex(2));
    REQUIRE(graph.HasEdge(1, 2));
    REQUIRE(graph.HasEdge(2, 1));
  }

  {
    Graph graph;

    graph.AddVertex(1);
    graph.AddVertex(2);
    graph.AddVertex(3);

    REQUIRE(graph.HasVertex(1));
    REQUIRE(graph.HasVertex(2));
    REQUIRE(graph.HasVertex(3));
    REQUIRE_EQUAL(graph.HasEdge(1, 2), false);
    REQUIRE_EQUAL(graph.HasEdge(2, 1), false);
    REQUIRE_EQUAL(graph.HasEdge(2, 3), false);
    REQUIRE_EQUAL(graph.HasEdge(3, 2), false);
    REQUIRE_EQUAL(graph.HasEdge(1, 3), false);
    REQUIRE_EQUAL(graph.HasEdge(3, 1), false);
    REQUIRE_EQUAL(graph.Edges(1).size(), 0ul);
    REQUIRE_EQUAL(graph.Edges(2).size(), 0ul);
    REQUIRE_EQUAL(graph.Edges(3).size(), 0ul);
    REQUIRE_EQUAL(graph.IncomingEdges(1).size(), 0ul);
    REQUIRE_EQUAL(graph.IncomingEdges(2).size(), 0ul);
    REQUIRE_EQUAL(graph.IncomingEdges(3).size(), 0ul);

    graph.AddEdge(1, 2);

    REQUIRE(graph.HasVertex(1));
    REQUIRE(graph.HasVertex(2));
    REQUIRE(graph.HasVertex(3));
    REQUIRE(graph.HasEdge(1, 2));
    REQUIRE(graph.HasEdge(2, 1));
    REQUIRE_EQUAL(graph.HasEdge(2, 3), false);
    REQUIRE_EQUAL(graph.HasEdge(3, 2), false);
    REQUIRE_EQUAL(graph.HasEdge(1, 3), false);
    REQUIRE_EQUAL(graph.HasEdge(3, 1), false);
    REQUIRE_EQUAL(graph.Edges(1).size(), 1ul);
    REQUIRE_EQUAL(graph.Edges(2).size(), 1ul);
    REQUIRE_EQUAL(graph.Edges(3).size(), 0ul);
    REQUIRE_EQUAL(graph.IncomingEdges(1).size(), 1ul);
    REQUIRE_EQUAL(graph.IncomingEdges(2).size(), 1ul);
    REQUIRE_EQUAL(graph.IncomingEdges(3).size(), 0ul);

    graph.AddEdge(3, 2);

    REQUIRE(graph.HasVertex(1));
    REQUIRE(graph.HasVertex(2));
    REQUIRE(graph.HasVertex(3));
    REQUIRE(graph.HasEdge(1, 2));
    REQUIRE(graph.HasEdge(2, 1));
    REQUIRE(graph.HasEdge(2, 3));
    REQUIRE(graph.HasEdge(3, 2));
    REQUIRE_EQUAL(graph.HasEdge(1, 3), false);
    REQUIRE_EQUAL(graph.HasEdge(3, 1), false);
    REQUIRE_EQUAL(graph.Edges(1).size(), 1ul);
    REQUIRE_EQUAL(graph.Edges(2).size(), 2ul);
    REQUIRE_EQUAL(graph.Edges(3).size(), 1ul);
    REQUIRE_EQUAL(graph.IncomingEdges(1).size(), 1ul);
    REQUIRE_EQUAL(graph.IncomingEdges(2).size(), 2ul);
    REQUIRE_EQUAL(graph.IncomingEdges(3).size(), 1ul);
  }
}

/**
 * @brief Набор тестов на удаление вершины из графа.
 */
static void VerticesRemovalTest() {
  {
    Graph graph;

    REQUIRE_EQUAL(graph.HasVertex(1), false);

    graph.RemoveVertex(1);

    REQUIRE_EQUAL(graph.HasVertex(1), false);
  }

  {
    Graph graph;

    REQUIRE_EQUAL(graph.HasVertex(1), false);

    graph.AddVertex(1);

    REQUIRE(graph.HasVertex(1));

    graph.RemoveVertex(1);

    REQUIRE_EQUAL(graph.HasVertex(1), false);
  }

  {
    Graph graph;

    graph.AddVertex(1);
    graph.AddVertex(2);
    graph.AddVertex(3);

    REQUIRE(graph.HasVertex(1));
    REQUIRE(graph.HasVertex(2));
    REQUIRE(graph.HasVertex(3));
    REQUIRE_EQUAL(graph.Edges(1).size(), 0ul);
    REQUIRE_EQUAL(graph.Edges(2).size(), 0ul);
    REQUIRE_EQUAL(graph.Edges(3).size(), 0ul);
    REQUIRE_EQUAL(graph.IncomingEdges(1).size(), 0ul);
    REQUIRE_EQUAL(graph.IncomingEdges(2).size(), 0ul);
    REQUIRE_EQUAL(graph.IncomingEdges(3).size(), 0ul);

    graph.RemoveVertex(1);

    REQUIRE_EQUAL(graph.HasVertex(1), false);
    REQUIRE(graph.HasVertex(2));
    REQUIRE(graph.HasVertex(3));
    REQUIRE_THROW(graph.Edges(1), out_of_range);
    REQUIRE_EQUAL(graph.Edges(2).size(), 0ul);
    REQUIRE_EQUAL(graph.Edges(3).size(), 0ul);
    REQUIRE_THROW(graph.IncomingEdges(1), out_of_range);
    REQUIRE_EQUAL(graph.IncomingEdges(2).size(), 0ul);
    REQUIRE_EQUAL(graph.IncomingEdges(3).size(), 0ul);

    graph.AddVertex(1);

    REQUIRE(graph.HasVertex(1));
    REQUIRE(graph.HasVertex(2));
    REQUIRE(graph.HasVertex(3));
    REQUIRE_EQUAL(graph.Edges(1).size(), 0ul);
    REQUIRE_EQUAL(graph.Edges(2).size(), 0ul);
    REQUIRE_EQUAL(graph.Edges(3).size(), 0ul);
    REQUIRE_EQUAL(graph.IncomingEdges(1).size(), 0ul);
    REQUIRE_EQUAL(graph.IncomingEdges(2).size(), 0ul);
    REQUIRE_EQUAL(graph.IncomingEdges(3).size(), 0ul);

    graph.AddEdge(1, 2);
    graph.AddEdge(1, 3);

    REQUIRE(graph.HasEdge(1, 2));
    REQUIRE(graph.HasEdge(2, 1));
    REQUIRE(graph.HasEdge(1, 3));
    REQUIRE(graph.HasEdge(3, 1));

    graph.RemoveVertex(2);

    REQUIRE(graph.HasVertex(1));
    REQUIRE_EQUAL(graph.HasVertex(2), false);
    REQUIRE(graph.HasVertex(3));
    REQUIRE_EQUAL(graph.HasEdge(1, 2), false);
    REQUIRE_EQUAL(graph.HasEdge(2, 1), false);
    REQUIRE(graph.HasEdge(1, 3));
    REQUIRE(graph.HasEdge(3, 1));

    graph.RemoveVertex(3);

    REQUIRE(graph.HasVertex(1));
    REQUIRE_EQUAL(graph.HasVertex(2), false);
    REQUIRE_EQUAL(graph.HasVertex(3), false);
    REQUIRE_EQUAL(graph.HasEdge(1, 2), false);
    REQUIRE_EQUAL(graph.HasEdge(2, 1), false);
    REQUIRE_EQUAL(graph.HasEdge(1, 3), false);
    REQUIRE_EQUAL(graph.HasEdge(3, 1), false);
  }
}

/**
 * @brief Набор тестов на удаление ребра из графа.
 */
static void EdgesRemovalTest() {
  {
    Graph graph;

    REQUIRE_EQUAL(graph.HasEdge(1, 2), false);
    REQUIRE_EQUAL(graph.HasEdge(2, 1), false);

    graph.RemoveEdge(1, 2);

    REQUIRE_EQUAL(graph.HasEdge(1, 2), false);
    REQUIRE_EQUAL(graph.HasEdge(2, 1), false);
  }

  {
    Graph graph;

    REQUIRE_EQUAL(graph.HasEdge(1, 2), false);
    REQUIRE_EQUAL(graph.HasEdge(2, 1), false);
    REQUIRE_EQUAL(graph.HasVertex(1), false);
    REQUIRE_EQUAL(graph.HasVertex(2), false);

    graph.AddEdge(1, 2);

    REQUIRE(graph.HasEdge(1, 2));
    REQUIRE(graph.HasEdge(2, 1));
    REQUIRE(graph.HasVertex(1));
    REQUIRE(graph.HasVertex(2));

    graph.RemoveEdge(1, 2);

    REQUIRE_EQUAL(graph.HasEdge(1, 2), false);
    REQUIRE_EQUAL(graph.HasEdge(2, 1), false);
    REQUIRE(graph.HasVertex(1));
    REQUIRE(graph.HasVertex(2));
  }

  {
    Graph graph;

    REQUIRE_EQUAL(graph.HasEdge(1, 2), false);
    REQUIRE_EQUAL(graph.HasEdge(2, 1), false);
    REQUIRE_EQUAL(graph.HasEdge(2, 3), false);
    REQUIRE_EQUAL(graph.HasEdge(3, 2), false);
    REQUIRE_EQUAL(graph.HasVertex(1), false);
    REQUIRE_EQUAL(graph.HasVertex(2), false);
    REQUIRE_EQUAL(graph.HasVertex(3), false);

    graph.AddEdge(1, 2);

    REQUIRE(graph.HasEdge(1, 2));
    REQUIRE(graph.HasEdge(2, 1));
    REQUIRE_EQUAL(graph.HasEdge(2, 3), false);
    REQUIRE_EQUAL(graph.HasEdge(3, 2), false);
    REQUIRE(graph.HasVertex(1));
    REQUIRE(graph.HasVertex(2));
    REQUIRE_EQUAL(graph.HasVertex(3), false);

    graph.AddEdge(3, 2);

    REQUIRE(graph.HasEdge(1, 2));
    REQUIRE(graph.HasEdge(2, 1));
    REQUIRE(graph.HasEdge(2, 3));
    REQUIRE(graph.HasEdge(3, 2));
    REQUIRE(graph.HasVertex(1));
    REQUIRE(graph.HasVertex(2));
    REQUIRE(graph.HasVertex(3));

    graph.RemoveEdge(1, 2);

    REQUIRE_EQUAL(graph.HasEdge(1, 2), false);
    REQUIRE_EQUAL(graph.HasEdge(2, 1), false);
    REQUIRE(graph.HasEdge(2, 3));
    REQUIRE(graph.HasEdge(3, 2));
    REQUIRE(graph.HasVertex(1));
    REQUIRE(graph.HasVertex(2));
    REQUIRE(graph.HasVertex(3));

    graph.RemoveEdge(2, 3);

    REQUIRE_EQUAL(graph.HasEdge(1, 2), false);
    REQUIRE_EQUAL(graph.HasEdge(2, 1), false);
    REQUIRE_EQUAL(graph.HasEdge(2, 3), false);
    REQUIRE_EQUAL(graph.HasEdge(3, 2), false);
    REQUIRE(graph.HasVertex(1));
    REQUIRE(graph.HasVertex(2));
    REQUIRE(graph.HasVertex(3));
  }
}

static void GraphIteratorTest() {
  {
    Graph graph;

    std::vector<size_t> vertices;

    for (size_t id : graph.Vertices()) {
      vertices.push_back(id);
    }

    REQUIRE(vertices.empty());
  }

  {
    Graph graph;

    graph.AddVertex(3);

    std::vector<size_t> vertices;

    for (size_t id : graph.Vertices()) {
      vertices.push_back(id);
    }

    REQUIRE_EQUAL(vertices, std::vector<size_t>({ 3 }));
  }

  {
    Graph graph;

    graph.AddVertex(1);
    graph.AddVertex(2);
    graph.AddVertex(3);
    graph.AddVertex(4);

    graph.AddEdge(1, 2);
    graph.AddEdge(3, 5);
    graph.AddEdge(10, 15);

    std::vector<size_t> vertices;

    for (size_t id : graph.Vertices()) {
      vertices.push_back(id);
    }

    std::sort(vertices.begin(), vertices.end());

    REQUIRE_EQUAL(vertices, std::vector<size_t>({ 1, 2, 3, 4, 5, 10, 15 }));
  }
}

/**
 * @brief Набор случайных тестов на добавление вершин и рёбер в граф.
 */
static void RandomAdditionTest() {
  const int numTries = 100;
  const int numCommands = 100;

  // Служит для инициализации генератора случайных чисел.
  random_device rd;
  // Генератор случайных чисел.
  mt19937 generator(rd());
  // Функция распределения случайных целых чисел от 0 до 1 включительно.
  uniform_int_distribution<int> commands(0, 1);
  /* Функция распределения случайных целых чисел от 0 до максимального
     значения size_t включительно. */
  uniform_int_distribution<size_t> ids;

  for (int i = 0; i < numTries; i++) {
    Graph graph;
    unordered_set<size_t> vertices;
    unordered_set<std::pair<size_t, size_t>, Hash> edges;

    for (int j = 0; j < numCommands; j++) {
      const int command = commands(generator);

      if (command == 0) {
        // Добавить вершину.
        const size_t id = ids(generator);

        vertices.insert(id);

        graph.AddVertex(id);
      } else if (command == 1) {
        // Добавить ребро.
        const size_t id1 = ids(generator);
        const size_t id2 = ids(generator);

        vertices.insert(id1);
        vertices.insert(id2);
        edges.insert(make_pair(id1, id2));
        edges.insert(make_pair(id2, id1));

        graph.AddEdge(id1, id2);
      } else {
        // Такой команды быть не должно.
        REQUIRE(false);
      }

      for (size_t id : vertices)
        REQUIRE(graph.HasVertex(id));

      for (auto [id1, id2] : edges) {
        REQUIRE(graph.HasEdge(id1, id2));
        REQUIRE(graph.HasEdge(id2, id1));
      }
    }

    // Проверка итератора по вершинам.
    size_t numIteratedVertices = 0;
    std::unordered_set<size_t> iteratedVertices;

    for (size_t id : graph.Vertices()) {
      iteratedVertices.insert(id);
      numIteratedVertices++;
    }

    REQUIRE_EQUAL(numIteratedVertices, iteratedVertices.size());
    REQUIRE(vertices == iteratedVertices);
  }
}

/**
 * @brief Набор случайных тестов на добавление и удаление вершин и рёбер в граф.
 */
static void RandomTest() {
  const int numTries = 100;
  const int numCommands = 200;
  const size_t maxId = 10;

  // Служит для инициализации генератора случайных чисел.
  random_device rd;
  // Генератор случайных чисел.
  mt19937 generator(rd());
  // Функция распределения случайных целых чисел от 0 до 3 включительно.
  uniform_int_distribution<int> commands(0, 3);
  // Функция распределения случайных целых чисел от 0 до maxId включительно.
  uniform_int_distribution<size_t> ids(0, maxId);

  for (int i = 0; i < numTries; i++) {
    Graph graph;
    unordered_set<size_t> vertices;
    unordered_set<std::pair<size_t, size_t>, Hash> edges;
    unordered_set<size_t> removedVertices;
    unordered_set<std::pair<size_t, size_t>, Hash> removedEdges;

    for (int j = 0; j < numCommands; j++) {
      const int command = commands(generator);

      if (command == 0) {
        // Добавить вершину.
        const size_t id = ids(generator);

        vertices.insert(id);
        removedVertices.erase(id);

        graph.AddVertex(id);
      } else if (command == 1) {
        // Добавить ребро.
        const size_t id1 = ids(generator);
        const size_t id2 = ids(generator);

        vertices.insert(id1);
        vertices.insert(id2);
        removedVertices.erase(id1);
        removedVertices.erase(id2);

        edges.insert(make_pair(id1, id2));
        edges.insert(make_pair(id2, id1));
        removedEdges.erase(make_pair(id1, id2));
        removedEdges.erase(make_pair(id2, id1));

        graph.AddEdge(id1, id2);
      } else if (command == 2) {
        // Удалить вершину.
        const size_t id = ids(generator);

        removedVertices.insert(id);
        vertices.erase(id);

        vector<pair<size_t, size_t>> edgesToRemove;
        for (auto [id1, id2] : edges) {
          if (id1 == id || id2 == id)
            edgesToRemove.emplace_back(id1, id2);
        }

        for (pair<size_t, size_t> ed : edgesToRemove) {
          edges.erase(ed);
          removedEdges.insert(ed);
        }

        graph.RemoveVertex(id);
      } else if (command == 3) {
        // Удалить ребро.
        const size_t id1 = ids(generator);
        const size_t id2 = ids(generator);

        removedEdges.insert(make_pair(id1, id2));
        removedEdges.insert(make_pair(id2, id1));
        edges.erase(make_pair(id1, id2));
        edges.erase(make_pair(id2, id1));

        graph.RemoveEdge(id1, id2);
      } else {
        // Такой команды быть не должно.
        REQUIRE(false);
      }

      for (size_t id : vertices)
        REQUIRE(graph.HasVertex(id));

      for (size_t id : removedVertices)
        REQUIRE_EQUAL(graph.HasVertex(id), false);

      for (auto [id1, id2] : edges) {
        REQUIRE(graph.HasEdge(id1, id2));
        REQUIRE(graph.HasEdge(id2, id1));
      }

      for (auto [id1, id2] : removedEdges) {
        REQUIRE_EQUAL(graph.HasEdge(id1, id2), false);
        REQUIRE_EQUAL(graph.HasEdge(id2, id1), false);
      }
    }

    // Проверка итератора по вершинам.
    size_t numIteratedVertices = 0;
    std::unordered_set<size_t> iteratedVertices;

    for (size_t id : graph.Vertices()) {
      iteratedVertices.insert(id);
      numIteratedVertices++;
    }

    REQUIRE_EQUAL(numIteratedVertices, iteratedVertices.size());
    REQUIRE(vertices == iteratedVertices);
  }
}
