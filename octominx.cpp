#include "octominx.h"
#include <array>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <ostream>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <vector>


const Move intToMove(int i) { return {static_cast<Face>(i / 2), static_cast<Direction>(i % 2)}; }

const std::unordered_map<ColourSet1, CenterFacePieces> solvedWhiteStates = {
    {WHITE, CenterFacePieces{{UBL, URB, ULR}, {UL, UB, UR}}},
    {GREEN, CenterFacePieces{{FRL, FDR, FLD}, {LF, RF, DF}}},
    {GREY, CenterFacePieces{{TBR, TDB, TRD}, {RT, BT, DT}}},
    {ORANGE, CenterFacePieces{{NLB, NDL, NBD}, {BN, LN, DN}}},
};

const std::string_view corner_to_string(enum Corner corner) {
    switch (corner) {
    case URLF:
        return "URLF";
    case ULBN:
        return "ULBN";
    case UBRT:
        return "UBRT";
    case DNFL:
        return "DNFL";
    case DFRT:
        return "DFRT";
    case DTNB:
        return "DTNB";
    }
}

const std::string_view face_to_string(enum Face face) {
    switch (face) {
    case U:
        return "U";
    case L:
        return "L";
    case F:
        return "F";
    case R:
        return "R";
    case T:
        return "T";
    case B:
        return "B";
    case N:
        return "N";
    case D:
        return "D";
    }
}

const std::string_view edge_to_string(enum Edge edge) {
    switch (edge) {
    case UL:
        return "UL";
    case UB:
        return "UB";
    case UR:
        return "UR";
    case LF:
        return "LF";
    case BN:
        return "BN";
    case LN:
        return "LN";
    case RT:
        return "RT";
    case BT:
        return "BT";
    case RF:
        return "RF";
    case DF:
        return "DF";
    case DT:
        return "DT";
    case DN:
        return "DN";
    }
}

const std::string_view center_to_string(enum Center center) {
    switch (center) {
    case UBL:
        return "UBL";
    case URB:
        return "URB";
    case ULR:
        return "ULR";
    case LUN:
        return "LUN";
    case LFU:
        return "LFU";
    case LNF:
        return "LNF";
    case FRL:
        return "FRL";
    case FDR:
        return "FDR";
    case FLD:
        return "FLD";
    case RUF:
        return "RUF";
    case RTU:
        return "RTU";
    case RFT:
        return "RFT";
    case TBR:
        return "TBR";
    case TDB:
        return "TDB";
    case TRD:
        return "TRD";
    case BUT:
        return "BUT";
    case BNU:
        return "BNU";
    case BTN:
        return "BTN";
    case NLB:
        return "NLB";
    case NDL:
        return "NDL";
    case NBD:
        return "NBD";
    case DFN:
        return "DFN";
    case DTF:
        return "DTF";
    case DNT:
        return "DNT";
    }
}

Orientation operator!(Orientation orientation) {
    return orientation == ORIENTED ? UNORIENTED : ORIENTED;
}

std::ostream &operator<<(std::ostream &os, const Edge edge) {
    os << edge_to_string(edge);
    return os;
}

std::ostream &operator<<(std::ostream &os, const Center center) {
    os << center_to_string(center);
    return os;
}

std::ostream &operator<<(std::ostream &os, const Face face) {
    os << face_to_string(face);
    return os;
}

std::ostream &operator<<(std::ostream &os, const Corner corner) {
    os << corner_to_string(corner);
    return os;
}

std::ostream &operator<<(std::ostream &os, const Orientation orientation) {
    os << (orientation == ORIENTED ? "Oriented" : "Unoriented");
    return os;
}

std::ostream &operator<<(std::ostream &os, const Corners corners) {
    for (std::size_t i = 0; i < cornerCount; ++i) {
        os << static_cast<Corner>(i) << " : (" << corners[i].corner << ", "
           << corners[i].orientation << "), ";
    }
    return os;
}

std::ostream &operator<<(std::ostream &os, const Octominx &octominx) {
    os << "Centers: ";
    for (std::size_t i = 0; i < centerCount; ++i) {
        os << static_cast<Center>(i) << ": " << octominx.getCubestate().centers[i] << ", ";
    }
    os << "\nEdges: ";
    for (std::size_t i = 0; i < edgeCount; ++i) {
        os << static_cast<Edge>(i) << ": " << octominx.getCubestate().edges[i] << ", ";
    }
    os << "\nCorners: ";
    for (std::size_t i = 0; i < cornerCount; ++i) {
        os << static_cast<Corner>(i) << ": " << octominx.getCubestate().corners[i].corner << ", "
           << octominx.getCubestate().corners[i].orientation << ", ";
    }
    return os;
}

std::ostream &operator<<(std::ostream &os, const Direction direction) {
    os << (direction == CW ? "CW" : "CCW");
    return os;
}
std::ostream &operator<<(std::ostream &os, const Move &move) {
    os << move.face << " " << move.direction;
    return os;
}

std::ostream &operator<<(std::ostream &os, const CenterFacePieces &solution) {
    os << "Centers: ";
    for (Center center : solution.centers) {
        os << center << ", ";
    }
    os << "Edges: ";
    int i = 0;
    for (Edge edge : solution.edges) {
        os << static_cast<Edge>(i) << " : " << edge << ", ";
        i++;
    }
    return os;
}

bool operator==(const CenterFacePieces &lhs, const CenterFacePieces &rhs) {
    if (lhs.hash() != rhs.hash()) {
        return false;
    }
    return true;
}

template <typename T, typename IndexType, std::size_t N>
void Octominx::cycleThree(std::array<T, N> &arr, std::array<IndexType, 3> indices,
                          Direction direction) {
    if (direction == CW) {
        T temp = arr[indices[2]];
        arr[indices[2]] = arr[indices[1]];
        arr[indices[1]] = arr[indices[0]];
        arr[indices[0]] = temp;
    } else if (direction == CCW) {
        T temp = arr[indices[0]];
        arr[indices[0]] = arr[indices[1]];
        arr[indices[1]] = arr[indices[2]];
        arr[indices[2]] = temp;
    }
}

template <typename IndexType>
void Octominx::cycleThreePieces(PieceType piecetype, std::array<IndexType, 3> indices,
                                Direction direction) {

    switch (piecetype) {
    case CENTER:
        cycleThree(cubestate.centers, indices, direction);
        break;
    case EDGE:
        cycleThree(cubestate.edges, indices, direction);
        break;
    case CORNER:
        // Handle corners if needed
        cycleThree(cubestate.corners, indices, direction);
        break;
    }
}

void Octominx::cycleCenterSide(Move move) {
    auto centers = centerSideArray[move.face];
    cycleThreePieces(CENTER, centers, move.direction);
}

void Octominx::cycleCenterAdj(Move move) {
    auto centers1 = centerAdjArray1[move.face];
    cycleThreePieces(CENTER, centers1, move.direction);

    auto centers2 = centerAdjArray2[move.face];
    cycleThreePieces(CENTER, centers2, move.direction);
}

void Octominx::cycleEdgeSide(Move move) {
    auto edges = edgeSideArray[move.face];
    cycleThreePieces(EDGE, edges, move.direction);
}

void Octominx::cycleCornerSide(Move move) {
    auto corners = cornerSideArray[move.face];
    for (Corner pos : corners) {
        if (flipNextOrientation(pos, move) == true) {
            // std::cout << pos << " " << cubestate.corners[pos].orientation << "\n";
            cubestate.corners[pos].orientation = !(cubestate.corners[pos].orientation);
        }
    }
    cycleThreePieces(CORNER, corners, move.direction);
}

CenterFacePieces getWhiteFace(const Octominx &octominx) {
    CenterFacePieces whiteFaces{};
    int i = 0;
    for (size_t pos = 0; pos < centerCount; ++pos) {
        if (octominx.cubestate.centers[pos] == U) {
            whiteFaces.centers[i] = static_cast<Center>(pos);
            i++;
        }
    }

    for (size_t pos = 0; pos < edgeCount; ++pos) {
        if (octominx.cubestate.edges[pos] == UL || octominx.cubestate.edges[pos] == UB ||
            octominx.cubestate.edges[pos] == UR) {
            whiteFaces.edges[octominx.cubestate.edges[pos]] = static_cast<Edge>(pos);
        }
    }

    return whiteFaces;
}
const int CenterFacePieces::centerMoveHash(Center center, Move move) const {
    return center * moveCount + move.face * 2 +
           !move.direction; // move.direction is run through a NOT gate here because the lookup
                            // table i generated was for the opposite direction
}

const int CenterFacePieces::edgeMoveHash(Edge edge, Move move) const {
    return edge * moveCount + move.face * 2 + !move.direction;
}

const int CenterFacePieces::hash() const {
    // Ensure input values are between 1 and 24

    // Normalize input order without sorting
    int min_val = std::min({centers[0], centers[1], centers[2]});
    int max_val = std::max({centers[0], centers[1], centers[2]});
    int mid_val = centers[0] ^ centers[1] ^ centers[2] ^ min_val ^ max_val;  // XOR trick to find the middle

    // Compute a unique hash based on combination formula
    int hash = min_val;  // 276 = C(23, 2)
    hash = (hash << 8) + mid_val;
    hash = (hash << 8) + max_val;


    // return hash;
    if (this->edges[1] - this->edges[0] != 1) {
        hash = -hash;
    }

    return hash;
}

const void CenterFacePieces::doMove(Move move) {
    for (Center &center : centers) {
        // int hash = CenterHash(center, move);
        // std::cout << CenterHash(center, move) << "\n";
        center = centerTargetArray[centerMoveHash(center, move)];
    }
    for (Edge &edge : edges) {
        // int hash = EdgeHash(edge, move);
        // std::cout << EdgeHash(edge, move) << "\n";
        edge = edgeTargetArray[edgeMoveHash(edge, move)];
    }
}

std::string_view Octominx::findWhiteFaceSolution(int iterations) {
    // will not work if you enter in a scramble that needs less than 2 moves, if you can't figure
    // that out on your own anyway, you're stupid

    const CenterFacePieces whiteFaces = getWhiteFace(*this);

    std::vector<std::unique_ptr<CenterFacePieces>> outerLayerScramble;
    std::vector<std::unique_ptr<CenterFacePieces>> outerLayerSolved;

    for (int i = 0; i < moveCount; ++i) { // fill first iteration of outerLayerScramble
        Move move = intToMove(i);
        CenterFacePieces whiteFacesCopy = whiteFaces;
        std::unique_ptr<CenterFacePieces> pWhiteFacesCopy =
            std::make_unique<CenterFacePieces>(whiteFacesCopy);
        pWhiteFacesCopy->doMove(move);
        outerLayerScramble.push_back(std::move(pWhiteFacesCopy));
    }

    for (int i = 0; i < moveCount; i++) { // fill first iteration of outerLayerSolved
        for (ColourSet1 colour : {WHITE, GREEN, GREY, ORANGE}) {
            Move move = intToMove(i);
            CenterFacePieces solvedState = solvedWhiteStates.at(colour);
            std::unique_ptr<CenterFacePieces> pSolvedState =
                std::make_unique<CenterFacePieces>(solvedState);
            pSolvedState->doMove(move);
            outerLayerSolved.push_back(std::move(pSolvedState));
        }
    }
    std::unordered_set<CenterFacePieces, CenterFacePiecesHash> ScrambleSet;
    for (const auto &pCubeState : outerLayerScramble) {
        ScrambleSet.insert(*pCubeState);
    }
    int i = 0;
    for (size_t solvedIndex = 0; solvedIndex < outerLayerSolved.size(); ++solvedIndex) {
        const auto &pCubeState = outerLayerSolved[solvedIndex];
        if (ScrambleSet.find(*pCubeState) != ScrambleSet.end()) {
            auto scrambleIndex = std::distance(
                outerLayerScramble.begin(),
                std::find_if(
                    outerLayerScramble.begin(),
                    outerLayerScramble.end(),
                    [&](const std::unique_ptr<CenterFacePieces>& p) { return *p == *pCubeState; }
                )
            );

            std::cout << "Found a match\n";
            std::cout << "Index in outerLayerScramble: " << scrambleIndex << "\n";
            std::cout << "Index in outerLayerSolved: " << solvedIndex << "\n";
        }
    }

    // std::cout << whiteFaces << "\n";

    return "";
}
