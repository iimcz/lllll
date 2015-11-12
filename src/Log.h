/*
 * Log.h
 *
 *  Created on: 10. 10. 2015
 *      Author: neneko
 */

#ifndef CXX_SRC_LOG_H_
#define CXX_SRC_LOG_H_

#include <mutex>
#include <sstream>
#include <map>
#include <memory>
namespace iim {

enum class log_level {
    verbose,
    debug,
    info,
    warning,
    error,
    fatal
};

struct StreamGuard {
    StreamGuard(std::ostream& target)
        : target(target)
    {
    }
    ~StreamGuard() noexcept = default;
    StreamGuard(const StreamGuard&) = delete;
    StreamGuard(StreamGuard&&) = default;
    StreamGuard& operator=(const StreamGuard&) = delete;
    StreamGuard& operator=(StreamGuard&&) = default;
    std::ostream& target;
    std::mutex mutex;
    template <class T>
    void write(const T& msg)
    {
        std::unique_lock<std::mutex> _(mutex);
        target << msg;
    }
};

template <class Char, class Traits, class Allocator>
class BasicLogProxy {
public:
    BasicLogProxy(StreamGuard& str, bool quiet)
        : str_(str)
        , quiet_(quiet)
    {
    }
    ~BasicLogProxy() noexcept
    {
        if (!quiet_) {
            sstr_ << sstr_.widen('\n');
            str_.write(sstr_.str());
        }
    }
    BasicLogProxy(const BasicLogProxy&) = delete;
    BasicLogProxy(BasicLogProxy&& rhs)
        : str_(rhs.str_)
        , quiet_(rhs.quiet_)

    {
        if (!quiet_)
            sstr_ << rhs.sstr_.str();
        rhs.quiet_ = true;
    }

    BasicLogProxy& operator=(const BasicLogProxy&) = delete;
    BasicLogProxy& operator=(BasicLogProxy&& rhs)
    {
        quiet_ = rhs.quiet_;
        str_ = rhs.str_;
        rhs.quiet_ = true;
        if (!quiet_)
            sstr_ << rhs.sstr_.str();
        return *this;
    }
    template <class T>
    BasicLogProxy& operator<<(const T& msg)
    {
        if (!quiet_) {
            sstr_ << msg;
        }
        return *this;
    }

private:
    StreamGuard& str_;
    bool quiet_;
    std::stringstream sstr_;
};

template <class Char, class Traits = std::char_traits<Char>, class Allocator = std::allocator<Char> >
class BasicLog {
    using string_type = std::basic_string<Char, Traits, Allocator>;

public:
    BasicLog(std::ostream& target)
        : target_(std::make_shared<StreamGuard>(target))
        , level_(log_level::info)
    {
    }

    void set_level(log_level level)
    {
        level_ = level;
    }

    void set_name(const string_type& name)
    {
    	id_ = "[" + name + "] ";
    }
    BasicLogProxy<Char, Traits, Allocator> operator[](log_level lev)
    {
        static const std::map<log_level, string_type> level_names = {
            { log_level::verbose, "[VERBOSE] " },
            { log_level::debug, "[DEBUG] " },
            { log_level::info, "[INFO] " },
            { log_level::warning, "[WARNING] " },
            { log_level::error, "[ERROR] " },
            { log_level::fatal, "[FATAL] " },
        };

        bool quiet = lev < level_;
        BasicLogProxy<Char, Traits, Allocator> proxy{ *target_, quiet };
        if (!quiet) {
        	if (!id_.empty()) proxy << id_;
            proxy << level_names.at(lev);
        }
        return std::move(proxy);
    }

private:
    std::shared_ptr<StreamGuard> target_;
    log_level level_;
    string_type id_;
};

using Log = BasicLog<char>;
}

#endif /* CXX_SRC_LOG_H_ */
