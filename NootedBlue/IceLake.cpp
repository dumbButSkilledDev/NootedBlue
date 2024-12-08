//!  Copyright © 2023 ChefKiss Inc. Licensed under the Thou Shalt Not Profit License version 1.5. See LICENSE for
//!  details.

#include "IceLake.hpp"
#include "Registers.hpp"
#include <Headers/kern_api.hpp>

static const char *pathIceLakeFB = "/System/Library/Extensions/AppleIntelICLLPGraphicsFramebuffer.kext/Contents/MacOS/"
							   "AppleIntelICLLPGraphicsFramebuffer";
static const char *pathIceLakeHW =
	"/System/Library/Extensions/AppleIntelICLGraphics.kext/Contents/MacOS/AppleIntelICLGraphics";

static KernelPatcher::KextInfo kextIceLakeFB {"com.apple.driver.AppleIntelICLLPGraphicsFramebuffer", &pathIceLakeFB, 1, {}, {},
	KernelPatcher::KextInfo::Unloaded};
static KernelPatcher::KextInfo kextIceLakeHW {"com.apple.driver.AppleIntelICLGraphics", &pathIceLakeHW, 1, {}, {},
	KernelPatcher::KextInfo::Unloaded};

void ICL::init() {
	callback = this;
	
	lilu.onKextLoadForce(&kextIceLakeFB);
	lilu.onKextLoadForce(&kextIceLakeHW);
}

bool ICL::setDeviceSpecifics() {
	switch (NBlue::callback->deviceId) {
		case 0x8A50:
		case 0x8A54:
		case 0x8A56:
		case 0x8A57:
		case 0x8A58:
		case 0x8A59:
			DBGLOG("ICL", "setDeviceSpecifics: iGPU is an Ice Lake derivative");
			this->activeGeneration = IceLake;
			break;
		case 0x4541:
		case 0x4551:
		case 0x4555:
		case 0x4557:
		case 0x4570:
		case 0x4571:
			DBGLOG("ICL", "setDeviceSpecifics: iGPU is an Elkhart Lake derivative");
			this->activeGeneration = ElkhartLake;
			break;
		case 0x4E51:
		case 0x4E55:
		case 0x4E57:
		case 0x4E61:
		case 0x4E71:
			DBGLOG("ICL", "setDeviceSpecifics: iGPU is a Jasper Lake derivative");
			this->activeGeneration = JasperLake;
			break;
		//! # TODO: Fill in Tiger Lake spots
	}
	return true;
}

bool ICL::processKext(KernelPatcher &patcher, size_t index, mach_vm_address_t address, size_t size) {
	//! # TODO: Port WhateverGreen fixes such as `-igfxcdc` and `-igfxdvmt`
	if (kextIceLakeFB.loadIndex == index) {
		DBGLOG("ICL", "Framebuffer kext has loaded!");
	} else if (kextIceLakeHW.loadIndex == index) {
		DBGLOG("ICL", "Graphics Accelerator kext has loaded!");
		return true;
	}
	return false;
}

//! https://elixir.bootlin.com/linux/latest/source/drivers/gpu/drm/i915/display/intel_cdclk.c#L1423
UInt32 ICL::wrapGetCDClockVoltageLevel(void *) {
	if (callback->activeGeneration >= ICLReliantGenerations::TigerLake) {
		if (callback->cdclock > 556800) {
			return 3;
		} else if (callback->cdclock > 326400) {
			return 2;
		} else if (callback->cdclock > 312000) {
			return 1;
		} else {
			return 0;
		}
	} else if (callback->activeGeneration == ICLReliantGenerations::ElkhartLake || callback->activeGeneration == ICLReliantGenerations::JasperLake) {
		if (callback->cdclock > 326400) {
			return 3;
		} else if (callback->cdclock > 312000) {
			return 2;
		} else if (callback->cdclock > 180000) {
			return 1;
		} else {
			return 0;
		}
	} else {
		if (callback->cdclock > 556800) {
			return 2;
		} else if (callback->cdclock > 312000) {
			return 1;
		} else {
			return 0;
		}
	}
	return 0;
}

void ICL::tigerLakeRegisterFixups(void *that, UInt32 addr, UInt32 val) {
	if (addr == TRANS_CLK_SEL_A || addr == TRANS_CLK_SEL_B) {
		//! TLDR; Intel right shifts by 0x28 on Tiger Lake rather than 0x29.
		//! Fixes that here
		val = (val << 1);
	}
}
