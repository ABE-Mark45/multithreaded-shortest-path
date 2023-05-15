#pragma once
#include <utils/types.h>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace graph {
class Graph {
 private:
  AdjacencyListType adjList_;
  std::unordered_map<NodeIdType, NodeIdType> nodeToIndexMapping_;
  NodeIndexType getIdxAndChange(NodeIdType u);

 public:
  NodeIndexType getIdx(NodeIdType u) const;
  size_t getVertexCount() const;
  void addEdge(NodeIdType u, NodeIdType v);
  void removeEdge(NodeIdType u, NodeIdType v);
  const std::unordered_set<NodeIdType>& getNeighbours(
      NodeIndexType u) const;
};
}  // namespace graph