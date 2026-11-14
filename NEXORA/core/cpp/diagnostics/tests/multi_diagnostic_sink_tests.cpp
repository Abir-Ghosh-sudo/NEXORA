
#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "nexora/diagnostics/multi_diagnostic_sink.hpp"

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
        event.severity = DiagnosticSeverity::Info;
        event.code = DiagnosticCode::GeneralFailure;
        event.message = message;
        return event;
    }

    class TestSink final : public DiagnosticSink
    {
    public:
        void write(const DiagnosticEvent& event) override
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

    void testAddSink()
    {
        MultiDiagnosticSink multiSink;

        auto sink = std::make_shared<TestSink>();
        multiSink.addSink(sink);

        expect(
            multiSink.sinkCount() == 1,
            "Adding a sink increases sink count"
        );
    }

    void testDispatchToMultipleSinks()
    {
        MultiDiagnosticSink multiSink;

        auto firstSink = std::make_shared<TestSink>();
        auto secondSink = std::make_shared<TestSink>();

        multiSink.addSink(firstSink);
        multiSink.addSink(secondSink);

        multiSink.write(createEvent("Shared diagnostic event"));

        expect(
            firstSink->messages.size() == 1,
            "First sink receives event"
        );

        expect(
            secondSink->messages.size() == 1,
            "Second sink receives event"
        );

        expect(
            firstSink->messages[0] == "Shared diagnostic event" &&
            secondSink->messages[0] == "Shared diagnostic event",
            "All sinks receive the same event message"
        );
    }

    void testFlushAllSinks()
    {
        MultiDiagnosticSink multiSink;

        auto firstSink = std::make_shared<TestSink>();
        auto secondSink = std::make_shared<TestSink>();

        multiSink.addSink(firstSink);
        multiSink.addSink(secondSink);

        multiSink.flush();

        expect(
            firstSink->flushCount == 1,
            "Flush reaches first sink"
        );

        expect(
            secondSink->flushCount == 1,
            "Flush reaches second sink"
        );
    }

    void testClearSinks()
    {
        MultiDiagnosticSink multiSink;

        multiSink.addSink(std::make_shared<TestSink>());
        multiSink.addSink(std::make_shared<TestSink>());

        multiSink.clearSinks();

        expect(
            multiSink.sinkCount() == 0,
            "Clearing sinks resets sink count"
        );
    }

    void testNullSinkRejected()
    {
        MultiDiagnosticSink multiSink;

        bool rejected = false;

        try
        {
            multiSink.addSink(nullptr);
        }
        catch (const std::invalid_argument&)
        {
            rejected = true;
        }

        expect(
            rejected,
            "Adding a null sink throws invalid_argument"
        );
    }
}

int main()
{
    std::cout << "\n=== NEXORA Multi Diagnostic Sink Tests ===\n\n";

    testAddSink();
    testDispatchToMultipleSinks();
    testFlushAllSinks();
    testClearSinks();
    testNullSinkRejected();

    std::cout << "\n-----------------------------\n";
    std::cout << "Total failures: " << failures << '\n';

    if (failures == 0)
    {
        std::cout << "All multi diagnostic sink tests passed!\n";
        return EXIT_SUCCESS;
    }

    std::cerr << "Some multi diagnostic sink tests failed!\n";
    return EXIT_FAILURE;
}
