//!  Copyright © 2023 ChefKiss Inc. Licensed under the Thou Shalt Not Profit License version 1.5. See LICENSE for
//!  details.

#include "NBlue.hpp"
#include <Headers/kern_api.hpp>
#include <Headers/kern_devinfo.hpp>
#include <Headers/kern_util.hpp>

SelectiveInjectionTable tbl[] = {
	{0x22B0, "Broadwell.xml"},
	{0x22B1, "Broadwell.xml"},
	{0x22B2, "Broadwell.xml"},
	{0x22B3, "Broadwell.xml"},
	{0xFFFF, nullptr},
};

NBlue *NBlue::callback = nullptr;

void NBlue::init() {
    callback = this;
    lilu.onPatcherLoadForce(
        [](void *user, KernelPatcher &patcher) { static_cast<NBlue *>(user)->processPatcher(patcher); }, this);
    lilu.onKextLoadForce(
        nullptr, 0,
        [](void *user, KernelPatcher &patcher, size_t index, mach_vm_address_t address, size_t size) {
            static_cast<NBlue *>(user)->processKext(patcher, index, address, size);
        },
        this);
}

void NBlue::processPatcher(KernelPatcher &patcher) {
    auto *devInfo = DeviceInfo::create();
    if (devInfo) {
        devInfo->processSwitchOff();

        this->iGPU = OSDynamicCast(IOPCIDevice, devInfo->videoBuiltin);
        PANIC_COND(!this->iGPU, "nblue", "videoBuiltin is not IOPCIDevice");

        WIOKit::renameDevice(this->iGPU, "IGPU");
        WIOKit::awaitPublishing(this->iGPU);
        char name[256] = {0};
        for (size_t i = 0, ii = 0; i < devInfo->videoExternal.size(); i++) {
            auto *device = OSDynamicCast(IOPCIDevice, devInfo->videoExternal[i].video);
            if (device) {
                snprintf(name, arrsize(name), "GFX%zu", ii++);
                WIOKit::renameDevice(device, name);
                WIOKit::awaitPublishing(device);
            }
        }

        static uint8_t builtin[] = {0x00};
        this->iGPU->setProperty("built-in", builtin, arrsize(builtin));
        this->deviceId = WIOKit::readPCIConfigValue(this->iGPU, WIOKit::kIOPCIConfigDeviceID);
		
		size_t tblSize = arrsize(tbl);
		bool matched = false;
		size_t tableEntry = 0;
			
		for (size_t i = 0; i < tblSize;) {
			if (tbl[i].deviceId == this->deviceId) {
				DBGLOG("NBlue", "Matched against table!");
				matched = true;
				tableEntry = i;
			}
			i++;
		}
		
		if (!matched) {
			PANIC("NBlue", "Failed to match against the device table!");
		}
		
		//! SKL, BDW & HSW got ditched
		if ((getKernelVersion() >= KernelVersion::Ventura && (!strcmp(tbl[tableEntry].xmlName, "Haswell.xml") || !strcmp(tbl[tableEntry].xmlName, "Broadwell.xml") || !strcmp(tbl[tableEntry].xmlName, "Skylake.xml")))) { return; }
		
		char filename[128];
		snprintf(filename, 128, "Framebuffer%s", tbl[tableEntry].xmlName);

        DeviceInfo::deleter(devInfo);
    } else {
        SYSLOG("nblue", "Failed to create DeviceInfo");
    }
}

bool NBlue::processKext(KernelPatcher &patcher, size_t index, mach_vm_address_t address, size_t size) {
    return true;
}
