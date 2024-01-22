/**
 * @file events.c
 * @author Michael Baumgarten
 * @brief Event definitions
 */

#include "events.h"
#include "stb_ds.h"

void EM_registerCallback(EventManager* em, uint32_t event_type, EventCallback_fn callback_fn, void* data) {
  EventCallback* cb = malloc(sizeof(EventCallback));
  cb->eventType = event_type;
  cb->callback = callback_fn;
  cb->data = data;
  arrput(*em->callbacks, cb);
}

EventManager* EM_init(void) {
  EventManager* em = malloc(sizeof(EventManager));
  em->callbacks = malloc(sizeof(EventCallback**));
  *em->callbacks = NULL;
  SDL_AddEventWatch(EM_handleEvent, em);
  return em;
}

void EM_free(EventManager* em) {
  arrfree(*em->callbacks);
  free(em->callbacks);
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
  EventCallback** callbacks = *em->callbacks;
  for (int32_t i = 0; i < arrlen(callbacks); i++) {
    EventCallback* cb = callbacks[i];
    if (cb->eventType == event->type) {
      cb->callback(cb->eventType, event, cb->data);
    }
  }
  return 1;
}
