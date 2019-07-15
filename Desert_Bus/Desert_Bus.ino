#include <Arduboy2.h>
#include <ArduboyTones.h>
#include "Font3x5.h"
Font3x5 Font = Font3x5();
Arduboy2 ard;
Sprites sprites;
ArduboyTones sound(ard.audio.enabled);
#include "Music.h"
#include "Bitmaps.h"
#include "GameStates.h"
#include "Game.h"

void setup() {
  ard.boot();
  ard.flashlight();
  ard.clear();
  ard.setFrameRate(30);
  ard.initRandomSeed();
  sound.tones(score);
}

void loop() {
  if(!ard.nextFrame())
    return;

  ard.pollButtons();
  ard.clear();
  switch(gameState){
    case GameState::MainMenu: MainMenuLoop(); break;
    case GameState::Game: GameLoop(); break;
    case GameState::End: EndGameLoop(); break;
    case GameState::PlayerNaming: PlayerNameLoop(); break;
  }
  
  ard.display();
}
