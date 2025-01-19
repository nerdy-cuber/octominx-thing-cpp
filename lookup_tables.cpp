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
    ULR, URB, UBL, UBL, NDL, FRL, UBL, UBL, UBL, UBL, UBL, UBL, TBR, NBD, UBL, UBL, UBL, ULR, URB,
    URB, URB, URB, URB, URB, FRL, TRD, URB, URB, TDB, NLB, URB, URB, URB, UBL, ULR, ULR, NLB, FLD,
    ULR, ULR, FDR, TBR, ULR, ULR, ULR, ULR, ULR, ULR, RUF, BUT, BTN, DFN, LNF, LFU, LUN, LUN, LUN,
    LUN, LUN, LUN, LUN, LUN, LUN, LUN, RTU, BNU, LFU, LFU, LUN, LNF, DFN, RFT, LFU, LFU, LFU, LFU,
    LFU, LFU, LFU, LFU, LNF, LNF, BNU, DNT, LFU, LUN, DTF, RUF, LNF, LNF, LNF, LNF, LNF, LNF, LNF,
    LNF, FRL, FRL, FRL, FRL, UBL, NDL, FLD, FDR, TRD, URB, FRL, FRL, FRL, FRL, FRL, FRL, FDR, FDR,
    FDR, FDR, FDR, FDR, FRL, FLD, TBR, ULR, FDR, FDR, FDR, FDR, NDL, TDB, FLD, FLD, FLD, FLD, ULR,
    NLB, FDR, FRL, FLD, FLD, FLD, FLD, FLD, FLD, NBD, TRD, BUT, LUN, RUF, RUF, RUF, RUF, LNF, DTF,
    RFT, RTU, RUF, RUF, RUF, RUF, RUF, RUF, BNU, LFU, RTU, RTU, RTU, RTU, RTU, RTU, RUF, RFT, DTF,
    BTN, RTU, RTU, RTU, RTU, RFT, RFT, RFT, RFT, RFT, RFT, LFU, DFN, RTU, RUF, DNT, BUT, RFT, RFT,
    RFT, RFT, TBR, TBR, TBR, TBR, TBR, TBR, TBR, TBR, ULR, FDR, TRD, TDB, NBD, UBL, TBR, TBR, TDB,
    TDB, TDB, TDB, TDB, TDB, TDB, TDB, TDB, TDB, TBR, TRD, NLB, URB, FDR, NDL, TRD, TRD, TRD, TRD,
    TRD, TRD, TRD, TRD, URB, FRL, TDB, TBR, TRD, TRD, FLD, NBD, LUN, RUF, BUT, BUT, BUT, BUT, BUT,
    BUT, BUT, BUT, RFT, DNT, BTN, BNU, BUT, BUT, LFU, RTU, DNT, LNF, BNU, BNU, BNU, BNU, BNU, BNU,
    BNU, BNU, BUT, BTN, BNU, BNU, BTN, BTN, DFN, LUN, BTN, BTN, BTN, BTN, BTN, BTN, RTU, DTF, BNU,
    BUT, BTN, BTN, NLB, NLB, NBD, NDL, FLD, ULR, NLB, NLB, NLB, NLB, NLB, NLB, URB, TDB, NLB, NLB,
    NDL, NDL, NLB, NBD, FRL, UBL, NDL, NDL, NDL, NDL, NDL, NDL, NDL, NDL, TDB, FDR, NBD, NBD, NDL,
    NLB, NBD, NBD, NBD, NBD, NBD, NBD, NBD, NBD, UBL, TBR, TRD, FLD, DFN, DFN, LUN, BTN, DFN, DFN,
    RFT, LFU, DFN, DFN, DFN, DFN, DFN, DFN, DNT, DTF, DTF, DTF, DTF, DTF, DTF, DTF, RUF, LNF, DTF,
    DTF, BTN, RTU, DTF, DTF, DFN, DNT, DNT, DNT, LNF, BNU, DNT, DNT, DNT, DNT, DNT, DNT, BUT, RFT,
    DNT, DNT, DTF, DFN,
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
