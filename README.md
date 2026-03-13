# Homework 22.2 — E-Commerce Catalog

C++ implementation of an e-commerce product catalog with efficient search and filtering, inspired by the Django Product + Category models.

## Data Structures & Complexity

| Operation | Time | Method |
|---|---|---|
| Category lookup | O(1) amortized | unordered_map -> vector of indices |
| Price range query | O(log n + k) | sort + lower_bound + upper_bound |
| Statistics (min/max/mean) | O(n) | single pass |
| Median | O(n log n) | sort prices |
| Low stock filter | O(n) | linear scan |

## Build & Run

```bash
g++ -std=c++17 -O2 -o homework22 main.cpp
./homework22
```

## Description

- Product and Category structs mirroring Django ORM models
- Hash map index for O(1) category-based product lookup
- Binary search (lower_bound/upper_bound) for price range queries on sorted index
- Full catalog statistics: min, max, mean, median price and total inventory
- Inventory alert system for low-stock products
