#include "octominx.h"
#include <memory>
#include <vector>
#include <iostream>
#include <fstream>

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


const std::vector<std::unique_ptr<Scramble>> Octominx::scrambleArrayInitialIter(const std::unique_ptr<Scramble>& scramble) {
    std::vector<std::unique_ptr<Scramble>> newScrambleWhiteCenters = {}; // fill first iteration
    for (Move move: moves) {
        Scramble newState = *scramble;
//        if (move == Move(U, CCW)) {
//        std::cout << "scramble: " << newState << "\n";
//        }
        newState.doMove(move);
//        if (move == Move(U, CCW)) {
//        std::f << "scramble after move: " << newState << "\n";
//        }
        newScrambleWhiteCenters.push_back(std::make_unique<Scramble>(newState));
    }
    return newScrambleWhiteCenters;
}

const std::vector<std::unique_ptr<Scramble>> Octominx::solvedArrayInitialIter(const std::vector<std::unique_ptr<Scramble>>& scrambleArray) {
    std::vector<std::unique_ptr<Scramble>> newSolvedWhiteCenters = {};
    for (const std::unique_ptr<Scramble>& state: scrambleArray) {
        for (Move move: moves) {
            Scramble newState = *state;
//            if (move == Move(R, CCW)) {
//            	std::cout << "solved: " << newState << "\n";
//            }
            newState.doMove(move);
//            if (move == Move(R, CCW)) {
//            std::cout << "solved after move: " << newState << "\n";
//            }
            newSolvedWhiteCenters.push_back(std::make_unique<Scramble>(newState));
        }
    }
    return newSolvedWhiteCenters;
}

void Octominx::ArrayNextIter(std::vector<std::unique_ptr<Scramble>> &scrambleArray) {
    std::vector<std::unique_ptr<Scramble>> newScrambleArray = {};

    for (const std::unique_ptr<Scramble> &state: scrambleArray) {
        for (Move move: moves) {
            if (state->scramble.back().face == move.face) {
                continue;
            }
            Scramble newState = *state;
            newState.doMove(move);
            newScrambleArray.push_back(std::make_unique<Scramble>(newState));
        }
    }
	scrambleArray = std::move(newScrambleArray);
}

std::vector<DoubleMoveList> Octominx::findTwoSameScrambles(const std::vector<std::unique_ptr<Scramble>> &scrambleArray, const std::vector<std::unique_ptr<Scramble>> &solveArray) {
//    std::ofstream outFile("output.txt");
    std::vector<DoubleMoveList> res{};
    for (const std::unique_ptr<Scramble> &scrambleState: scrambleArray) {
        for (const std::unique_ptr<Scramble> &solveState: solveArray) {
//            std::cout << *scrambleState << "\n";
//            std::cout << *solveState << "\n";
            if (scrambleState->state.hash() == solveState->state.hash()) {
//                std::cout << scrambleState->state.hash() << "\n";
//                std::cout << solveState->state.hash() << "\n";
//
//				if (outFile.is_open()) {
//                    outFile << scrambleState->scramble << " " << scrambleState->state << " " << solveState->scramble << " " << solveState->state << "\n";
//				}
                res.push_back(DoubleMoveList{scrambleState->scramble, solveState->scramble});
            }
        }
    }
    return res;
}



std::vector<MoveList> Octominx::findWhiteFaceSolution(int iterations) {

    Scramble scrambleWhiteCenters = {getWhiteFace(*this), MoveList{}};
    std::unique_ptr<Scramble> pScrambleWhiteCenters = std::make_unique<Scramble> (scrambleWhiteCenters);
    std::vector<std::unique_ptr<Scramble>> solvedWhiteCenters = {};

    for (Scramble state: solvedStates) {
        solvedWhiteCenters.push_back(std::make_unique<Scramble>(state));
    }

    // for (const std::unique_ptr<Scramble>& pState : solvedWhiteCenters) {
    //     std::cout << "solvedWhiteCenters: " << *pState << "\n";
    // }



//    std::cout << solvedWhiteCenters.size() << "\n";

    std::vector<std::unique_ptr<Scramble>> scrambleWhiteCentersList = scrambleArrayInitialIter(std::move(pScrambleWhiteCenters));
//    for (const std::unique_ptr<Scramble> &potatoSlice: potato) {
//        std::cout << potatoSlice->scramble << std::endl;
//    }
    ArrayNextIter(scrambleWhiteCentersList);
//    std::cout << scrambleWhiteCentersList.size() << std::endl;
    std::vector<std::unique_ptr<Scramble>> solvedWhiteCentersList = solvedArrayInitialIter(solvedWhiteCenters);
    ArrayNextIter(solvedWhiteCentersList);
//    std::cout << solvedWhiteCentersList.size() << std::endl;
//    std::cout << *solvedWhiteCentersList.back() << std::endl;
    std::vector<DoubleMoveList> doubleMoveListList = findTwoSameScrambles(scrambleWhiteCentersList, solvedWhiteCentersList);
////    std::cout << b.scrambledScramble->scramble << " " << reverseMoves(b.solvedScramble->scramble) << "\n";
    std::vector<MoveList> res{};
    for (DoubleMoveList &doubleMoveList: doubleMoveListList) {
        MoveList solution = std::move(doubleMoveList.scrambledScramble);
        doubleMoveList.solvedScramble = reverseMoves(doubleMoveList.solvedScramble);
        solution.insert(solution.end(), std::make_move_iterator(doubleMoveList.solvedScramble.begin()), std::make_move_iterator(doubleMoveList.solvedScramble.end()));
        res.push_back(solution);
    }
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
