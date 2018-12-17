#ifndef PFF_PFF_HPP
#define PFF_PFF_HPP

#include <string>
#include <unordered_map>
#include <vector>
#include <numeric>
#include <memory>
#include <iostream>
#include <iomanip>

namespace PFF {

class context {
friend class format;
template <typename T> friend class _;
private:
    context(std::istream & istream, std::ostream & ostream)
        : istream{istream}
        , ostream{ostream}
    {}

    auto push_namespace(std::string name) {
        namespaces.push_back(name);
    }

    auto pop_namespace() {
        namespaces.pop_back();
    }

    auto print_namespace() const {
        for (auto && name : namespaces) {
            ostream << name << "/";
        }
    }

    std::istream & istream;
    std::ostream & ostream;
    std::size_t indent{};
    std::size_t offset{};
    std::size_t indent_space{4};
    std::vector<std::string> namespaces;
};

struct format {
    virtual ~format() {}
    virtual auto size() const -> std::size_t = 0;
    virtual auto parse(PFF::context & context) const -> void = 0;
    virtual auto parse(
        std::istream & istream = std::cin,
        std::ostream & ostream = std::cout
    ) const -> void {
        PFF::context context{istream, ostream};
        parse(context);
    }
};

template<typename T>
class _ : public format {
public:
    using value_type = T;

    _(std::string name, T) : _name{std::move(name)} {}

    virtual auto size() const -> std::size_t override {
        return sizeof(T);
    }

    virtual auto parse(PFF::context & context) const -> void override {
        value_type value{};
        char * ptr{reinterpret_cast<char *>(& value)};
        context.istream.read(ptr, sizeof(value_type));

        context.ostream << "[0x" << std::hex << std::setw(sizeof(value_type) * 2) << std::setfill('0') << context.offset << " - 0x" << std::setw(sizeof(value_type) * 2) << std::setfill('0') << (context.offset + sizeof(value_type) - 1) << std::dec <<  "]";
        context.offset += sizeof(value_type);

        context.ostream << ' ';
        auto iter = _map.find(value);
        if (iter != _map.end()) {
            context.ostream << iter->second;
        } else {
            context.ostream << "0x" << std::hex << std::setw(sizeof(value_type) * 2) << std::setfill('0') << value << std::dec;
        }
        context.ostream << " ";
        context.print_namespace();
        context.ostream << _name;
        context.ostream << std::endl;
    }

    virtual auto parse(
        std::istream & istream = std::cin,
        std::ostream & ostream = std::cout
    ) const -> void override {
        PFF::context context{istream, ostream};
        parse(context);
    }

    auto map(std::string name, T value) -> _ {
        _map[value] = std::move(name);
        return *this;
    }

private:
    std::string _name;
    std::unordered_map<T ,std::string> _map;
};

struct abstract_tag {};

template<>
class _<abstract_tag> : public format {
public:
    template<typename ... Formats>
    _(std::string name, Formats && ... formats)
        : _name{std::move(name)}
    {
        append(std::forward<Formats>(formats) ...);
    }

    _(const _ &) = default;
    _(_ &&) = default;

    auto size() const -> std::size_t override {
        std::size_t size = 0;
        for (auto && child : children) {
            size += child->size();
        }
        return size;
    }

    virtual auto parse(PFF::context & context) const -> void override {
        context.push_namespace(_name);
        for (auto && child : children) {
            child->parse(context);
        }
        context.pop_namespace();
    }

    virtual auto parse(
        std::istream & istream = std::cin,
        std::ostream & ostream = std::cout
    ) const -> void override {
        PFF::context context{istream, ostream};
        parse(context);
    }

private:
    auto append() -> void {}

    template<typename Format, typename ... Formats>
    auto append(Format && fmt, Formats && ... fmts) -> void {
        children.push_back(std::make_shared<Format>(std::forward<Format>(fmt)));
        append(std::forward<Formats>(fmts) ...);
    }

    std::string _name;
    std::vector<std::shared_ptr<format>> children;
};

// class _<string_tag> {
// public:
//     template<typename ... Formats>
//     _(std::string name, PFF::string n) : _name{std::move(name)} {}
//
//     virtual auto size() const -> std::size_t override {
//         return ;
//     }
//
// private:
//     PFF:::string string;
// };

// template<
//     typename Function,
//     typename = std::enable_if_t<!std::is_integral_v<Integral>>>
// class string {
// public:
//     string(Function function) : function{function} {}
//     auto size(context ctx) -> std::size_t {
//         return function(ctx);
//     }
//
// private:
//     Function function;
// };
//
// template<
//     typename Integral,
//     typename = std::enable_if_t<std::is_integral_v<Integral>>>
// class string {
// public:
//     string(Integral size) : _size{size} {}
//     auto size(context ctx) -> std::size_t {
//         return _size;
//     }
//
// private:
//     Integral _size;
// };

template<typename T, typename ... U>
_(std::string, _<T> &&, U && ...) -> _<abstract_tag>;

template<typename T>
_(std::string, T &&) -> _<std::decay_t<T>>;

} // namespace PFF

#endif // PFF_PFF_HPP
