#pragma once

#include <iostream>
#include <vector>

inline int row = -1, col = -1;
inline std::vector<uint8_t> neighbor_count;
inline std::vector<bool> is_alive;
inline std::vector<int> live_cells;
inline std::vector<int> modified_cells;

inline void Initialize() {
    std::cin >> col >> row;
    int total_cells = row * col;
    neighbor_count.assign((total_cells + 1) / 2, 0);
    is_alive.assign(total_cells, false);
    live_cells.clear();
    
    char c;
    int count = 0;
    int r = 0, c_idx = 0;
    
    while (std::cin >> c) {
        if (c == '!') break;
        if (c >= '0' && c <= '9') {
            count = count * 10 + (c - '0');
        } else {
            if (count == 0) count = 1;
            if (c == 'b') {
                c_idx += count;
            } else if (c == 'o') {
                for (int i = 0; i < count; ++i) {
                    if (r < row && c_idx < col) {
                        int key = r * col + c_idx;
                        is_alive[key] = true;
                        live_cells.push_back(key);
                    }
                    c_idx++;
                }
            } else if (c == '$') {
                r += count;
                c_idx = 0;
            }
            count = 0;
        }
    }
}

inline void Tick() {
    modified_cells.clear();
    for (int key : live_cells) {
        int r = key / col;
        int c = key % col;
        for (int dr = -1; dr <= 1; ++dr) {
            int nr = r + dr;
            if (nr < 0 || nr >= row) continue;
            for (int dc = -1; dc <= 1; ++dc) {
                if (dr == 0 && dc == 0) continue;
                int nc = c + dc;
                if (nc < 0 || nc >= col) continue;
                int nkey = nr * col + nc;
                
                int idx = nkey >> 1;
                int shift = (nkey & 1) << 2;
                if (((neighbor_count[idx] >> shift) & 15) == 0) {
                    modified_cells.push_back(nkey);
                }
                neighbor_count[idx] += (1 << shift);
            }
        }
    }
    
    std::vector<int> next_live_cells;
    for (int key : modified_cells) {
        int idx = key >> 1;
        int shift = (key & 1) << 2;
        int count = (neighbor_count[idx] >> shift) & 15;
        neighbor_count[idx] &= ~(15 << shift);
        
        if (count == 3 || (count == 2 && is_alive[key])) {
            next_live_cells.push_back(key);
        }
    }
    
    for (int key : live_cells) {
        is_alive[key] = false;
    }
    for (int key : next_live_cells) {
        is_alive[key] = true;
    }
    live_cells = std::move(next_live_cells);
}

inline void PrintGame() {
    std::cout << col << " " << row << "\n";
    
    int empty_rows = 0;
    bool has_output = false;
    
    for (int r = 0; r < row; ++r) {
        int last_alive = -1;
        for (int c = col - 1; c >= 0; --c) {
            if (is_alive[r * col + c]) {
                last_alive = c;
                break;
            }
        }
        
        if (last_alive == -1) {
            empty_rows++;
        } else {
            if (has_output) {
                int dollars = empty_rows + 1;
                if (dollars == 1) std::cout << "$";
                else std::cout << dollars << "$";
            } else {
                if (empty_rows > 0) {
                    if (empty_rows == 1) std::cout << "$";
                    else std::cout << empty_rows << "$";
                }
            }
            empty_rows = 0;
            has_output = true;
            
            int count = 0;
            char current = 'b';
            for (int c = 0; c <= last_alive; ++c) {
                char cell = is_alive[r * col + c] ? 'o' : 'b';
                if (cell == current) {
                    count++;
                } else {
                    if (count > 0) {
                        if (count == 1) std::cout << current;
                        else std::cout << count << current;
                    }
                    current = cell;
                    count = 1;
                }
            }
            if (count > 0) {
                if (count == 1) std::cout << current;
                else std::cout << count << current;
            }
        }
    }
    std::cout << "!\n";
}

inline int GetLiveCell() {
    return live_cells.size();
}
