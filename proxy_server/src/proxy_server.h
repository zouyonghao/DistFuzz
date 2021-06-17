#ifndef PROXY_SERVER_H
#define PROXY_SERVER_H

#include <list>
#include <map>
#include <mutex>
#include <thread>
#include <vector>

enum SUPPORTED_ACTION
{
    NOOP,
    // REORDER,
    LOST,
    DELAY,
    ASYNC_DELAY, // TODO: think about this argument: async delay is reorder?
    DUP,
    ACTION_COUNT
};

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
    std::mutex lock_for_connection;
};
struct replace_item
{
    std::string target_name;
    char *target_string;
    int size;
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
    std::string src_ip;
    int src_port;
    std::string dest_ip;
    int dest_port;
    std::vector<struct replace_pair> replace_pairs;
    int delay_time; // deprecated
    int skip_messages; // skip delay/reorder/drop process for some messages at start
    int skiped_messages = 0;

    std::thread *thread0;
    bool running = true;
    char *reorder_buffer[3]; // deprecated
    std::mutex connection_lock;

    std::list<struct connection_pair *> connection_pairs;

    bool debug = false;
    bool do_write = true;

    void accept_connection_handler();

    int connect_to_server();

    void close_connection_pair(struct connection_pair *cp);

    /*
     * receive from src and send to dest
     */
    void receive_and_send_handler(struct connection_pair *cp, int src_sock,
                                  int dest_sock, DIRECTION direction);

    void cleanup(int signo);

public:
    ProxyServer(std::string src_ip, int _src_port, std::string dest_ip,
                int _dest_port, int _delay_time,
                std::vector<struct replace_pair> _replace_pairs);
    ~ProxyServer();

    void set_skip_messages(int _skip_message)
    {
        this->skip_messages = _skip_message;
    }

    void enable_debug()
    {
        debug = true;
    }

    void disable_write()
    {
        do_write = false;
    }
};

#endif