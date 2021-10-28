#ifndef __COMMON_H__
#define __COMMON_H__

#define MAP_SIZE_POW2 23
#define MAP_SIZE (1u << MAP_SIZE_POW2)

#define BOOTSTRAP_SERVER "192.168.32.177:9092"
static const char *topic_code_coverage_str = "code_coverage";
static const char *topic_order_coverage_str = "order_coverage";

#endif