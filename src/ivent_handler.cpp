#include "ivent_handler.h"

void ivent_handler::handle_ivent (ivent& iv) {
    ivents.push_back(iv);
    if (iv.id == 1) {
        if (maped[iv.name].inside) {
            ivents.emplace_back(iv.time, 13, "YouShallNotPass");
            return;
        }
        if (iv.time < start_time || iv.time > end_time) {
            ivents.emplace_back(iv.time, 13, "NotOpenYet");
            return;
        }
        maped[iv.name].inside = true;
    }
    if (iv.id == 2) {
        if (!maped[iv.name].inside) {
            ivents.emplace_back(iv.time, 13, "ClientUnknown");
            return;
        }
        if (table_ocupied[iv.table] != -1) {
            ivents.emplace_back(iv.time, 13, "PlaceIsBusy");
            return;
        }
        if (maped[iv.name].playing) {
            table_time[maped[iv.name].table] += iv.time - table_ocupied[maped[iv.name].table];
            table_cost[maped[iv.name].table] += cost * ((iv.time - table_ocupied[maped[iv.name].table] + 59) / 60);
            table_ocupied[maped[iv.name].table] = -1;
            tables++;
        }
        table_ocupied[iv.table] = iv.time;
        maped[iv.name].table = iv.table;
        maped[iv.name].playing = true;
        tables--;
    }
    if (iv.id == 3) {
        if (!maped[iv.name].inside) {
            ivents.emplace_back(iv.time, 13, "ClientUnknown");
            return;
        }
        if (tables != 0 || maped[iv.name].playing) {
            ivents.emplace_back(iv.time, 13, "ICanWaitNoLonger!");
            return;
        }
        if (waiting_queue.size() > tables_all) {
            maped[iv.name].inside = false;
            maped[iv.name].playing = false;
            maped[iv.name].waiting = false;
            ivents.emplace_back(iv.time, 11, iv.name);
            return;
        }
        maped[iv.name].waiting = true;
        waiting_queue.push_back(iv.name);
    }
    if (iv.id == 4) {
        if (!maped[iv.name].inside) {
            ivents.emplace_back(iv.time, 13, "ClientUnknown");
            return;
        }
        maped[iv.name].inside = false;
        table_time[maped[iv.name].table] += iv.time - table_ocupied[maped[iv.name].table];
        table_cost[maped[iv.name].table] += cost * ((iv.time - table_ocupied[maped[iv.name].table] + 59) / 60);
        table_ocupied[maped[iv.name].table] = -1;
        tables++;
        if (maped[iv.name].playing && !waiting_queue.empty()) {
            ivents.emplace_back(iv.time, 12, waiting_queue[0], maped[iv.name].table);
            maped[waiting_queue[0]].table = maped[iv.name].table;
            maped[waiting_queue[0]].waiting = false;
            maped[waiting_queue[0]].playing = true;
            waiting_queue.pop_front();
            table_ocupied[maped[iv.name].table] = iv.time;
            tables--;
        }
        maped[iv.name].playing = false;
        maped[iv.name].waiting = false;
    }
}

std::string print_time(int x) {
    std::string res;
    int h = x / 60;
    int m = x % 60;
    res.push_back('0' + h / 10);
    res.push_back('0' + h % 10);
    res.push_back(':');
    res.push_back('0' + m / 10);
    res.push_back('0' + m % 10);
    return res;
}

void ivent_handler::print_ivents() {
    for (auto& el: maped) {
        if (el.second.inside) {
            ivents.emplace_back(end_time, 11, el.first);
            if (maped[el.first].playing) {
                table_time[maped[el.first].table] += end_time - table_ocupied[maped[el.first].table];
                table_cost[maped[el.first].table] += cost * ((end_time - table_ocupied[maped[el.first].table] + 59) / 60);
            }
        }
    }
    std::cout << print_time(start_time) << std::endl;
    for (ivent& iv: ivents) {
        std::cout << print_time(iv.time) << " " << iv.id << " " << iv.name;
        if (iv.table != -1) {
            std::cout << " " << iv.table + 1;
        }
        std::cout << std::endl;
    }
    std::cout << print_time(end_time) << std::endl;
    for (int i = 0; i < tables_all; ++i) {
        std::cout << i + 1 << " " << table_cost[i] << " " << print_time(table_time[i]) << std::endl;
    }
}