#include "bank-helper.hpp"
#include "gameplay.hpp"
#include "ggsound.hpp"
#include "global.hpp"
#include "score.hpp"
#include "soundtrack-ptr.hpp"
#include "title.hpp"
#include <mapper.h>
#include <nesdoug.h>
#include <neslib.h>
#include <peekpoke.h>

int main() {
  set_prg_bank(0);

  pal_bright(0);
  ppu_off();

  bank_bg(0);
  bank_spr(1);

  set_vram_buffer();

  Global global_state;

  global_state.game_state = Global::GameState::Title;

  banked_lambda(GGSound::BANK, []() {
    GGSound::init(GGSound::Region::NTSC, song_list, sfx_list, instrument_list,
                  GGSound::BANK);
  });

  while (true) {
    switch (global_state.game_state) {
    case Global::GameState::Title: {
      Title title(global_state);
      title.run();
      break;
    }
    case Global::GameState::Gameplay: {
      global_state.misses = 0;
      global_state.plates_cleared = 0;
      global_state.timer_frames = 0;
      global_state.timer_seconds = 180;

      u8 stage = 0;
      const u8 creatures_per_stage[] = {8,  8,  10, 10, 12, 12,
                                        12, 14, 16, 16, 16};
      const u8 imposters_per_stage[] = {1, 2, 1, 2, 1, 2, 3, 3, 3, 2, 3};
      const u8 fixed_features_per_stage[] = {4, 4, 3, 3, 2, 2, 2, 2, 2, 1, 1};

      static_assert(sizeof(creatures_per_stage) == sizeof(imposters_per_stage));
      static_assert(sizeof(creatures_per_stage) ==
                    sizeof(fixed_features_per_stage));

      while (global_state.misses < 3 && global_state.timer_seconds > 0) {
        Gameplay gameplay(global_state, creatures_per_stage[stage],
                          imposters_per_stage[stage],
                          fixed_features_per_stage[stage]);
        gameplay.run();
        if (stage < sizeof(creatures_per_stage) - 1) {
          stage++;
        } else {
          stage = 7;
        }
      }
      if (global_state.plates_cleared >
          global_state.plates_cleared_high_score) {
        global_state.plates_cleared_high_score = global_state.plates_cleared;
      }
      global_state.game_state = Global::GameState::Score;
    }
    case Global::GameState::Score: {
      Score score(global_state);
      score.run();
    } break;
    }
  }
}