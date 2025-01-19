    auto moves = scrambleToMoves("U L F R B D N T");
    for (Move move : moves) {
        std::cout << move << "\n";
        octominx.doMove(move);
        std::cout << octominx << "\n\n";
    }
    octominx.doMove({U, CW});