################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
mpu6050/%.obj: ../mpu6050/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: MSP430 Compiler'
	"D:/ti/ccs1271/ccs/tools/compiler/ti-cgt-msp430_21.6.1.LTS/bin/cl430" -vmspx --data_model=restricted -Ooff --use_hw_mpy=F5 --include_path="E:/Grade3_1/MSP430_SEU/SEU_MSP2024/controller/key" --include_path="E:/Grade3_1/MSP430_SEU/SEU_MSP2024/controller/mpu6050" --include_path="E:/Grade3_1/MSP430_SEU/SEU_MSP2024/controller/oled" --include_path="E:/Grade3_1/MSP430_SEU/SEU_MSP2024/controller/system" --include_path="D:/ti/ccs1271/ccs/ccs_base/msp430/include" --include_path="E:/Grade3_1/MSP430_SEU/SEU_MSP2024/controller" --include_path="D:/ti/ccs1271/ccs/tools/compiler/ti-cgt-msp430_21.6.1.LTS/include" --advice:power=all --define=__MSP430F5529__ -g --printf_support=full --diag_warning=225 --diag_wrap=off --display_error_number --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU23 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="mpu6050/$(basename $(<F)).d_raw" --obj_directory="mpu6050" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


