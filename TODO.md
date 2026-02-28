# State Machine Simulator - Development TODO

## Week 01 (Feb 01 - Feb 07 2026): COMPLETE

- [x] Clone ImGui, ImPlot, install GLFW
- [x] Create basic CMakeLists.txt
- [x] Get the ImGui example_glfw_opengl3 compiling and running
- [x] Add one slider that controls a float variable
- [x] Display that float variable as text
- [x] BONUS: Built complete state machine core logic
- [x] BONUS: Integrated GUI with state machine
- [x] BONUS: Added state history timeline
- [x] BONUS: Implemented all rocket flight transitions

---

## Week 02 (Feb 08 - Feb 14 2026): Code Organization & Plotting

**Goal:** Refactor codebase and add real-time telemetry plots

### Code Refactoring

- [ ] Split main.cpp into separate files:
  - [ ] Create gui/control_panel.h and gui/control_panel.cpp (sensor sliders)
  - [ ] Create gui/state_display.h and gui/state_display.cpp (state machine panel)
  - [ ] Create gui/timeline.h and gui/timeline.cpp (history display)
  - [ ] Keep main.cpp minimal (just initialization and main loop)
- [ ] Update CMakeLists.txt to include new GUI source files
- [ ] Test that everything still compiles and runs

### ImPlot Integration

- [ ] Add ImPlot to external dependencies
- [ ] Update CMakeLists.txt to compile ImPlot
- [ ] Create gui/telemetry_plots.h and gui/telemetry_plots.cpp
- [ ] Add data history storage (circular buffers for sensor values over time)
- [ ] Create altitude plot (real-time graph)
- [ ] Create velocity plot
- [ ] Create acceleration plot
- [ ] Add plot panel to main layout (maybe bottom section or right side)

### Polish

- [ ] Fix sensor slider ranges (altitude 0-15000, velocity -500 to 500, accel -50 to 300, battery 0-100)
- [ ] Add units to slider labels (ft, m/s, m/s^2, %)
- [ ] Color-code current state display (BOOST=red, COAST=yellow, APOGEE=green, etc.)

---

## Week 03 (Feb 15 - Feb 21 2026): Advanced Features (Beta)

- [ ] Launch Profile automation (button to run realistic flight sequence)
- [ ] Playback controls (pause/play/speed adjustment)
- [ ] Save/Load sensor profiles to JSON
- [ ] Export state history to CSV
- [ ] Add transition condition tooltips (hover to see "needs accel_z > 1000")

---

## Current Status

Ahead of schedule! Week 1 complete with bonus features. Ready to organize code and add plotting for Week 2.
