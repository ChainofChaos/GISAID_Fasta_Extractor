#include "argparse/argparse.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
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

int main(int argc, char *argv[]) {
    argparse::ArgumentParser program("GISAID_Fasta_Extractor");
    program.add_argument("fastafile")
            .help("Specify the path of fasta file input")
            .required();
    try {
        program.parse_args(argc, argv);
    }
    catch (const std::runtime_error &err) {
        std::cerr << err.what() << std::endl;
        std::cerr << program;
        std::exit(1);
    }
    std::string textFile;
    std::ifstream fastaFile(program.get<std::string>("fastafile"));
    std::map<std::string, std::map<std::string, int>> count;
    std::vector<std::string> countries;
    std::filesystem::create_directory("Countries");
    std::filesystem::create_directory("Proteins");
    if (fastaFile.is_open()) {
        std::cout << "Extracting Information..." << std::endl;
        bool isHuman = true;
        std::string header, protein, id, sequence, country, date;
        std::string previousId = "";
        std::regex epiRegex("EPI_[A-Z0-9_]+");
        std::regex dateRegex(R"(20\d{2}-\d{2}-\d{2})");
        while (std::getline(fastaFile, textFile)) {
            if (textFile[0] == '>') {
                std::vector<std::string> splittedString = splitString(textFile, '|');
                splittedString.resize(11);
                if (!splittedString[6].empty()) {
                    if (splittedString[6] != "Human" || splitString(splittedString[2], '-')[1] == "00") {
                        isHuman = false;
                        continue;
                    }
                }
                isHuman = true;
                protein = splittedString[0].substr(1);
                std::smatch m;
                std::regex_search(textFile, m, epiRegex);
                id = m[0];
                std::regex_search(textFile, m, dateRegex);
                date = m[0];
                country = splitString(splittedString[1], '/')[1];
                if (std::find(countries.begin(), countries.end(), country) == countries.end()) {
                    countries.push_back(country);
                }
                header = textFile;
            } else {
                if (textFile.empty()) continue;
                if (!isHuman) {
                    continue;
                }
                sequence = textFile;
                if (id != previousId) {
                    count[date.substr(0, date.find_last_of('-'))][country] += 1;
                    previousId = id;
                }
                std::ofstream outProteinFastaFile("Proteins/" + protein + ".fasta", std::ios_base::app);
                outProteinFastaFile << header << '\n';
                outProteinFastaFile << sequence << '\n';
                std::ofstream outCountryFastaFile("Countries/" + country + ".fasta", std::ios_base::app);
                outCountryFastaFile << header << '\n';
                outCountryFastaFile << sequence << '\n';
            }
        }
    } else {
        throw std::invalid_argument("File doesn't exist or access is denied. Please check your input.");
    }
    std::cout << "Creating CSV..." << std::endl;
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
    std::cout << "Finished!" << std::endl;
    return 0;
}
