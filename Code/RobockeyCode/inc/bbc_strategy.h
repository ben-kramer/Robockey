/* File: bbc_strategy.h
 * 
 * Authors: Ben Kramer <krab@seas.upenn.edu> Cameron Zawacki,
 *          Ben Bernstein
 *
 * Strategy controlling functions
 */

#include "m_general.h"
#include "bbc_definitions.h"
#include <bbc_localize.h>

void determine_strategy();

void drive_to_goal();

void drive_to_puck();

void return_to_goal();