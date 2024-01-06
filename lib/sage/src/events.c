/**
 * @file events.c
 * @author Michael Baumgarten
 * @brief Event definitions
 */

#include "events.h"

ARRAY_INIT(EventCB, EventCallback)

int EM_handleEvent(void* data, SDL_Event* event);

void EM_registerCallback(EventManager* em, uint32_t event_type, EventCallback_fn callback_fn, void* data) {
  EventCallback* cb = malloc(sizeof(EventCallback));
  cb->eventType = event_type;
  cb->callback = callback_fn;
  cb->data = data;
  EventCBArray_append(&em->callbacks, cb);
}

EventManager* EM_init(void) {
  EventManager* em = malloc(sizeof(EventManager));
  EventCBArray_init(&em->callbacks, 10);
  SDL_AddEventWatch(EM_handleEvent, em);
  return em;
}

void EM_free(EventManager* em) {
  EventCBArray_free(&em->callbacks);
  free(em);
}

/**
 * @brief This function is called whenever an event occurs. It will call all of
 * the registered callbacks for the event type.
 *
 * @param data event manager
 * @param event event that occured
 * @return int always 1 (this value is ignored)
 */
int EM_handleEvent(void* data, SDL_Event* event) {
  EventManager* em = (EventManager*)data;
  for (int32_t i = 0; i < em->callbacks.length; i++) {
    EventCallback* cb = em->callbacks.items[i];
    if (cb->eventType == event->type) {
      cb->callback(cb->eventType, event, cb->data);
    }
  }
  return 1;
}
