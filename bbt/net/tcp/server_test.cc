#include "gmock/gmock.h"

#include "bbt/net/buffer.h"
#include "bbt/net/tcp/client.h"
#include "bbt/net/tcp/server.h"
#include "bbt/net/connection.h"

namespace bbt {
namespace net {
namespace {

TEST(TcpServer, echo_server_example) {
  std::string svr_received, cli_received;

  asio::io_context io_context(1);
  std::thread t([&]() { io_context.run(); });

  // server
  TcpServer svr(io_context);
  svr.set_conn_callback([](const ConnPtr& conn) {});
  svr.set_receive_callback([&](const ConnPtr& conn, Buffer* buf) {
    svr_received = buf->ToString();
    conn->Send("World", 5);
  });
  svr.Listen("0.0.0.0", "56565");

  // client
  TcpClient cli(io_context);
  cli.set_conn_callback([](const ConnPtr& conn) {});
  cli.set_receive_callback([&](const ConnPtr& conn, Buffer* buf) {
    cli_received = buf->ToString();
  });

  cli.Connect("0.0.0.0", "56565");
  cli.Send("Hello", 5);

  // ASSERT_EQ(svr_received, "Hello");
  // ASSERT_EQ(cli_received, "World");

  io_context.stop();
  t.join();
}

}  // namespace
}  // namespace net
}  // namespace bbt