# Data-Encryption

When communicating, the program encrypts data by performing bitwise XOR encryption of the ASCII value of each character in the message, converts the encrypted character to 7-bit binary string, and copying the encrypted binary string to a character array displayed as a specific QR code structure. 

To store longer messages, the program also compresses the 256-bit binary string to 64-bit hexadecimal string. 

This program utilizes bit manipulation concepts, such as XOR and con
