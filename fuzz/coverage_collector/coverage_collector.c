#include <signal.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "constants.h"
#include "librdkafka/rdkafka.h"

static volatile sig_atomic_t run = 1;
static void stop(int sig) { run = 0; }

int main(int argc, char const *argv[])
{
    uint8_t *coverage_map = calloc(MAP_SIZE, sizeof(uint8_t));
    rd_kafka_topic_partition_list_t *subscription;
    char hostname[128];
    char errstr[512];
    rd_kafka_resp_err_t err;

    rd_kafka_conf_t *conf = rd_kafka_conf_new();
    if (gethostname(hostname, sizeof(hostname)))
    {
        fprintf(stderr, "%% Failed to lookup hostname\n");
        exit(1);
    }
    if (rd_kafka_conf_set(conf, "client.id", hostname, errstr, sizeof(errstr)) != RD_KAFKA_CONF_OK)
    {
        fprintf(stderr, "%% %s\n", errstr);
        exit(1);
    }
    if (rd_kafka_conf_set(conf, "group.id", "foo", errstr, sizeof(errstr)) != RD_KAFKA_CONF_OK)
    {
        fprintf(stderr, "%% %s\n", errstr);
        exit(1);
    }
    if (rd_kafka_conf_set(conf, "bootstrap.servers", BOOTSTRAP_SERVER, errstr, sizeof(errstr)) != RD_KAFKA_CONF_OK)
    {
        fprintf(stderr, "%% %s\n", errstr);
        exit(1);
    }

    /* Create Kafka consumer handle */
    rd_kafka_t *rk;
    if (!(rk = rd_kafka_new(RD_KAFKA_CONSUMER, conf, errstr, sizeof(errstr))))
    {
        fprintf(stderr, "%% Failed to create new consumer: %s\n", errstr);
        exit(1);
    }

    rd_kafka_poll_set_consumer(rk);
    subscription = rd_kafka_topic_partition_list_new(1);
    rd_kafka_topic_partition_list_add(subscription, KAFKA_COVERAGE_TOPIC, RD_KAFKA_PARTITION_UA);
    /* Subscribe to the list of topics */
    err = rd_kafka_subscribe(rk, subscription);
    if (err)
    {
        fprintf(stderr, "%% Failed to subscribe to %d topics: %s\n", subscription->cnt, rd_kafka_err2str(err));
        rd_kafka_topic_partition_list_destroy(subscription);
        rd_kafka_destroy(rk);
        return 1;
    }

    rd_kafka_topic_partition_list_destroy(subscription);
    signal(SIGINT, stop);
    uint32_t old_cov_count = 0;
    while (run)
    {
        rd_kafka_message_t *rkm;

        rkm = rd_kafka_consumer_poll(rk, 100);
        if (!rkm)
            continue;
        if (rkm->err)
        {
            fprintf(stderr, "%% Consumer error: %s\n", rd_kafka_message_errstr(rkm));
            rd_kafka_message_destroy(rkm);
            continue;
        }

        if (!rkm->payload)
        {
            rd_kafka_message_destroy(rkm);
            continue;
        }

        /* Proper message. */
        // printf("Message on %s [%" PRId32 "] at offset %" PRId64 ":\n", rd_kafka_topic_name(rkm->rkt), rkm->partition,
        //        rkm->offset);

        // printf("Value: %ld\n", *((uint64_t *)rkm->payload));

        // rd_kafka_message_destroy(rkm);

        uint64_t index = (*((uint64_t *)rkm->payload)) % MAP_SIZE;
        if (coverage_map[index] < 255)
        {
            coverage_map[index]++;
        }

        rd_kafka_message_destroy(rkm);

        uint32_t cov_count = 0;
        for (uint32_t i = 0; i < MAP_SIZE; i++)
        {
            if (coverage_map[i] != 0)
            {
                cov_count++;
            }
        }
        if (cov_count > old_cov_count)
        {
            struct timespec ts;
            clock_gettime(CLOCK_REALTIME, &ts); // Works on Linux
            fprintf(stderr, "time is %ld, coverage count : %d\n", ts.tv_sec, cov_count);
            old_cov_count = cov_count;
        }
    }

    /* Close the consumer: commit final offsets and leave the group. */
    fprintf(stderr, "%% Closing consumer\n");
    rd_kafka_consumer_close(rk);

    /* Destroy the consumer */
    rd_kafka_destroy(rk);

    return 0;
}
