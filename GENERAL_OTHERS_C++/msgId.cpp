#include <stdio.h>
#include <iostream>
#include <stdint.h>

using namespace std;
#define _MOVIK_CAE_PUBSUB_SYNC_START_MSGID 1004
#define MOVIK_CAE_PUBSUB_SYNC_START_MSGID(_TOPIC) (_MOVIK_CAE_PUBSUB_SYNC_START_MSGID | (_TOPIC << 16))
#define GET_PUBSUB_TOPIC_FROM_MSGID(msgId) ((msgId >> 16) & 0xffff)
int main()
{
    uint32_t msgId = 0x5b0003ec;
    uint32_t topic_id = 0;
    topic_id = GET_PUBSUB_TOPIC_FROM_MSGID(msgId);
    printf("topic id %u %x \n", topic_id, topic_id);

    return 0;
}
