#ifndef OCTOMINX_H
#define OCTOMINX_H
#include <algorithm>
#include <array>
#include <cstddef>
#include <ostream>
#include <string_view>
#include <utility>
#include <vector>
enum Center {
    UBL,
    URB,
    ULR,
    NLB,
    NDL,
    NBD,
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
    Move(const Face f, const Direction d) : face(f), direction(d) {}
    void reverse() {
        direction = direction == CW ? CCW : CW;
    }
    bool operator==(const Move &other) const {
        return face == other.face && direction == other.direction;
    } // TODO: measure if using a hash is faster
};
const std::array<Move, moveCount> moves = {
    Move{U, CW}, Move{U, CCW}, Move{N, CW}, Move{N, CCW},Move{L, CW}, Move{L, CCW}, Move{F, CW}, Move{F, CCW},
    Move{R, CW}, Move{R, CCW}, Move{T, CW}, Move{T, CCW}, Move{B, CW}, Move{B, CCW},
     Move{D, CW}, Move{D, CCW}};

using MoveList = std::vector<Move>;

struct CenterMove {
    Center center;
    Move move;
};


struct CenterFacePieces {
    std::array<Center, 3> centers;
    std::array<Edge, 3> edges;

    static const std::array<Center, centerCount * moveCount> centerTargetArray;
    static const std::array<Edge, edgeCount * moveCount> edgeTargetArray;
    void doMove(Move move);
    static int centerMoveHash(Center center, Move move);
    static CenterMove unhashCenter(int hash);
    static int edgeMoveHash(Edge edge, Move move);
    long long hash() const;
    
};

struct CenterFacePiecesHash {
    std::size_t operator()(const CenterFacePieces& cfp) const {

    // Normalize input order without sorting
    int min_center_val = std::min({cfp.centers[0], cfp.centers[1], cfp.centers[2]});
    int max_center_val = std::max({cfp.centers[0], cfp.centers[1], cfp.centers[2]});
    int mid_center_val = cfp.centers[0] ^ cfp.centers[1] ^ cfp.centers[2] ^ min_center_val ^ max_center_val;  // XOR trick to find the middle
    int min_edge_val = std::min({cfp.edges[0], cfp.edges[1], cfp.edges[2]});
    int max_edge_val = std::max({cfp.edges[0], cfp.edges[1], cfp.edges[2]});
    int mid_edge_val = cfp.edges[0] ^ cfp.edges[1] ^ cfp.edges[2] ^ min_edge_val ^ max_edge_val;  // XOR trick to find the middle
    // Compute a unique hash based on combination formula
    int hash = min_center_val;  // 276 = C(23, 2)
    hash = (hash << 8) + mid_center_val;
    hash = (hash << 8) + max_center_val;
    hash = (hash << 8) + min_edge_val;
    hash = (hash << 8) + mid_edge_val;
    hash = (hash << 8) + max_edge_val;


    // return hash;
    if ((cfp.edges[0] < cfp.edges[1] < cfp.edges[2]) || (cfp.edges[1] < cfp.edges[2] < cfp.edges[0]) || (cfp.edges[2] < cfp.edges[0] < cfp.edges[1])) {
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

struct Scramble {
    CenterFacePieces state;
    std::vector<Move> scramble{};
    void doMove(Move move) {
        state.doMove(move);
        scramble.push_back(move);
    }
    bool operator==(const Scramble& other) const {
        return state.hash() == other.state.hash();
    }
};

struct ScrambleHash {
    std::size_t operator()(const Scramble& item) const {
        return item.state.hash();
    }
};

struct DoubleMoveList {
    MoveList &scrambledScramble;
    MoveList &solvedScramble;
};

std::string_view center_to_string(enum Center center);

std::string_view edge_to_string(enum Edge edge);

std::string_view face_to_string(enum Face face);

std::string_view corner_to_string(enum Corner corner);

class Octominx {
  private:
    Cubestate cubestate;
    // Template function to cycle three pieces
    template <typename T, typename IndexType, std::size_t N>
    void cycleThree(std::array<T, N> &arr, std::array<IndexType, 3> indices, Direction direction);
    static bool flipNextOrientation (const Corner pos, const Move move);
    // Method to cycle three pieces (either centers or edges)
    template <typename IndexType>
    void cycleThreePieces(PieceType piecetype, std::array<IndexType, 3> indices,
                          Direction direction);
    void cycleCenterSide(Move move);
    void cycleCenterAdj(Move move);
    void cycleEdgeSide(Move move);
    void cycleCornerSide(Move move);
    const std::vector<Scramble> scrambleArrayInitialIter(const Scramble& scramble);
    const std::vector<Scramble> solvedArrayInitialIter(const std::vector<Scramble>& scramble);
    void ArrayNextIter(std::vector<Scramble> &scrambleArray);
    std::vector<std::pair<MoveList, MoveList>> findTwoSameScrambles(const std::vector<Scramble> &scrambleArray, const std::vector<Scramble> &solveArray);

  public:
    Octominx() = default;
    ~Octominx() = default;
    using CenterArray = std::array<std::array<Center, 3>, faceCount>;
    using EdgeArray = std::array<std::array<Edge, 3>, faceCount>;
    using CornerArray = std::array<std::array<Corner, 3>, faceCount>;
    static const CenterArray centerSideArray, centerAdjArray1, centerAdjArray2;
    static const EdgeArray edgeSideArray;
    static const CornerArray cornerSideArray;
    static const std::array<Scramble, 4> solvedStates;
    Cubestate getCubestate() const { return cubestate; };
    Corners getCorners() const { return cubestate.corners; };
    Edges getEdges() const { return cubestate.edges; };
    Centers getCenters() const { return cubestate.centers; };

    void doMove(const Move move) {
        cycleCenterSide(move);
        cycleCenterAdj(move);
        cycleEdgeSide(move);
        cycleCornerSide(move);
    };
    std::vector<MoveList> findWhiteFaceSolution(int iterations);

    friend CenterFacePieces getWhiteFace(const Octominx &octominx);
};
Orientation operator!(Orientation orientation);
std::ostream &operator<<(std::ostream &os, const Edge edge);
std::ostream &operator<<(std::ostream &os, const Center center);
std::ostream &operator<<(std::ostream &os, const Face face);
std::ostream &operator<<(std::ostream &os, const Corner corner);
std::ostream &operator<<(std::ostream &os, const Orientation orientation);
std::ostream &operator<<(std::ostream &os, const Corners &corners);
std::ostream &operator<<(std::ostream &os, const Octominx &octominx);
std::ostream &operator<<(std::ostream &os, const Direction direction);
std::ostream &operator<<(std::ostream &os, const Move &move);
std::ostream &operator<<(std::ostream &os, const CenterFacePieces &solution);
std::ostream &operator<<(std::ostream &os, const Scramble &scramble);
std::ostream &operator<<(std::ostream &os, const MoveList &moveList);
bool operator==(const CenterFacePieces &lhs, const CenterFacePieces &rhs);

#endif
