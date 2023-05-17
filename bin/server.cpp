#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/transport/TServerSocket.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include "dispatcher/Dispatcher.h"
#include "gen-cpp/GraphService.h"
#include "graph/graph.h"
#include "logger/Logger.h"

class GraphServiceHandler : public graph_rpc::GraphServiceIf {
 private:
  graph::Graph graph_;
  dispatcher::Dispatcher dispatcher_;
  logger::Logger logger_;

  void initGraph(const std::string& path) {
    std::ifstream inputStream(path);

    std::string line;

    while (std::getline(inputStream, line) && line != "S") {
      std::stringstream ss(line);
      graph::NodeIdType u, v;
      ss >> u >> v;
      graph_.addEdge(u, v);
    }
    inputStream.close();
  }

 public:
  GraphServiceHandler(coordinator::CoordinatorType type,
                      const std::string& inputGraphPath,
                      const std::string& outputLogFile)
      : dispatcher_(graph_, type), logger_(outputLogFile) {
    logger_.println("Starting Server");
    logger_.println("Initializing graph");
    initGraph(inputGraphPath);
    logger_.println("Graph initialized successfully");
  }

  void sendQueryBatch(
      std::vector<graph::DistanceType>& result,
      const std::vector<graph_rpc::Query>& queryBatch) override {
    dispatcher_.dispatchQueries(queryBatch, result);
  }
};

int main(int argc, char** argv) {
  if (argc != 4) {
    throw std::runtime_error(
        "./server <bfs | cached_bfs | all_pairs> <input-graph> <output-log>");
  }

  std::string type(argv[1]);
  coordinator::CoordinatorType coordinatorType;
  if (type == "bfs") {
    coordinatorType = coordinator::BFS;
  } else if (type == "cached_bfs") {
    coordinatorType = coordinator::CACHED_BFS;
  } else if (type == "all_pairs") {
    coordinatorType = coordinator::ALL_PAIRS;
  } else {
    throw std::runtime_error(
        "./server <bfs | cached_bfs | all_pairs> <input-graph> <output-log>");
  }

  apache::thrift::server::TSimpleServer server(
      std::make_shared<graph_rpc::GraphServiceProcessor>(
          std::make_shared<GraphServiceHandler>(coordinatorType, argv[2],
                                                argv[3])),
      std::make_shared<apache::thrift::transport::TServerSocket>(9090),
      std::make_shared<apache::thrift::transport::TBufferedTransportFactory>(),
      std::make_shared<apache::thrift::protocol::TBinaryProtocolFactory>());
  std::cout << "Starting the server..." << std::endl;
  server.serve();
  std::cout << "Server stopped." << std::endl;
  return 0;
}