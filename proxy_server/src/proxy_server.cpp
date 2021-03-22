
#include <arpa/inet.h> //inet_addr
#include <proxy_server.h>
#include <sys/socket.h>
#include <unistd.h> //write

#include <algorithm>
#include <iostream>
#include <list>

#include <dst_event.hpp>
#include <dst_random.h>

ProxyServer::ProxyServer(int _src_port, int _dest_port, int _delay_time,
                         std::vector<struct replace_pair> _replace_pairs)
    : src_port(_src_port), dest_port(_dest_port), delay_time(_delay_time),
      replace_pairs(_replace_pairs)
{
    printf("src_port = %d, dest_port = %d\n", src_port, dest_port);
    thread0 = new std::thread(&ProxyServer::accept_connection_handler, this);
}

void ProxyServer::accept_connection_handler()
{
    int socket_desc, client_sock, c;
    struct sockaddr_in server, client;
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc < 0)
    {
        std::cout << "Could not create socket" << std::endl;
        return;
    }
    std::cout << "Socket created" << std::endl;

    int enable = 0;
    if (setsockopt(socket_desc, SOL_SOCKET, SO_REUSEADDR, &enable,
                   sizeof(int)) != 0)
    {
        std::cout << "setsockopt(SO_REUSEADDR) failed\n";
        exit(-1);
    }
    if (setsockopt(socket_desc, SOL_SOCKET, SO_REUSEPORT, &enable,
                   sizeof(int)) != 0)
    {
        std::cout << "setsockopt(SO_REUSEPORT) failed\n";
        exit(-1);
    }
    struct linger linger;
    linger.l_onoff = 1;
    linger.l_linger = 0;
    if (setsockopt(socket_desc, SOL_SOCKET, SO_LINGER, (const char *)&linger,
                   sizeof(linger)) != 0)
    {
        std::cout << "setsockopt(SO_LINGER) failed\n";
        exit(-1);
    }

    // Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.1.1");
    server.sin_port = htons(src_port);

    // Bind
    if (bind(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        // print the error message
        perror("bind failed. Error");
        return;
    }
    std::cout << "bind done\n";

    // Listen
    listen(socket_desc, 3);

    // Accept and incoming connection
    std::cout << "Waiting for incoming connections...\n";
    c = sizeof(struct sockaddr_in);
    while (running &&
           (client_sock = accept(socket_desc, (struct sockaddr *)&client,
                                 (socklen_t *)&c)))
    {
        std::cout << "Connection accepted, client port is " << client.sin_port
                  << std::endl;

        struct connection_pair *current_connection = new connection_pair;
        current_connection->src_sock = client_sock;
        current_connection->dest_sock = connect_to_server();
        if (current_connection->dest_sock < 0)
        {
            close(current_connection->src_sock);
            continue;
        }

        std::thread *thread1 =
            new std::thread(&ProxyServer::receive_and_send_handler, this,
                            current_connection, current_connection->src_sock,
                            current_connection->dest_sock, SRC_TO_DEST);
        std::thread *thread2 =
            new std::thread(&ProxyServer::receive_and_send_handler, this,
                            current_connection, current_connection->dest_sock,
                            current_connection->src_sock, DEST_TO_SRC);

        current_connection->thread1 = thread1;
        current_connection->thread2 = thread2;

        puts("Handler assigned");
        connection_pairs.push_back(current_connection);
    }

    if (client_sock < 0)
    {
        perror("accept failed");
    }

    close(socket_desc);
    cleanup(0);
}

int ProxyServer::connect_to_server()
{
    int sock;
    struct sockaddr_in server;

    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
    {
        printf("Could not create socket");
    }
    std::cout << "Socket created\n";

    int enable;
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
    {
        std::cout << "setsockopt(SO_REUSEADDR) failed\n";
        exit(-1);
    }
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEPORT, &enable, sizeof(int)) < 0)
    {
        std::cout << "setsockopt(SO_REUSEPORT) failed\n";
        exit(-1);
    }

    server.sin_addr.s_addr = inet_addr("127.0.1.1");
    server.sin_family = AF_INET;
    server.sin_port = htons(dest_port);

    // Connect to remote server
    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        perror("connect failed. Error");
        return -1;
    }

    std::cout << "Connected\n";
    return sock;
}

void ProxyServer::close_connection_pair(struct connection_pair *cp)
{
    connection_lock.lock();
    if (cp != NULL &&
        std::find(connection_pairs.begin(), connection_pairs.end(), cp) !=
            connection_pairs.end())
    {
        close(cp->src_sock);
        close(cp->dest_sock);
        connection_pairs.remove(cp);
        delete cp;
    }
    connection_lock.unlock();
}

bool is_replace_str(char *target, const std::string &replace_str)
{
    for (int i = 0; i < replace_str.length(); i++)
    {
        if (target[i] != replace_str[i])
        {
            return false;
        }
    }
    return true;
}

void ProxyServer::receive_and_send_handler(struct connection_pair *cp,
                                           int src_sock, int dest_sock,
                                           DIRECTION direction)
{
    int read_size;
    char client_message[2000];

    // Receive a message from client
    while (running && (read_size = recv(src_sock, client_message, 2000, 0)) > 0)
    {
        // Send the message back to client
        // std::cout << "message from " << src_sock << ", size = " << read_size;
        // for (int i = 0; i < read_size; i++)
        // {
        //     std::cout << client_message[i];
        // }
        // std::cout << std::endl;
        for (int i = 0; i < read_size; i++)
        {
            for (auto &s : replace_pairs)
            {
                const struct replace_item &src =
                    direction == SRC_TO_DEST ? s.src : s.dest;
                const struct replace_item &dest =
                    direction == SRC_TO_DEST ? s.dest : s.src;

                if (is_replace_str((char *)client_message + i,
                                   src.target_string))
                {
                    std::cout << "Receiving from " << src.target_name
                              << ", send it to " << dest.target_name << "\n";
                    __dst_event_trigger(("Receiving from " + src.target_name +
                                         ", send it to " + dest.target_name)
                                            .c_str());
                    for (int k = 0; k < dest.target_string.length(); k++)
                    {
                        client_message[i] = dest.target_string[k];
                        i++;
                    }
                }
            }
        }
        std::cout << "\nMessage Replaced!\n";
        for (int i = 0; i < read_size; i++)
        {
            std::cout << client_message[i];
        }
        std::cout << std::endl;

        uint16_t random = __dst_get_random_uint16_t();
        usleep(random);
        __dst_event_trigger(
            ("sleep for " + std::to_string(random) + "n").c_str());
        int ret = write(dest_sock, client_message, read_size);
        if (ret < 0)
        {
            std::cout << "write failed!\n";
            break;
        }
    }

    close_connection_pair(cp);
}

void ProxyServer::cleanup(int signo)
{
    // connection_lock.lock();
    // running = false;
    // std::cout << "cleaning up..." << std::endl;
    // for (struct connection_pair *cp : connection_pairs)
    // {
    //     close(cp->src_sock);
    //     close(cp->dest_sock);
    //     cp->thread1->join();
    //     cp->thread2->join();
    // }
    // connection_lock.unlock();
}

ProxyServer::~ProxyServer()
{
    // cleanup(0);
}