gunzip ntl-11.5.1.tar.gz
tar xf ntl-11.5.1.tar
cd ntl-11.5.1/src
./configure --prefix=./deps
make
sudo make install