//!  Copyright © 2023 ChefKiss Inc. Licensed under the Thou Shalt Not Profit License version 1.5. See LICENSE for
//!  details.

#include "FramebufferPatcher.hpp"
#include <sys/sysctl.h>

//! EXPERIMENTAL AND SUBJECT TO CHANGE

//! Formula:
//! VRAM = RAM / 8
//! Stability of such formula needs to be tested, especially on low total RAM systems.

UInt64 FramebufferPatcher::getAmountOfVRamToPatchIn() {
	UInt64 memsize;
	size_t size = sizeof(memsize);
	sysctlbyname("hw.memsize", &memsize, &size, NULL, 0);
	memsize = ((memsize / 1024) / 1024); //! B -> KB -> MB
	DBGLOG("FramebufferPatcher", "Physical RAM: %llu MB, RAM to consume: %llu MB", memsize, (memsize / 8));
	return (memsize / 8);
}

//! # TODO: Allow the end-user to patch the framebuffer on their own by manually configuring the FB, atleast until I find out how to get GMBus IDs from the GMBus itself

//! The rest of the dynamic patcher is a Work-In-Progress
