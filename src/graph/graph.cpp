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

std::optional<NodeIdType> Graph::getIdx(NodeIdType u) const {
  if (nodeToIndexMapping_.count(u)) {
    return nodeToIndexMapping_.at(u);
  }
  return std::nullopt;
}

size_t Graph::getVertexCount() const {
  return nodeToIndexMapping_.size();
}

void Graph::addEdge(NodeIdType u, NodeIdType v) {
  NodeIdType uIdx = getIdxAndChange(u);
  NodeIdType vIdx = getIdxAndChange(v);

  auto oldSize = adjList_[uIdx].size();
  if (uIdx != vIdx) {
    adjList_[uIdx].insert(vIdx);
  }

  if (logger_) {
    logger_->logAddEdge(u, v, adjList_[uIdx].size() > oldSize);
  }
}

void Graph::removeEdge(NodeIdType u, NodeIdType v) {
  NodeIdType uIdx = getIdxAndChange(u);
  NodeIdType vIdx = getIdxAndChange(v);

  auto oldSize = adjList_[uIdx].size();
  adjList_[uIdx].erase(vIdx);

  if (logger_) {
    logger_->logAddEdge(u, v, adjList_[uIdx].size() < oldSize);
  }
}

const std::unordered_set<NodeIdType>& Graph::getNeighbours(NodeIdType u) const {
  return adjList_[u];
}

}  // namespace graph