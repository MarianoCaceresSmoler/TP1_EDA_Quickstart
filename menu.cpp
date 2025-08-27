/**
 * @brief Menu program source
 * @author Enzo Nicolas Rosa Fernandez
 * @author Mariano Caceres Smoler
 * @author Francisco Chiusaroli
 *
 */

#include "menu.h"

#include <chrono>
#include <stdio.h>

#define TYPE_COUNTER 3
#define TOGGLE_LIMIT 5

struct resource {
  Font Font_Gothic;
  Font Font_Golden;
  Font Font_Typerwriter;

  Sound Typewriter_forward[TYPE_COUNTER];
  Sound Typewriter_backward[TYPE_COUNTER];
};

static void intialize_resources(resource *Master_resource);
static void kill_resources(resource *Master_resource);
static long long timestamp_millis();
static void animation_intro(resource *Master_resource);

/**
 * @name menu
 *
 * @brief Master function for menu browsing. Applies animations, logic and resources.
 *
 * @param simVisualType: Pointer to enumerator which indicates how the models should look like.
 * @param simLocgicalType: Pointer to enumerator which indicates which mathematical model should be used.
 * @param view: Graphical interface.
 */

void menu(visual_sim_type_t *simVisualType, logical_sim_type_t *simLogicalType, View *view) {

  resource Master_resource;

  intialize_resources(&Master_resource);

  animation_intro(&Master_resource);

  kill_resources(&Master_resource);
}

/**
 * @name initialize_resources
 *
 * @brief Initializes every necessary (and unnecessary) resource for later on.
 *
 * @param Master_resource: Pointer to struct holding the resources' information.
 */

static void intialize_resources(resource *Master_resource) {

  Master_resource->Font_Gothic = LoadFontEx("./Fonts/Gothic_regular.ttf", 32, 0, 250);
  Master_resource->Font_Golden = LoadFontEx("./Fonts/Golden.otf", 64, 0, 250);
  Master_resource->Font_Typerwriter = LoadFontEx("./Fonts/TypeWriter.ttf", 64, 0, 250);

  for ( int i = 0; i < TYPE_COUNTER; i++ )
  {
    Master_resource->Typewriter_forward[i] = LoadSound("./Audio/Typewriter_forward.wav");
    Master_resource->Typewriter_backward[i] = LoadSound("./Audio/Typewriter_backward.wav");
  }
}

/**
 * @name kill_resources
 *
 * @brief Destroys every resource which has been previously loaded for cleanup.
 *
 * @param Master_resource: Pointer to struct holding the resources' information.
 */

static void kill_resources(resource *Master_resource) {

  UnloadFont(Master_resource->Font_Gothic);
  UnloadFont(Master_resource->Font_Golden);
  UnloadFont(Master_resource->Font_Typerwriter);

  for ( int i = 0; i < TYPE_COUNTER; i++ )
  {
    UnloadSound(Master_resource->Typewriter_forward[i]);
    UnloadSound(Master_resource->Typewriter_backward[i]);
  }
}

/**
 * @name timestamp_millis
 *
 * @brief Similar to millis from arduino.h, records a timestamp from the moment the program
 * has begun to the instant it is being called. Uses <chrono> as base.
 *
 * @return time in milliseconds since the moment the program started.
 */

static long long timestamp_millis() {
  auto now = std::chrono::steady_clock::now();
  auto ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now);
  return ms.time_since_epoch().count();
}

/**
 * @name animation_intro
 *
 * @brief Handles animations for the first seconds of the program launch
 *
 * @param Master_resource: Pointer to struct holding the resources' information.
 */

static void animation_intro(resource *Master_resource) {

  long long timer_1;
  long long timer_2;
  long long timer_3;

  short int buffer_length = 0;
  short int toggle_counter = 0;
  short int sound_counter = 0;
  char end_symbol = '|';

  char present_1[] = "Lionel Messi Studios present";
  char buffer[sizeof(present_1) + 1] = {0};

  bool finish = 0;

  timer_1 = timestamp_millis();
  timer_2 = timer_1;

  while ( finish == 0 )
  {

    BeginDrawing();

    ClearBackground(BLACK);

    timer_3 = timestamp_millis();

    if ( toggle_counter < TOGGLE_LIMIT && buffer_length < sizeof(present_1) && timer_3 - timer_1 >= 100 )
    {
      sprintf(buffer, "%.*s%c", buffer_length, present_1, end_symbol);
      timer_1 = timestamp_millis();
      buffer_length++;
      PlaySound(Master_resource->Typewriter_forward[sound_counter]);
      if ( sound_counter >= TYPE_COUNTER - 1 )
      {
        sound_counter = 0;
      }
      else
      {
        sound_counter++;
      }
    }
    else if ( toggle_counter < TOGGLE_LIMIT && buffer_length >= sizeof(present_1) && timer_3 - timer_1 >= 500 )
    {
      if ( end_symbol != 0 )
      {
        end_symbol = 0;
      }
      else
      {
        end_symbol = '|';
      }
      sprintf(buffer, "%s%c", present_1, end_symbol);
      timer_1 = timestamp_millis();
      toggle_counter++;
    }
    else if ( toggle_counter >= TOGGLE_LIMIT && buffer_length > 0 && timer_3 - timer_1 >= 100 )
    {
      buffer_length--;
      sprintf(buffer, "%.*s%c", buffer_length, present_1, end_symbol);
      timer_1 = timestamp_millis();

      PlaySound(Master_resource->Typewriter_backward[sound_counter]);

      if ( sound_counter >= TYPE_COUNTER - 1 )
      {
        sound_counter = 0;
      }
      else
      {
        sound_counter++;
      }

      if ( buffer_length <= 0 )
      {
        buffer[0] = 0;
      }
    }
    else if ( toggle_counter >= TOGGLE_LIMIT && !buffer_length && timer_3 - timer_1 >= 2000 )
    {
      finish = 1;
    }

    DrawTextEx(Master_resource->Font_Typerwriter, buffer, {GetScreenWidth() * 0.4f + 64, GetScreenHeight() * 0.7f}, 64, 0.0, WHITE);

    EndDrawing();

    if ( GetKeyPressed() != 0 ) finish = 1;
  }
}