DESCRIPTION = "User Lib for manipulating GPIO pins"
SECTION = "base"
PRIORITY = "optional"
LICENSE = "GPL"
DEPENDS = "gpio-module"

PV="svn${SRCDATE}"
PR = "r0"

SVN_REPO_URI = "http://svn.hylands.org/linux/gpio/lib"
SRC_URI = "svn://svn.hylands.org/linux/gpio/;module=lib;proto=http"

S = "${WORKDIR}/lib"

TARGET_CC_ARCH += "${LDFLAGS}"

do_stage () {
    oe_libinstall -so -a libgpio ${STAGING_LIBDIR}
    install -m 0644 ${S}/user-gpio.h ${STAGING_INCDIR}/
}

do_install() {	
    install -d ${D}${libdir}/
    install -m 0755 ${S}/libgpio.so ${D}${libdir}/
}

PACKAGES = "${PN}"

FILES_${PN} = "${libdir}/libgpio.so"

