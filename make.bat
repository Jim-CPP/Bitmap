cls

del *.exe

g++ -mwindows -m64 -static -Wall -Wextra Bitmap.cpp BitmapClass.cpp -o Bitmap.exe -loleaut32 -luuid -lole32
