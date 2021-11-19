
#include <arpa/inet.h> //inet_addr
#include <proxy_server.h>
#include <sys/socket.h>
#include <unistd.h> //write

#include <algorithm>
#include <future>
#include <iostream>
#include <list>

#include <dst_event.h>
#include <dst_random.h>
#include <dst_log.hpp>

ProxyServer::ProxyServer(std::string _src_ip, int _src_port,
                         std::string _dest_ip, int _dest_port, int _delay_time,
                         std::vector<struct replace_pair> _replace_pairs)
    : src_ip(_src_ip), src_port(_src_port), dest_ip(_dest_ip),
      dest_port(_dest_port), delay_time(_delay_time),
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
        std::cerr << "setsockopt(SO_REUSEADDR) failed\n";
        close(socket_desc);
        return;
    }
    if (setsockopt(socket_desc, SOL_SOCKET, SO_REUSEPORT, &enable,
                   sizeof(int)) != 0)
    {
        std::cerr << "setsockopt(SO_REUSEPORT) failed\n";
        close(socket_desc);
        return;
    }
    struct linger linger;
    linger.l_onoff = 1;
    linger.l_linger = 0;
    if (setsockopt(socket_desc, SOL_SOCKET, SO_LINGER, (const char *)&linger,
                   sizeof(linger)) != 0)
    {
        std::cerr << "setsockopt(SO_LINGER) failed\n";
        close(socket_desc);
        return;
    }

    // Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(src_ip.c_str());
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
                  << ", server port is " << src_port << std::endl;

        struct connection_pair *current_connection = new connection_pair;
        current_connection->src_sock = client_sock;
        current_connection->dest_sock = connect_to_server();
        if (current_connection->dest_sock < 0)
        {
            close(current_connection->src_sock);
            free(current_connection);
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

    printf("stoped!\n");

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

    server.sin_addr.s_addr = inet_addr(dest_ip.c_str());
    server.sin_family = AF_INET;
    server.sin_port = htons(dest_port);

    // Connect to remote server
    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        std::cerr << "connect failed.\n";
        return -1;
    }

    std::cout << "Connected\n";
    return sock;
}

void ProxyServer::close_connection_pair(struct connection_pair *cp)
{
    std::cout << "closing...\n";
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

bool is_replace_str(char *target, char *replace_str, int size)
{
    for (int i = 0; i < size; i++)
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
    uint8_t client_message[1000];
    bool should_break = false;
    // Receive a message from client
    while (!should_break && running &&
           (read_size = read(src_sock, client_message, 1000)) > 0)
    {
        if (debug)
        {
            std::cout << "read size = " << read_size << "\n";
            // Send the message back to client
            std::cout << "message in char from " << src_sock
                      << ", size = " << read_size << "\n";
            for (int i = 0; i < read_size; i++)
            {
                std::cout << client_message[i];
            }
            std::cout << std::endl;

            std::cout << "\nmessage content in bits :\n";
            for (int i = 0; i < read_size; i++)
            {
                // std::cout << client_message[i];
                printf("%x ", client_message[i]);
            }
            std::cout << "\n";
        }
        for (int i = 0; i < read_size; i++)
        {
            for (auto &s : replace_pairs)
            {
                const struct replace_item &src =
                    direction == SRC_TO_DEST ? s.src : s.dest;
                const struct replace_item &dest =
                    direction == SRC_TO_DEST ? s.dest : s.src;

                if (is_replace_str((char *)(client_message + i),
                                   src.target_string, src.size))
                {
                    std::cout << "Receiving from " << src.target_name
                              << ", send it to " << dest.target_name << "\n";
                    // __dst_event_trigger(("Receiving from " + src.target_name
                    // +
                    //                      ", send it to " + dest.target_name)
                    //                         .c_str());
                    for (int k = 0; k < dest.size; k++)
                    {
                        printf("%x ", client_message[i]);
                        client_message[i] = dest.target_string[k];
                        i++;
                    }
                }
            }
        }
        // std::cout << "\nMessage content:\n";
        // for (int i = 0; i < read_size; i++)
        // {
        //     // std::cout << client_message[i];
        //     printf("%x", client_message[i]);
        // }
        std::cout << std::endl;

        uint8_t select_random =
            __dst_get_random_uint8_t() % SUPPORTED_ACTION::ACTION_COUNT;

        bool need_dup = false;

        if (skiped_messages < skip_messages)
        {
            skiped_messages++;
            goto WRITE;
        }

        std::cerr << "select_random is " << std::to_string(select_random)
                  << "\n";
        switch (select_random)
        {
        case SUPPORTED_ACTION::NOOP:
        {
            break;
        }
        case SUPPORTED_ACTION::DELAY:
        {
            uint32_t random = __dst_get_random_uint32_t();
            usleep(random);
            __dst_event_trigger(
                ("sleep for " + std::to_string(random) + "n").c_str());
            break;
        }
        case SUPPORTED_ACTION::LOST:
        {
            continue;
        }
        // case SUPPORTED_ACTION::REORDER:
        //     // TODO
        //     break;
        case SUPPORTED_ACTION::ASYNC_DELAY:
        {
            uint8_t tmp_client_message[1000];
            for (int i = 0; i < read_size; i++)
            {
                tmp_client_message[i] = client_message[i];
            }
            std::async(
                std::launch::async,
                [dest_sock, tmp_client_message, read_size, &should_break, cp,
                 this]
                {
                    uint32_t random = __dst_get_random_uint32_t();
                    usleep(random);
                    std::lock_guard<std::mutex> lk(cp->lock_for_connection);
                    __dst_event_trigger(
                        ("sleep for " + std::to_string(random) + "n").c_str());
                    if (do_write)
                    {
                        int ret =
                            write(dest_sock, tmp_client_message, read_size);
                        if (ret < 0)
                        {
                            std::cerr << "write failed!\n";
                            should_break = true;
                        }
                    }
                });
            continue;
        }
        case SUPPORTED_ACTION::DUP:
        {
            need_dup = true;
            break;
        }
        default:
            break;
        }

    WRITE:
    {
        std::lock_guard<std::mutex> lk(cp->lock_for_connection);
        if (do_write)
        {
            int ret = write(dest_sock, client_message, read_size);
            if (ret != read_size)
            {
                // TODO
                std::cerr << "write less than read!!\n";
            }
            if (ret < 0)
            {
                perror("write failed!");
                break;
            }
        }
    }
        if (need_dup)
        {
            need_dup = false;
            goto WRITE;
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