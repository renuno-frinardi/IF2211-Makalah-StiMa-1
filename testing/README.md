# Testing Dataset

This folder contains directory trees used for BFS/DFS traversal experiments.

## Scenarios

- **wide_shallow/**: 50 direct subdirectories, target at `dir_050/target.txt`. Designed to favor BFS for shallow targets.
- **deep_narrow/**: Single chain of 30 nested directories, target at the deepest level. Designed to show DFS competitiveness on deep structures.
- **mixed_project/**: Realistic Linux project layout with `src/`, `include/`, `docs/`, `tests/`, `build/`, `assets/`. Target at `src/traversal/deep/target.txt`.
- **no_target/**: Multi-level directory tree with many files but no `target.txt`. Tests worst-case full traversal.

## Generation

These trees are generated deterministically by `scripts/generate_testing_tree.sh`. Running it again will delete and recreate them identically.
