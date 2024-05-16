#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cctype>

#include <termcolor/termcolor.hpp>

std::string convert_to_snake_case(const std::string& str) {
    std::string result;
    for (char c : str) {
        if (isupper(c)) {
            result += '_';
            result += tolower(c);
        }
        else {
            result += c;
        }
    }
    return result;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "_usage: " << argv[0] << " [-file1.cpp] [-file2.cpp] ... [-file_n.cpp]" << std::endl;
        return 1;
    }

    std::vector<std::string> file_paths;
    for (int i = 1; i < argc; ++i) {
        file_paths.emplace_back(argv[i]);
    }

    for (const auto& file_path : file_paths) {
        std::ifstream file(file_path);
        if (!file.is_open()) {
            std::cerr << "_failed to open file: " << file_path << std::endl;
            continue;
        }

        std::string file_content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        file.close();

        std::string modified_content;
        std::string current_word;
        for (char c : file_content) {
            if (isupper(c)) {
                if (!current_word.empty()) {
                    modified_content += convert_to_snake_case(current_word);
                    current_word.clear();
                }
            }
            current_word += c;
        }

        if (!current_word.empty()) {
            modified_content += convert_to_snake_case(current_word);
        }

        std::ofstream output_file(file_path);
        if (!output_file.is_open()) {
            std::cerr << "_failed to open output file: " << file_path << std::endl;
            continue;
        }

        output_file << modified_content;
        output_file.close();

        std::cout << termcolor::green << "File processed: " << file_path << std::endl;
    }

    return 0;
}