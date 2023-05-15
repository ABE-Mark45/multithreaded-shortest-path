#include "processor/CachedBFSProcessor.h"

#include <queue>
#include <string>
#include <unordered_map>
#include "utils/constants.h"

namespace processor {

auto CachedBFSProcessor::to_key(graph::NodeIdType u, graph::NodeIdType v) {
  return std::to_string(u) + "|" + std::to_string(v);
}

void CachedBFSProcessor::reset() {
  cache_.clear();
}

graph::DistanceType CachedBFSProcessor::calculateDistance(graph::NodeIdType u,
                                                          graph::NodeIdType v) {
  if (u == v) {
    return graph_constants::kZeroDistance;
  }

  auto uIdx = graph_.getIdx(u);
  auto vIdx = graph_.getIdx(v);
  if (cache_.count(to_key(uIdx, vIdx))) {
    return cache_.at(to_key(uIdx, vIdx));
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
        cache_.insert({to_key(uIdx, neighbourIdx), currentDistance + 1});
        distance.insert({neighbourIdx, currentDistance + 1});
        q.push(neighbourIdx);
      }
    }
  }

  if (distance.count(vIdx)) {
    return distance.at(vIdx);
  } else {
    cache_.insert({to_key(uIdx, vIdx), graph_constants::kInvalidDistance});
    return graph_constants::kInvalidDistance;
  }
}
}  // namespace processor