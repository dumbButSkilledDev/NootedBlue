//!  Copyright © 2023 ChefKiss Inc. Licensed under the Thou Shalt Not Profit License version 1.5. See LICENSE for
//!  details.

#include "NBlue.hpp"

enum ICLReliantGenerations : UInt32 {
	IceLake = 0,
	ElkhartLake, //! ELK & JSL are actually very similar hardware wise to ICL
	JasperLake,
	TigerLake,
};

class ICL {
	public:
	void init();
	bool processKext(KernelPatcher &patcher, size_t index, mach_vm_address_t address, size_t size);
	bool setDeviceSpecifics();
	
	ICL *callback;
	
	ICLReliantGenerations activeGeneration;
	bool disableFirmwareLoading = true;
};
