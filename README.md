# Analisis Perbandingan Algoritma BFS dan DFS untuk Traversal Pencarian File pada Filesystem Linux

## Deskripsi

Repository ini berisi eksperimen untuk membandingkan performa algoritma **Breadth-First Search (BFS)** dan **Depth-First Search (DFS)** dalam mencari file berdasarkan nama persis pada struktur direktori Linux. Program mengimplementasikan kedua algoritma secara eksplisit menggunakan struktur data queue (BFS) dan stack (DFS), bukan rekursi.

## Struktur Direktori

```
bfs-dfs-filesystem-experiment/
├── Makefile                  # Build system
├── README.md                 # File in
├── include/                  # Header files
│   ├── bfs.h
│   ├── dfs.h
│   ├── fs_utils.h
│   ├── metrics.h
│   ├── output_writer.h
│   ├── path_queue.h
│   └── path_stack.h
├── src/                      # Source implementation
│   ├── main.c                # CLI entry point
│   ├── bfs.c                 # BFS traversal
│   ├── dfs.c                 # DFS traversal
│   ├── fs_utils.c            # Filesystem utilities
│   ├── metrics.c             # Metrics struct
│   ├── output_writer.c       # CSV output
│   ├── path_queue.c          # Dynamic queue for BFS
│   └── path_stack.c          # Dynamic stack for DFS
├── scripts/                  # Helper scripts
│   ├── generate_testing_tree.sh  # Generate test datasets
│   └── run_all_experiments.sh    # Run all experiments
├── testing/                  # Test directory trees
│   ├── README.md
│   ├── wide_shallow/         # Wide and shallow structure
│   ├── deep_narrow/          # Deep and narrow structure
│   ├── mixed_project/        # Realistic project structure
│   └── no_target/            # No target file present
├── results/                  # Experiment results (CSV)
│   └── .gitkeep
└── bin/                      # Compiled binary
    └── .gitkeep
```

## Dependensi

- **Linux** (diuji pada ArchLinux)
- **GCC** (GNU C Compiler)
- **GNU Make**
- **Bash**

## Cara Build

```bash
make clean && make
```

Binary akan dihasilkan di `bin/findcmp`. Kompilasi menggunakan flag:
- `-Wall -Wextra -Werror -std=c11 -O2 -D_POSIX_C_SOURCE=200809L`

## Cara Membuat Dataset Testing

```bash
make setup
# atau langsung:
bash scripts/generate_testing_tree.sh
```

Script ini akan menghapus isi folder `testing/` lama dan membentuk ulang struktur direktori deterministik.

## Cara Menjalankan

### BFS Saja

```bash
./bin/findcmp --algo bfs --root testing/wide_shallow --target target.txt \
  --out results/bfs_wide.csv
```

### DFS Saja

```bash
./bin/findcmp --algo dfs --root testing/wide_shallow --target target.txt \
  --out results/dfs_wide.csv
```

### Keduanya (BFS + DFS)

```bash
./bin/findcmp --algo both --root testing/wide_shallow --target target.txt \
  --out results/wide.csv
```

### Dengan Pengulangan (10 kali)

```bash
./bin/findcmp --algo both --root testing/wide_shallow --target target.txt \
  --repeat 10 --out results/wide_repeat.csv
```

### Mode Verbose

```bash
./bin/findcmp --algo both --root testing/wide_shallow --target target.txt --verbose
```

### Demo (FAST)

```bash
make run-demo
```

Menjalankan BFS dan DFS pada skenario `wide_shallow` dan `deep_narrow`.

## Cara Menjalankan Seluruh Eksperimen

```bash
make experiments
# atau langsung:
bash scripts/run_all_experiments.sh
```

Script akan:
1. Membuat dataset jika belum ada.
2. Menjalankan `findcmp --algo both --repeat 10` pada keempat skenario.
3. Menyimpan hasil ke `results/*.csv`.
4. Menggabungkan semua CSV ke `results/all_results.csv`.

## Opsi CLI

| Opsi         | Deskripsi                                  | Default                 |
|-------------|---------------------------------------------|-------------------------|
| `--algo`    | Algoritma: `bfs`, `dfs`, `both`            | *(wajib)*               |
| `--root`    | Root direktori pencarian                    | *(wajib)*               |
| `--target`  | Nama file target (exact match)              | *(wajib)*               |
| `--repeat`  | Jumlah pengulangan                          | `1`                     |
| `--out`     | Path output CSV                             | `results/results.csv`   |
| `--verbose` | Cetak log detail ke stderr                  | *(off)*                 |
| `--help`    | Tampilkan bantuan                           |                         |

## Format Output CSV

CSV memiliki satu header dan satu baris per run:

```csv
run_id,algorithm,root_path,target_name,found,found_path,visited_dirs,visited_files,visited_entries,skipped_dirs,error_count,max_frontier_size,max_depth_seen,elapsed_ms
```

| Kolom              | Deskripsi                                                    |
|--------------------|--------------------------------------------------------------|
| `run_id`           | Nomor pengulangan                                            |
| `algorithm`        | BFS atau DFS                                                 |
| `root_path`        | Path root pencarian                                          |
| `target_name`      | Nama file target                                             |
| `found`            | 1 jika ditemukan, 0 jika tidak                               |
| `found_path`       | Path lengkap file ditemukan (atau "not_found")               |
| `visited_dirs`     | Jumlah direktori yang dibuka dan diproses                    |
| `visited_files`    | Jumlah file reguler yang diperiksa namanya                   |
| `visited_entries`  | Jumlah seluruh entri yang diperiksa                          |
| `skipped_dirs`     | Jumlah direktori yang gagal dibuka (permission, dll)         |
| `error_count`      | Jumlah error selama traversal                                |
| `max_frontier_size`| Ukuran maksimum queue (BFS) atau stack (DFS)                 |
| `max_depth_seen`   | Kedalaman maksimum path relatif terhadap root                |
| `elapsed_ms`       | Waktu eksekusi dalam milidetik                               |