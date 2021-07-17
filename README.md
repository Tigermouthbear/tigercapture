# TigerCapture
An opensource screenshotting and file uploading tool for GNU/Linux. Supports uploading to Imgur and custom hosts using configuration similar to ShareX.

## Installing

### Arch
AUR package available as `tigercapture` at https://aur.archlinux.org/packages/tigercapture/
````
git clone https://aur.archlinux.org/tigercapture.git
cd tigercapture
makepkg -si
````

### Other Distros
````
git clone --recursive https://github.com/Tigermouthbear/tigercapture.git
cd tigercapture
mkdir build && cd build
cmake ..
make
sudo make install
````

#### Dependencies
- Qt5