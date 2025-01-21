#include "octominx.h"
#include <array>
#include <cstddef>
#include <iostream>
#include <ostream>
#include <string_view>
#include <vector>



std::string_view corner_to_string(const enum Corner corner) {
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
    throw std::invalid_argument("Invalid Corner enum value");
}

std::string_view face_to_string(const enum Face face) {
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
    throw std::invalid_argument("Invalid Face enum value");
}

std::string_view edge_to_string(const enum Edge edge) {
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
    throw std::invalid_argument("Invalid Edge enum value");
}

std::string_view center_to_string(const enum Center center) {
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
    throw std::invalid_argument("Invalid Center enum value");
}

Orientation operator!(const Orientation orientation) {
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

std::ostream &operator<<(std::ostream &os, const Corners &corners) {
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
    for (const Center center : solution.centers) {
        os << center << ", ";
    }
    os << "Edges: ";
    int i = 0;
    for (const Edge edge : solution.edges) {
        os << static_cast<Edge>(i) << " : " << edge << ", ";
        i++;
    }
    return os;
}

std::ostream &operator<<(std::ostream &os, const Scramble &scramble) {
    os << "Scramble: ";
    for (const Move move: scramble.scramble) {
        os << move << ", ";
    }
    os << "State: " << scramble.state;
    return os;
}

std::ostream &operator<<(std::ostream &os, const MoveList &moveList) {
    os << "List of moves: ";
    for (const Move &move : moveList) {
        os << move << ", ";
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
                          const Direction direction) {
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
void Octominx::cycleThreePieces(const PieceType piecetype, std::array<IndexType, 3> indices,
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

void Octominx::cycleCenterSide(const Move move) {
    auto centers = centerSideArray[move.face];
    cycleThreePieces(CENTER, centers, move.direction);
}

void Octominx::cycleCenterAdj(const Move move) {
    auto centers1 = centerAdjArray1[move.face];
    cycleThreePieces(CENTER, centers1, move.direction);

    auto centers2 = centerAdjArray2[move.face];
    cycleThreePieces(CENTER, centers2, move.direction);
}

void Octominx::cycleEdgeSide(const Move move) {
    auto edges = edgeSideArray[move.face];
    cycleThreePieces(EDGE, edges, move.direction);
}

bool Octominx::flipNextOrientation(const Corner pos, const Move move) {
        // Handle corner orientations when doing a move if needed, I barely know how this works
        // White pieces on the top face are oriented if the white sticker is facing up, they are
        // unoriented if the white sticker is not facing up White pieces on the bottom face are
        // oriented if the white sticker is on the right, else they are unoriented if the white
        // sticker is on the left Yellow pieces on the bottom face are oriented if the yellow
        // sticker is facing down, they are unoriented if the yellow sticker is not facing down
        // Yellow pieces on the top face are oriented if the yellow sticker is on the right, else
        // they are unoriented if the yellow sticker is on the left
        if ((pos == ULBN) or (pos == UBRT) or (pos == URLF)) {
            if (move.direction == CW) {
                switch (pos) {
                case URLF: {
                    return (move.face == R) ? (true) : (false);
                }
                case ULBN: {
                    return (move.face == L) ? (true) : (false);
                }
                case UBRT: {
                    return (move.face == B) ? (true) : (false);
                }
                case DNFL:
                case DFRT:
                case DTNB:
                    break;
                }
            } else {
                switch (pos) {
                case URLF: {
                    return (move.face == R || move.face == L || move.face == F) ? (true) : (false);
                }
                case ULBN: {
                    return (move.face == L || move.face == B || move.face == N) ? (true) : (false);
                }
                case UBRT: {
                    return (move.face == B || move.face == R || move.face == T) ? (true) : (false);
                }
                case DNFL:
                case DFRT:
                case DTNB:
                    break;
                }
            }
        }

        else if ((pos == DNFL) or (pos == DFRT) or (pos == DTNB)) {
            if (move.direction == CCW) {
                switch (pos) {
                case DNFL: {
                    return (move.face == F) ? (true) : (false);
                }
                case DFRT: {
                    return (move.face == R) ? (true) : (false);
                }
                case DTNB: {
                    return (move.face == N) ? (true) : (false);
                }
                case URLF:
                case ULBN:
                case UBRT:
                    break;
                }
            } else {
                switch (pos) {
                case DNFL: {
                    return (move.face == N || move.face == F || move.face == L) ? (true) : (false);
                }
                case DFRT: {
                    return (move.face == F || move.face == R || move.face == T) ? (true) : (false);
                }
                case DTNB: {
                    return (move.face == T || move.face == N || move.face == B) ? (true) : (false);
                }
                case URLF:
                case ULBN:
                case UBRT:
                    break;
                }
            }
        }
        return false;
}

void Octominx::cycleCornerSide(const Move move) {
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
int CenterFacePieces::centerMoveHash(const Center center, const Move move) {
    return center * moveCount + move.face * 2 +
           move.direction; // move.direction is run through a NOT gate here because the lookup
                            // table I generated was for the opposite direction
}

// CenterMove CenterFacePieces::unhashCenter(const int hash) {
//     Center center = static_cast<Center>(hash / moveCount);
//     Move move = Move{static_cast<Face>((hash % moveCount)/2), static_cast<Direction>((hash + 1) % 2)};
//     return CenterMove{center, move};
// }


int CenterFacePieces::edgeMoveHash(Edge edge, Move move) {
    return edge * moveCount + move.face * 2 + !move.direction;
}

long long CenterFacePieces::hash() const {


    // Normalize input order without sorting
    const int min_center_val = std::min({this->centers[0], this->centers[1], this->centers[2]});
    const int max_center_val = std::max({this->centers[0], this->centers[1], this->centers[2]});
    const int mid_center_val = this->centers[0] ^ this->centers[1] ^ this->centers[2] ^ min_center_val ^ max_center_val;  // XOR trick to find the middle
    const int min_edge_val = std::min({this->edges[0], this->edges[1], this->edges[2]});
    const int max_edge_val = std::max({this->edges[0], this->edges[1], this->edges[2]});
    const int mid_edge_val = this->edges[0] ^ this->edges[1] ^ this->edges[2] ^ min_edge_val ^ max_edge_val;  // XOR trick to find the middle
    // Compute a unique hash based on combination formula
    long long hash = min_center_val;  // 276 = C(23, 2)
    hash = (hash << 8) + mid_center_val;
    hash = (hash << 8) + max_center_val;
    hash = (hash << 8) + min_edge_val;
    hash = (hash << 8) + mid_edge_val;
    hash = (hash << 8) + max_edge_val;


    // return hash;
    if ((this->edges[0] < this->edges[1] && this->edges[1] < this->edges[2]) ||
    (this->edges[1] < this->edges[2] && this->edges[2] < this->edges[0]) ||
    (this->edges[2] < this->edges[0] && this->edges[0] < this->edges[1])) {
        hash = -hash;
    }

    return hash;
}

void CenterFacePieces::doMove(Move move) {
    for (Center &center : centers) {
        // int hash = CenterHash(center, move);
        // std::cout << CenterHash(center, move) << "\n";
        // std::cout << centerMoveHash(center, move) << "\n";
        center = centerTargetArray[centerMoveHash(center, move)];
    }
    for (Edge &edge : edges) {
        // int hash = EdgeHash(edge, move);
        // std::cout << EdgeHash(edge, move) << "\n";
        // std::cout << edgeMoveHash(edge, move) << "\n";
        edge = edgeTargetArray[edgeMoveHash(edge, move)];
    }
}


// void printBytesInBinary(long value) {
//     unsigned char* bytePointer = reinterpret_cast<unsigned char*>(&value);
//     std::cout << "Bytes of long value (" << value << ") in binary: ";
//     for (size_t i = 0; i < sizeof(long); ++i) {
//         std::bitset<8> bits(bytePointer[i]);
//         std::cout << bits << " ";
//     }
//     std::cout << std::endl;
// }

const std::array<Scramble, 4> Octominx::solvedStates = {
    Scramble({{CenterFacePieces{{UBL, URB, ULR}, {UL, UB, UR}}}, {}}),
    Scramble({CenterFacePieces{{FRL, FDR, FLD}, {LF, RF, DF}}, {}}),
    Scramble({CenterFacePieces{{TBR, TDB, TRD}, {RT, BT, DT}}, {}}),
    Scramble({CenterFacePieces{{NLB, NDL, NBD}, {BN, LN, DN}}, {}}),
};
