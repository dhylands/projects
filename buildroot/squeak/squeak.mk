#############################################################
#
# squeak
#
#############################################################
SQUEAK_SITE:=http://piumarta.com/squeak/release
SQUEAK_VERSION:=Squeak-3.6-3
SQUEAK_SOURCE:=$(SQUEAK_VERSION).src.tar.gz
SQUEAK_DIR:=$(BUILD_DIR)/$(SQUEAK_VERSION)
SQUEAK_BINARY:=squeak
SQUEAK_TARGET_BINARY:=usr/bin/squeak
SQUEAK_CAT:=zcat

$(DL_DIR)/$(SQUEAK_SOURCE):
	 $(WGET) -P $(DL_DIR) $(SQUEAK_SITE)/$(SQUEAK_SOURCE)

squeak-source: $(DL_DIR)/$(SQUEAK_SOURCE)

$(SQUEAK_DIR)/.unpacked: $(DL_DIR)/$(SQUEAK_SOURCE)
	$(SQUEAK_CAT) $(DL_DIR)/$(SQUEAK_SOURCE) | tar -C $(BUILD_DIR) $(TAR_OPTIONS) -
	toolchain/patch-kernel.sh $(SQUEAK_DIR) package/squeak/ squeak-*.patch
	(cd $(SQUEAK_DIR)/platforms/unix/config; ./mkacinc > acplugins.m4; autoconf; rm acplugins.m4);
	touch $(SQUEAK_DIR)/.unpacked

$(SQUEAK_DIR)/build/.configured: $(SQUEAK_DIR)/.unpacked
	mkdir -p $(SQUEAK_DIR)/build
	(cd $(SQUEAK_DIR)/build; \
		$(TARGET_CONFIGURE_OPTS) \
		CFLAGS="$(TARGET_CFLAGS)" \
		../platforms/unix/config/configure \
		--target=$(GNU_TARGET_NAME) \
		--host=$(GNU_TARGET_NAME) \
		--build=$(GNU_HOST_NAME) \
		--disable-iconv \
		--prefix=/usr/squeak \
		--exec-prefix=/usr \
                --without-x \
		--without-npsqueak \
		--without-rfb \
		--without-ffi \
		$(DISABLE_NLS) \
		$(DISABLE_LARGEFILE) \
	);
	touch $(SQUEAK_DIR)/build/.configured

$(SQUEAK_DIR)/$(SQUEAK_BINARY): $(SQUEAK_DIR)/build/.configured
	$(MAKE) -C $(SQUEAK_DIR)/build

$(TARGET_DIR)/$(SQUEAK_TARGET_BINARY): $(SQUEAK_DIR)/$(SQUEAK_BINARY)
	$(MAKE) ROOT=$(TARGET_DIR) -C $(SQUEAK_DIR)/build install
	rm -rf $(TARGET_DIR)/usr/squeak

squeak: uclibc $(TARGET_DIR)/$(SQUEAK_TARGET_BINARY)

squeak-clean:
	$(MAKE) DESTDIR=$(TARGET_DIR) -C $(SQUEAK_DIR) uninstall
	-$(MAKE) -C $(SQUEAK_DIR) clean

squeak-dirclean:
	rm -rf $(SQUEAK_DIR)

#############################################################
#
# Toplevel Makefile options
#
#############################################################
ifeq ($(strip $(BR2_PACKAGE_SQUEAK)),y)
TARGETS+=squeak
endif
