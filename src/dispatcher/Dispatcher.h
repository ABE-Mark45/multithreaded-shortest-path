#pragma once
#include "batch/Batch.h"
#include "coordinator/AllPairsCoordinator.h"
#include "coordinator/BFSCoordinator.h"
#include "coordinator/CachedBFSCoordinator.h"
#include "coordinator/ICoordinator.h"
#include "gen-cpp/graph_types.h"
#include "graph/graph.h"
#include "logger/Logger.h"
#include "utils/types.h"

namespace dispatcher {

class Dispatcher {
 private:
  graph::Graph& graph_;
  std::shared_ptr<logger::Logger> logger_;
  std::unique_ptr<coordinator::ICoordinator> coordinator_;
  void processMutatingBatch(const Batch& batch);
  void processDistanceBatch(const Batch& batch,
                            std::vector<graph::DistanceType>& results);

 public:
  Dispatcher() = delete;

  Dispatcher(graph::Graph& graph, std::shared_ptr<logger::Logger> logger,
             coordinator::CoordinatorType type)
      : graph_(graph), logger_(logger) {
    switch (type) {
      case coordinator::ALL_PAIRS: {
        coordinator_ =
            std::make_unique<coordinator::AllPairsCoordinator>(graph_);
        break;
      }
      case coordinator::CACHED_BFS: {
        coordinator_ =
            std::make_unique<coordinator::CachedBFSCoordinator>(graph_);
        break;
      }
      case coordinator::BFS: {
        coordinator_ = std::make_unique<coordinator::BFSCoordinator>(graph_);
        break;
      }
    }
  }

  void dispatchQueries(const std::vector<graph_rpc::Query>& queries,
                       std::vector<graph::DistanceType>& results);
};

}  // namespace dispatcher