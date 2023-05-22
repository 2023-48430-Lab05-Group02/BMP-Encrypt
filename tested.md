# Tested Program Features

## Interactive Mode

### Encrypt Mode

#### 1 BPP
Fixed

#### 8 BPP
Fixed

#### 32 BPP
Encrypt produces correct file that can then be decrypted successfully.

### Decrypt Mode

#### 1 BPP
Fixed

#### 8 BPP
Fixed

#### 32 BPP
Successful decrypt performed.

### Compress Mode

#### 1 BPP
Fixed

#### 8 BPP
Fixed

#### 32 BPP
Working as expected.

### Info Mode
- Handles encrypted files successfully: No decryption key provided.
- ~~Files that have been encrypted then decrypted seem to crash.~~ Related to encryption/decryption problems.
- Outputs proper information from all other files as expected.

# Memory Safety
Still some considerable issues in bmp.c, esp related bmp_raw,
string copying for certain return types. Also, the bmp itself
in certain cases??
Uninit conditions caused by fread() directly into the file
header buffer -- This is valid, and they are actually initialized.