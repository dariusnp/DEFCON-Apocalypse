#pragma once

#include <vector>
#include <string>
#include <climits>
#include <iostream>

#include "../../Dependencies/GLFW/include/glfw3.h"
#include "../../Dependencies/glm/glm.hpp"
#include "../iamgui/imgui.h"
#include "../backends/imgui_impl_glfw.h"
#include "../backends/imgui_impl_opengl3.h"


namespace sas
{
    enum class GameState
    {
        MainMenu,
        Options,
        MainGame,
        PauseMenu,
        PauseOptions,
        Exit,
        GameEnding
    };


    class UI
    {
    private:

        mutable bool volume = true;
        float& stamina;
        GameState& currState;
        int objectiveIndex = 0;
        std::string objective[7] = { "Kill Arasaka's Goons", "Get the key", "Open the gate", "Take the gold", "Kill the security Guads", "Kill Adam Smasher", "Celebrate!:D" };
        ImGuiIO io;


        void renderPause() const noexcept;
        void renderOptions() const noexcept;
        void renderMain() const noexcept;
        void renderMenu() const noexcept;
        void renderEnding() const noexcept;

    public:
        UI(GLFWwindow* window, GameState& NcurrState, float& Nstamina)
            : currState(NcurrState), stamina(Nstamina)
        {
            IMGUI_CHECKVERSION();
            ImGui::CreateContext();
            this->io = ImGui::GetIO();

            ImGui::StyleColorsDark();
            ImGui_ImplGlfw_InitForOpenGL(window, true);
            ImGui_ImplOpenGL3_Init("#version 130");
        };

        bool getVolState() const noexcept;
        void renderUI() const noexcept;
        void uppdateUI(const GameState& nextState) noexcept;
        void nextObjective() noexcept;
    };

}