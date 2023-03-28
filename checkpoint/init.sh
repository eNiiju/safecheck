# Check if root
if [ "$EUID" -ne 0 ]; then
    printf "Please run this script as root.\n"
    exit
fi

# Install bcm2835 library
wget http://www.airspayce.com/mikem/bcm2835/bcm2835-1.56.tar.gz
tar -zxf bcm2835-1.56.tar.gz
cd bcm2835-1.56
./configure
sudo make check
sudo make install
cd ..
rm -rf bcm2835-1.56 bcm2835-1.56.tar.gz
