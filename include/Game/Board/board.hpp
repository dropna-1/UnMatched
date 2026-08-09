#pragma once
#include <iostream>
#include <vector>

class Space {
public:
    std::vector<int> neighbors;
    std::vector<int> secret;
    std::vector<int> zone;
    Space(std::vector<int> neighbors, std::vector<int> zone, std::vector<int> secret={}) 
    :neighbors(neighbors), zone(zone), secret(secret) {}
};

class Board {
    std::vector<Space> spaces;
public:
    Board(){
        spaces.push_back({{1, 2}, {1}, {11, 19, 23}}); /*secret*/
        spaces.push_back({{0, 4}, {1}});
        spaces.push_back({{0, 3, 13}, {1}});
        spaces.push_back({{2, 12}, {1}});
        spaces.push_back({{1, 12, 5}, {1, 2}});
        spaces.push_back({{4, 6, 7}, {2}});
        spaces.push_back({{5, 7, 8}, {2}});
        spaces.push_back({{5, 6, 8}, {2}});
        spaces.push_back({{6, 7, 9, 10, 17}, {2, 3, 6}});
        spaces.push_back({{8, 10, 11}, {3}});
        spaces.push_back({{8, 9, 15}, {3}});
        spaces.push_back({{9}, {3}, {0, 19, 23}}); /*secret*/
        spaces.push_back({{3, 4, 14}, {1, 4}});
        spaces.push_back({{2, 14, 22}, {4}});
        spaces.push_back({{12, 13}, {4}});
        spaces.push_back({{10, 16, 17}, {5}});
        spaces.push_back({{15, 18, 31}, {5}});
        spaces.push_back({{8, 15, 18, 19, 28, 29}, {6, 5}});
        spaces.push_back({{16, 17}, {5}});
        spaces.push_back({{17, 20}, {6}, {0, 11, 23}}); /*secret*/
        spaces.push_back({{19, 21, 26}, {6}});
        spaces.push_back({{20, 22}, {4, 6}});
        spaces.push_back({{13, 21, 23}, {4, 7}});
        spaces.push_back({{22, 24}, {7}, {0, 11, 19}}); /*secret*/
        spaces.push_back({{23, 25}, {7}});
        spaces.push_back({{24, 26, 27}, {7}});
        spaces.push_back({{20, 25, 27, 28}, {6, 7}});
        spaces.push_back({{25, 26, 28}, {7}});
        spaces.push_back({{17, 26, 27, 29}, {7}});
        spaces.push_back({{17, 28, 30, 31}, {7}});
        spaces.push_back({{29, 31}, {7}});
        spaces.push_back({{16, 29, 30}, {5, 7}});
    }
    const Space& getSpace(int id) const{
        return spaces.at(id);
    }
    int size() const{
        return spaces.size();
    }
};