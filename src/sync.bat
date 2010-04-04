rmdir /Q /S libco
rmdir /Q /S nall
rmdir /Q /S ruby

mkdir libco
mkdir nall
mkdir ruby

xcopy /E ..\..\libco libco
xcopy /E ..\..\nall nall
xcopy /E ..\..\ruby ruby
