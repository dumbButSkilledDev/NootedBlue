//!  Copyright © 2023 ChefKiss Inc. Licensed under the Thou Shalt Not Profit License version 1.5. See LICENSE for
//!  details.

#pragma once
#include <IOKit/IOService.h>

class MEIFixer : public IOService {
	OSDeclareDefaultStructors(MEIFixer);
	
	virtual bool start(IOService *provider);
};
