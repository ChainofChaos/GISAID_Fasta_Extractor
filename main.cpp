#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>
#include <map>

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
    std::ifstream fastaFile(R"(D:\source\Cpp\GISAID_Fasta_Extractor\test.fasta)");
    std::string textFile;
    std::map<std::string, std::map<std::string, int>> count;
    std::vector<std::string> countries;
    std::filesystem::create_directory("Countries");
    if (fastaFile.is_open()) {
        bool isHuman = true;
        std::string header, sequence, country, date;
        while (std::getline(fastaFile, textFile)) {
            if (textFile[0] == '>') {
                std::vector<std::string> splittedString = splitString(textFile, '|');
                if (splittedString[6] != "Human" || splitString(splittedString[2], '-')[1] == "00") {
                    isHuman = false;
                    continue;
                }
                isHuman = true;
                date = splittedString[2];
                country = splitString(splittedString[1], '/')[1];
                if (std::find(countries.begin(), countries.end(), country) == countries.end()) {
                    countries.push_back(country);
                }
                header = textFile;
            } else {
                if (!isHuman) {
                    continue;
                }
                sequence = textFile;
                count[date.substr(0, date.find_last_of('-'))][country] += 1;
                std::ofstream outCountryFastaFile("Countries/" + country + ".fasta", std::ios_base::app);
                outCountryFastaFile << header << '\n';
                outCountryFastaFile << sequence << '\n';
            }
        }
    }
    std::ofstream outCount("gisaid_monthly.csv");
    // Write headers
    for (auto const &[key, val]: count) {
        outCount << ',' + key;
    }
    outCount << '\n';
    // Write the countries count based on the date
    std::sort(countries.begin(), countries.end());
    for (const auto &country: countries) {
        outCount << country;
        for (auto const &[key, val]: count) {
            auto pos = val.find(country);
            if (pos == val.end()) {
                outCount << ",0";
            } else {
                outCount << ',' + std::to_string(pos->second);
            }
        }
        outCount << '\n';
    }
    return 0;
}
