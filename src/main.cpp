#include "ivent_handler.h"
#include "reader.h"

#include "stdexcept"
#include "fstream"
#include "iostream"
#include "optional"

std::optional<ivent> read_and_check(reader& reader_) {
    std::vector<std::string> splitted = reader_.split(reader_.get_line());
    if (splitted.size() == 4) {
        if (reader_.is_time(splitted[0]) && reader_.is_number(splitted[1]) && reader_.is_name(splitted[2]) && reader_.is_number(splitted[3])) {
            int id = stoi(splitted[1]);
            if (id != 2) {
                return std::nullopt;
            }
            return ivent(reader_.get_time(splitted[0]), stoi(splitted[1]), splitted[2], stoi(splitted[3]));
        }
    }
    if (splitted.size() == 3) {
        if (reader_.is_time(splitted[0]) && reader_.is_number(splitted[1]) && reader_.is_name(splitted[2])) {
            int id = stoi(splitted[1]);
            if (id < 1 || id > 4) {
                return std::nullopt;
            }
            return ivent(reader_.get_time(splitted[0]), stoi(splitted[1]), splitted[2], 0);
        }
    }
    return std::nullopt;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        throw std::runtime_error("wrond arguments");
    }
    std::ifstream in(argv[1]);
    reader reader_(in);
    int tables;
    if (reader_.read_line() && reader_.is_number(reader_.get_line())) {
        tables = stoi(reader_.get_line());
    } else {
        return 0;
    }
    int start_time, end_time;
    if (reader_.read_line()) {
        std::vector<std::string> splitted = reader_.split(reader_.get_line());
        if (splitted.size() != 2 || !(reader_.is_time(splitted[0]) && reader_.is_time(splitted[1]))) {
            reader_.wrong_input_file();
            return 0;
        }
        start_time = reader_.get_time(splitted[0]);
        end_time = reader_.get_time(splitted[1]);
    } else {
        return 0;
    }
    int cost;
    if (reader_.read_line() && reader_.is_number(reader_.get_line())) {
        cost = stoi(reader_.get_line());
    } else {
        return 0;
    }

    size_t linenum = 4;
    ivent_handler handler(tables, start_time, end_time, cost);
    int lasttime = -1;
    while (reader_.try_read_line()) {
        auto iv = read_and_check(reader_);
        if (iv == std::nullopt) {
            reader_.wrong_input_file();
            return 0;
        }
        ivent ivr = iv.value();
        if (lasttime > ivr.time) {
            reader_.wrong_input_file();
            return 0;
        }
        lasttime = ivr.time;
        handler.handle_ivent(ivr);
        linenum++;
    }
    handler.print_ivents();
    reader_.close();
}