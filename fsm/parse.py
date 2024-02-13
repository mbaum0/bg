import tomllib
from jinja2 import Template
from textwrap import dedent

def gen_event_defs(events):
    template = """
        typedef enum {
            {% for e in events -%}
            {% if not loop.last -%}
                {{ e }}_EVENT,
            {% else -%}
                {{ e }}_EVENT
            {%- endif %}
            {%- endfor %}
        } Event;

        typedef struct {
            Event event_type;
        } EventData;

        """
    template = dedent(template)
    return Template(template).render({"events":events})

def gen_state_defs(states):
    template = """
        typedef enum {
            {% for s in states -%}
                {{ s }}_STATE,
            {% endfor -%}
            NUM_STATES
        } State;
        """
    template = dedent(template)
    return Template(template).render({"states":states})

def gen_event_queue():
    eq =  """
        // Define event queue
        #define MAX_EVENTS 10
        typedef struct {
            EventData events[MAX_EVENTS];
            int front, rear;
        } EventQueue;
    """
    return dedent(eq)

def gen_fsm_struct(context_name):
    template = """
        typedef struct FiniteStateMachine {
            State current_state;
            EventQueue eventQueue;
            void (*state_functions[NUM_STATES])(struct FiniteStateMachine*, void*);
            void (*state_init_functions[NUM_STATES])(struct FiniteStateMachine*, void*);
        } FiniteStateMachine;

        void fsm_init(FiniteStateMachine* fsm);
        void fsm_enqueue_event(FiniteStateMachine* fsm, Event event);
        bool fsm_dequeue_event(FiniteStateMachine* fsm, EventData* event);
        void fsm_step(FiniteStateMachine* fsm, void* ctx);
        void fsm_transition(FiniteStateMachine *fsm, State next_state, void* ctx);

        typedef struct {} {{ context_name }};
        """
    template = dedent(template)
    return Template(template).render({"context_name":context_name})

def gen_state_fxs(states, context_name):
    template = """
        {% for s in states -%}
        void {{ s.lower() }}_state(FiniteStateMachine* fsm, void* ctx);
        void {{ s.lower() }}_init_state(FiniteStateMachine* fsm, void* ctx);
        {% endfor -%}
        """
    template = dedent(template)
    return Template(template).render({"states": states, "context_name":context_name})


def gen_fsm_header(events, states, context_name):
    result = "#pragma once\n"
    result += "#include <stdbool.h>\n"
    result += gen_event_defs(events)
    result += gen_state_defs(states)
    result += gen_event_queue()
    result += gen_fsm_struct(context_name)
    result += gen_state_fxs(states, context_name)
    with open("fsm.h", "w") as f:
        f.write(result)

def gen_state_file(state, events, context_name):
    result = '#include "fsm.h"\n'
    result += '#include <stdio.h>'

    template = """
        void {{ state.lower() }}_state(FiniteStateMachine* fsm, void* ctx) {
            {{ context_name }}* {{ context_name.lower() }} = ({{ context_name }}*)ctx;
            {%- if events | length > 0 %}
            EventData event;
            while (fsm_dequeue_event(fsm, &event)) {
                {% for e in events -%}
                if (event.event_type == {{ e }}_EVENT) {
                    // Transition to a different state here
                    // fsm_transition(fsm, NEW_STATE, ctx);
                }
                {% endfor %}
            }
            {% endif %}
        }
        void {{ state.lower() }}_init_state(FiniteStateMachine* fsm, void* ctx) {
            {{ context_name }}* {{ context_name.lower() }} = ({{ context_name }}*)ctx;
            // this is called when the {{ state.lower() }} state is first entered
        }
        """
    template = dedent(template)
    result += Template(template).render({"state": state, "events": events, "context_name":context_name})
    fname = state.lower() + ".c"
    with open(fname, "w") as f:
        f.write(result)

def gen_fsm_file(states, context_name):
    template = """
            #include "fsm.h"
            #include <stdlib.h>
            #include <stdio.h>

            void fsm_init(FiniteStateMachine *fsm) {
                {% for s in states %}
                fsm->state_functions[{{ s }}_STATE] = {{ s.lower() }}_state;
                fsm->state_init_functions[{{ s }}_STATE] = {{ s.lower() }}_init_state;
                {%- endfor %}
                fsm->current_state = {{ states[0] }}_STATE;
                fsm->eventQueue.front = 0;
                fsm->eventQueue.rear = 0;
            }

            void fsm_enqueue_event(FiniteStateMachine *fsm, Event event) {
                if ((fsm->eventQueue.rear + 1) % MAX_EVENTS == fsm->eventQueue.front) {
                    // event queue overflow
                    return;
                }
                fsm->eventQueue.events[fsm->eventQueue.rear].event_type = event;
                fsm->eventQueue.rear = (fsm->eventQueue.rear + 1) % MAX_EVENTS;
            }

            bool fsm_dequeue_event(FiniteStateMachine *fsm, EventData* event) {
                if (fsm->eventQueue.front == fsm->eventQueue.rear) {
                    return false; // Event queue is empty
                }
                *event = fsm->eventQueue.events[fsm->eventQueue.front];
                fsm->eventQueue.front = (fsm->eventQueue.front + 1) % MAX_EVENTS;
                return true;
            }

            void fsm_step(FiniteStateMachine* fsm, void* ctx) {
                fsm->state_functions[fsm->current_state](fsm, ctx);
            }

            void fsm_transition(FiniteStateMachine *fsm, State next_state, void* ctx) {
                // Here you can add any transition logic if needed
                // For now, we'll simply update the current state
                // based on the next state provided
                // You may want to add additional logic here
                // such as handling state entry/exit actions
                fsm->state_init_functions[next_state](fsm, ctx);
                fsm->current_state = next_state;
            }
        """
    template = dedent(template)
    result = Template(template).render({"states": states, "context_name":context_name})
    with open("fsm.c", "w") as f:
        f.write(result)


def main():
    with open("scheme.toml", "rb") as f:
        data = tomllib.load(f)
        events = set()
        states = []
        context_name = ""
        for state, data in data.items():
            if state == "FSM":
                context_name = data["name"]
            if state != "FSM":
                states.append(state)
                s_events = data['events']
                events.update(s_events)
                gen_state_file(state, s_events, context_name)
        gen_fsm_header(events, states, context_name)
        gen_fsm_file(states, context_name)

main()