#include "rs.h"
#include "ansi/rs_ansi_2.h"



void rs_example (void) {
    constexpr auto FG_RED_BG_GREEN = SGR_FG_RED + SGR_BG_GREEN;
    constexpr auto hello_defaulted = red("Hello");
    constexpr auto hello_not_defaulted = red<false>("Hello");

    printf(">>> %s <<<\n", (FG_RED_BG_GREEN + "red text with a green background" + SGR_RESET)->data());
    printf("    defaulted: \"%s, world!\"\n", hello_defaulted->data());
    printf("not defaulted: \"%s, world!\x1b[0m\"\n", hello_not_defaulted->data());
    // debug_print(hello_defaulted);
    // debug_print(hello_not_defaulted);

    return;




    // // Test log levels
    // {
    //     using namespace rs;
    // 
    //     log::error("This is an error!");
    //     log::warn("This is a warning!");
    //     log::info("This is some info!");
    //     log::trace("This is a trace!");
    // 
    //     // This should cause snprintf to error.
    //     wchar_t s[] = { 0xffff };
    //     log::info("%ls", s);
    // }

    // Test log context
    {
        const auto lambda = [](rs_log_context& ctx){
            ctx.error("This is an error! (%d)", 1234);
            ctx.warn("This is a warning!");
            ctx.info("This is some info!");
            ctx.trace("This is a trace!");
            
            // This should cause snprintf to error.
            wchar_t invalid_encoding[] = { 0xffff, 0 };
            ctx.info("My data: %ls", invalid_encoding);
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

