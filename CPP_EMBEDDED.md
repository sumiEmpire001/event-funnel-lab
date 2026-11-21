# Embedded C++ companion

The existing Python implementation remains unchanged. This C++11 fixed-capacity algorithm is a distinct, deliberately limited embedded companion.

Bounded telemetry stage counters for fixed user IDs and monotonically ordered ticks. Does not parse CSV, perform timezone conversion, handle identity stitching or tied-event sorting; unlike Python, input must already be ordered per user.

## Host verification

```sh
cmake -S cpp -B build
cmake --build build
ctest --test-dir build --output-on-failure
python -m unittest discover -s tests -v
```

The C++ source includes host assertions when built with EMBEDDED_TEST. GitHub Actions runs GCC and Clang host builds; existing Python CI remains separate. **No AVR/ESP32 board build or physical device test is claimed.**

## Board integration and safety

For Arduino Uno (AVR, 2 KB SRAM), choose small template capacities and measure memory use; for ESP32, select capacities according to available heap and task stack. Compile the core as C++11 in the board toolchain after separating the optional test main. Do not allocate large instances on a small stack. All input must be bounded and validated. A latched fault requires an explicit reset. This is not an actuator driver; outputs must never directly energize pumps, relays, motors or other equipment. Add independent interlocks, watchdog, emergency stop, brownout handling and sensor fault detection before physical deployment.
