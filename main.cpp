#include <algorithm>
#include <array>
#include <cstddef>
#include <iostream>
#include <ostream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

enum Center {
    UBL,
    URB,
    ULR,
    LUN,
    LFU,
    LNF,
    FRL,
    FDR,
    FLD,
    RUF,
    RTU,
    RFT,
    TBR,
    TDB,
    TRD,
    BUT,
    BNU,
    BTN,
    NLB,
    NDL,
    NBD,
    DFN,
    DTF,
    DNT,
};

enum Edge { UL, UB, UR, LF, BN, LN, RT, BT, RF, DF, DT, DN };

enum Face { U, L, F, R, B, D, N, T };

enum Corner { URLF, ULBN, UBRT, DNFL, DFRT, DTNB };

enum Direction { CW, CCW };

enum PieceType { CENTER, EDGE, CORNER };

enum Orientation { ORIENTED, UNORIENTED };

constexpr size_t CenterCount = 24;
constexpr size_t EdgeCount = 12;
constexpr size_t CornerCount = 6;

struct CornerPiece {
    enum Corner corner;
    enum Orientation orientation;
};

struct Cubestate {
    std::array<enum Center, CenterCount> centers = {UBL, URB, ULR, LUN, LFU, LNF, FRL, FDR,
                                                    FLD, RUF, RTU, RFT, TBR, TDB, TRD, BUT,
                                                    BNU, BTN, NLB, NDL, NBD, DFN, DTF, DNT};
    std::array<enum Edge, EdgeCount> edges = {UL, UB, UR, LF, BN, LN, RT, BT, RF, DF, DT, DN};
    std::array<struct CornerPiece, CornerCount> corners = {
        CornerPiece{URLF, ORIENTED}, CornerPiece{ULBN, ORIENTED}, CornerPiece{UBRT, ORIENTED},
        CornerPiece{DNFL, ORIENTED}, CornerPiece{DFRT, ORIENTED}, CornerPiece{DTNB, ORIENTED}};
};

struct Move {
    enum Face face;
    enum Direction direction;
};

constexpr std::string_view center_to_string(enum Center center) {
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

constexpr std::string_view edge_to_string(enum Edge edge) {
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

constexpr std::string_view face_to_string(enum Face face) {
    switch (face) {
    case U:
        return "U";
    case L:
        return "L";
    case F:
        return "F";
    case R:
        return "R";
    case B:
        return "B";
    case D:
        return "D";
    case N:
        return "N";
    case T:
        return "T";
    }
}

constexpr std::string_view corner_to_string(enum Corner corner) {
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

class Octominx {
  private:
    Cubestate cubestate;
    // Template function to cycle three pieces
    template <typename T, std::size_t N>
    void cycleThree(std::array<T, N> &arr, std::size_t idx1, std::size_t idx2, std::size_t idx3,
                    Direction direction) {
        if (direction == CW) {
            T temp = arr[idx3];
            arr[idx3] = arr[idx2];
            arr[idx2] = arr[idx1];
            arr[idx1] = temp;
        } else if (direction == CCW) {
            T temp = arr[idx1];
            arr[idx1] = arr[idx2];
            arr[idx2] = arr[idx3];
            arr[idx3] = temp;
        }
    }
    bool flipNextOrientation(Corner pos, Move move) {
        // Handle corner orientations when doing a move if needed, I barely know how this works
        if (move.direction == CW) {
            return (move.face == U || move.face == D) ? (false) : (true);
        } else {
            switch (pos) {
            case URLF: {
                return (move.face == L) ? (true) : (false);
            }
            case ULBN: {
                return (move.face == B) ? (true) : (false);
            }
            case UBRT: {
                return (move.face == R) ? (true) : (false);
            }
            case DNFL: {
                return (move.face == F) ? (true) : (false);
            }
            case DFRT: {
                return (move.face == R) ? (true) : (false);
            }
            case DTNB: {
                return (move.face == B) ? (true) : (false);
            }
            }
        }
    }
    // Method to cycle three pieces (either centers or edges)
    /**
     * @brief Cycles three pieces of a given type in the cube state.
     *
     * This function cycles three pieces (center, edge, or corner) in the cube state
     * based on the specified indices and direction. It calls the appropriate helper
     * function to perform the cycling operation.
     *
     * @param piecetype The type of piece to cycle (CENTER, EDGE, or CORNER).
     * @param idx1 The index of the first piece to cycle.
     * @param idx2 The index of the second piece to cycle.
     * @param idx3 The index of the third piece to cycle.
     * @param direction The direction in which to cycle the pieces.
     */
    void cycleThreePieces(PieceType piecetype, std::size_t idx1, std::size_t idx2, std::size_t idx3,
                          Direction direction) {
        switch (piecetype) {
        case CENTER:
            cycleThree(cubestate.centers, idx1, idx2, idx3, direction);
            break;
        case EDGE:
            cycleThree(cubestate.edges, idx1, idx2, idx3, direction);
            break;
        case CORNER:
            // Handle corners if needed
            cycleThree(cubestate.corners, idx1, idx2, idx3, direction);
            break;
        }
    }
    void cycleCenterSide(Move move) {
        switch (move.face) {
        case U:
            cycleThreePieces(CENTER, LUN, BUT, RUF, move.direction);
            break;
        case N:
            cycleThreePieces(CENTER, LNF, DNT, BNU, move.direction);
            break;
        case L:
            cycleThreePieces(CENTER, ULR, FLD, NLB, move.direction);
            break;
        case F:
            cycleThreePieces(CENTER, LFU, RFT, DFN, move.direction);
            break;
        case R:
            cycleThreePieces(CENTER, URB, TRD, FRL, move.direction);
            break;
        case T:
            cycleThreePieces(CENTER, RTU, BTN, DTF, move.direction);
            break;
        case B:
            cycleThreePieces(CENTER, UBL, NBD, TBR, move.direction);
            break;
        case D:
            cycleThreePieces(CENTER, FDR, TDB, NDL, move.direction);
            break;
        }
    }

    void cycleCenterAdj(Move move) {
        switch (move.face) {
        case U:
            cycleThreePieces(CENTER, LUN, BUT, RUF, move.direction);
            cycleThreePieces(CENTER, LFU, BNU, RTU, move.direction);
            break;
        case N:
            cycleThreePieces(CENTER, LNF, DNT, BNU, move.direction);
            cycleThreePieces(CENTER, LUN, DFN, BTN, move.direction);
            break;
        case L:
            cycleThreePieces(CENTER, ULR, FLD, NLB, move.direction);
            cycleThreePieces(CENTER, UBL, FRL, NDL, move.direction);
            break;
        case F:
            cycleThreePieces(CENTER, LFU, RFT, DFN, move.direction);
            cycleThreePieces(CENTER, LNF, RUF, DTF, move.direction);
            break;
        case R:
            cycleThreePieces(CENTER, URB, TRD, FRL, move.direction);
            cycleThreePieces(CENTER, ULR, TBR, FDR, move.direction);
            break;
        case T:
            cycleThreePieces(CENTER, RTU, BTN, DTF, move.direction);
            cycleThreePieces(CENTER, RFT, BUT, DNT, move.direction);
            break;
        case B:
            cycleThreePieces(CENTER, UBL, NBD, TBR, move.direction);
            cycleThreePieces(CENTER, URB, NLB, TDB, move.direction);
            break;
        case D:
            cycleThreePieces(CENTER, FDR, TDB, NDL, move.direction);
            cycleThreePieces(CENTER, FLD, TRD, NBD, move.direction);
            break;
        }
    }
    void cycleEdgeSide(Move move) {
        switch (move.face) {
        case U:
            cycleThreePieces(EDGE, UL, UB, UR, move.direction);
            break;
        case N:
            cycleThreePieces(EDGE, BN, LN, DN, move.direction);
            break;
        case L:
            cycleThreePieces(EDGE, UL, LF, LN, move.direction);
            break;
        case F:
            cycleThreePieces(EDGE, LF, RF, DF, move.direction);
            break;
        case R:
            cycleThreePieces(EDGE, UR, RT, RF, move.direction);
            break;
        case T:
            cycleThreePieces(EDGE, RT, BT, DT, move.direction);
            break;
        case B:
            cycleThreePieces(EDGE, UB, BN, BT, move.direction);
            break;
        case D:
            cycleThreePieces(EDGE, DF, DT, DN, move.direction);
            break;
        }
    }
    void cycleCornerSide(Move move) {
        switch (move.face) {
        case U:
            cycleThreePieces(CORNER, URLF, ULBN, UBRT, move.direction);
            break;
        case N:
            cycleThreePieces(CORNER, ULBN, DNFL, DTNB, move.direction);
            break;
        case L:
            cycleThreePieces(CORNER, URLF, DNFL, ULBN, move.direction);
            break;
        case F:
            cycleThreePieces(CORNER, URLF, DFRT, DNFL, move.direction);
            break;
        case R:
            cycleThreePieces(CORNER, URLF, UBRT, DFRT, move.direction);
            break;
        case T:
            cycleThreePieces(CORNER, UBRT, DTNB, DNFL, move.direction);
            break;
        case B:
            cycleThreePieces(CORNER, UBRT, ULBN, DTNB, move.direction);
            break;
        case D:
            cycleThreePieces(CORNER, DNFL, DFRT, DTNB, move.direction);
            break;
        }
    }

  public:
    Octominx() = default;
    ~Octominx() = default;
    Cubestate getCubestate() const { return cubestate; }
    void doMove(Move move) {
        cycleCenterSide(move);
        cycleCenterAdj(move);
        cycleEdgeSide(move);
        cycleCornerSide(move);
    };
};

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

std::ostream &operator<<(std::ostream &os, const Octominx &octominx) {
    os << "Centers: ";
    for (std::size_t i = 0; i < octominx.getCubestate().centers.size(); ++i) {
        os << static_cast<Center>(i) << ": " << octominx.getCubestate().centers[i] << ", ";
    }
    os << "\nEdges: ";
    for (std::size_t i = 0; i < octominx.getCubestate().edges.size(); ++i) {
        os << static_cast<Edge>(i) << ": " << octominx.getCubestate().edges[i] << ", ";
    }
    os << "\nCorners: ";
    for (std::size_t i = 0; i < octominx.getCubestate().corners.size(); ++i) {
        os << static_cast<Corner>(i) << ": " << octominx.getCubestate().corners[i].corner
                  << ", " << octominx.getCubestate().corners[i].orientation << ", ";
    }
    return os;
}

std::ostream &operator<<(std::ostream &os, const Move &move) {
    os << move.face << " " << move.direction;
    return os;
}

auto scrambleToMoves(std::string_view scramble) {
    std::unordered_map<char, Face> faceMap = {{'U', U}, {'L', L}, {'F', F}, {'R', R},
                                              {'B', B}, {'D', D}, {'N', N}, {'T', T}};
    std::unordered_map<char, Direction> directionMap = {{' ', CW}, {'\'', CCW}};
    std::vector<Move> moves;
    for (char c : scramble) {
        if (faceMap.find(c) != faceMap.end()) {
            moves.push_back({faceMap[c], CW});
        } else if (directionMap.find(c) != directionMap.end()) {
            moves.back().direction = directionMap[c];
        }
    }
    return moves;
}

int main() {
    Octominx octominx;
    // print(octominx);
    auto moves = scrambleToMoves("U L F R B D N T");
    for (Move move : moves) {
        std::cout << move << "\n";
        octominx.doMove(move);
    }
    std::cout << octominx << "\n";
    return 0;
}