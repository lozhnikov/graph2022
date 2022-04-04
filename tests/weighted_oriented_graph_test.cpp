/**
 * @file weighted_oriented_graph_test.cpp
 * @author Mikhail Lozhnikov
 *
 * Тесты для класса graph::WeightedOrientedGraph.
 */

#include "test_core.hpp"
#include <weighted_oriented_graph.hpp>

using graph::WeightedOrientedGraph;

static void SimpleTest();
static void AnotherSimpleTest();

void TestWeightedOrientedGraph() {
  TestSuite suite("TestWeightedOrientedGraph");

  RUN_TEST(suite, SimpleTest);
  RUN_TEST(suite, AnotherSimpleTest);
}

static void SimpleTest() {
  WeightedOrientedGraph<int> graph;

  graph.AddVertex(1);

  REQUIRE(graph.HasVertex(1));

  graph.AddVertex(2);

  REQUIRE(graph.HasVertex(2));

  graph.AddEdge(1, 2, 3);

  REQUIRE(graph.HasEdge(1, 2));
}

static void AnotherSimpleTest() {
  WeightedOrientedGraph<int> graph;

  graph.AddVertex(1);

  REQUIRE(graph.HasVertex(1));

  graph.AddVertex(2);

  REQUIRE(graph.HasVertex(2));

  graph.AddEdge(1, 2, 3);

  REQUIRE(graph.HasEdge(1, 2));

  graph.RemoveVertex(2);

  REQUIRE(!graph.HasEdge(1, 2));
}
