
#include <unistd.h>    //write
#include <arpa/inet.h> //inet_addr
#include <sys/socket.h>

#include <list>
#include <iostream>
#include <algorithm>

#include <proxy_server.h>

ProxyServer::ProxyServer(int _src_port,
                         int _dest_port,
                         int _delay_time,
                         std::vector<struct replace_pair> _replace_pairs)
    : src_port(_src_port),
      dest_port(_dest_port),
      delay_time(_delay_time),
      replace_pairs(_replace_pairs)
{
    printf("src_port = %d, dest_port = %d\n", src_port, dest_port);
    thread0 = new std::thread(&ProxyServer::accept_connection_handler, this);
}

void ProxyServer::set_src_name(std::string _src_name)
{
    this->src_name = _src_name;
}

void ProxyServer::set_dest_name(std::string _dest_name)
{
    this->dest_name = _dest_name;
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
    if (setsockopt(socket_desc, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) != 0)
    {
        std::cout << "setsockopt(SO_REUSEADDR) failed\n";
        exit(-1);
    }
    if (setsockopt(socket_desc, SOL_SOCKET, SO_REUSEPORT, &enable, sizeof(int)) != 0)
    {
        std::cout << "setsockopt(SO_REUSEPORT) failed\n";
        exit(-1);
    }

    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.1.1");
    server.sin_port = htons(src_port);

    //Bind
    if (bind(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        //print the error message
        perror("bind failed. Error");
        return;
    }
    std::cout << "bind done\n";

    //Listen
    listen(socket_desc, 3);

    //Accept and incoming connection
    std::cout << "Waiting for incoming connections...\n";
    c = sizeof(struct sockaddr_in);
    while (running && (client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t *)&c)))
    {
        std::cout << "Connection accepted, client port is" << client.sin_port << std::endl;

        struct connection_pair *current_connection = new connection_pair;
        current_connection->src_sock = client_sock;
        current_connection->dest_sock = connect_to_server();
        if (current_connection->dest_sock < 0)
        {
            close(current_connection->src_sock);
            continue;
        }

        std::thread *thread1 = new std::thread(&ProxyServer::receive_from_src_handler, this, current_connection);
        std::thread *thread2 = new std::thread(&ProxyServer::receive_from_dest_handler, this, current_connection);

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

    //Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");

    int enable;
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
    {
        printf("setsockopt(SO_REUSEADDR) failed\n");
        exit(-1);
    }

    server.sin_addr.s_addr = inet_addr("127.0.1.1");
    server.sin_family = AF_INET;
    server.sin_port = htons(dest_port);

    //Connect to remote server
    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        perror("connect failed. Error");
        return -1;
    }

    puts("Connected\n");
    return sock;
}

void ProxyServer::close_connection_pair(struct connection_pair *cp)
{
    connection_lock.lock();
    if (cp != NULL && std::find(connection_pairs.begin(), connection_pairs.end(), cp) != connection_pairs.end())
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
/*
 * receive from src and send to dest
 * */
void ProxyServer::receive_from_src_handler(struct connection_pair *current_pair)
{
    int read_size;
    char client_message[2000];

    //Receive a message from client
    while (running && (read_size = recv(current_pair->src_sock, client_message, 2000, 0)) > 0)
    {
        //Send the message back to client
        printf("message from %d : size = %d\n", current_pair->src_sock, read_size);
        for (int i = 0; i < read_size; i++)
        {
            printf("%c", client_message[i]);
        }
        printf("\n");
        // TODO: log name here
        for (int i = 0; i < read_size; i++)
        {
            for (auto &s : replace_pairs)
            {
                if (is_replace_str((char *)client_message + i, s.src))
                {
                    for (int k = 0; k < s.dest.length(); k++)
                    {
                        client_message[i] = s.dest[k];
                        i++;
                    }
                }
            }
        }
        printf("\n");
        printf("Message replaced!\n");
        for (int i = 0; i < read_size; i++)
        {
            printf("%c", client_message[i]);
        }
        printf("\n");

        usleep(delay_time);
        int ret = write(current_pair->dest_sock, client_message, read_size);
        if (ret < 0)
        {
            printf("write failed!\n");
            break;
        }
    }

    close_connection_pair(current_pair);
}

/*
 * receive from dest and send back to src
 * */
void ProxyServer::receive_from_dest_handler(struct connection_pair *current_pair)
{
    int read_size;
    char client_message[2000];

    while (running && (read_size = recv(current_pair->dest_sock, client_message, 2000, 0)) > 0)
    {
        //Send the message back to client
        printf("message from dest_sock: size = %d\n", read_size);
        for (int i = 0; i < read_size; i++)
        {
            printf("%c", client_message[i]);
        }
        printf("\n");
        for (int i = 0; i < read_size; i++)
        {
            for (auto &s : replace_pairs)
            {
                if (is_replace_str((char *)client_message + i, s.dest))
                {
                    for (int k = 0; k < s.src.length(); k++)
                    {
                        client_message[i] = s.src[k];
                        i++;
                    }
                }
            }
        }
        printf("\n");
        printf("Message replaced!\n");
        for (int i = 0; i < read_size; i++)
        {
            printf("%c", client_message[i]);
        }
        printf("\n");

        usleep(delay_time);
        int ret = write(current_pair->src_sock, client_message, read_size);

        if (ret < 0)
        {
            printf("write failed!\n");
            break;
        }
    }

    close_connection_pair(current_pair);
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