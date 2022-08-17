#include "diff_match_patch.h"

#include <utils/dst_sequence_coverage.hpp>

#include <iostream>

using namespace std;

void test_diffs()
{

    diff_match_patch<string> dmp;
    diff_match_patch<string>::Diffs diffs = dmp.diff_main(
        R"(35 2740453
114 2741845
29 2740453
29 2741845
30 2740453
27 2741845
30 2740453
27 2741845
29 2740453
27 2741845
30 2740453
27 2741845
34 2740453
27 2741845
29 2740453
27 2741845
29 2740453
27 2741845
30 2740453
27 2741845
34 2740453
27 2741845
30 2740453
27 2741845
30 2740453
27 2741845
29 2740453
27 2741845
29 2740453
27 2741845
30 2740453
27 2741845
30 2740453
27 2741845
29 2740453
27 2741845
30 2740453
27 2741845
30 2740453
27 2741845
30 2740453
)",
        R"(102 2740453
114 2741845
29 2740453
29 2741845
29 2740453
27 2741845
30 2740453
27 2741845
30 2740453
27 2741845
30 2740453
27 2741845
30 2740453
27 2741845
30 2740453
27 2741845
30 2740453
27 2741845
30 2740453
27 2741845
34 2740453
27 2741845
29 2740453
27 2741845
29 2740453
27 2741845
30 2740453
27 2741845
33 2740453
27 2741845
30 2740453
27 2741845
29 2740453
27 2741845
29 2740453
27 2741845
)");
    for (auto diff : diffs)
    {
        cout << diff.operation << diff.text << endl;
    }
    int total_lines = 0;
    int diff_lines = 0;
    int insertions = 0;
    int deletions = 0;
    for (auto diff : diffs)
    {
        switch (diff.operation)
        {
        case diff_match_patch<string>::INSERT:
            insertions += 1;
            break;
        case diff_match_patch<string>::DELETE:
            deletions += 1;
            break;
        case diff_match_patch<string>::EQUAL:
            // A deletion and an insertion is one substitution.
            diff_lines += std::max(insertions, deletions);
            total_lines += std::max(insertions, deletions);
            total_lines += 2;
            insertions = 0;
            deletions = 0;
            break;
        }
    }
    cout << "diff_lines = " << diff_lines << endl
         << "total_lines = " << total_lines << endl
         << (double)diff_lines / total_lines << endl;
}

void test_sequences() { dst_test_event_sequence_coverage(); }

int main()
{
    // test_diffs();
    test_sequences();
    return 0;
}