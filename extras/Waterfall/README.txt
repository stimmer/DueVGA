DueVGA Waterfall
================

This sketch displays a waterfall plot of an audio input. It samples the input 
signal, performs an FFT and displays the result in a scrolling window, with 
brighter pixels representing higher energy bands.

--------------------------------------------------------------------------------
IMPORTANT: To use this sketch you will need to alter the Arduino IDE to link
with the libarm_cortexM3l_math.a library. To do this you need to alter one file:

In the directory containing the IDE, open hardware/arduino/sam/platform.txt
Look for the very long line that says this:

recipe.c.combine.pattern="{compiler.path}{compiler.c.elf.cmd}" {compiler.c.elf.flags} -mcpu={build.mcu} "-T{build.variant.path}/{build.ldscript}" "-Wl,-Map,{build.path}/{build.project_name}.map" -o "{build.path}/{build.project_name}.elf" "-L{build.path}" -lm -lgcc -mthumb -Wl,--cref -Wl,--check-sections -Wl,--gc-sections -Wl,--entry=Reset_Handler -Wl,--unresolved-symbols=report-all -Wl,--warn-common -Wl,--warn-section-align -Wl,--warn-unresolved-symbols -Wl,--start-group "{build.path}/syscalls_sam3.c.o" {object_files} "{build.variant.path}/{build.variant_system_lib}" "{build.path}/{archive_file}" -Wl,--end-group

Just after "{build.variant.path}/{build.variant_system_lib}", you need to add "{build.variant.path}/libarm_cortexM3l_math.a"
So the line will now say this:

recipe.c.combine.pattern="{compiler.path}{compiler.c.elf.cmd}" {compiler.c.elf.flags} -mcpu={build.mcu} "-T{build.variant.path}/{build.ldscript}" "-Wl,-Map,{build.path}/{build.project_name}.map" -o "{build.path}/{build.project_name}.elf" "-L{build.path}" -lm -lgcc -mthumb -Wl,--cref -Wl,--check-sections -Wl,--gc-sections -Wl,--entry=Reset_Handler -Wl,--unresolved-symbols=report-all -Wl,--warn-common -Wl,--warn-section-align -Wl,--warn-unresolved-symbols -Wl,--start-group "{build.path}/syscalls_sam3.c.o" {object_files} "{build.variant.path}/{build.variant_system_lib}" "{build.variant.path}/libarm_cortexM3l_math.a" "{build.path}/{archive_file}" -Wl,--end-group

--------------------------------------------------------------------------------

Compile and upload the sketch in the 'waterfall' subfolder.

The input is an audio signal on pin A0. Something like the headphone output from
your computer will work fine. For protection put a 10uF capacitor between the
headphone output signal and the A0 input. Start with a low volume and increase
slowly.