#pragma once

#include <vector>
#include "gen-cpp/graph_types.h"

class Batch {
 private:
  std::vector<graph_rpc::Query> queries_;
  const graph_rpc::CategoryType::type type_;
  static auto getQueryCategoryType(graph_rpc::QueryType::type type);

 public:
  Batch() = delete;
  Batch(graph_rpc::CategoryType::type type) : type_(type) {}

  graph_rpc::CategoryType::type getCategoryType() const;
  void addQuery(const graph_rpc::Query& query);

  const std::vector<graph_rpc::Query>& getQueriesView() const;
  static std::vector<Batch> transformToBatches(
      const std::vector<graph_rpc::Query>& queries);
};
