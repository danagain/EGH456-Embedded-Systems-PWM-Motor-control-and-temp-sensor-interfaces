################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
DK_TM4C129X.obj: ../DK_TM4C129X.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"/Applications/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.6.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="/Users/danielhuffer/workspace_v7/i2ctmp006_DK_TM4C129X_TI" --include_path="/Users/danielhuffer/workspace_v7/i2ctmp006_DK_TM4C129X_TI" --include_path="/Users/danielhuffer/ti/tirtos_tivac_2_16_00_08/products/TivaWare_C_Series-2.1.1.71b" --include_path="/Users/danielhuffer/ti/tirtos_tivac_2_16_00_08/products/bios_6_45_01_29/packages/ti/sysbios/posix" --include_path="/Applications/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.6.LTS/include" --define=ccs="ccs" --define=PART_TM4C129XNCZAD --define=ccs --define=TIVAWARE -g --gcc --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi --preproc_with_compile --preproc_dependency="DK_TM4C129X.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

i2ctmp006.obj: ../i2ctmp006.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"/Applications/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.6.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="/Users/danielhuffer/workspace_v7/i2ctmp006_DK_TM4C129X_TI" --include_path="/Users/danielhuffer/workspace_v7/i2ctmp006_DK_TM4C129X_TI" --include_path="/Users/danielhuffer/ti/tirtos_tivac_2_16_00_08/products/TivaWare_C_Series-2.1.1.71b" --include_path="/Users/danielhuffer/ti/tirtos_tivac_2_16_00_08/products/bios_6_45_01_29/packages/ti/sysbios/posix" --include_path="/Applications/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.6.LTS/include" --define=ccs="ccs" --define=PART_TM4C129XNCZAD --define=ccs --define=TIVAWARE -g --gcc --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi --preproc_with_compile --preproc_dependency="i2ctmp006.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

build-1772179361:
	@$(MAKE) --no-print-directory -Onone -f subdir_rules.mk build-1772179361-inproc

build-1772179361-inproc: ../i2ctmp006.cfg
	@echo 'Building file: "$<"'
	@echo 'Invoking: XDCtools'
	"/Users/danielhuffer/ti/xdctools_3_32_00_06_core/xs" --xdcpath="/Users/danielhuffer/ti/tirtos_tivac_2_16_00_08/packages;/Users/danielhuffer/ti/tirtos_tivac_2_16_00_08/products/tidrivers_tivac_2_16_00_08/packages;/Users/danielhuffer/ti/tirtos_tivac_2_16_00_08/products/bios_6_45_01_29/packages;/Users/danielhuffer/ti/tirtos_tivac_2_16_00_08/products/ndk_2_25_00_09/packages;/Users/danielhuffer/ti/tirtos_tivac_2_16_00_08/products/uia_2_00_05_50/packages;/Users/danielhuffer/ti/tirtos_tivac_2_16_00_08/products/ns_1_11_00_10/packages;/Applications/ti/ccsv7/ccs_base;" xdc.tools.configuro -o configPkg -t ti.targets.arm.elf.M4F -p ti.platforms.tiva:TM4C129XNCZAD -r release -c "/Applications/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.6.LTS" --compileOptions "-mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path=\"/Users/danielhuffer/workspace_v7/i2ctmp006_DK_TM4C129X_TI\" --include_path=\"/Users/danielhuffer/workspace_v7/i2ctmp006_DK_TM4C129X_TI\" --include_path=\"/Users/danielhuffer/ti/tirtos_tivac_2_16_00_08/products/TivaWare_C_Series-2.1.1.71b\" --include_path=\"/Users/danielhuffer/ti/tirtos_tivac_2_16_00_08/products/bios_6_45_01_29/packages/ti/sysbios/posix\" --include_path=\"/Applications/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.6.LTS/include\" --define=ccs=\"ccs\" --define=PART_TM4C129XNCZAD --define=ccs --define=TIVAWARE -g --gcc --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi  " "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

configPkg/linker.cmd: build-1772179361 ../i2ctmp006.cfg
configPkg/compiler.opt: build-1772179361
configPkg/: build-1772179361


