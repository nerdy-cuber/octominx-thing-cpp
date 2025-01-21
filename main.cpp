#include <iostream>
#include <ostream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>
#include <sstream>
#include "octominx.h"

std::vector<std::string> splitStringBySpaces(const std::string& str) {
    std::vector<std::string> words;
    std::istringstream iss(str);
    for (std::string s; iss >> s;) {
        words.push_back(s);
    }
    return words;
}

auto scrambleToMoves(std::string_view const scramble) {
    std::unordered_map<char, Face> faceMap = {{'U', U}, {'L', L}, {'F', F}, {'R', R},
                                              {'B', B}, {'D', D}, {'N', N}, {'T', T}};
    std::unordered_map<char, Direction> directionMap = {{' ', CW}, {'\'', CCW}};
    std::vector<Move> moves;

    // Split the scramble string by spaces
    const std::vector<std::string> words = splitStringBySpaces(std::string(scramble));

    for (const auto& word : words) {
        if (faceMap.find(word[0]) != faceMap.end()) {
            moves.emplace_back(faceMap[word[0]], CW);
            if (word.size() > 1 && directionMap.find(word[1]) != directionMap.end()) {
                moves.back().direction = directionMap[word[1]];
            }
        }
    }

    return moves;
}

int main() {
    Octominx octominx;
    // std::cout << octominx << "\n\n";
    const auto moves = scrambleToMoves("R U R' U'");
    for (const Move move : moves) {
        // std::cout << move << "\n";
        octominx.doMove(move);
        // std::cout << octominx << "\n\n";
    }

    // std::cout << octominx << "\n";
    // CenterFacePieces whiteFaces = getWhiteFace(octominx);
    // std::cout << "White Face Pieces: " << whiteFaces << "\n";
    // auto moves2 = scrambleToMoves("L U' L' U");
    std::cout << "white center: " << getWhiteFace(octominx) << "\n";

    auto start = std::chrono::high_resolution_clock::now();
    const auto solutions = octominx.findWhiteFaceSolution(0);
    auto end = std::chrono::high_resolution_clock::now();
    // for (const auto& solution : solutions) {
    //     std::cout << "solution: " << solution << "\n";
    // }
    std::cout << solutions.size() << "\n";
    // whiteFaces.hash();
    // std::cout << "White Face Pieces after second scramble: " << whiteFaces << "\n";
    std::cout << "Time taken: " << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() << "ns\n";

    return 0;
}