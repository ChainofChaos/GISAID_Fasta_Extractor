#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>

std::vector<std::string> splitString(const std::string &text, char delim) {
    std::vector<std::string> splits;
    std::stringstream ss;
    ss << text;
    std::string word;
    while (std::getline(ss, word, delim)) {
        splits.push_back(word);
    }
    return splits;
}

int main() {
    std::ifstream fastaFile(R"(D:\source\Python\Gisaid\spikeprot0628\spikeprot0628.fasta)");
    std::string textFile;
    std::filesystem::create_directory("Countries");
    if (fastaFile.is_open()) {
        bool isHuman = true;
        std::string header, sequence, country;
        while (std::getline(fastaFile, textFile)) {
            if (textFile[0] == '>') {
                std::vector<std::string> splittedString = splitString(textFile, '|');
                if (splittedString[6] != "Human" || splitString(splittedString[2], '-')[1] == "00") {
                    isHuman = false;
                    continue;
                }
                isHuman = true;
                country = splitString(splittedString[1], '/')[1];
                header = textFile;
            } else {
                if (!isHuman) {
                    continue;
                }
                sequence = textFile;
                std::ofstream outCountryFastaFile("Countries/" + country + ".fasta", std::ios_base::app);
                outCountryFastaFile << header << '\n';
                outCountryFastaFile << sequence << '\n';
            }
        }
    }
    return 0;
}
