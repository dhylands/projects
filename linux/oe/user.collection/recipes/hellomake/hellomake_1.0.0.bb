DESCRIPTION = "hello world sample program"

PR = "r0"

DEPENDS = ""

SRC_URI = " \
  file://hello.c \
  file://Makefile \
"

S = "${WORKDIR}"

do_install () {
    install -d ${D}${bindir}/
    install -m 0755 ${S}/hello ${D}${bindir}/
}

FILES_${PN} = "${bindir}/hello"

