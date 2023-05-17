#pragma once
#include <inttypes.h>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace graph {
using NodeIdType = uint32_t;
using NodeIndexType = uint32_t;
using DistanceType = int32_t;
using DistanceDictType = std::unordered_map<NodeIndexType, DistanceType>;
using IndexType = std::vector<DistanceDictType>;
using AdjacencyListType = std::vector<std::unordered_set<NodeIdType>>;
}  // namespace graph

namespace coordinator {
enum CoordinatorType { BFS, ALL_PAIRS, CACHED_BFS };
}