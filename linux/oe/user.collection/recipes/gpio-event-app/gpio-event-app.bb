DESCRIPTION = "User App for Detecting GPIO events"
SECTION = "base"
PRIORITY = "optional"
LICENSE = "GPL"
DEPENDS = "gpio-event-module"

PV="svn${SRCDATE}"
PR = "r2"

SVN_REPO_URI = "http://svn.hylands.org/linux/gpio-event/app"
SRC_URI = "svn://svn.hylands.org/linux/gpio-event/;module=app;proto=http"

S = "${WORKDIR}/app"

TARGET_CC_ARCH += "${LDFLAGS}"

do_install() {	
    install -d ${D}${bindir}/
    install -m 0755 ${S}/gpio-event ${D}${bindir}/
}

PACKAGES = "${PN}"

FILES_${PN} = "${bindir}/gpio-event"

