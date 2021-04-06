#include <cstring>
#include <signal.h>

#include <fstream>
#include <iostream>

#include <boost/interprocess/ipc/message_queue.hpp>

#include <dst_event_control.hpp>

using namespace boost::interprocess;

bool running = true;

void kill_handler(int signal)
{
    std::cout << "receive sigkill, closing..."
              << "\n";
    running = false;
}

int main(int argc, char *argv[])
{
    signal(SIGKILL, kill_handler);
    signal(SIGTERM, kill_handler);
    std::ofstream out("operation_log", std::ios::out | std::ios::binary);
    out << "[";
    try
    {
        message_queue::remove(EventConstant::EVENT_QUEUE_ID);
        message_queue mq(create_only, EventConstant::EVENT_QUEUE_ID,
                         EventConstant::MAX_QUEUE_NUMBER,
                         EventConstant::MAX_QUEUE_MESSAGE_SIZE);
        while (running)
        {
            std::string s;
            message_queue::size_type message_size;
            uint32_t message_priority;
            s.resize(EventConstant::MAX_QUEUE_MESSAGE_SIZE);

            if (!mq.try_receive(&s[0], s.size(), message_size, message_priority))
            {
                sleep(1);
                continue;
            }
            s.resize(message_size);
            std::string::size_type n =
                s.find(std::string(EventConstant::NEED_RECORD_MESSAGE_PREFIX));
            if (n != std::string::npos)
            {
                std::string content = s.substr(
                    n + strlen(EventConstant::NEED_RECORD_MESSAGE_PREFIX));
                std::cout << "Receive need record message: " << content << "\n";
                out << content << ",\n";
            }
            else
            {
                std::cout << "Receive message :" << s << "\n";
            }
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
    out << "]";
    out.close();

    message_queue::remove(EventConstant::EVENT_QUEUE_ID);

    return 0;
}