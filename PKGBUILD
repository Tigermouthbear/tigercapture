pkgname='tigercapture'
pkgver="0.1"
pkgrel=0
url='https://github.com/Tigermouthbear/tigercapture'
license=('GPLv3')
pkgdesc="A screen capturing and uploading util"
arch=('x86_64')
makedepends=('make' 'cmake' 'git')
source=("git://github.com/Tigermouthbear/tigercapture.git")
sha1sums=('SKIP')

build() {
	echo $srcdir
	ls $srcdir
	cd $srcdir/$pkgname
	mkdir -p build
	cd build
	cmake \
            -DCMAKE_BUILD_TYPE='Release' \
            -DCMAKE_INSTALL_PREFIX='/usr' \
            -Wno-dev \
            ..
	make
}

package() {
	echo $srcdir/$pkgname/build
	ls $srcdir/$pkgname/build
	cd $srcdir/$pkgname/build
    #install -D -m644 "${startdir}/COPYING" "${pkgdir}/usr/share/licenses/${pkgname}/COPYING"
	make DESTDIR="$pkgdir" install
}
