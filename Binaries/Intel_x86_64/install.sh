#!/bin/sh

ProjName=PCBUSB
ProjVersion=0.13
BaseDir=$(dirname $0)
InstallDir=/usr/local/lib
IncludeDir=/usr/local/include

if [ -f "$BaseDir/lib$ProjName.$ProjVersion.dylib" ]
then
	if [ ! -d "$InstallDir" ]
	then
		mkdir "$InstallDir"
	fi
	if [ -f "$InstallDir/lib$ProjName.dylib" ]
	then
		rm -f "$InstallDir/lib$ProjName.dylib"
	fi
	if [ -f "$InstallDir/lib$ProjName.$ProjVersion.dylib" ]
	then
		rm -f "$InstallDir/lib$ProjName.$ProjVersion.dylib"
	fi
	cp "$BaseDir/lib$ProjName.$ProjVersion.dylib" "$InstallDir/lib$ProjName.$ProjVersion.dylib"
	ln -s "$InstallDir/lib$ProjName.$ProjVersion.dylib" "$InstallDir/lib$ProjName.dylib"
	chmod 755 "$InstallDir/lib$ProjName.$ProjVersion.dylib"
	chmod 755 "$InstallDir/lib$ProjName.dylib"
else
	echo "error dylib"
fi

if [ -f "$BaseDir/$ProjName.h" ]
then
	if [ -d "$IncludeDir" ]
	then
		cp "$BaseDir/$ProjName.h" "$IncludeDir/$ProjName.h"
	else
		mkdir "$IncludeDir"; cp "$BaseDir/$ProjName.h" "$IncludeDir/$ProjName.h"
	fi
else
	echo "error header"
fi
