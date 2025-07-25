# TRAILzyb
### <ins>T</ins>ransmission of <ins>R</ins>emote <ins>A</ins>rea <ins>I</ins>nformation on <ins>L</ins>andscapes

John Pertell  
SUNY Polytechnic Institute  
Undergraduate Computer Science Capstone  
Capstone Advisor: Dr. Michael Reale  
Spring 2025  

TrailZYB is a software solution to capture and transmit landscape images from remote areas with no
cellular coverage. Utilizing amateur radio frequencies allocated by the FCC, TrailZYB provides software
utilities that encodes, transmits, decodes, and displays images alongside any other metadata the user
may want to provide. Operating TrailZYB requires an FCC amateur radio license which is subject to FCC
Part 97 rules & regulations.

[Capstone Report](Capstone_Report.pdf)

## Current Supported Operating Systems
- Raspberry Pi OS v6.6 (mobile stations)
- Ubuntu v24.04.1 desktop

## Current Supported Software Terminal Node Controller(TNC)
- Direwolf

## Installation

To build and install **mobile stations**, execute the following:
```sh
git clone https://github.com/Johnny4251/TRAILzyb
cd TRAILzyb
chmod +x install-mobile.sh
sudo ./install-mobile.sh
```

To build and install for **general purpose stations**, execute the following:
```sh
git clone https://github.com/Johnny4251/TRAILzyb
cd TRAILzyb
chmod +x install.sh
sudo ./install.sh
```

Ensure Direwolf AX.25 Software TNC is installed and active during use. The Direwolf project can be found at [Direwolf GitHub Repository](https://github.com/wb2osz/direwolf). 

## License
This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.
