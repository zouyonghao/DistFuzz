#ifndef PROXY_SERVER_H
#define PROXY_SERVER_H

#include <map>
#include <mutex>
#include <thread>

struct connection_pair
{
    int src_sock;
    int dest_sock;
    std::thread *thread1;
    std::thread *thread2;
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
    std::map<std::string, std::string> replace_pairs;
    int delay_time;

    std::thread *thread0;
    bool running = true;
    char *reorder_buffer[3];
    std::mutex connection_lock;

    std::list<struct connection_pair *> connection_pairs;

    void accept_connection_handler();

    int connect_to_server();

    void close_connection_pair(struct connection_pair *cp);

    //the thread function
    void receive_from_src_handler(struct connection_pair *cp);

    //the thread function
    void receive_from_dest_handler(struct connection_pair *cp);

    void cleanup(int signo);

public:
    ProxyServer(int _src_port,
                int _dest_port,
                int _delay_time,
                std::map<std::string, std::string> _replace_pairs);
    ~ProxyServer();
};

#endif