//!  Copyright © 2023 ChefKiss Inc. Licensed under the Thou Shalt Not Profit License version 1.5. See LICENSE for
//!  details.

#include "NBlue.hpp"
#include "FramebufferPatcher.hpp"

struct BDWFramebuffer {
	uint32_t framebufferId;
	uint8_t  fMobile;
	uint8_t  fPipeCount;
	uint8_t  fPortCount;
	uint8_t  fFBMemoryCount;
	uint32_t fStolenMemorySize;
	uint32_t fFramebufferMemorySize;
	uint32_t fUnifiedMemorySize;
	uint32_t fBacklightFrequency;
	uint32_t fBacklightMax;
	uint32_t pad[3];
	ConnectorInfo connectors[4];
	FramebufferFlags flags;
	uint32_t unk1;
	uint32_t camelliaVersion;
	uint32_t unk2[6];
	uint32_t fNumTransactionsThreshold;
	uint32_t fVideoTurboFreq;
	uint32_t fRC6_Threshold;
} PACKED;

class BDW {
    public:
	void init();
	bool processKext(KernelPatcher &patcher, size_t index, mach_vm_address_t address, size_t size);
	
	bool isCherryview;
};
