
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#include "nexora/diagnostics/diagnostic_sink.hpp"

namespace
{
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

    class TestDiagnosticSink final
        : public nexora::diagnostics::DiagnosticSink
    {
    public:
        void write(
            const nexora::diagnostics::DiagnosticEvent& event
        ) override
        {
            messages.push_back(event.message);
        }

        void flush() override
        {
            ++flushCount;
        }

        std::vector<std::string> messages;
        int flushCount = 0;
    };

    nexora::diagnostics::DiagnosticEvent createEvent(
        const std::string& message
    )
    {
        using namespace nexora::diagnostics;

        DiagnosticEvent event{};
        event.severity = DiagnosticSeverity::Info;
        event.code = DiagnosticCode::Unknown;
        event.message = message;

        return event;
    }

    void testWriteEvent()
    {
        TestDiagnosticSink sink;

        sink.write(createEvent("First event"));

        expect(sink.messages.size() == 1, "Sink receives one event");

        expect(
            sink.messages[0] == "First event",
            "Sink preserves event message"
        );
    }

    void testMultipleEvents()
    {
        TestDiagnosticSink sink;

        sink.write(createEvent("First"));
        sink.write(createEvent("Second"));
        sink.write(createEvent("Third"));

        expect(
            sink.messages.size() == 3,
            "Sink receives multiple events"
        );

        expect(
            sink.messages[2] == "Third",
            "Sink preserves event order"
        );
    }

    void testFlush()
    {
        TestDiagnosticSink sink;

        sink.flush();

        expect(
            sink.flushCount == 1,
            "Sink flush is invoked"
        );

        sink.flush();

        expect(
            sink.flushCount == 2,
            "Sink supports repeated flush"
        );
    }

    void testPolymorphicSink()
    {
        TestDiagnosticSink concreteSink;

        nexora::diagnostics::DiagnosticSink& sink = concreteSink;

        sink.write(createEvent("Polymorphic event"));
        sink.flush();

        expect(
            concreteSink.messages.size() == 1,
            "Sink works through base interface"
        );

        expect(
            concreteSink.flushCount == 1,
            "Base interface invokes flush"
        );
    }
}

int main()
{
    std::cout << "\n=== NEXORA Diagnostic Sink Tests ===\n\n";

    testWriteEvent();
    testMultipleEvents();
    testFlush();
    testPolymorphicSink();

    std::cout << "\n-----------------------------\n";
    std::cout << "Total failures: " << failures << '\n';

    if (failures == 0)
    {
        std::cout << "All diagnostic sink tests passed!\n";
        return EXIT_SUCCESS;
    }

    std::cerr << "Some diagnostic sink tests failed!\n";
    return EXIT_FAILURE;
}
