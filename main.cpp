#include <iostream>
#include <vector>
#include <string>
#include <string_view>
#include <unordered_map>
#include <algorithm>
#include <numeric>
#include <optional>
#include <iomanip>
#include <ranges>
#include <cmath>

struct Product {
    int id;
    int category_id;
    std::string name;
    double price;
    int stock;
};

struct Category {
    int id;
    std::string name;
};

class Catalog {
    std::vector<Product> products_;
    std::vector<Category> categories_;
    // cat_map[c] = {i : products_[i].category_id = c}
    std::unordered_map<int, std::vector<int>> cat_map_;
    std::vector<int> price_sorted_idx_;
    bool price_sorted_ = false;

public:
    void add_category(Category&& c) {
        categories_.push_back(std::move(c));
    }

    void add_category(const Category& c) {
        categories_.push_back(c);
    }

    void add_product(Product&& p) {
        const int idx = static_cast<int>(products_.size());
        cat_map_[p.category_id].push_back(idx);
        products_.push_back(std::move(p));
        price_sorted_ = false;
    }

    void add_product(const Product& p) {
        auto copy = p;
        add_product(std::move(copy));
    }

    // O(n log n)
    void build_price_index() {
        const int n = static_cast<int>(products_.size());
        price_sorted_idx_.resize(n);
        std::iota(price_sorted_idx_.begin(), price_sorted_idx_.end(), 0);
        std::ranges::sort(price_sorted_idx_,
            [&](int a, int b) { return products_[a].price < products_[b].price; });
        price_sorted_ = true;
    }

    // {p in P : p.category_id = cat_id}
    [[nodiscard]] auto by_category(int cat_id) const -> std::vector<Product> {
        std::vector<Product> result;
        const auto it = cat_map_.find(cat_id);
        if (it == cat_map_.end()) return result;
        result.reserve(it->second.size());
        for (const int idx : it->second)
            result.push_back(products_[idx]);
        return result;
    }

    // find by id, nullopt if not found
    [[nodiscard]] auto find_product(int id) const -> std::optional<Product> {
        for (const auto& p : products_) {
            if (p.id == id) return p;
        }
        return std::nullopt;
    }

    // range query [lo, hi]: lower_bound O(log n)
    [[nodiscard]] auto price_range(double lo, double hi) -> std::vector<Product> {
        if (!price_sorted_) build_price_index();
        std::vector<Product> result;
        const auto lb = std::ranges::lower_bound(price_sorted_idx_, lo,
            {}, [&](int idx) { return products_[idx].price; });
        const auto ub = std::ranges::upper_bound(price_sorted_idx_, hi,
            {}, [&](int idx) { return products_[idx].price; });
        for (auto it = lb; it != ub; ++it)
            result.push_back(products_[*it]);
        return result;
    }

    struct Stats {
        double min_price, max_price, mean_price, median_price;
        int total_products, total_stock;
    };

    // min, max, mean, median: O(n log n)
    [[nodiscard]] auto compute_stats() const -> Stats {
        Stats s{};
        const int n = static_cast<int>(products_.size());
        if (n == 0) return s;
        s.total_products = n;
        s.min_price = products_[0].price;
        s.max_price = products_[0].price;
        double sum = 0;
        s.total_stock = 0;
        std::vector<double> prices(n);
        for (int i = 0; i < n; ++i) {
            prices[i] = products_[i].price;
            sum += products_[i].price;
            s.min_price = std::min(s.min_price, products_[i].price);
            s.max_price = std::max(s.max_price, products_[i].price);
            s.total_stock += products_[i].stock;
        }
        s.mean_price = sum / n;
        // median: O(n log n)
        std::ranges::sort(prices);
        if (n % 2 == 1)
            s.median_price = prices[n / 2];
        else
            s.median_price = (prices[n / 2 - 1] + prices[n / 2]) / 2.0;
        return s;
    }

    // {p in P : p.stock < threshold}
    [[nodiscard]] auto low_stock(int threshold) const -> std::vector<Product> {
        std::vector<Product> result;
        for (const auto& p : products_)
            if (p.stock < threshold)
                result.push_back(p);
        return result;
    }

    [[nodiscard]] auto category_name(int cat_id) const -> std::string_view {
        for (const auto& c : categories_)
            if (c.id == cat_id) return c.name;
        return "Unknown";
    }

    [[nodiscard]] auto size() const -> int {
        return static_cast<int>(products_.size());
    }
};

void print_product(const Product& p) {
    std::cout << "  [" << p.id << "] " << p.name
              << " | cat:" << p.category_id
              << " | $" << std::fixed << std::setprecision(2) << p.price
              << " | stock:" << p.stock << "\n";
}

int main() {
    Catalog catalog;

    catalog.add_category({1, "Electronics"});
    catalog.add_category({2, "Clothing"});
    catalog.add_category({3, "Books"});
    catalog.add_category({4, "Home & Garden"});

    // |P| = 16
    catalog.add_product({1,  1, "Laptop Pro 15",       1299.99, 25});
    catalog.add_product({2,  1, "Wireless Mouse",         29.99, 150});
    catalog.add_product({3,  1, "USB-C Hub",              49.99, 80});
    catalog.add_product({4,  1, "Mechanical Keyboard",   129.99, 45});
    catalog.add_product({5,  2, "Cotton T-Shirt",         19.99, 200});
    catalog.add_product({6,  2, "Denim Jeans",            59.99, 75});
    catalog.add_product({7,  2, "Winter Jacket",         149.99, 3});
    catalog.add_product({8,  2, "Running Shoes",          89.99, 60});
    catalog.add_product({9,  3, "C++ Primer",             49.99, 30});
    catalog.add_product({10, 3, "CLRS Algorithms",        79.99, 20});
    catalog.add_product({11, 3, "Design Patterns",        44.99, 15});
    catalog.add_product({12, 3, "Clean Code",             34.99, 2});
    catalog.add_product({13, 4, "Garden Hose 50ft",       39.99, 40});
    catalog.add_product({14, 4, "LED Desk Lamp",          24.99, 100});
    catalog.add_product({15, 4, "Ceramic Plant Pot",      12.99, 0});
    catalog.add_product({16, 4, "Tool Set 120pc",        199.99, 10});

    std::cout << "=== Full Catalog (" << catalog.size() << " products) ===\n";

    std::cout << "\n--- Electronics (category 1) ---\n";
    for (const auto& p : catalog.by_category(1)) print_product(p);

    std::cout << "\n--- Books (category 3) ---\n";
    for (const auto& p : catalog.by_category(3)) print_product(p);

    // find_product(9)
    if (const auto found = catalog.find_product(9)) {
        std::cout << "\nfind_product(9):\n";
        print_product(*found);
    }

    // range query [lo, hi]: lower_bound O(log n)
    std::cout << "\n--- Price range [$25.00, $60.00] ---\n";
    for (const auto& p : catalog.price_range(25.0, 60.0)) print_product(p);

    std::cout << "\n--- Catalog Statistics ---\n";
    const auto stats = catalog.compute_stats();
    std::cout << "  Total products: " << stats.total_products << "\n";
    std::cout << "  Total stock:    " << stats.total_stock << "\n";
    std::cout << "  Min price:      $" << std::fixed << std::setprecision(2) << stats.min_price << "\n";
    std::cout << "  Max price:      $" << stats.max_price << "\n";
    std::cout << "  Mean price:     $" << stats.mean_price << "\n";
    std::cout << "  Median price:   $" << stats.median_price << "\n";

    // {p in P : p.stock < threshold}
    constexpr int threshold = 5;
    std::cout << "\n--- Low Stock Alert (stock < " << threshold << ") ---\n";
    for (const auto& p : catalog.low_stock(threshold)) print_product(p);

    return 0;
}
