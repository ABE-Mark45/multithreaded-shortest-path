#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/transport/TServerSocket.h>
#include <iostream>
#include "dispatcher/Dispatcher.h"
#include "gen-cpp/GraphService.h"
#include "graph/graph.h"

class GraphServiceHandler : public graph_rpc::GraphServiceIf {
 private:
  graph::Graph graph_;
  dispatcher::Dispatcher dispatcher_;

 public:
  GraphServiceHandler(coordinator::CoordinatorType type)
      : dispatcher_(graph_, type) {}

  void sendQueryBatch(
      std::vector<graph::DistanceType>& result,
      const std::vector<graph_rpc::Query>& queryBatch) override {
    dispatcher_.dispatchQueries(queryBatch, result);
  }
};

int main() {
  apache::thrift::server::TSimpleServer server(
      std::make_shared<graph_rpc::GraphServiceProcessor>(
          std::make_shared<GraphServiceHandler>(coordinator::CACHED_BFS)),
      std::make_shared<apache::thrift::transport::TServerSocket>(9090),
      std::make_shared<apache::thrift::transport::TBufferedTransportFactory>(),
      std::make_shared<apache::thrift::protocol::TBinaryProtocolFactory>());
  std::cout << "Starting the server..." << std::endl;
  server.serve();
  std::cout << "Server stopped." << std::endl;
  return 0;
}