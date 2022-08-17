#include "dst_sequence_coverage_constants.h"
#include "dst_share_mem_util.h"
#include <dst_log.hpp>

#include <boost/filesystem.hpp>
#include <diff_match_patch.h>

/* TODO: handling exceptions */
static std::string read_file_content(std::string path)
{
    std::ifstream t(path);
    std::stringstream buffer;
    buffer << t.rdbuf();
    return buffer.str();
}

/* TODO: handling exceptions */
static void write_file_content(std::string path, std::string content)
{
    std::ofstream o(path);
    std::stringstream buffer(content);
    buffer >> o.rdbuf();
}

static double get_sequence_score(std::string sequence1, std::string sequence2)
{
    diff_match_patch<std::string> dmp;
    diff_match_patch<std::string>::Diffs diffs = dmp.diff_main(sequence1, sequence2);
    int total_lines = 0;
    int diff_lines = 0;
    int insertions = 0;
    int deletions = 0;
    for (auto diff : diffs)
    {
        switch (diff.operation)
        {
        case diff_match_patch<std::string>::INSERT:
            insertions += 1;
            break;
        case diff_match_patch<std::string>::DELETE:
            deletions += 1;
            break;
        case diff_match_patch<std::string>::EQUAL:
            // A deletion and an insertion is one substitution.
            diff_lines += std::max(insertions, deletions);
            total_lines += std::max(insertions, deletions);
            total_lines += 2;
            insertions = 0;
            deletions = 0;
            break;
        }
    }
    return (double)diff_lines / total_lines;
}

static void dst_test_event_sequence_coverage(void)
{
    /* check folder existance */
    if (!boost::filesystem::exists(SEQUENCE_DIR))
    {
        LOG_ERROR << "sequence folder not exists, skip calculating sequence coverage\n";
        return;
    }
    else if (boost::filesystem::is_regular_file(SEQUENCE_DIR))
    {
        LOG_ERROR << "sequence folder is a file (which should be wrong), skip calculating sequence coverage\n";
        return;
    }
    if (!boost::filesystem::exists(TOTAL_SEQUENCE_FOLDER))
    {
        boost::filesystem::create_directory(TOTAL_SEQUENCE_FOLDER);
    }
    else if (boost::filesystem::is_regular_file(TOTAL_SEQUENCE_FOLDER))
    {
        LOG_ERROR << "total sequence folder is a file (which should be wrong), skip calculating sequence coverage\n";
        return;
    }

    /* read current sequences */
    std::list<std::string> current_sequences;
    for (auto sequence_file : boost::filesystem::directory_iterator(SEQUENCE_DIR))
    {
        current_sequences.push_back(read_file_content(sequence_file.path().string()));
    }

    if (boost::filesystem::detail::is_empty(TOTAL_SEQUENCE_FOLDER))
    {
        LOG_ERROR << "total sequence folder is empty" << std::endl;
        write_file_content(std::string(TOTAL_SEQUENCE_FOLDER) + "/sequence_" + std::to_string(random()),
                           current_sequences.back());
    }

    /* compare with total_sequences */
    for (auto current_sequence : current_sequences)
    {
        bool is_new_sequence = true;
        for (auto sequence_file : boost::filesystem::directory_iterator(TOTAL_SEQUENCE_FOLDER))
        {
            std::string tmp_sequence = read_file_content(sequence_file.path().string());
            double score = get_sequence_score(current_sequence, tmp_sequence);
            if (score < SEQUENCE_DIFF_SCORE)
            {
                is_new_sequence = false;
                break;
            }
            LOG_INFO << "get score: " << score << std::endl;
        }
        if (is_new_sequence)
        {
            uint64_t rand = random();
            write_file_content(std::string(TOTAL_SEQUENCE_FOLDER) + "/sequence_" + std::to_string(rand),
                               current_sequence);
            increase_coverage(rand);
        }
    }
}