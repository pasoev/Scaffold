#include "gamestate.h"

void pushState(struct GameStateMachine *fsm, struct GameState *state) {
    list_ins_next(fsm->gameStates, list_tail(fsm->gameStates), state);
    /* struct GameState *curState = (struct GameState*) list_data(list_tail(fsm->gameStates)); */
    state->onEnter(state);
}

void changeState(struct GameStateMachine *fsm, struct GameState *state) {
    if (list_size(fsm->gameStates) > 0) {
        struct GameState *curState = (struct GameState*) list_tail(fsm->gameStates)->data;
        if (curState->stateId == state->stateId) {
            return;
        }
    }

    popState(fsm);
    /* Push back the new state */
    pushState(fsm, state);

}

void popState(struct GameStateMachine *fsm) {
    if (list_size(fsm->gameStates) != 0) {
        /* Get the second to last element. */
        ListElmt *iter = list_head(fsm->gameStates);
        if (list_size(fsm->gameStates) > 1) {
            while (!list_istail(list_next(iter))) {
                iter = list_next(iter);
            }
        }

        struct GameState * curState;
        if (list_next(iter) == NULL) {
            curState = (struct GameState*) list_data(iter);
            if (curState->onExit()) {
                list_rem_next(fsm->gameStates, NULL, (void*) &curState);
            }
        } else {
            curState = (struct GameState*) list_data(list_next(iter));
            if (curState->onExit()) {
                list_rem_next(fsm->gameStates, iter, (void*) &curState);
            }
        }
    }
}

void popAllStates(struct GameStateMachine *fsm) {
    if (list_size(fsm->gameStates) != 0) {
        /* Get the second to last element. */
        ListElmt *iter = list_head(fsm->gameStates);
	struct GameState * curState;
	for(iter = list_head(fsm->gameStates); iter != NULL; iter = list_next(iter)){
	  curState = (struct GameState*) list_data(iter);
	  if (curState->onExit()) {
	    list_rem_next(fsm->gameStates, NULL, (void*) &curState);
	  }
	}
    }
}


