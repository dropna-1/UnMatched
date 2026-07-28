#pragma once
#include <iostream>
#include <vector>
using namespace std;

class Space {
public:
    vector<int> neighbors;
    vector<int> zone;
    Space(vector<int> neighbors, vector<int> zone) 
    :neighbors(neighbors), zone(zone){}
};

class Board {
    vector<Space> spaces;
public:
    Board(){
        spaces.push_back({{1, 2, 11, 19, 23}, {1}}); /*secret*/
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
        spaces.push_back({{0, 9, 19, 23}, {3}}); /*secret*/
        spaces.push_back({{3, 4, 14}, {1, 4}});
        spaces.push_back({{2, 14, 22}, {4}});
        spaces.push_back({{12, 13}, {4}});
        spaces.push_back({{10, 16, 17}, {5}});
        spaces.push_back({{1, 2}, {5}});
        spaces.push_back({{8, 15, 18, 19, 28, 29}, {5, 6}});
        spaces.push_back({{16, 17}, {5}});
        spaces.push_back({{0, 11, 23, 17, 20}, {6}}); /*secret*/
        spaces.push_back({{19, 21, 26}, {6}});
        spaces.push_back({{20, 22}, {4, 6}});
        spaces.push_back({{13, 21, 23}, {4, 7}});
        spaces.push_back({{0, 11, 19, 22, 24}, {7}}); /*secret*/
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