#include "processor/BFSProcessor.h"

#include <queue>
#include "utils/constants.h"

namespace processor {
graph::DistanceType BFSProcessor::calculateDistance(graph::NodeIdType u,
                                                    graph::NodeIdType v) {
  auto uIdxOpt = graph_.getIdx(u);
  auto vIdxOpt = graph_.getIdx(v);
  if (!uIdxOpt || !vIdxOpt) {
    return graph_constants::kInvalidDistance;
  }
  auto uIdx = uIdxOpt.value();
  auto vIdx = vIdxOpt.value();

  if (uIdx == vIdx) {
    return graph_constants::kZeroDistance;
  }

  graph::DistanceDictType distance{{uIdx, 0}};
  std::queue<graph::NodeIndexType> q;
  q.push(uIdx);

  while (!q.empty()) {
    auto currentIdx = q.front();
    auto currentDistance = distance.at(currentIdx);
    q.pop();

    if (currentIdx == vIdx) {
      break;
    }

    for (const auto neighbourIdx : graph_.getNeighbours(currentIdx)) {
      if (!distance.count(neighbourIdx)) {
        distance.insert({neighbourIdx, currentDistance + 1});
        q.push(neighbourIdx);
      }
    }
  }

  if (distance.count(vIdx)) {
    return distance.at(vIdx);
  } else {
    return graph_constants::kInvalidDistance;
  }
}
}  // namespace processor