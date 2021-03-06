#pragma once

#include <mutex>

#include "plugins/shell/shell.h"
#include "mavlink_include.h"
#include "plugin_impl_base.h"
#include "system.h"

namespace mavsdk {

class System;

class ShellImpl : public PluginImplBase {
public:
    ShellImpl(System& system);
    ~ShellImpl();

    void init() override;
    void deinit() override;

    void enable() override;
    void disable() override;

    Shell::Result shell_command(const Shell::ShellMessage& shell_message);

    Shell::Result shell_command_response_async(Shell::result_callback_t& callback);
    ShellImpl(const ShellImpl&) = delete;
    ShellImpl& operator=(const ShellImpl&) = delete;

private:
    bool send_shell_message_mavlink();

    void process_shell_message(const mavlink_message_t& message);

    void receive_shell_message_timeout();

    bool is_transfer_in_progress();

    std::promise<void> _transfer_finished_promise{};
    std::future<void> _transfer_finished_future{};

    std::mutex _transfer_mutex{};

    void* _shell_message_timeout_cookie{nullptr};

    Shell::result_callback_t _result_subscription{nullptr};

    void finish_transfer(Shell::Result result, Shell::ShellMessage response_shell_message);

    Shell::ShellMessage _shell_message{};

    Shell::ShellMessage _response{};
};
} // namespace mavsdk