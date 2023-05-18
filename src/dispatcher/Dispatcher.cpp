#include "dispatcher/Dispatcher.h"

namespace dispatcher {

void Dispatcher::processMutatingBatch(const Batch& batch) {
  coordinator_->invalidateCaches();
  for (const auto& query : batch.getQueriesView()) {
    if (query.type == graph_rpc::QueryType::ADD_EDGE) {
      graph_.addEdge(query.u, query.v);
    } else if (query.type == graph_rpc::QueryType::REMOVE_EDGE) {
      graph_.removeEdge(query.u, query.v);
    }
  }
}

void Dispatcher::processDistanceBatch(
    const Batch& batch, std::vector<graph::DistanceType>& results) {
  auto partialResults = coordinator_->admitBatch(batch);
  for (size_t i = 0; const auto& query: batch.getQueriesView()) {
    logger_->logDistanceQuery(query, partialResults[i++]);
  }
  results.insert(results.end(), partialResults.cbegin(), partialResults.cend());
}

void Dispatcher::dispatchQueries(const std::vector<graph_rpc::Query>& queries,
                                 std::vector<graph::DistanceType>& results) {
  auto batches = Batch::transformToBatches(queries);

  for (const auto& batch : batches) {
    if (batch.getCategoryType() == graph_rpc::CategoryType::DISTANCE) {
      processDistanceBatch(batch, results);
    } else {
      processMutatingBatch(batch);
    }
  }
}

}  // namespace dispatcher