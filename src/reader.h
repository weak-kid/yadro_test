#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include <optional>
#include <vector>

class reader {
public:
    reader(std::ifstream& in);
    bool is_number(const std::string& s);
    bool is_time(const std::string& s);
    bool is_name(const std::string &s);
    int get_time(const std::string &s);
    bool read_line();
    bool try_read_line();
    std::vector<std::string> split(const std::string& s);
    std::string get_line();
    void wrong_input_file();
    void close();
private:
    size_t current_line = 0;
    std::string line = "";
    std::ifstream in;
};