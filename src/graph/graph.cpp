#include "graph.h"
#include <queue>

namespace graph {

NodeIdType Graph::getIdxAndChange(NodeIdType u) {
  if (nodeToIndexMapping_.count(u)) {
    return nodeToIndexMapping_.at(u);
  }

  NodeIdType idx = nodeToIndexMapping_.size();
  nodeToIndexMapping_.insert({u, idx});
  adjList_.push_back({});
  return idx;
}

NodeIdType Graph::getIdx(NodeIdType u) const {
  return nodeToIndexMapping_.at(u);
}

size_t Graph::getVertexCount() const {
  return nodeToIndexMapping_.size();
}

void Graph::addEdge(NodeIdType u, NodeIdType v) {
  NodeIdType uIdx = getIdxAndChange(u);
  NodeIdType vIdx = getIdxAndChange(v);

  adjList_[uIdx].insert(vIdx);
}

void Graph::removeEdge(NodeIdType u, NodeIdType v) {
  NodeIdType uIdx = getIdxAndChange(u);
  NodeIdType vIdx = getIdxAndChange(v);

  adjList_[uIdx].erase(vIdx);
}

const std::unordered_set<NodeIdType>& Graph::getNeighbours(
    NodeIdType u) const {
  return adjList_[u];
}

}  // namespace graph