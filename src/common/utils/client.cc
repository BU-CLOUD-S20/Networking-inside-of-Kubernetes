#include <tacopie/tacopie>

#include <condition_variable>
#include <iostream>
#include <mutex>
#include <signal.h>
#include <vector>

std::condition_variable cv;

void
signint_handler(int) {
  cv.notify_all();
}

void
on_new_message(tacopie::tcp_client& client, const tacopie::tcp_client::read_result& res) {
  if (res.success) {
    std::cout << "Client recv data" << std::endl;
    client.async_write({res.buffer, nullptr});
    client.async_read({1024, std::bind(&on_new_message, std::ref(client), std::placeholders::_1)});
  }
  else {
    std::cout << "Client disconnected" << std::endl;
    client.disconnect();
  }
}

int
main(void) {
  tacopie::tcp_client client;
  client.connect("172.28.1.1", 8001);
  std::cout << client.get_host() << std::endl;
  std::vector<char> buf;
  buf.push_back('i');
  client.async_write({buf, nullptr});
  //client.async_read({1024, std::bind(&on_new_message, std::ref(client), std::placeholders::_1)});

  signal(SIGINT, &signint_handler);
/*
  std::mutex mtx;
  std::unique_lock<std::mutex> lock(mtx);
  cv.wait(lock);
*/
  return 0;
}