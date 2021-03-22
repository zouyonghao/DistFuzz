#include <string>

class EventConstant
{
public:
    const static char *EVENT_QUEUE_ID;
    const static uint32_t MAX_QUEUE_NUMBER;
    const static uint32_t MAX_QUEUE_MESSAGE_SIZE;
};

const char * EventConstant::EVENT_QUEUE_ID = "__ds_event_queue__";
const uint32_t EventConstant::MAX_QUEUE_NUMBER = 1000;
const uint32_t EventConstant::MAX_QUEUE_MESSAGE_SIZE= 1000;