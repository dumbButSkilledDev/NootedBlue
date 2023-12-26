//!  Copyright © 2023 ChefKiss Inc. Licensed under the Thou Shalt Not Profit License version 1.5. See LICENSE for
//!  details.

#include "MEIFixer.hpp"
#include <IOKit/pci/IOPCIDevice.h>
#include <Headers/kern_iokit.hpp>

bool MEIFixer::start(IOService *provider) {
	//! # TODO: Find out how to flag matching for the TXE and IMEI
	IOLog("MEIFixer >>");
	IOPCIDevice *pci = OSDynamicCast(IOPCIDevice, provider);
	if (pci == nullptr) {
		IOLog("MEIFixer: wtf? pci pointer was null");
		return false;
	}
	UInt16 vendor = pci->configRead16(kIOPCIConfigVendorID);
	if (vendor == 0x8086) {
		WIOKit::renameDevice(provider, "IMEI");
	} else {
		IOLog("MEIFixer: wtf? vendor isn't 0x8086");
		return false;
	}
	return true;
};
