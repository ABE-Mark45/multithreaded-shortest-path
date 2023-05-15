#pragma once
#include "graph/graph.h"
#include "utils/types.h"

namespace processor {
class BFSProcessor {
 private:
  const graph::Graph& graph_;

 public:
  BFSProcessor(const graph::Graph& graph) : graph_(graph) {}
  BFSProcessor() = delete;
  BFSProcessor(const BFSProcessor&) = delete;
  BFSProcessor(BFSProcessor&&) = delete;

  graph::DistanceType calculateDistance(graph::NodeIdType u,
                                        graph::NodeIdType v);
};
}  // namespace processor