################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
user_code/src/%.obj: ../user_code/src/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccs1240/ccs/tools/compiler/ti-cgt-msp430_21.6.1.LTS/bin/cl430" -vmsp --opt_for_speed=3 --use_hw_mpy=none --include_path="C:/ti/ccs1240/ccs/ccs_base/msp430/include" --include_path="C:/Users/bjoer/Desktop/Laile Projekt/Versuch_1" --include_path="C:/Users/bjoer/Desktop/Laile Projekt/Versuch_1/user_code/inc" --include_path="C:/ti/ccs1240/ccs/tools/compiler/ti-cgt-msp430_21.6.1.LTS/include" --advice:power=all --define=__MSP430G2553__ -g --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="user_code/src/$(basename $(<F)).d_raw" --obj_directory="user_code/src" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


