#pragma once
#include <chrono>
#include <fstream>
#include <iostream>
#include "gen-cpp/graph_types.h"
#include "utils/types.h"

namespace logger {
class Logger {
 private:
  std::ofstream outputFileStream_;
  std::ostream* output_;
  std::chrono::system_clock::time_point startTime_;

  void logCurrentTime();

 public:
  Logger(const std::string& filePath = "");
  //   Logger(std::ostream& stream = std::cout);
  ~Logger();

  void println(std::string_view str);

  void logDistanceQuery(graph_rpc::Query query, graph::DistanceType distance);
  void logAddEdge(graph::NodeIdType u, graph::NodeIdType v, bool isEdgeAdded);
  void logRemoveEdge(graph::NodeIdType u, graph::NodeIdType v,
                     bool isEdgeRemoved);
};
}  // namespace logger