DESCRIPTION = "Linux Driver for Detecting GPIO events"
SECTION = "base"
PRIORITY = "optional"
LICENSE = "GPL"
RDEPENDS = "kernel (${KERNEL_VERSION})"
DEPENDS = "virtual/kernel"

PR = "r2"

INITSCRIPT_NAME = "gpio-event"
INITSCRIPT_PARAMS = "defaults 40"

SVN_REPO_URI = "http://svn.hylands.org/linux/gpio-event"
SRC_URI = "svn://svn.hylands.org/linux/gpio-event"

S = "${WORKDIR}"

inherit module update-rc.d

do_install() {	
    install -d ${D}${base_libdir}/modules/${KERNEL_VERSION}/extra/
    install -m 0644 ${WORKDIR}/gpio-event.ko ${D}${base_libdir}/modules/${KERNEL_VERSION}/extra/

    install -d ${D}${sysconfdir}/init.d/
    install -m 0755 ${WORKDIR}/gpio-event.init ${D}${sysconfdir}/init.d/gpio-event
}

PACKAGES = "${PN}"

FILES_${PN} = "${base_libdir}/modules/"
FILES_${PN} += "${sysconfdir}/"

