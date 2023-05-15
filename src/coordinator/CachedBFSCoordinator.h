#pragma once
#include <atomic>
#include <thread>
#include "batch/Batch.h"
#include "coordinator/ICoordinator.h"
#include "gen-cpp/graph_types.h"
#include "graph/graph.h"
#include "processor/CachedBFSProcessor.h"
#include "queue/ConcurrentQueue.h"

namespace coordinator {

class CachedBFSCoordinator : public ICoordinator {
 public:
  CachedBFSCoordinator() = delete;
  CachedBFSCoordinator(const CachedBFSCoordinator&) = delete;
  CachedBFSCoordinator& operator=(const CachedBFSCoordinator&) = delete;

  CachedBFSCoordinator(CachedBFSCoordinator&&) = default;
  CachedBFSCoordinator& operator=(CachedBFSCoordinator&&) = default;

  CachedBFSCoordinator(
      const graph::Graph& graph,
      size_t threadCount = std::thread::hardware_concurrency());

  ~CachedBFSCoordinator();

  std::vector<graph::DistanceType> admitBatch(const Batch& batch) override;
  void invalidateCaches() override;

 private:
  const size_t kThreadCount_;
  std::vector<std::thread> threads_;
  std::vector<queue::ConcurrentQueue<std::pair<size_t, graph_rpc::Query>>>
      queues_;
  queue::ConcurrentQueue<std::pair<size_t, graph::DistanceType>> sink_;
  std::vector<processor::CachedBFSProcessor> distanceProcessors_;
  std::atomic_bool stop_ = false;
};

}  // namespace coordinator