#pragma once

#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <chrono>
#include <thread>
#include "MainMenu.h"
#include "Particle.h"
#include <vector>
#include "OptionsMenu.h"
#include "Loading.h"
#include "In-Game.h"
#include "Pawn.h"

enum class GameState { 
    MainMenu, 
    Options, 
    Play,
    Loading,
    Game
};

class Game
{
private:
    sf::RenderWindow* window;
    sf::VideoMode videoMode;
    sf::Event ev;
    sf::Texture bgTexture;
    MainMenu menu; // Istanza unica del menu
    std::vector<Particle> particles; // Particelle
    GameState currentState = GameState::MainMenu;
    OptionsMenu* optionsMenu; // Usa un puntatore
    Loading loading;
    bool transitioning = false;
    bool fadeOutPhase = false;    // false: fase di fade in, true: fade out
    float transitionTimer = 0.f;
    float transitionDuration = 1.f; // Durata per ciascuna fase (fade in o fade out)
    sf::RectangleShape transitionOverlay;
    GameState targetState; // stato verso cui transiziona
    InGame* gameScene = nullptr;
    float loadingStateTimer = 0.f;
    float loadingDelay = 4.f; // delay in secondi
    sf::Music backMenuMusic;
    Pawn player;
    
    float dt;
    bool fullscreen = false;

    void initializeVars();
    void initWindow();
    void initParticles();
    void updateParticles(float dt);
public:
    Game();
    virtual ~Game();

    const bool running() const;
    void drawText(sf::Text text);
    sf::Sprite getBackground();
    void startTransition(GameState newState);
    void toggleFullscreen();
    void playMenuBackgroundMusic();
    void stopMenuBackgroundMusic();
    void playButtonSound();
    void pollEvents();
    void update();
    void render();
};

#endif // ! GAME_H