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
bool check_boost(SensorData &s)
{
    return s.accel_z > 30.0;
};

bool check_coast(SensorData &s)
{
    return s.accel_z < 10.0;
};

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

    SensorData data{};

    // Before your GUI loop starts:
    StateMachine test_sm;

    State idle("IDLE");
    State boost("BOOST");
    State coast("COAST");

    test_sm.addState(&idle);
    test_sm.addState(&boost);
    test_sm.addState(&coast);
    test_sm.setInitState(&idle);

    test_sm.defineTransition(&idle, &boost, check_boost);
    test_sm.defineTransition(&boost, &coast, check_coast);

    // Test it
    SensorData sensors;
    std::cout << "Initial: " << test_sm.getCurrentState()->name << "\n";

    sensors.accel_z = 35.0;
    test_sm.update(sensors);
    std::cout << "After accel=35: " << test_sm.getCurrentState()->name << "\n";

    sensors.accel_z = 5.0;
    test_sm.update(sensors);
    std::cout << "After accel=5: " << test_sm.getCurrentState()->name << "\n";

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::SetNextWindowSize(ImVec2(0.0f, 0.0f));

        // UI
        ImGui::Begin("Sensor Controls");

        // Sliders
        ImGui::SliderFloat("Altitude", &data.altitude, 0.0f, 10000.0f, "value = %0.3f");
        ImGui::SliderFloat("Velocity", &data.velocity, 0.0f, 10000.0f, "value = %0.3f");
        ImGui::SliderFloat("Accel_X", &data.accel_x, 0.0f, 10000.0f, "value = %0.3f");
        ImGui::SliderFloat("Accel_Y", &data.accel_y, 0.0f, 10000.0f, "value = %0.3f");
        ImGui::SliderFloat("Accel_Z", &data.accel_z, 0.0f, 10000.0f, "value = %0.3f");
        ImGui::SliderFloat("battery", &data.battery, 0.0f, 10000.0f, "value = %0.3f");
        ImGui::Checkbox("GPS Lock: ", &data.gps_lock);
        ImGui::Checkbox("RF Lock:", &data.radio_lock);

        ImGui::End();

        ImGui::Begin("State Machine");
        // ImGui::Text("Current: %s", data.state.c_str());
        ImGui::Text("\nTransitions: \n");
        ImGui::End();

        ImGui::Begin("State History Timeline");

        ImGui::End();

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
