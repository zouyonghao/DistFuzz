#include "Fuzzer.h"

bitset<MAP_SIZE> traceRecord;
bitset<MAP_SIZE> concurrent_trace_record;
static bool scoreChange = false;


void MinimizeBit(bitset<MAP_SIZE> *trace, u8 *globalTraceBit) {
    u32 i;
    (*trace).reset();
    for (i = 0; i < MAP_SIZE; i++) {
        if (globalTraceBit[i]) (*trace)[i] = 1;
    }
}


void UpdateQueueTop(seed_container::iterator s, u8 *globalTraceBit) {
    u32 i;
    u64 score = s->execTime * s->fileLen;

    for (i = 0; i < MAP_SIZE; i++) {
        if (globalTraceBit[i]) {
            // if the queueTop[i] is not empty
            if (queueTop[i] != queue.end()) {
                if (score > queueTop[i]->execTime * queueTop[i]->fileLen) {
                    continue;
                }

                // if (*((u32*)variableStateChangeCount) < 40) {
                //     continue;
                // }

                if (!--queueTop[i]->topCount) {
                    delete queueTop[i]->trace;
                    queueTop[i]->trace = NULL;
                }
            }

            queueTop[i] = s;
            s->topCount++;

            if (!s->trace) {
                s->trace = new bitset<MAP_SIZE>;
                MinimizeBit(s->trace, globalTraceBit);
            }

            scoreChange = true;
        }
    }
} 

void CullQueue(void) {
    //vector<seed>::iterator q;
    if (scoreChange == false) return;
    scoreChange = false;
    pendFavor = false;

    u32 i;
    traceRecord.reset();
    concurrent_trace_record.reset();
    for (auto &tmp:queue){
        tmp.favored = false;
    }
    for (i = 0; i < MAP_SIZE; i++) {   
        if (queueTop[i] != queue.end() && traceRecord[i] == 0) {
            if (queueTop[i]->trace) {
                traceRecord |= *(queueTop[i]->trace);
            }
            queueTop[i]->favored = true;

            if (queueTop[i]->wasFuzzed == false) {
                pendFavor = true;
            }
        }
    }
#ifndef NO_CONCURRENCY_FUZZ
    // for concurrent queue top
    for (i = 0; i < MAP_SIZE; i++) {
        if (concurrent_queue_top[i] != queue.end() &&
            concurrent_trace_record[i] == 0) {
            
            if (concurrent_queue_top[i]->concurrent_trace) {
                concurrent_trace_record |= *(concurrent_queue_top[i]->concurrent_trace);
            }
        
            concurrent_queue_top[i]->favored = true;

            if (concurrent_queue_top[i]->wasFuzzed == false) {
                pendFavor = true;
            }
        }
    }
#endif
}
