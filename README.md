# eiti-tin-ftp-stattr
A simple system providing FTP traffic data collection &amp; analysis.

Build system (scons)
---

**Build all**  
``scons``  
or  
``scons all``

**Build specified project**  
_(Available projects: **agent**, **supervisor**, **terminal**)_  
``scons <projectname>``

**Building with different mode**  
_(Available modes: **release**, **debug**)_  
``scons mode=<modename>``

**Example: build _agent_ project in _release_ mode**  
``scons agent mode=release``
