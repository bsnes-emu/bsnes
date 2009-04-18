rm -r nall
rm -r ruby
rm -r bpp

cp -r ../../../nall ./nall
cp -r ../../../ruby ./ruby
cp -r ../../../bpp ./bpp

rm ruby/test*
rm ruby/cc.*

rm bpp/test*
rm bpp/*.bpp
rm bpp/bpp
rm bpp/cc.*
