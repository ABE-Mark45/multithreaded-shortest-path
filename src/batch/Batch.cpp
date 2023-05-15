#include "batch/Batch.h"

/* static */ auto Batch::getQueryCategoryType(graph_rpc::QueryType::type type) {
  return type == graph_rpc::QueryType::DISTANCE
             ? graph_rpc::CategoryType::DISTANCE
             : graph_rpc::CategoryType::MUTATION;
}
graph_rpc::CategoryType::type Batch::getCategoryType() const {
  return type_;
}
void Batch::addQuery(const graph_rpc::Query& query) {
  queries_.emplace_back(query);
}

const std::vector<graph_rpc::Query>& Batch::getQueriesView() const {
  return queries_;
}
/* static */ std::vector<Batch> Batch::transformToBatches(
    const std::vector<graph_rpc::Query>& queries) {
  std::vector<Batch> batches;

  for (const auto& query : queries) {
    auto queryCategoryType = getQueryCategoryType(query.type);
    if (batches.empty() ||
        queryCategoryType != batches.back().getCategoryType()) {
      batches.emplace_back(getQueryCategoryType(query.type));
    }
    batches.back().addQuery(query);
  }

  return batches;
}