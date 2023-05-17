#pragma once
#include "batch/Batch.h"
#include "coordinator/ICoordinator.h"
#include "gen-cpp/graph_types.h"
#include "graph/graph.h"
#include "processor/BFSProcessor.h"

namespace coordinator {

class BFSCoordinator : public ICoordinator {
 public:
  BFSCoordinator() = delete;
  BFSCoordinator(const BFSCoordinator&) = delete;
  BFSCoordinator& operator=(const BFSCoordinator&) = delete;

  BFSCoordinator(BFSCoordinator&&) = default;
  BFSCoordinator& operator=(BFSCoordinator&&) = default;

  BFSCoordinator(const graph::Graph& graph);

  ~BFSCoordinator();

  std::vector<graph::DistanceType> admitBatch(const Batch& batch) override;
  void invalidateCaches() override;

 private:
  processor::BFSProcessor distanceProcessor_;
};

}  // namespace coordinator