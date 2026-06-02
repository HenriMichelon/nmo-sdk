#!/bin/sh
# $Id: mkrelease.sh,v 1.1.1.1 2002/11/03 19:18:16 hmichelon Exp $
# This script build the source release & docs
# Only tested under FreeBSD 

tgz_opt="zcf"
tbz_opt="ycf"
zip_opt="-q -r -9"

if [ ! -f VERSION ]
then
	echo "Please run mkrelease.sh from the base nmo-sdk directory."
	exit 1
fi

VERSION=`cat VERSION`
echo "Building NMO-SDK release version $VERSION."

doxygen=`which doxygen`
if [ -z "$doxygen" ]
then
	echo "doxygen not found."
	exit 1
fi	

zip=`which zip`
if [ -z "$zip" ]
then
	echo "zip not found."
	exit 1
fi	

tar=`which tar`
if [ -z "$tar" ]
then
	echo "tar not found."
	exit 1
fi	

echo "Removing CVS directories..."
find . -name "CVS" -exec rm -rf {} \;
rm -rf CVSROOT

echo "Removing netatalk directories..."
find . -name ".AppleDouble" -exec rm -rf {} \;

echo "Setting permissions on directories"
find . -type d -print -exec chmod 0755 {} \;

echo "Setting permissions on files"
find . -type f -print -exec chmod 0644 {} \;
chmod 0755 configure
chmod 0755 examples/configure
chmod 0755 build/mkrelease.sh
chmod 0755 build/tools/mkdep

echo "Building sources archives..."
rm -rf docs
oldpwd=`pwd`
cd ..
DEST="nmo-sdk.src.$VERSION"
$tar $tgz_opt $DEST.tar.gz nmo-sdk-$VERSION
$tar $tbz_opt $DEST.tar.bz2 nmo-sdk-$VERSION
$zip $zip_opt $DEST.zip nmo-sdk-$VERSION

echo "Generating documentation..."
cd $oldpwd
mkdir ./docs 2>/dev/null
$doxygen build/docs/doxygen.cfg

echo "Building docs archives..."
cd ..
DEST="nmo-sdk.doc.$VERSION"
$tar $tgz_opt $DEST.tar.gz nmo-sdk-$VERSION/docs
$tar $tbz_opt $DEST.tar.bz2 nmo-sdk-$VERSION/docs
$zip $zip_opt $DEST.zip nmo-sdk-$VERSION/docs

