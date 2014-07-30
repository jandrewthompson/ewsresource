What The Heck Is This??
=======================

Good Question.

Mostly, this is a place for me to fiddle around with KDE and Akonadi 
development.  The ultimate goal would be to fetch mail/calender via the Exchange
Web Services (EWS) into Akonadi.  At this time, there doesn't seem to be a legitimate
solution to using KMail/Kontact with Exchange that doesn't involve IMAP 
(not always possible for some users) or some other Rube Goldberg machine.  
I've tried this.  It's fun for a while, but not sustainable for real work.  

Secondary goal for this project is to have an excuse to brush up on my C++ skills.
Not my favorite language by any stretch, but I feel it's worth remaining competent in.


How To Build
===========

### On Unix:
```
cd <project_name_path>
mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX=$KDEDIRS -DCMAKE_BUILD_TYPE=Debug ..      <- do not forget the ..
make
make install or su -c 'make install'
```

where $KDEDIRS points to your KDE installation prefix 
    ( probably /usr )

to uninstall the project:
```
make uninstall or su -c 'make uninstall'
```

Note: you can use another build path. Then cd in your build dir and:
```
export KDE_SRC=path_to_your_src
cmake $KDE_SRC -DCMAKE_INSTALL_PREFIX=$KDEDIRS -DCMAKE_BUILD_TYPE=Debug
```

### On Windows:

```
cd <project_name_path>
mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX=%KDEDIRS% -DCMAKE_BUILD_TYPE=Debug ..      <- do not forget the ..
[n]make
[n]make install
```

where %KDEDIRS% points to your KDE installation prefix.

to uninstall the project:
```
[n]make uninstall
```

*Note: use nmake if you're building with the Visual Studio compiler, or make
if you're using the minGW compiler*



Documentation
=============

The Akonadi-KDE API documentation can be found here:
http://api.kde.org/4.x-api/kdepimlibs-apidocs/akonadi/html/index.html

General developer information, e.g. tutorials can be found here:
http://techbase.kde.org/Projects/PIM/Akonadi

Project homepage is http://www.akonadi-project.org/

IRC: #akonadi on freenode network

----------------------------
Please send all comments to:
Kevin Krammer
kevin@akonadi-project.org

Last update: July 2009
