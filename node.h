// #include "octominx.h"
// #include <vector>
// #include <memory>
// #include "intToMove.cpp"


// struct Node {
//     CenterFacePieces state;
//     std::vector<std::unique_ptr<Node>> children{};
// };

// class BidrectionalTreeCenterFacePieces {
//     private:
//         Node m_rootScramble;
//         std::array<Node, 4> m_rootSolved = {
//             Node({CenterFacePieces{{UBL, URB, ULR}, {UL, UB, UR}}}),
//             Node{CenterFacePieces{{FRL, FDR, FLD}, {LF, RF, DF}}},
//             Node{CenterFacePieces{{TBR, TDB, TRD}, {RT, BT, DT}}},
//             Node{CenterFacePieces{{NLB, NDL, NBD}, {BN, LN, DN}}},
//         };
//         std::vector<std::unique_ptr<CenterFacePieces>> m_outerLayerScramble; 
//         std::vector<std::unique_ptr<CenterFacePieces>> m_outerLayerSolved;
//         void scrambleNextIter(int iteration) {
//             assert(iteration >= 2);
//             // assert(scrambleArray.size() == 16 * pow(14, (iteration-2)));
//             // std::vector<std::unique_ptr<CenterFacePieces>> nextIterScrambleArray;

//             // for (int i = 0; i < m_outerLayerScramble.size(); ++i) {
//             //     Move moveI = intToMove(i % moveCount);
//             //     for (int j = 0; j < moveCount; j++) { 
//             //         Move moveJ = intToMove(j);
//             //         if (moveI.face == moveJ.face) {
//             //             continue; // removes redundant moves such as U U or U U'
//             //         }
//             //         CenterFacePieces centerFacePieces = *m_outerLayerScramble[i];
//             //         centerFacePieces.doMove(moveJ);
//             //         std::unique_ptr<CenterFacePieces> pCenterFacePieces = std::make_unique<CenterFacePieces>(centerFacePieces);
//             //         nextIterScrambleArray.push_back(pCenterFacePieces);
//             //     }
//             // }
//             // m_outerLayerScramble = std::move(nextIterScrambleArray);
//             for (int i = 0; i < m_outerLayerScramble.size(); ++i) {
//                 for (int j = 0; j < moveCount; j++) {
//                     m_outerLayerScramble
//                 }
//             }
//         }
//         void solvedArrayNextIter(int iteration) {
//             assert(iteration >= 2);
//             // assert(solvedArray.size() == 4 * 16 * pow(14, (iteration-2)));
//             std::vector<std::unique_ptr<CenterFacePieces>> nextIterSolvedArray;
//                 for (int i = 0; i < m_outerLayerSolved.size(); ++i) {
//                     Move moveI = intToMove(i % moveCount);
//                     for (int j = 0; j < moveCount; j++) { 
//                         Move moveJ = intToMove(j);
//                         if (moveI.face == moveJ.face) {
//                             continue; // removes redundant moves such as U U or U U'
//                         }
//                         CenterFacePieces centerFacePieces = *m_outerLayerSolved[i];
//                         centerFacePieces.doMove(moveJ);
//                         std::unique_ptr<CenterFacePieces> pCenterFacePieces = std::make_unique<CenterFacePieces>(centerFacePieces);
//                         nextIterSolvedArray.push_back(pCenterFacePieces);
//                     }
//                 }
//             m_outerLayerSolved = std::move(nextIterSolvedArray);
//         }
//         void scrambleArrayInitialIter() {
//             for (int i = 0; i < moveCount; ++i) { // fill first iteration of outerLayerScramble
//                 Move move = intToMove(i);
//                 CenterFacePieces whiteFacesCopy = m_rootScramble.state;
//                 std::unique_ptr<CenterFacePieces> pWhiteFacesCopy =
//                     std::make_unique<CenterFacePieces>(whiteFacesCopy);
//                 pWhiteFacesCopy->doMove(move);
//                 m_outerLayerScramble.push_back(std::move(pWhiteFacesCopy));
//             }
//         }
//         void solvedArrayInitialIter() {
//             for (ColourSet1 colour : {WHITE, GREEN, GREY, ORANGE}) {
//                 for (int i = 0; i < moveCount; i++) { // fill first iteration of outerLayerSolved
//                     Move move = intToMove(i);
//                     CenterFacePieces solvedState = m_rootSolved[colour].state;
//                     std::unique_ptr<CenterFacePieces> pSolvedState =
//                         std::make_unique<CenterFacePieces>(solvedState);
//                     pSolvedState->doMove(move);
//                     // std::cout << i << " " << move << " " << *pSolvedState << "\n";
//                     m_outerLayerSolved.push_back(std::move(pSolvedState));
//                 }
//             }
//         }

//         std::vector<std::unique_ptr<class Tp>

//     public: 
//         void findWhiteFaceSolution(int iterations) {
//             // will not work if you enter in a scramble that needs less than 2 moves, if you can't figure
//             // that out on your own anyway, you're stupid
//             assert(iterations >= 2);

//             scrambleArrayInitialIter();
//             solvedArrayInitialIter();

//             for (int i = 0; i < m_outerLayerScramble.size(); ++i) {
//                 for (int j = 0; j < m_outerLayerSolved.size(); ++j) {
//                     auto pScrambleCubeState = m_outerLayerScramble[i].get();
//                     auto pSolvedCubeState = m_outerLayerSolved[j].get();
//                     if (*pScrambleCubeState == *pSolvedCubeState) {
//                         std::cout << "Found a match\n";
//                         std::cout << *pScrambleCubeState << " index " << i << " move " << intToMove(i) << "\n";
//                         std::cout << *pSolvedCubeState << " index " << j << " move " << (intToMove(j)).reverseMove() << "\n";
//                     }
//                 }
//             // # Efficient code commented out
//             // std::unordered_set<CenterFacePieces, CenterFacePiecesHash> ScrambleSet;
//             // for (const auto &pCubeState : outerLayerScramble) {
//             //     ScrambleSet.insert(*pCubeState);
//             // }
//             // std::cout << "whiteFaces " << whiteFaces << "\n";
//             // for (size_t i = 0; i < outerLayerScramble.size(); ++i) {
//             //     std::cout << i << " Outer Layer Scramble: " << intToMove(i) << " " << *outerLayerScramble[i] << "\n";
//             // }
//             // for (size_t i = 0; i < outerLayerSolved.size(); ++i) {
//             //     std::cout << i << " Outer Layer Solved: " << intToMove(i / 4) << " " << *outerLayerSolved[i] << "\n";
//             // }

//             // for (size_t solvedIndex = 0; solvedIndex < outerLayerSolved.size(); ++solvedIndex) {
//             //     const auto &pCubeState = outerLayerSolved[solvedIndex];
//             //     // std::cout << solvedIndex << " " << intToMove(solvedIndex % 16) << " " <<*pCubeState << "\n";
//             //     if (ScrambleSet.find(*pCubeState) != ScrambleSet.end()) {
//             //         auto scrambleIndex = std::distance(
//             //             outerLayerScramble.begin(),
//             //             std::find_if(
//             //                 outerLayerScramble.begin(),
//             //                 outerLayerScramble.end(),
//             //                 [&](const std::unique_ptr<CenterFacePieces>& p) { return *p == *pCubeState; }
//             //             )
//             //         );

//             //         std::cout << "Found a match\n";
//             //         std::cout << "Move in outerLayerScramble: " << scrambleIndex << " " << intToMove(scrambleIndex) << "\n";
//             //         std::cout << "Move in outerLayerSolved: " << solvedIndex << " " << intToMove(solvedIndex / 4) << "\n";
//             //     }
//             // }

//             // std::cout << whiteFaces << "\n";

//             }
//          }





// };