#ifndef DEV_BT_HC10_HPP_
#define DEV_BT_HC10_HPP_

#include <ecl/err.hpp>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

#include <algorithm>

namespace ecl
{


//! HC10 common operations.
//! \tparam Uart bus to work with.
//! \details HC10 driver exclusively holds UART bus.
template<typename Uart>
class hc10_common
{
public:
    static err init();

    static err power_up();

    static err power_down();

private:
    static err send_recv(const char *cmd, const char *exp_resp, size_t cmd_size);
    static err xfer_common(const uint8_t *tx, uint8_t *rx, size_t tx_sz, size_t rx_sz);

    struct uart_guard
    {
        uart_guard() { Uart::lock(); }
        ~uart_guard() { Uart::unlock(); }
    };
};

//------------------------------------------------------------------------------

template<typename Uart>
err hc10_common<Uart>::init()
{
    constexpr char at_cmd[] = "AT";

    err rc;

    Uart::init();

    // Locks UART
    uart_guard guard;

    if (is_error(rc = send_recv(at_cmd, "OK", sizeof(at_cmd)))) {
        return rc;
    }

    return err::ok;
}

//------------------------------------------------------------------------------

template<typename Uart>
err hc10_common<Uart>::send_recv(const char *cmd, const char *exp_resp, size_t cmd_size)
{
    // Real response
    char resp[16] = {0};

    auto rc = xfer_common(reinterpret_cast<const uint8_t*>(cmd),
                          reinterpret_cast<uint8_t*>(resp),
                          cmd_size, sizeof(resp) - 1);

    if (is_error(rc)) {
        return rc;
    }

    if (strncmp(resp, exp_resp, std::min(sizeof(resp), cmd_size))) {
        return err::inval;
    }

    return err::ok;
}

template<typename Uart>
err hc10_common<Uart>::xfer_common(const uint8_t *tx, uint8_t *rx, size_t tx_sz, size_t rx_sz)
{
    Uart::set_buffers(tx, rx, tx_sz, rx_sz);
    return Uart::xfer();
}

//------------------------------------------------------------------------------

template<typename Uart, typename Hdls>
class hc10_async
{
public:
    static err start_discovery();

    static err xfer(const uint8_t *tx, uint8_t *rx, size_t sz);

    static err xfer(const uint8_t *tx, size_t sz);

    static err xfer(uint8_t *rx, size_t sz);

private:
    static err xfer_async(const uint8_t *tx, uint8_t *rx, size_t tx_sz);
};

//------------------------------------------------------------------------------

template<typename Uart>
class hc10_sync
{
public:
    static err start_discovery();

    static err xfer(const uint8_t *tx, uint8_t *rx, size_t sz);

    static err xfer(const uint8_t *tx, size_t sz);

    static err xfer(uint8_t *rx, size_t sz);

private:
    static err xfer_sync(const uint8_t *tx, uint8_t *rx, size_t sz);
};

//!
template<typename ModeTrait>
class hc10
{
public:
    //!
    //! \brief init
    //! \return
    //!
    static err init();

    //!
    static err power_up();

    //!
    static err power_down();

    //!
    static err start_discovery();

    static err xfer(const uint8_t *tx, uint8_t *rx, size_t sz);

    static err xfer(const uint8_t *tx, size_t sz);

    static err xfer(uint8_t *rx, size_t sz);

private:

};

} // namespace ecl

#endif // DEV_BT_HC10_HPP_
