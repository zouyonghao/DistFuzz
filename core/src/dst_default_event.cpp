#include <cstring>
#include <iostream>
#include <string>

#include <boost/interprocess/ipc/message_queue.hpp>

#include <dst_event.h>
#include <dst_event_control.hpp>

using namespace boost::interprocess;

int __dst_event_trigger(const char *s)
{
    try
    {
        // Open a message queue.
        // message_queue mq(open_or_create, EventConstant::EVENT_QUEUE_ID,
        //                  EventConstant::MAX_QUEUE_NUMBER,
        //                  EventConstant::MAX_QUEUE_MESSAGE_SIZE);
        message_queue mq(open_only, EventConstant::EVENT_QUEUE_ID);
        mq.send(s, strlen(s), 0);
    }
    catch (interprocess_exception &ex)
    {
        // ignored
        // std::cout << ex.what() << std::endl;
        return 1;
    }
    return 0;
}

int __dst_event_record(const char *s)
{
    return __dst_event_trigger((EventConstant::NEED_RECORD_MESSAGE_PREFIX + std::string(s)).c_str());
}