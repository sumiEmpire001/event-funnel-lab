# bounded ordered funnel state machine

A portable, dependency-free C++11 component for host testing and adaptation to Arduino/ESP32 firmware. No Arduino core is required for the algorithm. Use the header from your sketch and supply validated readings from an actual sensor or telemetry source.

```sh
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
python -m unittest discover -s tests -v
```

## Differences from Python

C++ is an online bounded per-user state machine, unlike Python's batch sort by timestamp and event name. Out-of-order events are rejected rather than sorted; unmatched events do not advance stages. No attribution/session logic.

## Safety

This is software-only: **no physical boards, sensor accuracy, interrupts, watchdogs, electrical isolation, or actuators have been tested**. Validate all physical readings, set outputs to a safe state on missing/stale data, and use independent hardware interlocks before connecting any relay, pump, or motor. Uno has limited RAM; the STL map/string/vector used here can allocate memory dynamically and is **not suitable for memory-constrained AVR production firmware without a fixed-capacity rewrite**. ESP32 integration is also unverified. Host CI does not compile a board sketch.
