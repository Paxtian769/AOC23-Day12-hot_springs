#include <iostream>
#include <fstream>
#include <string>
#include <list>
using namespace std;

string get_pattern(string line, int &pos) {
    string pattern = "";
    while (line[pos] != ' ') {
        pattern += line[pos];
        pos++;
    }
    return pattern;
}

bool isNum(char c) {
    return ('0' <= c && c <= '9');
}

list<int> get_nums(string line, int &pos) {
    list<int> nums;
    while (pos < line.length()) {
        int currNum = 0;
        while (isNum(line[pos])) {
            currNum = (currNum * 10) + (line[pos] - '0');
            pos++;
        }
        nums.push_back(currNum);
        pos++;
    }
    return nums;
}

bool pattern_match(string pattern, string testString) {
    bool isValid = true;
    if (testString.length() > 0) {
        for (int i = 0; i < pattern.length() && i < testString.length() && isValid; i++) {
            isValid = (pattern[i] == '?' || pattern[i] == testString[i]);
        }
    }
    return isValid;
}

string build_hash_string(int n) {
    string hash_string = "";
    for (int i = 0; i < n; i++) {
        hash_string += '#';
    }
    return hash_string;
}

list<string> generateCombinations(list<string> &words, int numPeriods, string pattern) {
    list<string> combinations;
    string testString;
    if (numPeriods == 0) {
        string newWord = "";
        for (list<string>::iterator word = words.begin(); word != words.end(); word++) {
            newWord += *word;
        }
        if (pattern_match(pattern, newWord)) {
            combinations.push_back(newWord);
        }
    }
    else {
        string prefix = "";
        for (list<string>::iterator currWord = words.begin(); currWord != words.end(); currWord++) {
            list<string> wordCopy;
            if (prefix.length() > 0) {
                wordCopy.push_back(prefix);
            }
            if (pattern_match(pattern, prefix+".")) {
                int pos = prefix.length();
                bool valid = true;
                for (list<string>::iterator copyWord = currWord; copyWord != words.end(); copyWord++) {
                    if (copyWord == currWord) {
                        wordCopy.push_back("."+*copyWord);
                    }
                    else {
                        wordCopy.push_back(*copyWord);
                    }
                }
                combinations.merge(generateCombinations(wordCopy, numPeriods-1, pattern));
            }
            prefix += *currWord;
        }
        if (prefix.length() + numPeriods <= pattern.length() && pattern_match(pattern, prefix)) {
            list<string> wordCopy;
            wordCopy.push_back(prefix+".");
            combinations.merge(generateCombinations(wordCopy, numPeriods-1, pattern));
        }
    }

    combinations.sort();
    combinations.unique();

    return combinations;
}

int main(int argc, char **argv) {
    ifstream input_file;
    input_file.open("input.txt");
    string line;
    int validArrangements = 0;

    while (getline(input_file, line)) {
        int pos=0;

        string pattern = get_pattern(line, pos); // the pattern to be matched
        pos++;
        list<int> nums = get_nums(line, pos); // numbers at the end of the line to fit the pattern

        int baseLength = 0;
        list<string> hashStrings;
        list<int>::iterator currNum = nums.begin();
        hashStrings.push_back(build_hash_string(*currNum));
        baseLength+=*currNum;
        currNum++;
        while (currNum != nums.end()) {
            hashStrings.push_back("."+build_hash_string(*currNum));
            baseLength+=*currNum+1;
            currNum++;
        }
        int periods_to_add = pattern.length() - baseLength;

        int validThisCycle=0;
        if (periods_to_add > 0) {
            list<string> testStrings = generateCombinations(hashStrings, periods_to_add, pattern);
            validArrangements += testStrings.size();
            validThisCycle = testStrings.size();
        }
        else {
            validThisCycle++;
            validArrangements++;
        }

    }

    cout << "Number of valid arrangements is " << validArrangements << endl;

    return 0;
}
