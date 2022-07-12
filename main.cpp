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
    std::ifstream fastaFile(R"(D:\source\Python\Gisaid\spikeprot0628\spikeprot0628.fasta)");
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
                if (std::find(countries.begin(), countries.end(), country) != countries.end()) {
                    countries.push_back(country);
                }
                header = textFile;
            } else {
                if (!isHuman) {
                    continue;
                }
                sequence = textFile;
                count[date.substr(0, date.find_last_of('-'))][country] += 1;
//                for (auto const& [key,val] : count){
//                    for (auto const& [country, counts] : val){
//                        std::cout << "Key : " << key << " Country: " << country << " Counts : " << counts << std::endl;
//                    }
//                }
                std::ofstream outCountryFastaFile("Countries/" + country + ".fasta", std::ios_base::app);
                outCountryFastaFile << header << '\n';
                outCountryFastaFile << sequence << '\n';
            }
        }
    }
    std::ofstream outCount("DateCount.csv");
    outCount << "date,count\n";

//    for (auto const &[key, val]: count) {
//        outCount << key << ',' << val << '\n';
//    }
    return 0;
}
