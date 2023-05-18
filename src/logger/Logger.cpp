#include "logger/Logger.h"
#include <exception>
#include <iomanip>

namespace logger {
Logger::Logger(const std::string& filePath)
    : output_(&std::cout),
      startTime_(std::chrono::high_resolution_clock::now()) {
  if (!filePath.empty()) {
    outputFileStream_.open(filePath);
    if (outputFileStream_.is_open()) {
      output_ = &outputFileStream_;
    }
  }
}

// Logger::Logger(std::ostream& stream) : output_(stream) {}
Logger::~Logger() {
  if (outputFileStream_.is_open()) {
    outputFileStream_.close();
  }
}

void Logger::println(std::string_view str) {
  *output_ << str << std::endl;
}

void Logger::logCurrentTime() {
  using namespace std::chrono;

  auto elapsed =
      duration_cast<milliseconds>(high_resolution_clock::now() - startTime_);
  auto h = duration_cast<hours>(elapsed);
  elapsed -= h;
  auto m = duration_cast<minutes>(elapsed);
  elapsed -= m;
  auto s = duration_cast<seconds>(elapsed);
  elapsed -= s;
  auto milliseconds = elapsed;

  *output_ << '[' << std::setfill('0') << std::setw(2) << h.count() << ":"
           << std::setw(2) << m.count() << ":" << std::setw(2) << s.count()
           << ":" << std::setw(3) << milliseconds.count() << "] ";
}

void Logger::logDistanceQuery(graph_rpc::Query query,
                              graph::DistanceType distance) {
  logCurrentTime();
  *output_ << "[DISTANCE] (" << query.u << ", " << query.v << ") -> "
           << distance << std::endl;
}

void Logger::logAddEdge(graph::NodeIdType u, graph::NodeIdType v,
                        bool isEdgeAdded) {
  logCurrentTime();
  if (isEdgeAdded) {
    *output_ << "[ADD] (" << u << ", " << v << ")" << std::endl;
  } else {
    *output_ << "[ADD][Skip] (" << u << ", " << v << ")" << std::endl;
  }
}
void Logger::logRemoveEdge(graph::NodeIdType u, graph::NodeIdType v,
                           bool isEdgeRemoved) {
  logCurrentTime();
  if (isEdgeRemoved) {
    *output_ << "[REMOVE] (" << u << ", " << v << ")" << std::endl;
  } else {
    *output_ << "[REMOVE][Skip] (" << u << ", " << v << ")" << std::endl;
  }
}

}  // namespace logger