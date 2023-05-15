#include "coordinator/CachedBFSCoordinator.h"

namespace coordinator {

CachedBFSCoordinator::CachedBFSCoordinator(const graph::Graph& graph,
                                           size_t threadCount)
    : kThreadCount_(threadCount), queues_(threadCount) {
  for (size_t i = 0; i < kThreadCount_; i++) {
    distanceProcessors_.emplace_back(graph);

    threads_.emplace_back([this, i]() {
      while (true) {
        const auto [idx, query] = queues_[i].pop();
        if (query == graph_rpc::Query{}) {
          return;
        }
        auto distance =
            distanceProcessors_[i].calculateDistance(query.u, query.v);
        sink_.push({idx, distance});
      }
    });
  }
}

CachedBFSCoordinator::~CachedBFSCoordinator() {
  stop_ = true;
  for (auto& queue : queues_) {
    queue.stop();
  }
  sink_.stop();

  for (auto& thread : threads_) {
    thread.join();
  }
}

std::vector<graph::DistanceType> CachedBFSCoordinator::admitBatch(
    const Batch& batch) {
  for (size_t idx = 0; const auto& query : batch.getQueriesView()) {
    size_t threadId = query.u % kThreadCount_;
    queues_[threadId].push({idx++, query});
  }

  const auto kBatchSize = batch.getQueriesView().size();
  std::vector<graph::DistanceType> results(kBatchSize);

  for (size_t i = 0; i < kBatchSize; i++) {
    const auto [idx, distance] = sink_.pop();
    results[idx] = distance;
  }

  return results;
}

void CachedBFSCoordinator::invalidateCaches() {
  for (auto& processor: distanceProcessors_) {
    processor.reset();
  }
}

}  // namespace coordinator