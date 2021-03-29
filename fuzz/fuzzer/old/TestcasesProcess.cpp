#include "Fuzzer.h"

u64 GetFileSize(const char *filename) {
	FILE *pfile = fopen(filename, "rb");
	if (!pfile) {
		cout << "fopen() fail in GetFileSize" << endl;
		exit(-1);
	}
	fseek(pfile, 0, SEEK_END);
	u64 size = ftell(pfile);
	fclose(pfile);
	// cout << size << endl;
	return size;
}

void AddToQueue(const char* fname, u64 fsize, 
				const char *elist_fname,
				const char *cm_file) {
	seed s;
	s.fileName = fname;
	s.fileLen = fsize;
	if (elist_fname != NULL) {
		s.elistFileName = elist_fname;
	}
	if (cm_file != NULL) {
		s.control_matrix_file = cm_file;
	}
	// cout << string(fname) << endl;
	queue.push_back(s);
}

void ReadTestcases(string dirOfCases) {
	
	DIR *dirp = opendir(dirOfCases.c_str());
	dirent* dp;
	while ((dp = readdir(dirp))!= NULL) {
		if (strcmp(dp->d_name, ".") && strcmp(dp->d_name, "..")) {
			// cout << dirOfCases + string(dp->d_name) << endl;
			u64 fsize = GetFileSize((dirOfCases + string(dp->d_name)).c_str());
			AddToQueue((dirOfCases + string(dp->d_name)).c_str(), fsize);
			// cout << "Input files are" << endl;
			// cout << dirOfCases + string(dp->d_name) << endl;
			// queue.back().trimDone = true;
		}
	}
	closedir(dirp);
}