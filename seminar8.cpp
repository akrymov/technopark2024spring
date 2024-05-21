#include <iostream>

#include <iostream>
#include <array>
#include <vector>
#include <map>
#include <queue>

static const size_t LINE_SIZE = 4;
static const size_t FIELD_SIZE = LINE_SIZE * LINE_SIZE;

using FieldArray = std::array<uint8_t, FIELD_SIZE>;

const FieldArray EMPTY_STATE = {
    0, 0, 0,0,//2+1 % 3 == 0
    0, 0, 0,0,//5+1 % 3 == 0
    0, 0, 0,0,//8
    0, 0, 0,0//8

};

const FieldArray GOAL_STATE = {
    1, 2, 3,//0
    4, 5, 6,//3
    7, 8,9,10,11,12,13,14,15,0//6 % 3 == 0
};

class FieldState {
public:
    FieldState(const FieldArray& arr) : state(arr) {
        for (uint8_t i = 0; i < state.size(); ++i) {
            if (state[i] == 0) {
                zero_pos = i;
                break;
            }
        }
    }

    // FieldState(const FieldState& fs) : state(fs.state), zero_pos(fs.zero_pos) {
    // }
    FieldState(const FieldState& fs)=default;

    bool operator==(const FieldState& r) const {
        return state == r.state;
    }

    bool operator!=(const FieldState& r) const {
        return state != r.state;
    }

    bool operator<(const FieldState& r) const {
        return state < r.state;
    }

    std::vector<FieldState> GetNextVertices() const {
        std::vector<FieldState> result;
        result.reserve(4);

        if (zero_pos >= LINE_SIZE) { //up
            result.push_back(moveUp());
        }
        if (zero_pos < (FIELD_SIZE - LINE_SIZE)) { //down
            result.push_back(moveDown());
        }
        if ((zero_pos + 1) % LINE_SIZE != 0) { //right
            result.push_back(moveRight());
        }
        if (zero_pos % LINE_SIZE != 0) { //left
            result.push_back(moveLeft());
        }
        return result;
    }

    void print() const {
        for (int i = 0; i < state.size(); ++i) {
            std::cout << (int)state[i] << " ";
            if ((i + 1) % LINE_SIZE == 0) {
                std::cout << std::endl;
            }
        }
    }

private:
    FieldState moveUp() const {
        FieldState fs(*this);
        uint8_t new_zero_pos = fs.zero_pos - LINE_SIZE;
        std::swap(fs.state[fs.zero_pos], fs.state[new_zero_pos]);
        fs.zero_pos = new_zero_pos;
        return fs;
    }

    FieldState moveDown() const {
        FieldState fs(*this);
        uint8_t new_zero_pos = fs.zero_pos + LINE_SIZE;
        std::swap(fs.state[fs.zero_pos], fs.state[new_zero_pos]);
        fs.zero_pos = new_zero_pos;

        return fs;
    }

    FieldState moveRight() const {
        FieldState fs(*this);
        uint8_t new_zero_pos = fs.zero_pos + 1;
        std::swap(fs.state[fs.zero_pos], fs.state[new_zero_pos]);
        fs.zero_pos = new_zero_pos;

        return fs;
    }

    FieldState moveLeft() const {
        FieldState fs(*this);
        uint8_t new_zero_pos = fs.zero_pos - 1;
        std::swap(fs.state[fs.zero_pos], fs.state[new_zero_pos]);
        fs.zero_pos = new_zero_pos;

        return fs;
    }

    FieldArray state;
    uint8_t zero_pos;

};

void bfs(const FieldState& start) {
    const FieldState goal(GOAL_STATE);
    const FieldState empty(EMPTY_STATE);

    //key - to, value - from
    std::map<FieldState, FieldState> parents;
    std::queue<FieldState> q;
    parents.insert(std::make_pair(start, empty));
    q.push(start);

    FieldState cur(EMPTY_STATE);
    while (!q.empty()) {
        cur = q.front();
        q.pop();

        if (cur == goal) {
            break;
        }

        std::vector<FieldState> children = cur.GetNextVertices();
        for (FieldState& st : children) {
            if (parents.find(st) == parents.end()) {
                parents.insert(std::make_pair(st, cur));
                q.push(st);
            }
        }
    }
    int steps_count = 0;
    if (cur == goal) {
        while (cur != empty) {
            steps_count++;
            cur.print();

            auto it = parents.find(cur);
            cur = it->second;
            std::cout << "-------------------" << std::endl;
        }
        std::cout << "Solution found, steps_count=" << steps_count << std::endl;

    } else {
        std::cout << "Solution NOT found, vertices count=" <<  parents.size() << std::endl;
    }

}

int main() {
    FieldState start({
        1,2,3,4,
        0,5,6,8,
        9,10,7,11,
        13,14,15,12
    });
    bfs(start);
}
