# NootedBlue
A [Lilu](https://github.com/acidanthera/Lilu) plugin that provides ~~support for unsupported Intel iGPUs~~

The Source Code of this Original Work is licensed under the `Thou Shalt Not Profit License version 1.0`. See [`LICENSE`](https://github.com/NootInc/NootedRed/blob/master/LICENSE)

## wait, what? it got updated internally?

hi yes that was me, anyways don't take this as a revival, im not going to fix your intel integrated graphics because fortunately im not stuck with a Tiger Lake or Alder Lake or any unsupported Lake device

Note: you still have to set `AAPL,ig-platform-id` and you also have to add `-disablegfxfirmware` if you are on Skylake+
Refer to [WhateverGreen's Framebuffer list](https://github.com/acidanthera/WhateverGreen/blob/master/Manual/FAQ.IntelHD.en.md) on what framebuffer to set for your system.
