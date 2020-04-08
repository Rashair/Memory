################################################################################
#
# Memory-game
#
################################################################################

MEMORY_GAME_VERSION = v1.2.1
MEMORY_GAME_SITE = git://github.com/Rashair/Memory.git
MEMORY_GAME_SITE_METHOD = git
MEMORY_GAME_DEPENDENCIES = libgpiod

define MEMORY_GAME_BUILD_CMDS
	$(MAKE) $(TARGET_CONFIGURE_OPTS) -C $(@D)
endef

define MEMORY_GAME_INSTALL_TARGET_CMDS
	$(INSTALL) -D -m 0755 $(@D)/memory-game $(TARGET_DIR)/usr/bin
endef


$(eval $(generic-package))
