# ToneMappingACES
A command line tool of ACES tone mapping.

The tool is implemented with OpenCV. Note: HDR and OpenEXR support is required.

The ACES curve this tool used is a simplified version from: https://knarkowicz.wordpress.com/2016/01/06/aces-filmic-tone-mapping-curve/

HDR image is processed as LDR image: LDR = (ACES(HDR * exposure))^(1.0/2.2)
