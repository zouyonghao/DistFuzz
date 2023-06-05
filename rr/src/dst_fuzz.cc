#include "dst_fuzz.h"
#include <sys/shm.h>
#include <asm/unistd_64.h>

bool dst_fuzz_inited = false;
static uint8_t* fuzz_coverage_map;
#define FUZZ_COVERAGE_MAP_ENV_ID "AFLCplusplus_BRANCH_TRACE_SHM_ID"
#define FUZZ_COVERAGE_MAP_SIZE (1u << 23)

void dst_fuzz(rr::RecordTask* t) {
  if (!dst_fuzz_inited) {
    if (getenv("DST_FUZZ") != NULL) {
      char* res_shm_fuzz_coverage_map = getenv(FUZZ_COVERAGE_MAP_ENV_ID);
      if (!res_shm_fuzz_coverage_map) {
        fprintf(
            stderr, "%s",
            "\033[33m[FUZZ PRINT] Can not Get Environment Variable\033[0m\n");
      } else {
        fprintf(stderr,
                "\033[33m[FUZZ PRINT]Get Environment Variable %s\033[0m\n",
                res_shm_fuzz_coverage_map);
        fuzz_coverage_map =
            (uint8_t*)shmat((int)atoi(res_shm_fuzz_coverage_map), NULL, 0);
      }
    } else {
      fuzz_coverage_map =
          (uint8_t*)malloc(FUZZ_COVERAGE_MAP_SIZE * sizeof(uint8_t));
    }
  }

  switch (t->regs().original_syscallno()) {
    case __NR_read:
      // handle_random_event(tcp, false, tcp->u_arg[2], READ_ERROR_CODES,
      //                     sizeof(READ_ERROR_CODES) /
      //                         sizeof(READ_ERROR_CODES[0]));
      break;
    case __NR_write:
      // handle_random_event(tcp, true, tcp->u_arg[2], WRITE_ERROR_CODES,
      //                     sizeof(WRITE_ERROR_CODES) /
      //                         sizeof(WRITE_ERROR_CODES[0]));
      break;
    case __NR_readv:
    case __NR_preadv:
    case __NR_preadv2:
      // handle_random_event(
      //     tcp, false, get_length_from_iovec(tcp, 1, 2), READ_ERROR_CODES,
      //     sizeof(READ_ERROR_CODES) / sizeof(READ_ERROR_CODES[0]));
      break;
    case __NR_writev:
    case __NR_pwritev:
    case __NR_pwritev2:
      // handle_random_event(
      //     tcp, true, get_length_from_iovec(tcp, 1, 2), WRITE_ERROR_CODES,
      //     sizeof(WRITE_ERROR_CODES) / sizeof(WRITE_ERROR_CODES[0]));
      break;
    case __NR_recvfrom:
      // handle_random_event(tcp, false, tcp->u_arg[2], READ_ERROR_CODES,
      //                     sizeof(READ_ERROR_CODES) /
      //                         sizeof(READ_ERROR_CODES[0]));
      break;
    case __NR_sendto:
      // handle_random_event(tcp, true, tcp->u_arg[2], WRITE_ERROR_CODES,
      //                     sizeof(WRITE_ERROR_CODES) /
      //                         sizeof(WRITE_ERROR_CODES[0]));
      break;
    case __NR_recvmsg:
      // handle_random_event(
      //     tcp, false, get_length_from_msg(tcp, 1), READ_ERROR_CODES,
      //     sizeof(READ_ERROR_CODES) / sizeof(READ_ERROR_CODES[0]));
      break;
    case __NR_sendmsg:
      // handle_random_event(
      //     tcp, true, get_length_from_msg(tcp, 1), WRITE_ERROR_CODES,
      //     sizeof(WRITE_ERROR_CODES) / sizeof(WRITE_ERROR_CODES[0]));
      break;
    case __NR_recvmmsg:
      // handle_random_event(
      //     tcp, false, get_length_from_mmsg(tcp, 1, 2), READ_ERROR_CODES,
      //     sizeof(READ_ERROR_CODES) / sizeof(READ_ERROR_CODES[0]));
      break;
    case __NR_sendmmsg:
      // handle_random_event(
      //     tcp, true, get_length_from_mmsg(tcp, 1, 2), WRITE_ERROR_CODES,
      //     sizeof(WRITE_ERROR_CODES) / sizeof(WRITE_ERROR_CODES[0]));
      break;
    default:
      break;
  }

  fuzz_coverage_map[0] = 0;
}