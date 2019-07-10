#include <iostream>
#include <regex>
#include <string>

using std::cout;
using std::endl;
using std::cin;
using std::string;

void test()
{
    string pattern("[^c]ei");
    pattern = "[[:alpha:]]*" + pattern + "[[:alpha:]]*";
    std::regex r(pattern);
    std::smatch results;

    string test_str = "receipt freind theif receive";
    if (regex_search(test_str, results, r)) {
        cout << results.str() << endl;
    }
}

int main()
{
    test();

    return 0;
}

