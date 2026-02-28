#include <cstdio>
#include <iostream>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "core/sensor_data.h"
#include "core/state_machine.h"
#include "core/state.h"

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

// Define condition functions
// Existing
bool check_boost(SensorData &s)
{
    return s.accel_z > 1000.0; // 3+ Gs indicates liftoff
}

bool check_burnout(SensorData &s)
{
    return s.accel_z < 500.0; // Motor burned out, accel dropping
}

bool check_coast(SensorData &s)
{
    return s.accel_z < 100.0; // Low acceleration, coasting
}

bool check_apogee(SensorData &s)
{
    return s.velocity < 0.0; // Velocity goes negative = falling
}

bool check_descent_drogue(SensorData &s)
{
    return true; // Immediately after apogee
}

bool check_descent_main(SensorData &s)
{
    return s.altitude < 1000.0; // Deploy main chute at 1000 ft
}

bool check_landed(SensorData &s)
{
    return s.altitude < 50.0 && s.velocity > -10.0; // Near ground, slow descent
}

int main()
{
    // Init GLFW
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow *window = glfwCreateWindow(800, 600, "Apollo", NULL, NULL);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    // Load OpenGL
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    // Setup ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    SensorData sensors;
    StateMachine rocket;

    State init("INIT");
    State idle("IDLE");
    State boost("BOOST");
    State burnout("BURNOUT");
    State coast("COAST");
    State apogee("APOGEE");
    State descent_drogue("DESCENT_DROGUE");
    State descent_main("DESCENT_MAIN");
    State landed("LANDED");

    rocket.addState(&init);
    rocket.addState(&idle);
    rocket.addState(&boost);
    rocket.addState(&burnout);
    rocket.addState(&coast);
    rocket.addState(&apogee);
    rocket.addState(&descent_drogue);
    rocket.addState(&descent_main);
    rocket.addState(&landed);
    rocket.setInitState(&idle);

    rocket.defineTransition(&idle, &boost, check_boost);
    rocket.defineTransition(&boost, &burnout, check_burnout);
    rocket.defineTransition(&burnout, &coast, check_coast);
    rocket.defineTransition(&coast, &apogee, check_apogee);
    rocket.defineTransition(&apogee, &descent_drogue, check_descent_drogue);
    rocket.defineTransition(&descent_drogue, &descent_main, check_descent_main);
    rocket.defineTransition(&descent_main, &landed, check_landed);

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        // Get window dimensions
        ImGuiIO &io = ImGui::GetIO();
        float window_width = io.DisplaySize.x;
        float window_height = io.DisplaySize.y;

        // Left panel - sensor values
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImVec2(window_width * 0.25f, window_height));
        // CHANGE: Add flags here
        ImGui::Begin("Sensor Controls", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

        // Sliders
        ImGui::SliderFloat("Altitude", &sensors.altitude, 0.0f, 10000.0f, "value = %0.3f");
        ImGui::SliderFloat("Velocity", &sensors.velocity, -10000.0f, 10000.0f, "value = %0.3f");
        ImGui::SliderFloat("Accel_X", &sensors.accel_x, 0.0f, 10000.0f, "value = %0.3f");
        ImGui::SliderFloat("Accel_Y", &sensors.accel_y, 0.0f, 10000.0f, "value = %0.3f");
        ImGui::SliderFloat("Accel_Z", &sensors.accel_z, 0.0f, 10000.0f, "value = %0.3f");
        ImGui::SliderFloat("battery", &sensors.battery, 0.0f, 10000.0f, "value = %0.3f");
        ImGui::Checkbox("GPS Lock: ", &sensors.gps_lock);
        ImGui::Checkbox("RF Lock:", &sensors.radio_lock);

        if (ImGui::Button("Reset"))
        {
            // Reset all sensor values to zero
            sensors.altitude = 0.0f;
            sensors.velocity = 0.0f;
            sensors.accel_x = 0.0f;
            sensors.accel_y = 0.0f;
            sensors.accel_z = 0.0f;
            sensors.battery = 0.0f;
            sensors.gps_lock = false;
            sensors.radio_lock = false;

            rocket.setInitState(&idle);

            rocket.clearHistory();
        }

        ImGui::End();

        // Right panel - state machine
        ImGui::SetNextWindowPos(ImVec2(window_width * 0.25f, 0));
        ImGui::SetNextWindowSize(ImVec2(window_width * 0.75f, window_height * 0.6f));
        ImGui::Begin("State Machine", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
        ImGui::Text("Current: %s", rocket.getCurrentState()->name.c_str());

        ImGui::SeparatorText("Transitions");
        auto available = rocket.getAvailableTransitions();

        for (const auto &trans : available)
        {
            bool can_fire = trans.condition(sensors);
            ImGui::Text("%s %s -> %s",
                        can_fire ? "[✓]" : "[ ]",
                        trans.from->name.c_str(),
                        trans.to->name.c_str());
        }

        ImGui::End();

        // Bottom Right Panel - timelinee
        ImGui::SetNextWindowPos(ImVec2(window_width * 0.25f, window_height * 0.6f));
        ImGui::SetNextWindowSize(ImVec2(window_width * 0.75f, window_height * 0.4f));
        ImGui::Begin("State History Timeline", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

        auto history = rocket.getHistory();

        if (history.empty())
        {
            ImGui::Text("No state changes yet...");
        }
        else
        {
            for (const auto &change : history)
            {
                ImGui::Text("[%s] %s -> %s",
                            change.timestamp.c_str(),
                            change.from_state.c_str(),
                            change.to_state.c_str());
            }
        }
        ImGui::End();

        rocket.update(sensors);

        // Render
        ImGui::Render();
        int w, h;
        glfwGetFramebufferSize(window, &w, &h);
        glViewport(0, 0, w, h);
        glClearColor(0.1f, 0.1f, 0.12f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
