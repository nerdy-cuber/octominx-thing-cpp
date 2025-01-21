#include "octominx.h"
#include <cassert>
#include <memory>
#include <utility>
#include <vector>
#include <iostream>
#include <fstream>
#include <unordered_set>


MoveList reverseMoves(const std::vector<Move> &moveList) {
    std::vector<Move> reversed = moveList; // Copy the vector
    std::reverse(reversed.begin(), reversed.end()); // Reverse the order

    // Invert each move's direction
    for (Move &move : reversed) {
        move.reverse();
    }

    return reversed;
}



//std::string_view face_to_string(const enum Face face) {
//    switch (face) {
//    case U:
//        return "U";
//    case L:
//        return "L";
//    case F:
//        return "F";
//    case R:
//        return "R";
//    case T:
//        return "T";
//    case B:
//        return "B";
//    case N:
//        return "N";
//    case D:
//        return "D";
//    }
//    throw std::invalid_argument("Invalid Face enum value");
//}
//
//std::ostream &operator<<(std::ostream &os, const Face face) {
//    os << face_to_string(face);
//    return os;
//}
//
//std::ostream &operator<<(std::ostream &os, const Direction direction) {
//    os << (direction == CW ? "CW" : "CCW");
//    return os;
//}
//
//std::ostream &operator<<(std::ostream &os, const Move &move) {
//    os << move.face << " " << move.direction;
//    return os;
//}


const std::vector<Scramble> Octominx::scrambleArrayInitialIter(const Scramble& scramble) {
    std::vector<Scramble> newScrambleWhiteCenters = {}; // fill first iteration
    for (Move move: moves) {
        Scramble newState = scramble;
//        if (move == Move(U, CCW)) {
//        std::cout << "scramble: " << newState << "\n";
        newState.doMove(move);
//        if (move == Move(U, CCW)) {
//        std::f << "scramble after move: " << newState << "\n";
//        }
        newScrambleWhiteCenters.push_back(newState);
    }
    return newScrambleWhiteCenters;
}

const std::vector<Scramble> Octominx::solvedArrayInitialIter(const std::vector<Scramble>& scrambleArray) {
    std::vector<Scramble> newSolvedWhiteCenters = {};
    for (const Scramble& state: scrambleArray) {
        for (Move move: moves) {
            Scramble newState = state;
//            if (move == Move(R, CCW)) {
//            	std::cout << "solved: " << newState << "\n";
//            }
            newState.doMove(move);
//            if (move == Move(R, CCW)) {
//            std::cout << "solved after move: " << newState << "\n";
//            }
            newSolvedWhiteCenters.push_back(Scramble(newState));
        }
    }
    return newSolvedWhiteCenters;
}

void Octominx::ArrayNextIter(std::vector<Scramble> &scrambleArray) {
    std::vector<Scramble> newScrambleArray = {};

    for (const Scramble &state: scrambleArray) {
//        std::cout << *state << std::endl;
        for (Move move: moves) {
            if (state.scramble.back().face == move.face) {
                continue;
            }
            Scramble newState = state;
            newState.doMove(move);
            newScrambleArray.push_back(newState);
        }
    }
	scrambleArray = newScrambleArray;
}

std::vector<std::pair<MoveList, MoveList>> Octominx::findTwoSameScrambles(const std::vector<Scramble> &scrambleArray, const std::vector<Scramble> &solveArray) {
	// auto start = std::chrono::system_clock::now();
    std::unordered_set<Scramble, ScrambleHash> set(scrambleArray.begin(), scrambleArray.end());
    // std::ofstream outFile("output.txt");
    std::vector<std::pair<MoveList, MoveList>> res{};
    for (const Scramble& scramble : solveArray) {
        auto it = set.find(scramble);
        if (it != set.end()) {
            res.push_back(std::make_pair(it->scramble, scramble.scramble));
        }
    }
//     for (const Scramble &scrambleState: scrambleArray) {
//         for (const Scramble &solveState: solveArray) {
// //            std::cout << *scrambleState << "\n";
// //            std::cout << *solveState << "\n";
//             if (scrambleState->state.hash() == solveState->state.hash()) {
// //                std::cout << scrambleState->state.hash() << "\n";
// //                std::cout << solveState->state.hash() << "\n";
// //
// 				if (outFile.is_open()) {
//                     outFile << scrambleState->scramble << " " << scrambleState->state << " " << solveState->scramble << " " << solveState->state << "\n";
// 				}
//                 res.push_back(DoubleMoveList{scrambleState->scramble, solveState->scramble});
//             }
//         }
//     }
    // auto end = std::chrono::system_clock::now();
    // std::cout << "time taken for findTwoSameScramles with arrays size:" << scrambleArray.size() << " and " << solveArray.size() << " " << std::chrono::duration_cast<std::chrono::seconds>(end - start).count() << " seconds\n";
    return res;
}



std::vector<MoveList> Octominx::findWhiteFaceSolution(int iterations) {
    assert(iterations >= 2);
    std::vector<MoveList> res{};

    Scramble scrambleWhiteCenters = {getWhiteFace(*this), MoveList{}};

    std::vector<Scramble> solvedWhiteCenters = {};
    for (Scramble state: solvedStates) {
        solvedWhiteCenters.push_back(state);
    }

    // for (const std::unique_ptr<Scramble>& pState : solvedWhiteCenters) {
    //     std::cout << "solvedWhiteCenters: " << *pState << "\n";
    // }



//    std::cout << solvedWhiteCenters.size() << "\n";

    std::vector<Scramble> scrambleWhiteCentersList = scrambleArrayInitialIter(scrambleWhiteCenters);
//    for (const std::unique_ptr<Scramble> &potatoSlice: potato) {
//        std::cout << potatoSlice->scramble << std::endl;
//    }
//    std::cout << scrambleWhiteCentersList.size() << std::endl;
    std::vector<Scramble> solvedWhiteCentersList = solvedArrayInitialIter(solvedWhiteCenters);
    auto doubleMoveListList = findTwoSameScrambles(scrambleWhiteCentersList, solvedWhiteCentersList);
    // for (const Scramble &scrambleState: scrambleWhiteCentersList) {
    //     std::cout << scrambleState << std::endl;
    // }
    // for (const Scramble &scrambleState: solvedWhiteCentersList) {
    //     std::cout << scrambleState << std::endl;
    // }
//    std::cout << scrambleWhiteCentersList.size() << std::endl;
//    std::cout << solvedWhiteCentersList.size() << std::endl;
    for (auto &doubleMoveList: doubleMoveListList) {
        auto [scrambleMoveList, solvedMoveList] = doubleMoveList;
        MoveList solution = scrambleMoveList;
        solvedMoveList = reverseMoves(solvedMoveList);
//        solution.insert(solution.end(), std::make_move_iterator(doubleMoveList.solvedScramble.begin()), std::make_move_iterator(doubleMoveList.solvedScramble.end()));
        for (Move move : solvedMoveList) {
            solution.push_back(move);
        }
        res.push_back(solution);
    }
    for (size_t i = 2; i < iterations; i++) {
        // std::cout << "current iteration: " << i << std::endl;

    	if ((i % 2) == 0 ) {
            ArrayNextIter(scrambleWhiteCentersList);
        } else {
            ArrayNextIter(solvedWhiteCentersList);
        }
        // std::cout << "scramble array size: " << scrambleWhiteCentersList.size() << std::endl;
//        std::cout << "yay" << std::endl;
        // std::cout << "solved array size: " << solvedWhiteCentersList.size() << std::endl;

        auto solutions = findTwoSameScrambles(scrambleWhiteCentersList, solvedWhiteCentersList);
        // std::cout << "solutions size: " << solutions.size() << "\n";
        for (auto &doubleMoveList: solutions) {
            auto [scrambleMoveList, solvedMoveList] = doubleMoveList;
            MoveList solution = scrambleMoveList;
            solvedMoveList = reverseMoves(solvedMoveList);
    //        solution.insert(solution.end(), std::make_move_iterator(doubleMoveList.solvedScramble.begin()), std::make_move_iterator(doubleMoveList.solvedScramble.end()));
            for (Move move : solvedMoveList) {
                solution.push_back(move);
            }
            res.push_back(solution);
    }
    }
//    std::cout << solvedWhiteCentersList.size() << std::endl;
//    std::cout << *solvedWhiteCentersList.back() << std::endl;
////    std::cout << b.scrambledScramble->scramble << " " << reverseMoves(b.solvedScramble->scramble) << "\n";
//     std::vector solvedWhiteCentersList = solvedArrayInitialIter(solvedWhiteCenters);
	return res;
}


//
//int main() {
//    std::vector<Move> potato = {{R, CW}, {U, CW}, {R, CCW}, {U, CCW}};
//	std::vector<Move> reversedPotato = reverseMoves(potato);
//    for (const Move &move: reversedPotato) {
//        std::cout << move << "\n";
//    }
//    return 0;
//}
