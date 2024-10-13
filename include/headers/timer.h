#pragma once

#include <cassert>

#include <SDL2/SDL_timer.h>

enum class TimeDuration : uint64_t {
    Unknown = 0,
    Second = 1,
    Decisecond = 10,
    Centisecond = 100,
    Millisecond = 1000,
    Microsecond = 1000000,
    Nanosecond = 1000000000,
    Picosecond = 1000000000000,
    Femtosecond = 1000000000000000,
};

template <typename T = float, auto FixedStepHz = 60, auto Duration = TimeDuration::Second>
    requires std::same_as<decltype(Duration), TimeDuration>
struct Timer {
    static TimeDuration unit() {
        constexpr static uint64_t pico{ std::to_underlying(TimeDuration::Picosecond) };
        constexpr static uint64_t nano{ std::to_underlying(TimeDuration::Nanosecond) };
        constexpr static uint64_t micr{ std::to_underlying(TimeDuration::Microsecond) };
        constexpr static uint64_t mili{ std::to_underlying(TimeDuration::Millisecond) };

        const uint64_t freq{ SDL_GetPerformanceFrequency() };

        if (pico < freq)
            return TimeDuration::Picosecond;
        if (nano < freq)
            return TimeDuration::Nanosecond;
        if (micr < freq)
            return TimeDuration::Microsecond;
        if (mili < freq)
            return TimeDuration::Millisecond;

        assert(!("unknow perf timer tick frequency"));
        return TimeDuration::Unknown;
    }

public:
    // data type that will be output
    using time_type = T;
    // the unti that will be output by the timer
    constexpr static TimeDuration time_unit{ Duration };
    // the timer internally always stores the
    // highest resolution of time point units
    static inline const TimeDuration tick_unit{ Timer::unit() };

public:
    [[nodiscard]]
    constexpr T convert(const uint64_t timestamp_duration) {
        constexpr uint64_t to_ratio{ std::to_underlying(time_unit) };
        const float seconds{ static_cast<float>(timestamp_duration) / m_perf_counter_freq };
        return static_cast<T>(seconds * to_ratio);
    }

    [[nodiscard]]
    static uint64_t timer_freq() {
        // get tick frequency
        return SDL_GetPerformanceFrequency();
    }

    [[nodiscard]]
    static uint64_t now() {
        // get current performance counter tick
        return SDL_GetPerformanceCounter();
    }

    [[nodiscard]]
    constexpr time_type delta() {
        const uint64_t curr_timestamp{ Timer::now() };
        const uint64_t prev_timestamp{ m_delta_timestamp };

        m_delta_timestamp = curr_timestamp;
        return this->convert(curr_timestamp - prev_timestamp);
    }

    [[nodiscard]]
    constexpr time_type elapsed() {
        const uint64_t curr_timestamp{ Timer::now() };
        const uint64_t init_timestamp{ m_start_timestamp };
        return this->convert(curr_timestamp - init_timestamp);
    }

    [[nodiscard]]
    constexpr uint64_t tick_count() const {
        return m_tick_count;
    }

    void reset() {
        m_start_timestamp = Timer::now();
        m_tick_count = 0;
        m_tick_delta_time = 0;
    }

    template <std::invocable TCallable>
    void tick(TCallable&& callable) {
        // query the current time.
        m_elapsed_time = this->elapsed();

        // update the tick() specific delta time
        m_tick_delta_time = m_elapsed_time - m_prev_tick_timepoint;

        // capture the current elapsed time and store it as the
        // timepoint of the previous tick() call so subsequent
        // calls to this function can use it for the delta time
        // calculation directly above, which is then used below
        m_prev_tick_timepoint = m_elapsed_time;

        // update the timer that tracks the time in between
        m_fps_cur_timer += m_tick_delta_time;

        // capture the last frame count value before it's updated below
        const uint64_t last_tick_count{ m_tick_count };

        // if a fixed timestep value was set from the 2nd template parameter
        // then handle the nevessary time tracking / delay required to hold
        // a steady fixed time update delta.
        if constexpr (m_fixed_timestep > 0) {
            if (std::abs(m_tick_delta_time - m_fixed_timestep) < 1.0f / 4000)
                m_tick_delta_time = m_fixed_timestep;

            m_fixed_timestep_leftover_duration += m_tick_delta_time;
            while (m_fixed_timestep_leftover_duration >= m_fixed_timestep) {
                m_elapsed_time = m_fixed_timestep;
                m_fixed_timestep_leftover_duration -= m_fixed_timestep;
                std::invoke(std::forward<TCallable>(callable));
                ++m_tick_count;
            }
        }
        else {
            // variable timestep update logic
            m_elapsed_time = m_tick_delta_time;
            m_fixed_timestep_leftover_duration = 0;
            std::invoke(std::forward<TCallable>(callable));
            ++m_tick_count;
        }

        // track the current framerate
        if (m_tick_count != last_tick_count)
            ++m_fps_cur_count;

        if (m_fps_cur_timer >= 1.0f) {
            m_fps_avg_count = m_fps_cur_count;
            m_fps_cur_count = 0;
            m_fps_cur_timer -= 1.0f;
        }
    }

public:
    uint64_t m_tick_count{ 0 };
    uint64_t m_start_timestamp{ Timer::now() };
    uint64_t m_delta_timestamp{ Timer::now() };

    double m_elapsed_time{ 0.0 };                      // total time elapsed since timer was started, or since reset() was called
    double m_tick_delta_time{ 0.0 };                   // how long it's been since tick() was last called
    double m_prev_tick_timepoint{ 0.0 };               // the point in time when tick() was last called
    double m_fixed_timestep_leftover_duration{ 0.0 };  // keeps track of any duration that flows over the fixed timestep delta
    double m_fps_avg_count{ 0.0 };
    double m_fps_cur_count{ 0.0 };
    double m_fps_cur_timer{ 0.0 };

    constexpr static double m_fixed_timestep{ FixedStepHz > 0 ? 1.0f / FixedStepHz : -1.0f };
    constexpr static TimeDuration m_duration_unit{ Duration };
    const uint64_t m_perf_counter_freq{ SDL_GetPerformanceFrequency() };
};
