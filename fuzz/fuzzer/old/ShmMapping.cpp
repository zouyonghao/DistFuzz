#include "Fuzzer.h"

#define SHM_ENV_VAR					"AFLCplusplus_SHM_ID"
static s32 shmId;

#define ERROR_LIST_SHM_ENV_VAR		"AFLCplusplus_ERROR_LIST_SHM_ID"
#define ERROR_TRACED_SHM_ENV_VAR	"AFLCplusplus_ERROR_TRACED_SHM_ID"
static s32 error_list_shmId;
static s32 error_traced_shmId;

#define CONTROL_MATRIX_SHM_ENV_VAR		"AFLCplusplus_CONTROL_MATRIX_SHM_ID"
#define ADJACENCY_MATRIX_SHM_ENV_VAR	"AFLCplusplus_ADJACENCY_MATRIX_SHM_ID"
#define TRAVEL_MATRIX_SHM_ENV_VAR		"AFLCplusplus_TRAVEL_MATRIX_SHM_ID"
static s32 control_matrix_shmid;
static s32 adjacency_matrix_shmid;
static s32 travel_matrix_shmid;

#define GLOBAL_STATE_ENV_VAR	"AFLCplusplus_GLOBAL_STATE_SHM_ID" // indicate whether a state change has been recorded
static s32 global_state_shmid;

#define VARIABLE_COUNT_SHM_ENV_VAR "AFLCplusplus_VARIABLE_COUNT_SHMD_ID" // count the number of state change
static s32 variable_count_shmid;

#define BRANCH_TRACE_SHM_ENV_VAR	"AFLCplusplus_BRANCH_TRACE_SHM_ID"
static s32 branch_trace_shmid;

static s32 SingleShmSetup(u8 *& mem_ptr, u32 mem_size, string env_var) {
	s32 shm_id = shmget(IPC_PRIVATE, mem_size, IPC_CREAT|IPC_EXCL|0600);
	if (shm_id < 0) {
		cout << "fail to shmget()" << endl;
		exit(-1);
	}

	string shm_str = to_string(shm_id);
	if (setenv(env_var.c_str(), shm_str.c_str(), 1)) {
		cout << "fail to setenv()" << endl;
		exit(-1);
	}

	mem_ptr = (u8 *)shmat(shm_id, NULL, 0);
	if (!mem_ptr) {
		cout << "fail to shmat()" << endl;
		exit(-1);
	}

	memset(mem_ptr, 0, mem_size); // init the share memory to 0
	return shm_id;
}

void ShmSetup(void) {
	shmId = SingleShmSetup(globalTraceBit, MAP_SIZE, SHM_ENV_VAR);
	global_state_shmid = SingleShmSetup(globalStateBit, MAP_SIZE, GLOBAL_STATE_ENV_VAR);
	variable_count_shmid = SingleShmSetup(variableStateChangeCount, sizeof(u32), VARIABLE_COUNT_SHM_ENV_VAR);

	branch_trace_shmid = SingleShmSetup(branchTraceBit, MAP_SIZE, BRANCH_TRACE_SHM_ENV_VAR);

	// error_size_shmId = SingleShmSetup(errorMap, MAX_MAP_SIZE, ERROR_MAP_SHM_ENV_VAR);
	error_list_shmId = SingleShmSetup(errorList, MAX_LIST_SIZE, ERROR_LIST_SHM_ENV_VAR);
	error_traced_shmId = SingleShmSetup(errorTraced, MAX_LIST_SIZE, ERROR_TRACED_SHM_ENV_VAR);

	u8 *list_tmp;
#ifndef NO_CONCURRENCY_FUZZ
	control_matrix_shmid = SingleShmSetup(list_tmp, CONTROL_MATRIX_SIZE * sizeof(matrix_element) / sizeof(u8), CONTROL_MATRIX_SHM_ENV_VAR);
	control_matrix = (matrix_element *)list_tmp;
	adjacency_matrix_shmid = SingleShmSetup(list_tmp, ADJACENCY_MATRIX_SIZE * sizeof(matrix_element) / sizeof(u8), ADJACENCY_MATRIX_SHM_ENV_VAR);
	adjacency_matrix = (matrix_element *)list_tmp;
	travel_matrix_shmid = SingleShmSetup(list_tmp, TRAVEL_MATRIX_SIZE * sizeof(matrix_element) / sizeof(u8), TRAVEL_MATRIX_SHM_ENV_VAR);
	travel_matrix = (matrix_element *)list_tmp;
#endif
}

void ShmRemove(void) {
	shmctl(shmId, IPC_RMID, NULL);
	shmctl(error_list_shmId, IPC_RMID, NULL);
	shmctl(error_traced_shmId, IPC_RMID, NULL);

	shmctl(control_matrix_shmid, IPC_RMID, NULL);
	shmctl(adjacency_matrix_shmid, IPC_RMID, NULL);
	shmctl(travel_matrix_shmid, IPC_RMID, NULL);
}

u8 *multi_proc_shm_create(string shm_file, int mem_size) {
	s32 shm_id = shmget(IPC_PRIVATE, mem_size, IPC_CREAT|IPC_EXCL|0600);
	if (shm_id < 0) {
		cout << "fail to shmget() in multi_proc_shm_create()" << endl;
		exit(-1);
	}

	string shm_str = to_string(shm_id);
	ofstream outfile(shm_file, ios::out|ios::binary);
	outfile << shm_str;
	outfile.close();

	u8 *mem_ptr = (u8 *)shmat(shm_id, NULL, 0);
	if (!mem_ptr) {
		cout << "fail to shmat() in multi_proc_shm_create()" << endl;
		exit(-1);
	}

	memset(mem_ptr, 255, mem_size); // init the share memory to 0
	return mem_ptr;
}

u8 *multi_proc_shm_get(string shm_file) {
	string shm_str;
	ifstream infile(shm_file, ios::in|ios::binary);
	infile >> shm_str;
	infile.close();

	s32 shm_id = stoi(shm_str);
	u8 *mem_ptr = (u8 *)shmat((int)shm_id, NULL, 0);

	return mem_ptr;
}