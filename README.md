# apollo

## plan

### GUI 
```bash
┌─────────────────┬──────────────────────┐
│ State Machine   │ Sensor Controls      │
│ ┌─────────────┐ │ Altitude: [====│   ] │
│ │ Current:    │ │ Velocity: [══│     ] │
│ │ BOOST       │ │ Accel X:  [   │====] │
│ │             │ │ Accel Y:  [     │==] │
│ └─────────────┘ │ Accel Z:  [====│   ] │
│ Transitions:    │ GPS Lock: [X]        │
│ • BOOST→COAST   │ LoRa:     [X]        │
│   when vel<0    │ Battery:  [====│   ] │
│ • COAST→APOGEE  │                      │
│   when alt max  │ [Inject Fault ▼]     │
├─────────────────┴──────────────────────┤
│ State History Timeline                 │
│ ████BOOST██COAST████APOGEE███DESCENT   │
│ 0s────5s────10s────15s────20s          │
├────────────────────────────────────────┤
│ Real-time Plots                        │
│ [Altitude, Velocity, Acceleration]     │
└────────────────────────────────────────┘
```
