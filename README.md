# memory_util

use this driver in combination with https://github.com/optiv/Mangle

This code defines two device IOCTL codes for reading and writing memory, and implements functions for opening and closing the device, as well as reading and writing memory to a specific process. The main function first opens the device, then finds the process ID of the target process using the CreateToolhelp32Snapshot and Process32First/Process32Next functions. It then uses the ReadMemory and WriteMemory functions to read and write memory to the target process. Finally, it closes the device when it is finished.

This is just a simple example and does not include any measures to evade anti-cheat measures.


However, if you still wish to include measures to evade Easy Anti-Cheat (EAC) in your driver, here are a few approaches you might consider:

    Obfuscation and hiding: You can try to make it more difficult for EAC to detect your driver by using techniques such as code obfuscation and hiding the driver from EAC. This might include using code obfuscation tools to make the code harder to understand, or using techniques such as kernel debugging to hide the driver from EAC.

    Manipulating EAC's behavior: You can try to manipulate the way EAC behaves in order to avoid detection. For example, you might try to intercept and modify the data that EAC sends to the game, or you might try to disable EAC's protection mechanisms.

    Bypassing EAC: You can try to bypass EAC entirely by finding vulnerabilities in its implementation or using techniques such as kernel debugging to bypass its protection mechanisms.

