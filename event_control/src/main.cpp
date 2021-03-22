#include <iostream>

#include <boost/interprocess/ipc/message_queue.hpp>

#include <dst_event_control.hpp>

using namespace boost::interprocess;

int main(int argc, char *argv[])
{
    try
    {
        message_queue::remove(EventConstant::EVENT_QUEUE_ID);
        message_queue mq(create_only, EventConstant::EVENT_QUEUE_ID,
                         EventConstant::MAX_QUEUE_NUMBER,
                         EventConstant::MAX_QUEUE_MESSAGE_SIZE);
        while (true)
        {
            std::string s;
            message_queue::size_type message_size;
            uint32_t message_priority;
            s.resize(EventConstant::MAX_QUEUE_MESSAGE_SIZE);

            mq.receive(&s[0], s.size(), message_size, message_priority);
            s.resize(message_size);

            std::cout << "Receive message :" << s << "\n";
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }

    message_queue::remove(EventConstant::EVENT_QUEUE_ID);

    return 0;
}