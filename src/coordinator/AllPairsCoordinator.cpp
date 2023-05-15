#include "coordinator/AllPairsCoordinator.h"

#include <execution>
#include <numeric>
#include <queue>
#include "utils/constants.h"

namespace coordinator {
void AllPairsCoordinator::rebuildIndex() {
  index_ = graph::IndexType(graph_.getVertexCount());
  std::vector<graph::NodeIndexType> vertexIndices(graph_.getVertexCount());
  std::iota(vertexIndices.begin(), vertexIndices.end(), 0);

  std::for_each(std::execution::par_unseq, vertexIndices.cbegin(),
                vertexIndices.cend(), [this](const auto source) {
                  auto& distances = index_.at(source);
                  distances.insert({source, 0});
                  std::queue<graph::NodeIndexType> q;
                  q.push(source);

                  while (!q.empty()) {
                    const auto u = q.front();
                    const auto currentDistance = distances.at(u);
                    q.pop();

                    for (const auto v : graph_.getNeighbours(u)) {
                      if (!distances.count(v)) {
                        distances.insert({v, currentDistance + 1});
                        q.push(v);
                      }
                    }
                  }
                });

  isCacheInvalid = false;
}

void AllPairsCoordinator::invalidateCaches() {
  isCacheInvalid = true;
}
std::vector<graph::DistanceType> AllPairsCoordinator::admitBatch(
    const Batch& batch) {
  if (isCacheInvalid) {
    rebuildIndex();
  }

  std::vector<graph::DistanceType> results(batch.getQueriesView().size());

  std::transform(std::execution::par_unseq, batch.getQueriesView().cbegin(),
                 batch.getQueriesView().cend(), results.begin(),
                 [this](const auto& query) {
                   auto uIdx = graph_.getIdx(query.u);
                   auto vIdx = graph_.getIdx(query.v);

                   if (!index_[uIdx].count(vIdx)) {
                     return graph_constants::kInvalidDistance;
                   }

                   return index_[uIdx].at(vIdx);
                 });

  return results;
}

}  // namespace coordinator