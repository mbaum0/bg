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
            void (*state_functions[NUM_STATES])(struct FiniteStateMachine*, EventQueue*, void*);
        } FiniteStateMachine;

        void fsm_init(FiniteStateMachine* fsm);
        void fsm_enqueue_event(EventQueue* queue, Event event);
        bool fsm_dequeue_event(EventQueue* queue, EventData* event);
        void fsm_step(FiniteStateMachine* fsm, EventQueue* event_queue, void* ctx);
        void fsm_transition(FiniteStateMachine *fsm, State next_state);
        """
    template = dedent(template)
    return Template(template).render({"context_name":context_name})

def gen_state_fxs(states, context_name):
    template = """
        {% for s in states -%}
        void {{ s.lower() }}_state_function(FiniteStateMachine* fsm, EventQueue* event_queue, void* ctx);
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

def main():
    with open("scheme.toml", "rb") as f:
        data = tomllib.load(f)
        events = []
        states = []
        context_name = ""
        for state, data in data.items():
            if state == "FSM":
                context_name = data["name"]
            if state != "FSM":
                states.append(state)
            if 'events' in data.keys():
                events.extend(data['events'])
        gen_fsm_header(events, states, context_name)


main()