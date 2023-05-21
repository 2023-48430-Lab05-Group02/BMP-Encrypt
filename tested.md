# Tested Program Features

## Interactive Mode

### Encrypt Mode

#### 1 BPP
Encrypt produces filesize much smaller than expected. No Errors.

#### 32 BPP
Encrypt produces correct file that can then be decrypted successfully.

### Decrypt Mode

#### 1 BPP
Unable to produce 1BPP encrypted file for testing.

#### 32 BPP
Successful decrypt performed.

### Info Mode
- Handles encrypted files successfully: No decryption key provided.
- Files that have been encrypted then decrypted seem to crash.
- Outputs proper information from all other files as expected.

# Memory Safety
Still some considerable issues in bmp.c, esp related bmp_raw,
string copying for certain return types. Also, the bmp itself
in certain cases??
Uninit conditions caused by fread() directly into the file
header buffer -- This is valid, and they are actually initialized.