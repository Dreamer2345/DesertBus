#pragma once
#include <Arduboy2.h>

String Name = "TELER";

uint8_t RoadFrame = 0;
uint8_t PlayerX = 64;

uint16_t Miles = 0;

uint8_t Seconds = 0;
uint8_t Mins = 30;
uint8_t Hours = 7;


byte ObjX = 0;
uint8_t ObjY = 0;
byte ObjDir = 0;

char TempName[5] = {'A','A','A','A','A'};
uint8_t CharIndex = 0;

bool Complete = false;
bool Sound = false;
bool NightTime = false;

void DrawRoad(uint8_t HorizontalOffset,uint8_t Frame){
  const uint8_t SideWidth = 10;
  const uint8_t DividerLines = 3;
  const uint8_t DividerThickness = 3;
  const uint8_t DividerDepth = 4;

  uint8_t DividerFrame = Frame % 47;
  
  Point VerticalCutoff = Point(HorizontalOffset,25);
  Point LeftCutoff = Point(HorizontalOffset-64,47);
  Point RightCutoff = Point(HorizontalOffset+64,47);

  uint8_t CenterOffsX = (HorizontalOffset) - DividerThickness;
  uint8_t PixelIncrement = (47 / DividerLines);
  
  for(uint8_t i = 0; i < DividerThickness; i++){
    ard.drawLine(CenterOffsX+i,47,VerticalCutoff.x,VerticalCutoff.y,WHITE);
  }

  for(uint8_t i = 0; i < DividerLines+1; i++){
    for(uint8_t j = 0; j < DividerDepth; j++){
      ard.drawFastHLine(0,((i-1)*PixelIncrement)+DividerFrame+j,128,BLACK);
    }
  }  
  if(!NightTime){
    ard.drawLine(LeftCutoff.x,LeftCutoff.y,VerticalCutoff.x,VerticalCutoff.y,WHITE);
    ard.drawLine(LeftCutoff.x-SideWidth,LeftCutoff.y,VerticalCutoff.x,VerticalCutoff.y,WHITE);
    ard.drawLine(RightCutoff.x,RightCutoff.y,VerticalCutoff.x,VerticalCutoff.y,WHITE);
    ard.drawLine(RightCutoff.x+SideWidth,RightCutoff.y,VerticalCutoff.x,VerticalCutoff.y,WHITE);
  }else{
    sprites.drawExternalMask(22,27,RoadLights,RoadLightsMask,0,0);
  }
}

void UpdateX(){
  if(ard.pressed(LEFT_BUTTON)){
    PlayerX++;
  }
  
  if(ard.pressed(RIGHT_BUTTON)){
    PlayerX--;
  }
  
  if(ard.everyXFrames(10)){
    PlayerX--;
  }
}

void UpdateTime(){
  if(ard.everyXFrames(30)){
    Seconds++;
    if(Seconds % 8 == 0){
    Miles++;
  }
  }
  
  if(Seconds >= 60){
    Seconds = 0;
    Mins++;
  }

  if(Mins >= 60){
    Mins = 0;
    Hours++;
  }

  if(Hours >= 24){
    Hours = 0;
  }

  if((Hours >= 20)||(Hours < 6)){
    NightTime = true;
  } else {
    NightTime = false;
  }
  
}

void UpdateBush(){
  ObjY++;
  if(ObjY >= 55){
    ObjY = 10;
    ObjDir = random(0,2);
    if(ObjDir == 0){
      ObjX = PlayerX - 32;
    }else{
      ObjX = PlayerX + 16;
    }
  }

  if(ObjDir == 0){
    ObjX -= 4;
  } else {
    ObjX += 4;
  }
}

void DrawBush(uint8_t offset){
  byte BushPos = offset + (ObjX - 64);
  uint8_t frame = (ObjY / 16) % 3;
  sprites.drawSelfMasked(BushPos,ObjY,Bush,frame);
}

void DrawCabin(){
  sprites.drawExternalMask(0,0,Dash,DashMask,0,0);
  
  if(ard.pressed(LEFT_BUTTON)){
    sprites.drawSelfMasked(7,42,Steering,1);  
  }
  else{
    if(ard.pressed(RIGHT_BUTTON)){
      sprites.drawSelfMasked(7,42,Steering,2);  
    }
    else{
      sprites.drawSelfMasked(7,42,Steering,0);  
    }
  }
  Font.setCursor(13,4);
  Font.print(Name);
  Font.setCursor(109,50);
  if(Hours > 9){
    Font.print(Hours);
  }
  else{
    Font.print("0"+String(Hours));
  }
  Font.setCursor(119,50);
  if(Mins > 9){
    Font.print(Mins);
  }
  else{
    Font.print("0"+String(Mins));
  }

  String MilesString = String(Miles);
  String Output = "";
  if(MilesString.length() > 1){
    uint8_t Padding = 6 - (MilesString.length());
    
    if(Padding != 0){
      for(uint8_t i = 0; i < Padding; i++){
        Output += "0";
      }
    }
    
    Output += MilesString;
  } else {
    Output = "00000"+MilesString;
  }
  
  String MilesTotal = Output;
  char LastDigit = Output[Output.length()-1];
  
  Font.setCursor(66,50);
  Font.print(MilesTotal);
  Font.setTextColor(0);
  Font.printChar(LastDigit,87,50);
  Font.setTextColor(1);
}

void GameOverCheck(){
  if((PlayerX <= 4)||(PlayerX > 100)){
    gameState = GameState::End;
  }

  if(Miles >= 3600){
    Complete = true;
    gameState = GameState::End;
  }
}

void GameLoop(){
  RoadFrame++;
  UpdateX();
  UpdateTime();
  UpdateBush();
  GameOverCheck();
  if(gameState == GameState::Game){
    DrawRoad(PlayerX,RoadFrame);
    if(!NightTime)
      DrawBush(PlayerX);
    DrawCabin();
  }
}


void MainMenuLoop(){
  sprites.drawSelfMasked(0,0,Title,0);
  if(ard.justPressed(A_BUTTON)){
    gameState = GameState::PlayerNaming;
    
    Complete = false;
    Seconds = 0;
    Mins = random(0,60);
    Hours = random(0,25);
    Miles = 0;
    Name = "";
    PlayerX = 64;
    ObjX = 64;
    ObjY = 0;  
  }
  
  if(ard.justPressed(B_BUTTON)){
    Sound = !Sound;
    if(Sound){
      ard.audio.on();
    }else{
      ard.audio.off();
    }
  }

  if(Sound){
    sprites.drawSelfMasked(86,46,Speaker,0);
  } else {
    sprites.drawSelfMasked(86,46,Speaker,1);
  }
  
}




void PlayerNameLoop(){
  if(ard.justPressed(RIGHT_BUTTON)){
    if(CharIndex < 4)
      CharIndex++;
    else
    {
      Name = String(TempName);
      gameState = GameState::Game;
    }
  }
  
  if(ard.justPressed(LEFT_BUTTON)){
    if(CharIndex > 0)
      CharIndex--;
  }
  
  if(ard.justPressed(UP_BUTTON)){
    if(TempName[CharIndex] == ' ')
       TempName[CharIndex] = 'A';
    else{
      if(TempName[CharIndex] != 'Z')
        TempName[CharIndex]++;
      else
        TempName[CharIndex] = ' ';
    }
  }

  if(ard.justPressed(DOWN_BUTTON)){
    if(TempName[CharIndex] == ' ')
       TempName[CharIndex] = 'Z';
    else{
      if(TempName[CharIndex] != 'A')
        TempName[CharIndex]--;
      else
        TempName[CharIndex] = ' ';
    }
  }
  
  sprites.drawSelfMasked(0,0,TransportCard,0);
  Font.setCursor(51,15);
  Font.print(TempName);
  Font.setCursor(51 + (CharIndex * 4),20);
  Font.write('!');
  
  Font.setCursor(47,22);
  Font.print(0);
  
  Font.setCursor(43,29);
  if(Hours > 9){
    Font.print(Hours);
  }
  else{
    Font.print("0"+String(Hours));
  }
  Font.setCursor(53,29);
  if(Mins > 9){
    Font.print(Mins);
  }
  else{
    Font.print("0"+String(Mins));
  }

  if(Complete)
    sprites.drawSelfMasked(58,44,Tick,0);
}

void EndGameLoop(){
  sprites.drawSelfMasked(0,0,TransportCard,0);
  Font.setCursor(51,15);
  Font.print(Name);
  Font.setCursor(47,22);
  Font.print(Miles/10);
  
  Font.setCursor(43,29);
  if(Hours > 9){
    Font.print(Hours);
  }
  else{
    Font.print("0"+String(Hours));
  }
  Font.setCursor(53,29);
  if(Mins > 9){
    Font.print(Mins);
  }
  else{
    Font.print("0"+String(Mins));
  }

  if(Complete)
    sprites.drawSelfMasked(58,44,Tick,0);


  if(ard.pressed(A_BUTTON)){
    gameState = GameState::MainMenu;
  }   
}




