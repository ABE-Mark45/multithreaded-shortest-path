#include "processor/BFSProcessor.h"

#include <queue>
#include "utils/constants.h"

namespace processor {
graph::DistanceType BFSProcessor::calculateDistance(graph::NodeIdType u,
                                                    graph::NodeIdType v) {
  if (u == v) {
    return graph_constants::kZeroDistance;
  }
  auto uIdx = graph_.getIdx(u);
  auto vIdx = graph_.getIdx(v);

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