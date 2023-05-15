#pragma once
#include <string>
#include <unordered_map>
#include "graph/graph.h"

namespace processor {

class CachedBFSProcessor {
 private:
  std::unordered_map<std::string, graph::DistanceType> cache_;
  const graph::Graph& graph_;

  auto to_key(graph::NodeIdType u, graph::NodeIdType v);

 public:
  CachedBFSProcessor(const graph::Graph& graph) : graph_(graph) {}
  CachedBFSProcessor() = delete;

  CachedBFSProcessor(const CachedBFSProcessor&) = delete;
  CachedBFSProcessor& operator=(const CachedBFSProcessor&) = delete;

  CachedBFSProcessor(CachedBFSProcessor&&) = default;
  CachedBFSProcessor& operator=(CachedBFSProcessor&&) = default;

  void reset();

  graph::DistanceType calculateDistance(graph::NodeIdType u,
                                        graph::NodeIdType v);
};
}  // namespace processor