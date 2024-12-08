//!  Copyright © 2023 ChefKiss Inc. Licensed under the Thou Shalt Not Profit License version 1.5. See LICENSE for
//!  details.

#include "NBlue.hpp"

//! thx WEG
enum ICLCoreDisplayClockDecimalFrequency {
	ICL_CDCLK_FREQ_172_8 = 0x158,
	ICL_CDCLK_FREQ_180_0 = 0x166,
	ICL_CDCLK_FREQ_192_0 = 0x17E,
	ICL_CDCLK_FREQ_307_2 = 0x264,
	ICL_CDCLK_FREQ_312_0 = 0x26E,
	ICL_CDCLK_FREQ_552_0 = 0x44E,
	ICL_CDCLK_FREQ_556_8 = 0x458,
	ICL_CDCLK_FREQ_648_0 = 0x50E, //! lowest w/o panic, wtf Intel?
	ICL_CDCLK_FREQ_652_8 = 0x518,
};

enum ICLReliantGenerations : UInt32 {
	IceLake = 0,
	ElkhartLake, //! ELK & JSL are actually very similar hardware wise to ICL
	JasperLake,
	TigerLake,
	RocketLake,
};

class ICL {
	public:
	void init();
	bool processKext(KernelPatcher &patcher, size_t index, mach_vm_address_t address, size_t size);
	bool setDeviceSpecifics();
	
private:
	static ICL *callback;
	
	ICLReliantGenerations activeGeneration;
	bool disableFirmwareLoading = true;
	int cdclock = 0;
	
	static UInt32 wrapGetCDClockVoltageLevel(void *that);
	static void tigerLakeRegisterFixups(void *that, UInt32 addr, UInt32 val);
};

// ----- Patches ----- //

//! IGAccelerator::probe
static const UInt8 kIGAccelProbeBypassCheckOriginal[] = {0x3D, 0x86, 0x80, 0x02, 0xFF, 0x74, 0x2A};
static const UInt8 kIGAccelProbeBypassCheckPatched[] = {0x66, 0x90, 0x66, 0x90, 0x90, 0xEB, 0x2A};

static const UInt8 kIGAccelGetGPUInfoBypassCheckOriginal[] = {0x83, 0xF9, 0x01, 0x0F, 0x85, 0x4E, 0x03, 0x00, 0x00};
static const UInt8 kIGAccelGetGPUInfoBypassCheckPatched[] = {0x83, 0xF9, 0x01, 0xE9, 0x90, 0x01, 0x00, 0x00, 0x00};
