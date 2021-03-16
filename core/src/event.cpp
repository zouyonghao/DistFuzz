#include <iostream>
#include <string>

#include <boost/interprocess/ipc/message_queue.hpp>

#include <event.hpp>
#include <event_control.hpp>

using namespace boost::interprocess;

extern "C" int event_trigger(const char *s)
{
    try
    {
        // Open a message queue.
        message_queue mq(open_or_create, EventConstant::EVENT_QUEUE_ID,
                         EventConstant::MAX_QUEUE_NUMBER,
                         EventConstant::MAX_QUEUE_MESSAGE_SIZE);
        mq.send(s, strlen(s) + 1, 0);
    }
    catch (interprocess_exception &ex)
    {
        std::cout << ex.what() << std::endl;
        return 1;
    }
    return 0;
}