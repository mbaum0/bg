/**
 * @file events.h
 * @author Michael Baumgarten
 * @brief Contains custom event definitions
 */
#pragma once

#include <SDL.h>
#include <stdbool.h>
#include <stdint.h>

typedef struct EventManager EventManager;
typedef void (*EventCallback_fn)(uint32_t eventType, SDL_Event* e, void* data);
typedef struct EventCallback EventCallback;

struct EventCallback {
  uint32_t eventType;
  EventCallback_fn callback;
  void* data;
};

struct EventManager {
  EventCallback*** callbacks; // pointer to the callbacks array
};

/**
 * @brief Registers a callback function to be called when the specified event
 * occurs.
 *
 * @param em The event manager to register the callback with
 * @param event_type The type of event to register the callback for
 * @param callback_fn The callback function to be called when the event occurs
 * @param data The data to be passed to the callback function
 */
void EM_registerCallback(EventManager* em, uint32_t event_type, EventCallback_fn callback_fn, void* data);

/**
 * @brief Initialize the event manager
 */
EventManager* EM_init(void);

/**
 * @brief Free the event manager
 */
void EM_free(EventManager* em);