#pragma once

#include "pluginterfaces/base/funknown.h"
#include "pluginterfaces/vst/vsttypes.h"

namespace JonasM {

enum MyParams : Steinberg::Vst::ParamID {
    kParamDCOffsetId = 102,
    kParamClipId = 103,
    kParamMultiplierId = 104,
};

static const Steinberg::FUID kDCOffsetterProcessorUID (0x54B41A67, 0x59565BB1, 0x8EEEE67E, 0x16FEA83D);
static const Steinberg::FUID kDCOffsetterControllerUID (0xFEA0DCD2, 0x3353576E, 0x8CF33EBE, 0x36CEAFA0);

#define DCOffsetterVST3Category "Fx"

}
