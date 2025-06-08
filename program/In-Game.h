#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

enum class Location {
    Cortile,
    ScaleDiEntrata,
    ScalePrincipali,
    CorridoioElementari1,
    CorridoioElementari2,
    CorridoioElementari3,
    CorridoioMedie1,
    CorridoioMedie2
};

enum class InGameState { Storia, Cap1Intro, TransitionToCortile, Cortile };

class InGame
{
private:
    sf::Texture storyTexture;
    sf::Texture cap1Texture;
    sf::Texture cortileTexture;
    sf::Texture scale1Texture;
    sf::Texture corridoio1Texture;
    sf::Texture corridoio2Texture;
    sf::Texture corridoio3Texture;
    sf::Texture scale2Texture;
    sf::Texture corridoio4Texture;
    sf::Texture corridoio5Texture;
    sf::Sprite storySprite;
    sf::Sprite cortile;
    sf::Sprite scaleDiEntrata;
    sf::Sprite corridoioElementari1;
    sf::Sprite corridoioElementari2;
    sf::Sprite corridoioElementari3;
    sf::Sprite corridoioMedie1;
    sf::Sprite corridoioMedie2;
    sf::Sprite scaleCentrali;
    sf::Text continueText;
    sf::Font font;
    sf::Sprite cap1Sprite;
    sf::RectangleShape transitionOverlay;
    float transitionDuration = 1.f;
    bool fadeOutPhase = false;  
    bool introStarted = false;
    bool traveled = false;

    bool transitioning = false;
    float transitionTimer;
    Location targetState = Location::Cortile;
    Location currentState = Location::Cortile;

    Location currentLoc = Location::Cortile;
    static sf::RenderWindow locWindow;
    InGameState sceneState = InGameState::Storia;
public:
    InGame();
    bool loadBackground(const std::string &filename);
    void playCampanaLoop();
    void draw(sf::RenderWindow &window);
    bool createBackgrounds(sf::RenderWindow &window);
    void updateGameScene(sf::RenderWindow &window);
    void startTransition(Location newState, sf::RenderWindow &window);
    void drawScene(sf::RenderWindow &window);
    void initNewCapitoloIntro(sf::RenderWindow &window);
    void drawCapitoloIntro(sf::RenderWindow &window);
    void drawCortile(sf::RenderWindow &window);
};