################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := C:/Users/Sejbi/Desktop/TI/ccsv7/tools/compiler/ti-cgt-msp430_16.9.4.LTS

GEN_OPTS__FLAG := 
GEN_CMDS__FLAG := 

ORDERED_OBJS += \
"./ball_movement.obj" \
"./gamemenu.obj" \
"./informationboard.obj" \
"./pong.obj" \
"./hal_libraries/hal_UCS.obj" \
"./hal_libraries/hal_lcd.obj" \
"./hal_libraries/hal_lcd_fonts.obj" \
"./hal_libraries/hal_pmm.obj" \
"../lnk_msp430f5438a.cmd" \
$(GEN_CMDS__FLAG) \
-llibmath.a \
-llibc.a \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include hal_libraries/subdir_vars.mk
-include subdir_rules.mk
-include hal_libraries/subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(SV7A_DEPS)),)
-include $(SV7A_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(PDE_DEPS)),)
-include $(PDE_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
Pong_base.out \

EXE_OUTPUTS__QUOTED += \
"Pong_base.out" \

BIN_OUTPUTS += \
Pong_base.hex \

BIN_OUTPUTS__QUOTED += \
"Pong_base.hex" \


# All Target
all: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@$(MAKE) --no-print-directory -Onone "Pong_base.out"

# Tool invocations
Pong_base.out: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@echo 'Building target: $@'
	@echo 'Invoking: MSP430 Linker'
	"C:/Users/Sejbi/Desktop/TI/ccsv7/tools/compiler/ti-cgt-msp430_16.9.4.LTS/bin/cl430" -vmspx --data_model=restricted -O2 --use_hw_mpy=F5 --advice:power=all --define=__MSP430F5438A__ --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU23 --silicon_errata=CPU40 -z -m"Pong_base.map" --heap_size=160 --stack_size=160 --cinit_hold_wdt=on -i"C:/Users/Sejbi/Desktop/TI/ccsv7/ccs_base/msp430/include" -i"C:/Users/Sejbi/Desktop/TI/ccsv7/ccs_base/msp430/lib/5xx_6xx_FRxx" -i"C:/Users/Sejbi/Desktop/TI/ccsv7/tools/compiler/ti-cgt-msp430_16.9.4.LTS/lib" -i"C:/Users/Sejbi/Desktop/TI/ccsv7/tools/compiler/ti-cgt-msp430_16.9.4.LTS/include" --reread_libs --diag_wrap=off --display_error_number --warn_sections --xml_link_info="Pong_base_linkInfo.xml" --use_hw_mpy=F5 --rom_model -o "Pong_base.out" $(ORDERED_OBJS)
	@echo 'Finished building target: $@'
	@echo ' '

Pong_base.hex: $(EXE_OUTPUTS)
	@echo 'Invoking: MSP430 Hex Utility'
	"C:/Users/Sejbi/Desktop/TI/ccsv7/tools/compiler/ti-cgt-msp430_16.9.4.LTS/bin/hex430" --memwidth=8 --romwidth=8 -o "Pong_base.hex" $(EXE_OUTPUTS__QUOTED)
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(BIN_OUTPUTS__QUOTED)$(EXE_OUTPUTS__QUOTED)
	-$(RM) "ball_movement.obj" "gamemenu.obj" "informationboard.obj" "pong.obj" "hal_libraries\hal_UCS.obj" "hal_libraries\hal_lcd.obj" "hal_libraries\hal_lcd_fonts.obj" "hal_libraries\hal_pmm.obj" 
	-$(RM) "ball_movement.d" "gamemenu.d" "informationboard.d" "pong.d" "hal_libraries\hal_UCS.d" "hal_libraries\hal_lcd.d" "hal_libraries\hal_lcd_fonts.d" "hal_libraries\hal_pmm.d" 
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

