#include <iostream>
#include <cstdlib>
#include <memory>
#include <array>
#include <algorithm>
#include <cassert>

struct Day {
    Day(uint8_t date, uint8_t value) : date(date), value(value) {}
    uint8_t date  = 0;
    uint8_t value = 0;
};

struct TotalProfit {
    TotalProfit() : day_max(0, 0), day_min(0, 255), profit(0) {}
    Day day_max;
    Day day_min;
    uint8_t profit;
};

struct History {
    static const uint8_t count = 30;
    uint8_t days[count];

    History() {
        for (uint8_t i = 0; i < count; i++) {
            days[i] = rand() % 255;
        }
    }

    ~History() {
        std::cout << "destr" << "\n";
    }

    History(std::array<uint8_t, count>&& values) {
        std::move(values.begin(), values.end(), days);
    }

    void show() const {
        for (auto i = 0; i < count; i++)
            std::cout << "d: " << (int)(i + 1) << " v: " << (int)days[i] << "\n";
    }
};

class Algo {
public:
    Algo() : history(std::make_unique<History>()){}

    void findMaxProfit();
    void setHistory(std::unique_ptr<History>&& h) {
        history = std::move(h);
    }
    
    void assertDay(uint8_t d_min, uint8_t d_max){
        assert(profit.day_max.date == d_max);
        assert(profit.day_min.date == d_min);
    }

private:
    std::unique_ptr<History> history;
    TotalProfit profit;
};

void Algo::findMaxProfit() {
    history->show();

    Day day_min(0, 255);
    Day day_max(0, 0);
    profit.profit = 0;

    for (uint8_t i = 0; i < history->count; i++) {
        if (history->days[i] < day_min.value) {
            day_min.value = history->days[i];
            day_min.date = i + 1;
            day_max.value = 0;
            day_max.date = 0;
        }

        if (history->days[i] > day_max.value) {
            day_max.value = history->days[i];
            day_max.date = i + 1;
            uint8_t delta = day_max.value - day_min.value;
            if (delta > profit.profit) {
                profit.day_max = day_max;
                profit.day_min = day_min;
                profit.profit = delta;
            }
        }
    }

    std::cout << "Profit: " << (int)profit.profit 
              << ", Day Min: " << (int)profit.day_min.date
              << ", Day Max: " << (int)profit.day_max.date << "\n"; 

}

int main() {
    auto h1 = std::make_unique<History>();
    auto h2 = std::make_unique<History>();
    auto h3 = std::make_unique<History>();
    auto algo = std::make_unique<Algo>();

    std::cout << "History 1:\n";
    algo->setHistory(std::move(h1));
    algo->findMaxProfit();

    std::cout << "History 2:\n";
    algo->setHistory(std::move(h2));
    algo->findMaxProfit();

    std::cout << "History 3:\n";
    algo->setHistory(std::move(h3));
    algo->findMaxProfit();

    std::array<uint8_t, History::count> values = {
        163, 151, 1, 85, 83, 190, 241, 252, 249, 121,
        107, 2, 20, 19, 233, 226, 45, 25, 142, 31,
        86, 8, 87, 39, 167, 5, 212, 208, 1, 255
    };

    auto custom_history = std::make_unique<History>(std::move(values));
    algo->setHistory(std::move(custom_history));
    algo->findMaxProfit();
    algo->assertDay(3, 30);

    return 0;
}
