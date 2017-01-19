#include <dev/hc10.hpp>

#include <CppUTest/TestHarness.h>
#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTestExt/MockSupport.h>

// Error code helper
// TODO: move it to 'utils' headers and protect with check of
// current test state (enabled or disabled)
static SimpleString StringFrom(ecl::err err)
{
    return SimpleString{ecl::err_to_str(err)};
}



//------------------------------------------------------------------------------
// HC10 common mode mocks

struct hc10_common_uart_mock
{
public:
    static ecl::err init()
    {
        mock("uart_mock").actualCall("init");
        return (ecl::err) mock("uart_mock").returnIntValueOrDefault((int)ecl::err::ok);
    }

    static void lock()
    {
        mock("uart_mock").actualCall("lock");
        m_locked = true;
    }

    static void unlock()
    {
        mock("uart_mock").actualCall("unlock");
        m_locked = false;
    }

    static ecl::err set_buffers(const uint8_t *tx, uint8_t *rx, size_t size)
    {
        mock("uart_mock").actualCall("set_buffers")
                .withParameter("size", size)
                .withStringParameter("tx", (const char*)tx)
                .withPointerParameter("rx", rx);

        return (ecl::err) mock("uart_mock").returnIntValueOrDefault((int)ecl::err::ok);
    }

    static ecl::err set_buffers(const uint8_t *tx, uint8_t *rx, size_t tx_sz, size_t rx_sz)
    {
        mock("uart_mock").actualCall("set_buffers")
                .withStringParameter("tx", (const char *)tx);

        m_tx = (const char*)tx;
        m_rx = (char*)rx;
        m_rx_sz = rx_sz;

        return (ecl::err) mock("uart_mock").returnIntValueOrDefault((int)ecl::err::ok);
    }

    static ecl::err xfer(size_t *sent = nullptr, size_t *received = nullptr)
    {
        mock("uart_mock").actualCall("xfer").withOutputParameter("rx", m_rx);

        return (ecl::err) mock("uart_mock").returnIntValueOrDefault((int)ecl::err::ok);
    }

    static void cleanup()
    {
        m_locked = false;
        m_tx = nullptr;
        m_rx = nullptr;
        m_rx_sz = 0;
    }

    static const char   *m_tx;
    static char         *m_rx;
    static size_t       m_rx_sz;
    static bool         m_locked;

};

const char *hc10_common_uart_mock::m_tx;
char *hc10_common_uart_mock::m_rx;
size_t hc10_common_uart_mock::m_rx_sz;
bool hc10_common_uart_mock::m_locked;

//------------------------------------------------------------------------------

TEST_GROUP(hc10_common)
{
    void setup()
    {

    }

    void teardown()
    {
        hc10_common_uart_mock::cleanup();
        mock().clear();
    }
};

//------------------------------------------------------------------------------
// HC-10 common mode tests

using hc10_common = ecl::hc10_common<hc10_common_uart_mock>;

TEST(hc10_common, init)
{
    const char valid_resp[] = "OK";

    mock("uart_mock").expectOneCall("init");
    mock("uart_mock").expectOneCall("lock");
    mock("uart_mock").expectOneCall("unlock");
    mock("uart_mock").expectOneCall("set_buffers")
            .withStringParameter("tx", "AT");
    mock("uart_mock").expectOneCall("xfer")
            .withOutputParameterReturning("rx", valid_resp, sizeof(valid_resp));

    auto rc = hc10_common::init();

    CHECK_EQUAL(ecl::err::ok, rc);

    mock().checkExpectations();
}

TEST(hc10_common, init_invalid_resp)
{
    const char invalid_resp[] = "BLEBLE";

    mock("uart_mock").expectOneCall("init");
    mock("uart_mock").expectOneCall("lock");
    mock("uart_mock").expectOneCall("unlock");
    mock("uart_mock").expectOneCall("set_buffers")
            .withStringParameter("tx", "AT");
    mock("uart_mock").expectOneCall("xfer")
            .withOutputParameterReturning("rx", invalid_resp, sizeof(invalid_resp));

    auto rc = hc10_common::init();

    CHECK_EQUAL(ecl::err::inval, rc);

    mock().checkExpectations();
}


int main(int argc, char *argv[])
{
    return CommandLineTestRunner::RunAllTests(argc, argv);
}
