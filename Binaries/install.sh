#!/bin/sh

ProjName=PCBUSB
ProjVersion=0.6
BaseDir=$(dirname $0)
InstallDir=/usr/local/lib
IncludeDir=/usr/local/include

if [ -f "$BaseDir/lib$ProjName.$ProjVersion.dylib" ]
then
	if [ -d "$InstallDir" ]
	then
		cp "$BaseDir/lib$ProjName.$ProjVersion.dylib" "$InstallDir/lib$ProjName.$ProjVersion.dylib"
	else
		mkdir "$InstallDir"; cp "$BaseDir/lib$ProjName.$ProjVersion.dylib" "$InstallDir/lib$ProjName.$ProjVersion.dylib"
	fi
	if [ ! -f "$InstallDir/lib$ProjName.dylib" ]
	then
		ln -s "$InstallDir/lib$ProjName.$ProjVersion.dylib" "$InstallDir/lib$ProjName.dylib"
	else
		rm "$InstallDir/lib$ProjName.dylib"; ln -s "$InstallDir/lib$ProjName.$ProjVersion.dylib" "$InstallDir/lib$ProjName.dylib"
	fi
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
