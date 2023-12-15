#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <unordered_map>
using namespace std;

string unfold_pattern(string pattern) {
    return pattern+"?"+pattern+"?"+pattern+"?"+pattern+"?"+pattern;
}

string get_pattern(string line, int &pos) {
    string pattern = "";
    while (line[pos] != ' ') {
        pattern += line[pos];
        pos++;
    }
    //Part 1
    // return pattern;
    //Part 2
    return unfold_pattern(pattern);
}

bool isNum(char c) {
    return ('0' <= c && c <= '9');
}

list<int> unfoldedNums(list<int> nums) {
    list<int> unfolded;
    for (int i = 0; i < 5; i ++) {
        for (list<int>::iterator numIt = nums.begin(); numIt != nums.end(); numIt++) {
            unfolded.push_back(*numIt);
        }
    }
    return unfolded;
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
    // Part 1
    // return nums;
    // Part 2
    return unfoldedNums(nums);
}

bool pattern_match(string pattern, string testString) {
    bool isValid = true;
    if (testString.length() > 0) {
        if (pattern.length() == testString.length()) {
            for (int i = 0; i < pattern.length() && i < testString.length() && isValid; i++) {
                isValid = (pattern[i] == '?' || pattern[i] == testString[i]);
            }
        }
        else {
            isValid = false;
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

bool is_match_at_pos(string pattern, string testString, int pos) {
    bool isMatch = true;

    if (pos+testString.length() < pattern.length()) {
        for (int i = pos; i < pos+testString.length() && isMatch; i++) {
            isMatch = (pattern[i] == '?' || pattern[i] == testString[i-pos]);
        }
    }

    return isMatch;
}

list<string> generateCombinations(list<string> &hashStrings, list<string>::iterator currHashString, string pattern, string baseString, unordered_map<string, list<string>> &memoMap) {
    list<string> combinations;
    unordered_map<string, list<string>>::iterator mapIt = memoMap.find(baseString);
    if (mapIt != memoMap.end()) {
        combinations = mapIt->second;
    }
    else {
        if (currHashString == hashStrings.end()) {
            string newBaseString = baseString;
            while (newBaseString.length() < pattern.length()) {
                newBaseString+=".";
            }
            if (pattern_match(pattern, newBaseString)) {
                combinations.push_back(newBaseString);
            }
        }
        else {
            // get how many more characters remain in the hash hashStrings
            int chars_remain=0;
            list<string>::iterator it = currHashString;
            it++;
            while (it != hashStrings.end()) {
                chars_remain += (*it).length();
                it++;
            }
            // add periods to front of current hash string
            string currentString = *currHashString;
            while (baseString.length() + currentString.length() + chars_remain <= pattern.length()) {
                if (is_match_at_pos(pattern, currentString, baseString.length())) {
                    it = currHashString;
                    it++;
                    mapIt = memoMap.find(baseString+currentString);
                    if (mapIt != memoMap.end()) {
                        combinations.merge(mapIt->second);
                    }
                    else {
                        combinations.merge(generateCombinations(hashStrings, it, pattern, baseString+currentString, memoMap));
                    }
                }
                currentString.insert(0, ".");
            }

        }

        combinations.sort();
        combinations.unique();

        memoMap.emplace(baseString, combinations);
    }

    return combinations;
}

int main(int argc, char **argv) {
    ifstream input_file;
    input_file.open("input.txt");
    string line;
    int validArrangements = 0;
    int rownum = 0;

    while (getline(input_file, line)) {
        int pos=0;

        string pattern = get_pattern(line, pos); // the pattern to be matched
        pos++;
        list<int> nums = get_nums(line, pos); // numbers at the end of the line to fit the pattern

        int baseLength = 0;
        list<string> hashStrings;
        list<int>::iterator currNum = nums.begin();
        string currHashString = build_hash_string(*currNum);
        hashStrings.push_back(currHashString);
        baseLength+=*currNum;
        currNum++;
        while (currNum != nums.end()) {
            currHashString = "."+build_hash_string(*currNum);
            hashStrings.push_back(currHashString);
            baseLength+=*currNum+1;
            currNum++;
        }
        int periods_to_add = pattern.length() - baseLength;

        unordered_map<string, list<string>> memoMap;

        int validThisCycle=0;
        if (periods_to_add > 0) {
            string baseString = "";
            list<string>::iterator currentHashString = hashStrings.begin();
            list<string> testStrings = generateCombinations(hashStrings, currentHashString, pattern, baseString, memoMap);
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
