#ifndef OCTOMINX_H
#define OCTOMINX_H
#include <array>
#include <cstddef>
#include <iostream>
#include <ostream>
#include <string_view>
#include <unordered_map>
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

enum Face { U, N, L, F, R, T, B, D };

enum Corner { URLF, ULBN, UBRT, DNFL, DFRT, DTNB };

enum Direction { CW, CCW };

enum PieceType { CENTER, EDGE, CORNER };

enum Orientation { UNORIENTED, ORIENTED };

enum ColourSet1 { WHITE, GREEN, GREY, ORANGE };

constexpr size_t centerCount = 24;
constexpr size_t edgeCount = 12;
constexpr size_t cornerCount = 6;
constexpr size_t faceCount = 8;
constexpr size_t moveCount = 16;
constexpr size_t solvedCenterStateCount = 4;
struct CornerPiece {
    enum Corner corner;
    enum Orientation orientation;
};

using Centers = std::array<enum Face, centerCount>;

using Edges = std::array<enum Edge, edgeCount>;

using Corners = std::array<CornerPiece, cornerCount>;

struct Move {
    enum Face face;
    enum Direction direction;
};

Move intToMove(int i);

struct CenterFacePieces {
    std::array<Center, 3> centers;
    std::array<Edge, 3> edges;

    static const std::array<Center, centerCount * moveCount> centerTargetArray;
    static const std::array<Edge, edgeCount * moveCount> edgeTargetArray;
    const void doMove(Move move);
    const int centerMoveHash(Center center, Move move) const;
    const int edgeMoveHash(Edge edge, Move move) const;
    const int hash() const;
};

struct CenterFacePiecesHash {
    std::size_t operator()(const CenterFacePieces& cfp) const {

    // Normalize input order without sorting
    int min_val = std::min({cfp.centers[0], cfp.centers[1], cfp.centers[2]});
    int max_val = std::max({cfp.centers[0], cfp.centers[1], cfp.centers[2]});
    int mid_val = cfp.centers[0] ^ cfp.centers[1] ^ cfp.centers[2] ^ min_val ^ max_val;  // XOR trick to find the middle

    // Compute a unique hash based on combination formula
    int hash = min_val;  // 276 = C(23, 2)
    hash = (hash << 8) + mid_val;
    hash = (hash << 8) + max_val;


    // return hash;
    if (cfp.edges[1] - cfp.edges[0] != 1) {
        hash = -hash;
    }

    return hash;
}};


struct Cubestate {
    Centers centers = {U, U, U, N, N, N, L, L, L, F, F, F, R, R, R, T, T, T, B, B, B, D, D, D};
    Edges edges = {UL, UB, UR, LF, BN, LN, RT, BT, RF, DF, DT, DN};
    Corners corners = {CornerPiece{URLF, ORIENTED}, CornerPiece{ULBN, ORIENTED},
                       CornerPiece{UBRT, ORIENTED}, CornerPiece{DNFL, ORIENTED},
                       CornerPiece{DFRT, ORIENTED}, CornerPiece{DTNB, ORIENTED}};
};

const std::string_view center_to_string(enum Center center);

const std::string_view edge_to_string(enum Edge edge);

const std::string_view face_to_string(enum Face face);

const std::string_view corner_to_string(enum Corner corner);

class Octominx {
  private:
    Cubestate cubestate;
    // Template function to cycle three pieces
    template <typename T, typename IndexType, std::size_t N>
    void cycleThree(std::array<T, N> &arr, std::array<IndexType, 3> indices, Direction direction);
    constexpr bool flipNextOrientation(Corner pos, Move move) {
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
    // Method to cycle three pieces (either centers or edges)
    template <typename IndexType>
    void cycleThreePieces(PieceType piecetype, std::array<IndexType, 3> indices,
                          Direction direction);
    void cycleCenterSide(Move move);
    void cycleCenterAdj(Move move);
    void cycleEdgeSide(Move move);
    void cycleCornerSide(Move move);

  public:
    Octominx() = default;
    ~Octominx() = default;
    using CenterArray = std::array<std::array<Center, 3>, faceCount>;
    using EdgeArray = std::array<std::array<Edge, 3>, faceCount>;
    using CornerArray = std::array<std::array<Corner, 3>, faceCount>;
    static const CenterArray centerSideArray, centerAdjArray1, centerAdjArray2;
    static const EdgeArray edgeSideArray;
    static const CornerArray cornerSideArray;
    Cubestate getCubestate() const { return cubestate; };
    Corners getCorners() const { return cubestate.corners; };
    Edges getEdges() const { return cubestate.edges; };
    Centers getCenters() const { return cubestate.centers; };

    void doMove(Move move) {
        cycleCenterSide(move);
        cycleCenterAdj(move);
        cycleEdgeSide(move);
        cycleCornerSide(move);
    };
    std::string_view findWhiteFaceSolution(int iterations);

    friend CenterFacePieces getWhiteFace(const Octominx &octominx);
};
Orientation operator!(Orientation orientation);
std::ostream &operator<<(std::ostream &os, const Edge edge);
std::ostream &operator<<(std::ostream &os, const Center center);
std::ostream &operator<<(std::ostream &os, const Face face);
std::ostream &operator<<(std::ostream &os, const Corner corner);
std::ostream &operator<<(std::ostream &os, const Orientation orientation);
std::ostream &operator<<(std::ostream &os, const Corners corners);
std::ostream &operator<<(std::ostream &os, const Octominx &octominx);
std::ostream &operator<<(std::ostream &os, const Direction direction);
std::ostream &operator<<(std::ostream &os, const Move &move);
std::ostream &operator<<(std::ostream &os, const CenterFacePieces &solution);
bool operator==(const CenterFacePieces &lhs, const CenterFacePieces &rhs);

#endif
