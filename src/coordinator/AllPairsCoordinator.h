#pragma once
#include <unordered_set>
#include <vector>
#include "coordinator/ICoordinator.h"
#include "graph/graph.h"
#include "utils/types.h"

namespace coordinator {
class AllPairsCoordinator : public ICoordinator {
 private:
  const graph::Graph& graph_;
  graph::IndexType index_;
  bool isCacheInvalid = true;
  void rebuildIndex();

 public:
  AllPairsCoordinator() = delete;
  AllPairsCoordinator(const graph::Graph& graph) : graph_(graph) {}

  void invalidateCaches() override;
  std::vector<graph::DistanceType> admitBatch(const Batch& batch) override;
};

}  // namespace coordinator