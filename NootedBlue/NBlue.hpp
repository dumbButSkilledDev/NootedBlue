//!  Copyright © 2023 ChefKiss Inc. Licensed under the Thou Shalt Not Profit License version 1.5. See LICENSE for
//!  details.

#pragma once
#include <Headers/kern_patcher.hpp>
#include <Headers/kern_user.hpp>
#include <Headers/kern_util.hpp>
#include <IOKit/pci/IOPCIDevice.h>

struct SelectiveInjectionTable {
	UInt16 deviceId;
	const char *xmlName;
};

//! # TODO: Fill in Fimrware sections with IOKit Personality entries

SelectiveInjectionTable tbl[] = {
	{0x22B0, "Broadwell.xml"},
	{0x22B1, "Broadwell.xml"},
	{0x22B2, "Broadwell.xml"},
	{0x22B3, "Broadwell.xml"},
	{0xFFFF, nullptr},
};

class NBlue {
    public:
    static NBlue *callback;
    void init();
    void processPatcher(KernelPatcher &patcher);
    bool processKext(KernelPatcher &patcher, size_t index, mach_vm_address_t address, size_t size);

	UInt32 deviceId;
    IOPCIDevice *iGPU {nullptr};
};
