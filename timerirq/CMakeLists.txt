add_executable(timerirq)

target_sources(timerirq PRIVATE timerirq.c)

# Pull in our pico_stdlib which pulls in commonly used features
target_link_libraries(timerirq pico_stdlib)

# create map/bin/hex file etc.
pico_add_extra_outputs(timerirq)

