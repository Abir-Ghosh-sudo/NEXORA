
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#include "nexora/diagnostics/diagnostic_dispatcher.hpp"

namespace
{
    using namespace nexora::diagnostics;

    int failures = 0;

    void expect(bool condition, const char* testName)
    {
        if (condition)
        {
            std::cout << "[PASS] " << testName << '\n';
        }
        else
        {
            std::cerr << "[FAIL] " << testName << '\n';
            ++failures;
        }
    }

    DiagnosticEvent createEvent(const std::string& message)
    {
        DiagnosticEvent event{};
        event.severity = DiagnosticSeverity::Error;
        event.code = DiagnosticCode::GeneralFailure;
        event.message = message;
        return event;
    }

    class TestSink final : public DiagnosticSink
    {
    public:
        void write(const DiagnosticEvent& event) override
        {
            receivedMessages.push_back(event.message);
        }

        void flush() override
        {
            ++flushCount;
        }

        std::vector<std::string> receivedMessages;
        int flushCount = 0;
    };

    void testDispatchRecordsEvent()
    {
        DiagnosticManager manager;
        TestSink sink;
        DiagnosticDispatcher dispatcher(manager, sink);

        dispatcher.dispatch(createEvent("Dispatcher test event"));

        expect(
            manager.eventCount() == 1,
            "Dispatcher records event in manager"
        );
    }

    void testDispatchWritesToSink()
    {
        DiagnosticManager manager;
        TestSink sink;
        DiagnosticDispatcher dispatcher(manager, sink);

        dispatcher.dispatch(createEvent("Sink delivery test"));

        expect(
            sink.receivedMessages.size() == 1,
            "Dispatcher sends event to sink"
        );

        expect(
            sink.receivedMessages[0] == "Sink delivery test",
            "Sink receives correct event message"
        );
    }

    void testMultipleDispatches()
    {
        DiagnosticManager manager;
        TestSink sink;
        DiagnosticDispatcher dispatcher(manager, sink);

        dispatcher.dispatch(createEvent("Event one"));
        dispatcher.dispatch(createEvent("Event two"));

        expect(
            manager.eventCount() == 2,
            "Manager records multiple dispatched events"
        );

        expect(
            sink.receivedMessages.size() == 2,
            "Sink receives multiple dispatched events"
        );
    }

    void testFlush()
    {
        DiagnosticManager manager;
        TestSink sink;
        DiagnosticDispatcher dispatcher(manager, sink);

        dispatcher.flush();

        expect(
            sink.flushCount == 1,
            "Dispatcher forwards flush to sink"
        );
    }
}

int main()
{
    std::cout << "\n=== NEXORA Diagnostic Dispatcher Tests ===\n\n";

    testDispatchRecordsEvent();
    testDispatchWritesToSink();
    testMultipleDispatches();
    testFlush();

    std::cout << "\n-----------------------------\n";
    std::cout << "Total failures: " << failures << '\n';

    if (failures == 0)
    {
        std::cout << "All diagnostic dispatcher tests passed!\n";
        return EXIT_SUCCESS;
    }

    std::cerr << "Some diagnostic dispatcher tests failed!\n";
    return EXIT_FAILURE;
}
