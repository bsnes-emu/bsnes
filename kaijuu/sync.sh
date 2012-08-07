if [ -d ./nall ]; then rm -r ./nall; fi
if [ -d ./phoenix ]; then rm -r ./phoenix; fi
cp -r ../nall ./nall
cp -r ../phoenix ./phoenix
rm -r nall/test
rm -r phoenix/nall
rm -r phoenix/test
