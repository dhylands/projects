DESCRIPTION = "User App for manipulating GPIO pins"
SECTION = "base"
PRIORITY = "optional"
LICENSE = "GPL"
DEPENDS = "gpio-lib"

PV="svn${SRCDATE}"
PR = "r0"

SVN_REPO_URI = "http://svn.hylands.org/linux/gpio/app"
SRC_URI = "svn://svn.hylands.org/linux/gpio/;module=app;proto=http"

S = "${WORKDIR}/app"

TARGET_CC_ARCH += "${LDFLAGS}"

do_install() {	
    install -d ${D}${bindir}/
    install -m 0755 ${S}/gpio ${D}${bindir}/
}

PACKAGES = "${PN}"

FILES_${PN} = "${bindir}/gpio"

