#pragma once

#include <iostream>
#include <string>
#include <chrono>
#include <cstdio>
#include <cstdint>

class ProgressBar
{
public:
    ProgressBar(const std::string& label, uint64_t totalEntries, int barWidth = 40)
        : label_(label), total_(totalEntries), current_(0), barWidth_(barWidth),
          lastPrintedPct_(-1), currentDepth_(-1), maxDepth_(-1)
    {
        startTime_ = std::chrono::steady_clock::now();
        lastPrintTime_ = startTime_;
        print();
    }

    void update(uint64_t currentCount)
    {
        current_ = currentCount;
        int pct10 = static_cast<int>(getPercent() * 10);
        auto now = std::chrono::steady_clock::now();
        double sinceLast = std::chrono::duration<double>(now - lastPrintTime_).count();
        // Redraw when percentage changes OR at least every second (keeps elapsed time alive)
        if (pct10 != lastPrintedPct_ || sinceLast >= 1.0)
        {
            lastPrintedPct_ = pct10;
            lastPrintTime_ = now;
            print();
        }
    }

    void finish()
    {
        current_ = total_;
        lastPrintedPct_ = 1000;
        print();
        std::cout << std::endl;
    }

    void setDepth(int current, int max)
    {
        currentDepth_ = current;
        maxDepth_ = max;
    }

private:
    std::string label_;
    uint64_t total_;
    uint64_t current_;
    int barWidth_;
    int lastPrintedPct_;
    int currentDepth_;
    int maxDepth_;
    std::chrono::steady_clock::time_point startTime_;
    std::chrono::steady_clock::time_point lastPrintTime_;

    double getPercent() const
    {
        if (total_ == 0) return 100.0;
        return (static_cast<double>(current_) / total_) * 100.0;
    }

    static std::string formatTime(double seconds)
    {
        if (seconds < 0) return "--:--:--";
        int h = static_cast<int>(seconds) / 3600;
        int m = (static_cast<int>(seconds) % 3600) / 60;
        int s = static_cast<int>(seconds) % 60;
        char buf[16];
        std::snprintf(buf, sizeof(buf), "%02d:%02d:%02d", h, m, s);
        return std::string(buf);
    }

    static std::string formatCount(uint64_t count)
    {
        if (count >= 1000000000ULL)
        {
            return std::to_string(count / 1000000) + "M";
        }
        else if (count >= 1000000ULL)
        {
            double millions = count / 1000000.0;
            char buf[16];
            std::snprintf(buf, sizeof(buf), "%.1fM", millions);
            return std::string(buf);
        }
        else if (count >= 1000ULL)
        {
            double thousands = count / 1000.0;
            char buf[16];
            std::snprintf(buf, sizeof(buf), "%.1fK", thousands);
            return std::string(buf);
        }
        return std::to_string(count);
    }

    void print() const
    {
        double pct = getPercent();
        int filled = static_cast<int>((pct / 100.0) * barWidth_);
        if (filled > barWidth_) filled = barWidth_;

        auto now = std::chrono::steady_clock::now();
        double elapsed = std::chrono::duration<double>(now - startTime_).count();

        // Build the bar: [########------------]
        std::string bar(barWidth_, ' ');
        for (int i = 0; i < filled; i++) bar[i] = '#';
        for (int i = filled; i < barWidth_; i++) bar[i] = '-';

        if (currentDepth_ >= 0)
        {
            // Depth mode: show depth instead of ETA (for DFS-based generation
            // where linear ETA would be misleading)
            std::printf("\r  %s [%s] %5.1f%%  %s/%s  elapsed %s  depth %d/%d   ",
                label_.c_str(),
                bar.c_str(),
                pct,
                formatCount(current_).c_str(),
                formatCount(total_).c_str(),
                formatTime(elapsed).c_str(),
                currentDepth_, maxDepth_);
        }
        else
        {
            // Standard mode: show linear ETA (suitable for BFS-based generation)
            double eta = -1;
            if (pct > 0.01 && pct < 100.0)
            {
                eta = (elapsed / pct) * (100.0 - pct);
            }
            std::printf("\r  %s [%s] %5.1f%%  %s/%s  elapsed %s  eta %s   ",
                label_.c_str(),
                bar.c_str(),
                pct,
                formatCount(current_).c_str(),
                formatCount(total_).c_str(),
                formatTime(elapsed).c_str(),
                formatTime(eta).c_str());
        }
        std::fflush(stdout);
    }
};
