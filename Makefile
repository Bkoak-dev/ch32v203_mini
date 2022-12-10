# @file        Makefile
# @brief       The outermost Makefile
# @details     The root Makefile to compile the project
# @author      wangjingfan726@gmail.com
# @date        2022-12-10
# @version     v0.0.1

ifneq ($(wildcard .config),)
        -include .config
else
        $(warning oh dear, please make defconfig first.)
endif

CPU          ?=ch32v103
BOARD        ?=mini

sub-dir      := ld
sub-makefile := $(foreach dir,$(sub-dir),$(dir)/Makefile)
include $(sub-makefile)

NAME         := $(CONFIG_IMG_NAME:"%"=%)
OBJ_DIR      := build/$(CPU)_$(BOARD)
KCONFIG_DIR  := $(PWD)/tools/kconfig
GENERATED_DIR:= include/generated
LINK_SCRIPT  := $(CONFIG_LINK_FILE:"%"=%)

#compile info
REPO_REV     := $(shell git log --pretty=format:"%H" -1)
COMPILE_HOST := $(shell whoami | sed 's/\\/\\\\/')@$(shell hostname)
COMPILE_TIME := $(shell date -R)
COMPILE_DIFF := $(shell mkdir -p $(OBJ_DIR); git diff > $(OBJ_DIR)/patch_id; md5sum $(OBJ_DIR)/patch_id | awk '{print $$1}')

###############################################################################################
.phony: compile config menuconfig help

compile:
	@echo "#ifndef COMPILE_H_" > $(GENERATED_DIR)/compile.h
	@echo "#define COMPILE_H_" >> $(GENERATED_DIR)/compile.h
	@echo "" >> $(GENERATED_DIR)/compile.h
	@echo "#define CH32V103_MINIMINI_COMMIT_ID   \"$(REPO_REV)\"" >> $(GENERATED_DIR)/compile.h
	@echo "#define CH32V103_MINI_BUILD_UTS   \"$(COMPILE_TIME)\"" >> $(GENERATED_DIR)/compile.h
	@echo "#define CH32V103_MINI_BUILD_HOST  \"$(COMPILE_HOST)\"" >> $(GENERATED_DIR)/compile.h
	@echo "#define CH32V103_MINI_PATCH_ID    \"$(COMPILE_DIFF)\"" >> $(GENERATED_DIR)/compile.h
	@echo "#define CH32V103_MINI_BOARD_TYPE  \"$(BOARD)\"" >> $(GENERATED_DIR)/compile.h
	@echo "#define CH32V103_MINI_LD_FILE     \"$(CONFIG_LINK_FILE)\"" >> $(GENERATED_DIR)/compile.h
	@echo "" >> $(GENERATED_DIR)/compile.h
	@echo "#endif // COMPILE_H_" >> $(GENERATED_DIR)/compile.h

%defconfig: compile
	@echo make config: $@
	@cp configs/$@ .config
	@$(MAKE) -C $(KCONFIG_DIR)
	@$(KCONFIG_DIR)/conf -s --silentoldconfig Kconfig

menuconfig:
	@$(MAKE) -C $(KCONFIG_DIR)
	@$(KCONFIG_DIR)/mconf Kconfig
	@$(KCONFIG_DIR)/conf -s --silentoldconfig Kconfig

help:
	@echo make [OPTIONS]
	@echo '  xxx_defconfig   - Create autoconf.h .config according to Kconfig'
	@echo '  menuconfig      - Configuration interface based on text menu'
	@echo 'Execute "make" or "make all" to build all targets marked with [*] '
	@echo 'For further info see the README.md file'
