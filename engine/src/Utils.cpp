//
// Created by spaske00 on 13.5.24..
//

#include "engine/util/Utils.hpp"
#include "engine/util/Errors.hpp"
#include "spdlog/spdlog.h"
#include <fstream>

namespace rg {

    void Configuration::initialize() {
        auto config_path = get_config_path();
        std::ifstream f(config_path);
        if (!f.is_open()) {
            throw rg::FileNotFoundError(config_path, "Failed to load configuration file.");
        }

        try {
            m_config = json::parse(f);
        } catch (const std::exception &e) {
            std::string message(e.what());
            throw rg::ConfigurationError(std::format("Error \"{}\" occurred while parsing the configuration file. "
                                                     "Please make sure that the file is in the correct json format.",
                                                     message));
        }
    }

    std::filesystem::path Configuration::get_config_path() {
        auto config_arg = ArgParser::instance()->arg<std::string>("--configuration");
        if (!config_arg.has_value() || !exists(std::filesystem::path(config_arg.value()))) {
            return create_default();
        }
        return config_arg.value();
    }

    std::filesystem::path Configuration::create_default() {
        std::ofstream f(CONFIG_FILE_NAME.data());
        if (!f.is_open()) {
            throw rg::ConfigurationError(std::format("Failed to open configuration file."));
        }
        json default_config;
        default_config["shaders"]["path"]         = "resources/shaders";
        default_config["window"]["width"]         = 800;
        default_config["window"]["height"]        = 600;
        default_config["window"]["title"]         = "Hello, window!";
        default_config["assets"]["models_path"]   = "resources/models";
        default_config["assets"]["textures_path"] = "resources/textures";
        f << default_config.dump(4);
        return CONFIG_FILE_NAME;
    }

    Configuration *Configuration::instance() {
        static Configuration configuration;
        return &configuration;
    }

    Configuration::json &Configuration::config() {
        return Configuration::instance()->m_config;
    }

    std::string ConfigurationError::report() const {
        return std::format("ConfigurationError {}:{}. {}.", location().file_name(), location().line(), message());
    }

    ArgParser *ArgParser::instance() {
        static ArgParser arg_parser;
        return &arg_parser;
    }

    void ArgParser::initialize(int argc, char **argv) {
        m_argc = argc;
        m_argv = argv;
    }

    std::string ArgParser::get_arg_value(std::string_view arg_name) {
        for (int i = 0; i < m_argc; ++i) {
            std::string_view token(m_argv[i]);
            if (token == arg_name) {
                std::string arg_value(m_argv[i + 1]);
                RG_GUARANTEE(!arg_value.starts_with("--"), "No get_arg_value for argument: \"{}\" provided.", arg_name);
                return arg_value;
            }
        }
        return "";
    }

    std::string read_text_file(const std::filesystem::path &path) {
        RG_GUARANTEE(std::filesystem::exists(path), "File {} doesn't exist.", path.string());
        std::ifstream file(path);
        std::stringstream ss;
        ss << file.rdbuf();
        return ss.str();
    }
} // namespace rg