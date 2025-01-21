#include "octominx.h"

const Octominx::CenterArray Octominx::centerSideArray = {{{UBL, URB, ULR},
                                                          {NLB, NDL, NBD},
                                                          {LUN, LFU, LNF},
                                                          {FRL, FDR, FLD},
                                                          {RUF, RTU, RFT},
                                                          {TBR, TDB, TRD},
                                                          {BUT, BNU, BTN},
                                                          {DFN, DTF, DNT}}};

const Octominx::CenterArray Octominx::centerAdjArray1 = {{{LUN, BUT, RUF},
                                                          {LNF, DNT, BNU},
                                                          {ULR, FLD, NLB},
                                                          {LFU, RFT, DFN},
                                                          {URB, TRD, FRL},
                                                          {RTU, BTN, DTF},
                                                          {UBL, NBD, TBR},
                                                          {FDR, TDB, NDL}}};

const Octominx::CenterArray Octominx::centerAdjArray2 = {{{LFU, BNU, RTU},
                                                          {LUN, DFN, BTN},
                                                          {UBL, FRL, NDL},
                                                          {LNF, RUF, DTF},
                                                          {ULR, TBR, FDR},
                                                          {RFT, BUT, DNT},
                                                          {URB, NLB, TDB},
                                                          {FLD, TRD, NBD}}};

const Octominx::EdgeArray Octominx::edgeSideArray = {{
    {UL, UB, UR}, // U
    {BN, LN, DN}, // N
    {UL, LF, LN}, // L
    {LF, RF, DF}, // F
    {UR, RT, RF}, // R
    {RT, BT, DT}, // T
    {UB, BN, BT}, // B
    {DF, DT, DN}  // D
}};

const Octominx::CornerArray Octominx::cornerSideArray = {{
    {URLF, ULBN, UBRT}, // U
    {ULBN, DNFL, DTNB}, // N
    {URLF, DNFL, ULBN}, // L
    {URLF, DFRT, DNFL}, // F
    {URLF, UBRT, DFRT}, // R
    {UBRT, DTNB, DFRT}, // T
    {UBRT, ULBN, DTNB}, // B
    {DNFL, DFRT, DTNB}  // D
}};

const std::array<Center, centerCount *moveCount> CenterFacePieces::centerTargetArray = {
    URB, ULR, UBL, UBL, FRL, NDL, UBL, UBL, UBL, UBL, UBL, UBL, NBD, TBR, UBL, UBL, ULR, UBL, URB,
    URB, URB, URB, URB, URB, TRD, FRL, URB, URB, NLB, TDB, URB, URB, UBL, URB, ULR, ULR, FLD, NLB,
    ULR, ULR, TBR, FDR, ULR, ULR, ULR, ULR, ULR, ULR, NLB, NLB, NDL, NBD, ULR, FLD, NLB, NLB, NLB,
    NLB, NLB, NLB, TDB, URB, NLB, NLB, NDL, NDL, NBD, NLB, UBL, FRL, NDL, NDL, NDL, NDL, NDL, NDL,
    NDL, NDL, FDR, TDB, NBD, NBD, NLB, NDL, NBD, NBD, NBD, NBD, NBD, NBD, NBD, NBD, TBR, UBL, FLD,
    TRD, BUT, RUF, DFN, BTN, LFU, LNF, LUN, LUN, LUN, LUN, LUN, LUN, LUN, LUN, LUN, LUN, BNU, RTU,
    LFU, LFU, LNF, LUN, RFT, DFN, LFU, LFU, LFU, LFU, LFU, LFU, LFU, LFU, LNF, LNF, DNT, BNU, LUN,
    LFU, RUF, DTF, LNF, LNF, LNF, LNF, LNF, LNF, LNF, LNF, FRL, FRL, FRL, FRL, NDL, UBL, FDR, FLD,
    URB, TRD, FRL, FRL, FRL, FRL, FRL, FRL, FDR, FDR, FDR, FDR, FDR, FDR, FLD, FRL, ULR, TBR, FDR,
    FDR, FDR, FDR, TDB, NDL, FLD, FLD, FLD, FLD, NLB, ULR, FRL, FDR, FLD, FLD, FLD, FLD, FLD, FLD,
    TRD, NBD, LUN, BUT, RUF, RUF, RUF, RUF, DTF, LNF, RTU, RFT, RUF, RUF, RUF, RUF, RUF, RUF, LFU,
    BNU, RTU, RTU, RTU, RTU, RTU, RTU, RFT, RUF, BTN, DTF, RTU, RTU, RTU, RTU, RFT, RFT, RFT, RFT,
    RFT, RFT, DFN, LFU, RUF, RTU, BUT, DNT, RFT, RFT, RFT, RFT, TBR, TBR, TBR, TBR, TBR, TBR, TBR,
    TBR, FDR, ULR, TDB, TRD, UBL, NBD, TBR, TBR, TDB, TDB, TDB, TDB, TDB, TDB, TDB, TDB, TDB, TDB,
    TRD, TBR, URB, NLB, NDL, FDR, TRD, TRD, TRD, TRD, TRD, TRD, TRD, TRD, FRL, URB, TBR, TDB, TRD,
    TRD, NBD, FLD, RUF, LUN, BUT, BUT, BUT, BUT, BUT, BUT, BUT, BUT, DNT, RFT, BNU, BTN, BUT, BUT,
    RTU, LFU, LNF, DNT, BNU, BNU, BNU, BNU, BNU, BNU, BNU, BNU, BTN, BUT, BNU, BNU, BTN, BTN, LUN,
    DFN, BTN, BTN, BTN, BTN, BTN, BTN, DTF, RTU, BUT, BNU, BTN, BTN, DFN, DFN, BTN, LUN, DFN, DFN,
    LFU, RFT, DFN, DFN, DFN, DFN, DFN, DFN, DTF, DNT, DTF, DTF, DTF, DTF, DTF, DTF, LNF, RUF, DTF,
    DTF, RTU, BTN, DTF, DTF, DNT, DFN, DNT, DNT, BNU, LNF, DNT, DNT, DNT, DNT, DNT, DNT, RFT, BUT,
    DNT, DNT, DFN, DTF,
};

const std::array<Edge, edgeCount *moveCount> CenterFacePieces::edgeTargetArray = {
    UR, UB, UL, UL, LN, LF, UL, UL, UL, UL, UL, UL, UL, UL, UL, UL, UL, UR, UB, UB, UB, UB, UB, UB,
    UB, UB, UB, UB, BT, BN, UB, UB, UB, UL, UR, UR, UR, UR, UR, UR, RF, RT, UR, UR, UR, UR, UR, UR,
    LF, LF, LF, LF, UL, LN, DF, RF, LF, LF, LF, LF, LF, LF, LF, LF, BN, BN, DN, LN, BN, BN, BN, BN,
    BN, BN, BN, BN, UB, BT, BN, BN, LN, LN, BN, DN, LF, UL, LN, LN, LN, LN, LN, LN, LN, LN, LN, LN,
    RT, RT, RT, RT, RT, RT, RT, RT, UR, RF, DT, BT, RT, RT, RT, RT, BT, BT, BT, BT, BT, BT, BT, BT,
    BT, BT, RT, DT, BN, UB, BT, BT, RF, RF, RF, RF, RF, RF, LF, DF, RT, UR, RF, RF, RF, RF, RF, RF,
    DF, DF, DF, DF, DF, DF, RF, LF, DF, DF, DF, DF, DF, DF, DN, DT, DT, DT, DT, DT, DT, DT, DT, DT,
    DT, DT, BT, RT, DT, DT, DF, DN, DN, DN, LN, BN, DN, DN, DN, DN, DN, DN, DN, DN, DN, DN, DT, DF,
};
