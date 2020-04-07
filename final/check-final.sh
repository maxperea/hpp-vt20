# Script for checking Final.tar.gz file

# Remove old tmpdir_for_checking if it exists
rm -rf tmpdir_for_checking

echo Creating temporary directory tmpdir_for_checking

# Create new tmpdir_for_checking directory
mkdir tmpdir_for_checking || exit 1

echo Copying Final.tar.gz file into tmpdir_for_checking directory
cp Final.tar.gz tmpdir_for_checking || exit 1

# cd into the tmpdir_for_checking directory
cd tmpdir_for_checking || exit 1

echo Unpacking the Final.tar.gz file
tar -xzf Final.tar.gz || exit 1

echo Doing cd into Final directory
cd Final || exit 1

echo Checking that report.pdf file exists
ls -l report.pdf || exit 1

#### A3

echo Doing cd into A3 directory
cd A3 || exit 1

echo Checking that no .o files are included
find . -name \*.o | grep o && exit 1

echo Checking that galsim executable is not included
find . -name galsim | grep galsim && exit 1

echo Checking that no .gal files are included
find . -name \*.gal | grep gal && exit 1

echo Checking that make gives the galsim executable
make || exit 1
ls -l galsim || exit 1

echo Checking that doing make again gives \"is up to date\" message
(make | grep "is up to date") || (make | grep "Nothing to be done for" ) || exit 1

echo Checking that make clean removes .o files and galsim file
make clean || exit 1
find . -name \*.o | grep o && exit 1
find . -name galsim | grep galsim && exit 1

echo Going back into Final directory
cd .. || exit 1


#### A4

echo Doing cd into A4 directory
cd A4 || exit 1

echo Checking that no .o files are included
find . -name \*.o | grep o && exit 1

echo Checking that galsim executable is not included
find . -name galsim | grep galsim && exit 1

echo Checking that no .gal files are included
find . -name \*.gal | grep gal && exit 1

echo Checking that make gives the galsim executable
make || exit 1
ls -l galsim || exit 1

echo Checking that doing make again gives \"is up to date\" message
(make | grep "is up to date") || (make | grep "Nothing to be done for" ) || exit 1

echo Checking that make clean removes .o files and galsim file
make clean || exit 1
find . -name \*.o | grep o && exit 1
find . -name galsim | grep galsim && exit 1

echo Going back into Final directory
cd .. || exit 1

#### A5

echo Doing cd into A5 directory
cd A5 || exit 1

echo Checking that no .o files are included
find . -name \*.o | grep o && exit 1

echo Checking that galsim executable is not included
find . -name galsim | grep galsim && exit 1

echo Checking that no .gal files are included
find . -name \*.gal | grep gal && exit 1

echo Checking that make gives the galsim executable
make || exit 1
ls -l galsim || exit 1

echo Checking that doing make again gives \"is up to date\" message
(make | grep "is up to date") || (make | grep "Nothing to be done for" ) || exit 1

echo Checking that make clean removes .o files and galsim file
make clean || exit 1
find . -name \*.o | grep o && exit 1
find . -name galsim | grep galsim && exit 1

echo Going back into Final directory
cd .. || exit 1


#### A6

echo Doing cd into A6 directory
cd A6 || exit 1

echo Checking that no .o files are included
find . -name \*.o | grep o && exit 1

echo Checking that galsim executable is not included
find . -name galsim | grep galsim && exit 1

echo Checking that no .gal files are included
find . -name \*.gal | grep gal && exit 1

echo Checking that make gives the galsim executable
make || exit 1
ls -l galsim || exit 1

echo Checking that doing make again gives \"is up to date\" message
(make | grep "is up to date") || (make | grep "Nothing to be done for" ) || exit 1

echo Checking that make clean removes .o files and galsim file
make clean || exit 1
find . -name \*.o | grep o && exit 1
find . -name galsim | grep galsim && exit 1

echo Going back into Final directory
cd .. || exit 1



# If we get to this point, then all the different tests above have passed.
echo
echo Congratulations, your Final.tar.gz file seems OK!
echo
