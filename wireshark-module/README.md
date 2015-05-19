#﻿HOWTO: WIRESHARK LUA JSON DISSECTOR

**1. Download Wireshark**

	sudo apt-get install wireshark

**2. Change access permissions to sniff as a user**

	 sudo dpkg-reconfigure wireshark-common 
 	 sudo usermod -a -G wireshark $USER

**3. Logout**

	gnome-session-quit --logout --no-prompt

**4. Add LUA file to Wireshark plugins**

	cp ./eiti-tin-ftp-stattr/tin-json.lua /usr/lib/x86_64-linux-gnu/wireshark/libwireshark3/plugins

**5. Check if plugin was added properly**

	Wireshark -> Help -> About Wireshark -> plugins -> tin-json.lua

**6. Sniff**
