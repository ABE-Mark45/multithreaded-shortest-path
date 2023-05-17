#include "logger/Logger.h"
#include <exception>

namespace logger {
Logger::Logger(const std::string& filePath) : output_(&std::cout) {
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

void Logger::logDistanceQuery(graph_rpc::Query query,
                              graph::DistanceType distance) {
  *output_ << "Source: " << query.u << "\t\tDestination: " << query.v
           << "\t\tDistance: " << distance << std::endl;
}

}  // namespace logger