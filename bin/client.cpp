#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/transport/TSocket.h>
#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <thread>
#include "gen-cpp/GraphService.h"
#include "logger/Logger.h"

class Client {
 private:
  std::shared_ptr<apache::thrift::transport::TTransport> socket_;
  std::shared_ptr<apache::thrift::transport::TTransport> transport_;
  std::shared_ptr<apache::thrift::protocol::TProtocol> protocol_;
  graph_rpc::GraphServiceClient client_;
  std::ifstream inputFileStream_;
  logger::Logger logger_;

  auto parseNextBatch() {
    std::vector<graph_rpc::Query> batch;
    std::string line;
    while (getline(inputFileStream_, line) && line != "F") {
      std::stringstream ss(line);

      graph_rpc::Query q;
      char type;
      ss >> type;

      switch (type) {
        case 'A':
          q.type = graph_rpc::QueryType::ADD_EDGE;
          break;
        case 'Q':
          q.type = graph_rpc::QueryType::DISTANCE;
          break;
        case 'D':
          q.type = graph_rpc::QueryType::REMOVE_EDGE;
          break;
        default:
          throw std::runtime_error("Undefined operation");
      }

      ss >> q.u >> q.v;
      batch.emplace_back(std::move(q));
    }

    return batch;
  }

 public:
  Client(const std::string& inputPath, const std::string& outputPath)
      : socket_(std::make_shared<apache::thrift::transport::TSocket>(
            "localhost", 9090)),
        transport_(
            std::make_shared<apache::thrift::transport::TBufferedTransport>(
                socket_)),
        protocol_(std::make_shared<apache::thrift::protocol::TBinaryProtocol>(
            transport_)),
        client_(protocol_),
        inputFileStream_(inputPath),
        logger_(outputPath) {
    transport_->open();
  }

  ~Client() {
    transport_->close();
    inputFileStream_.close();
  }

  auto sendNextBatch() {
    auto batch = parseNextBatch();

    if (batch.empty()) {
      return false;
    }

    std::vector<graph::DistanceType> result;

    auto startTime = std::chrono::high_resolution_clock::now();
    logger_.println("Sending batch to server");
    client_.sendQueryBatch(result, batch);
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
        endTime - startTime);

    for (size_t i = 0; const auto& query: batch) {
      if (query.type == graph_rpc::QueryType::DISTANCE) {
        logger_.logDistanceQuery(query, result[i++]);
      }
    }

    logger_.println("Batch was calculated in " + std::to_string(duration.count()) + " us");
    return true;
  }
};

int main(int argc, char** argv) {
  using namespace std::chrono_literals;
  if (argc != 3) {
    throw std::runtime_error("./client <input-path> <log-path>");
  }
  Client client(argv[1], argv[2]);

  while (client.sendNextBatch()) {
    // std::this_thread::sleep_for(3s);
  }
  return 0;
}