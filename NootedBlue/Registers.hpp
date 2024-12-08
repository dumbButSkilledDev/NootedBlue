
#pragma once
#include <IOKit/IOTypes.h>

static constexpr UInt32 CDCLK_CTL = 0x46000;
static constexpr UInt32 ICL_CDCLK_PLL_FREQ_REF_24_0 = 24000000 * 54;
static constexpr UInt32 ICL_CDCLK_PLL_FREQ_REF_19_2 = 19200000 * 68;
static constexpr UInt32 ICL_CDCLK_PLL_FREQ_REF_38_4 = 38400000 * 34;

static constexpr UInt32 TRANS_CLK_SEL_A = 0x46140;
static constexpr UInt32 TRANS_CLK_SEL_B = 0x46144;

static constexpr UInt32 SKL_DSSM = 0x51004;
static constexpr UInt32 ICL_DSSM_CDCLK_PLL_REFCLK_MASK = (7 << 29);
static constexpr UInt32 ICL_DSSM_CDCLK_PLL_REFCLK_24MHz = (0 << 29);
static constexpr UInt32 ICL_DSSM_CDCLK_PLL_REFCLK_19_2MHz = (1 << 29);
static constexpr UInt32 ICL_DSSM_CDCLK_PLL_REFCLK_38_4MHz = (2 << 29);
