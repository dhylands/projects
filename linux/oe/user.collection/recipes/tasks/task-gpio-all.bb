DESCRIPTION = "All user space GPIO manipulation packages"

inherit task

PR = "r2"

RRECOMMENDS_${PN} = " \
	gpio-app gpio-lib gpio-module \
	gpio-event-app gpio-event-module \
	"

