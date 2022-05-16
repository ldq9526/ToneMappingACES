# ToneMappingACES
A command line tool of ACES tone mapping. The tool is implemented with OpenCV.

The tool is implemented with OpenCV. Note: HDR and OpenEXR support is required.

HDR image is processed as LDR image: LDR = (ACES(HDR * exposure))^(1.0/2.2)
