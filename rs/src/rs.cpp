#include "rs.h"
#include "ansi/rs_ansi_2.h"



void rs_example (const char* text /*="The cake is a lie."*/) {
    // Test ANSI
    {
        constexpr auto FG_RED_BG_GREEN = SGR_FG_RED + SGR_BG_GREEN;
        printf(">>> %s <<<\n", (FG_RED_BG_GREEN + "red text with a green background!!1!" + SGR_RESET)->data());

        constexpr auto hello_defaulted      = red("Hello");
        constexpr auto hello_not_defaulted  = red<false>("Hello");
        printf("    defaulted: \"%s, world!\"\n", hello_defaulted->data());
        printf("not defaulted: \"%s, world!\x1b[0m\"\n", hello_not_defaulted->data());
    }

    // Test log context
    {
        const auto lambda = [](rs_log_context& ctx){
            ctx.error("This is an error! (%d)", 1234); // TODO: Last character is not printed for some reason... Unless we add another character...
            ctx.warn("This is a warning!");
            ctx.info("This is some info!");
            ctx.trace("This is a trace!");
            
            // This should cause snprintf to fail.
            // If it occurs you will see: "(failed to format string)"
            wchar_t invalid_encoding[] = { 0xffff, 0 };
            ctx.info("%ls", invalid_encoding);
        };

        {
            auto ctx = rs_log_context(rs_log_level::ALL, stdout);
            lambda(ctx);
        }
        {
            auto ctx = rs_log_context(rs_log_level::ALL, "my-log.txt");
            lambda(ctx);
        }
    }
}

