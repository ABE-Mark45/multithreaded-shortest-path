#include "graph/graph.h"
#include <gtest/gtest.h>
#include <random>
#include <vector>
#include "batch/Batch.h"
#include "coordinator/AllPairsCoordinator.h"
#include "coordinator/CachedBFSCoordinator.h"
#include "processor/BFSProcessor.h"
#include "processor/CachedBFSProcessor.h"

namespace {
const std::vector<std::pair<graph::NodeIdType, graph::NodeIdType>> edges{
    {0, 1},   {0, 2},   {0, 13},  {0, 15},  {0, 16},  {0, 21},  {0, 28},
    {1, 10},  {1, 13},  {1, 24},  {1, 26},  {1, 27},  {2, 4},   {2, 13},
    {2, 15},  {2, 21},  {3, 5},   {3, 12},  {3, 13},  {3, 15},  {3, 27},
    {4, 18},  {4, 19},  {4, 27},  {5, 1},   {5, 4},   {5, 14},  {5, 16},
    {5, 17},  {6, 2},   {6, 10},  {6, 18},  {6, 22},  {7, 9},   {7, 13},
    {8, 13},  {8, 15},  {8, 20},  {8, 21},  {8, 25},  {9, 8},   {9, 10},
    {9, 13},  {9, 21},  {9, 24},  {10, 3},  {10, 6},  {10, 28}, {11, 5},
    {11, 17}, {11, 24}, {12, 5},  {12, 17}, {13, 6},  {13, 12}, {13, 22},
    {13, 29}, {14, 4},  {14, 10}, {14, 27}, {15, 0},  {15, 1},  {15, 11},
    {15, 24}, {16, 0},  {16, 3},  {16, 9},  {16, 21}, {16, 25}, {17, 2},
    {17, 5},  {17, 6},  {17, 7},  {17, 8},  {17, 13}, {17, 28}, {18, 0},
    {18, 13}, {18, 21}, {18, 26}, {18, 28}, {20, 4},  {20, 9},  {20, 22},
    {21, 2},  {21, 6},  {21, 16}, {22, 4},  {22, 6},  {22, 7},  {22, 18},
    {22, 25}, {23, 14}, {23, 27}, {24, 0},  {24, 1},  {24, 2},  {24, 21},
    {25, 12}, {25, 17}, {25, 18}, {25, 19}, {25, 28}, {26, 1},  {26, 2},
    {27, 4},  {27, 8},  {27, 10}, {27, 13}, {27, 22}, {27, 25}, {28, 0},
    {28, 2},  {28, 14}, {28, 17}, {28, 18}, {28, 20}, {29, 8},  {29, 20},
    {29, 21}, {29, 28}};
const std::vector<std::vector<graph::DistanceType>> distances{
    {
        0, 1, 1, 2, 2, 3, 2, 3, 3,  2, 2, 2, 2, 1, 2,
        1, 1, 2, 2, 3, 2, 1, 2, -1, 2, 2, 2, 2, 1, 2,
    },
    {
        2, 0, 2, 2, 2, 3, 2, 3, 2,  4, 1, 4, 2, 1, 3,
        3, 3, 3, 3, 3, 3, 2, 2, -1, 1, 2, 1, 1, 2, 2,
    },
    {
        2, 2, 0, 3, 1, 3, 2, 3, 3,  3, 3, 2, 2, 1, 4,
        1, 2, 3, 2, 2, 3, 1, 2, -1, 2, 3, 3, 2, 3, 2,
    },
    {
        2, 2, 3, 0, 2, 1, 2, 3, 2,  3, 2, 2, 1, 1, 2,
        1, 2, 2, 3, 3, 3, 3, 2, -1, 2, 2, 3, 1, 3, 2,
    },
    {
        2, 3, 3, 3, 0, 4, 3, 3, 2,  4, 2, 4, 3, 2, 3,
        3, 3, 3, 1, 1, 3, 2, 2, -1, 4, 2, 2, 1, 2, 3,
    },
    {
        2, 1, 2, 2, 1, 0, 2, 2, 2,  2, 2, 4, 3, 2, 1,
        3, 1, 1, 2, 2, 3, 2, 3, -1, 2, 2, 2, 2, 2, 3,
    },
    {
        2, 3, 1, 2, 2, 3, 0, 2, 4,  3, 1, 3, 3, 2, 3,
        2, 3, 3, 1, 3, 3, 2, 1, -1, 3, 2, 2, 3, 2, 3,
    },
    {
        3, 3, 3, 3, 3, 3, 2, 0, 2,  1, 2, 4, 2, 1, 4,
        3, 3, 3, 3, 4, 3, 2, 2, -1, 2, 3, 4, 4, 3, 2,
    },
    {
        2, 2, 2, 3, 2, 3, 2, 3, 0,  2, 3, 2, 2, 1, 3,
        1, 2, 2, 2, 2, 1, 1, 2, -1, 2, 1, 3, 3, 2, 2,
    },
    {
        2, 2, 2, 2, 3, 3, 2, 3, 1,  0, 1, 3, 2, 1, 3,
        2, 2, 3, 3, 3, 2, 1, 2, -1, 1, 2, 3, 3, 2, 2,
    },
    {
        2, 3, 2, 1, 3, 2, 1, 3, 3,  3, 0, 3, 2, 2, 2,
        2, 3, 2, 2, 4, 2, 3, 2, -1, 3, 3, 3, 2, 1, 3,
    },
    {
        2, 2, 2, 3, 2, 1, 2, 2, 2,  3, 3, 0, 3, 2, 2,
        3, 2, 1, 3, 3, 3, 2, 3, -1, 1, 3, 3, 3, 2, 3,
    },
    {
        3, 2, 2, 3, 2, 1, 2, 2, 2,  3, 3, 4, 0, 2, 2,
        3, 2, 1, 3, 3, 3, 3, 3, -1, 3, 3, 3, 3, 2, 3,
    },
    {
        3, 3, 2, 3, 2, 2, 1, 2, 2,  3, 2, 4, 1, 0, 3,
        3, 3, 2, 2, 3, 2, 2, 1, -1, 4, 2, 3, 3, 2, 1,
    },
    {
        3, 4, 3, 2, 1, 3, 2, 3, 2,  4, 1, 4, 3, 2, 0,
        3, 4, 3, 2, 2, 3, 3, 2, -1, 4, 2, 3, 1, 2, 3,
    },
    {
        1, 1, 2, 3, 3, 2, 3, 3, 3,  3, 2, 1, 3, 2, 3,
        0, 2, 2, 3, 4, 3, 2, 3, -1, 1, 3, 2, 2, 2, 3,
    },
    {
        1, 2, 2, 1, 3, 2, 2, 3, 2,  1, 2, 3, 2, 2, 3,
        2, 0, 2, 2, 2, 3, 1, 3, -1, 2, 1, 3, 2, 2, 3,
    },
    {
        2, 2, 1, 3, 2, 1, 1, 1, 1,  2, 2, 3, 2, 1, 2,
        2, 2, 0, 2, 3, 2, 2, 2, -1, 3, 2, 3, 3, 1, 2,
    },
    {
        1, 2, 2, 3, 3, 3, 2, 3, 3,  3, 3, 3, 2, 1, 2,
        2, 2, 2, 0, 4, 2, 1, 2, -1, 3, 3, 1, 3, 1, 2,
    },
    {
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, 0,  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    },
    {
        3, 3, 3, 3, 1, 4, 2, 2, 2,  1, 2, 4, 3, 2, 4,
        3, 3, 3, 2, 2, 0, 2, 1, -1, 2, 2, 3, 2, 3, 3,
    },
    {
        2, 3, 1, 2, 2, 3, 1, 3, 3,  2, 2, 3, 3, 2, 4,
        2, 1, 3, 2, 3, 4, 0, 2, -1, 3, 2, 3, 3, 3, 3,
    },
    {
        2, 3, 2, 3, 1, 3, 1, 1, 3,  2, 2, 4, 2, 2, 3,
        3, 3, 2, 1, 2, 3, 2, 0, -1, 3, 1, 2, 2, 2, 3,
    },
    {
        4, 4, 4, 3, 2, 4, 3, 3, 2, 4, 2, 4, 3, 2, 1,
        3, 4, 3, 3, 3, 3, 3, 2, 0, 4, 2, 4, 1, 3, 3,
    },
    {
        1, 1, 1, 3, 2, 4, 2, 4, 3,  3, 2, 3, 3, 2, 3,
        2, 2, 3, 3, 3, 3, 1, 3, -1, 0, 3, 2, 2, 2, 3,
    },
    {
        2, 3, 2, 4, 3, 2, 2, 2, 2,  3, 3, 4, 1, 2, 2,
        3, 3, 1, 1, 1, 2, 2, 3, -1, 4, 0, 2, 3, 1, 3,
    },
    {
        3, 1, 1, 3, 2, 4, 3, 4, 3,  4, 2, 3, 3, 2, 4,
        2, 3, 4, 3, 3, 4, 2, 3, -1, 2, 3, 0, 2, 3, 3,
    },
    {
        3, 3, 3, 2, 1, 3, 2, 2, 1,  3, 1, 3, 2, 1, 3,
        2, 3, 2, 2, 2, 2, 2, 1, -1, 3, 1, 3, 0, 2, 2,
    },
    {
        1, 2, 1, 3, 2, 2, 2, 2, 2,  2, 2, 3, 3, 2, 1,
        2, 2, 1, 1, 3, 1, 2, 2, -1, 3, 3, 2, 2, 0, 3,
    },
    {
        2, 3, 2, 3, 2, 3, 2, 3, 1,  2, 3, 3, 3, 2, 2,
        2, 2, 2, 2, 3, 1, 1, 2, -1, 3, 2, 3, 3, 1, 0,
    }};

const graph::NodeIdType sourceNode = 19;
};  // namespace

TEST(GraphTest, BFSProcessor) {
  graph::Graph graph;
  for (const auto [u, v] : edges) {
    graph.addEdge(u, v);
  }

  processor::BFSProcessor processor{graph};

  for (auto i = 0; i < graph.getVertexCount(); i++) {
    for (auto j = 0; j < graph.getVertexCount(); j++) {
      auto actualDistance = processor.calculateDistance(i, j);
      EXPECT_EQ(actualDistance, distances[i][j]);
    }
  }
}

TEST(GraphTest, CachedBFSProcessor) {
  graph::Graph graph;
  for (const auto [u, v] : edges) {
    graph.addEdge(u, v);
  }

  processor::CachedBFSProcessor processor{graph};

  for (auto i = 0; i < graph.getVertexCount(); i++) {
    for (auto j = 0; j < graph.getVertexCount(); j++) {
      auto actualDistance = processor.calculateDistance(i, j);
      EXPECT_EQ(actualDistance, distances[i][j]);
    }
  }
}

TEST(GraphTest, CachedBFSCoordinator) {
  graph::Graph graph;
  for (const auto [u, v] : edges) {
    graph.addEdge(u, v);
  }

  const auto kVertexCount = graph.getVertexCount();
  const auto kQueryCount = 1000;

  coordinator::CachedBFSCoordinator coordinator{graph};
  Batch batch{graph_rpc::CategoryType::DISTANCE};

  for (size_t i = 0; i < kQueryCount; i++) {
    graph_rpc::Query query;
    query.type = graph_rpc::QueryType::DISTANCE;
    query.u = rand() % kVertexCount;
    query.v = rand() % kVertexCount;
    batch.addQuery(std::move(query));
  }

  auto result = coordinator.admitBatch(batch);

  ASSERT_EQ(result.size(), batch.getQueriesView().size());
  ASSERT_EQ(result.size(), kQueryCount);

  for (size_t i = 0; const auto& query : batch.getQueriesView()) {
    auto u = query.u;
    auto v = query.v;
    ASSERT_EQ(result[i++], distances[u][v]);
  }
}

TEST(GraphTest, AllPairsCoordinator) {
  graph::Graph graph;
  for (const auto [u, v] : edges) {
    graph.addEdge(u, v);
  }

  const auto kVertexCount = graph.getVertexCount();
  const auto kQueryCount = 1000;

  coordinator::AllPairsCoordinator coordinator{graph};
  Batch batch{graph_rpc::CategoryType::DISTANCE};

  for (size_t i = 0; i < kQueryCount; i++) {
    graph_rpc::Query query;
    query.type = graph_rpc::QueryType::DISTANCE;
    query.u = rand() % kVertexCount;
    query.v = rand() % kVertexCount;
    batch.addQuery(std::move(query));
  }

  auto result = coordinator.admitBatch(batch);

  ASSERT_EQ(result.size(), batch.getQueriesView().size());
  ASSERT_EQ(result.size(), kQueryCount);

  for (size_t i = 0; const auto& query : batch.getQueriesView()) {
    auto u = query.u;
    auto v = query.v;
    ASSERT_EQ(result[i++], distances[u][v]);
  }
}

TEST(GraphTest, CachedBFSCoordinatorMultiBatch) {
  graph::Graph graph;
  for (const auto [u, v] : edges) {
    graph.addEdge(u, v);
  }

  const auto kVertexCount = graph.getVertexCount();

  const auto kBatchCount = 100;
  const auto kQueryCount = 1000;

  coordinator::CachedBFSCoordinator coordinator{graph};

  std::vector<Batch> batches;

  for (size_t i = 0; i < kBatchCount; i++) {
    Batch batch{graph_rpc::CategoryType::DISTANCE};
    for (size_t j = 0; j < kQueryCount; j++) {
      graph_rpc::Query query;
      query.type = graph_rpc::QueryType::DISTANCE;
      query.u = rand() % kVertexCount;
      query.v = rand() % kVertexCount;
      batch.addQuery(std::move(query));
    }
    batches.emplace_back(batch);
  }

  for (const auto& batch : batches) {
    coordinator.invalidateCaches();
    auto result = coordinator.admitBatch(batch);

    ASSERT_EQ(result.size(), batch.getQueriesView().size());
    ASSERT_EQ(result.size(), kQueryCount);

    for (size_t i = 0; const auto& query : batch.getQueriesView()) {
      auto u = query.u;
      auto v = query.v;
      ASSERT_EQ(result[i++], distances[u][v]);
    }
  }
}


TEST(GraphTest, AllPairsCoordinatorMultiBatch) {
  graph::Graph graph;
  for (const auto [u, v] : edges) {
    graph.addEdge(u, v);
  }

  const auto kVertexCount = graph.getVertexCount();

  const auto kBatchCount = 100;
  const auto kQueryCount = 1000;

  coordinator::AllPairsCoordinator coordinator{graph};

  std::vector<Batch> batches;

  for (size_t i = 0; i < kBatchCount; i++) {
    Batch batch{graph_rpc::CategoryType::DISTANCE};
    for (size_t j = 0; j < kQueryCount; j++) {
      graph_rpc::Query query;
      query.type = graph_rpc::QueryType::DISTANCE;
      query.u = rand() % kVertexCount;
      query.v = rand() % kVertexCount;
      batch.addQuery(std::move(query));
    }
    batches.emplace_back(batch);
  }

  for (const auto& batch : batches) {
    coordinator.invalidateCaches();
    auto result = coordinator.admitBatch(batch);

    ASSERT_EQ(result.size(), batch.getQueriesView().size());
    ASSERT_EQ(result.size(), kQueryCount);

    for (size_t i = 0; const auto& query : batch.getQueriesView()) {
      auto u = query.u;
      auto v = query.v;
      ASSERT_EQ(result[i++], distances[u][v]);
    }
  }
}
