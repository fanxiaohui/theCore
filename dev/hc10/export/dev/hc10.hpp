#ifndef DEV_BT_HC10_HPP_
#define DEV_BT_HC10_HPP_

#include <ecl/err.hpp>
#include <stdint.h>
#include <stddef.h>

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
    static err xfer_common(const uint8_t *tx, uint8_t *rx, size_t sz);
};

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
    static err xfer_async(const uint8_t *tx, uint8_t *rx, size_t sz);
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
