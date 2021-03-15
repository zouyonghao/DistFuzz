#ifndef PROXY_SERVER_H
#define PROXY_SERVER_H

#include <list>
#include <map>
#include <mutex>
#include <thread>
#include <vector>

enum DIRECTION
{
    SRC_TO_DEST,
    DEST_TO_SRC
};

struct connection_pair
{
    int src_sock;
    int dest_sock;
    std::thread *thread1;
    std::thread *thread2;
};
struct replace_item
{
    std::string target_string;
    std::string target_name;
};
struct replace_pair
{
    struct replace_item src;
    struct replace_item dest;
};
/**
 * This class proxy a port to another
 * for example:
 *     127.0.1.1:8302 to 127.0.1.1:8303
 *
 * It can also replace the conresponding strings
 * for example:
 *     xxxx:8302 to xxxx:8303
 *     xxxx:8300 to xxxx:8304
 *     xxxx:8301 to xxxx:8305
 *
 */
class ProxyServer
{
private:
    int src_port;
    int dest_port;
    std::vector<struct replace_pair> replace_pairs;
    int delay_time;

    std::thread *thread0;
    bool running = true;
    char *reorder_buffer[3];
    std::mutex connection_lock;

    std::list<struct connection_pair *> connection_pairs;

    void accept_connection_handler();

    int connect_to_server();

    void close_connection_pair(struct connection_pair *cp);

    // the thread function
    void receive_and_send_handler(struct connection_pair *cp, int src_sock,
                                  int dest_sock, DIRECTION direction);

    void cleanup(int signo);

public:
    ProxyServer(int _src_port, int _dest_port, int _delay_time,
                std::vector<struct replace_pair> _replace_pairs);
    ~ProxyServer();
};

#endif