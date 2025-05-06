#include "reader.h"


reader::reader(std::ifstream& in_) {
    in = move(in_);
}

bool reader::is_number(const std::string &s)
{
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    if (!s.empty() && it == s.end()) {
        return true;
    }
    wrong_input_file();
    return false;
}

bool reader::is_time(const std::string &s)
{
    if (s.size() == 5 && 
        std::isdigit(s[0]) && 
        std::isdigit(s[1]) &&
        std::isdigit(s[3]) && 
        std::isdigit(s[4]) &&
        s[2] == ':') {
        return true;
    }
    wrong_input_file();
    return false;
}

bool reader::is_name(const std::string &s)
{
    std::string::const_iterator it = s.begin();
    while (it != s.end() && (std::isdigit(*it) || ('a' <= *it && *it <= 'z') || *it == '-' || *it == '_')) ++it;
    if (!s.empty() && it == s.end()) {
        return true;
    }
    wrong_input_file();
    return false;
}

int reader::get_time(const std::string &s) {
    return ((s[0] - '0') * 10 + (s[1] - '0')) * 60 + ((s[3] - '0') * 10 + (s[4] - '0'));
}

void reader::wrong_input_file() {
    std::cout << "wrong input file format at line " << current_line << "\n";
    in.close();
}

bool reader::read_line() {
    if (!try_read_line()) {
        wrong_input_file();
        return false;
    }
    return true;
}

bool reader::try_read_line() {
    if (getline(in, line)) {
        current_line++;
        return true;
    }
    return false;
}


std::vector<std::string> reader::split(const std::string& s) {
    std::vector<std::string> tokens;
    std::string token;
    size_t start = 0;
    size_t end;
    
    while ((end = s.find(' ', start)) != std::string::npos) {
        tokens.push_back(s.substr(start, end - start));
        start = end + 1;
    }
    
    tokens.push_back(s.substr(start));
    return tokens;
}

std::string reader::get_line() {
    return line;
}

void reader::close() {
    in.close();
}