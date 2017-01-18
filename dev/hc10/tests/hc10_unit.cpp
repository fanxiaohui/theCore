#include <dev/hc10.hpp>

#include <CppUTest/TestHarness.h>
#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTestExt/MockSupport.h>

//------------------------------------------------------------------------------

TEST_GROUP(hc10_common)
{
    void setup()
    {

    }

    void teardown()
    {

    }
};

//------------------------------------------------------------------------------
// HC10 common mode mocks

struct hc10_common_uart_mock
{
public:
    ecl::err init()
    {
        mock("uart_mock").actualCall("init");
        return (ecl::err) mock("uart_mock").returnIntValueOrDefault((int)ecl::err::ok);
    }

    void lock()
    {
        mock("uart_mock").actualCall("lock");
    }

    void unlock()
    {
        mock("uart_mock").actualCall("unlock");
    }

    ecl::err set_buffers(const uint8_t *tx, uint8_t *rx, size_t size)
    {
        mock("uart_mock").actualCall("set_buffers")
                .withParameter("size", size)
                .withConstPointerParameter("tx", tx)
                .withPointerParameter("rx", rx);

        return (ecl::err) mock("uart_mock").returnIntValueOrDefault((int)ecl::err::ok);
    }

    ecl::err xfer(size_t *sent = nullptr, size_t *received = nullptr)
    {
        mock("uart_mock").actualCall("xfer")
                .withPointerParameter("sent", sent)
                .withPointerParameter("received", received);

        return (ecl::err) mock("uart_mock").returnIntValueOrDefault((int)ecl::err::ok);
    }

};

//------------------------------------------------------------------------------
// HC-10 common mode tests

TEST(hc10_common, init)
{

}

int main(int argc, char *argv[])
{
    return CommandLineTestRunner::RunAllTests(argc, argv);
}
