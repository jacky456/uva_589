#include <iostream>
#include <vector>
#include <array>
#include <queue>
#include <map>
#include <string>
#include <algorithm>

#define BIG 200000
using namespace std;

class mycmp {
public:
    bool operator()(array<int, 5> &a, array<int, 5> &b) {
        return (a[4] > b[4]);
    }
};

void
init(vector<vector<int>> &mapa, int row, int col, array<int, 2> &start, array<int, 2> &box, array<int, 2> &target) {
    for (int i = 0; i < row; i++) {
        vector<int> r;
        for (int j = 0; j < col; j++) {
            r.push_back(1);
            char c;
            cin >> c;
            switch (c) {
                case '#':
                    r.back() = 0;
                    break;
                case 'S':
                    start[0] = i;
                    start[1] = j;
                    break;
                case 'B':
                    box[0] = i;
                    box[1] = j;
                    break;
                case 'T':
                    target[0] = i;
                    target[1] = j;
                    break;
                default:
                    break;
            }
        }
        mapa.push_back(r);
    }

}

array<int, 5> trans(array<int, 2> a, array<int, 2> b, int x) {
    array<int, 5> result = {a[0], a[1], b[0], b[1], x};

    return result;
};

array<array<int, 2>, 2> netrans(array<int, 2> a, array<int, 2> b) {
    array<array<int, 2>, 2> result = {a, b};

    return result;
};

array<array<int, 2>, 2> retrans(const array<int, 5> tmp) {
    array<int, 2> a = {tmp[0], tmp[1]}, b = {tmp[2], tmp[3]};
    array<array<int, 2>, 2> result = {a, b};

    return result;
};

array<int, 2> add(array<int, 2> a, array<int, 2> b) {
    array<int, 2> result = {a[0] + b[0], a[1] + b[1]};

    return result;
};

bool check(vector<vector<int>> &mapa, array<int, 2> next) {
    int i = next[0], j = next[1];
    if (i >= 0 && i < mapa.size() && j >= 0 && j < mapa[0].size())
        if (mapa[i][j] == 1)
            return true;

    return false;
}

void printPath(map<array<array<int, 2>, 2>, int> &step, map<array<array<int, 2>, 2>, int> &visited,
               array<array<int, 2>, 2> current) {
    string output;
    array<int, 2> north{1, 0}, south{-1, 0}, east{0, -1}, west{0, 1};
    vector<array<int, 2>> neighbors{north, south, east, west};  // 0 1 2 3

    while (visited[current] != 1){
        int dir = step[current];
        if (dir < 4)
            current = netrans(add(current[0], neighbors[dir]), add(current[1], neighbors[dir]));
        else
            current = netrans(add(current[0], neighbors[dir - 4]), current[1]);

        switch (dir) {
            case 0:
                output += "N";
                break;
            case 1:
                output += "S";
                break;
            case 2:
                output += "E";
                break;
            case 3:
                output += "W";
                break;
            case 4:
                output += "n";
                break;
            case 5:
                output += "s";
                break;
            case 6:
                output += "e";
                break;
            case 7:
                output += "w";
                break;
            default:
                break;
        }
    }

    reverse(output.begin(), output.end());
    cout << output << endl;

}

void findPath(vector<vector<int>> &mapa, array<int, 2> &start, array<int, 2> &box, array<int, 2> &target) {
    map<array<array<int, 2>, 2>, int> visited, step;
    priority_queue<array<int, 5>, vector<array<int, 5>>, mycmp> sequence;
    visited[netrans(start, box)] = 1;
    sequence.push(trans(start, box, 1));
    bool isprint = false;
    array<int, 2> north{-1, 0}, south{1, 0}, east{0, 1}, west{0, -1};
    vector<array<int, 2>> neighbors{north, south, east, west};  // 0 1 2 3

    while (!sequence.empty()) {
        array<array<int, 2>, 2> current = retrans(sequence.top());
        sequence.pop();

        if (current[1] == target) {
            isprint = true;
            printPath(step, visited, current);
            break;
        }

        for (int i = 0; i < neighbors.size(); i++) {
            if (current[1] == add(current[0], neighbors[i])) {
                if (check(mapa, add(current[1], neighbors[i]))) {
                    array<array<int, 2>, 2> next = netrans(add(current[0], neighbors[i]),
                                                           add(current[1], neighbors[i]));
                    if (visited.count(next) == 0)
                        visited[next] = 0;
                    if (visited.count(current) == 0)
                        visited[current] = 0;

                    int nex = visited[next];
                    int now = visited[current] + BIG;
                    if (nex > now || nex == 0) {
                        visited[next] = now;
                        step[next] = i;
                        sequence.push(trans(next[0], next[1], now));
                    }
                }
            } else if (check(mapa, add(current[0], neighbors[i]))) {
                array<array<int, 2>, 2> next = netrans(add(current[0], neighbors[i]), current[1]);
                if (visited.count(next) == 0)
                    visited[next] = 0;
                if (visited.count(current) == 0)
                    visited[current] = 0;

                int nex = visited[next];
                int now = visited[current] + 1;
                if (nex > now || nex == 0) {
                    visited[next] = now;
                    step[next] = i + 4;
                    sequence.push(trans(next[0], next[1], now));
                }
            }
        }
    }

    if (!isprint)
        cout << "Impossible." << endl;
}

int main() {
    int row, col;
    int casenum = 0;

    while (cin >> row >> col) {          //attention 0 0 will be read
        if (row == 0 && col == 0)
            break;
        casenum++;
        vector<vector<int>> mapa;
        array<int, 2> start{}, box{}, target{};

        init(mapa, row, col, start, box, target);
        cout << "Maze #" << casenum << endl;

        findPath(mapa, start, box, target);
        cout << endl;
    }
    return 0;
}
