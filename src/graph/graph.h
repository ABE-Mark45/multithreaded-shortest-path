#pragma once
#include <utils/types.h>
#include <optional>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "logger/Logger.h"

namespace graph {
class Graph {
 private:
  AdjacencyListType adjList_;
  std::unordered_map<NodeIdType, NodeIdType> nodeToIndexMapping_;
  std::shared_ptr<logger::Logger> logger_;
  NodeIndexType getIdxAndChange(NodeIdType u);

 public:
  Graph(std::shared_ptr<logger::Logger> logger = nullptr) : logger_(logger) {}
  std::optional<NodeIndexType> getIdx(NodeIdType u) const;
  size_t getVertexCount() const;
  void addEdge(NodeIdType u, NodeIdType v);
  void removeEdge(NodeIdType u, NodeIdType v);
  const std::unordered_set<NodeIdType>& getNeighbours(NodeIndexType u) const;
};
}  // namespace graph