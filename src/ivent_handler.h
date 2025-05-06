#pragma once

#include <string>
#include <vector>
#include <map>
#include <deque>
#include <iostream>

struct ivent {
    int time, id;
    std::string name;
    int table;
    ivent(int time_, int id_, std::string& name_, int table_): time(time_), id(id_), name(name_), table(table_ - 1) {}
    ivent(int time_, int id_, std::string name_): time(time_), id(id_), name(name_), table(-1) {}
};

struct client {
    bool inside;
    bool waiting;
    bool playing;
    int table;
};

class ivent_handler {
public:
    ivent_handler(int tables_, int start_time_, int end_time_, int cost_): 
    tables(tables_), tables_all(tables_), start_time(start_time_), end_time(end_time_), cost(cost_), table_ocupied(tables_, -1), table_cost(tables_), table_time(tables_) {}
    void handle_ivent (ivent&);
    void print_ivents();
private:
    int tables;
    size_t tables_all;
    int start_time;
    int end_time;
    int cost;
    std::vector<int> table_ocupied; 
    int free_tables;
    std::vector<int> table_cost;
    std::vector<int> table_time;
    std::vector<ivent> ivents;
    std::map<std::string, client> maped;
    std::deque<std::string> waiting_queue;
};
