from __future__ import annotations

from typing import TYPE_CHECKING

from rule_builder.options import OptionFilter
from rule_builder.rules import Has, HasAll, Rule

from .options import HardMode

if TYPE_CHECKING:
    from .world import SM3DWWorld

def set_all_rules(world: SM3DWWorld) -> None:
  set_all_entrance_rules(world)
  set_all_location_rules(world)
  set_completion_condition(world)

def set_all_entrance_rules(world: SM3DWWorld) -> None:
  world_1_to_world_2 = world.get_enterance("World 1 to World 2")

def set_all_location_rules(world: SM3DWWorld) -> None:
  can_ride_plessy: Rule = Has("Plessy Unlock")

def set_completion_condition(world: SM3DWWorld) -> None:
  #comment :P
