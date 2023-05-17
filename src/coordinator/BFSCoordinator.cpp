#include "coordinator/BFSCoordinator.h"

namespace coordinator {

BFSCoordinator::BFSCoordinator(const graph::Graph& graph)
    : distanceProcessor_(graph) {}

BFSCoordinator::~BFSCoordinator() {}

std::vector<graph::DistanceType> BFSCoordinator::admitBatch(
    const Batch& batch) {
  std::vector<graph::DistanceType> results;
  
  for (const auto& query : batch.getQueriesView()) {
    auto distance = distanceProcessor_.calculateDistance(query.u, query.v);
    results.push_back(distance);
  }

  return results;
}

void BFSCoordinator::invalidateCaches() {}

}  // namespace coordinator