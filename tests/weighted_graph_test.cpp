/**
 * @file weighted_graph_test.cpp
 * @author Mikhail Lozhnikov
 *
 * Тесты для класса graph::WeightedGraph.
 */

#include "test_core.hpp"
#include <weighted_graph.hpp>

using graph::WeightedGraph;

static void SimpleTest();
static void AnotherSimpleTest();

void TestWeightedGraph() {
  TestSuite suite("TestWeightedGraph");

  RUN_TEST(suite, SimpleTest);
  RUN_TEST(suite, AnotherSimpleTest);
}

static void SimpleTest() {
  WeightedGraph<int> graph;

  graph.AddVertex(1);

  REQUIRE(graph.HasVertex(1));

  graph.AddVertex(2);

  REQUIRE(graph.HasVertex(2));

  graph.AddEdge(1, 2, 3);

  REQUIRE(graph.HasEdge(1, 2));
}

static void AnotherSimpleTest() {
  WeightedGraph<int> graph;

  graph.AddVertex(1);

  REQUIRE(graph.HasVertex(1));

  graph.AddVertex(2);

  REQUIRE(graph.HasVertex(2));

  graph.AddEdge(1, 2, 3);

  REQUIRE(graph.HasEdge(1, 2));

  graph.RemoveVertex(2);

  REQUIRE(!graph.HasEdge(1, 2));
}
