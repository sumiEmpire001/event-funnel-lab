# Event Funnel Lab

A Python standard-library CLI for measuring ordered event funnels. Groups timestamped events by user, requires each funnel stage in chronological order, and reports unique users reaching each stage and conversion rates.

## Run

```bash
python funnel.py examples/events.csv --steps view signup purchase
```

Input CSV columns: `user_id,event,timestamp`, with timezone-aware ISO 8601 timestamps. Stage names are provided after `--steps` in required order. Each user is counted once per stage.

## Tests

```bash
python -m unittest discover -s tests -v
```

Eight tests run on Python 3.10, 3.12 and 3.13 through GitHub Actions.

## Limitations

This tool does not handle session boundaries, attribution windows, bot filtering, identity stitching, or statistical significance. Tied timestamps are ordered by event name. Use results as exploratory funnel metrics rather than a production analytics source.

## C++11 companion implementation

A separate portable C++ algorithm is available in [cpp/core.hpp](cpp/core.hpp), with [host tests](cpp/tests.cpp), [CMake](CMakeLists.txt) and [GitHub Actions](.github/workflows/cpp-core.yml). The Python implementation and its existing tests remain unchanged. See [C++ limitations and safety notes](cpp/README.md) for differences, embedded memory constraints and unverified Arduino/ESP32 hardware integration.
