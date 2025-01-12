#include "stdafx.h"
#include "flags.h"

namespace flags
{

const MDFlagsSetOUT pFlagsOrderUpdate15[] {
    {"\"Day\":", MDFlagsSet::Day},                                                   // 0x1
    {"\"IOC\":", MDFlagsSet::IOC},                                                   // 0x2
    {"\"NonQuote\":", MDFlagsSet::NonQuote},                                         // 0x4
    {"\"EndOfTransaction\":", MDFlagsSet::EndOfTransaction},                     // 0x01000
    {"\"DueToCrossCancel\":", MDFlagsSet::DueToCrossCancel},                     // 0x02000
//    {"\"SecondLeg\":", MDFlagsSet::SecondLeg},                                 // 0x04000
    {"\"FOK\":", MDFlagsSet::FOK},                                               // 0x80000
    {"\"Replace\":", MDFlagsSet::Replace},                                      // 0x100000
    {"\"Cancel\":", MDFlagsSet::Cancel},                                        // 0x200000
    {"\"MassCancel\":", MDFlagsSet::MassCancel},                                // 0x400000
    {"\"Negotiated\":", MDFlagsSet::Negotiated},                               // 0x4000000
    {"\"MultiLeg\":", MDFlagsSet::MultiLeg},                                   // 0x8000000
    {"\"CrossTrade\":", MDFlagsSet::CrossTrade},                              // 0x20000000
    {"\"NegotiatedMatchByRef\":", MDFlagsSet::NegotiatedMatchByRef},          // 0x80000000
    {"\"COD\":", MDFlagsSet::COD},                                           // 0x100000000
//    {"\"ActiveSide\":", MDFlagsSet::ActiveSide},                         // 0x20000000000
//    {"\"PassiveSide\":", MDFlagsSet::PassiveSide},                       // 0x40000000000
    {"\"Synthetic\":", MDFlagsSet::Synthetic},                            // 0x200000000000
    {"\"RFS\":", MDFlagsSet::RFS},                                        // 0x400000000000
    {"\"SyntheticPassive\":", MDFlagsSet::SyntheticPassive},           // 0x200000000000000
    {"\"BOC\":", MDFlagsSet::BOC},                                    // 0x1000000000000000
    {"\"DuringDiscreteAuction\":", MDFlagsSet::DuringDiscreteAuction},// 0x4000000000000000
    {"", MDFlagsSet::DuringDiscreteAuction }
};

const MDFlagsSetOUT pFlagsOrderExecution16[]{
    {"\"Day\":", MDFlagsSet::Day},
    {"\"IOC\":", MDFlagsSet::IOC},
    {"\"NonQuote\":", MDFlagsSet::NonQuote},
    {"\"EndOfTransaction\":", MDFlagsSet::EndOfTransaction},
//    {"\"DueToCrossCancel\":", MDFlagsSet::DueToCrossCancel},
//    {"\"SecondLeg\":", MDFlagsSet::SecondLeg},
//    {"\"FOK\":", MDFlagsSet::FOK},
//    {"\"Replace\":", MDFlagsSet::Replace},
//    {"\"Cancel\":", MDFlagsSet::Cancel},
//    {"\"MassCancel\":", MDFlagsSet::MassCancel},
    {"\"Negotiated\":", MDFlagsSet::Negotiated},
    {"\"MultiLeg\":", MDFlagsSet::MultiLeg},
//    {"\"CrossTrade\":", MDFlagsSet::CrossTrade},
    {"\"NegotiatedMatchByRef\":", MDFlagsSet::NegotiatedMatchByRef},
//    {"\"COD\":", MDFlagsSet::COD},
    {"\"ActiveSide\":", MDFlagsSet::ActiveSide},
    {"\"PassiveSide\":", MDFlagsSet::PassiveSide},
    {"\"Synthetic\":", MDFlagsSet::Synthetic},
    {"\"RFS\":", MDFlagsSet::RFS},
//    {"\"SyntheticPassive\":", MDFlagsSet::SyntheticPassive},
//    {"\"BOC\":", MDFlagsSet::BOC},
    {"\"DuringDiscreteAuction\":", MDFlagsSet::DuringDiscreteAuction},
    {"", MDFlagsSet::DuringDiscreteAuction }
};

}; // namespace flags
