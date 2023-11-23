#pragma once

#include "MK66F18.h"
#include "fsl_uart.h"

#include <array>
#include <cstdint>
#include <deque>
#include <iterator>
#include <optional>
#include <string>
#include <type_traits>
#include <vector>
#include <iostream>

class Stream {
private:
    std::vector<std::uint8_t> buffer;

protected:
    virtual std::optional<std::uint8_t> _get() = 0;
    virtual bool _eof() = 0;
    virtual void _put(std::uint8_t c) = 0;

public:
    // Read
    template <typename T>
    std::optional<std::decay_t<T>> get() {
        static_assert(std::is_integral_v<std::decay_t<T>>, "T must be an integral type");
        static_assert(sizeof(T) == 1, "T must be a byte");

        if (eof()) {
            return std::nullopt;
        }

        if (!buffer.empty()) {
            auto c = buffer.back();
            buffer.pop_back();
            return c;
        }

        return _get();
    }

    template <typename T>
    std::vector<std::decay_t<T>> read(std::size_t count) {
        static_assert(std::is_integral_v<std::decay_t<T>>, "T must be an integral type");
        static_assert(sizeof(T) == 1, "T must be a byte");

        if (eof()) {
            return {};
        }

        std::vector<std::decay_t<T>> data;
        data.reserve(count);

        while (data.size() < count) {
            auto c = get<std::decay_t<T>>();
            if (!c) {
                break;
            }
            data.push_back(*c);
        }

        return data;
    }

    template <typename T>
    std::optional<std::decay_t<T>> peek() {
        static_assert(std::is_integral_v<std::decay_t<T>>, "T must be an integral type");
        static_assert(sizeof(T) == 1, "T must be a byte");

        if (eof()) {
            return std::nullopt;
        }

        if (!buffer.empty())
            return buffer.back();

        return _get();
    }

    template <typename T>
    void putback(T c) {
        static_assert(std::is_integral_v<std::decay_t<T>>, "T must be an integral type");
        static_assert(sizeof(T) == 1, "T must be a byte");

        buffer.push_back(static_cast<std::decay_t<T>>(c));
    }

    template <typename It>
    void putback(It begin, It end) {
        using T = decltype(*begin);
        static_assert(std::is_integral_v<std::decay_t<T>>, "T must be an integral type");
        static_assert(sizeof(T) == 1, "T must be a byte");

        for (auto it = std::make_reverse_iterator(end); it != std::make_reverse_iterator(begin); ++it) {
            putback(*it);
        }
    }

    template <typename T>
    std::optional<std::vector<std::decay_t<T>>> readUntil(T delim) {
        static_assert(std::is_integral_v<std::decay_t<T>>, "T must be an integral type");
        static_assert(sizeof(T) == 1, "T must be a byte");

        std::vector<std::decay_t<T>> data;
        while (true) {
            auto c = get<std::decay_t<T>>();
            if (!c) {
                putback(data.begin(), data.end());
                return std::nullopt;
            }

            if (*c == delim) {
                return data;
            }
            data.push_back(*c);
        }
        return data;
    }

    bool eof() {
        return _eof();
    }

    // Write
    template <typename T>
    void put(T c) {
        static_assert(std::is_integral_v<std::decay_t<T>>, "T must be an integral type");
        static_assert(sizeof(T) == 1, "T must be a byte");

        _put(static_cast<std::uint8_t>(c));
    }

    template <typename It>
    void write(It begin, It end) {
        using T = decltype(*begin);
        static_assert(std::is_integral_v<std::decay_t<T>>, "T must be an integral type");
        static_assert(sizeof(T) == 1, "T must be a byte");

        for (auto it = begin; it != end; ++it) {
            put<std::decay_t<T>>(*it);
        }
    }

    template <typename T>
    void write(const std::vector<std::decay_t<T>>& data) {
        write(data.begin(), data.end());
    }

    void print(const std::string& str) {
        write(str.begin(), str.end());
    }

    void println(const std::string& str, std::string end = "\n") {
        print(str);
        print(end);
    }

    virtual ~Stream() = default;
};

class MockStream : public Stream {
private:
    std::deque<std::uint8_t> inBuffer;
    std::deque<std::uint8_t> outBuffer;

    std::optional<std::uint8_t> _get() override {
        std::uint8_t out = inBuffer.front();
        inBuffer.pop_front();
        return out;
    }

    bool _eof() override {
        return inBuffer.empty();
    }

    void _put(std::uint8_t c) override {
        outBuffer.push_back(c);
    }

public:
    MockStream() = default;
    MockStream(const std::deque<std::uint8_t>& in)
        : inBuffer(in) {
    }

    MockStream(const std::string& in)
        : inBuffer(in.begin(), in.end()) {
    }

    // template <typename It, typename T = std::decay_t<decltype(*std::declval<It>())>>
    // MockStream(It begin, It end)
    //     : inBuffer(reinterpret_cast<std::uint8_t>(&(*begin)), reinterpret_cast<std::uint8_t>(&(*end)))
    // {
    //     static_assert(std::is_integral_v<T>, "T must be an integral type");
    //     static_assert(sizeof(T) == 1, "T must be a byte");
    // }

    ~MockStream() override = default;

    template<typename Container>
    Container getOutBuffer() const {
        return {outBuffer.begin(), outBuffer.end()};
    }

    void clearOutBuffer() {
        outBuffer.clear();
    }

    std::deque<std::uint8_t>& getInBuffer() {
        return inBuffer;
    }
};

class UARTStream : public Stream {
private:
    using Buffer = std::vector<std::uint8_t>;

    UART_Type* m_uart;
    uart_handle_t m_handle;
    uart_transfer_t m_rxTransfer;
    uart_transfer_t m_txTransfer;
    Buffer m_ringBuffer;
    Buffer m_rxBuffer;

    volatile bool m_receivedFlag = false;

    static void trampoline(UART_Type *base, uart_handle_t *handle, status_t status, void *userData) {
        (void) base;
        (void) handle;
        auto self = reinterpret_cast<UARTStream*>(userData);
        self->callback(status);
    }

    void callback(status_t status) {
        if (status == kStatus_UART_RxIdle) {
            m_receivedFlag = true;
        }
    }

    std::optional<std::uint8_t> _get() override {
        std::size_t received;
        m_rxTransfer.data = m_rxBuffer.data();
        m_rxTransfer.dataSize = m_rxBuffer.size();
        UART_TransferReceiveNonBlocking(m_uart, &m_handle, &m_rxTransfer, &received);
        if (received != m_rxBuffer.size()) {
            while (!m_receivedFlag) {
                asm volatile("nop");
            }
            m_receivedFlag = false;
        }
        return m_rxBuffer[0];
    }

    bool _eof() override {
        return false;
    }

    void _put(std::uint8_t c) override {
        UART_WriteBlocking(m_uart, &c, 1);
    }

public:
    UARTStream(UART_Type* uart)
        : m_uart(uart) {
        m_rxBuffer.resize(1);
        m_ringBuffer.resize(256);
        UART_TransferCreateHandle(m_uart, &m_handle, trampoline, this);
        UART_TransferStartRingBuffer(m_uart, &m_handle, m_ringBuffer.data(), m_ringBuffer.size());
    }

    ~UARTStream() override = default;
};

class DebugStream : public Stream {
private:
    std::optional<std::uint8_t> _get() override {
        int data = ::getchar();
        if (data == EOF) {
            return std::nullopt;
        }

        return static_cast<std::uint8_t>(data);
    }

    bool _eof() override {
        return feof(stdin);
    }

    void _put(std::uint8_t c) override {
        ::putchar(static_cast<int>(c));
    }

public:
    DebugStream() = default;
    ~DebugStream() override = default;
};
