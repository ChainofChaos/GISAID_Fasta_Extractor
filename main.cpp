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
                if (splitString(textFile, '|')[6] != "Human") {
                    isHuman = false;
                    continue;
                }
                isHuman = true;
                country = splitString(textFile, '/')[1];
                header = textFile;
            } else {
                if (!isHuman) {
                    continue;
                }
                sequence = textFile;
                std::ofstream outCountryFastaFile("Countries/"+country + ".fasta", std::ios_base::app);
                outCountryFastaFile << header << '\n';
                outCountryFastaFile << sequence << '\n';
            }
        }
    }
    return 0;
}
