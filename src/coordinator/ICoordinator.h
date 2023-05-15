#pragma once
#include <vector>
#include "batch/Batch.h"
#include "utils/types.h"

namespace coordinator {
class ICoordinator {
 public:
  virtual void invalidateCaches() = 0;
  virtual std::vector<graph::DistanceType> admitBatch(const Batch& batch) = 0;
};
}  // namespace coordinator
