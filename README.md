# Map Logic — Board Game Generator for Robot Simulation

A Qt6 desktop application for visually designing board game maps that are used as simulation environments for LEGO Mindstorm robots. You draw nodes, connect them into a graph, assign tile types, place robots, and export the layout to JSON so the robot simulation can read it.

## What It Does

The app lets you build a graph-based board interactively:

- **Create nodes** by clicking on empty space — each node is a tile on the board
- **Connect nodes** into paths the robot can travel along
- **Assign tile types** to change how a tile behaves in the simulation
- **Place and drag robots** along the paths to simulate movement
- **Export the board** to `~/Desktop/map.json` for use in the Mindstorm robot simulation

Robots are constrained to move only along defined connections — they snap to the nearest path segment when dragged, and the app tracks which node they are currently closest to.

## Node Types

| Type   | Color  | Purpose |
|--------|--------|---------|
| START  | Orange | Starting position for robots |
| NORMAL | Blue   | Standard tile |
| TRAP   | Red    | Trap tile — triggers a penalty |
| EVENT  | Green  | Event tile — triggers a random event |
| SHOP   | Purple | Shop tile — allows item purchase |

## Controls

| Input | Action |
|-------|--------|
| Left click (empty area) | Create a new node |
| Drag node | Move node |
| Drag robot | Move robot along paths |
| `C` | Enter Connect Mode — click two nodes to link them |
| `D` | Enter Delete Mode — click a node to remove it |
| `R` | Spawn a new robot at the start position |
| `S` | Save the current board to `~/Desktop/map.json` |
| `1` | Set selected node to NORMAL |
| `2` | Set selected node to TRAP |
| `3` | Set selected node to EVENT |
| `4` | Set selected node to SHOP |
| `5` | Set selected node to START |

## JSON Export Format

Pressing `S` saves the board to `~/Desktop/map.json`:

```json
{
  "nodes": [
    {
      "id": 0,
      "x": 300,
      "y": 300,
      "type": "START",
      "neighbors": [1, 2]
    },
    {
      "id": 1,
      "x": 500,
      "y": 300,
      "type": "NORMAL",
      "neighbors": [0]
    }
  ]
}
```

The Mindstorm robot simulation reads this file to know the board layout, node positions, tile types, and which nodes are connected.

## Requirements

- Qt 6.5 or later
- CMake 3.19 or later
- C++17 compatible compiler

## Build

```bash
cmake -B build
cmake --build build
```
