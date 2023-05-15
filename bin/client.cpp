#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/transport/TSocket.h>
#include <iostream>
#include "gen-cpp/GraphService.h"

int main() {
  std::shared_ptr<apache::thrift::transport::TTransport> socket =
      std::make_shared<apache::thrift::transport::TSocket>("localhost", 9090);
  std::shared_ptr<apache::thrift::transport::TTransport> transport =
      std::make_shared<apache::thrift::transport::TBufferedTransport>(socket);
  std::shared_ptr<apache::thrift::protocol::TProtocol> protocol =
      std::make_shared<apache::thrift::protocol::TBinaryProtocol>(transport);
  graph_rpc::GraphServiceClient client(protocol);

  transport->open();

  std::vector<int32_t> result;
  graph_rpc::Query q;
  q.type = graph_rpc::QueryType::ADD_EDGE;
  q.u = 1;
  q.v = 2;

  client.sendQueryBatch(result, {q, q, q});
  for (const auto ans : result) {
    std::cout << ans << std::endl;
  }

  transport->close();

  return 0;
}