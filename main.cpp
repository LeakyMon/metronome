#include <gpiod.h>
#include <iostream>
#include <unistd.h>
#include <csignal>
#include <signal.h>
#include <thread>

gpiod_chip *chip = nullptr;
gpiod_line *input_line = nullptr;
gpiod_line *output_line = nullptr;


void cleanup() {
    if (output_line) {
        gpiod_line_set_value(output_line, 0);  // Set the line to low
        gpiod_line_release(output_line);       // Release the line
    }
     if (input_line) {
        gpiod_line_set_value(input_line, 0);  // Set the line to low
        gpiod_line_release(input_line);
    }
    if (chip) {
        gpiod_chip_close(chip);  // Close the GPIO chip
    }
}

void signalHandler(int signum) {
    std::cout << "Interrupt signal (" << signum << ") received.\n";

    if (output_line) {
        std::cout<<"Light"<<std::endl;
    gpiod_line_set_value(output_line, 0);  // Turn off the light
    gpiod_line_release(output_line);
    }
    if (input_line) {
        std::cout<<"Btn"<<std::endl;
        gpiod_line_release(input_line);
    }
    if (chip) {
        std::cout<<"Chip"<<std::endl;
        gpiod_chip_close(chip);
    }
    else{
        std::cout<<"A;; clear"<<std::endl;
    }

    exit(signum);
}




void check_button(gpiod_line* input_line, gpiod_line* output_line) {
    int last_val = gpiod_line_get_value(input_line);
    std::cout << "Last val " << last_val << std::endl;

    while (true) {
        int val = gpiod_line_get_value(input_line);

        if (val != last_val) {
            if (val == 1) {
                std::cout << "Button pressed" << std::endl;
                gpiod_line_set_value(output_line, 1);
                std::this_thread::sleep_for(std::chrono::milliseconds(500)); // Debounce delay
            } else {
                gpiod_line_set_value(output_line, 0);
            }
            last_val = val;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Check every 100 ms
    }
}

int main() {
    const char *chipname = "gpiochip4"; // Use the appropriate chip name
    const unsigned int input_line_offset = 17; // GPIO pin for the button
    const unsigned int output_line_offset = 23; // GPIO pin for the output
    signal(SIGINT, signalHandler);

    chip = gpiod_chip_open_by_name(chipname);
    if (!chip) {
        std::cerr << "Could not open chip." << std::endl;
        return 1;
    }

    input_line = gpiod_chip_get_line(chip, input_line_offset);
    if (!input_line) {
        std::cerr << "Could not get input line." << std::endl;
        gpiod_chip_close(chip);
        return 1;
    }

    output_line = gpiod_chip_get_line(chip, output_line_offset);
    if (!output_line) {
        std::cerr << "Could not get output line." << std::endl;
        gpiod_chip_close(chip);
        return 1;
    }

    // Request the input line with pull-up resistor
    if (gpiod_line_request_input_flags(input_line, "input-check", GPIOD_LINE_REQUEST_FLAG_BIAS_PULL_UP) < 0) {
        std::cerr << "Could not request input line with pull-up." << std::endl;
        gpiod_chip_close(chip);
        return 1;
    }

    if (gpiod_line_request_output(output_line, "output-control", 0) < 0) {
        std::cerr << "Could not request output line." << std::endl;
        gpiod_chip_close(chip);
        return 1;
    }

    check_button(input_line, output_line);

    gpiod_line_release(input_line);
    gpiod_line_release(output_line);
    gpiod_chip_close(chip);

    cleanup();

    return 0;
}

